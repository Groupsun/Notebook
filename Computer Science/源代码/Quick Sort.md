看似简单的快排，实现出来分分钟数组越界。

Review More:
```C++
/* Quick Sort Code by SunnyChen */
int Partition(int *a, int left, int right)
{
    int p=left,q=right+1;
    int pilot=a[p];
    while(true)
    {
        while(a[++p]<pilot&&p<q);
        while(a[--q]>pilot);
        if(p>=q)
            break;
        swap(a[p],a[q]);
    }
    a[left]=a[q];
    a[q]=pilot;
    return q;
}

void quicksort(int *a, int left, int right)
{
    if(left>=right)
        return;

    int correct=Partition(a,left,right);
    quicksort(a,left,correct-1);
    quicksort(a,correct+1,right);
}
```
最坏时间复杂度：O(n2)

平均时间复杂度：O(nlogn)

绝大多数的情况都接近平均时间复杂度，可以使用递归树或者主方法来求递归式。当且仅当递归树退化成一个链表的时候进入最坏情况（partition一部分为0，另一部分为n-1）。即使递归树不是平衡的，也几乎都能接近平均时间。并且相对于归并排序，快排不需要另外开一个临时数组，是一个原址排序，排序大规模对象的首选。
