import { EmPtr } from './EmPtr';

export class CppArray<X> extends EmPtr {
  Push(value: X): void {}
  Size(): number { return 0; }
}
