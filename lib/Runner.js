import {TickAB} from "./types/TickAB";
import {TickTAB} from "./types/TickTAB";

export class Test {
  /**
   * @param {import('./index.js')} lib
   */
  run(lib) {
  }
}

function set(schema, path, value, delimiter = '.') {
  const pList = path.split(delimiter);
  const len = pList.length;
  for (let i = 0; i < len - 1; i++) {
    const elem = pList[i];
    if (!schema[elem]) schema[elem] = {}
    schema = schema[elem];
  }

  schema[pList[len - 1]] = value;
}

/**
 * Credits: https://github.com/emscripten-core/emscripten/issues/5874#issuecomment-348141138
 *
 * Generates an object that corresponds to the namespaces in c++
 * if classes are exported like this
 * EMSCRIPTEN_BINDINGS(MyClass) {
 *  class_< a::b::c::MyClass >("a.b.c.MyClass")
 *  //..
 *  }
 */
function namespaces(module) {
  // final namespace
  const ns = {};

  // find all fields in module with replaced special chars
  // (all special characters seem to be replaced by $)
  const filtered = Object.keys(module).filter(k => k.indexOf('$') !== -1);

  for (let key of filtered)
    set(ns, key, module[key], '$');

  return ns;
}

export async function runTest(what, contentJs, contentWasm) {
  /** @type {import('./index.js')} */
  let lib = await contentJs({
    locateFile(path) {
      if (path.endsWith(`.wasm`)) {
        return contentWasm;
      }
      return path
    }
  });

  // Merging module with namespaced classes.
  lib = {...lib, ...namespaces(lib)};

  lib.TickAB = TickAB;
  lib.TickTAB = TickTAB;

  const test = new what();
  test.run(lib);
}

export function run(what, contentJs, contentWasm) {
  runTest(what, contentJs, contentWasm).then(() => {
  });
}
