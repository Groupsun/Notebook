Original paper: Clow, J., Tzimpragos, G., Dangwal, D., Guo, S., McMahan, J., & Sherwood, T. (2017, September). A pythonic approach for rapid hardware prototyping and instrumentation. In 2017 27th International Conference on Field Programmable Logic and Applications (FPL) (pp. 1-7). IEEE.

## Introduction

Traditional HDL (Verilog, VHDL) -> still the dominant HDLs. Other approach -> HLS: gaining ground. Methods mention before: Still required deep hardware understanding (optimized solutions, etc.)

PyRTL: 

- Python based
- Rapid prototyping of complex digital hardware
- A collection of classes for pythonic RTL design, simulation, tracing and testing
- Simplicity, usability, clarity and extensibility.

Aims at:

- Lowering the barrier of entry to digital design.
- Promoting the co-design of hardware transforms and analysis with digital designs.
- Ultimately allowing complex hardware design patterns to be expressed in a way that promotes reuse beyond just hardware blocks.

PyRTL restrict users to a set of reasonable digital design practices. Small, well-defined internal core -> easy to add new functionality. Elaboration-through-execution (introspection). Import common HDLs.

Main contributions of this paper:

- Python embedded hardware design language: dynamic typing,introspection, comprehensions, etc.
- A concise IR simplifies the co-design of hardware transforms and analysis with digital designs.
- Instrumentation API similar to software-like.
- Integration of PyRTL-generated hardware overlays into Xilinx PYNQ.

## Related work

PyRTL: A complete tool chain. Variable types could be inferred during Python program execution. Hardware instrumentation framework -> walking and augmenting accelerator functionality concisely and efficiently.

## PyRTL Overview

Restricts users to a set of reasonable digital design practices. Clock and reset signals are implicit, BRAM are synchronous by default, no X or Z state, no unregistered feedbacks, etc.

Valid code always corresponds to synthesizable hardware. Dynamic, object-oriented -> common software abstractions

### *PyRTL Datatypes & Operators*

Primary data structure: `WireVector` - list of individual wires -> Python built-in function support (eg. `len()`, indexing, slicing, etc.)

```Python
wire2 = wire1[0]
wire3 = wire1[-1]
wire4 = wire1[:3]
```

`WireVector` length could be inferred from the producing operation or being assigned to.

Two different operator: 

- Assignment operator: `=`. No hardware create, name binding. Execute by Python runtime
- Connection operator: `<<=` 