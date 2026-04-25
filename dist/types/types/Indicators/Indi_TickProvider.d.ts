import { CppArray } from '../CppArray';
import { TickTAB } from '../TickTAB';
import { Indicator } from './Indicator';
export declare class Indi_TickProvider extends Indicator {
    Feed(ticks: CppArray<TickTAB>): void;
    BufferSize(): number;
}
