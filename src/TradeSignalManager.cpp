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
 * Trade.
 */

// Includes.
#include "classes/Trade/TradeSignalManager.h"

#include "classes/Common.define.h"
#include "classes/Common.extern.h"
#include "classes/Std.h"
#include "classes/String.extern.h"

// Declare and define the external functions and variables.
#ifdef __cplusplus
extern "C" {
#endif
int sum(int a, int b) { return a + b; }
bool test() {
  printf("%s\n", __builtin_FUNCTION());
  TradeSignalManager tsm;
  TradeSignalEntry signal1;
  // TaskAction<ActionType1> _taction1;
  // TaskAction<ActionType2> _taction2;
  // TaskCondition<ConditionType1> _tcond1;
  // TaskCondition<ConditionType2> _tcond2;
  printf("%s", tsm.ToString().c_str());
  return true;
}
#ifdef __cplusplus
}
#endif

int main(int argc, char **argv) {
  printf("Hello World!\n");
  return 0;
}
