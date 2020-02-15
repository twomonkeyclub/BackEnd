//堆排序思想：将待排序列先构造一个最大堆，堆顶元素即为最大值，然后移走堆顶元素，调整剩余元素为最大堆，如此反复执行，便得到有序序列
//若从小到大排列，建立最大堆
//若从大到小排列，建立最小堆

//堆排序，平均情况O(nlogn),最好O(nlogn)，最坏O(nlogn),空间复杂度O(1)，不稳定

//解释时间复杂度O(nlogn)
//建立堆的过程是nlogn,遍历所有非叶子结点，将其和子树调整为最大堆
//调整堆的过程是logn，整体是nlogn

//由数组变为最大堆有两种方式，一种是逐个插入（O(n)），一种是heapify(从第一个非叶子结点开始调整，bottom to up)（O(1)）
//大话数据结构书上的排序基本都是数组的0位置不放元素，从1开始
//这里从0位置开始
#include <iostream>
#include <vector>
using namespace std;

void build_max_heap(vector<int> &a, int len);
void heapadjust(vector<int> &a, int i, int len);

void heap_sort(vector<int> &a) {
	int len = a.size();
	build_max_heap(a, len);
	for (int i = len - 1; i >= 1; i--) {
		//将堆顶的元素和最后一个元素交换位置
		swap(a[0], a[i]);
		
		//每次将数组前面的部分再调整为最大堆，后续调整的时候传进去的就是当前数组能取到的最大下标了，因为i赋初值时已经解决
		heapadjust(a, 0, i - 1);	
	}
}
//建堆的意思是将数组转换成最大堆
//本质：从下往上，从右往左，将每个非叶子结点当做根节点，将其和子树调整成最大堆，首先找到最后一个叶子结点的父节点len / 2 - 1
//最大堆，每个结点都大于等于左右孩子结点的值
void build_max_heap(vector<int> &a, int len) {	//建堆
	for (int i = len / 2 - 1; i >= 0; i--) {
		heapadjust(a, i, len - 1);				//建堆的时候应该传进去len-1，即当前数组能取到的最大下标，即保证堆调整过程中左右孩子不会数组越界
	}
}

//以i当做根节点，调整其和其子树为最大堆
void heapadjust(vector<int> &a, int i, int len){
	int temp, j;
	temp = a[i];
	
	//当前结点为j，左孩子序号是2*i，右孩子序号是2*i+1
	//这里的结点从0开始，不是从1开始，因此左孩子序号是2*i + 1，右孩子序号是2*i+2
	for (j = 2*i+1; j <= len; j *= 2){
		//先比较左右孩子,找到左右孩子中较大的那个
		if (j < len && a[j] < a[j+1])
			++j;

		//如果左右孩子中最大值小于当前值，则不需要移动
		if (a[j] <= temp)
			break;
		//若最大值大于当前值，需要将最大值往前移动，赋给i位置
		//此时不要动j，直到找到j所在位置再赋值
		a[i] = a[j];

		//将当前值和左右孩子比较后，若移动了，则继续往下比较，将j向下移动到刚才较大的孩子处
		i = j;
	}
	//直到找到temp的合适位置
	a[i] = temp;
}

int main(){
	vector<int>a;
	a.push_back(7);
	a.push_back(6);
	a.push_back(5);
	a.push_back(4);
	a.push_back(1);
	a.push_back(2);
	a.push_back(3);
	heap_sort(a);

	for (int i = 0; i<7; ++i){
		printf("%d ", a[i]);
	}

	system("pause");
	return 0;
}
