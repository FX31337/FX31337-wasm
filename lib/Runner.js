import {TickAB} from "./types/TickAB";
import {TickTAB} from "./types/TickTAB";

export class Test {
  /**
   * @param {import('./index.js')} lib
   */
  run(lib) {
  }
}

export async function runTest(what, libPath) {
  const module = (await import(`${libPath}`)).default;

  // WASM file will be loaded asynchronously.

  /** @type {import('./index.js')} */
  const lib = await module({wasmBinaryFile: `${libPath}.wasm`});

  lib.TickAB = TickAB;
  lib.TickTAB = TickTAB;

  const test = new what();
  test.run(lib);
}

export function run(what, libPath) {
  runTest(what, libPath).then(() => {
  });
}
