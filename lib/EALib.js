export const EALib = {
  TradeSignal: require('./types/TradeSignal.js').TradeSignal,
  TradeSignalManager: require('./types/TradeSignalManager.js').TradeSignalManager,
  TaskManager: require('./types/TaskManager.js').TaskManager,
  Task: require('./types/Task.js').Task,
  TaskEntry: require('./types/TaskEntry.js').TaskEntry,
  Indi_TickProvider: require('./types/Indicators/Indi_TickProvider.js').Indi_TickProvider,
  TickAB: require('./types/TickAB.js').TickAB,
  TickABArray: require('./types/Arrays/TickAB.double.js').TickABArray,
  TickTAB: require('./types/TickTAB.js').TickTAB,
  TickTABArray: require('./types/Arrays/TickTAB.double.js').TickTABArray,
};
