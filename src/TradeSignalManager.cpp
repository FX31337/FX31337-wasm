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
#include <cmath>
#include "classes/Trade/TradeSignalManager.h"
#include "classes/Common.extern.h"
#include "classes/Common.define.h"
#include "classes/Common.extern.h"
#include "classes/Std.h"
#include "classes/String.extern.h"
#include "classes/Task/Taskable.h"
#include "classes/Task/TaskAction.h"
#include "classes/Task/TaskCondition.struct.h"
#include "classes/Task/TaskGetter.struct.h"
#include "classes/Task/TaskSetter.struct.h"

#ifdef EMSCRIPTEN
#include <emscripten/bind.h>
using namespace emscripten;
#endif

class Test {
 public:
   int add(int a, int b) { return a + b; }
};


#ifdef EMSCRIPTEN
EMSCRIPTEN_BINDINGS(Test) {
  class_<Test>("Test")
    .constructor()
    .function("add", &Test::add)
    ;
}
#endif

// Declare and define the external functions and variables.
#ifdef __cplusplus
extern "C" {
#endif

  // Define test classes.
class ConditionType1 : public TaskConditionBase {
 public:
  bool Check(const TaskConditionEntry &_entry) { return true; }
};
class ConditionType2 : public TaskConditionBase {
 public:
  bool Check(const TaskConditionEntry &_entry) { return true; }
};
class ActionType1 : public TaskActionBase {
 public:
  bool Run(const TaskActionEntry &_entry) { return true; }
};
class ActionType2 : public TaskActionBase {
 public:
  bool Run(const TaskActionEntry &_entry) { return true; }
};
class TaskType1 : public Taskable<MqlParam> {
  bool Check(const TaskConditionEntry &_entry) { return true; }
  MqlParam Get(const TaskGetterEntry &_entry) {
    MqlParam _result;
    return _result;
  }
  bool Run(const TaskActionEntry &_entry) { return true; }
  bool Set(const TaskSetterEntry &_entry, const MqlParam &_entry_value) { return true; }
};

int sum(int a, int b) { return a + b; }
bool test() {
  //printf("%s\n", __builtin_FUNCTION());
  TradeSignalManager tsm;
  TradeSignalEntry signal1;
  TaskAction<ActionType1> _taction1;
  TaskAction<ActionType2> _taction2;
  TaskCondition<ConditionType1> _tcond1;
  TaskCondition<ConditionType2> _tcond2;
  printf("Task: %s", tsm.ToString().c_str());
  return true;
}
#ifdef __cplusplus
}
#endif

int main(int argc, char **argv) {
  /*
    test();
  printf("Hello World 2!\n");
  std::getchar();

  */
  return 0;
}
