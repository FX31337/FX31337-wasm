import { TradeSignal } from './TradeSignal';
import { EmSmartPtr } from './EmSmartPtr';
export declare class TradeSignalManager extends EmSmartPtr {
    SignalAdd(tradeSignal: TradeSignal): void;
    ToString(): string;
}
