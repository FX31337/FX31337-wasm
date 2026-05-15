/**
 * A single typed output value from an indicator entry.
 * Holds type information and the actual value (stored in a union for different types).
 *
 * Delivered as an Emscripten heap object (class_<>); call .delete() when done.
 */
export class IndicatorDataEntryValue {
  /** Flags byte encoding the data type in the upper 4 bits. */
  flags: number = 0;

  /**
   * Gets the data type of this value.
   * @returns ENUM_DATATYPE value (TYPE_DOUBLE, TYPE_FLOAT, TYPE_INT, TYPE_LONG, etc.).
   */
  GetDataType(): number { return 0; }

  /**
   * Sets the data type of this value.
   * @param type The ENUM_DATATYPE to set.
   */
  SetDataType(type: number): void {}

  /**
   * Gets the value as a double.
   */
  GetDbl(): number { return 0; }

  /**
   * Gets the value as a 32-bit float.
   */
  GetFloat(): number { return 0; }

  /**
   * Gets the value as a 32-bit integer.
   */
  GetInt(): number { return 0; }

  /**
   * Gets the value as a 64-bit long integer (returned as number; may lose precision for very large values).
   */
  GetLong(): number { return 0; }

  /**
   * Gets the value cast to double.
   */
  Get(): number { return 0; }

  /**
   * Sets the value as a double.
   * @param value The value to set.
   */
  Set(value: number): void {}

  /**
   * Converts the value to a string.
   */
  ToString(): string { return ''; }

  /**
   * Frees the C++ heap memory. Call this when done using the value.
   */
  delete(): void {}
}

