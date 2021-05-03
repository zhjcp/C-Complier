int a;
int array[3][5];
char c;
char c;//错误：变量重复定义
char *str;
float m,n;
const int ci=1;
int fibo(int a){
    if (a==1||a==2) 
        return 1;
    return fibo(a-1)+fibo(a-2);
}
int fibo(int a){
    //错误：函数重复定义
}
char fiboc(char cc){
    fibo[0][0]=1;//错误：对函数使用下标访问
    m[0][0]=2;//错误：对非数组变量使用下标访问
    fun(a);//错误：使用未定义函数
    fibo();//错误：函数调用参数过少
    fibo(c);//错误：函数调用参数类型不匹配
    fibo(a,c);//错误：函数调用参数过多   
    m();//错误：误用变量名作函数名
    a=fibo;//错误：误用函数作为普通变量
    2++;//错误：对常量使用自增、自减
    return 'a';
}
int main(){
    int m,n,i;
    m=read();
    i=1;
    xxx=2;//错误：使用未定义变量
    2=1;//错误：赋值表达式左边
    break;//错误：循环外的break
    continue;//错误：循环外的continue
    while (i<m)
    {
        n=fibo(i);
        write(n);
        i=i+1;
        break;
        continue;
    }
    return 'a';//函数返回值类型错误
}