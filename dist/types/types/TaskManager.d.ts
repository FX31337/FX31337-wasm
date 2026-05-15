import { EmPtr } from './EmPtr';
import { Task } from './Task';
export declare class TaskManager extends EmPtr {
    Add(task: Task | string): void;
    Clear(): void;
}
