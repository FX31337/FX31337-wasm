import { EmSmartPtr } from './EmSmartPtr';
import { TaskEntry } from './TaskEntry';
export declare class Task extends EmSmartPtr {
    Add(taskEntry: TaskEntry): void;
}
