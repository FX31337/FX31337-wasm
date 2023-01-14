import {TradeSignal} from "./TradeSignal";
import {EmSmartPtr} from "./EmSmartPtr";

export class TradeSignalManager extends EmSmartPtr {
  /**
   * @param {TradeSignal} tradeSignal
   */
  SignalAdd(tradeSignal);

  /**
   * @returns {string}
   */
  ToString();
}
