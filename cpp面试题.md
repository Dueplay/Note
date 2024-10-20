1.快速上手一个c/c++项目 -- 调试法

编译并用调试器gdb把程序运行起来，有参数记得set args。

把程序终端下来(Ctrl + X), 查看每个线程的调用关系，看相关代码。

gdb常用命令：

gdb filename

gdb attach -p pid

查看线程 info threads

切换线程 thread threadno

查看线程堆栈 bt

切换到某个堆栈 frame N 

查看某个端口是否被占用：lsof -i -Pn | grep port

2.动态库需要提供的文件

- window

  ```
  # 隐式加载
  *.h
  *.lib // 声明
  *.dll // 实现
  
  # 显示加载,只需要*.dll 
  LoadLibrary("*.dll");
  GetProcAddress(, "func_name");
  
  ```

- Linux

  ```
  # 隐示加载
  *.h
  *.so
  
  # 显示加载, 只需要*.so
  # man dlopen
  dlopen()
  ```

3.定位bug，开发环境没问题，测试环境有复现几率不大的bug，偶尔出现。

gdb回放功能

4.客户端端口限制的时候如何进行tcp连接->bind也能用与客户端

客户端连接服务器的流程

```
1.创建socket
# 调用bind函数给客户端指定一个端口
2.调用connect函数连接指定的ip和port
```

nc模拟客户端指定端口连接 

```
nc -v -p port 服务端ip 服务端port
```

5.判断字节序函数

字节序

大端(Big Endian), 小端(Little Endian), 有网络字节序(大端)，主机字节序。

大端：高位存放在内存低地址，低位存放在内存高地址。(**高低低高**)

小端：高位存放在内存高地址，低位存放在内存低地址。(**高高低低**)

```
int32_t a = 0x12345678;
函数实现原理:
将一个多字节的整型值拆成单字节，看低地址位(高)是否是数值的高位(或低位)，低地址存放的时低位则为小端，存放高位则位大端。

char b = *(char *)&a;
b == 0x12还是0x78
```

6.异步connect

```
1.创建socket
2.设置socket为非阻塞
3.调用connect函数，并且不用判断其返回值
4.调用io复用函数(select,epoll)判断socket是否可写
对于Linux，在判断可写的情况下，需要再判断是否有错，因为socket创建了就是可写的。
```

7.一个window程序只能启动一个实例，eg：微信，杀毒软件等。

window单例进程的实现原理，比如可以利用一个具名的内核变量，mutex，event等，前一个程序启动了，后一个再去创建这个变量则失败。

8.TCP粘包问题，解包逻辑

```
// socket读事件触发收到数据后，调用下面函数
void XXSession::onRead() {
	while (true) {
		// 1.判断收取的数据是否够一个包头大小，不够continue，足够执行下一步
		// 2.从接收缓冲区取出包头数据，解析得到包体大小，判断缓冲区是否够一个包体大小，不够continue，够则执行下一步
		// 3.解析包体得到业务数据，进行业务逻辑
		// 4.业务逻辑处理完退出
	}
}
```

9.nc命令的使用

```
# 模拟一个服务器
nc -v -l ip port
# 模拟一个客户端
nc -v ip port
# 客户端以指定的端口号去连接
-p port
# 服务端多次连接不退出
-k 
```

其他常见网络命令

```
ifconfig/ipconfig
ping
telnet
netstat
lsof
nc
tcpdump
```

10.

11.epoll水平模式和边缘模式在收发数据时的差异

水平模式可以在每次读事件触发时，按需收取需要的字节数量；边缘模式在每次读事件触发时，必须要把数据收完，调用recv/read函数，函数返回-1并且errno为ewouldblock，才表明数据收完。比如：服务端采用边缘模式，如果在socket读事件触发后每次只固定收取5k大小的数据，这是不行的，因为客户端可能发送10k的数据过来，由于时edge模式，没收取完该socket也不会再触发读事件了。

发数据发送不出去时，应该把放在发送缓冲区的尾部，下次写事件触发时，从首部依次读取数据，发送。

水平模式，一旦注册了写事件，数据发完后，要立即移除写事件，不然会一直触发， 浪费cpu资源，因为只要条件（比如文件描述符可写）满足，事件就会持续触发，即使没有实际的数据需要写入；边缘模式可以不移除，下次需要监听写事件时，再次注册写事件，因为事件只在状态从不可写变为可写时触发一次，即状态变化时触发一次，如果数据发送完后，没有立即移除写事件，事件也不会重复触发，直到状态再次变化。

11.Linux server端对SIGPIPE信号的处理方法。

考虑这样的场景

```
1. A与B正常连接
2. B关闭了连接
3. A发送数据，A的send或者write函数返回n，如果n=0，则对端关闭了连接，如果>0, A的报文到了B端之后，B回应一个RST报文
4. A的逻辑是继续调用send/write函数，A程序就crash(此时send/write函数由于B端的关闭，会产生一个SIGPIPE信号，os默认处理这个信号就是让程序退出)
```

```
// 方法1.
int main() {
	// 程序入口s忽略这个信号，对于所有socket都生效
	signal(SIGPIPE, SIG_IGN);
}

// 方法2
int n = send(socket, buf, bulen, MSG_NOSIGNAL);
n == -1 && errno == EPIPE
```

不用gdb可以用pstack打印堆栈

pstack pid