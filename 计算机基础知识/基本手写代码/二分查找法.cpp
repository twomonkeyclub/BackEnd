/**************************************************************************
**	二分查找法
**	在有序数组中查找target，查找到返回index，否则返回-1
*************************************************************************/
#include <iostream>
using namespace std;

int binarySearch(int arr[], int n, int target){

	//在[l,r]中查找target
	//包含右边界
	int l = 0, r = n - 1;
	int res;
	while (l <= r){
		//int mid = (l + r) / 2;
		//防止溢出
		int mid = l + (r - l) / 2;
		if (arr[mid] == target){
			res = mid;
			break;
		}
		//在[l,mid-1]中查找target
		else if (arr[mid] > target)
			r = mid - 1;
		//在[mid+1,r]中查找target
		else
			l = mid + 1;
	}
	return res;
}


int main(){

	int a[6] = { 0, 1, 2, 3, 4, 5 };
	int res = binarySearch(a, 6, 4);
	cout << res << endl;
	system("pause");
	return 0;
}