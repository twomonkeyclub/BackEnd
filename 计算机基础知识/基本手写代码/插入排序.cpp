//插入排序，平均情况O(n^2),最好O(n)，最坏O(n^2),，空间复杂度O(1)，稳定

/*
插入排序思想
a[5] = {2,1,3,4,5}
外层循环：a[0]不用管，默认有序，从a[1]开始往后循环
内层循环：将需要移动的元素a[1]存入临时变量
		遍历a[1]前面的元素，与当前a[1]进行比较，若遍历的当前元素大于a[1]，元素后移a[j] = a[j - 1]
		直到数据元素小于等于临时变量
		移动完后，将当前位置赋值为临时变量
*/
#include <iostream>
using namespace std;

//频繁交换
void insertSortBad(int *a, int n){
	int i, j;
	for (i = 1; i < n; ++i){
		for (j = i; a[j - 1] > a[j] && j > 0; --j){
			swap(a[j - 1], a[j]);
		}
	}
}
//将频繁交换转换为赋值
void insertSort(int *a, int n){
	//临时变量,存放需要移动的元素
	int temp = 0;
	int i, j;
	for (i = 1; i < n; ++i){
			temp = a[i];
			for (j = i; a[j - 1] > temp && j > 0; --j){
				a[j] = a[j - 1];
			}
		a[j] = temp;
	}
}

int main(){
	int a[7] = { 2, 1, 3, 4, 5, 9, 8 };
	insertSortBad(a, 7);
	for (int i = 0; i < 7; ++i)
		cout << a[i] << endl;

	system("pause");
	return 0;
}