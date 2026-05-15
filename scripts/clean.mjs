/**
 * Platform-agnostic clean script.
 * Removes Emscripten build artifacts (.o .obj .wasm .js) from src/.
 */
import { readdirSync, unlinkSync, existsSync } from 'fs';
import { join, dirname, resolve } from 'path';
import { fileURLToPath } from 'url';

const src = resolve(dirname(fileURLToPath(import.meta.url)), '..', 'src');

if (!existsSync(src)) process.exit(0);

let count = 0;
for (const file of readdirSync(src)) {
  if (/\.(o|obj|wasm|js)$/.test(file)) {
    unlinkSync(join(src, file));
    console.log(`removed: src/${file}`);
    count++;
  }
}
console.log(count ? `Cleaned ${count} file(s).` : 'Nothing to clean.');
