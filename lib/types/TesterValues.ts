import { TesterValuesColumns } from './TesterValuesColumns';

/**
 * Result returned by Tester.GetValues() / Tester.GetValuesByParams().
 * Delivered as a plain JS object (no .delete() required).
 */

export interface TesterValues {
  /** Columns for TF-based indicators whose values align to the requested time-step. */
  timestep_based: TesterValuesColumns[];
  /** Columns for non-TF ("loose") indicators whose values do not align to the time-step. */
  loose: TesterValuesColumns[];
}
