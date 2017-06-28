#include <stdio.h>
#include <stdlib.h>

void prnArr(int arr[], int len)
{
    printf("arr=[");
    for (int i = 0; i < len; i++) {
        if (i == len - 1) {
            printf("%d", arr[i]); 
        } else {
            printf("%d,", arr[i]); 
        }
    }
    printf("]\n");
}

void swap(int indexA, int indexB, int arr[])
{
    arr[indexA] = arr[indexB] - arr[indexA];
    arr[indexB] = arr[indexB] - arr[indexA];
    arr[indexA] = arr[indexB] + arr[indexA];
}

void insertSort(void)
{
    int arr[10] = {34, 32, 67, 12, 8, 27, 84, 14, 58, 22};
    int len = 10;

    int k, tmp;
    for (int i = 1; i < len; i++) {
        k = i;  //k表示待插入的位置
        tmp = arr[k]; //将待插入的值拎出来

        for (int j = i-1; j >= 0; j--) {
            if (arr[j] > tmp) {
                arr[j+1] = arr[j]; //元素后移
                k = j;
            } 
        }

        arr[k] = tmp; //将待插入的值插入位置k
    }

    prnArr(arr, len);
}

void bubbleSort(void)
{
    int arr[10] = {34, 32, 67, 12, 8, 27, 84, 14, 58, 22};
    int len = 10;

    for (int i = 0; i < len-1; i++) {
        for (int j = 0; j < len-1-i; j++) {
            if (arr[j] > arr[j+1]) {
                swap(j, j+1, arr); 
            }  
        } 
    }

    prnArr(arr, len);
}

void bubbleSort_improve(void)
{
    int arr[10] = {34, 32, 67, 12, 8, 27, 84, 14, 58, 22};
    int len = 10;
    int sorted;    //该变量表示是否已排好序，0表示排好序，1表示没有.

    for (int i = 0; (i < len-1) && sorted; i++) {
        sorted = 0;   //假设已排好序
        for (int j = 0; j < len-1-i; j++) {
            if (arr[j] > arr[j+1]) {
                swap(j, j+1, arr); 
                sorted = 1;         //只要发生swap，说明还没排好序
            }  
        } 
    }

    prnArr(arr, len);
}

void shellSort(void)
{
    int arr[10] = {34, 32, 67, 12, 8, 27, 84, 14, 58, 22};
    int len = 10;
    int k, tmp;
    int gap = len;

    do {
        //关于gap的值，下面这条表达式是业界通过大量实验数据得到
        gap = gap / 3 + 1; 

        for (int i = gap; i < len; i += gap) {
            k = i;
            tmp = arr[k];

            for (int j = i-gap; j >= 0; j -= gap) {
                if (arr[j] > tmp) {
                    arr[j+gap] = arr[j]; 
                    k = j;
                } 
            }

            arr[k] = tmp;
        }
    } while (gap > 1);

    prnArr(arr, len);
}

int partition(int arr[], int low, int high)
{
    int pv = arr[low];

    while (low < high) {
        while (low < high && arr[high] >= pv) {
            high--;    //比pv大，且本来就在右边，所以high索引往前移动
        } 
        arr[low] = arr[high];

        while (low < high && arr[low] <= pv) {
            low++;     //比pv小，且本来就在左边，所以low索引往后移动 
        }
        arr[high] = arr[low];
    }

    arr[low] = pv;
    //返回枢轴的位置
    return low;
}

void QuickSort(int arr[], int low, int high)
{
    if (low < high) {
        //选一个pv值，把数据分别放在pv值的左右两边,并把pv值返回
        int pivot = partition(arr, low, high);     

        //对pivot左边的子序列进行快速排序
        QuickSort(arr, low, pivot-1);
        //对pivot右边的子序列进行快速排序
        QuickSort(arr, pivot+1, high);
    }

}

void test_quickSort(void)
{
    int arr[10] = {34, 32, 67, 12, 8, 27, 84, 14, 58, 22};
    int len = sizeof(arr) / sizeof(int);

    QuickSort(arr, 0, len-1);

    prnArr(arr, len);
}

void merge(int src[], int dest[], int low, int mid, int high)
{
    int i = low;
    int j = mid + 1;
    int k = low;

    while (i <= mid && j <= high) {
        if (src[i] < src[j]) {
            dest[k++] = src[i++];
        } else {
            dest[k++] = src[j++]; 
        }
    }

    while (i <= mid) {  //若还剩几个尾部元素
        dest[k++] = src[i++]; 
    }

    while (j <= high) {  //若还剩几个尾部元素
        dest[k++] = src[j++]; 
    }
}

void MergeSort(int src[], int dest[], int low, int high, int max)
{
    if (low == high) { //只有一个元素时，不需要归并，结果赋给dest[low]
        dest[low] = src[low]; 
    } else {  //如果有多个元素，则进行两路划分
        int mid = (low + high) / 2; 
        int *space = (int *)calloc(1, sizeof(int) * max);

        //递归进行两路划分
        //当剩下一个元素时，递归划分结果，然后开始merge归并操作
        if (space != NULL) {
            MergeSort(src, space, low, mid, max); 
            MergeSort(src, space, mid+1, high, max);
            merge(space, dest, low, mid, high);  //调用归并函数进行归并
        }

        free(space);
    }
}

void test_mergeSort(void)
{

    int arr[10] = {34, 32, 67, 12, 8, 27, 84, 14, 58, 22};
    int len = sizeof(arr) / sizeof(int);

    MergeSort(arr, arr, 0, len-1, len);

    prnArr(arr, len);
}

void main(void)
{
    //insertSort();
    //bubbleSort();
    //bubbleSort_improve();
    //shellSort();
    //test_quickSort();
    test_mergeSort();
}
