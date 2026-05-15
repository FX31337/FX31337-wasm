import { EmSmartPtr } from '../EmSmartPtr';
import { IndicatorData } from './IndicatorData';

export class IndicatorBase extends EmSmartPtr {
  SetSource(baseIndicator: IndicatorData): void {}
  SetName(name: string): void {}
  GetEntry(shift: number): IndicatorDataEntry { return 0; }
}
