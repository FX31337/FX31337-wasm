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
//#define __debug_verbose__

// Local includes.
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
  static void Add(Ref<IndicatorData> _indi) { Platform::Add(_indi.Ptr()); }

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
      .class_function("Add", &Tester::Add)
      .class_function("AddPlatformWise", &Tester::AddPlatformWise)
      .class_function("RunAllTicks", &Tester::RunAllTicks)
      .class_function("RunTick", &Tester::RunTick);
}

REGISTER_ARRAY_OF(ArrayIndicatorData, Ref<IndicatorData>, "IndicatorDataArray");

#endif

int main(int argc, char **argv) {
  // Tester::Init();

  Print("Hello World from Indicator test!");
  std::cout << "Hello from C++!" << std::endl;
  return 0;
}
