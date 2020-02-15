# STL
> * [vector如何扩展内存和释放内存](#vector如何扩展内存和释放内存)
> * [STL中各种容器对比](#各种容器对比)
> * [STL中的swap函数](#STL中的swap函数)
> * [STL中哈希表扩容](#STL中的哈希表扩容)
> * [STL迭代器失效的情况和原因](#STL迭代器失效的情况和原因)
> * [vector删除元素后如何避免当前迭代器会失效](#vector删除元素后如何避免当前迭代器会失效)
> * [vector的iterator和const_iterator和const iterator](#vector的iterator和const_iterator和constiterator)

## [vector如何扩展内存和释放内存](https://www.cnblogs.com/biyeymyhjob/archive/2012/09/12/2674004.html)
> * 内存增长
> * [1.5还是2倍扩容](https://blog.csdn.net/dengheCSDN/article/details/78985684)
> * 内存释放

## 各种容器对比

|容器	|底层数据结构|时间复杂度	|有无序	|可不可重复 |	其他|
| :------: | :------: | :------: |:------: |:------: |:------: |
|array		|数组			|	随机读改 O(1)| 无序	|可重复	|支持快速随机访问                                                         |                               
|vector		|数组			|	随机读改、尾部插入、尾部删除 O(1)、头部插入、头部删除 O(n)	|无序	|可重复	|支持快速随机访问                                                         |
|list		|双向链表			|插入、删除 O(1)、随机读改 O(n)								|无序	|可重复	|支持快速增删                                                            |  
|deque		|双端队列			|头尾插入、头尾删除 O(1)									|无序	|可重复	|一个中央控制器 + 多个缓冲区，支持首尾快速增删，支持随机访问                    |           
|stack		|deque / list	|顶部插入、顶部删除 O(1)									|无序	|可重复	|deque 或 list 封闭头端开口，不用 vector 的原因应该是容量大小有限制，扩容耗时   | 
|queue		|deque / list	|尾部插入、头部删除 O(1)									|无序	|可重复	|deque 或 list 封闭头端开口，不用 vector 的原因应该是容量大小有限制，扩容耗时  	|
|priority_queue			|vector + max - heap	|插入、删除 O(log2n)				|有序	|可重复	|vector容器 + heap处理规则                                               |   
|set		|红黑树			|插入、删除、查找 O(log2n)									|有序	|不可重复																		|
|multiset	|红黑树			|插入、删除、查找 O(log2n)									|有序	|可重复  |																		 |
|map		|红黑树			|插入、删除、查找 O(log2n)									|有序	|不可重复|																		|
|multimap	|红黑树			|插入、删除、查找 O(log2n)									|有序	|可重复  |	 |

## [STL中的swap函数](https://blog.csdn.net/ryfdizuo/article/details/6435847)
* 除了数组，其他容器在交换后本质上是将内存地址进行了交换，而元素本身在内存中的位置是没有变化
* swap在交换的时候并不是完全将2个容器的元素互换，而是交换了2个容器内的内存地址。

## STL中的哈希表扩容
* 这里需要知道STL中的swap底层，其实扩容也是vector扩容
    * 创建一个新桶，该桶是原来桶两倍大最接近的质数(判断n是不是质数的方法：用n除2到sqrt(n)范围内的数) ；
    * 将原来桶里的数通过指针的转换，插入到新桶中(注意STL这里做的很精细，没有直接将数据从旧桶遍历拷贝数据插入到新桶，而是通过指针转换两个桶的地址)
    * 通过swap函数将新桶和旧桶交换，销毁新桶

## vector删除元素后如何避免当前迭代器会失效
删除时，将当前的迭代器存起来

## STL迭代器失效的情况和原因
迭代器失效分三种情况考虑，也是分三种数据结构考虑，分别为数组型，链表型，树型数据结构。

* 数组型数据结构
	* 该数据结构的元素是分配在连续的内存中
	* insert和erase操作，都会使得删除点和插入点之后的元素挪位置，所以，插入点和删除掉之后的迭代器全部失效，也就是说insert(*iter)(或erase(*iter))，然后在iter++，是没有意义的。
	* 解决方法：erase(*iter)的返回值是下一个有效迭代器的值。 `iter = cont.erase(iter);`

```C++
//不要直接在循环条件中写++iter
for (iter = cont.begin(); iter != cont.end();)
{
   (*it)->doSomething();
   if (shouldDelete(*iter))
      iter = cont.erase(iter);  //erase删除元素，返回下一个迭代器
   else
      ++iter;
}
```
	
* 链表型数据结构
	* 对于list型的数据结构，使用了不连续分配的内存
	* 插入不会使得任何迭代器失效
	* 删除运算使指向删除位置的迭代器失效，但是不会失效其他迭代器.
	* 解决办法两种，erase(*iter)会返回下一个有效迭代器的值，或者erase(iter++).

* 树形数据结构
	* 使用红黑树来存储数据
	* 插入不会使得任何迭代器失效
	* 删除运算使指向删除位置的迭代器失效，但是不会失效其他迭代器.
	* **erase迭代器只是被删元素的迭代器失效，但是返回值为void**，所以要采用erase(iter++)的方式删除迭代器。


**注意** ：经过erase(iter)之后的迭代器完全失效，该迭代器iter不能参与任何运算，包括iter++,*ite

## vector的iterator和const_iterator和const iterator

* 三种的区别
    * iterator，可遍历，可改变所指元素 
    * const_iterator，可遍历，不可改变所指元素 
    * const iterator，不可遍历，可改变所指元素 
* const_iterator转iterator，iterator不能转const_iterator
    * const_iterator 主要是 **在容器被定义成常量、或者非常量容器但不想改变元素值的情况** 下使用的，而且容器被定义成常量之后，它返回的迭代器只能是const_iterator
    * 有些容器成员函数只接受iterator作为参数，而不是const_iterator。那么，如果你只有一const_iterator，而你要在它所指向的容器位置上插入新元素呢？
        * const_iterator转iterator
        * 强制转换的函数会报错，只能通过 `advance(a, distance(a, b));` 其中，distance用于取得两个迭代器之间的距离，advance用于将一个迭代器移动指定的距离
        * 如果a是iterator，b是const_iterator，distance会报错，可以显式的指明distance调用的模板参数类型，从而避免编译器自己得出它们的类型

```C++
typedef deque<int> IntDeque;
typedef IntDeque::iterator iter;
typedef IntDeque::const_iterator ConstIter;
IntDeque d;
ConstIter ci;
Iter i(d.begin());
advance(i,distance<ConstIter>(i,ci)); 
```