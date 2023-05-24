//+------------------------------------------------------------------+
//|                                                     FX31337 wasm |
//|                                 Copyright 2022-2022, EA31337 Ltd |
//|                          https://github.com/FX31337/FX31337-wasm |
//+------------------------------------------------------------------+

/*
 * This file is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 * Task runner.
 */

// Includes
#ifdef EMSCRIPTEN
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>
#endif
#define __debug__
#define __debug_indicator__
#define __debug_emscripten__
//#define __debug_verbose__

// Local includes.
#include "classes/Array.extern.h"
#include "classes/Chart.enum.h"
#include "classes/DictStruct.mqh"
#include "classes/Indicator/Indicator.h"
#include "classes/Indicator/tests/classes/IndicatorTfDummy.h"
#include "classes/Indicator/tests/classes/Indicators.h"
#include "classes/Indicators/Indi_RSI.mqh"
#include "classes/Indicators/Tick/Indi_TickProvider.h"
#include "classes/Platform.h"
#include "classes/Tick/Tick.struct.h"

#define INDICATOR_TEST_SYMBOL "EURUSD"
#define INDICATOR_TEST_TIMEFRAME PERIOD_M1

/*

 Example in JS:

 const tester = new lib.Tester('EURUSD', lib.timeframes.M5);
 const ticks  = new lib.indicators.TickProvider();
 const rsi    = new lib.indicators.RSI(13);

 tester.Add(rsi);

 // Note that all timeframes shares the same ticks and so you may reuse single
 // TickProvider indicator for other Tester instances for the same symbols pair.
 ticks.Add([
   {timestamp: ..., ask: ..., bid: ...},
   {timestamp: ..., ask: ..., bid: ...}
 ]);

 // You can also use tester.RunTick() method in a loop or if you're sure that
 // new tick arrived.
 tester.RunAllTicks();

 for (let indicator of tester.GetIndicatorsInfo()) {
   console.log(`Indicator ${indicator.name}'s has completed. `)
 }

*/

/**
 * Details about given indicator. Retrieved from Tester class.
 *
 * We can later user Tester::GetIndicatorData(int indi_index, int abs_shift, int mode = 0) to retrieve data from the
 * indicator.
 */
struct TestIndicatorInfo {
  // Name of the indicator.
  string name;

  // Index of the indicator in the Platform.
  int index;

  // Number of values calculated for this indicator.
  int num_values;

  // Symbol pair indicator works on.
  string symbol;

  // Time-frame indicator works on.
  ENUM_TIMEFRAMES tf;
};

/**
 * Single value for the TesterValuesColumn.
 */
struct TesterValuesColumnValue {
  // Indicator value.
  double value;

  // Names of the indicator value.
  string name;

  // Type of the indicator value.
  ENUM_DATATYPE type;
};

/**
 * Chart column data. A part of TesterValues items array;
 */
struct TesterValuesColumn {
  // Time in ms of data in the the column.
  int64 time_ms;

  ARRAY(TesterValuesColumnValue, values);
};

/**
 * Structure returned by Tester::GetValues();
 */
struct TesterValues {
  // Values that fit passed timeStep.
  ARRAY(TesterValuesColumn, timestep_based);

  // Values that don't fit passed timeStep.
  ARRAY(TesterValuesColumn, loose);
};

class Tester {
 public:
  /**
   * Constructor.
   */
  Tester() {}

  /**
   * Adds indicator which must previously set its candle/tick sources in order
   * to work.
   */
  static void Add(IndicatorData *_indi) { Platform::Add(_indi); }

  /**
   * Adds indicator using given symbol and timeframe. Uses default Tf and Tick
   * indicator for current platform. Under C++/Emscripten default Tick
   * indicator is TickProvider. Note that you must feed TickProvider for each
   * symbol used.
   *
   * In JS you may retrieve default tick indicator via:
   * lib.Tester.GetDefaultTickIndicator(symbol: string).
   * The same with default candle indicator:
   * lib.Tester.GetDefaultCandleIndicator(symbol: string, tf: lib.timeframes[tf]).
   */
  static void AddPlatformWise(Ref<IndicatorData> _indi, string _symbol, ENUM_TIMEFRAMES _tf) {
    Platform::AddWithDefaultBindings(_indi.Ptr(), _symbol, _tf);

    Indi_TickProvider *_tick_provider = dynamic_cast<Indi_TickProvider *>(Platform::FetchDefaultTickIndicator(_symbol));

    // Feeding with random ticks only if tick provider is empty.
    // @todo Should be moved to Tester::FeedTickProvidersWithRandomTicks() or somewhere else.
    if (_tick_provider != nullptr && _tick_provider PTR_DEREF BufferSize() == 0) {
      FeedTickProvider(_tick_provider);
    }
  }

  /**
   * Calculates "timeFromMs", "timeToMs" and "timeStepSecs" parameters to directly pass them into
   * lib.Tester.GetValues(). Scroll 0.0f = Time for a value of absolute index 0. When Zoom is 1, Scroll determines
   * number of minutes to retrieve. Efective absolute index is: GetIndexByTimeMs(MinuteMs / zoom * scroll).
   */
  /*/
  static TesterValuesFetchParams GetTimeByScrollAndZoom(float scroll, float zoom) {
  }
  */

  /**
   * Retrieves chunk of values that fits and don't fit given timeStep, but are
   * between given timeFrom and timeTo.
   *
   * Values chunk contains indicator values aggregated by given number of
   * seconds. When you zoom out the chart, you increase that number of seconds.
   * Each item in the chunk will represent a time-frame being multiplication of
   * the given number of seconds. timeFrom and timeTo parameters is a
   * time-range in ms (BigInt type).
   */
  static TesterValues GetValues(datetime _timeFromMs, datetime _timeToMs, int _timeStepSecs,
                                bool _aggregateNoFits = true) {
    /*
      Pseudo-code:

      0   60  120 <- Fits to time-step
      |   |   |
       | |   |
       2 50  90 <- Doesn't fit to time-step

      We supports getting values on history and live.

      Getting values that fits time-step is easy. We check time of the last entry and calculate indicies to retrieve
      from given time. Next step is aggregating those values into given time-step. When we have more that one value in
      the same aggregated time, we generate two entries which determines minimum and maximum values.

      Values that don't fit given time-step are those generated by non-TF indicators, e.g., Renko. We may choose to
      aggregate values, so chart will be more clear when zoomed out. Same as fit time-step fit values, when aggregating
      we generate min/max values.

      Seeking over non-TF values must be optimized. We don't want to go over all the values and check their time. To do
      that, we modify IndicatorData and store cache containing times for each N-th value. E.g., we store time for value
      0, 100, 200, 300 and so on. Also, we add new method to IndicatorData, GetValueIndicesRangeAbs(time_from_ms,
      time_to_ms, inclusive_from, inclusive_to) which returns absolute start and end indices of values for the given
      time range. It will use values' time cache. The idea of the cache is to add time when every N-th value is
      generated.

    */
    TesterValues values;
    return values;
  }

  /**
   * Helper function for feeding given tick provider with some random ticks.
   **/
  static void FeedTickProvider(Ref<Indi_TickProvider> _tick_provider) {
    // We'd like to have reproducible ticks.
    srand(230);

    Print("Feeding Tick Provider with random values...");
    ARRAY(TickTAB<double>, _ticks);

    datetime _dt = Platform::Timestamp();

    double _curr_ask = 1.0;
    double _curr_bid = 1.2;

    for (int i = 0; i < 100; ++i) {
      if (i % 10 == 0) srand(i);

      double _bit = 0.1;
      double _sign = ((rand() % RAND_MAX) < RAND_MAX / 2) ? -1 : 1;

      _curr_ask += _bit * _sign;
      _curr_bid += _bit * _sign;
      // One tick per 20s. 3 ticks in a minute.
      ArrayPush(_ticks, TickTAB<double>((_dt + i) * 1000 * 20, _curr_ask, _curr_bid));
    }

    _tick_provider REF_DEREF Feed(_ticks);

    Print("Given Tick Provider has now ", _tick_provider REF_DEREF BufferSize(), " random values.");
  }

  static void Init() {
    Platform::Init();

    // Relative Strength Index (RSI).
    // IndiRSIParams rsi_params(10, PRICE_OPEN);
    // Ref<IndicatorData> indi_rsi = new Indi_RSI(rsi_params, IDATA_INDICATOR);
    // AddPlatformWise(indi_rsi.Ptr(), INDICATOR_TEST_SYMBOL, INDICATOR_TEST_TIMEFRAME);
  }

  /**
   * Runs all ticks. Stops thread until all ticks are processed.
   */
  static void RunAllTicks() {
    while (RunTick()) {
      // Ticking all ticks.
    }
  }

  /**
   * Runs a single tick. Returns false if there's no more ticks to process (all
   * tick indicators said there will be no more ticks).
   */
  static bool RunTick() {
    Platform::Tick();

    for (DictStructIterator<long, Ref<IndicatorData>> _iter = Platform::GetIndicators() PTR_DEREF Begin();
         _iter.IsValid(); ++_iter) {
      if (_iter.Value() REF_DEREF IsCandleIndicator() || _iter.Value() REF_DEREF IsTickIndicator()) {
        // We don't need values of candle or tick indicators.
        continue;
      }

      // Forcing indicator to calculate its value for the current tick.
      IndicatorDataEntry _entry = _iter.Value() REF_DEREF GetEntry();

      if (_entry.IsValid()) {
        Print(_iter.Value() REF_DEREF GetFullName(), "'s value: ", _entry.ToCSV<double>());
      } else {
        Print(_iter.Value() REF_DEREF GetFullName(), " requires more ticks.");
      }
    }

    if (!Platform::HadTick()) {
      Print("There are no new ticks to process.");
      return false;
    }

    return true;
  }

  /**
   * Returns list of indicators added for testing.
   */
  static ARRAY_TYPE(Ref<IndicatorData>) GetIndicators() {
    ARRAY(Ref<IndicatorData>, _indis);

    for (DictStructIterator<long, Ref<IndicatorData>> iter = Platform::GetIndicators() PTR_DEREF Begin();
         iter.IsValid(); ++iter) {
      ArrayPush(_indis, iter.Value());
    }

    return _indis;
  }

  /*
    IndicatorData *_candles = Platform::FetchDefaultCandleIndicator(INDICATOR_TEST_SYMBOL, INDICATOR_TEST_TIMEFRAME);

    Print("Tick processed. Current OHLC = ", C_STR(_candles PTR_DEREF GetOHLC().ToCSV()));

    if (_candles PTR_DEREF IsNewBar()) {
      Print("");
      Print("[ IT WAS NEW BAR ]");
      Print("");

      for (int i = 0; i < indis.Size(); ++i) {
        IndicatorData *_indi = indis[i];

        IndicatorDataEntry _entry(_indi PTR_DEREF GetEntry());

        if (_indi PTR_DEREF Get<bool>(STRUCT_ENUM(IndicatorState, INDICATOR_STATE_PROP_IS_READY))) {
          if (_entry.IsValid()) {
            PrintFormat("%s: bar %d: %s", C_STR(_indi PTR_DEREF GetFullName()), _candles PTR_DEREF GetBars(),
                        C_STR(_indi PTR_DEREF ToString()));
          }
        }
      }
    }

    return true;
  }

  */
};

#ifdef EMSCRIPTEN
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(Tester) {
  emscripten::class_<Tester>("Tester")
      .constructor<>()
      .class_function("Init", &Tester::Init)
      .class_function("Add", &Tester::Add, emscripten::allow_raw_pointer<emscripten::arg<0>>())
      .class_function("AddPlatformWise", &Tester::AddPlatformWise)
      .class_function("RunAllTicks", &Tester::RunAllTicks)
      .class_function("RunTick", &Tester::RunTick)
      .class_function("FeedTickProvider", &Tester::FeedTickProvider,
                      emscripten::allow_raw_pointer<emscripten::arg<0>>())
      //.class_function("GetTimeByScrollAndZoom", &Tester::GetTimeByScrollAndZoom)
      /*
      .class_function(
          "GetValues", &Tester::GetValues,
          emscripten::optional_override([](int64 timeFromMs, int64 timeToMs, int timeStepSecs, bool aggregateNoFits) {
            return Tester::GetValues(timeFromMs, timeToMs, timeStepSecs, aggregateNoFits);
          }))
      */
      ;
}

REGISTER_ARRAY_OF(ArrayIndicatorData, Ref<IndicatorData>, "IndicatorDataArray");

#endif

int main(int argc, char **argv) {
#ifdef __debug_emscripten__

  Indi_TickProviderParams ticker_params;
  ticker_params.symbol = "EURUSD";
  Ref<Indi_TickProvider> ticker = new Indi_TickProvider(ticker_params);

  Ref<IndicatorTfDummy> tfM5 = new IndicatorTfDummy(PERIOD_M1);
  tfM5 REF_DEREF SetDataSource(ticker.Ptr());

  IndiRSIParams rsiM5_params(13, PRICE_OPEN, 0);
  Ref<Indi_RSI> rsiM5 = new Indi_RSI(rsiM5_params);
  rsiM5 REF_DEREF SetDataSource(tfM5.Ptr());

  Tester::Add(rsiM5.Ptr());

  // Note that all timeframes shares the same ticks and so you may reuse single TickProvider indicator for other Tester
  // instances for the same symbols pair.

  ARRAY(TickTAB<double>, ticks);

  ArrayPush(ticks, TickTAB<double>(1000 * 1, 0.1, 0.11));
  ArrayPush(ticks, TickTAB<double>(1000 * 60, 0.2, 0.22));
  ArrayPush(ticks, TickTAB<double>(1000 * 120, 0.3, 0.33));

  ticker REF_DEREF Feed(ticks);

  Tester::RunAllTicks();

#endif

  return 0;
}
