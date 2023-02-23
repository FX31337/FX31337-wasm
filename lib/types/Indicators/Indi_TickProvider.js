import {CppArray} from "./CppArray";
import {TickTAB} from "./TickTAB";
import {EmSmartPtr} from "../EmSmartPtr";

export class Indi_TickProvider extends EmSmartPtr {
  Feed(ticks: CppArray<TickTAB<number>>);

  BufferSize(): number;
}
