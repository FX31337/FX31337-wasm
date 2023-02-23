const Runner = require('../../lib/Runner.js');

class Test extends Runner.Test {
  run(lib) {
    const tickProvider = new lib.Indi_TickProvider();

    console.log (lib);

    const tick1 = new lib.TickTAB();
    const tick2 = new lib.TickTAB();
    const tick3 = new lib.TickTAB();

    tick1.time_ms = 0;
    tick1.ask = 1.0;
    tick1.bid = 1.1;

    tick2.time_ms = 100;
    tick2.ask = 1.1;
    tick2.bid = 1.2;

    tick3.time_ms = 250;
    tick3.ask = 1.2;
    tick3.bid = 1.3;

    const ticks = new lib.TickTABArray();

    ticks.Push(tick1);
    ticks.Push(tick2);
    ticks.Push(tick3);

    console.log (`There are: ${ticks.Size()} ticks added.`);
    console.log (`There was: ${tickProvider.BufferSize()} ticks in TickProvider buffer.`);

    tickProvider.Feed(ticks);

    console.log(`There are: ${tickProvider.BufferSize()} ticks in TickProvider buffer.`);
  }
}

Runner.run(Test, 'Indicator');
