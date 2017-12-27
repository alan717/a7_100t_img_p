void printArray(int arr[],int size);
void swap(int arr[],int fi,int si,int d);



void leftRotate(int arr[],int d,int n)
{
    if(d==0||d==n)
        return;

    if(n-d==d)
    {
        swap(arr,0,n-d,d);
        return;
    }

    if(d<n-d)
    {
        swap(arr,0,n-d,d);
        leftRotate(arr,d,n-d);
    }
                

}

/// 交换fi 和si 开始的d个元素
///
///

void swap(int arr[],int fi,int si,int d)
{
    int i,temp;
    for(i=0;i<d;i++)
    {
        temp=arr[fi+i];
        arr[fi+i]=arr[si+i];
        arr[si+i]=temp;
    }
}
