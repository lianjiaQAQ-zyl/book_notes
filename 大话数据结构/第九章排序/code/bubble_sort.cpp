#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void bubble_sort(vector<int> &nums){
    int n = nums.size();
    //两两比较相邻记录的关键字，如果反序则交换，直到没有反序的记录。
    //一次循环确定一个记录的位置，那么一共需要 n - 1 次循环
    for(int i = 0; i < n - 1; ++i){
        for(int j = n - 2;j >= i;--j){
            if(nums[j] > nums[j + 1]){
                swap(nums[j], nums[j+1]);
            }
        }
    }
    return;
}

int main(){
    vector<int> nums = {3, 4, 6, 1, 2, 4, 7};
    bubble_sort(nums);
    for(auto &num : nums){
        cout << num << "\n";
    }
    return 0;
}