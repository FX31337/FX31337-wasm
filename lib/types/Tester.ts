import { IndicatorData } from './Indicators/IndicatorData';
import { Indi_TickProvider } from './Indicators/Indi_TickProvider';

export class Tester {
  public static Init(): void {}
  public static Add(indicator: IndicatorData): void {}
  public static AddPlatformWise(indicator: IndicatorData, symbol: string, tf: number): void {}
  public static FeedTickProvider(tickProvider: Indi_TickProvider): void {}
  public static RunAllTicks(): void {}
  public static RunTick(): boolean { return false; }
}
