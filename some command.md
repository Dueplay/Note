#### gcc

##### 编译流程

源代码通过编译器编译生成汇编代码

汇编代码通过汇编器翻译为机器码，生成目标文件

链接器将目标文件与任何必需的库文件（如标准库）组合在一起，解析符号引用，生成最终的可执行文件。

##### 单步执行编译过程

运行预处理器，生成预处理后的代码

```bash
gcc -E hello.c
```

运行编译器，生成汇编代码hello.s文件

```bash
gcc -S hello.c
```

运行汇编器，生成目标文件hello .o文件

```bash
gcc -c hello.s
```

运行链接器ld，链接目标文件到可执行文件,默认输出为a.out(assembler汇编程序的输出)

```bash
ld hello.o
```

在ld hello.o时，发生以下错误

```bash
ld: warning: cannot find entry symbol _start; defaulting to 0000000000401000
ld: hello.o: in function `main':
hello.c:(.text+0x13): undefined reference to `puts'
```

1. **未找到入口符号（entry symbol）**：ld 发出警告说它找不到程序的入口点 `_start`，并默认使用了一个地址。在大多数情况下，可执行文件的入口点应该是 `main` 函数。你可以通过指定入口点来解决这个问题。
2. **`puts` 函数的未定义引用**：ld 报告了 `undefined reference to 'puts'` 错误，这表示程序引用了 `puts` 函数，但没有找到该函数的定义。你需要链接标准C库以解决这个问题。

要解决这两个问题，你可以使用以下命令：

ld  hello.o -e main -lc

- `-e main`：指定程序的入口点是 `main` 函数。
- `-lc`：链接标准的C库（libc）以解决对 `puts` 等标准C库函数的引用问题。



`-static`：静态链接，不使用动态链接库。生成文件很大

#### vim

1.全选文本剪切和删除

在normal模式（Esc），

`ggvG`将光标移到文件的开头，然后选择整个文件。

`x` 剪切选中的文本

`d`删除选中的文本

2.剪切行：按下 `dd` 键。这将删除并剪切当前行。

 `yy` 来复制当前行，然后使用 `p` 将其粘贴、

3.`set nowrap` 超出屏幕一行的文字不会换行

`set wrap` 取消不换行的设置

`set nu` 显示行号

4.修改单个单词 `cw`  `ciw` 

5.全部格式化 `gg=G`

#### objdump查看二进制文件

查看目标文件反汇编代码 `-d`反汇编的意思Disassembly

```bash
objdump -d xx.o
```

#### strace追踪执行的系统调用

strace的输出位标准错误输出

- 主要的系统调用：execve, read, write

- ```
  strace -f gcc a.c
  ```

  (gcc 会启动其他进程)

  - 可以管道给编辑器 `vim -`
  - 编辑器里还可以 `%!grep` (细节/技巧)

```bash
strace ls |& grep -e read -e write
```

可以实现系统调用的过滤等，-e read -e write只输出包含read，write的行

grep反向匹配：使用 `-v` 选项查找不匹配模式的行。

使用 `-o` 选项只显示匹配的文本，而不是整行。

#### diskpart

```powershell
diskpart
list disk
select disk X  # 替换 X 为你的目标磁盘编号
create partition primary size=102400  # 创建一个主分区，大小为100GB
format fs=ntfs quick
format fs=fat32 unit=4096 quick
assign letter=D
exit
# 列出磁盘上的分区
list partition
# 选择新创建的分区
select partition Y
detail partition
# 查看disk详细信息
detail disk
```

