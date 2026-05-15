import { CppArray } from './CppArray';
import { IndicatorDataEntryValue } from './IndicatorDataEntryValue';
// Note: IndicatorDataEntryValue is a heap-managed Emscripten object (class_<>).
// The .values CppArray holds references; individual entries are accessed via GetValueAt().

/**
 * An entry (single bar) of indicator data containing timestamp, flags, and output values.
 *
 * Each indicator calculates one entry per bar/tick. An entry holds multiple values
 * (e.g., main output, signal line, histogram) indexed by mode (0 = primary output).
 *
 * Delivered as a Emscripten heap object; call .delete() to free memory.
 */
export class IndicatorDataEntry {
  /** Unix timestamp (seconds) of the bar this entry represents. */
  timestamp: bigint = BigInt(0);

  /** Entry flags (validity, calculations state, etc.). */
  flags: number = 0;

  /** Array of output values for this entry. Each value can be of different types. */
  values: CppArray<IndicatorDataEntryValue> = new CppArray<IndicatorDataEntryValue>();

  /**
   * Creates a new IndicatorDataEntry.
   * @param size Number of output modes (values) this entry will hold (default 1).
   */
  constructor(size?: number) {}

  /**
   * Returns the number of output values in this entry.
   */
  GetSize(): number { return 0; }

  /**
   * Gets value at index cast to number type.
   * @param index Index of the value to retrieve.
   * @returns The value at the given index.
   */
  GetValueAt(index: number): number { return 0; }

  /**
   * Checks if any value in this entry equals the given number.
   * @param value The value to search for.
   * @returns True if the value is found in this entry.
   */
  HasValue(value: number): boolean { return false; }

  /**
   * Checks if the minimum value in this entry is >= the given number.
   * @param value The threshold value.
   */
  IsGe(value: number): boolean { return false; }

  /**
   * Checks if the minimum value in this entry is > the given number.
   * @param value The threshold value.
   */
  IsGt(value: number): boolean { return false; }

  /**
   * Checks if the maximum value in this entry is <= the given number.
   * @param value The threshold value.
   */
  IsLe(value: number): boolean { return false; }

  /**
   * Checks if the maximum value in this entry is < the given number.
   * @param value The threshold value.
   */
  IsLt(value: number): boolean { return false; }

  /**
   * Checks if all values in this entry are within the given range.
   * @param min Minimum threshold (inclusive).
   * @param max Maximum threshold (inclusive).
   */
  IsWithinRange(min: number, max: number): boolean { return false; }

  /**
   * Gets the average of all numeric values in this entry.
   * @param size Limit to first N values; pass 0 to use all values.
   * @returns The average value.
   */
  GetAvg(size: number): number { return 0; }

  /**
   * Gets the minimum value across all numeric values in this entry.
   * @param size Limit to first N values; pass 0 to use all values.
   * @returns The minimum value.
   */
  GetMin(size: number): number { return 0; }

  /**
   * Gets the maximum value across all numeric values in this entry.
   * @param size Limit to first N values; pass 0 to use all values.
   * @returns The maximum value.
   */
  GetMax(size: number): number { return 0; }

  /**
   * Gets the sum of all numeric values in this entry.
   * @param size Limit to first N values; pass 0 to use all values.
   * @returns The sum of all values.
   */
  GetSum(size: number): number { return 0; }

  /**
   * Gets two output values from this entry (modes 0 and 1).
   * @returns Object with val1 and val2 properties.
   */
  GetValues2(): { val1: number; val2: number } { return { val1: 0, val2: 0 }; }

  /**
   * Gets three output values from this entry (modes 0, 1, and 2).
   * @returns Object with val1, val2, val3 properties.
   */
  GetValues3(): { val1: number; val2: number; val3: number } { return { val1: 0, val2: 0, val3: 0 }; }

  /**
   * Gets four output values from this entry (modes 0, 1, 2, and 3).
   * @returns Object with val1, val2, val3, val4 properties.
   */
  GetValues4(): { val1: number; val2: number; val3: number; val4: number } {
    return { val1: 0, val2: 0, val3: 0, val4: 0 };
  }

  /**
   * Gets the timestamp as a UTC day-of-year (1-366).
   */
  GetDayOfYear(): number { return 0; }

  /**
   * Gets the timestamp as a month (1-12).
   */
  GetMonth(): number { return 0; }

  /**
   * Gets the timestamp as a year.
   */
  GetYear(): number { return 0; }

  /**
   * Gets the timestamp of this entry's bar.
   * @returns Unix timestamp in seconds.
   */
  GetTime(): bigint { return BigInt(0); }

  /**
   * Gets the data type of the value at the given mode.
   * @param mode Output mode/index (0 for primary output).
   * @returns ENUM_DATATYPE value.
   */
  GetDataType(mode: number): number { return 0; }

  /**
   * Gets the data type flags for the given ENUM_DATATYPE.
   * @param dtype The data type.
   * @returns Flags indicating type properties (real/integer, signed/unsigned, etc.).
   */
  GetDataTypeFlags(dtype: number): number { return 0; }

  /**
   * Resizes the values array to hold N output modes.
   * @param size New number of modes to support. Pass 0 for a no-op (returns true).
   * @returns True if successful.
   */
  Resize(size: number): boolean { return false; }

  /**
   * Checks if the given flag is set in this entry's flags.
   * @param flag The flag to check.
   * @returns True if the flag is set.
   */
  CheckFlag(flag: number): boolean { return false; }

  /**
   * Checks if any of the given flags are set in this entry's flags.
   * @param flags The flags to check.
   * @returns True if any flag is set.
   */
  CheckFlags(flags: number): boolean { return false; }

  /**
   * Checks if all of the given flags are set in this entry's flags.
   * @param flags The flags to check.
   * @returns True if all flags are set.
   */
  CheckFlagsAll(flags: number): boolean { return false; }

  /**
   * Adds (ORs) the given flags to this entry's flags.
   * @param flags The flags to add.
   */
  AddFlags(flags: number): void {}

  /**
   * Removes (ANDs with NOT) the given flags from this entry's flags.
   * @param flags The flags to remove.
   */
  RemoveFlags(flags: number): void {}

  /**
   * Sets or clears a specific flag.
   * @param flag The flag to set/clear.
   * @param value True to set, false to clear.
   */
  SetFlag(flag: number, value: boolean): void {}

  /**
   * Replaces all flags with the given value.
   * @param flags The new flags value.
   */
  SetFlags(flags: number): void {}

  /**
   * Gets the current flags value.
   */
  GetFlags(): number { return 0; }

  /**
   * Checks if this entry is valid (has the valid flag set).
   */
  IsValid(): boolean { return false; }

  /**
   * Generates a comma-separated string of all values in this entry.
   * @returns CSV string representation.
   */
  ToCSV(): string { return ''; }

  /**
   * Generates a string representation of this entry (same as ToCSV).
   * @returns String representation.
   */
  ToString(): string { return ''; }

  /**
   * Frees the C++ heap memory. Call this when done using the entry.
   */
  delete(): void {}
}
