#!/usr/bin/env python3
from datetime import datetime
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
import logging
import traceback


def tradesignal_setusererror(arg1):
    logger.debug(f"{arg1}")
    tradesignal_throw(arg1)


def tradesignal_throw(arg1):
    logger.debug(f"{arg1}")
    throw(TypeError, arg1)


# Drops into the debugger.
def tradesignal_debugbreak():
    logger.debug("todo")
    traceback.print_exc()
    breakpoint()


# Unknown function called.
def tradesignal_todo(arg1, arg2=0, arg3=0, arg4=0):
    logger.debug(f"{arg1}, {arg2}, {arg3}, {arg4}")
    traceback.print_exc()
    breakpoint()


# Configure logging.
logging.basicConfig(
    format="%(asctime)s,%(msecs)d %(levelname)-8s [%(filename)s:%(lineno)d] %(message)s",
    datefmt="%Y-%m-%d:%H:%M:%S",
    level=logging.DEBUG,
)
logger = logging.getLogger(__name__)

# Create a store.
store = Store()

# Register imports inside the env namespace.
import_obj = ImportObject()
import_obj.register(
    "env",
    {
        "_Z10DebugBreakv": Function(store, tradesignal_debugbreak),
        "_Z11ArrayResizeI8DictSlotIi11TradeSignalEEiR10_cpp_arrayIT_Eii": Function(
            store,
            tradesignal_todo,
            FunctionType([Type.I32, Type.I32, Type.I32], [Type.I32]),
        ),
        "_Z11ArrayResizeIP14SerializerNodeEiR10_cpp_arrayIT_Eii": Function(
            store,
            tradesignal_todo,
            FunctionType([Type.I32, Type.I32, Type.I32], [Type.I32]),
        ),
        "_Z12SetUserErrort": Function(
            store, tradesignal_setusererror, FunctionType([Type.I32], [])
        ),
        "_Z12StringFormatIJdEENSt3__212basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEERKS6_DpT_": Function(
            store, tradesignal_todo, FunctionType([Type.I32, Type.I32, Type.F64], [])
        ),
        "_Z13ShortToStringt": Function(
            store, tradesignal_todo, FunctionType([Type.I32, Type.I32], [])
        ),
        "_Z14StringToDoubleNSt3__212basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE": Function(
            store, tradesignal_todo, FunctionType([Type.I32], [Type.F64])
        ),
        "_Z15IntegerToStringlit": Function(
            store,
            tradesignal_todo,
            FunctionType([Type.I32, Type.I32, Type.I32, Type.I32], []),
        ),
        "_Z15StringToIntegerNSt3__212basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE": Function(
            store, tradesignal_todo, FunctionType([Type.I32], [Type.I32])
        ),
        "_Z18StringGetCharacterNSt3__212basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEi": Function(
            store, tradesignal_todo, FunctionType([Type.I32, Type.I32], [Type.I32])
        ),
        "_Z5AlertIJPKcEEvDpT_": Function(
            store, tradesignal_throw, FunctionType([Type.I32], [])
        ),
        "_Z5PrintIJPKcEEvDpT_": Function(store, print, FunctionType([Type.I32], [])),
        "_Z7MathMaxIiET_S0_S0_": Function(
            store, tradesignal_todo, FunctionType([Type.I32, Type.I32], [Type.I32])
        ),
        "_Z7MathMinIiET_S0_S0_": Function(
            store, tradesignal_todo, FunctionType([Type.I32, Type.I32], [Type.I32])
        ),
        "_Z9ArrayFreeI8DictSlotIi11TradeSignalEEiRK10_cpp_arrayIT_E": Function(
            store, tradesignal_todo, FunctionType([Type.I32], [Type.I32])
        ),
        "_Z9ArraySizeI8DictSlotIi11TradeSignalEEiRK10_cpp_arrayIT_E": Function(
            store, tradesignal_todo, FunctionType([Type.I32], [Type.I32])
        ),
        "_Z9ArraySizeIP14SerializerNodeEiRK10_cpp_arrayIT_E": Function(
            store, tradesignal_todo, FunctionType([Type.I32], [Type.I32])
        ),
        "_Z9StringLenNSt3__212basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE": Function(
            store, tradesignal_todo, FunctionType([Type.I32], [Type.I32])
        ),
        "__cxa_allocate_exception": Function(
            store, tradesignal_todo, FunctionType([Type.I32], [Type.I32])
        ),
        "__cxa_throw": Function(
            store, tradesignal_throw, FunctionType([Type.I32, Type.I32, Type.I32], [])
        ),
        "abort": Function(store, tradesignal_throw, FunctionType([], [])),
        "emscripten_memcpy_big": Function(
            store,
            tradesignal_todo,
            FunctionType([Type.I32, Type.I32, Type.I32], [Type.I32]),
        ),
        "emscripten_resize_heap": Function(
            store, tradesignal_todo, FunctionType([Type.I32], [Type.I32])
        ),
        "setTempRet0": Function(store, tradesignal_todo, FunctionType([Type.I32], [])),
    },
)
import_obj.register(
    "wasi_snapshot_preview1",
    {
        "fd_close": Function(
            store, tradesignal_todo, FunctionType([Type.I32], [Type.I32])
        ),
        "fd_seek": Function(
            store,
            tradesignal_todo,
            FunctionType(
                [Type.I32, Type.I32, Type.I32, Type.I32, Type.I32], [Type.I32]
            ),
        ),
        "fd_write": Function(
            store,
            tradesignal_todo,
            FunctionType([Type.I32, Type.I32, Type.I32, Type.I32], [Type.I32]),
        ),
    },
)

# Instantiate the module.
module = Module(store, open("../../dist/TradeSignalManager.wasm", "rb").read())
instance = Instance(module, import_obj)
memory = instance.exports.memory
assert isinstance(memory, Memory)
# Congrats, the Wasm module is cross-compiled!

print("Exported objects:")
for export in instance.exports:
    print(export)

print("Testing...")
assert instance.exports.sum(1, 2) == 3
result = instance.exports.test()
instance.exports.main(0, 0)
