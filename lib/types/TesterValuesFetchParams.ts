import { EmStruct } from './EmStruct';

/**
 * Parameters to pass into Tester.GetValuesByParams(). Can be calculated by
 * Tester.GetTimeByScrollAndZoom(scroll, zoom, visibleIntervals).
 */
export class TesterValuesFetchParams extends EmStruct {
  /** Beginning range (inclusive) of the values to be returned, in milliseconds. */
  timeFromMs: bigint = BigInt(0);
  /** Ending range (inclusive) of the values to be returned, in milliseconds. */
  timeToMs: bigint = BigInt(0);
  /** Width of a single column in seconds. Values within the same column are aggregated. */
  timeStepSecs: number = 0;
}
