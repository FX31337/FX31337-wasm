import resolve from 'rollup-plugin-node-resolve';
import babel from 'rollup-plugin-babel';
import typescript from 'rollup-plugin-typescript'
import alias from '@rollup/plugin-alias'

export default {
  input: './lib/index.js',
  plugins: [
    resolve(),
    babel({
      runtimeHelpers: true,
      exclude: 'node_modules/**' // only transpile our source code
    }),
    typescript(),
  ],
  output: {
    file: 'index.js',
    format: 'umd',
    name: 'fx31337-wasm'
  }
};
