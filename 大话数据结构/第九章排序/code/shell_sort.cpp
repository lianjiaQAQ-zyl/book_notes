//希尔排序是直接插入排序的改进
//他首先对要排序的数据进行分组，对于每一组的元素进行插入排序
//对于数组分组，它使用gap来进行选择，比如第一次选择[0, 0+gap, 0+gap*2, ...]
//然后不断缩小gap的值，直到实现元素有序，也就是gap=1的时候，进行缩小gap的时候常使用不断除以二的方法
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void shell_sort(vector<int> &nums){
    int n = nums.size();
    int increment = n / 2;
    //最外层的循环，来进行 increment 序列的变化，每次除以 2 能保证 increment 序列中有 1
    while(increment > 0){
        //选择每一组的数据来进行插入排序
        for(int i = increment; i < n;++i){
            //针对每一个数据，找到它在其所在组的位置，这里是组之间交替进行的，并不是每次处理完一个组，而是针对元素
            //需要找到 nums[i] 的插入位置，不能直接插入在后面
            if(nums[i] < nums[i - increment]){
                int insert_num = nums[i];
                int j = i - increment;
                for(;j >= 0 && insert_num < nums[j];j = j - increment){
                    nums[j + increment] = nums[j];
                }
                //找到了插入的位置
                nums[j + increment] = insert_num;
            }
        }
        increment = increment / 2;
    }
    return;
}
int main(){
    vector<int> nums = {3, 5, 6, 1, 2, 4, 7};
    shell_sort(nums);
    for(auto & num : nums){
        cout << num << " ";
    }
    return 0;
}