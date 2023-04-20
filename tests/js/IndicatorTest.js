const fs = require('fs');

const contentJs = require('./../../dist/IndicatorTest');
const contentWasm = 'dist/IndicatorTest.wasm';

const lib = require('../../index.js');

class Test extends lib.Test {
  run(lib) {
    const ticker = new lib.indicators.TickProvider();

    console.log (lib.indicators);

    const tfM5 = new lib.indicators.Tf(lib.timeframes.M5);
    tfM5.SetSource(ticker);

    const rsiM5 = new lib.indicators.RSI(13);
    rsiM5.SetSource(tfM5);

    lib.Tester.Add(rsiM5);

    // Note that all timeframes shares the same ticks and so you may reuse single TickProvider indicator for other Tester instances for the same symbols pair.
    /*
    ticks.Add([
      {timestamp: ..., ask: ..., bid: ...},
      {timestamp: ..., ask: ..., bid: ...}
    ]);
    */

    // You can also use tester.RunTick() method in a loop or if you're sure that new tick arrived.
    lib.Tester.RunAllTicks();
  }
}

lib.run(Test, contentJs, contentWasm);
