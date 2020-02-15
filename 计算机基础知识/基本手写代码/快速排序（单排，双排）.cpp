//快速排序思想：
//选出一个哨兵，通过一趟排序将待排序列分割成两部分
//其中一部分小于哨兵值，另外一部分大于哨兵值
//然后再对两部分分别进行上述操作，直到排序完成


//解释时间复杂度为nlogn
//分治算法，每次选出哨兵，将待排序列分成两部分，一直分下去，直到只有一个元素，平均分开，相当于n个结点的二叉树，共有logn层，递归深度为logn
//每一层都需要对当前分组的元素排序，问题规模大概为n，第一层遍历n个元素，第二层遍历n-1个...直到最后一层为一个
//nlogn

//解释空间复杂度O(logn)~O(n)
//主要是递归造成的栈空间的使用
//最好情况递归树深度为logn，最坏情况为n

//快排的最差情况什么时候发生？
//1. 已排序
//2. 数值全部相等（1的特殊情况）
//在上面的情况下选择的标定元素一直为第一个，则时间复杂度变为O(n^2)

//快速排序，平均情况O(nlogn),最好O(nlogn)，最坏O(n^2)（选择标定元素有关），空间复杂度O(logn)~O(n)，不稳定

/**************************************************************************
**	要取得[a,b)的随机整数，使用(rand() % (b-a))+ a;
**	要取得[a,b]的随机整数，使用(rand() % (b-a+1))+ a;
**	要取得(a,b]的随机整数，使用(rand() % (b-a))+ a + 1;
**	通用公式:a + rand() % n；其中的a是起始值，n是整数的范围。
**	要取得a到b之间的随机整数，另一种表示：a + (int)b * rand() / (RAND_MAX + 1)。
**	要取得0～1之间的浮点数，可以使用rand() / double(RAND_MAX)。
*************************************************************************/


#include <iostream>
using namespace std;

//双路快排，尽量写这种
//begin,begin+1...i...j...end
int partition2(int *data, int start, int end){
	//产生start和end之间的随机数
	int index = (rand() % (end - start + 1)) + start;

	//将选中的数字交换到start位置
	swap(data[index], data[start]);

	int pivot = data[start];

	//选择的pivot为start位置
	//data[start+1, i) <= pivot   i-1 为小于v的最后一个元素，i为当前左边访问的元素
	//data(j, end] >= pivot  j+1 为大于v的第一个元素，j为当前右边访问的元素
	int i = start + 1, j = end;
	while (true){
		while (i <= end && data[i] <= pivot)
			i++;
		while (j >= start + 1 && data[j] >= pivot)
			j--;
		if (i > j)
			break;
		swap(data[i], data[j]);

		//下面这两行，swap交换之后，双方需要移动，否则会增加一次无用的比较
		i++;
		j--;
	}
	//最后j停止在<= v的最后一个位置， i停止在>=pivot的第一个位置，pivot与j进行交换
	swap(data[start], data[j]);
	return j;
}

void swapOffer(int &a, int &b){
	int temp = a;
	a = b;
	b = temp;
}

//单路随机快排
//begin,begin+1...small,small+1...end
int partition(int *data, int start, int end){
	//产生start和end之间的随机数
	int index = (rand() % (end - start + 1)) + start;
	
	//将选中的数字交换到start位置
	swap(data[index], data[start]);

	//选择的pivot为start位置
	int pivot = data[start];
	
	//我们要达到这样的效果
	//data[start+1, small] < v   small为小于pivot的最后一个元素
	//data[small+1, i - 1] >= v   small+1为大于等于pivot的第一个元素
	
	//注意small的取值,small是为了标识小于
	int small = start;
	for (int i = start + 1; i <= end; ++i){
		if (data[i] < pivot){
			//若当前的元素小于pivot，需要将该元素放到data[start+1, small]中紧挨着small位置
			//将small+1和i进行交换，并将small的长度加长
			swap(data[small + 1], data[i]);
			++small;
		}
	}
	//最后将start放在应该的位置，即small和small+1之间，因为左侧全是小于pivot的，因此将small和pivot交换
	swap(data[small], data[start]);
	return small;
}

void quickSortOffer(int *data,int start, int end){
	if (start == end)
		return;
	if (start < end){
		int index = partition2(data,  start, end);
		quickSortOffer(data,  start, index - 1);
		quickSortOffer(data, index + 1, end);
	}
}

int main(){
	int arr[8] = { 2, 1, 3, 78, 78,53, 13, 20 };
	quickSortOffer(arr, 0, 7);  //0表示从数组0位置开始，到4位置排序 
	int i;
	for (i = 0; i<8; ++i){
		printf("%d ", arr[i]);
	}
	system("pause");
	return 0;
}
