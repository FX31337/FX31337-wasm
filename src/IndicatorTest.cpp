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
#include "classes/Indicator/Indicator.h"
#include "classes/Indicator/tests/classes/IndicatorTfDummy.h"
#include "classes/Indicator/tests/classes/Indicators.h"
#include "classes/Indicators/Indi_RSI.mqh"
#include "classes/Indicators/Tick/Indi_TickProvider.h"
#include "classes/Platform.h"
#include "classes/Tick/Tick.struct.h"

#define INDICATOR_TEST_SYMBOL "EURUSD"
#define INDICATOR_TEST_TIMEFRAME PERIOD_M1

class IndicatorTest {
  static Indicators indis;

 public:
  IndicatorTest() {}

  static void Add(IndicatorData *_indi, string _symbol, ENUM_TIMEFRAMES _tf) {
    indis.Add(_indi);

    Platform::AddWithDefaultBindings(_indi, _symbol, _tf);

    Indi_TickProvider *_tick_provider = dynamic_cast<Indi_TickProvider *>(Platform::FetchDefaultTickIndicator(_symbol));

    if (_tick_provider != nullptr && _tick_provider PTR_DEREF BufferSize() == 0) {
      FeedTickProvider(_tick_provider, _symbol);
    }
  }

  static void FeedTickProvider(Indi_TickProvider *_tick_provider, string _symbol) {
    // We'd like to have reproducible ticks.
    srand(230);

    PrintFormat("Feeding Tick Provider for symbol %s...", C_STR(_symbol));
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

    _tick_provider PTR_DEREF Feed(_ticks);
  }

  static void Init() {
    Platform::Init();

    indis.Clear();

    // Relative Strength Index (RSI).
    IndiRSIParams rsi_params(10, PRICE_OPEN);
    Ref<IndicatorData> indi_rsi = new Indi_RSI(rsi_params, IDATA_INDICATOR);
    Add(indi_rsi.Ptr(), INDICATOR_TEST_SYMBOL, INDICATOR_TEST_TIMEFRAME);
  }

  static void Run() {
    while (Tick()) {
      // Ticking all ticks.
    }
  }

  static bool Tick() {
    Platform::Tick();

    if (!Platform::HadTick()) {
      Print("There are no new ticks to process.");
      return false;
    }

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
};

Indicators IndicatorTest::indis;

#ifdef EMSCRIPTEN
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(IndicatorTest) {
  emscripten::class_<IndicatorTest>("IndicatorTest")
      .constructor<>()
      .class_function("Init", &IndicatorTest::Init)
      .class_function("Tick", &IndicatorTest::Tick)
      .class_function("Run", &IndicatorTest::Run);
}
#endif

int main(int argc, char **argv) {
  Platform::Init();

#ifndef EMSCRIPTEN

  IndicatorTest::Init();
  IndicatorTest::Run();

#endif

  Print("Hello World from Indicator test!");
  std::cout << "Hello from C++!" << std::endl;
  return 0;
}
