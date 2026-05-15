import { EmStruct } from './EmStruct';

/**
 * A single aggregated value entry within a TesterValuesColumns column, as
 * retrieved from Tester.GetValues(). When multiple raw indicator values fall
 * inside the same time-step column they are aggregated into open/high/low/close/avg.
 */
export class TesterValuesColumnValue extends EmStruct {
  /** Name of the indicator output (e.g. "RSI"). */
  name: string = '';
  /** Data type of the value (ENUM_DATATYPE integer). */
  type: number = 0;
  /** Timestamp in ms of the first value added to this column. */
  time_open_ms: number = 0;
  /** Timestamp in ms of the last value added to this column. */
  time_close_ms: number = 0;
  /** Value of the first (oldest) entry in the column. */
  value_open: number = 0;
  /** Highest value among all entries in the column. */
  value_high: number = 0;
  /** Lowest value among all entries in the column. */
  value_low: number = 0;
  /** Value of the last (newest) entry in the column. */
  value_close: number = 0;
  /** Average of all values in the column. */
  value_avg: number = 0;
  /** Number of raw values aggregated into this column entry. */
  num_values: number = 0;
}
