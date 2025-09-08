//算法思想
//对于一个数组，我们选择一个基准元素，然后将比基准元素小的数字都放在左边，比基准元素大的数字都放在右边
//然后对于基准元素的左右再分别进行排序
//分治的思想
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
void quick_sort(vector<int>& nums, int start, int end){
    //左闭右闭，排序 nums[start, end] 使得左侧都比基准元素小，右侧都比基准元素大
    //递归退出条件
    if(start >= end) return;
    //初始化基准元素，左右指针
    int base = nums[start], l = start, r = end;
    while(l < r){
        while(l < r && nums[r] >= base){
            r--;
        }
        nums[l] = nums[r]; //此时 nums[r] 比 base 小
        while(l < r && nums[l] <= base){
            l++;
        }
        nums[r] = nums[l]; //此时 nums[l] 比 base 大
    }
    // l == r，即 base 的位置
    nums[l] = base;
    //递归
    quick_sort(nums, start, l - 1);
    quick_sort(nums, l + 1, end);

}
// void quick_sort(vector<int>& nums, int l, int r){
//     //递归退出条件
//     if(l >= r) return;
//     //初始化基准元素，左右指针
//     int base = nums[l]; //以要排序数组的第一个元素作为基准元素
//     int left = l, right = r; //左闭右闭的模式，也就是要排序[left, right]
//     //循环遍历，移动元素
//     while(left < right){
//         //从右向左找，如果元素比base大，那么right就递减
//         while(left < right && nums[right] >= base){
//             right--;
//         }
//         //此时要么遍历完成，也就是left = right ，要么找到了右边的一个比base小的元素
//         //这里是关键！！！
//         //因为我们使用nums[left]作为基准元素，也就是将nums[left]已经存储了下来，所以相当于我们其实是在找base元素
//         //应该放置的位置，nums[right]的值比base小，那么应该在base的左边，我们直接将空闲出来的那一个区域放置nums[right]
//         //这样nums[right]所在的地方便成了新的空闲
//         nums[left] = nums[right]; 
//         //从左向右找，如果元素比base小，那么left就递增
//         while(left < right && nums[left] <= base){
//             left++;
//         }
//         //此时要么遍历完成，也就是left = right ，要么找到了左边的一个比base大的元素
//         //nums[right]的位置是空闲，放置nums[left]
//         nums[right] = nums[left]; 
//     }
//     //在整个遍历的过程中，我们保证了比base小的全在左边，比base大的全在右边，然后中间的位置就是base
//     nums[left] = base; //循环结束时left = right，也就是说此时空闲位置为nums[left]，我们将其置为base
//     //递归
//     quick_sort(nums, l, left - 1);
//     quick_sort(nums, left + 1, r);
// }
int main(){
    vector<int> nums = {3, 4, 6, 1, 2, 4, 7};
    quick_sort(nums, 0, 6);
    sort(nums.begin(), nums.end());
    for(auto & num : nums){
        cout << num << "\n";
    }
    return 0;
}