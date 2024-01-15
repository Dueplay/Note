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

#### 输出重定向

- **标准输入（STDIN）**：通常表示为数字 `0`，
- **标准输出（STDOUT）**：通常表示为数字 `1`，是大多数程序输出其执行结果的默认位置。
- **标准错误（STDERR）**：表示为数字 `2`，用于输出错误信息。

在命令行中，`>` 符号用于重定向输出。`2>&1` 的意思是将标准错误（`2`）重定向到标准输出（`1`）的同一位置。这通常用于将程序的所有输出（包括错误和正常信息）合并到一个流中，便于进一步处理或记录。

```shell
# 标准输出重定向到file.txt 
command > file.txt # 等同于command 1> file.txt， 1通常省略
command > file.txt 2>&1
# 有的时候我们不想看到输出信息，也不想保存输出信息，可以考虑dev下的null设备文件，所有写进去的数据都会被丢弃。
command > /dev/null 2>&1
```

这里的 `command` 可以是任何命令、脚本或程序。这个命令做了以下两件事：

1. `> file.txt` 将 `command` 的标准输出重定向到 `file.txt`。如果 `file.txt` 不存在，它将被创建；如果已存在，它将被覆盖。
2. `2>&1` 将 `command` 的标准错误重定向到标准输出的地方，即 `file.txt`。

结果，无论 `command` 生成的是标准输出还是错误输出，所有内容都会被写入 `file.txt`。

这种重定向在脚本编写和日志记录中非常有用，特别是当你需要捕获和分析一个命令的所有输出（包括错误）时。

一个>符号表示清空文件重新写入，两个>符号表示往文件的后面追加。

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

#### tmux

终端复用工具

```bash
# 安装
sudo apt install tmux
# 在当前终端开一个tmux终端
tmux
# 退出tmux回到终端
ctrl+b d
# 新建一个新tmux并命令为ping
tmux new -s ping
# 查看有几个tmux
tmux ls
0: 1 windows (created Thu Nov 30 21:01:43 2023)
ping: 1 windows (created Thu Nov 30 21:05:51 2023)
# 回到tmux0
tmux attach -t 0
# 在tmux0中切换到tmux ping中
tmux switch -t ping
# 关掉tmux0
tmux kill-seesion -t 0
# 关掉tmux ping
tmux kill-session -t ping
```

#### netcat

检查版本：

```shell
readlink -f $(which nc) # $(...): 这是命令替换的语法。它执行括号里的命令，并将输出替换到外部命令中。
```

结果会有两种：

- `/bin/nc.traditional`: 默认 GNU 基础版本，一般系统自带。
- `/bin/nc.openbsd`: openbsd 版本，强大很多。

都可以用 `apt-get install nc-traditional` 或者 `apt-get install nc-openbsd` 来选择安装。不管是 gnu 版本还是 openbsd 版本，都有新老的区别，主要是传送文件时 stdin 发生 EOF 了，老版本会自动断开，而新的 gnu/openbsd 还会一直连着

**端口测试**

你在服务器 A主机（192.168.1.2） 上面 8080 端口启动了一个服务，有没有通用的方法检测服务的 TCP 端口是否启动成功？或者在 B 主机上能不能正常访问该端口？

进一步，如果而 A 主机上用 netstat -an 发现端口成功监听了，你在 B 主机上的客户端却无法访问，那么到底是服务错误还是网络无法到达呢？我们当然可以在 B 主机上用 telnet 探测一下：

```bash
telnet 192.168.1.2 8080
```

但 telnet 并不是专门做这事情的，还需要额外安装，所以我们在 B 主机上用 netcat：

```bash
nc -vz 192.168.1.2 8080
```

即可，v 的意思是显示多点信息（verbose），z 代表不发送数据。那么如果 B 主机连不上 A 主机的 8080 端口，此时你就该检查网络和安全设置了，如果连的上那么再去查服务日志去。

nc 命令后面的 8080 可以写成一个范围进行扫描：

```bash
nc -v -v -w3 -z 192.168.1.2 8080-8083
```

两次 `-v` 是让它报告更详细的内容，`-w3` 是设置扫描超时时间为 3 秒。

**传输测试**

你在配置 iptable 或者安全组策略，禁止了所有端口，但是仅仅开放了 8080 端口，你想测试一下该设置成功与否怎么测试？安装个 nginx 改下端口，外面再用 chrome 访问下或者 telnet/curl 测试下？？还是 python -m 启动简单 http 服务 ？其实不用那么麻烦，在需要测试的 A 主机上：

```bash
nc -l -p 8080
```

这样就监听了 8080 端口，然后在 B 主机上连接过去：

```bash
nc 192.168.1.2 8080
```

两边就可以会话了，随便输入点什么按回车，另外一边应该会显示出来，注意，openbsd 版本 netcat 用了 `-l` 以后可以省略 `-p` 参数，写做：`nc -l 8080` ，但在 GNU netcat 下面无法运行，所以既然推荐写法是加上 `-p` 参数，两个版本都通用。

**老版本的 nc 只要 CTRL+D 发送 EOF 就会断开**，新版本一律要 CTRL+C 结束，**不管是服务端还是客户端只要任意一边断开了，另一端也就结束了，但是 openbsd 版本的 nc 可以加一个 `-k` 参数让服务端持续工作**。

那么你就可以先用 nc 监听 8080 端口，再远端检查可用，然后又再次随便监听个 8081 端口，远端检测不可用，说明你的安全策略配置成功了，完全不用安装任何累赘的服务。

**测试 UDP 会话**

两台主机 UDP 数据发送不过去，问题在哪呢？你得先确认一下两台主机之间 UDP 可以到达，这时候没有 nginx 给你用了，怎么测试呢？用 python 写个 udp 的 echo 服务？？ netcat 又登场了，在 A 主机上：

```bash
nc -u -l -p 8080
```

监听 udp 的 8080 端口，然后 B 主机上连上去：

```bash
nc -u 192.168.1.2 8080
```

然后像前面测试 tcp 的方法进行检测，结束了 CTRL+C 退出，看看一边输入消息另外一边能否收到。

**文件传输**

你在一台 B 主机上想往 A 主机上发送一个文件怎么办？不能用 scp / szrz 的话？继续 python 写个 http 上传？装个 ftpd 服务？不用那么麻烦，在 A 主机上监听端口：

```bash
nc -l -p 8080 > image.jpg
```

然后再 B 主机上：

```bash
nc 192.168.1.2 8080 < image.jpg
```

netcat 嘛，就是用于通过网络把东西 cat 过去，注意，老版本 GNU / OpenBSD 的 netcat 在文件结束（标准输入碰到 EOF），发送文件一端就会关闭连接，而新版本不会，你需要再开个窗口到 A 主机上看看接收下来的文件尺寸和源文件比较一下判断传输是否结束。

当传输完成后，你再任意一端 CTRL+C 结束它。对于新版 OpenBSD 的 netcat 有一个 `-N` 参数，可以指明 stdin 碰到 EOF 就关闭连接（和老版本一致），我们写作：

```bash
/bin/nc.openbsd -N 192.168.1.2 8080 < image.jpg
```

你机器上的 `nc` 命令有可能指向 `/bin/nc.traditional` 或者 `/bin/nc.openbsd` 任意一个，这里显示指明调用 openbsd 版本的 netcat。

这样在 openbsd 新版本的 netcat 中使用 `-N`参数，就不需要再开个终端去手工检查传输是否完成，传输结束了就会自动退出。其实 GNU 版本的 netcat 也有可以加个 `-q0` 参数，达到和 openbsd 版本 `-N` 的效果：

```bash
/bin/nc.traditional -q0 192.168.1.2 8080 < image.jpg 
```

只不过是 Linux 下面最新的 GNU netcat，对应 Windows 版本 没有该参数，所以从 Windows 传文件过去时，少不了再开个终端看一下进度，如果是 Linux 端发送就没问题了。通过管道协作，搭配 tar 命令，还可以方便的传一整个目录过去。

使用 netcat 这个系统默认安装的工具进行文件传输，可以算作你保底的手段，当 scp/ftp 都没法使用的情况下，你的一个杀手锏。

**网速吞吐量测试**

最简单的方法，GNU 版本的 netcat 加上 `-v -v` 参数后，结束时会统计接收和发送多少字节，那么此时 A 主机上显示运行 GNU 版本的 nc 监听端口：

```bash
/bin/nc.traditional -v -v -n -l -p 8080 > /dev/null
```

加 n 的意思是不要解析域名，避免解析域名浪费时间造成统计误差，然后 B 主机上：

```bash
time nc -n 192.168.1.2 8080 < /dev/zero
```

回车后执行十秒钟按 CTRL+C 结束，然后在 A 主机那里就可以看到接收了多少字节了，此时根据 time 的时间自己做一下除法即可得知，注意 GNU 的 netcat 统计的数值是 32 位 int，如果传输太多就回环溢出成负数了。

对于 OpenBSD 版本的 nc 我们可以用管道搭配 dd 命令进行统计，服务端运行：

```bash
nc -l -p 8080 > /dev/null
```

客户端运行 dd 搭配 nc：

```bash
dd if=/dev/zero bs=1MB count=100 | /bin/nc.openbsd -n -N 192.168.1.2 8080
```

结束以后会有结果出来，注意这里使用了 `-N` 代表 stdin 碰到 EOF 后就关闭连接，这里凡是写 `nc` 命令的地方，代表 GNU/OpenBSD 任意版本的 netcat 都可以，显示的指明路径，就代表必须使用特定版本的 netcat，上条命令等效的 GNU 版本是：

```bash
dd if=/dev/zero bs=1MB count=100 | /bin/nc.traditional -n -q0 192.168.1.2 8080
```

其实上面两种方法都把建立连接的握手时间以及 TCP 窗口慢启动的时间给计算进去了，不是特别精确，最精确的方式是搭配 pv 命令（监控统计管道数据的速度），在 A 主机运行：

```bash
nc -l -p 8080 | pv
```

然后再 B 主机运行：

```text
nc 192.168.1.2 8080 < /dev/zero
```

此时 A 主机那端持续收到 B 主机发送过来的数据并通过管道投递给 pv 命令后，你就能看到实时的带宽统计了，pv 会输出一个实时状态：

```bash
 353MiB 0:00:15 [22.4MiB/s] [          <=>  ]
```

让你看到最新的带宽吞吐量，这是最准确的吞吐量测试方法，在不需要 iperf 的情况下，直接使用 nc 就能得到一个准确的数据。

**系统后门**

假设你用串口登录到 A 主机，上面十分原始，包管理系统都没有，sshd/telnetd 都跑不起来，这时候你想用 B 主机通过网络登录 A 主机有没有办法？

GNU 版本的 netcat 有一个 `-e` 参数，可以在连接建立的时候执行一个程序，并把它的标准输入输出重定向到网络连接上来，于是我们可以在 A 主机上 `-e` 一下 bash：

```bash
/bin/nc.traditional -l -p 8080 -e /bin/bash
```

按回车打开系统后门，然后再 B 主机那里照常：

```bash
nc 192.168.1.2 8080
```

你就可以在 B 主机上登录 A 主机的 shell 了，操作完成 CTRL+C 结束。

对于 openbsd 版本的 netcat，`-e` 命令被删除了，没关系，我们可以用管道来完成，和刚才一样，在 A 主机上：

```bash
mkfifo /tmp/f
cat /tmp/f | /bin/bash 2>&1 | /bin/nc.openbsd -l -p 8080 > /tmp/f
```

然后 B 主机和刚才一样：

```bash
nc 192.168.1.2 8080
```

即可访问，用完注意将 `/tmp/f` 这个 fifo 文件删除。

#### ps

查看每个进程的更多信息，包括用户ID、CPU和内存使用、启动时间等

```shell
ps -aux
```

显示指定用户的所有进程

``` shell
ps -u [username]
```

显示进程树

```shell 
ps -ejH
```

 `ps` 命令本身不支持实时更新，但可以结合 `watch` 命令实现这一功能：

```shell
watch ps aux
```

按CPU或内存使用排序：将进程按CPU或内存使用量降序排序

```shell
ps aux --sort=-pcpu
ps aux --sort=-pmem
```

#### netstat

查看当前系统上所有 TCP 和 UDP 端口的监听状态，并显示关联到这些端口的进程信息

```shell
netstat -tulnp 
```

**`-n`** (Numeric)： 显示原始的数字地址和端口号，而不是尝试解析主机名、服务名或端口名。这加快了命令的执行速度并减少了命令可能因解析而失败的情况

**`-p`** (Program)： 显示哪个进程正在使用指定的套接字。这对于确定哪个应用程序正在监听或建立特定的网络连接特别有用。注意，这个选项通常需要 root 权限才能看到所有信息。

#### gdb

前提：

1.在编译时需要`-g`选项使程序带有调试符号信息。

strip 命令移除掉某个程序中的调试信息：在程序测试没问题后，需要发布到生产环境或者正式环境，会生成不 带调试符号信息的程序，以减小程序体积或提高程序执行效率。

2.建议关闭编译器的程序的优化选项。编译器的程序的优化选项一般有五个级别，从 O0~O4（注意第一个O0，是字母 O 加上数字 0）， O0 表示不优化（关闭优化），从 O1~O4 优化级别越来越高，O4 最大。关闭优化的目的是为了调试的时候，符号文件显示的调试变量等能与源代码完全对应起来。

##### 调试程序目标程序

```shell
# 使用 gdb 启动一个程序进行调试，也就是说这个程序还没有启动
gdb ./your_program

# 设置命令行参数
set args arg1 arg2 arg3
# 也可以在启动时设置参数
gdb --args ./your_program arg1 arg2 arg3

#运行
run # 简写r

# 显示源代码
list # 简写l，显示当前函数的源代码

# 开启TUI（Text User Interface）模式
# GDB的TUI模式提供了一个文本界面，可以在调试时显示源代码、汇编代码、寄存器状态和GDB输出.使用Ctrl + X，然后按A可以在TUI模式和常规模式之间切换。在TUI模式中，可以使用方向键上下滚动代码,Ctrl + X,然后按o可在让焦点在源码和命令之前切换。
layout src #显示源代码窗口,src,reg,asm

# 打印变量
p var
# 使输出更易读
set print pretty on # 关闭off

# 断点操作
b [函数名] # eg: b main
b [文件名]:[行号]
b [行号] # 当前文件
b [位置] if [条件] #在条件满足时触发断点
b [文件名]:[行号] thread [线程ID] # 只有当指定的线程达到该断点时，程序才会停止

# 列出所有断点
info breakpoints # 简写info b
# 禁用和启用断点
disable [断点号]
enable [断点号]

# 删除断点
delete [断点号]
delete # 删除全部断点
# 设置临时断点
tbreak [位置] #简写tb，该断点触发一次后自动删除。

# 断点触发后的操作
# 1.继续执行程序：当程序在断点处暂停时，使用此命令继续执行。
continue # c

# 逐行执行：在断点处执行下一行代码（不进入函数内部）。
next # n

#单步执行：执行下一行代码，如果是函数调用，则进入函数内部。
step # s


# 查看函数调用栈
backtrace # 简写bt
# 换到不同的线程
info threads # 查看当前所有的线程。每个线程都会有一个唯一的ID。
# 切换线程，比如，要切换到线程ID为 2 的线程
thread 2
# 然后就可以使用 continue、next、step 和 finish 等命令控制线程的执行。这些命令会在当前选中的线程上执行。
```

##### 附加进程

某些情况下，一个程序已经启动了，我们想调试这个程序，但是又不想重启这个程序。假设有这样一个场景，我们的聊天测试服务器程序正在运行，我们运行一段时间之后，发现这个聊天服务器再也没法接受新的客户端连接了，这个时候我们肯定是不能重启程序的，如果重启，当前程序的各种状态信息就丢失了。这个时候，我们只需要使用 `gdb attach 程序进程ID`来将 gdb 调试器附加到我们的聊天测试服务器程序上即可。假设，我们的聊天程序叫 chatserver，我们可以使用 ps 命令获取该进程的 PID，然后 gdb attach 上去，就可以调试了。

```shell
ps -ef | grep chatserver
```

我们得到 chatserver 的 PID 为 42921，然后我们使用 gdb attach 42921 把 gdb 附加到 chatserver 进程：

```shell
gdb attach 42921 # 42921 = $(pidof chatserver)
```

当用 `gdb attach` 上目标进程后，调试器会暂停下来，此时我们可以使用 `continue` 命令让程序继续运 行，或者加上相应的断点再继续运行程序。

当您调试完程序想结束此次调试，且不对当前进程 chatserver 有任何影响，也就是说想让这个程序继续运行，可以在gdb的命令行界面输入 `detach` 命令让程序与 gdb 调试器分离，这样 chatserver 可以继续运行。然后再退出gdb就可以了。

##### 进程 Crash 之后如何定位问题——调试 core 文件

有时候，我们的程序运行一段时间后，会突然崩溃。这当然不是我们希望看到的，我们需要解决这个问题。只要程序在崩溃的时候，有 core 文件(当一个程序崩溃并产生 "core dumped" 时，生成的核心转储文件,通常称为 "core" 文件。核心转储文件用于保存程序崩溃时的内存快照)产生，我们就可以使用这个 core 文件来定位崩溃的原因。当然，Linux 系统默认是不开启程序崩溃产生 core 文件的这一机制的，我们可以使用 `ulimit -c` 来 查看系统是否开启了这一机制。（ulimit 这个命令不仅仅可以查看 core 文件生成是否开 启，还可以查看其它的一些功能，如系统允许的最大文件描述符的数量等等，具体的可以使用 `ulimit - a` 命令来查看。）

如果我们需要修改某个选项的值， 可以使用 `ulimit 选项名 设置值`来修改，例如我们可以将 core 文件生成改成具体某个值（最大允许的字节数）或不限制大小，这里我们直接改成不限制大小，执行命令 `ulimit -c unlimited`

```shell
ulimit -c unlimited
ulimit -a
```

还有一个问题就是，这样修改以后，只对当前会话有效，当我们关闭这个 Linux 会话后，这个设置项的值就会被还原成 0。如果我们的程序以后台程序（守护进程）运行，也就是说当前会话虽然被关闭，程序仍然继续在后台运行，这样这个程序崩溃在某个时刻崩溃后，是无法产生 core 文件，这种情形不利于排查问题。所以，我们希望这个选项永久生效。设置永久生效的方式有两种。

1.在 /etc/security/limits.conf 中增加一行

```
#<domain> <type> <item> <value>
*          soft    core unlimited
```

这里设置的是不限制 core 文件的大小，也可以设置成具体的数值，如 1024 表示生成的 core 文件最大是 1024k。

2.把`ulimit -c unlimited`这一行，加到 /etc/profile 文件中去，放到这个文件最后一行即可，然后执行`source /etc/profile`让配置立即生效。当然这只是对 root 用户，如果想仅仅作用于某一用户，可以把`ulimit -c unlimited`加到该用户对应的 ~/.bashrc 或 ~/.bash_profile 文件中去。加进去后记得`source /etc/profile    `或者`source ~/.bashrc `

生成的core文件的默认命名方式是：core.pid，其位置是崩溃程序所在目录，举个例子，比如某个程序当时运行时其进程 ID 是16663，那么如果其崩溃产生的 core 文件的名称是 core.16663。调试 core 文件的命令是： 

```shell
gdb filename corename
# 查看崩溃时的调用堆栈，进一步分析就能找到崩溃的原因
bt 
```

自定义 core 文件的名称和目录 **/proc/sys/kernel/core_uses_pid** 可以控制产生的 core 文件的文件名中是否添加 PID 作为扩 展，如果添加则文件内容为 1，否则为 0; **/proc/sys/kernel/core_pattern** 可以设置格式化的 core 文件保存位置或文件名。修改方式如下：

```shell
echo "/corefile/core-%e-%p-%t-%s" >> /proc/sys/kernel/core_pattern
# 或者在/etc/sysctl.conf中设置
vim /etc/sysctl.conf
# 加入下面这行
kernel.core_pattern=/corefile/core-%e-%p-%t-%s
# 刷新，使其同步到/proc/sys/kernel/core_pattern文件中
sysctl -p /etc/sysctl.conf
```

%e：程序文件的完整路径（路径中的/会被!替代） 

%p：添加 pid 到 core 文件名中

%t：进程奔溃的时间戳 

%s：哪个信号让进程奔溃

%h:  添加主机名到 core 文件名中

%u: 添加当前 uid 到 core 文件名中

%g: 添加当前 gid 到 core 文件名中

需要注意的是，您使用的用户必须对指定 core 文件目录具有写权限，否则生成时会因为权限不足而导致无法生成 core 文件。

