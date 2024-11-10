redis

启动redis服务端，conf默认是在/etc/redis/中，默认是6379端口

```
redis-server /path/redis.conf
```

启动客户端

```
redis-cli -h ip -p port
```

基本命令

```
# 设置
set k v

# 查询
get k

# 删除
del k

keys pattern 
# 查看所有的kv
keys * 
# 判断key是否存在
exists key

# 删除所有
flushall

# 已二进制的形式存储，不支持中文，需要支持中文要在启动时加--raw以原始的形式来显示内容。
redis-cli --raw

# 查看key的过期时间， 输出-1表示为设置过期时间
TTL key

# 设置key的过期时间，单位为s。过期后get key无输出，keys也没法看到
expire key seconds

# 设置一个带有过期时间的kv
setex key seconds value

# key不存在时设置，存在则不做操作
setnx key value
```

列表List

用来存储和操作一组有顺序的数据，类似数组

`LPUSH`和`RPUSH`将元素插入到头部和尾部

```
# lpush/rpush 列表名 元素1 元素n，元素按照从左到右的顺序依次插入到头部/尾部
lpush letter a
```

获取列表内容：`LRANGE key start stop`

```
# 列表名 起始pos 结尾pos 
# pos以0开始， -1表示最后一个元素
lrange letter 0 -1

127.0.0.1:10080> rpush letter b c d
4
127.0.0.1:10080> lrange letter 0 -1
a
b
c
d
127.0.0.1:10080> lpush letter x y z
7
127.0.0.1:10080> lrange letter 0 -1
z
y
x
a
b
c
d

```

`LPOP`和`RPOP`从列表头部或者尾部删除元素

```
# 删除前面n个
LPOP key n
RPOP key n
```

`LLEN`查看列表长度

```
LLEN key 
```

`LTRIM`删除列表中指定范围以外的元素，只保留[start, stop]之间的元素

```
LTRIM ket start stop
```



集合Set

List中元素可以重复，Set中元素不能，且不具有顺序。

相关命令以S开头

SADD添加元素

```
# sadd key 元素1 ... 元素n
sadd course redis os mysql
```

SMEMBERS查看集合中元素

```
# smembers key
smembers course
```

SISMEMBER判断元素是否在集合中, 输出0为false，1为true

```
sismember course member
```

SREM删除集合中元素

```
srem key member1 member2...
```

集合运算

交：SINTER

并：SUNION

差：SDIFF

```
SINTER/SUNION/SDIFF key key
```



有序集合SortedSet，ZSet

有序集合中的每一个元素都会关联浮点类型的分数，然后按照这个分数对集合中元素进行从小到大的排序。分数相同按照字典序排序

有序集合的成员是唯一的，但是关联的分数可以重复。

相关命令以Z开头

ZADD添加元素

```
# zadd key 元素1 ... 元素n
# 元素由一个分数为一个成员组成，成员在前。
zadd grade 99 gxj 100 wsq 95 zs 98 ls
```

ZRANGE查看集合中成员，[WITHSCORES]分数同时输出

```
# ZRANGE key start stop [WITHSCORES]
ZRANGE grade 0 -1

ZRANGE grade 0 -1 WITHSCORES
```

ZSCORE查看某个成员的分数

```
# zscore key member
zscore grade gxj
```

ZRANK查看某个成员的index，从0开始

```
# zrank key member
zrank grade gxj
```

ZREVRANK查看某个成员的排名，REV表示reverse

```
# zrevrank key member
zrevrank grade gxj
```

ZREM 删除某个成员

```
ZREM key member
```



哈希Hash

字符类型的字段和值的映射表，kv的集合，适合用于存储对。将对象表示为一组kv

命令以H开头

HSET添加一个键值对

```
# HSET key field value [field value ...]
HSET person name gxj
HSET person age 24
```

HGET获取一个键值对

```
# HGET key field
# 获取person中的name
HGET person name
# 获取person中的age
HGET person name
```

HGETALL获取哈希中的所有键值对

```
HGETALL key
HGETALL person
```

HDEL删除哈希中的某个键值对

```
HDEL key field
hdel person age
```

HEXISTS判断某个键值对是否存在

```
HEXISTS key field
hexists person name
hexists person age
```

HKEYS获取哈希中的所有键

HLEN获取哈希中的所有kv的数量



发布订阅

`publish channel message`发布消息，将消息发送到指定的频道。

`subscribe channel `订阅频道，接受消息。

```
# 客户端1
subscribe cqu

# 客户端2
subscribe cqu

# 客户端3
publish cqu hello
```

局限性：消息无法持久化，无法记录历史消息等。



消息队列Stream

轻量级的消息队列，可以解决发布订阅的局限性。

命令为X开头



位图Bitmap

字符串string类型的拓展，本质上还是一个字符串。

可以维护一些状态，比如用户的登录状态，是否点赞，签到情况等。

命令以BIT开头

SETBIT key offset value, 将offset位置设置为0/1

```
setbit online 0 1
```

GETBIT key offset 获取位图中某个偏移量的值

```
getbit online 0
```

一次性设置多个bit的值

```
# 前四个为1，后四个为0
set online "\xF0"
```

BITCOUNT key 统计一个key的值里面有多少bit是1

```
bitcount online
```

BITPOS key bit 获取第一个出现0/1的pos

```
bitpos online 0
```



事务

一次请求中执行多个命令。

```
MULII # 开启一个事务，所有命令放入一个队列中
SET
LPUSH
SADD
EXEC/DISCARD # EXEC执行事务中的所有命令
```

redis中的事务不能保证所有命令都会成功。执行结果取决于事务中的命令。

可以保证以下三点：

1.在发送exec命令之前，所有命令都会被放入到一个队列中缓存起来，不会立即执行。

2.收到exec命令之后，事务开始执行，事务中任何一个命令执行失败，其他命令依然会被执行，不会因为某个命令的失败导致其他命令不会被执行这种情况。

3.在事务执行的过程中，其他客户端提交的命令请求，不会被插入到事务的执行命令序列中。



持久化

1.RDB，redis database

某个时间点上redis的完整数据副本(快照)。

根据conf中的save参数自动保存快照

save 时间 操作次数：时间内操作多少次触发快照。

命令save主动触发快照。

快照后的修改会丢失。

适合用于备份

2.AOF，append-only file

执行写命令的时候，不仅会把命令写入到内存，还会将命令写入到一个追加的文件中。追加文件：以日志的形式记录每一个写操作。重启时，通过重新执行AOF文件中命令重建内容。

conf中appendonly yes/no开启和关闭



主从复制

将一台redis服务器的数据复制到其他redis服务器。主节点复制到从节点，单向复制，只能从主节点到从节点，只能有一个主节点。

一般主节点负责写，从节点负责读。

主节点通过异步的方式将变化的数据发给从节点，从节点收到主节点的数据后更新自己的数据，达成数据一致。



哨兵模式

当一主多从的redis集群中主节点宕机，需要手动配置将从节点变为主节点，没法自动实现故障转移

1.监控，redis节点是否正常

2.通知，如果发现某个节点出了问题，哨兵就会通过发布订阅模式通知其他节点。

3.自动故障转移。当主节点不能正常工作时，哨兵开始一个自动故障转移的操作，将一个从节点升级为主节点，其他从节点指向新的主节点。

注意：哨兵本身也是一个节点，也会有单点故障的问题。一般使用三个哨兵来保证高可用。三个哨兵会选出一个领导者，领导者来监控其他节点。领导者挂了，其他哨兵会重新算出一个领导者。