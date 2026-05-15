import { Test, run, LibModule } from '../../lib/Runner';
import { TesterValues } from '../../lib/types/TesterValues';
import wasmJs from '../../dist/IndicatorTest';

class IndicatorRunTest extends Test {
  run(lib: LibModule): void {

    lib.Tester.Init();

    console.log(lib);

    const ticker = new lib.indicators.TickProvider({ symbol: 'EURUSD' });

    const tfM5 = new lib.indicators.Tf(lib.timeframes.M1);
    tfM5.SetSource(ticker);

    const rsiM5 = new lib.indicators.RSI({ period: 13, appliedPrice: lib.ap.open, shift: 0 });
    rsiM5.SetName("RSI M5");
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

    // const values: TesterValues = lib.Tester.GetValues(BigInt(946684800000), BigInt(946685980000), 60, true);

    const values: TesterValues = lib.Tester.GetValues(BigInt(0), BigInt(1000), 60, true);

    console.log(values.timestep_based);
  }
}

run(IndicatorRunTest, wasmJs, 'dist/IndicatorTest.wasm');
