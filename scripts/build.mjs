/**
 * Platform-agnostic Emscripten build script.
 *
 * On Windows  : delegates to 'wsl bash' so emsdk runs inside WSL.
 * On Linux/macOS: runs '/bin/bash' directly (emsdk expected at ~/emsdk).
 *
 * After compilation the generated .obj/.wasm/.js artifacts are moved
 * from src/ to dist/ (replacing the old 'postbuild' npm hook).
 */
import { spawnSync } from 'child_process';
import { platform } from 'os';
import { readdirSync, renameSync, existsSync, mkdirSync } from 'fs';
import { join, dirname, resolve } from 'path';
import { fileURLToPath } from 'url';

const root = resolve(dirname(fileURLToPath(import.meta.url)), '..');
const src  = join(root, 'src');
const dist = join(root, 'dist');

function bash(cmd) {
  const [bin, args] = platform() === 'win32'
    ? ['wsl', ['bash', '-c', cmd]]
    : ['/bin/bash', ['-c', cmd]];

  const result = spawnSync(bin, args, { stdio: 'inherit', cwd: root });
  if (result.error) throw result.error;
  if (result.status !== 0) process.exit(result.status ?? 1);
}

// Compile
bash('source ~/emsdk/emsdk_env.sh && make -C src');

// Move artifacts to dist/
if (!existsSync(dist)) mkdirSync(dist);
for (const file of readdirSync(src)) {
  if (/\.(obj|wasm|js)$/.test(file)) {
    renameSync(join(src, file), join(dist, file));
    console.log(`moved: src/${file} -> dist/${file}`);
  }
}
