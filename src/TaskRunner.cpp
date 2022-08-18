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
// #include <emscripten/emscripten.h>
// #include <emscripten/bind.h>

// Local includes.
#include "classes/Common.define.h"
#include "classes/Common.extern.h"
#include "classes/Std.h"
#include "classes/String.extern.h"
#include "classes/Task/Task.h"

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

// Declare and define the external functions and variables.
extern "C" {
int sum(int a, int b) { return a + b; }
bool test_task_action(int aid) {
  TaskActionEntry entry(aid);
  return false;
}
bool test_task_condition(int cid) {
  TaskConditionEntry entry(cid);
  return false;
}
bool test_task_getter(int gid) {
  TaskGetterEntry entry(gid);
  return false;
}
bool test_task_setter(int sid) {
  TaskSetterEntry entry(sid);
  return false;
}
}

int main(int argc, char **argv) {
  printf("Hello World!\n");
  return 0;
}
