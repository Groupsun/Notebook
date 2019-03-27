# ISA diversity

This note is drived from the paper "Harnessing ISA Diversity: Design of a Heterogeneous-ISA Chip Multiprocessor" ISCA 2014

ISA diversity include:

- Code Density
- Dynamic Instruction Count
- Register Pressure
- Support for Specialized Operations

## Code Density

High code density reduces the number of instruction cache misses, uses less energy and memory bandwidth for instruction fatch, and conserves power by enabling the use of smaller microarchitectural structures.

lowest density: Alpha, SPARC
 - RISC ISA with fixed-length instructions

middle density: x86-64, VAX
 - CISC ISA with variable-length instruction encoding

highest density: Thumb, AVR32
 - embedded ISA with code compression
 - packs two 16-bit instructions into one 32-bit instruction

## Dynamic Instruction Count

Dynamic instruction count - actual count of instruction executed by the cpu

Static instuction count - the count of instruction the program has(compiled)

example: loop statement

- Thumb: 2-operand instructions, lack the shift-modifier and predication support; Increases the dynamic instruction count.
- Alpha: 3-operand instructions, but with load-store architecture, no arithmetic instruction can directly operate on memory.
- x86-64: 2-operad instructions, with complex addressing modes that allow instructions to directly operate on memoery. x86-64 instructions are decoded into one or more simpler RISC-like uops, which increasing the number of dynamic instructions(uops) by about a factor of 1.3.

## Register Pressure

- Thumb: Reduced register set - only 8 32-bit programmable register for integer operations.
- Alpha: Two banks of 32 64-bits programmable registers, for integer and floating-point computation.
- x86-64: 16 64-bit registers for integer operations and 16 128-bit registers for floating-point and SIMD operations.

The number of programmable register is inversely proportional to the amount of register pressure.

register number: Alpha > x86-64 > Thumb

register pressure: Thumb > Alpha > x86-64

x86-64 has these memory addressing mode to reduce the number of register spills:

- Absolute memory addressing mode: Directly access memory operands, eliminating the need to allocate registers for temporary storeage of loaded values. 
- Sub-register addressing mode: Allows programmers to address 48 sub-registers to store/operate on smaller data types.
- Program counter relative addressing mode: Position-independent code without the overhead of a Global Offset Table.
- Register-to-register spills - Allow programmers(compilers) to spill general-purpose register to XMM register, minimizing the number of register spills into memory.

## Support for Specialized Operations - Floating-point and SIMD Support

- Thumb: No floating-point instructionis supported, so Thumb cores don't need to include floating-point instruction windows, register files, and functional units, resulting in up to 19.5% reduction in peak power and 30% savings in area.
- x86-64: Provides SIMD support through SSE/AVX extensions, making vectorization of loops and basic blocks possible.
- Alpha: MVI extension allows for only pack, unpack, amx, and min operations, forgo SIMD units in Alpha cores.
