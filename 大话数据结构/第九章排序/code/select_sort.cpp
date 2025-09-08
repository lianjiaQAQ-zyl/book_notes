//选择排序的思想是每一次从序列中找到最大或者最小的数字，顺序放在已经排好序的序列的最后
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void select_sort(vector<int> &nums){
    int n = nums.size();
    for(int i = 0;i < n;++i){
        int min_index = i;
        for(int j = i + 1;j < n;++j){
            if(nums[j] < nums[min_index]){
                min_index = j;
            }
        }
        // 下标为 i 的不是最小的，需要交换
        if(min_index != i){
            swap(nums[i], nums[min_index]);
        }
    }
    return;
}
int main(){
    vector<int> nums = {3, 4, 6, 1, 2, 4, 7};
    select_sort(nums);
    //sort(nums.begin(), nums.end());
    for(auto & num : nums){
        cout << num << "\n";
    }
    return 0;
}