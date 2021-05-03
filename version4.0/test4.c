int a,b,c;
int ar[2];
int arr[2][2];
float m,n;
int fibo(int a){
    if (a==1||a==2) 
        return 1;
    return fibo(a-1)+fibo(a-2);//测试：递归函数
}

int fiboo(int a){
    return a;
}

int main(){
    int m,n,i;
    ar[1]=1;//测试：一维数组调用
    arr[0][1]=2;//测试：二维数组调用
    write(ar[1]+arr[0][1]);//测试：多个数组之间的运算 + 函数调用
    m=read();//输入m的值  Enter an intege
    i=1;
    while (i<m)
    {
        if (i==2)
        {
            i++;
            continue;//测试: continue ok   i==2时就会执行continue直接进入下一轮循环==> 2不会被打印显示
        }
        n=fiboo(i);
        write(n);//输出n的值  
        if (i==8)  
        {
            break;//测试: break ok   i==8时就会执行break退出循环 ==> 8之后的数不会被打印显示
        }
        i=i+1;
    }
    return 1;
}