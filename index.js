(function (global, factory) {
  typeof exports === 'object' && typeof module !== 'undefined' ? factory(exports) :
  typeof define === 'function' && define.amd ? define(['exports'], factory) :
  (global = typeof globalThis !== 'undefined' ? globalThis : global || self, factory(global["fx31337-wasm"] = {}));
})(this, (function (exports) { 'use strict';

  class EmPtr {
    delete() {}
  }

  class TradeSignal extends EmPtr {}

  class EmSmartPtr {
    delete() {}
  }

  class TradeSignalManager extends EmSmartPtr {
    /**
     * @param {TradeSignal} tradeSignal
     */
    SignalAdd(tradeSignal) {}

    /**
     * @returns {string}
     */
    ToString() {}
  }

  class TaskManager extends EmPtr {
    /**
     * @param {Task|string} task
     */
    Add(task) {}
    Clear() {}
  }

  class Task extends EmSmartPtr {
    /**
     * @param {TaskEntry} taskEntry
     */
    Add(taskEntry) {}
  }

  class EmStruct {}

  class TaskEntry extends EmStruct {}

  class TickAB extends EmStruct {
    constructor() {
      super();
      this.ask = 0;
      this.bid = 0;
    }
  }

  class CppArray extends EmPtr {
    Push(value) {}
    Size(value) {}
  }

  class TickABArray extends CppArray {}

  class TickTAB extends TickAB {
    constructor() {
      super();
      this.time_ms = 0;
    }
  }

  class TickTABArray extends CppArray {}

  class IndicatorTest extends EmStruct {
    static Init() {}
    static Run() {}
  }

  class Test {
    /**
     * @param {import('./index.js')} lib
     */
    run(lib) {}
  }
  function set(schema, path, value, delimiter = '.') {
    const pList = path.split(delimiter);
    const len = pList.length;
    for (let i = 0; i < len - 1; i++) {
      const elem = pList[i];
      if (!schema[elem]) schema[elem] = {};
      schema = schema[elem];
    }
    schema[pList[len - 1]] = value;
  }

  /**
   * Credits: https://github.com/emscripten-core/emscripten/issues/5874#issuecomment-348141138
   *
   * Generates an object that corresponds to the namespaces in c++
   * if classes are exported like this
   * EMSCRIPTEN_BINDINGS(MyClass) {
   *  class_< a::b::c::MyClass >("a.b.c.MyClass")
   *  //..
   *  }
   */
  function namespaces(module) {
    // final namespace
    const ns = {};

    // find all fields in module with replaced special chars
    // (all special characters seem to be replaced by $)
    const filtered = Object.keys(module).filter(k => k.indexOf('$') !== -1);
    for (let key of filtered) set(ns, key, module[key], '$');
    return ns;
  }
  async function runTest(what, contentJs, contentWasm) {
    /** @type {import('./index.js')} */
    let lib = await contentJs({
      locateFile(path) {
        if (path.endsWith(`.wasm`)) {
          return contentWasm;
        }
        return path;
      }
    });

    // Merging module with namespaced classes.
    lib = {
      ...lib,
      ...namespaces(lib)
    };
    lib.TickAB = TickAB;
    lib.TickTAB = TickTAB;
    const test = new what();
    test.run(lib);
  }
  function run(what, contentJs, contentWasm) {
    runTest(what, contentJs, contentWasm).then(() => {});
  }

  class IndicatorBase extends EmSmartPtr {
    SetSource(baseIndicator) {}
  }

  class IndicatorData extends IndicatorBase {}

  class Indicator extends IndicatorData {}

  class Indi_TickProvider extends Indicator {
    Feed(ticks) {}
    BufferSize() {}
  }

  class IndicatorCandle extends Indicator {}

  class IndicatorTf extends IndicatorCandle {
    constructor(tf) {
      super();
    }
  }

  class Tester {
    static Add(indicator) {}
    static AddPlatformWise(indicator, symbol, tf) {}
    static FeedTickProvider(tickProvider) {}
    static RunAllTicks() {}
    static RunTick() {}
  }

  const indicators = {
    TickProvider: Indi_TickProvider,
    Tf: IndicatorTf
  };
  const timeframes = {
    CURRENT: 0,
    // Current timeframe.
    M1: 1,
    // 1 minute.
    M2: 2,
    // 2 minutes.
    M3: 3,
    // 3 minutes.
    M4: 4,
    // 4 minutes.
    M5: 5,
    // 5 minutes.
    M6: 6,
    // 6 minutes.
    M10: 10,
    // 10 minutes.
    M12: 12,
    // 12 minutes.
    M15: 15,
    // 15 minutes.
    M20: 20,
    // 20 minutes.
    M30: 30,
    // 30 minutes.
    H1: 60,
    // 1 hour.
    H2: 120,
    // 2 hours.
    H3: 180,
    // 3 hours.
    H4: 240,
    // 4 hours.
    H6: 360,
    // 6 hours.
    H8: 480,
    // 8 hours.
    H12: 720,
    // 12 hours.
    D1: 1440,
    // 1 day.
    W1: 10080,
    // 1 week.
    MN1: 43200 // 1 month.
  };

  exports.IndicatorTest = IndicatorTest;
  exports.Task = Task;
  exports.TaskEntry = TaskEntry;
  exports.TaskManager = TaskManager;
  exports.Test = Test;
  exports.Tester = Tester;
  exports.TickAB = TickAB;
  exports.TickABArray = TickABArray;
  exports.TickTAB = TickTAB;
  exports.TickTABArray = TickTABArray;
  exports.TradeSignal = TradeSignal;
  exports.TradeSignalManager = TradeSignalManager;
  exports.indicators = indicators;
  exports.run = run;
  exports.timeframes = timeframes;

}));
