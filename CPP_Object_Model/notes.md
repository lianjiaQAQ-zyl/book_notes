[TOC]



# 深入探索C++对象模型

## 第一章 关于对象（Object Lessons）

### 1.1 C++ 对象模式

1. 在 C++ 中，有两种 class member：`static 和 nonstatic`，以及三种 class function：`static, nonstatic, virtual` ；所谓的 C++ 对象模式，就在于编译器如何来存放这些数据，目前 C++ 采用的方式如下：
   * static 数据成员存放在类对象之外， nonstatic 数据成员存放在类对象之内。
   * static 函数和 nonstatic 函数都存放在类对象之外。
   * virtual 函数通过以下方式支持：
     *  每个**类**产生出一堆指向虚函数的函数指针，存放在表格之中，这个表格被称为 virtual table（vtbl）。
     * 每一个**类对象**中添加一个指针，指向相关的 vtbl，这个指针被称为 vptr，vptr 的值由编译器来确定，每一个 class 所关联的 type_info object（用以支持 RTTI）也有 vtbl 来指出，一般位于表格的第一项。
2. C++ 的继承模型中，base class subobject 的 data members 直接放置于 derived class object 中。
3. 虚拟继承的情况下，base class 不管在继承的串链中被派生多少次，永远只会存在一个实体。

### 1.2 关键词所带来的差异

1.  C++ 中的 struct 关键字很大程度上是为了与 C 保持兼容，在 C 里面使用 struct 关键字是为了定义一系列数据的集合，在 C++ 中除了封装数据，也可以封装对于数据的操作，所以 struct 关键字和 class 关键字更多的是逻辑意义上的不同，在 C++ 中，有以下不同：
   * 使用 struct 关键字声明的类默认 access level 是 public，使用 class 关键字声明的类默认 access level 是 private。
   * 因为模板是 C++ 所特有的，所以在 typename 可以和 class 关键字来声明模板参数类型，不支持 struct。
   
2. 由于 `struct` 关键字的语意在 C 与 CPP 中有些许不同，这就导致一些 C 中的技巧在 CPP 中将不再有用，比如：
   ```c
   struct mumble{
       // members
       char pc[1];
   };
   //读取一个 string，然后为 struct 和字符串分配足够的内存
   struct mumble *pmumble = (struct mumble *)malloc(sizeof(struct mumble) + strlen(string) + 1);
   ```

   这里是**柔性数组**的概念，核心目的是在结构体中存放一个只有在运行的时候才能去确定长度的数组。但是在 CPP 中，这种技巧可能不再有用，因为 **CPP 中保证，处于同一个 access level 的数据，必定按照其声明顺序出现在内存布局当中，然而被放置在多个 access level 中的数据，顺序则不一定了。**除此以外，还有以下原因：

   * base object 和 derived object 的布局也没有谁先谁后的强制规定。
   * 虚函数的存在。

### 1.3 对象的差异

1.  CPP 支持三种 programming paradigms:
   * 程序模型，和 C 一样。
   * 抽象数据类型模型，class 定义数据成员和方法。
   * 面向对象模型，定义抽象接口，继承、多态。
2. 只有通过 pointer 和 reference，才能在面向对象模型中实现多态。在 OO 模型中，程序员需要处理一个未知实体，它的类型虽然有所界定，但是由于继承体系，它的实际类型理论上有无限种可能，即被指定的 object 类型在执行点被解析之前，是无法确切知道它的类型的，而对于 ADT 模型中，每个实体在编译时就已经确定好了。
3. CPP 使用下列方式支持多态：
   * 隐含的转换操作，即一个基类类型的指针和引用可以指向派生类类型。
   * 使用虚函数机制，利用虚表来实现多态。
   * 通过 `dynamic_cast` 和 `typeid` 运算符。
4. 不管什么类型的指针，占用的内存大小都是确定的，它的类型只是影响了编译器如何解释某个特定地址中的内存内容及大小。
5. 当我们使用指针进行函数调用的时候，会依据指针的静态类型（声明的类型）在编译期间确定如下两点：
   * 固定的可用接口。即通过某个指针调用函数的时候，这个函数一定要是其静态类型中的接口。
   * 访问级别。通过静态类型中的类的访问接口来判断是否有权进行调用。
6. 指针和引用之所以支持多态，是因为它们并不会引发内存中任何**“与类型有关的内存委托操作”**[^1]，受到影响的仅仅是它们所指向内存的大小和内容解释方式。
7. 假定我们调用p->mem（）（或者obj.mem（）），则依次执行以下4个步骤：
   * 首先确定p（或obj）的静态类型。因为我们调用的是一个成员，所以该类型必然是类类型。
   * 在p（或obj）的静态类型对应的类中查找mem。如果找不到，则依次在直接基类中不断查找直至到达继承链的顶端。如果找遍了该类及其基类仍然找不到，则编译器将报错。
   * 一旦找到了mem，就进行常规的类型检查以确认对于当前找到的mem，本次调用是否合法。
   * 假设调用合法，则编译器将根据调用的是否是虚函数而产生不同的代码：
     * 如果mem是虚函数且我们是通过引用或指针进行的调用，则编译器产生的代码将在运行时确定到底运行该虚函数的哪个版本，依据是对象的动态类型。
     * 反之，如果mem不是虚函数或者我们是通过对象（而非引用或指针）进行调用，则编译器将产生一个常规函数调用。



## 第二章 构造函数语意学

1. 编译器背着程序员做了很多事情，比如会将单参数的构造函数作为类型转换运算符来隐式转换（使用 `explicit` 关键字可以避免），CPP 标准库中 string 类型并没有提供转化为 const char* 的隐式类型转换运算符，而是提供了一个具名实体，即 `string.c_str()`。

### 2.1 Default Constructor 的建构操作

1. CPP 中，当编译器需要的时候，会合成一个 default constructor，这个构造函数只执行编译器所需要的行动。换句话说，如果一个 class 中没有声明任何构造函数，就会有一个 default constructor 被编译器合成，但是这个构造函数大概率会是一个 trivial 的（无能的）。
2. 以下四种情况编译器合成的 constructor 将会是一个 nontrival 的:
   * **”带有 Default Constructor “ 的 Member  Class Object**:
     有两个类 A 和 类 B，类 B 中含有类 A 的对象成员，且没有默认构造函数，类 A 含有默认构造函数，那么编译器就会为类 B 合成一个构造函数，合成的构造函数只会负责类 A 对象的初始化，其他成员（如 char*）的初始化则是程序员自己的事情。**这个合成操作只会在 constructor 真正需要被调用时才发生。**因为 cpp 编译单元的存在，所以在不同的编译单元中，如果定义类 B 对象，就会合成一个构造函数，为了避免链接错误，编译器会使用 `inline` 关键字，如果函数太长，无法 `inline` ，那么编译器就会合成一个 `explicit non-inline static` 的构造函数。**如果类 B 提供了默认构造函数，那么编译器将无法合成新的默认构造，它会将类 A 的默认构造函数内嵌到类 B 的默认构造函数中（若有多个类的类对象，则按照声明顺序插入）。**
   * **"带有 Default Constructor" 的 Base Class**：
     如果派生类中没有任何 constructor，但是基类中有 default constructor，那么编译器就会为派生类合成一个 default constructor，依据基类的声明次序，调用上一层的base class 的 default constructor，对于后继的派生类而言，这个合成的默认构造函数和被明确提供的默认构造函数没有什么差异。
     如果派生类中有其他构造函数但是没有默认构造函数，那么编译器会扩张现有的每一个 constructor，将所有必要的 default constructor 程序代码加进去。（**先基类的，然后member class object的**）
   * **"带有一个 virtual function" 的 class:**
     当一个类中声明或继承一个虚函数的时候，编译器会合成构造函数。这是因为 CPP 对于多态的支持，利用了虚函数表和虚表指针，这些对于程序员是透明的，编译器需要去生成虚表，存放虚函数地址，并且在每一个类对象中，包含虚表地址，这些工作需要编译器去做。
   * **"带有一个 virtual base class 的 class":**
     在实现虚拟继承的时候，不同的编译器之间差异很大，但是每一种实现的共同点在于**必须使得 virtual base class在每一个派生类对象中的位置，能够在执行期准备妥当**。比如我们定义了一个派生类对象指针，用这个指针指向基类中的成员变量，因为这个指针指向的真正类型可以改变，所以编译器必须修改对这个成员变量的访问方式，从而使得可以在运行期间再得到成员变量的实际偏移位置。一种方式是通过一个指针来实现，将这个指针指向虚基类，这就使得需要编译器去修改构造函数，将这个指针存放在类对象中。
3. 总结：编译器合成构造函数只是为了自己使用，它要么通过调用基类或类对象成员变量的构造函数，要么是为了支持虚函数和虚继承机制，除了这几种情况外，实际上并不会合成构造函数（即 implicit trivial default constructors）。且合成的构造函数只会初始化子对象或者基类部分，对于其他 nonstatic data member，那是程序员自己的事情！

### 2.2 Copy Constructor 的建构操作

1. 在 CPP 中，会有以下三种情况将一个 object 的内容作为另一个 class object 的初始值：
   * 明确的赋值操作或初始化操作。
   * object 类型是函数参数类型。
   * object 类型是函数返回值类型。
2. 如果 class 没有提供一个 explicit copy constructor，那么当 class object 以相同 class 的另一个 object 作为初始值时，其内部通过 default memberwise initialization 完成，即将每一个内建的或派生的成员变量，从某个 object 拷贝到另一个 object 身上，不过对于类成员变量，则不会拷贝，而是递归的调用memberwise initialization(这个操作概念上是通过 copy constructor 实现的)。
3. 与默认构造函数的合成一致，CPP 标准将 copy constructor 区分为 trivial 和 nontrivial 两种，只有 nontrivial 的实体才会被合成到程序中，决定一个拷贝构造函数是否为 trivial 的标准在于 class 是否展现出所谓的 bitwise copy semantics（位逐次拷贝）。
4. 以下四种情况一个 class 将不再展现出 bitwise copy semantics，即需要合成拷贝构造函数：
   * 一个类内含某一个类对象，而内含的类提供了拷贝构造。
   * 派生类继承的基类，基类中有拷贝构造（不管是显式声明还是合成的）。
   * 类中有虚函数。（当 base class object 以派生类的object内容做初始化时，类对象的虚表指针不能按位赋值，需编译器干预）
   * 继承串链中有 virtual base class。（当 base class object 以派生类的 object 内容初始化时，编译器必须明确的将 virtual base class pointer/offset 初始化）



### 2.3 程序转化语意学

1. 对于明确的初始化操作（X x1 = x0），编译器会进行程序转化：

   * 重写每一个定义，剔除其中的初始化操作。
   * class 的 copy constructor 调用操作会被安插进去。

2. 参数的初始化：把一个 class object 当作参数传递给一个函数（或是作为一个函数的返回值），相当于以下的初始化操作：
   ```cpp
   X xx = arg; //xx 代表形参或返回值，arg 代表真正的参数值
   ```

   对于一次函数调用，大多数编译器的实现都是生成一个临时对象，然后调用 copy constructor 将其初始化，然后将临时对象作为函数的参数进行函数调用（这就使得需要修改函数声明，将函数声明为引用类型，因为要修改临时对象），当函数完成以后，调用对应的析构函数释放临时对象。

3. 返回值的初始化：对于返回类对象的函数，需要以下转换：

   * 加上一个额外参数，类型是 class object 的一个 reference，该参数用来放置拷贝构造而得到的返回值。
   * 在 return 指令之前安插一个 copy constructor 调用操作，将额外参数的值设置为将要传回的值。

   所以真正的返回值就是这个额外参数，编译器也会改写函数，让函数返回 void。

4. Named Return Value(NRV)优化：在一些函数中，所有的 return 指令传回相同的 named value，那么编译器可以自己做优化，方法是以 result 参数（返回类对象时声明的额外参数）取代 named return  value，这样在函数操作中直接处理 result，省略后续的拷贝构造。

### 2.4 成员初始化列表（Member Initialization List）

1. 必须使用 member initialization list 的情况：
   * 初始化一个 reference member
   * 初始化一个 const member
   * 调用一个 base class 的 constructor，而它拥有一组参数时
   * 调用一个 member class 的 constructor，而它拥有一组参数时
2. 编译器会一一操作 initialization list，以适当次序（在类中的声明次序，与初始化列表中的书写次序无关）在 constructor 之内安插初始化操作，并且在任何 explicit user code 之前。
3. 可以在初始化列表中调用类的成员函数，因为当函数声明结束（即函数形参的括号）时，this 指针已经建构。



## 第三章 Data 语意学

1. 在 CPP 中，即使是一个空类（没有任何成员变量和任何函数），这个类声明的对象的大小也不会是 0，编译器会内置一个 `char` 类型的变量，从而使得定义的两个类对象可以有不同的地址。
2. 在分析一个类对象大小的时候，往往需要考虑：
   * 编译器自动添加的 data member（主要是为了支持 virtual 机制）
   * 为了对齐（alignment）而补充的字节。

### 3.1 Data Member 的绑定

1. cpp中，一个 `inline` 函数实体，在整个 class 声明未被完全看见之前，是不会被评估求值的。（也就是这个函数的定义不会被编译器解析，从而避免将函数中使用的一些成员变量解释成外部的同名变量），**然而，这一策略对于 member function 的 argument list 并不为真。**也就是说，函数的返回值和形参会在第一次出现时就会适当的 resolve。

### 3.2 Data Member 的布局

1. cpp 标准要求，出现在同一个 access level （不同的 private 算不同的 level）中的成员变量（nonstatic），需要满足 **“后出现的成员变量在类对象中具有较高的地址”**，标准并没有要求连续存储（一种常见的情况是有时候为了对齐插入一些字节）。

### 3.3 Data Member 的存取

1. Static Data Members：被编译器提出于 class 之外，并被视为一个 global 变量（仅在 class 范围内），每一个 static data member 只有一个实体，所有针对该实体的引用，都会在内部转化为对唯一实体的直接引用操作。它也是 cpp 语言中，通过对象访问成员变量和通过指针访问成员变量唯一一种没有任何差异的变量。（obj.staticmember，与 objptr->staticmember）不管这个静态成员变量是来自哪里（自己定义还是继承），这个变量都是存放在 data segement 中，不会造成存取的任何负担，这也意味着如果两个类中定义了同名的 static 成员变量，编译器就需要进行名称修饰。
2. NonStatic Data Members：直接存放在一个 class object 之中，除非经过 explicit 或者 implicit （类的成员函数中访问类成员变量，其实是编译器添加了 this 指针）的 class object，否则没有办法进行存取。编译器对于具体数据成员的访问，就是通过类对象的初始地址加上数据成员的偏移量，这个偏移量在**编译期**就可以得知。
3. 对于 NonStatic Data Member 的访问，主要在于偏移量的获知，如果没有虚拟继承，那么这些偏移量都是可以在**编译器**获得的，但是如果访问的是一个从虚基类中继承的成员变量的时候，这个时候只有在运行期才能得到偏移量，所以 obj.member 和 
   objptr->member 就会不一样，前者确定是一个类对象，后者并不清楚 objptr 指向的是什么类对象，所以存取操作必然延迟到执行期间。

### 3.4 “继承” 与 Data Member

在 cpp 继承体系中，一个 derived class object 所表现出来的东西，是其自己的 members 和其 base class members 的总和。至于基类和派生类的成员变量出现在类对象中的先后次序， cpp 标准并没有明确规定。一般的实现是基类在前（地址低），派生类在后面。（虚拟继承除外，很多策略遇到虚拟继承都将不再有效）

#### 只要继承不要多态

1. cpp 标准规定，**出现在 derived class 中的 base class subobject 具有其完整性**，这也就意味着，如果基类的类对象中使用了内存对齐，那么派生类中依然会有这些内存对齐空间的存在，就可能造成空间膨胀。这样子设计的目的是因为当派生类对象被赋值或传递给基类对象时，会发生**对象切片**（基类部分被复制，派生类部分被丢弃）。如果派生类覆盖了基类的填充空间，基类子对象的内存布局会被破坏，导致未定义行为。或者当用一个基类对象去赋值一个派生类对象时，应该只拷贝基类对象有的那些 `member` ，如果不保留这些空闲空间，可能就会导致派生类对象中的成员有非法值。

#### 加上多态

1. 当我们的类中增加了多态的特性（继承与虚函数），势必会给 class 带来空间和时间上的额外开销，主要有：
   * 导入一个和 class 相关的 virtual table，存放它所声明的每一个 virtual function 的地址。
   * 在每一个 class object 中导入一个 vptr，从而使得执行期间每一个 object 可以找到相应的 virtual table。
   * 更改 constructor，使得它可以给每一个 class object 设置正确的 vptr 值。（从基类到派生类）先调用基类的构造函数，将 vptr 设置为基类的虚函数表地址，再调用派生类构造函数，设置为派生类的虚函数表地址。
   * 更改 deconstructor，使得它可以释放 vptr 占用的空间。（从派生类到基类）

#### 多重继承

1. 多重继承的问题主要发生在 derived class 和其第二或后继的 base class 之间的转换关系，对于一个多重派生对象，将其地址指定给第一个 base class 类型的指针，情况与单一继承相似，因为二者指向的是相同的起始地址；但是如果指向第二个 base class 类型的指针，就需要编译器去修改地址。
   ```cpp
   poind3d : public point2d{
       
   };
   vertex3d : public point3d, public vertex{
       
   };
   vertex3d v3d;
   vertex *pv;
   pv = &v3d;
   ```

   我们假设先继承的类在低地址，那么上述操作在执行的时候，编译器就需要额外的操作，**这里需要强制转化为 char * 类型，从而实现按照字节的加减，否则是按照大小进行加减，比如 int * 类型的 +1 其实是加了四个字节**：
   ```cpp
   pv = (vertex *)(((char *)&v3d) + sizeof(point3d));
   ```

   如果是指针之间的赋值（比如讲一个 vertex3d 的指针赋值给一个 vertex 指针），也需要编译器做上述的操作，当然不能简单的直接加减地址，还需要判断派生类的指针是否为 nullptr。

2. CPP 标准并没有要求多重继承的时候哪个类的成员变量在什么位置，编译器可以自行实现，目前常见的实现是按照类的声明顺序进行排布。

3. 即使是多重继承的情况下，数据成员在类对象中的位置在编译期就已经确定了，所以存取并不会产生额外的开销，与单一继承类似，不管是通过指针、引用还是类对象访问数据成员，都是加上一个 offset 就可以了。

#### 虚拟继承

1. 虚拟继承是为了解决继承链中可能存在多个基类的情况，一般的实现方法是，如果一个类中内含一个或者多个 virtual base class，那么将被分割成两部分，一个不变局部和一个共享局部。

   * 不变局部：不管后继如何衍化，总是拥有固定的 offset ，直接存取即可。
   * 表现的就是 virtual base class subobject，这一部分的数据，会因为后续的派生操作出现变化，因为C++ 标准规定，**最终派生类**负责构造所有虚拟基类，所以继承链不同，虚拟基类的偏移就不同，所以需要间接的存取。

   **一般的布局策略是先安排 derived class 的不变部分（即派生类独有的数据成员），再建立起共享部分（共享的基类）**，也就是将共享部分放在类对象的末尾，因为继承链的不同，所以共享部分的偏移也就不同，有以下三种常见的实现方式：

   * 编译器在每一个 derived class object 中安插一些指针，这些指针指向 virtual base class member。这种实现方式会导致安插指针的数量随着虚拟继承的个数增加而增加，同时如果继承链中有多个虚拟继承出现（B虚拟继承 A，C虚拟继承 B），那么就需要多次的间接存取。为了达到固定的存取时间（即不通过继承链深度增加间接性），有些编译器会拷贝操作取得所有的 nested virtual base class 指针，存放在对象中，以空间换取时间。

   * 为了减少指针数量，microsoft 使用了 virtual base class table，每个类对象中安插一个指针，指向这个 table，真正的 virtual class 指针，存放在这个表格当中。

   * 第二种处理方法是在 virtual function table 中存放 virtual base class 的 offset（而不是虚函数地址），即每个 offset 是共享部分到这个类的首地址的偏移量。
     ```
     +------------------+ <-- D对象的起始地址 (也是B子对象的起始地址)
     |   B::b_data      |    // B的不变部分
     +------------------+
     |   B's vbptr      | ----> 指向B的虚拟基类表，其中记录着到A的偏移量 (e.g., offset = 20)
     +------------------+
     |   C::c_data      |    // C的不变部分
     +------------------+
     |   C's vbptr      | ----> 指向C的虚拟基类表，其中也记录着到A的偏移量 (e.g., offset = 12)
     +------------------+
     |   D::d_data      |    // D自己的不变部分
     +------------------+
     |   A::data        | <-- 共享部分A！对于D对象，A被放在这里。
     +------------------+ <-- D对象的结束地址
     ```

### 3.6 指向Data Members 的指针

1. cpp 中提供了一种语言特性，可以声明指向 Data Members 的指针：
   ```cpp
   class point{
       public:
       	float x, y, z;
   };
   float point::*p1 = &point::x; //得到的是x坐标在 class object 中的偏移量
   point obj;
   obj.*p1 = 10;
   ```

   这种指针相当于是相对位置，所以访问的时候要使用具体的类对象。

2. 在一些编译器中，为了区分，会给偏移量加上 1，比如一个指针如果是空，可能是 0，那么对于一个对象的首地址的成员变量，也可能是 0，为了区分空指针，所以就会将首地址变为 1.



## 第四章 Function语意学

### 4.1 Member 的各种调用方式

1. C++ 中的成员函数类型不同，调用方式也不同：
   ```cpp
   obj.func();
   pt->func();
   //非静态成员函数
   name_mangling_func(&obj);
   name_mangling_func(pt);
   //虚函数
   //1. 通过对象调用不支持多态
   //2. 通过指针调用通过虚表指针支持多态
   //静态成员函数
   func();
   func();
   ```

   

   * Nonstatic member function：C++的涉及准则之一即**非静态成员函数应该与非成员函数具有相同的效率**，所以在编译器内部会将一个非静态成员函数转换为非成员函数：
     * 改写函数签名，即在函数的形参中添加 this 指针，该指针是一个常量指针，指向类对象的地址。
     * 在函数体中，将每一个对非静态数据成员的访问改为经由 this 指针访问。
     * 将成员函数重写为一个外部函数，进行名称修饰，使其在程序中成为独一无二的标识符。
   * Virtual member function：利用虚表指针，虚表指针安插在每一个“声明有（或继承自）一个或多个 virtual functions" 的类对象中，然后通过虚表指针找到对应的虚表，再在虚表中拿到对应的函数指针进行调用。 
   * Static member function：转化为一般的非成员函数执行即可，所以对于该类函数有以下限制：
     * 它不能够直接存取 class 中的 nonstatic data member，因为非静态数据成员与类对象进行绑定。
     * 它不能够被声明为 const，volatile 或 virtual，因为这些关键字都与类对象的实例绑定，const不能修改类对象实例，volatile表示类对象实例可能被修改，每次从内存读取。
     * 它不需要经由 class object 进行调用，通过 class object 调用只是语法上的便宜行事。

### 4.2 Virtual Member Functions（虚拟成员函数）

1. 多态指的是**以一个 public base class的指针或者引用，寻址出一个 derived class object**，所以需要在执行期间再来判断指针指向哪一个类型的对象，从而调用正确的成员函数，所以**必须有一些方式能在执行期决议对象的类型信息（runtime type resolution）**。

2. 首先解决什么样的类需要保存这些类型信息？我们无法通过关键字来进行识别，唯一的辨别方式就是通过查看这个类需不需要支持多态，我们可以通过**类中是否含有虚函数（virtual function）来进行判断**。
   其次我们需要清楚存储哪些信息？显然，我们需要知道：1）指针指向的是什么类型的对象，从而使得我们可以调用正确的成员函数。2）成员函数的地址，以便我们可以执行函数。

3. 在整个继承体系当中，每一个 virtual function 都被指派一个固定的索引值，这个索引值在整个继承体系中保持与特定的 virtual function 进行关联。只有 virtual function 才会将其调用地址存放在 virtual function table 之中。

4. **单一继承体系下的 virtual functions**:

   ```cpp
   ptr->z();
   ```

   我们虽然不知道 ptr 指针指向的对象的具体类型，但是通过虚函数表以及虚表指针，我们在编译期间有足够的信息来设定这个虚函数的调用：

   * 虽然不知道 ptr 指向的对象的类型，但可以通过 ptr 拿到虚表指针。
   * 虽然不知道哪一个 `z()` 函数实体会被调用，但是知道该函数位于虚表中的索引值。

   ```cpp 
   *(ptr->vptr[4])(ptr); //this 指针
   ```

5. **多重继承体系下的 virtual functions:**
   这种情况下主要问题在于：

   * 第二个及其后继的 base classes 身上
   * 必须在执行期调整 this 指针

   假如我们的派生类继承自 Base1 基类和 Base2 基类，即：
   ```cpp
   class Derived : public Base1, Public Base2{
       
   };
   ```

   按照对象模型，那么一个 Derived 对象会在开始存放 Base1 的数据成员，然后存放 Base2 的数据成员，最后是 Derived 数据成员，那么：
   ```cpp
   Base2 *pbase2 = new Derived;
   ```

   **那么根据指针的语义要求**，此时必须将 pbase2 指针指向 Derived 对象中的 subobject base2 部分（**个人觉得这也就是为什么一个基类指针可以指向派生类对象的原因，因为一个派生类对象是包含了基类对象的**），所以就需要对 pbase2 指向的地址进行调整：

   ```cpp
   Derived *temp = new Derived;
   Base2 *pbase2 = temp ? temp + sizeof(Base1) : 0;
   ```

   还有一种情况就是:
   ```cpp
   //1. 调用到正确的 virtual destructor 函数
   //2. 实施 delete 运算符
   //3. pbase2 可能需要调整，以便指出完整对象的起始位置
   delete pbase2;
   ```

   **在含有虚函数的基类中，要将析构函数声明为虚函数避免未定义行为**，所以编译器在 delete 这条语句的时候因为**基类指针可以指向派生类对象的缘故**，根本不知道要调用哪一个对象的析构函数，只有在运行期才能得知（虚函数表），同时还要得知相应的 offset，以便将 offset 加到 this 指针。
   利用 **thunk** 技术来在运行期间让编译器插入这一段代码，此时每一个 virtual table slot 继续内含一个指针，要么指向一个实际的 virtual function，要么指向一个相关的 thunk(如果需要调整 this 指针)，它里面有相关 offset 的代码。

6. **虚拟继承下的 virtual functions**:

   也是利用 thunk 技术调整 this 指针。

### 4.4 指向 Member Function 的指针



[^1]: 这里指的是编译器在编译阶段为对象分配内存、确定成员偏移量、安排虚表指针位置等操作时，**必须依据一个具体的、已知的类型**来做出决定。编译器必须“承诺”这个对象在内存中严格按照某个特定类型的布局来组织。
