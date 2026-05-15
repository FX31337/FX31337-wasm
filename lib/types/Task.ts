import { EmSmartPtr } from './EmSmartPtr';
import { TaskEntry } from './TaskEntry';

export class Task extends EmSmartPtr {
  Add(taskEntry: TaskEntry): void {}
}
