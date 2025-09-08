#include <iostream>
#include <vector>
#include <cmath>

// 合并两个有序子数组的函数
void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // 创建临时数组
    std::vector<int> leftArr(n1);
    std::vector<int> rightArr(n2);

    // 拷贝数据到临时数组 leftArr 和 rightArr
    for (int i = 0; i < n1; i++) {
        leftArr[i] = arr[left + i];
    }
    for (int i = 0; i < n2; i++) {
        rightArr[i] = arr[mid + 1 + i];
    }

    // 合并临时数组到 arr 中
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i];
            i++;
        } else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    // 拷贝剩余的元素
    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}

// 非递归归并排序
void mergeSortIterative(std::vector<int>& arr) {
    int n = arr.size();

    // 遍历子数组大小，子数组从 1 开始逐渐增大到整个数组大小
    for (int subArraySize = 1; subArraySize <= n - 1; subArraySize = 2 * subArraySize) {
        // 从数组的左侧开始合并子数组
        for (int left = 0; left < n - 1; left += 2 * subArraySize) {
            // 设置中间和右侧边界
            int mid = std::min(left + subArraySize - 1, n - 1);
            int right = std::min(left + 2 * subArraySize - 1, n - 1);

            // 合并子数组
            merge(arr, left, mid, right);
        }
    }
}

int main() {
    std::vector<int> arr = {12, 11, 13, 5, 6, 7};

    std::cout << "排序前数组: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    mergeSortIterative(arr);

    std::cout << "排序后数组: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
