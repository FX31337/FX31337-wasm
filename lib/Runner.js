import {TickAB} from "./types/TickAB";
import {TickTAB} from "./types/TickTAB";

export class Test {
  /**
   * @param {import('./index.js')} lib
   */
  run(lib) {
  }
}

export async function runTest(what, contentJs, contentWasm) {
  /** @type {import('./index.js')} */
  const lib = await contentJs({
    locateFile(path) {
      if (path.endsWith(`.wasm`)) {
        return contentWasm;
      }
      return path
    }
  });

  lib.TickAB = TickAB;
  lib.TickTAB = TickTAB;

  const test = new what();
  test.run(lib);
}

export function run(what, contentJs, contentWasm) {
  runTest(what, contentJs, contentWasm).then(() => {
  });
}
