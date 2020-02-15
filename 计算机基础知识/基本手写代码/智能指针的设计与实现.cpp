//智能指针的设计与实现
//智能指针类将一个计数器与类指向的对象相关联，引用计数跟踪该类有多少个对象共享同一指针。
//每次创建类的新对象时，初始化指针并将引用计数置为1；
//当对象作为另一对象的副本而创建时，拷贝构造函数拷贝指针并增加与之相应的引用计数；
//对一个对象进行赋值时，赋值操作符减少左操作数所指对象的引用计数（如果引用计数为减至0，则删除对象），并增加右操作数所指对象的引用计数；
//调用析构函数时，构造函数减少引用计数（如果引用计数减至0，则删除基础对象）。
//所有的智能指针都会重载 -> 和 * 操作符。智能指针还有许多其他功能，比较有用的是自动销毁。

#include <iostream>
#include <memory>

template<typename T>
class SmartPointer {
private:
	T* _ptr;
	//在赋值时，需要修改赋值后指针的引用计数
	size_t* _count;
public:
	//初始化
	SmartPointer(T* ptr = NULL) :
		_ptr(ptr) {
		if (_ptr) {
			_count = new size_t(1);
		}
		else {
			_count = new size_t(0);
		}
	}

	//拷贝构造
	SmartPointer(const SmartPointer& ptr) {
		if (this != &ptr) {
			this->_ptr = ptr._ptr;
			this->_count = ptr._count;
			(*this->_count)++;
		}
	}

	//重载=运算符
	SmartPointer& operator=(const SmartPointer& ptr) {
		if (this->_ptr == ptr._ptr) {
			return *this;
		}

		if (this->_ptr) {
			(*this->_count)--;
			if (this->_count == 0) {
				delete this->_ptr;
				delete this->_count;
			}
		}

		this->_ptr = ptr._ptr;
		this->_count = ptr._count;
		(*this->_count)++;
		return *this;
	}

	//重载*
	T& operator*() {
		assert(this->_ptr == nullptr);
		return *(this->_ptr);

	}

	//重载->
	T* operator->() {
		assert(this->_ptr == nullptr);
		return this->_ptr;
	}

	~SmartPointer() {
		//因为引用计数是指针，当智能指针声明为空时，仍需释放
		if (*this->_count == 0) {
			delete this->_ptr;
			delete this->_count;
			std::cout << "释放" << std::endl;
		}
		else
			(*this->_count)--;
		if (*this->_count == 0) {
			delete this->_ptr;
			delete this->_count;
			std::cout << "释放" << std::endl;
		}
	}

	size_t use_count(){
		return *this->_count;
	}
};

int main() {
	{
		//只初始化了两次
		SmartPointer<int> sp(new int(10));
		SmartPointer<int> sp2(sp);
		SmartPointer<int> sp3(new int(20));
		sp2 = sp3;
		std::cout << sp.use_count() << std::endl;
		std::cout << sp3.use_count() << std::endl;
		
		//SmartPointer<int> sp(NULL);
		//std::cout << sp.use_count() << std::endl;
	}
	
	system("pause");
	return 0;
}