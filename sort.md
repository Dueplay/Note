## 排序算法分类

基于比较的排序
通过比较大小来决定元素间的相对次序
可以证明时间复杂度下界为 0(logN)-不可能突破这个复杂度达到更快

非比较类排序
不通过比较大小来决定元素间的相对次序

## 基于比较的排序

### 初级排序算法

1.选择排序：每次从未排序数据中找到最小(大)值，放到已排序序列末尾。

2.插入排序 ： 从前到后依次考虑每个未排序数据，在已排序序列中找到合适位置插入

3.冒泡排序： 不断循环扫描，每次查看相邻的元素，如果逆序，则交换

都是O(n^2)

### 堆排序
堆排序(Heap Sort)是对选择排序的优化利用二叉堆高效地选出最小值
建立一个包含所有N个元素的二叉堆
重复N次从堆中取出最小值，即可得到有序序列
时间复杂度 0(NlogN)

```c++
void heap_sort(vector<int> a, int n) {
    priority_queue<int> q; // 大根堆
    for(int i = 0; i < n; i++) {
        q.push(-a[i]);
    }

    for(int i = 0; i < n; i++)  {
		a[i] = -q.top()
		q.pop()
    }
}

```

### 归并排序

归并排序(Merge Sort) 是一个基于分治的算法
时间复杂度 0(NlogN)
原问题:把数组排序
子问题:把数组前一半、后一半分别排序
然后再合并左右两半(两个有序数组)

```c++
void mergesort(vector<int>& nums, int l, int r) { // nums[l...r]
        if (l >= r) return;
        int mid = (l + r) >> 1;
        mergesort(nums, l, mid);
        mergesort(nums, mid + 1, r);
        merge(nums, l, mid, r);
    }

void merge(vector<int>& nums, int l, int mid, int r) {
    int i = l, j = mid + 1;
    vector<int> tem(r - l + 1, 0);
    for (int k = 0; k < tem.size(); k++) {
        if (j > r || (i <= mid && nums[i] <= nums[j])) {
            tem[k] = nums[i++];
        } else {
            tem[k] = nums[j++];
        }
    }
    for (int k = 0; k < tem.size(); k++) {
        nums[l + k] = tem[k];
    }
}
```

### 快速排序

快速排序(Quick Sort) 也是一个基于分治的算法
从数组中选取中轴无素pivot
将小元素放在 pivot 左边，大元素放在右边
然后分别对左边和右边的子数组进行快排

快速排序和归并排序具有相似性，但步骤顺序相反
归并排序: 先排左右子数组(左右内部有序)，然后合并两不有序数组
快速排序:先调配出左右子数组（内部无序），然后对左右子数组分别进行排序

随机选取pivot, 期望时间复杂度 0(NlogN)
如果每次选择的pivot都不好，每次pivot将数组分为一边1和一边n-1个数，会退化为O(N^2)

```c++
void quicksort(vector<int>& nums, int l, int r) {
        if (l == r) return;
        int pivot = partition(nums, l, r);
        quicksort(nums, l, pivot);
        quicksort(nums, pivot + 1, r);
    }

int partition(vector<int>& nums, int l, int r) {
    int pivot = l + rand() % (r - l + 1);
    int pivotval = nums[pivot];
    while (l <= r) {
        while (nums[r] > pivotval) r--;
        while (nums[l] < pivotval) l++;
        if (l <= r) {
            swap(nums[l], nums[r]);
            l++;
            r--;
        }
    }
    return r;
}
```

## 非比较类排序

### 计数排序 (Counting Sort)
计数排序要求输入的数据必须是**有确定范围**的整数。将输入的数据作为 key 存储在额外的cnt数组中，然后依次把计数大于1的填充回原数组
时间复杂度 O(N+M)，N为元素个数，M为数值范围

遍历两次嘛

### 基数排序(Radix Sort)
基数排序把数据切割成一位位数字(0-9)，从低位到高位对每位分别进行计数排序
时间复杂度O(NK)，K 为数字位数

## 排序的稳定性
对于序列中存在的若干个大小相等的元素(重复元素)
如果排序前后它们的相对次序一定保持不变，就称排序算法是稳定的
否则就称排序算法是不稳定的
插入、冒泡、归并、计数、基数和桶排序是稳定的
选择、希尔、快速，堆排序是不稳定的