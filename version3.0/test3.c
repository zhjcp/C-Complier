/*
 * 变量命名的类型：
 * 1. v  普通变量
 * 2. temp 临时变量
*/
int a;
int ar[2];
int arr[2][2];
char c;
char *str;
float m,n;
const int ci=1;
int fibo(int a,int b){
    if (a==1||a==2) //测试：包含多个条件的条件转移语句的中间代码 ok
        return 1;
    return fibo(a-1,1)+fibo(a-2,1);//测试：多个函数一起调用的中间代码 ok
}
int main(){
    int m,n,i;
    ar[1]=1;//测试：一维数组调用的中间代码
    arr[0][1]=2;//测试：二维数组调用的中间代码
    m=read();
    i=1;
    i++;//测试：一元+1和二元+1的中间代码 ok
    i=i+1;//测试：一元+1和二元+1的中间代码 ok
    while (i<m)//测试：循环的中间代码 ok
    {
        n=fibo(i,n);
        write(n);
        if(i<3)
            continue;//测试：continue跳转 ok
        i=i+1;
        if(i>3)
            break;//测试：break跳转 ok
    }
    return m;
}