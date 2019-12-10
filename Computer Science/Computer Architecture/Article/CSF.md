# Context-Sensitive Fencing: Securing Speculative Execution via Microcode Customization

## Introduction

Key components:

- A **microcode customization** mechanism that allows processors to surgically insert fences into the dynamic instruction stream to mitigate undesirable side-effects of speculative execution.
- A **decoder-level information flow tracking** (DLIFT) framework that identifies potentially unsafe execution patterns to trigger microcode customization.
- **Mistraining mitigations** that secure the branch predictor and return address stack.

Minimal impact on performance -> context-sensitive decoding (CSD)

Contributions:

- Context-sensitive fencing: decoding architecture (decoder level) -> no recompilation or binary translation
- Decoder-level dynamic information flow tracker (DLIFT)
- Reduce the cost of a fence-based Spectre mitigation technique from 48% overhead to less than 8%
- Decode-level branch predictor isolation -> mitigates branch mis-training

## Background and Related Work

### Speculative Execution (投机执行/预测执行)

Expecially in out-of-order processors (乱序处理器) -> 译码为条件分支(conditional branch)的指令得到实际分支结果前需要数个甚至数十个时钟周期(取决于流水线的深度)，在这段期间处理器会使用特定的策略来预测分支的路径并提前执行(否则流水线只能冒泡损失大量的Throughput)。如果预测失败就要回滚到正确的指令路径上执行，对于没有commit的指令来说其实不是很大的问题(架构状态没有改变即可)，但是由于cache的存在实际上可以通过一些旁路手段来获取到越权的信息。

### Microarchitectural Attacks

主要：Cache-based side-channel attacks. 典型的方法：缓冲时延(Cache timing).