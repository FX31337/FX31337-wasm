import { Test, run, LibModule } from '../../lib/Runner';

class TradeSignalManagerTest extends Test {
  run(lib: LibModule): void {
    const tsm = new lib['TradeSignalManager']();
    const signal1 = new lib['TradeSignal']();
    tsm.SignalAdd(signal1);
    console.log(`TSM: `, tsm.ToString());
  }
}

run(TradeSignalManagerTest, 'TradeSignalManager');

