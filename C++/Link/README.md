# 程序编译过程

cpp 预处理，通常生成后缀名为 .i 文件

cc1 编译，生成汇编文件，通常生成后缀名为 .s 的文件 （路径/usr/lib/gcc/x86_64-linux-gnu/7/cc1）

as 汇编器 ，生成可重定位的目标文件，通常后缀名为 .o 

ar 归档，可将多个.o文件归档成一个静态链接库，通常后缀名为 .a，

ld 链接器，可执行的目标文件



编译成库文件

静态库

```
gcc -c sum.c -o sum.o
ar rcs libsum.a sum.o
gcc -o main main.c -L. -lsum
```

**缺点**

- 每次库更新需要链接
- 对于标准库来说，常用函数的代码会被复制到每个进程的文本段，浪费内存资源

共享库 

也被称为共享目标，linux中常用 .so 后缀表示（多个进程共享代码段的单一副本，每个进程会有自己的读/写数据块）

```
gcc -shared -fpic -o libsum.so sum.c
gcc -o main main.c -L. -lsum
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./
```

*PIC（Position-Independent Code, 位置无关代码) *

传统静态链接

加载时的共享库的动态链接

动态链接器

运行时的共享库的动态链接

```
#include <dlfcn.h>

void *dlopen(const char*filename, int flag);

#include <dlfcn.h>
void *dlsym(void *handle, char *symbol);
```
