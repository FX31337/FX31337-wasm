import { Test, run, LibModule } from '../../lib/Runner';

class TaskRunnerTest extends Test {
  run(lib: LibModule): void {
    const tm = new lib['TaskManager']();
    const task1 = new lib['Task']();
    tm.Add(task1);

    console.log(tm);
    console.log(task1);
    task1.delete();

    tm.Clear();
  }
}

run(TaskRunnerTest, 'TaskRunner');

