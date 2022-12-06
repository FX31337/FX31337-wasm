const Runner = require('../../lib/Runner.js');

class Test extends Runner.Test {
  run(lib) {
    const tsm = new lib.TradeSignalManager();
    const signal1 = new lib.TradeSignal();
    tsm.SignalAdd(signal1);
    console.log(`TSM: `, tsm.ToString());
  }
}

Runner.run(Test, 'TaskRunner');
