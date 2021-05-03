int a,b,c;
float m,n;
const int ci=1;
int fibo(int a){
    if (a==1||a==2) 
        return 1;
    return fibo(a-1)+fibo(a-2);
}
int main(){
    int m,n,i,array[10][10];
    m=read();
    i=1;
    while (i<m)
    {
        n=fibo(i);
        write(n);
        i=i+1;
        array[0][0]=1;
    }
    return 1;
}