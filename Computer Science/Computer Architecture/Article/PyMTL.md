Original paper: Lockhart, D., Zibrat, G., & Batten, C. (2014). PyMTL: A Unified Framework for Vertically Integrated Computer Architecture Research. Paper presented at the Proceedings of the 47th Annual IEEE/ACM International Symposium on Microarchitecture, Cambridge, United Kingdom. 

## Introduction

- PyMTL, a vertically-integrated framework for concurrent-structural modeling and hardware design. Unified environment for FL, CL and RTL. modeling -> to EDA toolflows
- Introduce and evaluate SimJIT, a selective embedded just-in-time specializer for PyMTL CL and RTL models. -> Improve simulations' exe time.

## Background

- Functional-Level: FL.  Not timing constraints. Exploring Algorithms, fast emulation of hardware targes, creating golden models for validation of CL and RTL models. -> Productivity-Level languages: Python, MATLAB -> rapid implementation and verification
- Cycle-Level: CL. Capture the cycle-approximate behavior. Balance between accuracy, performance, and flexibility. -> Efficiency-Level languages: C++ (Classic object oriented language) -> CA simulation frameworks
- Register-Transfer-Level: RTL. Cycle-accurate, resource-accurate -> Verilog/VHDL -> Simulator Generators, EDA tools (Synthesis), Verification Tools.

### Mechanisms for closing the methodology gap:

- Concurrent-Structural Frameworks: Mapping problem ( Use general sequential, object-oriented languages)
- Unified Modeling Languages: Reduces congnitive overhaed for designers
- Hardware Generation Languages: HGL, highly-parameterizable hardware templates: Chisel, Genesis II and Bluespec
- HDL Integration: Native cosimulation of Verilog/VHDL RTL with FL/CL models written in more flexible general-purpose languages.
- SEJITS: Selective embedded just-in-time specialization -> Enables efficiency-level language performance from productivity-level language
- Latency-Insensitive Interfaces: Interoperable FL, CL, and RTL models -> control abstraction through module-to-module stall communication.

## PyMTL: A Unified Framework enabling modeling towards layout

A proof-of-concept framework designed to provide a unified environment for constructing FL, CL, and RTL models.

*A. PyMTL models*

Concurrent-structural fashion

Inherit the model class and define in the __init__() method. Circuit's logic is defined using nested functions decorated with annotations:
combinational -> @s.combinational

FL, CL, and RTL sequential logic -> @s.tick_[fl/cl/rtl]

Combinational blocks are updated only when their sensitive list changes.

Sequential blocks are updated every simulator cycle.

Restricte rules that translate PyMTL models to synthesizable Verilog HDL:

(1) describe all behavioral logic within s.tick_rlt and s.combinational blocks.

(2) Use only a restricted, translatable subset of Python for logic blocks.

(3) Pass all data using ports or wires with fixed-bitwidth message types.

So BitStructs, PortBundles, and type inference to improve the succinctness and productivity

*B. PyMTL Tools*

Model and config are elaborated into a model instance. Tools (simulator, translator, and user tool) manipulate the model instance (act as in-memory representations) to simulate or translate the design to like trace(.vcd), verilog, and user tool output, which could put into the EDA flow.

SimulationTool: Inspects elaborated models to register concurrent logic blocks, sesnsitivity lists, connected ports, and generate optimized Python simulators.

Split model/tool -> moduler nature -> clean boundary between hardware modeling logic and simulator.

*C. PyMTL by Example: Accelerator Coprocessor*

Functional Level:

Only algorithm specified, integrating FL into architecture framework can be a challenge -> classic methodology gap

Protocols provide by proxies, which provide programmer-friendly interfaces.

ListMemPortAdapter -> reuse exist library-provided utility functions.

Cycle Level:

Memory: Issue and receive, simple pipeline scheme

Reuse FL componenets

Register-Transfer Level:

Estimate area, energy, and cycle time. High-level models is notoriously difficult

Providing a productive environment for constructing highly parameterizable RTL implementations,  reuse of FL and CL.

*D. PyMTL by Example: Mesh Network*

Mesh network which interconnect tiles

FL: Fast implement, reuse in CL, RTL. **Not timing, just functionality!!**

## SimJIT: Closing the Performance-Productivity Gap

Python -> Degrades simulation performance compare to C++ (Static)

SimJIT -> custom JIT specializer converting PyMTL model into optimized C++ code with PyPy meta-tracing  JIT interpreter.

PyMTL includes support for automatically caching the result from translation for SimJIT-RTL

*A. SimJIT Design:*

SimJIT-CL + SimJIT-RTL

SimJIT-CL : 

Elaborated PyMTL model (reflection, inspection) -> C++ function blocks source + C interface source -> LLVM/gcc -> C shared library -> PyMTL wrapper -> PyMTL CFFI model

SimJIT-RTL:

Translate PyMTL model to Verilog source (no optimization, directly) -> Verilator -> C++ source + C interface -> LLVM/gcc -> C shared library -> PyMTL wrapper -> PyMTL CFFI model

PyPy: meta-trace JIT interpreter, optimize the hot-paths between Python and C++ call interface, reducing the overhead of using Python component wrappers.

## Experiment:

Examine the performance benefits of SimJIT and PyPy using the Accelerator and Mesh network examples.

*Accelerator:*

method: Construct 27 different tile models at varying levels of detail by composing FL, CL and RTL implementations of the processor(P), caches(C), and accelerator(A).

<P, C, A> 3*3*3 = 27 cases. Simulated environment: CPython(Original), SimJIT+PyPy.

Exp1: Simulator Performance vs. LOD(Level of Detail). LOD = p + c + a, corresponding to the model complexity: FL = 1, CL = 2, RLT = 3. Performance: executiong time of a normalized against the execution time of a simple object-oriented instruction-set simulator implemented in Python and executed using PyPy.

Best case: <FL, FL, FL>, (1, 1)

Significant drop: <FL, FL, FL>, LOD = 3.

CPython -> more detailed models -> taking advantage of pipelining or parallelism

SimJIT+PyPy -> equally significant role: Effectiveness of each specialization strategy and the complexity of each component being specialized. FL(PyPy), CL and RTL(SimJIT+PyPy)

*Mesh Network:*

Speedup vs. Number of Cycles

FL network: CPython, PyPy

CL network: Cpython, PyPy, C++, SimJIT-CL, SimJIT-CL+PyPy

RTL network: CPython, PyPy, Verilator, SimJIT-RTL, SimJIT-RTL+PyPy

**elaboration time impact of less than a second** -> PyPy is significantly impact on the simulation, not compilation

CL network experiment: 

C++ implementation using an in-house (handwritten) concurrent-structural modeling framework -> like Liberty and Cascade -> Cycle-exact simulation

C++ simulation:

- Speed: 300x than CPython for 10M-cycle
- Compilation overhead is significant. -> significantly impacts the code-test-debug loop (Changing a dependent components)

In small simulation cycles (less than 1K, for debug), interpreted language perform better than the compiled language.

RTL network experiment:

Verilog network: Verilator + handwritten C++ test harness -> compile to create a simulator binary.

Detail nature of RTL simulation -> Python act worst than C++

But the compilation overhead of C++ simulation is significant (>5min for a 64-node mesh network, with -O1 GCC)

Speedup vs. Injection Rate (Workload)

Comparison to CPython: PyPy - consistent; SimJIT-[CL/RTL] - increased performance under greater load.

Faltten out near 30% injection rate -> Increased amount of execution time per simulation cycle -> spend more time on optimized C++ code

Overheads:

SimJIT-RTL and SimJIT-CL overheads mainly on compilation and verilation

PyPy slighty increase overhead but negligible compared to the significant speedups

SimJIT-RTL verilation phase overhead is significant, but due to the caching scheme is one-time cost

## Related Work

Stratus, PHDL, PyHDL -> Generate HDL only

MyHDL -> Generate HDL and simulated in Python interpreter.

SysPy -> Aid processor-centric SoC designs targeting FPGAs.

PDSDL -> Generate HDL, simulated whitin a Python-based kernel

Genesis2 -> SystemVerilog + Perl, highly parameterizable hardware designs

Chisel -> DSEL Scala, no scala simulations

BlueSpec -> SystemVerilog HGL

Simulation frameworks (based on cycle-level simulation):

Liberty Simulation Environment -> concurrent-structural modeling greatly improved understanding and reuse of components

Cascade -> concurrent-structural simulation framework used in the design and verification of the Anton supercomputers; Cascade models within Verilog test harnesses as well as Verilog components within Cascade models

SystemC: More used in HLS

JIT previously: ISS (GEZEL environemnt) -> custom interpreted DSL for coprocessor design with existing ISS. But not a general-purpose language

## Conclusion

PyMTL - Unified, Vertically-integrated framework for FL, CL, and RTL modeling.

SimJIT - Combined with PyPy
