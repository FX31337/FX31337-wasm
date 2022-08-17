#!/usr/bin/env python3
from wasmer import Store, Module, Instance, Memory

store = Store()
module = Module(store, open('../../dist/TaskRunner.wasm', 'rb').read())
instance = Instance(module)

memory = instance.exports.memory

assert isinstance(memory, Memory)
