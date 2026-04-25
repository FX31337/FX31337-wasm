import { IndicatorData } from './Indicators/IndicatorData';
import { Indi_TickProvider } from './Indicators/Indi_TickProvider';
export declare class Tester {
    static Add(indicator: IndicatorData): void;
    static AddPlatformWise(indicator: IndicatorData, symbol: string, tf: number): void;
    static FeedTickProvider(tickProvider: Indi_TickProvider): void;
    static RunAllTicks(): void;
    static RunTick(): boolean;
}
