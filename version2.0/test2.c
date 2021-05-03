	int intNum;//测试int类型
	float floatNum;//测试室float类型
	char c; // 测试char类型
	char *s;// 测试字符串类型
	int array[10];//测试数组类型
	int total;//加数之和
	int index;//数组下标索引
	const float constNum=6.6;
	/* 		  
	 * fun函数说明	
	*/
	int fun(int a, float b ,char cc,char *ss)
	{
		int m;
		if (a>b) {
			m=a;
			intNum=666666;
			floatNum=0.999999;
			c='a';
			s="abc";
			total=a+b;//测试操作符
			index=total;
			floatNum=222+666;
			floatNum=0.222+0.666;
			floatNum=0.222+666;
			floatNum=222+0.666;
			array[0]=1;//测试数组赋值
		}
		else =b;
		while (a<)
		{
			if (a==5)
			{
				a=a+2;
			}else
			{
				a=a++;
			}				
		}	
		for (a=0;b<10;b++)
		{
			b=b+1;
			continue;
		}	
		switch (a)
		{
			case 1:
				{
					a=a+1;
					break;
				}
			case 2:
				{
					a=a+2;
					break;
				}
			default:
				{
					a*=3;
					break;
				}
		}		
		return m;
	}
	float x,y;
