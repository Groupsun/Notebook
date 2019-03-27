# Scoreboarding

计分板算法是在CDC 6600计算机中使用的一种动态调度流水线的算法。通过这个算法，在没有冲突以及硬件可用的情况下，指令可以乱序执行。

在计分板算法当中，每条指令的数据依赖性都会被记录。计分板机制会记录、分析不同指令之间的数据相关性。只有当一条指令与之前已发射（issue）的指令之间的冲突消失之后，这条指令才会被发射、执行。计分板会监视正在执行的指令，如果某条指令由于数据冒险而停顿，那么在所有数据相关性造成的冲突化解之后会通知停顿的指令开始执行。

## 阶段

在指令译码之后，会经过一下四个阶段：

1. 发射（issue）：系统会检查指令会读取以及写入哪个寄存器。这个信息会被记录下来并且在接下来的阶段中会使用到。为了防止输出依赖性（WAW），指令会在需要写同一个寄存器的先前另一条指令执行完毕之前停滞。指令也会在功能器件忙的时候停滞。

2. 读取操作数（Read operands）：当一个指令发射并且正确分配到所需要的硬件模块之后，指令等待所有的操作数可用。这个阶段解决读取依赖性（RAW），因为只有当先前的指令将结果写入到所需要的寄存器中之前，该寄存器中的数据都是不可用的。

3. 执行（Execution）：当所有的操作数都被获取之后，功能器件开始执行指令。当指令的结果产生后，计分板会被提醒。

4. 回写（Write result）：在这个阶段当中，结果准备写入到它的目标寄存器。然而，这个动作会被先前的想要读取同样寄存器的指令所停滞，直到先前的读指令经过读取操作数的阶段之后，当前指令的回写操作才回执行。这样的做法可以防止WAR数据冒险。

在顺序执行的处理器当中，可以使用旁路的方法解决数据冒险。

## 数据结构

为了控制指令的执行，计分板算法维护着3种状态表：

- 指令状态表（Instruction Status）：标志每条被执行的指令在处于4个阶段中的哪一个。
- 功能部件状态表（Functional Unit Status）：标志每个功能部件的状态。每个功能部件维护9个域：
    - Busy：标志该部件是否可用
    - Op：部件要执行的操作（MUL、DIV还是MOD）
    - Fi：目标寄存器
    - Fj、Fk：源寄存器
    - Qj、Qk：对源寄存器Fj、Fk进行操作的功能部件
    - Rj、Rk：标志Fj、Fk是否可用
- 寄存器状态（Register Status）：标志每个寄存器的状态，哪个功能比较想要写入寄存器。

## 详细算法描述

计分板的详细算法描述如下所示：

```
function issue(op, dst, src1, src2)
    wait until (!Busy[FU] AND !Result[dst]);    // FU可以是任何执行op的功能部件
    Busy[FU] ← Yes;
    Op[FU] ← op;
    Fi[FU] ← dst;
    Fj[FU] ← src1;
    Fk[FU] ← src2;
    Qj[FU] ← Result[src1];
    Qk[FU] ← Result[src2];
    Rj[FU] ← Qj[FU] == 0;
    Rk[FU] ← Qk[FU] == 0;
    Result[dst] ← FU;

function read_operands(FU)
    wait until (Rj[FU] AND Rk[FU]);
    Rj[FU] ← No;
    Rk[FU] ← No;

function execute(FU)
    // 执行FU的操作

function write_back(FU)
    wait until (∀f {(Fj[f]≠Fi[FU] OR Rj[f]=No) AND (Fk[f]≠Fi[FU] OR Rk[f]=No)})
    foreach f do
        if Qj[f]=FU then Rj[f] ← Yes;
        if Qk[f]=FU then Rk[f] ← Yes;
    Result[Fi[FU]] ← 0; // 0表示没有FU产生寄存器的结果
    Busy[FU] ← No;
```

## 备注

计分板算法在没有功能部件可用的时候必须停滞当前指令。在这种情况下，之后的指令可能会需要在该结构冒险解决后才能继续执行。一些其他的方法比如托马苏洛算法可以防止结构冒险以及通过寄存器重命名的方法解决WAR以及WAW依赖。
