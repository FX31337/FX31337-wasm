import {EmPtr} from "./EmPtr";

export class CppArray<X> extends EmPtr {
  Push(value: X) {}

  Size(value: X): number {}
}
