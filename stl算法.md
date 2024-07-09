stl算法

算法把迭代器作为中介从而间接操作容器中的元素，而不直接操作容器本身。所有标准库算法都实现为函数模板的形式，其中模板类型参数一般都是迭代器类型。模板化的函数通常可通过函数参数推导出模板类型，因此通常情况下可以像调用普通函
数(而非模板)那样调用算法。

## 简单例子

1.find和find_if

find()在某个迭代器范围内查找特定元素。可将其用于任意容器类型。这个算法返回所找到元素的迭代器: 如果没有找到元素，则返回迭代器范围的尾迭代器。注意调用 find()时指定的范围不要求是容器中元素的完整范围，还可以是元素的子集。
注意：
如果 fnd()没有找到元素，那么返回的迭代器等于函数调用中指定的尾迭代器，而不是底层容器的尾迭代器

```c++
auto endIt =cend(myVector);
auto it = find(cbegin(myVector),endIt,num);
// it == endIt -> not find

// c++17 
if (auto it = find(cbegin(myVector),endIt,num); it == endIt) {
    // not find 
}
```

一些容器(例如 map和 set)以类方法的方式提供自己的 find()函数。如果容器提供的方法具有与泛型算法同样的功能，那么应该使用容器相应的方法，那样速度更快。比如，泛型算法 find()的复杂度为线性时间，用于 map 迭代器时也是如此; 而 map 中find()方法的复杂度是对数时间。

find_if

find_if()和 find()类似，区别在于 find _f()接收谓词函数回调作为参数，而不是简单的匹配元素。谓词返回true或false。find_if()算法对范围内的每个元素调用谓词，直到谓词返回ture: 如果返回了true,find if()返回这个元素的迭代器。

```c++
find_if(cbegin(myVector),endIt,Prec);
```

2.accumulate

我们经常需要计算容器中所有元素的总和或其他算术值。acumulate函数就提供了这种功能，该函数在<numeric>(而非<algorithm>)中定义。通过这个函数的最基本形式可计算指定范围内元素的总和。

```c++
// 总和
int sum = accumulate(cbegin(nums)， cend(nums)， 0);

// 给accumulate提供的二元回调。也可以是lambda
int product(int num1，int num2) {
    return numl *num2;
}
// 计算乘积
double mult = accumulate(cbegin(nums)， cend(nums)，1，product);
    
```

## std:function 

std:function 在<functional>头文件中定义，可用来创建指向函数、函数对象或lambda 表达式的类型; 从根本上说可以指向任何可调用的对象。它被称为多态函数包装器，可以当成函数指针使用，还可用作实现回调的函数的参数。

模板定义：

```c++
std::function<R(ArgTypes..)>
```


R 是函数返回值的类型，ArgTypes 是一个以逗号分隔的函数参数类型的列表。

std:function 真正有用的场合是将回调作为类的成员变量。在接收函数指针作为自定义函数的参数时，也可以使用 std::function。

```c++
// 实现一个函数指针f1指向func
function<void(int，const string&)> fl = func;
f1(1，"test");
```

当然，上例可使用 auto 关键字，这样就不需要指定 f1 的具体类型了。下面的 f1 定义实现了同样的功能而且简短得多，但 f1 的编译器推断类型是函数指针(即 void(*f1)(int,const string&)而不是 std:function:

```c++
auto f1 = func;
```

std::function作为函数形参可以接收任意可调用对象。如果用普通函数指针作为函数参数，lambda表达式实参作为参数传递给该函数是不行的。

```c++
void process(const vector<int>& vec，function<void(int)> f) {
    for (auto& i:vec) f
		f(i);
}
	
void print(int num) {
    cout << num << " ";
}

int main() {
    vector<int> vec{0,1,2,3,4,5,6,7,8,9 };
	process(vec,print);
  	cout << endl;
    int sum=0;
    // 如果process第二个参数是普通函数指针,传递lambda是不行的
    process(vec，[&sum](int num){sum += num;});
	cout <<"sum = "<< sum <<endl;
	return 0;
}
```

## lambda

使用 lambda 表达式可编写内嵌的匿名函数，而不必编写独立函数或函数对象，使代码更容易阅读和理解。

语法：

```c++
[捕获列表] (参数列表) mutable -> 返回值类型 { 函数体 };
```

(参数列表) ：如果没有可省略

可以忽略返回类型。如果忽略了返回类型，编译器就根据函数返回类型推断规则来推断 lambda 表达式的返回类型

lambda 表达式的方括号[]部分称为 lambda 捕捉块capture block)。捕捉变量的意思是可在 lambda 表达式体中使用这个变量。

指定空白的捕捉块[]表示不从所在作用域内捕捉变量; 在捕捉块中只写出变量名将按值捕捉该变量; &变量名将按引用捕捉该变量

编译器将 lambda 表达式转换为某种未命名的仿函数(即函数对象)。捕捉的变量变成这个仿函数的数据成员,将按值捕捉的变量复制到仿函数的数据成员中。这些数据成员与捕捉的变量具有相同的 const 性质。

仿函数总是实现函数调用运算符 operator()。对于 lambda 表达式，这个函数调用运算符被**默认标记为 const**, 这表示即使在 lambda 表达式中按值捕捉了非 const 变量lambda 表达式也不能修改其副本。把 lambda 表达式指定为mutable，就可以把函数调用运算符标记为非const.

注意如果指定了 mutable，就必须给参数指定圆括号，即使圆括号为空。

在变量名前面加上&，就可按引用捕捉它。lambda 表达式可以直接在其内

修改按引用捕获的变量。

按引用捕捉变量时，必须确保执行 lambda 表达式时，该引用仍然是有效的。

![image-20240401212649173](E:\db资料\Note\stl算法.assets\image-20240401212649173.png)

使用默认捕捉时，只有在 lambda 表达式体中真正使用的变量才会被捕捉，使用值(=)或引用(&)捕捉。未使用的变量不捕捉。
警告:
不建议使用默认捕捉，即使只捕捉在 lambda 表达式体中真正使用的变量，也同样如此。使用=默认捕捉可能在无意中引发昂贵的复制。使用&默认捕捉可能在无意间修改所在作用域内的变量。建议**显式指定要捕捉的变量**。

泛型lambda表达式

可以给 lambda 表达式的参数使用自动推断类型功能，而无须显式指定它们的具体类型。要为参数使用自动推断类型功能，只需要将类型指定为 auto，类型推断规则与模板参数推断规则相同。

### lambda 捕捉表达式

lambda 捕捉表达式允许用任何类型的表达式初始化捕捉变量。这可用于在 lambda 表达式中引入根本不在其内部的作用域内捕捉的变量，例：

myCapture使用lambda捕捉表达式初始化为字符串“Pi:”

```c++
double pi=3.1415;
auto myLambda= [myCapture ="pi:"，pil[ cout << myCapture << pi;);
                                     
auto myPtr = std::make_unique<double>(3.1415);
auto myLambda = [p = std::move(myPtr)]{ cout <<*p;};                                    
// 捕捉变量允许使用与所在作用域(enclosing scope)内相同的名称。      automyLambda = [myPtr = std::move(myPtr)]{ cout <<*p;};                              
```

lambda可作为函数返回值，返回类型为function

还可以作为函数参数，参数类型为function

## 函数对象

在类中，可重载函数调用运算符()，使类的对象可取代函数指针。将这些对象称为函数对象(function objiect),或称为仿函数(functor)

### 函数对象适配器

#### 1.绑定器：

绑定器(binder)可用于将函数的参数绑定至特定的值。为此要使用<functiona>头文件中定义的 std::bind，它允许采用灵活的方式绑定可调用的参数。既可将函数的参数绑定至固定值，甚至还能重新安排函数参数的顺序。

假定有一个 func函数，它接收两个参数:

```c++
void func(intnum，strinqviewstr) {
    cout <<"func("<< num << "，"<< str <<")" <<endl;
}
```

通过 bind将 func函数的第二个参数绑定至固定值 myString。结果保存在 f1中。使用auto关键字是因为C++标准未指定 bind的返回类型，因而是特定于实现的。没有绑定至指定值的参数应该标记为 1、2和3等。这些都定义在 std::placeholders 名称空间中。在f1的定义中，func第一个参数应该出现在1这个位置。之后，就可以用一个整型参数调用 f1。

```c++
string myString ="abc";
auto f1 = bind(func，placeholders::_1，myString);
f1(16);
```

输出:
func(16，abc)
bind还可用于重新排列参数的顺序, fun第二个参数应该出现在f2的_2这个的位置。也就是说f2第一个参数将成为函数 func的第二个参数，第二个参数将成为函数 func的第一个参数。
auto f2= bind(func，placeholders::2，placeholders::1);
f2("Test"，32);
输出:
func(32，Test)

## 修改序列算法

从一个范围向另一个范围复制元素、删除元素以及反转某个范围内元素的顺序。
警告:
修改算法不能将元素插入目标范围中，仅可重写/修改目标范围中已经存在的元素。使用迭代器适配器，在目标范围中真正插入元素
map和 multimap 的范围不能用作修改算法的目标范围。这些算法改写全部元素，而在 map 中，元素是键值对。map和 multimap将键标记为 const，因此不能为其赋值。set和 multiset 也是如此。替换方案是使用插入迭代器。

### 1.转换

transform

transform算法对范围内的每个元素应用回调，期望回调生成一个新元素，并保存在指定的目标范围中。如果希望transform将范围内的每个元素替换为调用回调产生的结果，那么源范围和目标范围可以是同一范围。其参数是源序列的首尾迭代器、目标序列的首迭代器以及回调。

```c++
// 将vector中的每个元素增加100
transform(begin(myVector)，end(myVector)， begin(myVector)
[](int i)[ return i + 100;});
```

transform的另一种形式对范围内的元素对调用二元函数，需要将第一个范围的首尾迭代器、第二个范围的首迭代器以及目标范围的首迭代器作为参数。

例: 创建两个 vector，然后通过transform计算元素对的和并将结果保存回第一个 vector. 这里 : vec2的大小应该 >= vec1, 因为是到end(vec1)结束。

```c++
transform(begin(vec1)，end(vec1)，begin(vec2)，begin(vec1),
[](int a,int b){return a + b;]);
```

注意:
transform和其他修改算法通常返回一个引用目标范围内最后一个值后面那个位置(past-the-end)的迭代器

### 2.复制

copy算法可将一个范围内的元素复制到另一个范围，从这个范围内的第一个元素开始直到最后一个元素。源范围和目标范围必须不同，但在一定限制条件下可以重叠。限制条件如下:对于 copy(b,e,d)，如果 d 在b之前，则可以重叠; 但如果 d处于[b.e]范围，则行为不确定。与所有修改算法类似，copy不会向目标范围插入元素，只改写已有的元素。所以要保证目标范围要有足够空间。

这个例子将 vec1 中的所有元素复制到 vec2:

```c++
vector<int> vecl， vec2;
populateContainer(vecl);
vec2.resize(size(vec1)); // 以确保目标范围有足够空间
copy(cbegin(vec1)， cend(vec1)，begin(vec2));
```

copy_backward算法，这个算法将源范围内的元素反向复制到目标范围。换句话说，这个算法从**源范围的最后一个元素开始，将这个元素放在目标范围的最后一个位置**，然后在每一次复制之后反向移动。

源范围和目标范围在一定限制条件下可以重叠。限制条件如下:对于copy_backward(b,e,d)，如果 d 在e之后，则能正确重叠；但如果 d 处于b,e]范围，则行为不确定。

前面的例子可按如下代码修改为使用copy_backward而不是 copy。注意第三个参数应该指定end(vec2)而不是 begin(vec2):

```c++
copy_backward(cbegin(vec1)， cend(vec1)，end(vec2));
```

copy_if算法时，需要提供由两个迭代器指定的输入范围、由一个迭代器指定的输出范围以及一个谓词(函数或 lambda 表达式)。该算法将满足谓词的所有元素复制到目标范围。**复制不会创建或扩大容器，只是替换现有元素。因此，目标范围应当足够大，从而保存要复制的所有元素**。copy_if返回了目标范围内最后一个复制的元素后面那个位置(one-past-the-last-copied element)的选代器，以便确定需要从目标容器中删除的元素。例: 只把偶数复制到 vec2:

```c++
vector<int> vec1， vec2;
populateContainer(vec1);
vec2.resize(size(vec1));
auto endIterator = copy if(cbegin(vec1)， cend(vec1)，
begin(vec2)，[](int i){ return i%2 == 0;});
vec2.erase(endIterator， end(vec2));
```


copy_n从源范围复制n个元素到目标范围。copy_n的第一个参数是起始迭代器，第二个参数是指定要复制的元素个数，第三个参数是目标迭代器。该算法不执行任何边界检查，因此一定要确保起始选代器递增n个要复制的元素后，不会超过集合的 end()，否则程序会产生未定义的行为。

```c++
cnt = min(cnt, size(vec1))
vec2.resize(cnt);
copy_n(cbegin(vec1)， cnt，begin(vec2));
```

### 3.移动

move,基于移动语义。如果要在自定义类型元素的容器中使用这两个算法，那么需要在元素类中提供移动赋值运算符operator=(Class&& rhs).

```c++
class MyClass {
public:
    MyClass() = default;
    MyClass(const MyClass& src) = default;
    MyClass(string_view str) :mStr(str) {}
    virtual ~MyClass() = default;
    // Move assignment operator
    MyClass& operator=(MyClass&& rhs) noexcept {
        if (this== &rhs)
        return *this;
        mStr = std;:move(rhs.mStr);
        cout <<"Move operator= (mStr=" << mStr <<")" << endl;
        return *this;
    }
    void setStrinq(string view str) [ mStr =str; ]
    string view getString() const (return mStr;l
private:
	string mStr;
};
int main(){
    vector<MyClass> vecSrc{ MyClass("a")，MyClass("b")，MyClass("c")};
	vector<MyClass> vecDst(vecSrc.size());
	move(begin(vecSrc)，end(vecSrc)，beqin(vecDst));
	for (const auto& c : vecDst){ cout << c.getString() <<" "; }
	return 0;
}

输出如下所示:
Move operator=(mStr=a)
Move operator=(mStr=b)
Move operator=(mStr=c)
a b c
```

注意这段代码包含两种不同的 move用法。一种是，move函数接收一个参数，将 lvalue 转换为rvalue，在<utility>中定义:而另一种是，接收3个参数的 move是标准库的 move算法，这个算法在容器之间移动元素。

注意：在移动操作中，源对象将处于**有效但不确定**的状态。在这个例子中，这意味着在执行动操作后，不应该再使用 vecSrc 中的元素了，除非使它们重回确定状态，例如，在它们之上调用方法(不包任何预置条件)，如setString()。

move_backward使用和move同样的移动机制,但按从最后一个元素向第一个元素的顺序移动。对于move和 move_backward，在符合某些限制条件的情况下允许源范围和目标范围重叠。限制条件与 copy和copy_backward的相同。

### 4.替换
replace和 replace_if算法将一个范围内匹配某个值或满足某个谓词的元素替换为新的值。replace_if算法的第一个和第二个参数指定了容器中元素的范围。第三个参数是一个返回 true 或 false 的函数或lambda表达式，如果它返回 true，那么容器中的对应值被替换为第四个参数指定的值:如果它返回 false，则保留原始值。

```c++
replace_if(begin(vec)，end(vec)，[](int i)[ return i%2 != 0;}，0);
```

### 5.删除

假设要将某个范围内满足某特定条件的元素删除。第一个解决方案是查看文档，确定容器是否有erase方法，然后选代所有元素，并对每个满足条件的元素调用 erase。vector 是包含 erase方法的容器之一。然而，如果对 vector 容器应用erase，这个方法的效率非常低下，因为要保持 vector 在内存中的连续性，会涉及很多内存操作，时间复杂度 O(n*2)。这个方法还容易产生错误，因为必须非常小心地确保每次调用 erase之后选代器依然有效。例如，如下函数从 string vector 中删除空字符串，未使用算法。

```c++

void removeEmptyStringsWithoutAlgorithms(vector<string>& strings) {
    for (autoiter=beqin(strings); iter != end(strings);)
        if (iter->empty())
        	iter = strings.erase(iter);
        else
        	++iter;
}

```

这个问题的方法是“删除-擦除法”(remove-erase-idiom),只需要线性时间。算法只能访问迭代器抽象，不能访问容器。因此删除算法不能真正地从底层容器中删除元素，而是**将匹配给定值或谓词的元素替换为下一个不匹配给定值或谓词的元素**。为此使用移动赋值。结果是将范围分为两个集合:一个用于保存要保留的元素，另一个用于保存要删除的元素。**返回的迭代器指向要删除的元素范围内的第一个元素**。如果真的需要从容器中删除这些元素，必须先使用 remove算法，然后调用容器的 erase方法，将从返回的迭代器开始到范围尾部的所有元素删除。这就是删除-擦除法。

```c++
void removeEmptyStrings(vector<string>& strings){
    auto it = remove_if(begin(strings)，end(strings)
[](const string& str){ return str.empty(); })
	// Erase the removed elements.
	strings.erase(it, end(strings)); // [it, end)
}
```

注意：remove函数系列是稳定的，因为这些函数保持了容器中剩余元素的顺序，尽管这些算法将保留的元素向前移动了。

### 6.唯一化

unique算法是特殊的remove，remove能将所有重复的连续元素删除。list 容器提供了自己的具有同样语义的unique方法。通常情况下应该对有序序列使用unique，但unique也能用于无序序列。unique的基本形式是就地操作数据，还有一个名为 unique_copy的版本这个版本将结果复制到一个新的目标范围。

### 7.抽样

sample算法从给定的源范围返回n个随机选择的元素，并存储在目标范围。它需要 5个参数:
要从中抽样的范围的首尾迭代器
目标范围的首迭代器，将随机选择的元素存储在目标范围
要选择的元素数量
随机数生成引擎

```c++
vector<int>vec{ 1,2,3,4,5,6,7,8,9,10 };
const size_t numberOfSamples = 5;
vector<int> samples(numberOfSamples);
random_device seeder;
const auto seed = seeder.entropy() ? seeder() : time(nullptr);
default_random_engine engine(
static_cast<default_random_engine::result_type>(seed));
for (int i=0; i<6; ++i) {
    sample(cbegin(vec)，cend(vec)，begin(samples)，numberOfSamples，engine);
    for (const autok sample : samples) { cout << sample <<"";
    cout << endl;}
}

```

### 8.反转
reverse算法反转某个范围内元素的顺序。将范围内的第一个元素和最后一个元素交换，将第二个元素和倒数第二个元素交换，依此类推。
reverse最基本的形式是就地运行，要求两个参数: 范围的首尾迭代器。还有一个名为 reverse_copy的版本，这个版本将结果复制到新的目标范围，它需要3 个参数:源范围的首尾迭代器以及目标范围的起始迭代器,目标范围必须足够大，以便保存新元素。
9.乱序
shuffle以随机顺序重新安排某个范围内的元素，其复杂度为线性时间。它可用于实现洗牌等任务。参数是要乱序的范围的首尾迭代器，以及一个统一的随机数生成器对象，它指定如何生成随机数。



## 操作算法

此类算法只有两个: for each和 for_each_n，后者是在 c++17 中引入的。它们对范围内的每个元素执行回调，或对范围内的前n个元素执行回调。可利用这两个算法结合简单的函数回调或lambda 表达式执行一些任务，例如打印容器中的每个元素。但使用基于区间的 for 循环通常比使用这两个算法更简单、更容易理解。

### 1. for_each()

```c++
map<int，int> myMap={ {4，401}，{5，50}，{6，60} };
for_each(cbegin(myMap)， cend(myMap)， [](const auto& p)
         {cout << p.first <<“->"
<< p.second << endl; });
```

p的类型是 const pair<int,int>&。

利用lambda计算和与积：

```c++
vector<int> myVector {.....};
int sum=0;
int product =1;
for each(cbegin(myVector)， cend(myVector),
[&sum, &product](int i) {
    sum += i;
	product *= i;
});
```

关于 for_each和 for_each_n，使用 lambda 或回调时，允许通过引用获得参数并对其进行修改, 这样可以修改实际迭代器范围内的值

### 2.for_each_n

算法需要范围的起始选代器、要选代的元素数量以及函数回调。它返回的选代器等于 begin+n。它通常不执行任何边界检查。下例只迭代 map 的前两个元素:

```c++
for_each_n(cbegin(myMap)， 2，
[](const autos p) { cout << p.first << “->"
<< p.second << endl; });
```

## 交换算法

1.std::swap(a,b), 有效的交换两个值，并使用移动语义(如果可用的话)

2.std::exchange, 定义在<utility>头文件中，用新值替换旧值，并返回旧值。

exchange用于实现移动赋值运算符。移动赋值运算符需要将数据从源对象移到目标对象。通常，源对象中的数据会变为 null。

```c++
Foo& operator=(Foo&& rhs) noexcept {
    // Check for self-assignment
    if (this== &rhs) [ return *this; }
    // Free the old memory
    deletemPtr;mPtr =nullptr;
    // Move data
    // Move data from source to destination
    mPtr = rhs.mPtr;
    rhs.mPtr = nullptr;// Nullify datain source
    return *this;
}

//对于方法底部的mPtr和rhs.mPtr的赋值，可使用exchange0实现，如下所示:
Foo& operator=(Foo&& rhs) noexcept {
    // Check for self-assignment
    if (this ==&rhs) ( return *this; }
    // Free theold memory
    deletemPtr;mPtr=nullptr;
    // Move data
    mPtr = exchange(rhs.mPtr,nullptr); // Move + nullify
    return *this;
}

```



## 分区算法

partition_copy算法将来自某个来源的元素**复制到两个不同的目标范围**。为每个元素选择特定目标的依据是谓词的结果: true或 false, true放第一个。partition_copy的返回值是一对迭代器: 一个迭代器引用第一个目标范围内最后复制
的那个元素的后一个位置(one-past-the-last-copied element)，另一个选代器引用第二个目标范围内最后复制的那个元素的后一个位置。将这些返回的迭代器与 erase结合使用，可删除两个目标范围内多余的元素。下例要求用户输入一些整数，然后将这些整数分区到两个目标 vector 中，一个保存偶数，另一个保存奇数。

```c++
vector<int> vec1， vecOdd,vecEven;
populateContainer(vec1);
vecOdd.resize(size(vec1)); // 保证有足够空间
vecEven.resize(size(vec1));
auto pairIters = partition_copy(cbegin(vec1)， cend(vec1)
begin(vecEven)， begin(vecodd)
[](int i)[ return i % 2 ==0;});
vecEven.erase(pairIters.first，end(vecEven)); //删除两个目标范围内多余的元素
vecOdd.erase(pairIters.second,end(vecOdd));
```

partition0算法对序列排序，使谓词返回 true 的所有元素放在前面，使谓词返回 false 的所有元素放在后面
在每个分区中不保留元素最初的顺序。下例演示了如何把 vector 分为偶数在前、奇数在后的分区:

```c++
vector<int> vec;
populateContainer(vec);
partition(begin(vec)，end(vec)，[](int i){ return i%2==0;});
```


## 排序算法

标准库提供了一些不同的排序算法。“排序算法”重新排列容器中元素的顺序，使集合中的元素保持连续顺序。因此，排序算法只能应用于顺序集合。**排序和有序关联容器无关，因为有序关联容器已经维护了元素的顺**
**序。排序也和无序关联容器无关，因为无序关联容器就没有排序的概念。**一些容器(例如 list和 forward list)提供了自己的排序方法，因为这些方法内部实现的效率比通用排序机制的效率要高。

因此，通用的排序算法最适用于vector、deque、array和C风格数组。sort函数一般情况下在0(NlogN)时间内对某个范围内的元素排序。将sort应用于一个范围之后，根据运算符 operator<，这个范围内的元素以非递减顺序排列(最低到最高)。如果不希望使用这个顺序，可以指定一个不同的比较回调，例如greater。
sort函数的一个名为 stable_sort的变体能保持范围内相等元素的相对顺序。然而，由于这个算法需要维护范围内相等元素的相对顺序，因此这个算法比 sort算法低效。

```c++
sort(begin(vec)， end(vec)， greater<>()); // 非递增
```


还有 is_sorted和 s_sorted_until: 如果给定的范围是有序的is sorted就返回 true，而s_sorted_until返回给定范围内的一个迭代器，该迭代器之前的所有元素都是有序的。



## 二叉树搜索算法

有几个搜索算法**只用于有序序列或至少已分区的元素序列**。这些算法有 binary_search、lower_bound，upper_bound和 equal_range。

lower bound、upper bound和 equal range算法类似于 map 和 set 容器中的对应方法。。
lower bound算法在有序范围内查找不小于(即大于或等于)给定值的第一个元素，经常用于在有序的vector 中应将新值插入哪个位置，使 vector 依然有序。

```c++
vec.insert(iter, num);
auto iter = lower bound(begin(vec)，end(vec)，num);
```

binary search算法以对数时间而不是线性时间搜索元素，需要指定范围的首尾迭代器、要搜索的值以及可选的比较回调。如果在指定范围内找到这个值，这个算法返回 true，否则返回 false。



## 集合算法

集合算法可用于任意**有序范围**。includes算法实现了标准的子集判断功能，检查某个有序范围内的所有元素是否包含在另一个有序范围内，顺序任意。includes(a,b)检查b是否是a的子集

set_union ：并集得到的结果是两个集合中的所有元素

set_intersection ：交集得到的结果是所有同时存在于两个集合中的元素

set_difference：差集得到的结果是所有存在于第一个集合中，但是不存在于第二个集合中的元素

 set_symmetric_difference：对称差集得到的结果是两个集合的“异或”:所有存在于其中一个集合中，但不同时存在于两个集合中的元素

这四个的参数都是集合1的首尾迭代器，集合2的首尾迭代器，目标范围的起始迭代器

注意：

1.务必确保结果范围足够大，以保存操作的结果。

对于set_union和set _symmetric_difference，结果大小的上限是两个输入范围的总和。

对于 set_intersection，结果大小的上限是两个输入范围的最小大小。

对于set_difference，结果大小的上限是第一个输入范围的大小

2.不能使用关联容器(包括 set)中的迭代器范围来保存结果，因为这些容器不允许修改键。

merge算法可将**两个排好序的范围**归并在一起，并保持排好的顺序。结果是一个包含两个源范围内所有元素的有序范围。这个算法的复杂度为线性时间O(M+N)。这个算法需要以下参数:

第一个源范围的首尾迭代器
第二个源范围的首尾迭代器
目标范围的起始迭代器
(可选)比较回调
如果没有 merge，还可通过串联两个范围，然后对串联的结果应用sort，以达到同样的目的，但这样做的效率更低，复杂度为 O(Nlog N)而不是merge的线性复杂度。

注意:一定要确保提供足够大的目标范围，以保存归并的结果。



 ## 最大最小算法

min和 max算法通过运算符operator<或用户提供的二元谓词比较两个或多个任意类型的元素，分别返回个引用较小或较大元素的const引用。minmax算法返回一个包含两个或多个元素中最小值和最大值的 pair。
这些算法不接收迭代器参数。还有使用迭代器范围的 min_element、max element和 minmax_element。

```c++
max(x, y);
max({x1, x2, x3, x4});
auto [min1, max1] = minmax({x1, x2, x3, x4});
min_element(vec.begin(), vec.end());
```

std::clamp是一个小型辅助函数，在<algorithm>中定义，clamp（lo，v, hi）可用于确保值(v)在给定的最小值(lo)和最大值(hi)之间。如果v<10, 它返回对10的引用, 如果v>hi，它返回对h的引用:否则返回对V的引用。



## 并行算法

支持并行执行的算法包含选项，接收所谓的执行策略作为第一个参数。执行策略允许指定是否允许算法以并行方式或矢量方式执行。有三类标准执行策略，以及这些类型的三个全局实例，它们全部定义在std:execution 名称空间的<execution>头文件中

![image-20240401205835441](E:\db资料\Note\stl算法.assets\image-20240401205835441.png)

例子:

```c++
sort(std::execution::par，begin(myVector)，end(myVector));
```



## 数值处理算法

### 1.inner_product

<numeric>中定义的inner_product算法计算两个序列的内积，例如，下面程序中的内积计算为 (1x9)+(2x8)+(3x7)+(4x6):

```c++
vector<int>v1 {1, 2, 3, 4};
vector<int>v2 {9, 8, 7, 6};
cout <<inner_product(cbegin(v1)，cend(v1)，cbegin(v2)，0) << endl;
```

输出为70。

### 2. iota()
<numeric>头文件中定义的iota算法会生成指定范围内的序列值，从给定的值开始，并应用operator++来生成每个后续值。下面的例子展示了如何将这个新算法用于整数的 vector，不过要注意这个算法可用于任意实现了operator++的元素类型。

```c++
vector<int> vec(10);
iota(begin(vec)，end(vec)，5);
for (auto& i : vec) cout<<i << " ";
输出如下所示:
5 6 7 8 9 10 11 12 13 14
```

### 3. gcd 和 lcm

gcd算法返回两个整数的最大公约数，而 lcm算法返回两个整数的最小公倍数。