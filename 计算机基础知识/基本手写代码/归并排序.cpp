//两个序列从相同位置往右遍历比较，根据大小重新排列在新数组中的前提是，左右两个序列都是从小到大排列的

//归并排序思想：先递归再归并
//先递归，将左右归并为有序序列，再将左右进行归并
//将左右有序归并时，两个序列从相同位置往右遍历比较，根据大小重新排列在新数组中
//核心函数是merge，Msort函数是递归分治调用merge的过程

//归并排序，平均情况O(nlogn),最好O(nlogn)，最坏O(nlogn)，空间复杂度O(n + logn)，稳定

//解释时间复杂度为nlogn
//一趟归并要将整个序列所有元素遍历一遍，复杂度为O(n)
//递归树的高度为logn，每次递归都要一趟归并
//nlogn

//解释空间复杂度O(n + logn)
//主要是递归造成的栈空间的使用logn，和每次归并都需要将原数组归并到新数组，再拷贝回原数组，和每次归并都需要将原数组归并到新数组，再拷贝回原数组O(n)
//O(n + logn)


#include <iostream>
#include <vector>
using namespace std;

//函数是将sr两侧归并的数据，重新排列进tr中
//sr[s...m, m+1...t]
//int num = 0;
void merge(int sr[], int s, int m, int t){
	int length = t - s + 1;
	int *tr = new int[length];

	int i = s, j = m + 1, k = 0;

	while(i <= m && j <= t){
		if (sr[i] < sr[j])
			tr[k++] = sr[i++];
		else{
			//num += m - i + 1;
			tr[k++] = sr[j++];
		}
	}
	//当两侧数据长度并非完全相等时，只需要将左、右侧两侧剩余数据拷贝即可
	while (i <= m)
		tr[k++] = sr[i++];
	while (j <= t)
		tr[k++] = sr[j++];

	//将辅助数组的元素拷贝到原数组中
	for (k = 0; k < length; k++)
		sr[s + k] = tr[k];
}

//sr[s, t]原数组
void msort(int sr[], int s, int t){

	if (s >= t)
		return;

	//创建平分中点
	int m = s + (t - s) / 2;

	msort(sr, s, m);
	msort(sr, m+1, t);

	//优化，因为左右两侧都是排序好的数组，只有当左侧最大大于右侧最小时才合并，因为合并意味着比较，会浪费时间
	if (sr[m] > sr[m+1])
		merge(sr, s, m, t);

}

int main(){

	int sr[10] = { 2, 1, 3, 78, 87, 53, 13, 20, 0, 10};
	
	msort(sr, 0, 9);

	//std::cout << num << endl;

	for (int i = 0; i < 10; i++){
		std::cout << sr[i] << endl;
	}
	std::system("pause");
	return 0;
}
