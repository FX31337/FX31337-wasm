import { TickAB } from './TickAB';

export class TickTAB extends TickAB {
  time_ms: number;

  constructor() {
    super();
    this.time_ms = 0;
  }
}
