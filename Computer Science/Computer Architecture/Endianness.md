在阅读Cortex-M4架构文档时，在2.2.6关于M4的存储大小端模型的说明中，有提到M4支持两种大小端格式：

- Byte-invariant big-endian format
- Little-endian format

后者的小端格式很简单，但是前者的Byte-invariant指什么？通过查阅资料发现这个[回答](https://stackoverflow.com/a/61730/8415525)说明的比较详细，这里进行适当的翻译以及理解。

------

大小端格式的映射有两种方式：*address invariance* 以及 *data invariance*

# Address Invariance

Address invariance 有时候也称之为 *byte invariance* ，也就是M4架构当中的第一种格式。在这种模型下，地址是固定的，也就是说从特定地址读取的数据必定是从+0的偏移开始算起，而应用大小端则是从读取的数据中进行。

## 举例

字节地址固定但字节的LSB以及MSB是倒转的：

```
Addr   Memory
       7    0
       |    |    (LE)   (BE)
       |----|
 +0    | aa |    lsb    msb
       |----|
 +1    | bb |     :      :
       |----|
 +2    | cc |     :      :
       |----|
 +3    | dd |    msb    lsb
       |----|
       |    |

At Addr=0:          Little-endian          Big-endian
Read 1 byte:              0xaa                0xaa   (preserved)
Read 2 bytes:           0xbbaa              0xaabb
Read 4 bytes:       0xddccbbaa          0xaabbccdd
```

在这种情况下，当且仅当只对同一个地址读取**一个字节**时，大小端映射的结果是**相同**的。

# Data Invariance

在这种模型下，大小端应用的依据不是地址，而是读取的位宽（*datum*）。

## 举例1

32位的 data invariance （也可以称之为 *word invariance*）：

```
Addr                Memory

            | +3   +2   +1   +0 |  <- LE
            |-------------------|
+0      msb | dd | cc | bb | aa |  lsb
            |-------------------|
+4      msb | 99 | 88 | 77 | 66 |  lsb
            |-------------------|
     BE ->  | +0   +1   +2   +3 |


At Addr=0:             Little-endian              Big-endian
Read 1 byte:                 0xaa                    0xdd
Read 2 bytes:              0xbbaa                  0xddcc
Read 4 bytes:          0xddccbbaa              0xddccbbaa   (preserved)
Read 8 bytes:  0x99887766ddccbbaa      0x99887766ddccbbaa   (preserved)
```

## 举例2

16位的 data invariance（也可以称之为 *half-word invariance*）：

```
Addr           Memory

            | +1   +0 |  <- LE
            |---------|
+0      msb | bb | aa |  lsb
            |---------|
+2      msb | dd | cc |  lsb
            |---------|
+4      msb | 77 | 66 |  lsb
            |---------|
+6      msb | 99 | 88 |  lsb
            |---------|
     BE ->  | +0   +1 |


At Addr=0:             Little-endian              Big-endian
Read 1 byte:                 0xaa                    0xbb
Read 2 bytes:              0xbbaa                  0xbbaa   (preserved)
Read 4 bytes:          0xddccbbaa              0xddccbbaa   (preserved)
Read 8 bytes:  0x99887766ddccbbaa      0x99887766ddccbbaa   (preserved)
```

## 举例3

64位的 data invariance（也可以称之为 *double-word invariance*）：

```
Addr                         Memory

            | +7   +6   +5   +4   +3   +2   +1   +0 |  <- LE
            |---------------------------------------|
+0      msb | 99 | 88 | 77 | 66 | dd | cc | bb | aa |  lsb
            |---------------------------------------|
     BE ->  | +0   +1   +2   +3   +4   +5   +6   +7 |


At Addr=0:             Little-endian              Big-endian
Read 1 byte:                 0xaa                    0x99
Read 2 bytes:              0xbbaa                  0x9988
Read 4 bytes:          0xddccbbaa              0x99887766
Read 8 bytes:  0x99887766ddccbbaa      0x99887766ddccbbaa   (preserved)
```

总结来说，在这种情况下，当读取的位宽大于或者等于 data invariance 所要求的datum时，大小端映射的结果是相同的。





