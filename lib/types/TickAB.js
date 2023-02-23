const EmStruct = require("./EmStruct");

class TickAB extends EmStruct {
  constructor() {
    super();

    this.ask = 0;
    this.bid = 0;
  }
}

module.exports = TickAB;
