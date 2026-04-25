import { EmPtr } from './EmPtr';
export declare class CppArray<X> extends EmPtr {
    Push(value: X): void;
    Size(): number;
}
