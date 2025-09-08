//归并排序采用的是分治的思想，每一次将数据量减半，然后排完序之后再将数据进行合并

#include <iostream>
#include <vector>
using namespace std;

void merge(vector<int> &nums, int start, int mid, int end){
    //使用辅助数组分别存放这两个有序数组
    vector<int> leftArray(nums.begin() + start, nums.begin() + mid + 1);
    vector<int> rightArray(nums.begin() + mid + 1, nums.begin() + end + 1);
    //合并两个有序数组
    int leftId = 0, rightId = 0;
    int i;
    for(i = start;i <= end && leftId < leftArray.size() && rightId < rightArray.size();i++){
        if(leftArray[leftId] < rightArray[rightId]){
            nums[i] = leftArray[leftId];
            leftId++;
        }
        else{
            nums[i] = rightArray[rightId];
            rightId++;
        }
    }
    while(leftId < leftArray.size()){
        nums[i] = leftArray[leftId];
        i++;
        leftId++;
    }
    while(rightId < rightArray.size()){
        nums[i] = rightArray[rightId];
        i++;
        rightId++;
    }
    return ;
}
void merge_sort(vector<int> &nums, int start, int end){
    //递归退出条件，此处是[start, end]，左闭右闭
    if(start >= end) return;
    //进行递归
    int mid = start + (end - start) / 2;
    merge_sort(nums, start, mid);
    merge_sort(nums, mid + 1, end);
    //上述过程结束以后，我们得到了 [start, mid]，[mid + 1, end] 两个有序数组，问题就转化为了合并有序数组
    merge(nums, start, mid, end);
}
int main(){
    vector<int> nums = {3, 4, 6, 1, 2, 4, 7};
    merge_sort(nums, 0, 6);
    //sort(nums.begin(), nums.end());
    for(auto & num : nums){
        cout << num << "\n";
    }
    return 0;
}