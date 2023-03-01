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
#define __debug_verbose__

// Local includes.
#include "classes/Indicator/Indicator.h"
#include "classes/Indicator/tests/classes/IndicatorTfDummy.h"
#include "classes/Indicator/tests/classes/Indicators.h"
#include "classes/Indicators/Indi_RSI.mqh"
#include "classes/Indicators/Tick/Indi_TickProvider.h"
#include "classes/Platform.h"
#include "classes/Tick/Tick.struct.h"

class IndicatorTest {
  static Indicators indis;

 public:
  IndicatorTest() {}

  static void Add(IndicatorData *_indi) {
    indis.Add(_indi);

    Platform::AddWithDefaultBindings(_indi, _Symbol, PERIOD_CURRENT);
  }

  static void Init() {
    indis.Clear();

    // Relative Strength Index (RSI).
    IndiRSIParams rsi_params(14, PRICE_OPEN);
    Ref<IndicatorData> indi_rsi = new Indi_RSI(rsi_params);
    Add(indi_rsi.Ptr());

    for (int i = 0; i < indis.Size(); ++i) {
      Platform::AddWithDefaultBindings(indis[i], _Symbol, PERIOD_CURRENT);
    }
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

    Print("Tick processed.");

    IndicatorData *_candles = Platform::FetchDefaultCandleIndicator(_Symbol, PERIOD_CURRENT);

    if (_candles PTR_DEREF IsNewBar()) {
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
      .class_function("Run", &IndicatorTest::Run);
}
#endif

int main(int argc, char **argv) {
  Platform::Init();

  Print("Hello World from Indicator test!");
  std::cout << "Hello from C++!" << std::endl;
  return 0;
}
