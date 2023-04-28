const fs = require('fs');

const contentJs = require('./../../dist/IndicatorTest');
const contentWasm = 'dist/IndicatorTest.wasm';

const lib = require('../../index.js');

class Test extends lib.Test {
  run(lib) {
    const ticker = new lib.indicators.TickProvider({symbol: "EURUSD"});

    const tfM5 = new lib.indicators.Tf(lib.timeframes.M1);
    tfM5.SetSource(ticker);

    const rsiM5 = new lib.indicators.RSI({ period: 13, appliedPrice: lib.ap.open, shift: 0 });
    rsiM5.SetSource(tfM5);

    lib.Tester.Add(rsiM5);

    // Note that all timeframes shares the same ticks and so you may reuse single TickProvider indicator for other Tester instances for the same symbols pair.

    const ticks = new lib.TickTABArray();

    ticks.Push({ask: 0.1, bid: 0.11, time_ms: BigInt(1000) * BigInt(1)});
    ticks.Push({ask: 0.2, bid: 0.22, time_ms: BigInt(1000) * BigInt(60)});
    ticks.Push({ask: 0.3, bid: 0.33, time_ms: BigInt(1000) * BigInt(120)});

    //ticker.Feed(ticks);

    lib.Tester.FeedTickProvider(ticker);

    // You can also use tester.RunTick() method in a loop or if you're sure that new tick arrived.
    lib.Tester.RunAllTicks();
  }
}

lib.run(Test, contentJs, contentWasm);
