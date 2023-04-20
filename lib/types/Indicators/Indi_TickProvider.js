import {CppArray} from "../CppArray";
import {TickTAB} from "../TickTAB";
import {Indicator} from "./Indicator";

export class Indi_TickProvider extends Indicator {
  Feed(ticks: CppArray<TickTAB<number>>) {}

  BufferSize(): number {}
}
