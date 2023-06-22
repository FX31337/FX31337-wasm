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
 * Chart functionality.
 */

// Includes
// #include <emscripten/emscripten.h>
// #include <emscripten/bind.h>
#define __debug__
#define __debug_verbose__

// Local includes.
#include "classes/Common.define.h"
#include "classes/Common.extern.h"
#include "classes/Indicator/Indicator.h"
#include "classes/Std.h"
#include "classes/String.extern.h"

int main(int argc, char **argv) {
  Print("Hello World!");
  return 0;
}