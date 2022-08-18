#!/usr/bin/env python3
from wasmer import (
    Function,
    FunctionType,
    ImportObject,
    Instance,
    Memory,
    Module,
    Store,
    Type,
)
import datetime


def taskrunner_setusererror(arg1):
    taskrunner_throw(arg1)


def taskrunner_throw(arg1):
    print(__name__)
    throw(TypeError, arg1)


def taskrunner_datetime(arg1):
    print(__name__)
    return arg1


def taskrunner_timecurrent():
    return datetime.now()


# Drops into the debugger.
def taskrunner_debugbreak():
    breakpoint()


# Unknown function called.
def taskrunner_todo(arg1, arg2=0, arg3=0):
    print(__name__)
    breakpoint()


# Create a store.
store = Store()

# Register imports inside the env namespace.
import_obj = ImportObject()
import_obj.register(
    "env",
    {
        "_Z11ArrayResizeI8DictSlotIs9TaskEntryEEiR10_cpp_arrayIT_Eii": Function(
            store,
            taskrunner_todo,
            FunctionType([Type.I32, Type.I32, Type.I32], [Type.I32]),
        ),
        "_Z5AlertIJPKcEEvDpT_": Function(
            store, taskrunner_todo, FunctionType([Type.I32], [])
        ),
        "_Z7MathMaxIiET_S0_S0_": Function(
            store, taskrunner_todo, FunctionType([Type.I32, Type.I32], [Type.I32])
        ),
        "_Z7MathMinIiET_S0_S0_": Function(
            store, taskrunner_todo, FunctionType([Type.I32, Type.I32], [Type.I32])
        ),
        "_Z9ArrayFreeI8DictSlotIs9TaskEntryEEiRK10_cpp_arrayIT_E": Function(
            store, taskrunner_todo, FunctionType([Type.I32], [Type.I32])
        ),
        "_Z9ArraySizeI8DictSlotIs9TaskEntryEEiRK10_cpp_arrayIT_E": Function(
            store, taskrunner_todo, FunctionType([Type.I32], [Type.I32])
        ),
        "_ZN8datetimeC1ERKi": Function(
            store,
            taskrunner_timecurrent,
            FunctionType([Type.I32, Type.I32], [Type.I32]),
        ),
        "_ZN8datetimeC1Ev": Function(
            store, taskrunner_timecurrent, FunctionType([Type.I32], [Type.I32])
        ),
        "_Z10DebugBreakv": Function(store, taskrunner_debugbreak),
        "_Z11TimeCurrentv": Function(
            store, taskrunner_timecurrent, FunctionType([], [Type.I32])
        ),
        "_Z12SetUserErrort": Function(
            store, taskrunner_setusererror, FunctionType([Type.I32], [])
        ),
        "_Z12StringFormatIJP6ObjectEENSt3__212basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEERKS8_DpT_": Function(
            store, taskrunner_todo, FunctionType([Type.I32, Type.I32, Type.I32], [])
        ),
        "_Z12StringFormatIJPKcEENSt3__212basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEERKS8_DpT_": Function(
            store, taskrunner_todo, FunctionType([Type.I32, Type.I32, Type.I32], [])
        ),
        "_ZNK8datetimecvlEv": Function(
            store, taskrunner_datetime, FunctionType([Type.I32], [Type.I32])
        ),
        "__cxa_allocate_exception": Function(
            store, taskrunner_todo, FunctionType([Type.I32], [Type.I32])
        ),
        "__cxa_throw": Function(
            store, taskrunner_throw, FunctionType([Type.I32, Type.I32, Type.I32], [])
        ),
        "abort": Function(store, taskrunner_throw, FunctionType([], [])),
        "emscripten_memcpy_big": Function(
            store,
            taskrunner_todo,
            FunctionType([Type.I32, Type.I32, Type.I32], [Type.I32]),
        ),
        "emscripten_resize_heap": Function(
            store, taskrunner_todo, FunctionType([Type.I32], [Type.I32])
        ),
        "setTempRet0": Function(store, taskrunner_todo, FunctionType([Type.I32], [])),
    },
)
import_obj.register(
    "wasi_snapshot_preview1",
    {
        "fd_close": Function(
            store, taskrunner_todo, FunctionType([Type.I32], [Type.I32])
        ),
        "fd_seek": Function(
            store,
            taskrunner_todo,
            FunctionType(
                [Type.I32, Type.I32, Type.I32, Type.I32, Type.I32], [Type.I32]
            ),
        ),
        "fd_write": Function(
            store,
            taskrunner_todo,
            FunctionType([Type.I32, Type.I32, Type.I32, Type.I32], [Type.I32]),
        ),
    },
)

# Instantiate the module.
module = Module(store, open("../../dist/TaskRunner.wasm", "rb").read())
instance = Instance(module, import_obj)
memory = instance.exports.memory
assert isinstance(memory, Memory)
# Congrats, the Wasm module is cross-compiled!

print("Exported objects:")
for export in instance.exports:
    print(export)