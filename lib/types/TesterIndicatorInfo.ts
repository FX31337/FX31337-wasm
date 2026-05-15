import { EmStruct } from './EmStruct';

/**
 * Details about a given indicator, as retrieved from Tester.GetValues().
 */
export class TesterIndicatorInfo extends EmStruct {
  /** Name of the indicator. */
  name: string = '';
  /** Index of the indicator in the Platform. */
  index: number = 0;
  /** Number of values calculated for this indicator. */
  num_values: number = 0;
  /** Symbol pair the indicator works on. */
  symbol: string = '';
  /** Time-frame the indicator works on (ENUM_TIMEFRAMES integer value). */
  tf: number = 0;
}
