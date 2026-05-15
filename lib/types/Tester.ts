import { IndicatorData } from './Indicators/IndicatorData';
import { Indi_TickProvider } from './Indicators/Indi_TickProvider';
import { TesterValuesFetchParams } from './TesterValuesFetchParams';
import { TesterValues } from './TesterValues';

/**
 * Tester class for backtesting indicators.
 *
 * Allows adding indicators, feeding them with tick data, running historical ticks,
 * and retrieving aggregated indicator values for visualization and analysis.
 */
export class Tester {
  /**
   * Initializes the Tester and underlying platform.
   * Must be called once before using other Tester methods.
   */
  public static Init(): void {}

  /**
   * Adds an indicator to the Tester for testing.
   * The indicator must have its candle/tick sources already configured.
   *
   * @param indicator The indicator to add (must have source bindings set up).
   */
  public static Add(indicator: IndicatorData): void {}

  /**
   * Adds an indicator using the default candle and tick indicators for the given symbol/timeframe.
   *
   * Under Emscripten, the default tick indicator is TickProvider. After calling this method,
   * the TickProvider must be fed with actual tick data via FeedTickProvider().
   *
   * @param indicator The indicator to add.
   * @param symbol Symbol pair (e.g. "EURUSD").
   * @param tf Timeframe constant (e.g. from lib.timeframes).
   */
  public static AddPlatformWise(indicator: IndicatorData, symbol: string, tf: number): void {}

  /**
   * Feeds the default tick provider for a given symbol with tick data.
   *
   * @param tickProvider A TickProvider indicator instance to feed.
   */
  public static FeedTickProvider(tickProvider: Indi_TickProvider): void {}

  /**
   * Processes all remaining ticks in the system.
   * Blocks until all ticks are consumed by all indicators.
   */
  public static RunAllTicks(): void {}

  /**
   * Processes a single tick through all indicators.
   *
   * @returns `true` if a tick was processed, `false` if no more ticks are available.
   */
  public static RunTick(): boolean { return false; }

  /**
   * Retrieves aggregated indicator values for the given time range.
   *
   * Values are grouped into time-step columns. When multiple indicator values
   * fall within the same column, they are aggregated into open/high/low/close/average.
   * TF-based indicators (regular candles) are returned in `timestep_based`.
   * Non-TF indicators (e.g., Renko) are returned in `loose`.
   *
   * @param timeFromMs Start of the range in milliseconds (inclusive).
   * @param timeToMs End of the range in milliseconds (inclusive).
   * @param timeStepSecs Column width in seconds. Values within one column are aggregated.
   * @param aggregateNoFits Whether to aggregate loose-TF values (default: true).
   * @returns Object with `timestep_based` and `loose` column arrays.
   */
  public static GetValues(
    timeFromMs: bigint,
    timeToMs: bigint,
    timeStepSecs: number,
    aggregateNoFits?: boolean
  ): TesterValues { return { timestep_based: [], loose: [] }; }

  /**
   * Retrieves aggregated indicator values using pre-built parameters.
   *
   * Equivalent to GetValues() but takes a single TesterValuesFetchParams object
   * (which can be created via GetTimeByScrollAndZoom()).
   *
   * @param params Fetch parameters object with timeFromMs, timeToMs, timeStepSecs.
   * @param aggregateNoFits Whether to aggregate loose-TF values (default: true).
   * @returns Object with `timestep_based` and `loose` column arrays.
   */
  public static GetValuesByParams(
    params: TesterValuesFetchParams,
    aggregateNoFits?: boolean
  ): TesterValues { return { timestep_based: [], loose: [] }; }

  /**
   * Converts a scroll/zoom/viewport combination into fetch parameters.
   *
   * Base interval for zoom 1 is 1 minute. Zoom 2 means 60s / 2 = 30s per column.
   * Scroll 0.0 represents the oldest visible data point.
   *
   * @param scroll Scroll offset (0 = start of visible range).
   * @param zoom Zoom level (1 = 1 minute per column). Higher values = shorter intervals.
   * @param visibleIntervals Number of columns visible in the viewport.
   * @returns Parameters ready for GetValues() or GetValuesByParams().
   */
  public static GetTimeByScrollAndZoom(
    scroll: number,
    zoom: number,
    visibleIntervals: number
  ): TesterValuesFetchParams { return new TesterValuesFetchParams(); }
}
