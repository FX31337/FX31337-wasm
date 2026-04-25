import resolve from 'rollup-plugin-node-resolve';
import typescript from '@rollup/plugin-typescript';
import alias from '@rollup/plugin-alias';

export default {
  input: './lib/index.ts',
  plugins: [
    resolve(),
    typescript({
      tsconfig: './tsconfig.json',
    }),
  ],
  output: {
    file: 'index.js',
    format: 'cjs',
    exports: 'named',
  },
};
