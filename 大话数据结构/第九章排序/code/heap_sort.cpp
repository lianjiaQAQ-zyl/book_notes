#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;
//使用数组模拟
void max_heapify(vector<int> &nums, int start, int end){
    //负责调整数组内容，使得 nums[start ... end] 是一个大顶堆
    int dad = start;
    int left_son = 2 * dad + 1;
    //下标的范围在区间内再考虑
    while(left_son <= end){
        //选择孩子中关键字较大的孩子
        int choice_son = left_son;
        int right_son = left_son + 1;
        if(right_son <= end && nums[right_son] > nums[left_son]){
            choice_son = right_son;
        }
        //父亲节点比儿子节点大，不需要调整
        if(nums[dad] > nums[choice_son]){
            return; 
        }
        //调整，然后继续调整子树
        else{
            swap(nums[dad], nums[choice_son]);
            dad = choice_son;
            left_son = 2 * dad + 1;
        }
    }
}
void heap_sort(vector<int> &nums){
    int n = nums.size();
    //从最后一个父节点开始调整，最后一个节点的下标是 n - 1
    //若为左孩子，n 为偶数，则 2 * dad + 1 = n - 1, dad = (n - 2) / 2 = n / 2 - 1
    //若为右孩子，n 为奇数，则 2 * dad + 2 = n - 1, dad = (n - 3) / 2 = (n - 1) / 2 - 1 = n / 2 - 1
    //综上： dad = n / 2 - 1
    for(int i = n / 2 - 1; i >= 0; --i){
        max_heapify(nums, i, n - 1);
    }
    for(int i = 0;i < nums.size();++i){
        cout << nums[i] << "\n";
    }
    for(int i = n - 1; i > 0; --i){
        swap(nums[0], nums[i]); //与根节点交换，然后进行调整
        max_heapify(nums, 0, i - 1);
    }
}
int main(){
    vector<int> arr = {3, 5, 3, 0, 8, 6, 1, 5, 8, 6, 2, 4, 9, 4, 7, 0, 1, 8, 9, 7, 3, 1, 2, 5, 9, 7, 4, 0, 2, 6 };
    heap_sort(arr);
    for (int i = 0; i < arr.size(); i++)
        cout << arr[i] << ' ';
    cout << endl;
    return 0;
}