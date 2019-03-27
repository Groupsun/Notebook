# Java中的API

## java.util.Scanner 
用于控制台中输入与输出数据

输入数据：
```Java
Scanner in=new Scanner(System.in);
int n=in.nextInt();
```
next()方法遇到空格就会停止读入；
读入一行字符串使用nextLine()。

## java.text.DecimalFormat
用于格式化输出数据
```Java
DecimalFormat fix=DecimalFormat("0.00");
fix.format(12.345567);  //输出2位小数（已经四舍五入）
```

## java.util.Arrays
用于将一个数组中的数据进行排序（使用快速排序和归并排序）
```Java
static void sort(int[] a)
int[] a=new int[100];
Arrays.sort(a);
```

## Math库
该库已经包含在java.lang中，不需要进行import操作。
该库中的API有：
```Java
static double abs(double a)//a的绝对值
static double max(double a,double b)//a和b中的较大者
static double min(double a,double c)//a和b 中的较小者
static double sin(double theta)//正弦函数
static double cos(double theta)//余弦函数
static double tan(double theta)//正切函数
static double exp(double a)//整数函数e^a
static dobule log(double a)//自然对数ln a
static double pow(double a,double b)//求a的b次方
static double random()//[0,1)之间的随机数
static double sqrt(double a)//a的平方根
static double E//常数e
static double PI//圆周率
```

## String值和数字之间相互转换的API
```Java
public class Interger;
static int parseInt(String s);
static String toString (int i);
```

## 向文件中输入数据
```Java
import java.io.PrintStream;
import java.io.FileNotFoundException;
try{    //相关代码
    PrintStream pt=new PrintStream("//输出的text文件地址");
    System.setOut(pt);
}
catch(FileNotFoundException e)
{
     e.printStackTrace();
}
```
