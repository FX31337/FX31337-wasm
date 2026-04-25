import { EmStruct } from './EmStruct';

export class TickAB extends EmStruct {
  ask: number;
  bid: number;

  constructor() {
    super();
    this.ask = 0;
    this.bid = 0;
  }
}
