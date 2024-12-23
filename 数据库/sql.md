DDL

1.数据库操作

1.1 查询

```mysql
# 查询所有数据库
show databases;
# 查询当前数据库
select database();
```

1.2 创建

```mysql
# 创建数据库
create database [if not exists] 数据库名;
```

1.3 使用

```mysql
use 数据库名;
```

1.4 删除

```mysql
drop database [if exists] 数据库名;
```

注意：schemas可以替换上诉的database

2.表操作

2.1创建

```mysql
create table 表名(
	字段1 字段类型 [约束] [comment 字段1注释],
	字段n 字段类型 [约束] [comment 字段n注释]
)[comment 表注释];
```

约束: 作用于表中字段上的规则，用于限制存储在表中的数据。保证数据库中数据的正确性，有效性和完整性。

| 约束 | 描述                                                     | 关键字                      |
| ---- | -------------------------------------------------------- | --------------------------- |
| 非空 | 限制该字段不能为null                                     | not null                    |
| 唯一 | 保证字段的所有数据是唯一的                               | unique                      |
| 主键 | 主键是一行数据的唯一标识，非空且唯一                     | primary key(auto_increment) |
| 默认 | 未指定该字段的值，则采用默认值                           | default                     |
| 外键 | 该字段的值一定是其他表中存在的，保证数据的一致性和完整性 | foreign key                 |

```mysql
create table tb_user(
    id int primary key,
    username varchar(20) not null unique ,
    name varchar(10) not null ,
    age int,
    gender char(1) default '男'
)
```

2.2数据类型

2.2.1数值类型

| 类型      | 大小(byte) | 有符号(SIGNED)范围                                    | 无符号(UNSIGNED)范围                                       | 描述             | 备注                                               |
| --------- | ---------- | ----------------------------------------------------- | ---------------------------------------------------------- | ---------------- | -------------------------------------------------- |
| tinyint   | 1          | (-128，127)                                           | (0，255)                                                   | 小整数值         |                                                    |
| smallint  | 2          | (-32768，32767)                                       | (0，65535)                                                 | 大整数值         |                                                    |
| mediumint | 3          | (-8388608，8388607)                                   | (0，16777215)                                              | 大整数值         |                                                    |
| int       | 4          | (-2147483648，2147483647)                             | (0，4294967295)                                            | 大整数值         |                                                    |
| bigint    | 8          | (-2^63，2^63-1)                                       | (0，2^64-1)                                                | 极大整数值       |                                                    |
| float     | 4          | (-3.402823466 E+38，3.402823466351  E+38)             | 0 和 (1.175494351  E-38，3.402823466 E+38)                 | 单精度浮点数值   | float(5,2)：5表示整个数字长度，2  表示小数位个数   |
| double    | 8          | (-1.7976931348623157 E+308，1.7976931348623157 E+308) | 0 和  (2.2250738585072014 E-308，1.7976931348623157 E+308) | 双精度浮点数值   | double(5,2)：5表示整个数字长度，2  表示小数位个数  |
| decimal   |            |                                                       |                                                            | 小数值(精度更高) | decimal(5,2)：5表示整个数字长度，2  表示小数位个数 |

例子：

age tinyint unsigned

score double(3,1)

金额 decimal

2.2.2字符串类型

| 类型       | 大小                  | 描述                         |
| ---------- | --------------------- | ---------------------------- |
| char       | 0-255 bytes           | 定长字符串                   |
| varchar    | 0-65535 bytes         | 变长字符串                   |
| tinyblob   | 0-255 bytes           | 不超过255个字符的二进制数据  |
| tinytext   | 0-255 bytes           | 短文本字符串                 |
| blob       | 0-65 535 bytes        | 二进制形式的长文本数据       |
| text       | 0-65 535 bytes        | 长文本数据                   |
| mediumblob | 0-16 777 215 bytes    | 二进制形式的中等长度文本数据 |
| mediumtext | 0-16 777 215 bytes    | 中等长度文本数据             |
| longblob   | 0-4 294 967 295 bytes | 二进制形式的极大文本数据     |
| longtext   | 0-4 294 967 295 bytes | 极大文本数据                 |

char(10): 最多只能存10个字符,不足10个字符,占用10个字符空间
varchar(10): 最多只能存10个字符,不足10个字符, 按照实际长度存储

例子

phone char(11),

username varchar(20),

2.2.3时间日期类型

| 类型      | 大小(byte) | 范围                                       | 格式                | 描述                     |
| --------- | ---------- | ------------------------------------------ | ------------------- | ------------------------ |
| date      | 3          | 1000-01-01 至  9999-12-31                  | YYYY-MM-DD          | 日期值                   |
| time      | 3          | -838:59:59 至  838:59:59                   | HH:MM:SS            | 时间值或持续时间         |
| year      | 1          | 1901 至 2155                               | YYYY                | 年份值                   |
| datetime  | 8          | 1000-01-01 00:00:00 至 9999-12-31 23:59:59 | YYYY-MM-DD HH:MM:SS | 混合日期和时间值         |
| timestamp | 4          | 1970-01-01 00:00:01 至 2038-01-19 03:14:07 | YYYY-MM-DD HH:MM:SS | 混合日期和时间值，时间戳 |

例子：

birthday date

update_time datetime