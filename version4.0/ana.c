#include "def.h"
int global_returnType=0;
int global_inCicle=0;
char global_Snext[15];//用于循环体内的continue转移
char global_Efalse[15];//用于循环体内的break转移
char *lb="[";
char *rb="]";
char dess[33];


void mmyStrcat(char *dess,int index){//完成数组字符串的拼接 拼接标识符
        char s[10];
        itoa(index,s,10);
        dess=strcat(dess,lb);
        dess=strcat(dess,s);
        dess=strcat(dess,rb);
}

/*工具函数：拼接字符串，由于C函数的特点，使用static这种有效却不好的做法实现数据返回*/
char *strcat0(char *s1,char *s2){
    static char result[10];
    strcpy(result,s1);
    strcat(result,s2);
    return result;
}

/*新增别名，利用了static int no来记录已经存在的别名数量  v1 v2 v3 ...... 递增的数字后缀*/
char *newAlias() {
    static int no=1;
    char s[10];
    itoa(no++,s,10);
    return strcat0("v",s);
}

/*新增标签，利用了static int no来记录已经存在的别名数量*/
char *newLabel() {
    static int no=1;
    char s[10];
    itoa(no++,s,10);
    return strcat0("label",s);
}

/*新增临时变量,利用了static int no来记录已经存在的别名数量*/
char *newTemp(){
    static int no=1;
    char s[10];
    itoa(no++,s,10);
    return strcat0("temp",s);
}

//实验三：生成一条TAC代码的结点组成的双向循环链表，返回头指针
struct codenode *genIR(int op,struct opn opn1,struct opn opn2,struct opn result){
    struct codenode *h=(struct codenode *)malloc(sizeof(struct codenode));
    h->op=op;
    h->opn1=opn1;
    h->opn2=opn2;
    h->result=result;
    h->next=h->prior=h;
    return h;
}

//实验三：生成一条标号语句，返回头指针
struct codenode *genLabel(char *label){
    struct codenode *h=(struct codenode *)malloc(sizeof(struct codenode));
    h->op=LABEL;
    strcpy(h->result.id,label);
    h->next=h->prior=h;
    return h;
}

//实验三：生成GOTO语句，返回头指针
struct codenode *genGoto(char *label){
    struct codenode *h=(struct codenode *)malloc(sizeof(struct codenode));
    h->op=GOTO;
    strcpy(h->result.id,label);
    h->next=h->prior=h;
    return h;
}

//实验三：合并多个中间代码的双向循环链表，首尾相连
struct codenode *merge(int num,...){
    struct codenode *h1,*h2,*p,*t1,*t2;
    va_list ap;
    va_start(ap,num);
    h1=va_arg(ap,struct codenode *);
    while (--num>0) {
        h2=va_arg(ap,struct codenode *);
        if (h1==NULL) h1=h2;
        else if (h2){
            t1=h1->prior;
            t2=h2->prior;
            t1->next=h2;
            t2->next=h1;
            h1->prior=t2;
            h2->prior=t1;
            }
        }
    va_end(ap);
    return h1;
}

//实验三：输出中间代码
void prnIR(struct codenode *head){
    char opnstr1[32],opnstr2[32],resultstr[32];
    struct codenode *h=head;
    do {
        if (h->opn1.kind==INT)
             sprintf(opnstr1,"#%d",h->opn1.const_int);
        if (h->opn1.kind==FLOAT)
             sprintf(opnstr1,"#%f",h->opn1.const_float);
        if (h->opn1.kind==CHAR)
             sprintf(opnstr1,"#%c",h->opn1.const_char);
        if (h->opn1.kind==STRING)
             sprintf(opnstr1,"#%s",h->opn1.string);
        if (h->opn1.kind==ID)
             sprintf(opnstr1,"%s",h->opn1.id);
        if (h->opn2.kind==INT)
             sprintf(opnstr2,"#%d",h->opn2.const_int);
        if (h->opn2.kind==FLOAT)
             sprintf(opnstr2,"#%f",h->opn2.const_float);
        if (h->opn2.kind==CHAR)
             sprintf(opnstr2,"#%c",h->opn2.const_char);
        if (h->opn2.kind==STRING)
             sprintf(opnstr2,"#%s",h->opn2.string);
        if (h->opn2.kind==ID)
             sprintf(opnstr2,"%s",h->opn2.id);
        sprintf(resultstr,"%s",h->result.id);
        switch (h->op) {
            case ASSIGNOP:  printf("  %s := %s\n",resultstr,opnstr1);
                            break;
            case PLUS:
            case MINUS:
            case STAR:
            case DIV: printf("  %s := %s %c %s\n",resultstr,opnstr1, \
                      h->op==PLUS?'+':h->op==MINUS?'-':h->op==STAR?'*':'\\',opnstr2);
                      break;
            case SELFPLUS:  //自定义：自增、自减的中间代码==源操作数2是int 1的加减法
            case SELFMINUS:
                           printf("  %s := %s %c %s\n",resultstr,opnstr1, \
                           h->op==SELFPLUS?'+':'-',opnstr2);
                           break;
            case FUNCTION: printf("\nFUNCTION %s :\n",h->result.id);
                           break;
            case PARAM:    printf("  PARAM %s\n",h->result.id);
                           break;
            case LABEL:    printf("LABEL %s :\n",h->result.id);
                           break;
            case GOTO:     printf("  GOTO %s\n",h->result.id);
                           break;
            case JLE:      printf("  IF %s <= %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                           break;
            case JLT:      printf("  IF %s < %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                           break;
            case JGE:      printf("  IF %s >= %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                           break;
            case JGT:      printf("  IF %s > %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                           break;
            case EQ:       printf("  IF %s == %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                           break;
            case NEQ:      printf("  IF %s != %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                           break;
            case ARG:      printf("  ARG %s\n",h->result.id);
                           break;
            case CALL:     if (!strcmp(opnstr1,"write"))
                                printf("  CALL  %s\n", opnstr1);
                            else
                                printf("  %s := CALL %s\n",resultstr, opnstr1);
                           break;
            case RETURN:   if (h->result.kind)
                                printf("  RETURN %s\n",resultstr);
                           else
                                printf("  RETURN\n");
                           break;
            case BREAK:     printf("  GOTO %s\n",h->result.id);//break转移
                            break;
            case CONTINUE:  printf("  GOTO %s\n",h->result.id);//continue转移
                            break;
        }
    h=h->next;
    } while (h!=head);
}


//这里可以只收集错误信息，最后一次显示
void semantic_error(int line,char *msg1,char *msg2){
    //这里可以只收集错误信息，最后一次显示
    printf("在%d行,%s %s\n",line,msg1,msg2);
}



//显示符号表  
void prn_symbol(){ 
    int i=0;
    printf("   %9s    %6s   %6s    %6s    %4s    %6s\n","变量名","别 名","层 号","类  型","标记","偏移量");
    for(i=0;i<symbolTable.index;i++)       
        printf("%9s  %6s %6d     %6s    %4c    %6d\n",symbolTable.symbols[i].name,\
                symbolTable.symbols[i].alias,symbolTable.symbols[i].level,\
                symbolTable.symbols[i].type==INT?"int":(symbolTable.symbols[i].type==FLOAT?"float":(symbolTable.symbols[i].type==CHAR?"char":"char *")),\
                symbolTable.symbols[i].flag,symbolTable.symbols[i].offset);
}


//查找符号 好像是从后往前查找（大概因为使用的是顺序表，index表示当前符号的位置，也就是最后一个符号的位置）
int searchSymbolTable(char *name) {
    int i;
    for(i=symbolTable.index-1;i>=0;i--)
        if (!strcmp(symbolTable.symbols[i].name, name))  return i;
    return -1;
}


//插入符号  要查重
int fillSymbolTable(char *name,char *alias,int level,int type,char flag,int offset) {
    //首先根据name查符号表，不能重复定义 重复定义返回-1
    int i;
    /*符号查重，考虑外部变量声明前有函数定义，
    其形参名还在符号表中，这时的外部变量与前函数的形参重名是允许的*/
    for(i=symbolTable.index-1; i>=0 && (symbolTable.symbols[i].level==level||level==0); i--) {
        if (level==0 && symbolTable.symbols[i].level==1) continue;  //外部变量和形参不必比较重名
        if (!strcmp(symbolTable.symbols[i].name, name))  return -1; //注意：！strcmp代表重名 
        }
    //在不引发命名冲突的前提下，插入到符号表中
    strcpy(symbolTable.symbols[symbolTable.index].name,name);//名字
    strcpy(symbolTable.symbols[symbolTable.index].alias,alias);//别名
    symbolTable.symbols[symbolTable.index].level=level;//层次
    symbolTable.symbols[symbolTable.index].type=type;//类型
    symbolTable.symbols[symbolTable.index].flag=flag;//符号标记 V F ......
    symbolTable.symbols[symbolTable.index].offset=offset;//偏移  偏移是如何确定的？？？？？？？？？
    return symbolTable.index++; //返回的是符号在符号表中的位置序号，中间代码生成时可用序号取到符号别名
}


//填写临时变量到符号表，返回临时变量在符号表中的位置  不查重
int fill_Temp(char *name,int level,int type,char flag,int offset) {
    strcpy(symbolTable.symbols[symbolTable.index].name,"");//注意：插入的名字是空字符串
    strcpy(symbolTable.symbols[symbolTable.index].alias,name);
    symbolTable.symbols[symbolTable.index].level=level;
    symbolTable.symbols[symbolTable.index].type=type; 
    symbolTable.symbols[symbolTable.index].flag=flag;
    symbolTable.symbols[symbolTable.index].offset=offset;
    return symbolTable.index++; //返回的是临时变量在符号表中的位置序号
}



int LEV=0;      //层号
int func_size;  //1个函数的活动记录大小


//处理变量列表：  继承属性往后传递  将变量插入符号表
//EXT_DEC_LIST结构：a,b,c,d 而且是一颗二叉树（从parser.y的构造语句中可以看出）
// 二叉树结构==> 只需要递归地遍历 ptr[0]和ptr[1]
void ext_var_list(struct ASTNode *T){  //处理变量列表
    int rtn,num=1;
    struct ASTNode *T0;
    int indexPlace=0;//作用：记录当前要操作的arrayIndex位置
    switch (T->kind){
        case EXT_DEC_LIST:// 1. 处理变量列表的根节点
                T->ptr[0]->type=T->type;              //继承：将类型属性向下传递变量结点
                T->ptr[0]->offset=T->offset;          //继承：外部变量的偏移量向下传递
                T->ptr[1]->type=T->type;              //继承：将类型属性向下传递变量结点
                T->ptr[1]->offset=T->offset+T->width; //继承：外部变量的偏移量向下传递
                T->ptr[1]->width=T->width;     //继承：变量占用的宽度
                ext_var_list(T->ptr[0]);       //递归处理
                ext_var_list(T->ptr[1]);       //递归处理
                T->num=T->ptr[1]->num+1;       //参考parser.y  ptr[1]:指变量列表ExtDecList   1:指变量VarDec  
                break;
        case ID: // 2. 处理变量列表的子节点 即：变量的标识符ID
            rtn=fillSymbolTable(T->type_id,newAlias(),LEV,T->type,'V',T->offset);  //顺序插入 别名的数字后缀递增
            if (rtn==-1)
                semantic_error(T->pos,T->type_id, "变量重复定义");
            else T->place=rtn; // rtn:符号在符号表中的位置序号
            T->num=1;  // num:子树的子节点数量 ID节点显然没有子节点
            break;
        case ARRAY: // 3. 处理变量列表的数组子节点 即：array[10]
            rtn=fillSymbolTable(T->type_id,newAlias(),LEV,T->type,'A',T->offset);  //顺序插入 数组名
            if(rtn==-1)
                semantic_error(T->pos,T->type_id,"变量重复定义");
            else T->place=rtn; //rtn：符号在符号表中的位置序号
            T->num=1; //num:子树的子节点数量 ARRAY节点没有子节点
            T0=T->ptr[1];
            while (T0)
            {//统计数组容量 维数之积  根据数组容量给数组分配内存空间
                printf("=======数组维数%d\n",T0->type_int);
                T->num*=T0->type_int;//叠加

                //数组的维
                symbolTable.symbols[rtn].dimension[indexPlace]=T0->type_int;//记录维数
                indexPlace++;

                T0=T0->ptr[1];
            }  
            //T->num=T->num-1;//自定义：数组名和索引为0的数组项对应一个地址
            symbolTable.symbols[rtn].dimensionNum=indexPlace;  
            //根据上面记录的维数将每个数组项都填入表中  仅处理一维数组和二维数组  
            strcpy(dess,T->type_id);            
            if(indexPlace==1){            // 一维数组
                for(int i=0;i< symbolTable.symbols[rtn].dimension[0];i++){
                    mmyStrcat(dess,i);
                    fillSymbolTable(dess,dess,LEV,T->type,'I',T->offset+(i+1)*(T->type==INT?4:(T->type==FLOAT?8:(T->type==CHAR?1:10))) - (T->type==INT?4:(T->type==FLOAT?8:(T->type==CHAR?1:10))));  //顺序插入 数组项
                    strcpy(dess,T->type_id);
                }
            }else if(indexPlace==2)       //   二维数组
            {
                for(int i=0;i< symbolTable.symbols[rtn].dimension[0];i++){
                    for (int j = 0; j < symbolTable.symbols[rtn].dimension[1]; j++)
                    {
                        mmyStrcat(dess,i);
                        mmyStrcat(dess,j);
                        if(i==0)
                        fillSymbolTable(dess,dess,LEV,T->type,'I',T->offset+(j+1)*(T->type==INT?4:(T->type==FLOAT?8:(T->type==CHAR?1:10))) -(T->type==INT?4:(T->type==FLOAT?8:(T->type==CHAR?1:10))));  //顺序插入 数组项
                        if(i==1)
                        fillSymbolTable(dess,dess,LEV,T->type,'I',T->offset+ (symbolTable.symbols[rtn].dimension[1])*(T->type==INT?4:(T->type==FLOAT?8:(T->type==CHAR?1:10))) +(j+1)*(T->type==INT?4:(T->type==FLOAT?8:(T->type==CHAR?1:10))) -(T->type==INT?4:(T->type==FLOAT?8:(T->type==CHAR?1:10))));  //顺序插入 数组项
                        strcpy(dess,T->type_id);
                    }
                }
            }
            
            // 二维数组          
            break;
        case ASSIGNOP: // 4. 处理常量（声明且赋值）
            rtn=fillSymbolTable(T->ptr[0]->type_id,newAlias(),LEV,T->ptr[0]->type,'C',T->ptr[0]->offset);  //顺序插入 别名的数字后缀递增
            if (rtn==-1)
                semantic_error(T->ptr[0]->pos,T->ptr[0]->type_id, "常量重复定义");
            else T->ptr[0]->place=rtn; // rtn:符号在符号表中的位置序号
            T->ptr[0]->num=1;  // num:子树的子节点数量 ID节点显然没有子节点
            break;
        }
    }


//语义检查： 匹配函数参数 
int  match_param(int i,struct ASTNode *T){
    int j,num=symbolTable.symbols[i].paramnum;
    int type1,type2,pos=T->pos;
    T=T->ptr[0];
    if (num==0 && T==NULL) return 1;
    for (j=1;j<=num;j++) {
        if (!T){
            semantic_error(pos,"", "函数调用参数太少!");
            return 0;
            }
        type1=symbolTable.symbols[i+j].type;  //形参类型
        type2=T->ptr[0]->type;
        if (type1!=type2){
            semantic_error(pos,"", "函数参数类型不匹配!");
            return 0;
        }
        T=T->ptr[1];
    }
    if (T){ //num个参数已经匹配完，还有实参表达式
        semantic_error(pos,"", "函数调用参数太多!");
        return 0;
        }
    return 1;
    }

//中间代码生成： 布尔表达式，参考文献[2]p84的思想
void boolExp(struct ASTNode *T){  
  struct opn opn1,opn2,result;
  int op;
  int rtn;
  if (T)
	{
	switch (T->kind) {
        case INT:  
                   break;
        case FLOAT: 
                   break;
        case ID:    
                   break;
        case RELOP: //树：Exp RELOP Exp
                    //处理关系运算表达式,2个操作数都按基本表达式处理
                    T->ptr[0]->offset=T->ptr[1]->offset=T->offset;
                    Exp(T->ptr[0]);
                    T->width=T->ptr[0]->width;
                    Exp(T->ptr[1]);
                    if (T->width<T->ptr[1]->width) T->width=T->ptr[1]->width;
                    opn1.kind=ID; strcpy(opn1.id,symbolTable.symbols[T->ptr[0]->place].alias);
                    opn1.offset=symbolTable.symbols[T->ptr[0]->place].offset;
                    opn2.kind=ID; strcpy(opn2.id,symbolTable.symbols[T->ptr[1]->place].alias);
                    opn2.offset=symbolTable.symbols[T->ptr[1]->place].offset;
                    result.kind=ID; strcpy(result.id,T->Etrue);
                    if (strcmp(T->type_id,"<")==0)
                            op=JLT;
                    else if (strcmp(T->type_id,"<=")==0)
                            op=JLE;
                    else if (strcmp(T->type_id,">")==0)
                            op=JGT;
                    else if (strcmp(T->type_id,">=")==0)
                            op=JGE;
                    else if (strcmp(T->type_id,"==")==0)
                            op=EQ;
                    else if (strcmp(T->type_id,"!=")==0)
                            op=NEQ;
                    T->code=genIR(op,opn1,opn2,result);
                    /*
                     * genIR函数的功能：
                     *      输出中间代码
                     * genIR函数的实参：
                     *      1. op
                     *      2. opn1
                     *      3. opn2
                     *      4. result
                     * genIR函数的返回值：
                     *      循环条件对应的代码 如：a<b
                     */
                    T->code=merge(4,T->ptr[0]->code,T->ptr[1]->code,T->code,genGoto(T->Efalse));
                    /*
                     * merge函数的功能：
                     *      合并多个中间代码的双向循环链表，首尾相连
                     * merge函数的实参：
                     *      1. T->ptr[0]->code：Exp1对应代码
                     *      2. T->ptr[1]->code：Exp2对应代码
                     *      3. T->code：genIR函数的返回值 如：a<b
                     *      4. genGoto(T->Efalse)：生成GOTO语句，返回头指针，找到循环条件为假的语句处
                    */
                    break;
        case AND: 
        case OR:
                    if (T->kind==AND) {
                        strcpy(T->ptr[0]->Etrue,newLabel());
                        strcpy(T->ptr[0]->Efalse,T->Efalse);
                        }
                    else {
                        strcpy(T->ptr[0]->Etrue,T->Etrue);
                        strcpy(T->ptr[0]->Efalse,newLabel());
                        }
                    strcpy(T->ptr[1]->Etrue,T->Etrue);
                    strcpy(T->ptr[1]->Efalse,T->Efalse);
                    T->ptr[0]->offset=T->ptr[1]->offset=T->offset;
                    boolExp(T->ptr[0]);
                    T->width=T->ptr[0]->width;
                    boolExp(T->ptr[1]);
                    if (T->width<T->ptr[1]->width) T->width=T->ptr[1]->width;
                    if (T->kind==AND)
                        T->code=merge(3,T->ptr[0]->code,genLabel(T->ptr[0]->Etrue),T->ptr[1]->code);
                    else
                        T->code=merge(3,T->ptr[0]->code,genLabel(T->ptr[0]->Efalse),T->ptr[1]->code);
                    break;
        case NOT:   strcpy(T->ptr[0]->Etrue,T->Efalse);
                    strcpy(T->ptr[0]->Efalse,T->Etrue);
                    boolExp(T->ptr[0]);
                    T->code=T->ptr[0]->code;
                    break;
        }
	}
}

//处理基本表达式，参考文献[2]p82的思想
void Exp(struct ASTNode *T)
{
  int rtn,num,width;
  struct ASTNode *T0;
  struct ASTNode *T1;
  struct opn opn1,opn2,result;
  int indexPlace=0;//作用：记录当前要操作的arrayIndex位置
  if (T)
	{
	switch (T->kind) {
	case ID:    //查符号表，获得符号表中的位置，类型送type
                rtn=searchSymbolTable(T->type_id);
                if (rtn==-1)
                    semantic_error(T->pos,T->type_id, "变量未定义");
                if (symbolTable.symbols[rtn].flag=='F')
                    semantic_error(T->pos,T->type_id, "这是函数而不是变量，类型不匹配");
                else {
                    T->place=rtn;       //结点保存变量在符号表中的位置
                    T->code=NULL;       //标识符不需要生成TAC
                    T->type=symbolTable.symbols[rtn].type;
                    T->offset=symbolTable.symbols[rtn].offset;
                    T->width=0;   //未再使用新单元
                    }
                break;
    case INT:   T->place=fill_Temp(newTemp(),LEV,T->type,'T',T->offset); //为整常量生成一个临时变量
                T->type=INT;
                opn1.kind=INT;opn1.const_int=T->type_int;
                result.kind=ID; strcpy(result.id,symbolTable.symbols[T->place].alias);
                result.offset=symbolTable.symbols[T->place].offset;
                T->code=genIR(ASSIGNOP,opn1,opn2,result);
                T->width=4;
                break;
    case FLOAT: T->place=fill_Temp(newTemp(),LEV,T->type,'T',T->offset);   //为浮点常量生成一个临时变量
                T->type=FLOAT;
                opn1.kind=FLOAT; opn1.const_float=T->type_float;
                result.kind=ID; strcpy(result.id,symbolTable.symbols[T->place].alias);
                result.offset=symbolTable.symbols[T->place].offset;
                T->code=genIR(ASSIGNOP,opn1,opn2,result);
                T->width=4;
                break;
    case ARRAY_USE:
                    //仿照ID                  
                    //先拼接ID和索引
                    T1=T->ptr[1];
                    strcpy(dess,T->type_id);
                    while (T1)
                    {                     
                        //数组的索引
                        mmyStrcat(dess,T1->type_int);
                        T1=T1->ptr[1];
                    }
                     //按ID+索引查找
                    rtn=searchSymbolTable(dess);
                    if (rtn==-1)
                        semantic_error(T->pos,T->type_id, "数组未定义");
                    if (symbolTable.symbols[rtn].flag=='F')
                        semantic_error(T->pos,T->type_id, "对函数使用下标访问");
                    else if (symbolTable.symbols[rtn].flag=='V')
                        semantic_error(T->pos,T->type_id, "对非数组变量使用下标访问");
                    else if (symbolTable.symbols[rtn].flag=='C'){
                        semantic_error(T->pos,T->type_id, "对常量使用下标访问");
                    }
                    else {  
                        //T->ptr[0]在这里判断下标类型
                        //ps：至于数组的使用，只实现一维数组的使用
                        T0=T->ptr[1];
                        while (T0)
                        {
                            if (T0->ptr[0]->type!=ID&&T0->ptr[0]->type!=INT)
                            {
                                semantic_error(T->pos,T->type_id, "数组变量的下标不是整型表达式");
                                break;
                            }         
                            T0=T0->ptr[1];
                        }                      
                            T->place=rtn;       //结点保存变量在符号表中的位置
                            T->code=NULL;       //标识符不需要生成TAC
                            T->type=symbolTable.symbols[rtn].type;
                            T->offset=symbolTable.symbols[rtn].offset;      
                            T->width=0;   //未再使用新单元
                        }
                break;
	case ASSIGNOP:
                if (T->ptr[0]->kind!=ID&&T->ptr[0]->kind!=ARRAY_USE){
                    semantic_error(T->pos,"", "赋值语句需要左值");
                    }
                else {
                    Exp(T->ptr[0]);   //处理左值，例中仅为变量
                    T->ptr[1]->offset=T->offset;
                    Exp(T->ptr[1]);
                    T->type=T->ptr[0]->type;
                    T->width=T->ptr[1]->width;
                    T->code=merge(2,T->ptr[0]->code,T->ptr[1]->code);
                    opn1.kind=ID;   strcpy(opn1.id,symbolTable.symbols[T->ptr[1]->place].alias);//右值一定是个变量或临时变量
                    opn1.offset=symbolTable.symbols[T->ptr[1]->place].offset;
                    result.kind=ID; strcpy(result.id,symbolTable.symbols[T->ptr[0]->place].alias);
                    result.offset=symbolTable.symbols[T->ptr[0]->place].offset;
                    T->code=merge(2,T->code,genIR(ASSIGNOP,opn1,opn2,result));
                    }
                break;
	case AND:   //按算术表达式方式计算布尔值，to be continue
	case OR:    //按算术表达式方式计算布尔值，
	case RELOP: //按算术表达式方式计算布尔值，
                T->type=INT;
                T->ptr[0]->offset=T->ptr[1]->offset=T->offset;
                Exp(T->ptr[0]);
                Exp(T->ptr[1]);
                break;
	case PLUS:
	case MINUS:
	case STAR:
	case DIV:   T->ptr[0]->offset=T->offset;
                Exp(T->ptr[0]);
                T->ptr[1]->offset=T->offset+T->ptr[0]->width;
                Exp(T->ptr[1]);
                //判断T->ptr[0]，T->ptr[1]类型是否正确，可能根据运算符生成不同形式的代码，给T的type赋值
                //下面的类型属性计算，没有考虑错误处理情况
                if (T->ptr[0]->type==FLOAT || T->ptr[1]->type==FLOAT)
                     T->type=FLOAT,T->width=T->ptr[0]->width+T->ptr[1]->width+4;
                else T->type=INT,T->width=T->ptr[0]->width+T->ptr[1]->width+2;
                T->place=fill_Temp(newTemp(),LEV,T->type,'T',T->offset+T->ptr[0]->width+T->ptr[1]->width);
                opn1.kind=ID; strcpy(opn1.id,symbolTable.symbols[T->ptr[0]->place].alias);
                opn1.type=T->ptr[0]->type;opn1.offset=symbolTable.symbols[T->ptr[0]->place].offset;
                opn2.kind=ID; strcpy(opn2.id,symbolTable.symbols[T->ptr[1]->place].alias);
                opn2.type=T->ptr[1]->type;opn2.offset=symbolTable.symbols[T->ptr[1]->place].offset;
                result.kind=ID; strcpy(result.id,symbolTable.symbols[T->place].alias);
                result.type=T->type;result.offset=symbolTable.symbols[T->place].offset;
                T->code=merge(3,T->ptr[0]->code,T->ptr[1]->code,genIR(T->kind,opn1,opn2,result));
                T->width=T->ptr[0]->width+T->ptr[1]->width+(T->type==INT?4:(T->type==FLOAT?8:(T->type==CHAR?1:10)));
                break;
	case NOT:   //to be continue
                break;
	case UMINUS://to be continue
                break;
    case SELFPLUS:
    case SELFMINUS:
                if(T->ptr[0]->type==INT||T->ptr[0]->type==FLOAT||T->ptr[0]->type==CHAR||T->ptr[0]->type==STRING){
                     printf("在%d行,%s\n",T->pos, "对常量进行自增自减");
                }else
                {//int类型的ID
                 //自增、自减的临时变量1应该是预先存在符号表中的，这一点不同于二元运算
                    // 1. 首先是计算处理 +1 or -1
                    T->ptr[0]->offset=T->offset;
                    Exp(T->ptr[0]);
                    T->type=INT,T->width=T->ptr[0]->width+2;//暂时参考二元运算的处理，+2具体的含义是？
                    T->place=fill_Temp(newTemp(),LEV,T->type,'T',T->offset+T->ptr[0]->width);
                    opn1.kind=ID; strcpy(opn1.id,symbolTable.symbols[T->ptr[0]->place].alias);
                    opn1.type=T->ptr[0]->type;opn1.offset=symbolTable.symbols[T->ptr[0]->place].offset;
                    opn2.kind=ID; strcpy(opn2.id,symbolTable.symbols[2].alias);//ps：2是预设的int 1 的位置序号
                    opn2.type=INT;opn2.offset=symbolTable.symbols[2].offset;
                    //opn2.const_int=1;//ps：用于自增、自减
                    result.kind=ID; strcpy(result.id,symbolTable.symbols[T->place].alias);
                    result.type=T->type;result.offset=symbolTable.symbols[T->place].offset;
                    T->code=merge(2,T->ptr[0]->code,genIR(T->kind,opn1,opn2,result));
                    // 2.  下面为赋值处理
                    opn1.kind=ID;   strcpy(opn1.id,symbolTable.symbols[T->place].alias);//右值一定是个变量或临时变量
                    opn1.offset=symbolTable.symbols[T->place].offset;
                    result.kind=ID; strcpy(result.id,symbolTable.symbols[T->ptr[0]->place].alias);
                    result.offset=symbolTable.symbols[T->ptr[0]->place].offset;
                    T->code=merge(2,T->code,genIR(ASSIGNOP,opn1,opn2,result));
                  
                    T->width=T->ptr[0]->width+(T->type==INT?4:(T->type==FLOAT?8:(T->type==CHAR?1:10)));
                    break;
                }
                
                break;
    case FUNC_CALL: //根据T->type_id查出函数的定义，如果语言中增加了实验教材的read，write需要单独处理一下
                rtn=searchSymbolTable(T->type_id);
                if (rtn==-1){
                    semantic_error(T->pos,T->type_id, "函数未定义");
                    break;
                    }
                if (symbolTable.symbols[rtn].flag!='F'){
                    semantic_error(T->pos,T->type_id, "这是变量而不是函数，类型不匹配");
                    break;
                    }
                T->type=symbolTable.symbols[rtn].type;
                width=T->type==INT?4:(T->type==FLOAT?8:(T->type==CHAR?1:10));   //存放函数返回值的单数字节数
                if (T->ptr[0]){
                    T->ptr[0]->offset=T->offset;
                    Exp(T->ptr[0]);       //处理所有实参表达式求值，及类型
                    T->width=T->ptr[0]->width+width; //累加上计算实参使用临时变量的单元数
                    T->code=T->ptr[0]->code;
                    }
                else {T->width=width; T->code=NULL;}
                match_param(rtn,T);   //处理所有参数的匹配
                    //处理参数列表的中间代码
                T0=T->ptr[0];
                while (T0) {
                    result.kind=ID;  strcpy(result.id,symbolTable.symbols[T0->ptr[0]->place].alias);
                    result.offset=symbolTable.symbols[T0->ptr[0]->place].offset;
                    T->code=merge(2,T->code,genIR(ARG,opn1,opn2,result));
                    T0=T0->ptr[1];
                    }
                T->place=fill_Temp(newTemp(),LEV,T->type,'T',T->offset+T->width-width);
                opn1.kind=ID;     strcpy(opn1.id,T->type_id);  //保存函数名
                opn1.offset=rtn;  //这里offset用以保存函数定义入口,在目标代码生成时，能获取相应信息
                result.kind=ID;   strcpy(result.id,symbolTable.symbols[T->place].alias);
                result.offset=symbolTable.symbols[T->place].offset;
                T->code=merge(2,T->code,genIR(CALL,opn1,opn2,result)); //生成函数调用中间代码
                break;
    case ARGS:      //此处仅处理各实参表达式的求值的代码序列，不生成ARG的实参系列
                T->ptr[0]->offset=T->offset;
                Exp(T->ptr[0]);
                T->width=T->ptr[0]->width;
                T->code=T->ptr[0]->code;
                if (T->ptr[1]) {
                    T->ptr[1]->offset=T->offset+T->ptr[0]->width;
                    Exp(T->ptr[1]);
                    T->width+=T->ptr[1]->width;
                    T->code=merge(2,T->code,T->ptr[1]->code);
                    }
                break;
         }
      }
}


//核心函数：对抽象语法树的先根遍历,按display的控制结构修改完成符号表管理和语义检查和TAC生成（语句部分）
//step1: 完成符号表管理  
//step2: 完成语义检查
//step3: TAC生成
/*
 *  EXT_DEF_LIST:外部定义列表（外部变量和函数） 二叉树结构
 *  EXT_VAR_DEF: 外部定义—— 变量（列表） 如：int a , b;
 *  FUNC_DEF: 外部定义—— 函数定义       如：int fun（int a,int b）{}
 *  FUNC_DEC：函数声明                  如：fun（int a,int b）
 *  PARAM_LIST：函数形参列表            如： int a，int b
 *  PARAM_DEC：函数形参                 如：int a
 *  COMP_STM:  复合语句                 如：{DefList StmList}
 *  DEF_LIST： 局部变量（名）定义列表    如：a,b
 *  VAR_DEF:  局部变量定义              如：int a, b;
 *  STMT_LIST:  语句（说明）列表        如： 语句1; 语句2; 复合语句3; 
 *  EXP_STMT:  表达式语句               如：Exp SEMI
*/
void semantic_Analysis(struct ASTNode *T)
{
  int rtn,num,width;
  struct ASTNode *T0;
  struct opn opn1,opn2,result;
  if (T)
	{
	switch (T->kind) {
	case EXT_DEF_LIST:
            if (!T->ptr[0]) break;   //1. 二叉树的左节点 空判断
            T->ptr[0]->offset=T->offset;  //继承：偏移
            semantic_Analysis(T->ptr[0]);   //递归：访问外部定义列表中的第一个
            T->code=T->ptr[0]->code;  // 中间代码：code is 中间代码链表头指针
            if (T->ptr[1]){   //2. 二叉树的右节点
                T->ptr[1]->offset=T->ptr[0]->offset+T->ptr[0]->width;  //继承：偏移
                semantic_Analysis(T->ptr[1]); //访问该外部定义列表中的其它外部定义
                T->code=merge(2,T->code,T->ptr[1]->code); //中间代码：
                }
            break;
	case EXT_VAR_DEF:   
            T->type=T->ptr[1]->type=!strcmp(T->ptr[0]->type_id,"int")?INT:(!strcmp(T->ptr[0]->type_id,"float")?FLOAT:(!strcmp(T->ptr[0]->type_id,"char")?CHAR:STRING));//（继承）将第一个孩子(TYPE结点)中的类型送到第二个孩子（ID）的类型域
            T->ptr[1]->offset=T->offset;        //（继承）这个外部变量的偏移量向下传递
            T->ptr[1]->width=T->type==INT?4:(T->type==FLOAT?8:(T->type==CHAR?1:10));  //（继承）将一个变量的宽度向下传递 int 4 | float 8 | char 1 | string 预设10
            ext_var_list(T->ptr[1]);            //处理外部变量说明中的标识符序列
            T->width=(T->type==INT?4:(T->type==FLOAT?8:(T->type==CHAR?1:10)))* T->ptr[1]->num; //计算这个外部变量说明的宽度 （类型宽度*数量）
            T->code=NULL;             //这里假定外部变量不支持初始化
            break;
    case EXT_VARCONST_DEF:
            T->type=T->ptr[1]->ptr[0]->type=!strcmp(T->ptr[0]->type_id,"int")?INT:(!strcmp(T->ptr[0]->type_id,"float")?FLOAT:(!strcmp(T->ptr[0]->type_id,"char")?CHAR:STRING));//（继承）将第一个孩子(TYPE结点)中的类型送到第二个孩子（ID）的类型域
            T->ptr[1]->ptr[0]->offset=T->offset;        //（继承）这个外部变量的偏移量向下传递
            T->ptr[1]->ptr[0]->width=T->type==INT?4:(T->type==FLOAT?8:(T->type==CHAR?1:10));  //（继承）将一个变量的宽度向下传递 int 4 | float 8 | char 1 | string 预设10
            ext_var_list(T->ptr[1]);            //处理外部变量说明中的标识符序列
            T->width=(T->type==INT?4:(T->type==FLOAT?8:(T->type==CHAR?1:10)))* T->ptr[1]->ptr[0]->num; //计算这个外部变量说明的宽度 （类型宽度*数量）
            T->code=NULL;             //这里假定外部变量不支持初始化？？？？？？？
            break;
	case FUNC_DEF:      //填写函数定义信息到符号表
            T->ptr[1]->type=!strcmp(T->ptr[0]->type_id,"int")?INT:(!strcmp(T->ptr[0]->type_id,"float")?FLOAT:(!strcmp(T->ptr[0]->type_id,"char")?CHAR:STRING));//1. 获取函数返回类型(T->ptr[0])送到含函数名、参数的结点
            T->width=0;     //函数的宽度设置为0，不会对外部变量的地址分配产生影响
            T->offset=DX;   //设置局部变量在活动记录中的偏移量初值  每个函数对应的偏移就是DX的值，因为函数的offset=0 （参考line 550）
            semantic_Analysis(T->ptr[1]); //2. 处理函数名和参数结点部分(T->ptr[1])，这里不考虑用寄存器传递参数  ------> case FUNC_DEC:
            T->offset+=T->ptr[1]->width;   //用形参单元宽度修改函数局部变量的起始偏移量(semantic_Analysis函数处理了宽度)
            T->ptr[2]->offset=T->offset;
            strcpy(T->ptr[2]->Snext,newLabel());  //函数体语句执行结束后的位置属性  Snext:下一条语句位置标号 通过newLabel()先产生一个暂时不确定的下一条语句对应的标记
            global_returnType=T->ptr[1]->type; //自定义，为了判断返回值的类型是否匹配 
            semantic_Analysis(T->ptr[2]);         //3. 处理函数体结点（T->ptr[2]） ---> COMP_STM:
            //计算活动记录大小,这里offset属性存放的是活动记录大小，不是偏移
            symbolTable.symbols[T->ptr[1]->place].offset=T->offset+T->ptr[2]->width;
            T->code=merge(3,T->ptr[1]->code,T->ptr[2]->code,genLabel(T->ptr[2]->Snext)); //函数体的代码作为函数的代码
            break;
	case FUNC_DEC:      //根据返回类型，函数名填写符号表
            rtn=fillSymbolTable(T->type_id,newAlias(),LEV,T->type,'F',0);//函数不在数据区中分配单元，偏移量为0
            if (rtn==-1){
                semantic_error(T->pos,T->type_id, "函数重复定义");
                break;
                }
            else T->place=rtn;
            result.kind=ID;   
            strcpy(result.id,T->type_id);
            result.offset=rtn;
            T->code=genIR(FUNCTION,opn1,opn2,result);     //生成中间代码：FUNCTION 函数名
            T->offset=DX;   //设置形式参数在活动记录中的偏移量初值
            if (T->ptr[0]) { //判断是否有参数
                T->ptr[0]->offset=T->offset;
                semantic_Analysis(T->ptr[0]);  //处理函数参数列表 ------> case PARAM_LIST：
                T->width=T->ptr[0]->width;
                symbolTable.symbols[rtn].paramnum=T->ptr[0]->num;
                T->code=merge(2,T->code,T->ptr[0]->code);  //连接函数名和参数代码序列
                }
            else symbolTable.symbols[rtn].paramnum=0,T->width=0;//没有参数
            break;
	case PARAM_LIST:    //处理函数形式参数列表   PARAM_LIST：ParamDec（0） COMMA  VarList（1）
            T->ptr[0]->offset=T->offset;
            semantic_Analysis(T->ptr[0]); //含对ptr[0]->width的处理
            if (T->ptr[1]){ //VarList不为空，形参不止一个
                T->ptr[1]->offset=T->offset+T->ptr[0]->width;
                semantic_Analysis(T->ptr[1]);
                T->num=T->ptr[0]->num+T->ptr[1]->num;        //统计参数个数
                T->width=T->ptr[0]->width+T->ptr[1]->width;  //累加参数单元宽度
                T->code=merge(2,T->ptr[0]->code,T->ptr[1]->code);  //连接参数代码
                }
            else {
                T->num=T->ptr[0]->num;
                T->width=T->ptr[0]->width;
                T->code=T->ptr[0]->code;
                }
            break;
	case  PARAM_DEC:    //处理某个函数形式参数
            rtn=fillSymbolTable(T->ptr[1]->type_id,newAlias(),1,T->ptr[0]->type,'P',T->offset);
            if (rtn==-1)
                semantic_error(T->ptr[1]->pos,T->ptr[1]->type_id, "参数名重复定义");
            else T->ptr[1]->place=rtn;
            T->num=1;       //参数个数计算的初始值
            T->width=T->ptr[0]->type==INT?4:(T->ptr[0]->type==FLOAT?8:(T->ptr[0]->type==CHAR?1:10));  //参数宽度
            result.kind=ID;   
            strcpy(result.id, symbolTable.symbols[rtn].alias);
            result.offset=T->offset;
            T->code=genIR(PARAM,opn1,opn2,result);     //生成：FUNCTION 函数名
            break;
	case COMP_STM:
            LEV++;//设置层号加1         
            symbol_scope_TX.TX[symbol_scope_TX.top++]=symbolTable.index;//保存该层局部变量在符号表中的起始位置在symbol_scope_TX 
            T->width=0;
            T->code=NULL;
            if (T->ptr[0]) {
                T->ptr[0]->offset=T->offset;
                semantic_Analysis(T->ptr[0]);  // 1. 处理该层的局部变量: ---> DEF_LIST:
                T->width+=T->ptr[0]->width;  //叠加宽度
                T->code=T->ptr[0]->code;
                }
            if (T->ptr[1]){
                T->ptr[1]->offset=T->offset+T->width;
                strcpy(T->ptr[1]->Snext,T->Snext);  //S.next属性向下传递
                semantic_Analysis(T->ptr[1]);       // 2. 处理复合语句的语句序列:   ---> STM_LIST:
                T->width+=T->ptr[1]->width;  //叠加宽度
                T->code=merge(2,T->code,T->ptr[1]->code);
                }
             prn_symbol();       //打印符号表：c在退出一个符合语句前显示的符号表
             LEV--;    //出复合语句，层号减1
             symbolTable.index=symbol_scope_TX.TX[--symbol_scope_TX.top]; //删除该作用域中的符号
             break;
    case DEF_LIST:
            T->code=NULL;
            if (T->ptr[0]){
                T->ptr[0]->offset=T->offset;
                semantic_Analysis(T->ptr[0]);   //处理一个局部变量定义
                T->code=T->ptr[0]->code;
                T->width=T->ptr[0]->width;
                }
            if (T->ptr[1]) {//列表中不止一个变量
                T->ptr[1]->offset=T->offset+T->ptr[0]->width;
                semantic_Analysis(T->ptr[1]);   //处理列表中剩下的局部变量定义
                T->code=merge(2,T->code,T->ptr[1]->code);
                T->width+=T->ptr[1]->width;
                }
                break;
    case VAR_DEF://处理一个局部变量定义,将第一个孩子(TYPE结点)中的类型送到第二个孩子的类型域
                 //类似于上面的外部变量EXT_VAR_DEF，换了一种处理方法
                T->code=NULL;
                T->ptr[1]->type=!strcmp(T->ptr[0]->type_id,"int")?INT:(!strcmp(T->ptr[0]->type_id,"float")?FLOAT:(!strcmp(T->ptr[0]->type_id,"char")?CHAR:STRING));  //确定变量序列各变量类型
                T0=T->ptr[1]; //T0为变量名列表子树根指针，对ID、ASSIGNOP类结点在登记到符号表，作为局部变量
                num=0;
                T0->offset=T->offset;
                T->width=0;
                width=T->ptr[1]->type==INT?4:(T->ptr[1]->type==FLOAT?8:(T->ptr[1]->type==CHAR?1:10));  //一个变量宽度
                while (T0) {  //处理所以DEC_LIST列表中的结点
                    num++;
                    T0->ptr[0]->type=T0->type;  //类型属性向下传递
                    if (T0->ptr[1]) 
                            T0->ptr[1]->type=T0->type;
                    T0->ptr[0]->offset=T0->offset;  //类型属性向下传递
                    if (T0->ptr[1]) 
                            T0->ptr[1]->offset=T0->offset+width;
                    if (T0->ptr[0]->kind==ID){// 1. ====== 对ID
                        rtn=fillSymbolTable(T0->ptr[0]->type_id,newAlias(),LEV,T0->ptr[0]->type,'V',T->offset+T->width);//此处偏移量未计算，暂时为0
                        if (rtn==-1)
                            semantic_error(T0->ptr[0]->pos,T0->ptr[0]->type_id, "变量重复定义");
                        else T0->ptr[0]->place=rtn;
                        T->width+=width;
                        }
                    else if (T0->ptr[0]->kind==ASSIGNOP){// 2. ======= 对ASSIGNOP
                            rtn=fillSymbolTable(T0->ptr[0]->ptr[0]->type_id,newAlias(),LEV,T0->ptr[0]->type,'V',T->offset+T->width);//此处偏移量未计算，暂时为0
                            if (rtn==-1)
                                semantic_error(T0->ptr[0]->ptr[0]->pos,T0->ptr[0]->ptr[0]->type_id, "变量重复定义");
                            else {
                                T0->ptr[0]->place=rtn;
                                T0->ptr[0]->ptr[1]->offset=T->offset+T->width+width;
                                Exp(T0->ptr[0]->ptr[1]);
                                opn1.kind=ID; 
                                strcpy(opn1.id,symbolTable.symbols[T0->ptr[0]->ptr[1]->place].alias);
                                result.kind=ID; 
                                strcpy(result.id,symbolTable.symbols[T0->ptr[0]->place].alias);
                                T->code=merge(3,T->code,T0->ptr[0]->ptr[1]->code,genIR(ASSIGNOP,opn1,opn2,result));
                                }
                            T->width+=width+T0->ptr[0]->ptr[1]->width;
                            }
                    T0=T0->ptr[1];//迭代到下一个结点
                    }
                break;
	case STM_LIST:
                if (!T->ptr[0]) { T->code=NULL; T->width=0; break;}   //空语句序列 --> 直接退出
                if (T->ptr[1]) //STM_LIST里有2条以上语句连接，生成新标号作为第一条语句结束后到达的位置
                    strcpy(T->ptr[0]->Snext,newLabel());
                else     //语句序列仅有一条语句，S.next属性向下传递
                    strcpy(T->ptr[0]->Snext,T->Snext);
                T->ptr[0]->offset=T->offset;
                semantic_Analysis(T->ptr[0]);
                T->code=T->ptr[0]->code;
                T->width=T->ptr[0]->width;
                if (T->ptr[1]){     //STM_LIST里有2条以上语句连接,S.next属性向下传递
                    strcpy(T->ptr[1]->Snext,T->Snext);
                    T->ptr[1]->offset=T->offset;  //顺序结构共享单元方式
//                  T->ptr[1]->offset=T->offset+T->ptr[0]->width; //顺序结构顺序分配单元方式

                    semantic_Analysis(T->ptr[1]);
                    //序列中第1条为表达式语句，返回语句，复合语句时，第2条前不需要标号
                    if (T->ptr[0]->kind==RETURN ||T->ptr[0]->kind==EXP_STMT ||T->ptr[0]->kind==COMP_STM)
                        T->code=merge(2,T->code,T->ptr[1]->code);
                    else
                        T->code=merge(3,T->code,genLabel(T->ptr[0]->Snext),T->ptr[1]->code);
                    if (T->ptr[1]->width>T->width) T->width=T->ptr[1]->width; //顺序结构共享单元方式
//                        T->width+=T->ptr[1]->width;//顺序结构顺序分配单元方式
                    }
                break;
	case IF_THEN:
                strcpy(T->ptr[0]->Etrue,newLabel());  //设置条件语句真假转移位置
                strcpy(T->ptr[0]->Efalse,T->Snext);
                T->ptr[0]->offset=T->ptr[1]->offset=T->offset;
                boolExp(T->ptr[0]);
                T->width=T->ptr[0]->width;
                strcpy(T->ptr[1]->Snext,T->Snext);
                semantic_Analysis(T->ptr[1]);      //if子句
                if (T->width<T->ptr[1]->width) T->width=T->ptr[1]->width;
                T->code=merge(3,T->ptr[0]->code, genLabel(T->ptr[0]->Etrue),T->ptr[1]->code);
                break;  //控制语句都还没有处理offset和width属性
	case IF_THEN_ELSE:
                strcpy(T->ptr[0]->Etrue,newLabel());  //设置条件语句真假转移位置
                strcpy(T->ptr[0]->Efalse,newLabel());
                T->ptr[0]->offset=T->ptr[1]->offset=T->ptr[2]->offset=T->offset;
                boolExp(T->ptr[0]);      //条件，要单独按短路代码处理
                T->width=T->ptr[0]->width;
                strcpy(T->ptr[1]->Snext,T->Snext);
                semantic_Analysis(T->ptr[1]);      //if子句
                if (T->width<T->ptr[1]->width) T->width=T->ptr[1]->width;
                strcpy(T->ptr[2]->Snext,T->Snext);
                semantic_Analysis(T->ptr[2]);      //else子句
                if (T->width<T->ptr[2]->width) T->width=T->ptr[2]->width;
                T->code=merge(6,T->ptr[0]->code,genLabel(T->ptr[0]->Etrue),T->ptr[1]->code,\
                              genGoto(T->Snext),genLabel(T->ptr[0]->Efalse),T->ptr[2]->code);
                break;
	case WHILE: global_inCicle=1;//在循环中
                strcpy(T->ptr[0]->Etrue,newLabel());  //子结点继承属性的计算
                strcpy(T->ptr[0]->Efalse,T->Snext);
                T->ptr[0]->offset=T->ptr[1]->offset=T->offset;
                boolExp(T->ptr[0]);      //循环条件，要单独按短路代码处理
                T->width=T->ptr[0]->width;
                strcpy(T->ptr[1]->Snext,newLabel());//用于正常进入下一次循环，或者continue
                strcpy(global_Efalse,T->Snext); //全局自定义-1：用于break
                strcpy(global_Snext,T->ptr[1]->Snext); //全局自定义-2：用于continue
                semantic_Analysis(T->ptr[1]);      //循环体 ---> 转到EXP_STMT SKIP_STMT
                if (T->width<T->ptr[1]->width) T->width=T->ptr[1]->width;
                T->code=merge(5,genLabel(T->ptr[1]->Snext),T->ptr[0]->code, \
                genLabel(T->ptr[0]->Etrue),T->ptr[1]->code,genGoto(T->ptr[1]->Snext));
                /*
                 * merge函数功能：
                 * 合并多个中间代码的双向循环链表，首尾相连
                 * merge函数的实参说明：
                 * 1. genLabel(T->ptr[1]->Snext)：标号语句的头指针，对应本次循环结束后的下一条语句的代码（即：判断循环条件的语句）
                 * 2. T->ptr[0]->code：对应循环条件的代码
                 * 3. genLabel(T->ptr[0]->Etrue)：标号语句的头指针，对应循环条件为真时的要转移的代码
                 * 4. T->ptr[1]->code：对应循环体的代码
                 * 5. genGoto(T->ptr[1]->Snext)：GOTO语句的头指针，去往循环结束的下一条语句（1所构造的label处）
                 * ps：会发现merge并没有涉及循环条件为假时的代码，这是因为循环条件为假时的处理在boolExp()里面完成
                */
                global_inCicle=0;//离开循环
                break;
    case EXP_STMT:
                T->ptr[0]->offset=T->offset;
                semantic_Analysis(T->ptr[0]);
                T->code=T->ptr[0]->code;
                T->width=T->ptr[0]->width;
                break;
	case RETURN:if (T->ptr[0]){
                    T->ptr[0]->offset=T->offset;
                    Exp(T->ptr[0]);//返回值是一个Exp
                    //printf("=======函数返回值%d\n",global_returnType);
                    //printf("=======当前返回值%d\n",T->ptr[0]->type);
                    if(T->ptr[0]->type!=global_returnType){
                        semantic_error(T->pos,T->ptr[0]->type_id, "返回值与函数返回值的类型不匹配");
                    }
                    T->width=T->ptr[0]->width;
                    result.kind=ID; strcpy(result.id,symbolTable.symbols[T->ptr[0]->place].alias);
                    result.offset=symbolTable.symbols[T->ptr[0]->place].offset;
                    T->code=merge(2,T->ptr[0]->code,genIR(RETURN,opn1,opn2,result));
                    }
                else{
                    T->width=0;
                    result.kind=0;
                    T->code=genIR(RETURN,opn1,opn2,result);
                    }
                break;
    case SKIP_SEMI: 
                //strcpy(T->ptr[0]->Snext,T->Snext);//自定义：用于continue
                //strcpy(T->ptr[0]->Efalse,T->Efalse);//自定义:用于break
                semantic_Analysis(T->ptr[0]);
                T->code=T->ptr[0]->code;//自定义：赋值中间代码
                break;
    case BREAK:
                if (global_inCicle!=1)
                {
                    printf("在%d行,%s\n",T->pos, "在循环外非法使用break");
                }else
                {
                    T->width=0;
                    result.kind=0;
                    //printf("break=================%s\n",global_Efalse);
                    strcpy(result.id,global_Efalse);
                    //printf("break=================%s\n",result.id);
                    T->code=genIR(BREAK,opn1,opn2,result);
                }               
                break;
    case CONTINUE:
                if (global_inCicle!=1)
                {
                    printf("在%d行,%s\n",T->pos, "在循环外非法使用continue");
                }else
                {
                    T->width=0;
                    result.kind=0;
                    //printf("continue=================%s\n",global_Snext);
                    strcpy(result.id,global_Snext);
                    //printf("continue=================%s\n",result.id);
                    T->code=genIR(CONTINUE,opn1,opn2,result);
                }   
                break;
	case ID:
    case INT:
    case FLOAT:
	case ASSIGNOP:
	case AND:
	case OR:
	case RELOP:
	case PLUS:
	case MINUS:
    case SELFPLUS:
    case SELFMINUS:
	case STAR:
	case DIV:
	case NOT:
	case UMINUS:
    case FUNC_CALL:
                    Exp(T);          //处理基本表达式
                    break;
    }
    }
}


void semantic_Analysis0(struct ASTNode *T) {
    symbolTable.index=0;
    fillSymbolTable("read","",0,INT,'F',4);
    symbolTable.symbols[0].paramnum=0;//read的形参个数
    fillSymbolTable("write","",0,INT,'F',4);
    symbolTable.symbols[1].paramnum=1;
    fillSymbolTable("one","IntOne",0,INT,'C',0);
    symbol_scope_TX.TX[0]=1;  //外部变量在符号表中的起始序号为 0--> 1 因为预设了常量int 1用于自增、自减
    symbol_scope_TX.top=1;
    T->offset=4;              //外部变量在数据区的偏移量 0 --> 4 因为预设了常量int 1用于自增、自减
    semantic_Analysis(T);
    prnIR(T->code);
    objectCode(T->code);
 } 