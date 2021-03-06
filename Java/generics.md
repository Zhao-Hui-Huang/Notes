# 泛型
[Oracle Generics](https://docs.oracle.com/javase/tutorial/java/generics/index.html)

## 为什么使用泛型
- 在编译时可以进行强类型检查;
- 消除cast;
- 使程序员能够实现更通用的算法;

### 原始类型(Raw Types)
在使用泛型类的时候不指定类型参数。注意：非泛型类不是原始类型。

因为在JDK5.0以前，许多API类不是泛型的（如`Collections`类），所以原始类型出现在遗留代码。当使用原始类型时，你得到了一个预泛化的表现。为了后向兼容，将一个参数化的类型分配给他的原始类型是允许的。

```java
Box<String> stringBox = new Box<>();
Box rawBox = stringBox;               // OK
```

## 泛型方法
泛型方法是拥有自己类型参数的方法。在调用时，可以显式提供类型参数，也可以由编译器推断所需要的类型。

## 边界类型参数
限制可以用来作为类型参数的参数类型。
### 泛化方法和边界类型参数

## 泛型、继承与子类型
![泛型示例](https://docs.oracle.com/javase/tutorial/figures/java/generics-subtypeRelationship.gif)
> **注意：**给定两个具体的类型A和B，（例如：`Number`和`Integer`），不管类`A`和类`B`是否有关系，类`MyClass<A>`与类`MyClass<B>`无关。`MyClass<A>`与`MyClass<B>`公有的父类都是`Object`类。
### 泛型类与子类型

## 类型推断
### 类型推断与泛化方法
由于类型推断的存在使得你在调用泛化方法时可以像调用普通方法那样使用。
### 类型推断与泛化类
只要编译器能从上下文推断出类型参数的类型，你就可以通过一个空的尖括号来调用泛化类的构造器。
### 类型推断与泛化构造器
> **类型推断算法只使用调用参数，目标类型和可能明显期待的返回类型来进行类型推断。**

## 通配符
在泛化代码中，`?`被称为*通配符*，代表未知类型。通配符可以在以下几种场景：作为参数、域、本地变量的类型；有时也可以作为返回类型（虽然更优的编程实践是返回更具体的类型）。通配符不能作为泛化方法调用的类型参数，泛化类实例的的创建，或者是超类。
### 上界通配符
用于放宽对变量的限制。
### 无边界通配符
无界通配符适用于下面两种场景：

- 写一个可以通过`Object`类中方法实现的函数。
- 当泛化类中的代码不依赖类型参数。例如：`List.size`和`List.clear`。实际上，`Class<?>`经常被使用，因为`Class<T>`中的大部分代码不依赖T。

对于所有的具体类型A，`List<A>`是`List<?>`类型的子类型。

`List<Object>`与`List<?>`不同。`List<Object>`中可以插入`Object`或者任何`Object`的子类型。但是，`List<?>`中只能插入`null`。

### 下界通配符
### 通配符与子类型
![class Tree](https://docs.oracle.com/javase/tutorial/figures/java/generics-listParent.gif)

`List<Number>`与`List<Integer>`共有的父类为`List<?>`。

[A hierarchy of several generic List class declarations.](https://docs.oracle.com/javase/tutorial/figures/java/generics-wildcardSubtyping.gif)

### 类型捕获和Helper方法
### 通配符使用指导
**In**变量：向代码传递数据，如`copy(src, dest)`中的`src`。

**Out**变量：保存数据以便在其它地方使用，如`copy(src, dest)`中的`dest`。

## 类型消除
- 用bounds或者`object`来替换泛型中所有的类型参数。因些，生成的类中仅包含普通的类，接口和方法。
- 如果必要插入cast来保证类型安全
- 在扩展的泛型中生成桥接方法来保证多态
类型擦除确保不为参数化类型创建新类;因此，泛型不会产生运行时开销。
### 泛化类型的消除
在类型消除过程中，如果类型参数是有界的，Java编译器会将其替换为第一个边界类；如果是无界的，会将其替换为`Object`类。

### 泛化方法中的消除

### 类型消除的影响和桥接方法

### 非可具体化类型
#### 非可具体化类型
一个可具体化类型指的是类型信息在运行时完全可用。包括基本类型，非泛型，原始类型和无界通配符的调用。

非具体化类型，指的是类型信息在编译时被消除的那些类型，如没有被定义为无界类型的泛型的调用。一个非具体化类型在运行时没有它的具体信息可用。一个非具体化类型如`List<String>`和`List<Number>`；在运行时，JVM不能区分出这两种类型。正如在泛型的限制里面所述，在一些场景中是不可以使用泛型，如：在`instanceof`表达式中和数组中的元素。

#### 堆污染
堆污染指的是变量引用的对象类型与它自己的类型不符。这种情况通常发生在编译时程序进行了一些抛出未检查的异常的操作。例如：当混合使用原始类型或者是进行未检查的类型转换时，容易发生堆污染。

## 泛型的限制
- 不能使用基本类型初始化泛型
- 不能创建类型参数实例
- 不能声明类型为类型参数的静态域
- 不能向参数化类型使用`cast`或者`instanceof`
- 不能创建参数化类型的数组
- 不能创建、捕获和抛出参数化类型的对象


