	int intNum;//测试:int类型
	float floNum;//测试:float类型
	char c; // 测试:char类型
	char *s;// 测试:字符串类型
	int array[4][4];//测试:数组类型
	int total;//运算结果
	int index;//数组下标索引
	const float cNum=6.6;//测试:常量类型
	/* 		  
	 * fun函数说明	
	*/
	int fun (int a, float b ,char cc,char *ss)
	{
		int m=100;
		//break;//语法错误展示
		if (a>b) {
			m=a;
			intNum=666666;
			floNum=0.999999;
			c='a';
			s="abc";
			total=a+b;//测试:基本运算
			total=a-b;
			total=a*b;
			total=a/b;
			total+=1;//测试:复合运算
			total-=1;
			total*=1;
			total/=1;
			index=total;
			floNum=222+666;//测试:自动类型转换
			floNum=0.222+0.666;
			floNum=0.222+666;
			floNum=222+0.666;
			array[0][1]=1;//测试:数组赋值
		}
		else m=10;
		while (a<10)
		{
			if (a==5)
			{
				a=a+2;
			}else
			{
				//2++;//语法错误展示
				break;
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
