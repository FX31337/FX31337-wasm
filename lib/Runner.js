class Test {
  /**
   * @param {import('./EALib.js').EALib} lib
   */
  run(lib) {

  }
}

async function runTest(what, libPath) {
  libPath = '../dist/' + libPath;
  const module = require(`${libPath}.js`);

  // WASM file will be loaded asynchronously.

  /** @type {import('./EALib.js')} */
  const lib = await module({wasmBinaryFile: `${libPath}.wasm`});

  lib.TickAB = require("./types/TickAB");
  lib.TickTAB = require("./types/TickTAB");

  const test = new what();
  test.run(lib);
}

module.exports = {
  Test,
  run(what, libPath) {
    runTest(what, libPath).then(() => {
    });
  }
};
