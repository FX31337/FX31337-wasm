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
#define __debug_verbose__

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
 * Parameters to pass into lib.Tester.GetValues(). Could be calculated by
 * Tester::GetTimeByScrollAndZoom(float scroll, float zoom, int visible_intervals).
 */
struct TesterValuesFetchParams {
  // Beginning range (inclusive) of the values to be returned.
  int64 timeFromMs;

  // Ending range (inclusive) of the values to be returned.
  int64 timeToMs;

  // Width of the single column in structure retrieved from Tester::GetValues(). If more that one value fits the column,
  // values will be aggregated (min/max values will be generated).
  int timeStepSecs;

  /**
   * Returns string representation of the structure.
   */
  string ToString() {
    return "{ timeFromMs: " + IntegerToString(timeFromMs) + ", timeToMs: " + IntegerToString(timeToMs) +
           ", timeStepSecs: " + IntegerToString(timeStepSecs) + " }";
  }
};

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
  // Names of the indicator value.
  string name;

  // Type of the indicator value.
  ENUM_DATATYPE type;

  // Time of the first added value.
  int64 time_open_ms;

  // Time of the last added value.
  int64 time_close_ms;

  // AGGREGATED VALUES:

  // Value of the first added value.
  double value_open;

  // Highest added value.
  double value_high;

  // Lowest added value.
  double value_low;

  // Value of the last added value.
  double value_close;

  // Average of added values.
  double value_avg;

  // Number of values added (volume of the column).
  int num_values;

  /**
   * Constructor.
   **/
  TesterValuesColumnValue(string _name = "", ENUM_DATATYPE _type = TYPE_DOUBLE, int64 _time_open_ms = 0,
                          int64 _time_close_ms = 0, double _open = 0, double _high = 0, double _low = 0,
                          double _close = 0, double _avg = 0, int _num_values = 0) {
    name = _name;
    type = _type;
    time_open_ms = _time_open_ms;
    time_close_ms = _time_close_ms;
    value_open = _open;
    value_high = _high;
    value_low = _low;
    value_close = _close;
    value_avg = _avg;
    num_values = _num_values;
  }

  /**
   * Adds value to the column.
   **/
  void Add(double _value, int64 _time_ms) {
    if (num_values == 0) {
      // Adding first value to the column.
      time_open_ms = time_close_ms = _time_ms;
      value_open = value_high = value_low = value_close = value_avg = _value;
    } else {
      // Adding another value to the column.
      if (_time_ms < time_open_ms) {
        time_open_ms = _time_ms;
        value_open = _value;
      }
      if (_time_ms > time_close_ms) {
        time_close_ms = _time_ms;
        value_close = _value;
      }
      value_high = MathMax(value_high, _value);
      value_low = MathMax(value_low, _value);
      value_avg = ((value_avg * num_values) + _value) / (num_values + 1);
    }

    ++num_values;
  }

  /**
   * Returns string representation of the structure.
   */
  string ToString(int _indent = 0) {
    string _out, _padding;
    StringInit(_padding, _indent, ' ');
    _out += _padding + "{  name: \"" + name + "\", type: " + EnumToString(type) +
            ", open: " + DoubleToString(value_open) + ", high: " + DoubleToString(value_high) +
            ", low: " + DoubleToString(value_low) + ", close: " + DoubleToString(value_close) +
            ", avg: " + DoubleToString(value_avg) + " }";
    return _out;
  }
};

/**
 * Chart column data. A part of TesterValues items array.
 */
struct TesterValuesColumn {
  // Time in ms of data in the the column.
  int64 time_ms;

  // List of values for each indicator.
  ARRAY(TesterValuesColumnValue, values);

  /**
   * Constructor.
   **/
  TesterValuesColumn(int64 _time_ms = 0) : time_ms(_time_ms) {}

  /**
   * Returns string representation of the structure.
   */
  string ToString(int _indent = 0) {
    int i;
    string _out, _padding_outer, _padding_inner;
    StringInit(_padding_outer, _indent, ' ');
    StringInit(_padding_inner, _indent + 2, ' ');

    _out += _padding_outer + "{\n";
    _out += _padding_inner + "time_ms: " + IntegerToString(time_ms) + ",\n";
    _out += _padding_inner + "values: [\n";

    for (i = 0; i < ArraySize(values); ++i) {
      _out += values[i].ToString(_indent + 4) + "\n";
    }

    _out += _padding_inner + "]\n";
    _out += _padding_outer + "}";
    return _out;
  }
};

/**
 * Structure returned by Tester::GetValues();
 */
struct TesterValues {
  // Values that fit passed timeStep.
  ARRAY(TesterValuesColumn, timestep_based);

  // Values that don't fit passed timeStep.
  ARRAY(TesterValuesColumn, loose);

  /**
   * Returns string representation of the structure.
   */
  string ToString(int _indent = 0) {
    int i;
    string _out, _padding_outer, _padding_inner;
    StringInit(_padding_outer, _indent, ' ');
    StringInit(_padding_inner, _indent + 2, ' ');

    _out += _padding_outer + "{\n";
    _out += _padding_inner + "timestep_based: [\n";

    for (i = 0; i < ArraySize(timestep_based); ++i) {
      _out += timestep_based[i].ToString(_indent + 4) + "\n";
    }

    _out += _padding_inner + "],\n";
    _out += _padding_inner + "loose: [\n";

    for (i = 0; i < ArraySize(loose); ++i) {
      _out += loose[i].ToString(_indent + 4) + "\n";
    }

    _out += _padding_inner + "]\n";
    _out += _padding_outer + "}";
    return _out;
  }
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
   * number of minutes to retrieve. Effective absolute index is: GetIndexByTimeMs(MinuteMs / zoom * scroll).
   */
  static TesterValuesFetchParams GetTimeByScrollAndZoom(float scroll, float zoom, int visible_intervals) {
    // Base interval for zoom 1 is 1 minute. Zoom 2 means 60s / 2, i.e., 30s.
    // Essentialy the interval is: 1 minute / zoom.
    TesterValuesFetchParams result;
    result.timeFromMs = 60000.0 * scroll * zoom;
    result.timeToMs = 60000.0 * (scroll * visible_intervals) * zoom;
    result.timeStepSecs = (int)(60.0f / zoom);
    return result;
  }

  /**
   * @see Tester::GetValues(int64 _timeFromMs...) below.
   **/
  static TesterValues GetValues(const TesterValuesFetchParams &params, bool _aggregate_no_fits = true) {
    return GetValues(params.timeFromMs, params.timeToMs, params.timeStepSecs, _aggregate_no_fits);
  }

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
  static TesterValues GetValues(int64 _time_from_ms, int64 _time_to_ms, int _time_step_secs,
                                bool _aggregate_no_fits = true) {
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

    // Firstly, we need to know which indicators are TF-based and which aren't
    // TF-based. Non TF-based indicators generates values whenever they want,
    // so values from their shifts aren't generated in fixed intervals.
    //
    // All non TF-based indicators have INDI_FLAG_LOOSE_TF_CANDLE_INDICATOR
    // flag set. Such flag have e.g., Renko indicator.

    // Now we traverse indicators added to the Platform class via
    // Tester::Add(...) or via Platform::Add...().

    // First step is to calculate how much of those two types of indicators we
    // have.

    int _num_tf = 0, _num_loose_tf = 0;

    for (DictStructIterator<long, Ref<IndicatorData>> _iter = Platform::GetIndicators() PTR_DEREF Begin();
         _iter.IsValid(); ++_iter) {
      IndicatorData *_indi = _iter.Value().Ptr();
      if (_indi PTR_DEREF IsCandleIndicator() || _iter.Value() REF_DEREF IsTickIndicator()) {
        // We don't need values of candle or tick indicators.
        continue;
      }

      bool _is_loose_tf = (_indi PTR_DEREF GetFlags() & INDI_FLAG_LOOSE_TF_CANDLE_INDICATOR) != 0;

      if (_is_loose_tf) {
        ++_num_loose_tf;
      } else {
        ++_num_tf;
      }
    }

    ArrayResize(values.timestep_based, _num_tf);
    ArrayResize(values.loose, _num_loose_tf);

    // Next step is to generate values for each type of indicators.

    for (DictStructIterator<long, Ref<IndicatorData>> _iter = Platform::GetIndicators() PTR_DEREF Begin();
         _iter.IsValid(); ++_iter) {
      IndicatorData *_indi = _iter.Value().Ptr();
      if (_indi PTR_DEREF IsCandleIndicator() || _iter.Value() REF_DEREF IsTickIndicator()) {
        // We don't need values of candle or tick indicators.
        continue;
      }

      bool _is_loose_tf = (_indi PTR_DEREF GetFlags() & INDI_FLAG_LOOSE_TF_CANDLE_INDICATOR) != 0;

      if (!_is_loose_tf) {
        // It's a TF-based indicator (fixed interval).
        // Such indicators are based on the assigned TF indicator and TF value
        // will be retrieved from those TF indicators.
        ENUM_TIMEFRAMES _tf = _indi PTR_DEREF GetTf();

        // Time-frame length in miliseconds.
        int64 _tf_ms = ((int64)ChartTf::TfToMs(_tf));

        // Firstly, we round time range to the TF.
        _time_from_ms = _time_from_ms % _tf_ms;

        // We don't want to end at the start of the next time-frame, but just before it.
        _time_to_ms = _time_to_ms % _tf_ms + (_tf_ms - 1);

        // In order to get relative indices of values to retrieve, we calculate difference of current value's time
        // (relative index 0) and rounded start time.
        int64 _value_rel_0_time_ms = _indi PTR_DEREF GetBarTime(0);

        // Note that starting index could be negative. That means that we want to retrieve future values.
        int _value_index_from = (int)((_value_rel_0_time_ms - _time_from_ms) / _tf_ms);

        // Note that ending index could be negative. That means that we want to retrieve future values.
        int _value_index_to = (int)((_value_rel_0_time_ms - _time_to_ms) / _tf_ms);

#ifdef __debug_verbose__
        Print("Tester::GetValues(): Will aggregate values for TF ", EnumToString(_tf), ", relative indices from ",
              _value_index_from, " to ", _value_index_to);
#endif

        if (_value_index_from < 0 || _value_index_to < 0) {
          Alert("We don't yet support retrieving future values!");
          DebugBreak();
          return values;
        }

        // Now it's time to calculate

      } else {
        // It's a loose, i.e., non TF-based indicator (values are generated in
        // a non-fixed interval and can we generated anytime). We just traverse
        // values and try to retrieve values for a given time range.
      }
    }

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

    int64 _dt = Platform::Timestamp();

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
      .class_function("GetTimeByScrollAndZoom", &Tester::GetTimeByScrollAndZoom)
      .class_function(
          "GetValues", &Tester::GetValues,
          emscripten::optional_override([](int64 timeFromMs, int64 timeToMs, int timeStepSecs, bool aggregateNoFits) {
            return Tester::GetValues(timeFromMs, timeToMs, timeStepSecs, aggregateNoFits);
          }));
}

// struct IndicatorData[]
REGISTER_ARRAY_OF(ArrayIndicatorData, Ref<IndicatorData>, "IndicatorDataArray");

// struct TesterValuesColumn[]
REGISTER_ARRAY_OF(ArrayTesterValuesColumn, TesterValuesColumn, "TesterValuesColumnArray");

// struct TesterValuesFetchParams
EMSCRIPTEN_BINDINGS(TesterValuesFetchParams) {
  emscripten::value_object<TesterValuesFetchParams>("TesterValuesFetchParams")
      .field("timeFromMs", &TesterValuesFetchParams::timeFromMs)
      .field("timeToMs", &TesterValuesFetchParams::timeFromMs)
      .field("timeStepSecs", &TesterValuesFetchParams::timeStepSecs);
}

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

  TesterValuesFetchParams getValuesParams = Tester::GetTimeByScrollAndZoom(0, 0, 32);

  TesterValues values = Tester::GetValues(getValuesParams);

  ArrayResize(values.timestep_based, 1);
  values.timestep_based[0].time_ms = 5;
  ArrayResize(values.timestep_based[0].values, 1);
  values.timestep_based[0].values[0] = TesterValuesColumnValue("RSI (20)", TYPE_DOUBLE);
  values.timestep_based[0].values[0].Add(0.224, 0);
  values.timestep_based[0].values[0].Add(0.22, 1000);
  values.timestep_based[0].values[0].Add(0.24, 2000);
  values.timestep_based[0].values[0].Add(0.2254, 3000);
  values.timestep_based[0].values[0].Add(0.2284, 4000);
  values.timestep_based[0].values[0].Add(0.2214, 5000);
  values.timestep_based[0].values[0].Add(0.2114, 6000);

  ArrayResize(values.loose, 1);
  ArrayResize(values.loose[0].values, 1);
  values.loose[0].values[0] = TesterValuesColumnValue("RENKO (5)", TYPE_DOUBLE, 25, 28.21, 25, 26.3, 23, 24.4);

  Print("Fetched values for params: ", getValuesParams.ToString());

  Print("Values fetched:\n", values.ToString());

#ifdef __cplusplus
  std::system("PAUSE");
#endif

#endif

  return 0;
}
