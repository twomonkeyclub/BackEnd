//单例思路：静态成员建议在类外进行初始化，但在类内也可以初始化，只是通过类名访问静态成员的属性时，访问不到
//构造函数声明为private或protect防止被外部函数实例化
//内部保存一个private static的类指针保存唯一的实例
//实例的动作由一个public的类方法代劳

//懒汉模式：在getinstance中实例化
//饿汉模式：在单例类定义时实例化

//**********************
//懒汉模式最初实现
//**********************
//#include <iostream>
//
//using namespace std;
//
//class single{
//private:
//	static single *p;
//	single(){}
//	~single(){}
//
//public:
//	static single* getinstance();
//};
//
//single* single::p = NULL;
//single* single::getinstance(){
//	if (NULL == p)
//		p = new single;
//
//	return p;
//}


//**********************
//懒汉模式线程安全经典实现
//**********************
//#include <iostream>
//#include <unistd.h>
//#include <pthread.h>
//
//using namespace std;
//
//
//class single{
//private:
//	static single *p;
//	static pthread_mutex_t lock;
//	single(){
//		pthread_mutex_init(&lock, NULL);
//	}
//	~single(){}
//
//public:
//	static single* getinstance();
//
//};
//pthread_mutex_t single::lock;
//single* single::p = NULL;
//single* single::getinstance(){
//	if (NULL == p){
//		pthread_mutex_lock(&lock);
//		if (NULL == p)
//			p = new single;
//	}
//	pthread_mutex_unlock(&lock);
//	return p;
//}

//**************************************************
//懒汉模式线程安全内部静态变量实现
//将经典实现中的私有唯一实例删掉
//改为在instance函数里定义一个静态的实例
//也可以保证拥有唯一实例，在返回时只需要返回其指针就可以
//**************************************************
//#include <iostream>
//#include <unistd.h>
//#include <pthread.h>
//
//using namespace std;
//
//
//class single{
//private:
//	static pthread_mutex_t lock;
//	single(){
//		pthread_mutex_init(&lock, NULL);
//	}
//	~single(){}
//
//public:
//	static single* getinstance();
//
//};
//pthread_mutex_t single::lock;
//single* single::getinstance(){
//	pthread_mutex_lock(&lock);
//	static single obj;
//	pthread_mutex_unlock(&lock);
//	return &obj;
//}

//************************************************************
//饿汉模式，在定义单例类最初就实例化，此后返回的就一个，感觉相当于全局变量
//在饿汉模式下，在单例类定义的时候就已经定义了一个对象，对类进行了初始化。
//后面不管哪个线程调用成员函数getinstance()，都只不过是返回一个对象的指针而已。
//所以是线程安全的，不需要在成员函数getinstance中加锁。
//************************************************************
#include <iostream>

using namespace std;

class single{
private:
	static single* p;
	single(){}
	~single(){}

public:
	static single* getinstance();

};
single* single::p = new single();
single* single::getinstance(){
	return p;
}

int main(){

	single *p1 = single::getinstance();
	single *p2 = single::getinstance();

	if (p1 == p2)
		cout << "same" << endl;

	system("pause");
	return 0;
}
