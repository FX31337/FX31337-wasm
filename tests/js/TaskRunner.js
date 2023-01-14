const Runner = require('../../lib/Runner.js');

class Test extends Runner.Test {
  run(lib) {
    const tm = new lib.TaskManager();
    const task1 = new lib.Task();
    tm.Add(task1);

    console.log(tm);
    console.log(task1);
    task1.delete();

    tm.Clear();
  }
}

Runner.run(Test, 'TaskRunner');
