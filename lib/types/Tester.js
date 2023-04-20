import {IndicatorData} from "./Indicators/IndicatorData";
import {Indi_TickProvider} from "./Indicators/Indi_TickProvider";

export class Tester {
  static Add(indicator: IndicatorData) {}
  static AddPlatformWise(indicator: IndicatorData, symbol: string, tf) {}
  static FeedTickProvider(tickProvider: Indi_TickProvider) {}
  static RunAllTicks() {}
  static RunTick(): boolean {}
}
