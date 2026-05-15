/**
 * Platform-agnostic Emscripten build script.
 *
 * On Windows      : delegates to 'wsl bash' so emsdk runs inside WSL.
 * On CI (EMSDK set): emcc is already in PATH via setup-emsdk action; just runs make.
 * On Linux/macOS  : sources emsdk_env.sh from $HOME/emsdk, then runs make.
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

// Compile.
// When EMSDK is set (e.g. emscripten-core/setup-emsdk action), emcc is already
// in PATH — no need to source emsdk_env.sh again.
const makeCmd = process.env.EMSDK
  ? 'make -C src'
  : 'source ${HOME}/emsdk/emsdk_env.sh && make -C src';
bash(makeCmd);

// Move artifacts to dist/
if (!existsSync(dist)) mkdirSync(dist);
for (const file of readdirSync(src)) {
  if (/\.(obj|wasm|js)$/.test(file)) {
    renameSync(join(src, file), join(dist, file));
    console.log(`moved: src/${file} -> dist/${file}`);
  }
}
