import { CppArray } from '../CppArray';
import { TickTAB } from '../TickTAB';
import { Indicator } from './Indicator';

export interface Indi_TickProviderParams {
  symbol: string;
}

export class Indi_TickProvider extends Indicator {
  constructor(params: Indi_TickProviderParams) {
    super();
  }
  Feed(ticks: CppArray<TickTAB>): void {}
  BufferSize(): number { return 0; }
}
