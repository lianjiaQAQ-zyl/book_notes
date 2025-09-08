//整个排序的思路其实就是将无序变为有序的过程
//直接插入排序就是每次将无序队列中的一个元素取出来插入到已经排好序的队列中
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void insert_sort(vector<int> &nums){
    int n = nums.size();
    //第一个元素必然有序，现在需要将 nums[i] 插入到前面的某个位置
    for(int i = 1; i < n;i++){
        //如果比 nums[i - 1] 还大，直接插入末尾即可
        //否则，找到插入位置，在这个过程中需要
        if(nums[i] < nums[i - 1]){
            int insert_num = nums[i];
            int j = i - 1;
            for(;j >= 0 && insert_num < nums[j];j--){
                nums[j + 1] = nums[j];
            }
            //找到了该插入的位置，将元素进行插入
            nums[j + 1] = insert_num;
        }
    }
    return;
}
int main(){
    vector<int> nums = {3, 4, 6, 1, 2, 5, 7};
    insert_sort(nums);
    //sort(nums.begin(), nums.end());
    for(auto & num : nums){
        cout << num << "\n";
    }
    return 0;
}