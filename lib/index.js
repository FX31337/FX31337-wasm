export {TradeSignal} from './types/TradeSignal';
export {TradeSignalManager} from './types/TradeSignalManager';
export {TaskManager} from './types/TaskManager';
export {Task} from './types/Task';
export {TaskEntry} from './types/TaskEntry';
export {TickAB} from './types/TickAB';
export {TickABArray} from './types/Arrays/TickAB.double';
export {TickTAB} from './types/TickTAB';
export {TickTABArray} from './types/Arrays/TickTAB.double';
export {IndicatorTest} from './types/Tests/IndicatorTest';
export {Test, run} from './Runner';

import {Indi_TickProvider as TickProvider} from './types/Indicators/Indi_TickProvider';
import {IndicatorTf as Tf} from './types/Indicators/IndicatorTf';

export {Tester} from './types/Tester';

export const indicators = {
  TickProvider,
  Tf
};

export const timeframes = {
  CURRENT: 0,  // Current timeframe.
  M1: 1,       // 1 minute.
  M2: 2,       // 2 minutes.
  M3: 3,       // 3 minutes.
  M4: 4,       // 4 minutes.
  M5: 5,       // 5 minutes.
  M6: 6,       // 6 minutes.
  M10: 10,     // 10 minutes.
  M12: 12,     // 12 minutes.
  M15: 15,     // 15 minutes.
  M20: 20,     // 20 minutes.
  M30: 30,     // 30 minutes.
  H1: 60,      // 1 hour.
  H2: 120,     // 2 hours.
  H3: 180,     // 3 hours.
  H4: 240,     // 4 hours.
  H6: 360,     // 6 hours.
  H8: 480,     // 8 hours.
  H12: 720,    // 12 hours.
  D1: 1440,    // 1 day.
  W1: 10080,   // 1 week.
  MN1: 43200   // 1 month.
};
