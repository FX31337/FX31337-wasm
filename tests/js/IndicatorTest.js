const fs = require('fs');

const contentJs = require('./../../dist/IndicatorTest');
const contentWasm = 'dist/IndicatorTest.wasm';

const lib = require('../../index.js');

class Test extends lib.Test {
  /**
   * @param {lib} lib
   */
  run(lib) {
    const ticker = new lib.indicators.TickProvider({ symbol: "EURUSD" });

    const tfM5 = new lib.indicators.Tf(lib.timeframes.M1);
    tfM5.SetSource(ticker);

    const rsiM5 = new lib.indicators.RSI({ period: 13, appliedPrice: lib.ap.open, shift: 0 });
    rsiM5.SetSource(tfM5);

    lib.Tester.Add(rsiM5);

    // Note that all timeframes shares the same ticks and so you may reuse single TickProvider indicator for other Tester instances for the same symbols pair.

    const ticks = new lib.TickTABArray();

    ticks.Push({ ask: 0.1, bid: 0.11, time_ms: BigInt(1000) * BigInt(1) });
    ticks.Push({ ask: 0.2, bid: 0.22, time_ms: BigInt(1000) * BigInt(60) });
    ticks.Push({ ask: 0.3, bid: 0.33, time_ms: BigInt(1000) * BigInt(120) });

    //ticker.Feed(ticks);

    lib.Tester.FeedTickProvider(ticker);

    // You can also use tester.RunTick() method in a loop or if you're sure that new tick arrived.
    lib.Tester.RunAllTicks();

    // We want to display values of each indicator on the chart. To do so, we
    // need to know time of each indicator's value. To determine time, we check
    // indicator's time-frame, which is provided by assigned TF indicator.

    // Chart displays values in columns and our values are aggregated by TF. If TF is set to minute and we zoom out
    // the chart, there could by thousands of columns that would need to be displayed. To make chart be more clear, we
    // provide Tester.GetValues() method which aggregates already existing TF-based values into higher TF version. E.g.,
    // when using 1-minute TF for values we could fetch a 15-minute TF chunk using:
    // lib.Tester.GetValues(timeFromMs, timeToMs, 60 * 15).

    // You can use lib.Tester.GetTimeByScrollAndZoom(scrollX, zoom) to retrieve "from", "to" and "timeStep" parameters
    // to directly pass them into lib.Tester.GetValues().

    // Values chunk contains indicator values aggregated by given number of
    // seconds. When you zoom out the chart, you increase that number of
    // seconds. Each item in the chunk will represent a time-frame being
    // multiplication of the given number of seconds.
    // timeFrom and timeTo parameters is a time-range in ms (BigInt type).

    // There could be non-TF-based indicators which have starting time and values that don't fit timeStep passed to
    // lib.Tester.GetValues().

    // const params = lib.Tester.GetTimeByScrollAndZoom(chart.scrollX, chart.zoom);

    // const chunk = lib.Tester.GetValues(params.timeFromMs, params.timeToMs, params.timeStep);

    /*
      Chunk structure for timeFrom = 0, timeTo = 150, timeStep = 60:
      {
        timeStepBased: { // Values that fit passed timeStep.
          { time: 0, indicator_values: { indi_ref1: [ values... ], indi_ref2: [ values... ], indi_refN: ... } },
          { time: 60, ... },
          { time: 120, ... },
          { time: 160, ... }
        },
        loose: { // Values that don't fit passed timeStep.
          { time: 32, indicator_values: { indi_refN: ..., ... } },
          { time: 45, indicator_values: { indi_refN: ..., ... } },
          { time: 49, indicator_values: { indi_refN: ..., ... } },
          { time: 65, indicator_values: { indi_refN: ..., ... } },
        }
    */
  }
}

lib.run(Test, contentJs, contentWasm);
