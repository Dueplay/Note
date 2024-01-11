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

老版本的 nc 只要 CTRL+D 发送 EOF 就会断开，新版本一律要 CTRL+C 结束，不管是服务端还是客户端只要任意一边断开了，另一端也就结束了，但是 openbsd 版本的 nc 可以加一个 `-k` 参数让服务端持续工作。

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