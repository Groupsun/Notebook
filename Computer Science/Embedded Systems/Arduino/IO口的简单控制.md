# Arduino I/O口的简单控制

## 数字I/O口的使用

### 数字信号

在使用输入或输出功能之前，需要先通过pinMode()函数配置引脚的模式为输入模式或输出模式：

```C
pinMode(pin, mode);
```

其中Arduino引脚可配置的模式有：

<table>
    <tr>
        <th>模式名称</th>
        <th>说明</th>
    </tr>
    <tr>
        <th>INPUT</th>
        <th>输入模式</th>
    </tr>
    <tr>
        <th>OUTPUT</th>
        <th>输出模式</th>
    </tr>
    <tr>
        <th>INPUT_PULLUP</th>
        <th>输入上拉模式</th>
    </tr>
</table>

使用digitalWrite()函数使该引脚输出高低电平信号：

```C
digitalWrite(pin, value);
```

其中value值代表高低电平信号，HIGH指定输出高电平，LOW指定输出低电平。在Arduino中输出的低电平为0V，高电平为当前Arduino的工作电压。

使用digitalRead()函数读取外部输入的数字信号：

```C
digitalRead(pin);
```

输入电压识别不同的Arduino元件有不同的范围，例如5V供电时，将-0.5V ~ 1.5V的输入电压作为低电平识别，而将范围在3 ~ 5.5V的输入电压作为高电平识别。

注：所有实验代码均使用nrf51822的BLE板子来进行测试。

简单点亮LED灯的实验：

```C
void setup()
{
	pinMode(D13, OUTPUT);   // D13在Arduino中通常指代LED灯
}

void loop() 
{
	digitalWrite(D13, HIGH);
	delay(1000);
	digitalWrite(D13, LOW);
	delay(1000);
}
```

### 流水灯实验

```C
// 使用P01 ~ P05引脚连接LED0 ~ LED4
int pinID = A0;

void setup()
{
	for(int i = pinID;i <= A4;i++)
		pinMode(i, OUTPUT);
}

void loop() 
{
	for(int i = pinID;i <= A4;i++)
	{
		digitalWrite(i, HIGH);
		delay(1000);
		digitalWrite(i, LOW);
		delay(1000);
	}
}
```

### 按键控制LED实验

```C
void setup()
{
	// 配置为上拉按钮，按钮接通时为低电平
	pinMode(A0, INPUT_PULLUP);	// A0连接按键产生的电平信号
	pinMode(A1, OUTPUT);		// A1输出高低电平
}

void loop() 
{
	int value = digitalRead(A0);
	if(value == LOW)
	{
		digitalWrite(A1, HIGH);
		delay(200);
		digitalWrite(A1, LOW);
		delay(200);
	}else digitalWrite(A1, LOW);
}
```
