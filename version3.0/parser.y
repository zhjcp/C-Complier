%error-verbose
%locations
%{
#include "stdio.h"
#include "math.h"
#include "string.h"
#include "def.h"
extern int yylineno;
extern char *yytext;
extern FILE *yyin;
int yylex();
void yyerror(const char* fmt, ...);
char *keepIntIndex(char *des,int src);
void myStrcat(char *des,const char *src);
void splitQuote(char *str);
void display(struct ASTNode *,int);
%}

%union {
	int    type_int;
	float  type_float;
        char   type_char; //char
        char   type_string[100]; //string
	char   type_id[32];
	struct ASTNode *ptr;
};

//  %type 定义非终结符的语义值类型
%type  <ptr> program ExtDefList ExtDef  Specifier ConstSpecifier ExtDecList ARRAYLIST  FuncDec CompSt VarList VarDec  ParamDec Stmt StmList SKIP DefList Def DecList Dec Exp Args /*删除，防止编译错误！！：CaseStmtList0 CaseStmtList*/

//% token 定义终结符的语义值类型 在这里定义过的类型才能在lex.l中使用(如：return TYPE)
%token <type_int> INT              /*指定INT的语义值是type_int，有词法分析得到的数值*/
%token <type_id> ID  RELOP TYPE  CONST_TYPE  /*指定ID,RELOP 的语义值是type_id，有词法分析得到的标识符字符串*/
%token <type_float>  FLOAT         /*修正后注释：指定FLOAT的语义值是type_float，有词法分析得到的数值*/ /*原注释：指定ID的语义值是type_id，有词法分析得到的标识符字符串*/
%token <type_char>   CHAR          /*指定CHAR的语义值是type_char，有词法分析得到的字符*/
%token <type_string> STRING        /*指定STRING的语义值是type_string,有词法分析得到字符串值 */
%token <type_id> BREAK CONTINUE /* break continue */

%token DPLUS LP RP LB RB LC RC SEMI COMMA  COLON  CONST  /*用bison对该文件编译时，带参数-d，生成的.tab.h中给这些单词进行编码，可在lex.l中包含parser.tab.h使用这些单词种类码*/
%token PLUS SELFPLUS MINUS SELFMINUS STAR DIV ASSIGNOP PLUS_ASSIGNOP MINUS_ASSIGNOP STAR_ASSIGNOP DIV_ASSIGNOP AND OR NOT IF ELSE WHILE RETURN FOR SWITCH CASE  DEFAULT ARRAY ARRAY_USE
/*以下为接在上述token后依次编码的枚举常量，作为AST结点类型标记*/
%token EXT_DEF_LIST EXT_VAR_DEF EXT_VARCONST_DEF FUNC_DEF FUNC_DEC EXT_DEC_LIST ARRAY_LIST ARRAY_LIST_CALL PARAM_LIST PARAM_DEC VAR_DEF DEC_LIST DEF_LIST COMP_STM STM_LIST EXP_STMT SKIP_SEMI IF_THEN IF_THEN_ELSE CASE_THEN DEFAULT_THEN
%token FUNC_CALL  ARGS FUNCTION PARAM ARG CALL LABEL GOTO JLT JLE JGT JGE EQ NEQ


%left ASSIGNOP PLUS_ASSIGNOP MINUS_ASSIGNOP STAR_ASSIGNOP DIV_ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS SELFPLUS SELFMINUS
%left STAR DIV
%right UMINUS NOT DPLUS
%left LB RB

%nonassoc LOWER_THEN_ELSE
%nonassoc ELSE

%%

/*整个程序*/
program: ExtDefList    { /*display($1,0);*/ /*显示语法树*/ semantic_Analysis0($1);/*语义分析*/}//显示语法树,语义分析   //semantic_Analysis0($1);暂时没有定义，先删掉，防止编译报错！！！    
         ; 


/* 外部定义列表（函数和变量） */
ExtDefList: {$$=NULL;}
          | ExtDef ExtDefList {$$=mknode(2,EXT_DEF_LIST,yylineno,$1,$2);}   //每一个EXTDEFLIST的结点，其第1棵子树对应一个外部变量声明或函数
          ;  


/* 外部定义（变量） */
ExtDef:   Specifier ExtDecList SEMI   {$$=mknode(2,EXT_VAR_DEF,yylineno,$1,$2);}   //该结点对应一个外部变量声明
         |Specifier FuncDec CompSt    {$$=mknode(3,FUNC_DEF,yylineno,$1,$2,$3);}   //该结点对应一个函数定义
         |ConstSpecifier  Dec  SEMI {$$=mknode(2,EXT_VARCONST_DEF,yylineno,$1,$2);}   //该结点对应一个常量定义 后面可以通过定义一个Exp的继承属性，告诉这个Exp赋值表达式它是常量
         | error SEMI   {$$=NULL;}
         ;


/* 类型  */
Specifier:  TYPE    {$$=mknode(0,TYPE,yylineno);strcpy($$->type_id,$1);$$->type=!strcmp($1,"int")?INT:(!strcmp($1,"float")?FLOAT:(!strcmp($1,"char")?CHAR:STRING));}   
           ;        // PS：构造AST的结点、TYPE是AST的类型类型码（由yacc自动生成），$$是左边非终结符的语义值,依次赋予 语义值：$$->type_id、语义值类型：$$->type
                    // PS：Specifier在其他产生式中归约时会用到，这里先把语义值存进去就完事


/* 常量类型 */
ConstSpecifier: CONST TYPE  {$$=mknode(0,CONST_TYPE,yylineno); strcpy($$->type_id,$2); $$->type=!strcmp($2,"int")?INT:(!strcmp($2,"float")?FLOAT:(!strcmp($2,"char")?CHAR:STRING));} 
                ;

/* 变量名列表 */
ExtDecList:  VarDec      {$$=$1;}       /*每一个EXT_DECLIST的结点，其第一棵子树对应一个变量名(ID类型的结点),第二棵子树对应剩下的外部变量名*/
           | VarDec COMMA ExtDecList {$$=mknode(2,EXT_DEC_LIST,yylineno,$1,$3);} /* 形如：a,b,c */
           ;  


/* 变量（标识符）声明，包括数组 */
VarDec:   ID          {$$=mknode(0,ID,yylineno);strcpy($$->type_id,$1);}   //ID结点，标识符符号串存放结点的type_id
        | ID ARRAYLIST     {$$=mknode(2,ARRAY,yylineno,$1,$2); strcpy($$->type_id,$1);} /*多维数组的标识符,采用递归，但是左递归可能会导致一些意外  现在还没有提取数组的大小，只是提取了数组的名字   在ID归约时就建立了语法树结点，在这里不再需要建立树结点*/
         ;

/* 数组索引列表 [2][2]*/
ARRAYLIST: LB Exp RB   {$$=mknode(1,ARRAY_LIST,yylineno,$2);  if($2->type==INT) $$->type_int=$2->type_int;}
            | LB Exp RB ARRAYLIST {$$=mknode(2,ARRAY_LIST,yylineno,$2,$4);  if($2->type==INT) $$->type_int=$2->type_int;}
           ;
           
/*  */

/* 函数声明 */
FuncDec: ID LP VarList RP   {$$=mknode(1,FUNC_DEC,yylineno,$3);strcpy($$->type_id,$1);}//函数名存放在$$->type_id
        |ID LP  RP   {$$=mknode(0,FUNC_DEC,yylineno);strcpy($$->type_id,$1);$$->ptr[0]=NULL;}//函数名存放在$$->type_id
        | error RC   {$$=NULL;}
        ;  


/* 函数参数声明列表 */
VarList: ParamDec  {$$=mknode(1,PARAM_LIST,yylineno,$1);}
        | ParamDec COMMA  VarList  {$$=mknode(2,PARAM_LIST,yylineno,$1,$3);}
        | error RC   {$$=NULL;}
        ;


/* 函数参数声明 */
ParamDec: Specifier VarDec         {$$=mknode(2,PARAM_DEC,yylineno,$1,$2);}
         ;


/* 复合语句 */
CompSt: LC DefList StmList RC    {$$=mknode(2,COMP_STM,yylineno,$2,$3);}
       ;


/* 声明列表 一系列语句或语句块 */
StmList: {$$=NULL; }  
        | Stmt StmList  {$$=mknode(2,STM_LIST,yylineno,$1,$2);}
        ;


/* 声明 语句或语句块 */
Stmt:   Exp SEMI    {$$=mknode(1,EXP_STMT,yylineno,$1);}
      | CompSt      {$$=$1;}      //复合语句结点直接最为语句结点，不再生成新的结点
      | RETURN Exp SEMI   {$$=mknode(1,RETURN,yylineno,$2);}
      | IF LP Exp RP Stmt %prec LOWER_THEN_ELSE   {$$=mknode(2,IF_THEN,yylineno,$3,$5);}
      | IF LP Exp RP Stmt ELSE Stmt   {$$=mknode(3,IF_THEN_ELSE,yylineno,$3,$5,$7);}
      | WHILE LP Exp RP Stmt {$$=mknode(2,WHILE,yylineno,$3,$5);}
      | FOR LP Exp SEMI Exp SEMI Exp RP Stmt {$$=mknode(4,FOR,yylineno,$3,$5,$7,$9);}
      | CASE Exp COLON Stmt {$$=mknode(2,CASE_THEN,yylineno,$2,$4);}
      | DEFAULT COLON Stmt {$$=mknode(1,DEFAULT_THEN,yylineno,$3);}
      | SWITCH LP Exp RP Stmt {$$=mknode(2,SWITCH,yylineno,$3,$5);}
      | SKIP SEMI {$$=mknode(1,SKIP_SEMI,yylineno,$1);}
      ;

/* 跳转语句 */
SKIP:   BREAK {$$=mknode(0,BREAK,yylineno); strcpy($$->type_id,$1);}
      | CONTINUE {$$=mknode(0,CONTINUE,yylineno); strcpy($$->type_id,$1);}


/* 局部变量定义列表 */
DefList: {$$=NULL; }
        | Def DefList {$$=mknode(2,DEF_LIST,yylineno,$1,$2);}
        | error SEMI   {$$=NULL;}
        ;


/* 局部变量定义 */
Def:    Specifier DecList SEMI {$$=mknode(2,VAR_DEF,yylineno,$1,$2);}
        ;


/*局部变量名列表*/
DecList: Dec  {$$=mknode(1,DEC_LIST,yylineno,$1);}
       | Dec COMMA DecList  {$$=mknode(2,DEC_LIST,yylineno,$1,$3);}
	   ;


/* 赋值语句（用Exp，包含变量和常量）*/
Dec:     VarDec  {$$=$1;}
        | VarDec ASSIGNOP Exp  {$$=mknode(2,ASSIGNOP,yylineno,$1,$3);strcpy($$->type_id,"ASSIGNOP");}
       ;


/* 表达式 */
Exp:    Exp ASSIGNOP Exp {$$=mknode(2,ASSIGNOP,yylineno,$1,$3);strcpy($$->type_id,"ASSIGNOP");}//$$结点type_id空置未用，正好存放运算符
      | Exp AND Exp   {$$=mknode(2,AND,yylineno,$1,$3);strcpy($$->type_id,"AND");}
      | Exp OR Exp    {$$=mknode(2,OR,yylineno,$1,$3);strcpy($$->type_id,"OR");}
      | Exp RELOP Exp {$$=mknode(2,RELOP,yylineno,$1,$3);strcpy($$->type_id,$2);}  //词法分析关系运算符号自身值保存在$2中
      | Exp PLUS Exp  {$$=mknode(2,PLUS,yylineno,$1,$3);strcpy($$->type_id,"PLUS");/* 这个type_id赋值要放在最后，不然后面使用时会无效 */ if($1->type==INT&&$3->type==INT) {printf("int加法运算%d + %d = %d\n",$1->type_int,$3->type_int,$1->type_int+$3->type_int); $$->type=INT; $$->type_int=$1->type_int+$3->type_int;} if($1->type==FLOAT&&$3->type==FLOAT) { printf("float加法运算%f + %f = %f\n",$1->type_float,$3->type_float,$1->type_float+$3->type_float);}   if($1->type==INT&&$3->type==FLOAT) { printf("int和float混合加法运算%f + %f = %f\n",(float)$1->type_int,$3->type_float,(float)$1->type_int+$3->type_float);}   if($1->type==FLOAT&&$3->type==INT) { printf("float和int混合加法运算%f + %f = %f\n",$1->type_float,(float)$3->type_int,$1->type_float+(float)$3->type_int);}  strcpy($$->type_id,"PLUS");}
      | Exp SELFPLUS  {$$=mknode(1,SELFPLUS,yylineno,$1);strcpy($$->type_id,"SELFPLUS");} //自增，如：a++
      | SELFPLUS Exp  {$$=mknode(1,SELFPLUS,yylineno,$2);strcpy($$->type_id,"SELFPLUS");} //自增，如：++a
      | Exp MINUS Exp {$$=mknode(2,MINUS,yylineno,$1,$3);strcpy($$->type_id,"MINUS");/* 这个type_id赋值要放在最后，不然后面使用时会无效 */ if($1->type==INT&&$3->type==INT) {printf("int加法运算%d - %d = %d\n",$1->type_int,$3->type_int,$1->type_int-$3->type_int); $$->type=INT; $$->type_int=$1->type_int-$3->type_int;} if($1->type==FLOAT&&$3->type==FLOAT) { printf("float加法运算%f + %f = %f\n",$1->type_float,$3->type_float,$1->type_float-$3->type_float);}   if($1->type==INT&&$3->type==FLOAT) { printf("int和float混合加法运算%f + %f = %f\n",(float)$1->type_int,$3->type_float,(float)$1->type_int-$3->type_float);}   if($1->type==FLOAT&&$3->type==INT) { printf("float和int混合加法运算%f + %f = %f\n",$1->type_float,(float)$3->type_int,$1->type_float-(float)$3->type_int);}  strcpy($$->type_id,"MINUS");}
      | Exp SELFMINUS {$$=mknode(1,SELFMINUS,yylineno,$1);strcpy($$->type_id,"SELFMINUS");} //自减，如：a--
      | SELFMINUS Exp {$$=mknode(1,SELFMINUS,yylineno,$2);strcpy($$->type_id,"SELFMINUS");} //自减，如：--a
      | Exp STAR Exp  {$$=mknode(2,STAR,yylineno,$1,$3);strcpy($$->type_id,"STAR");}
      | Exp DIV Exp   {$$=mknode(2,DIV,yylineno,$1,$3);strcpy($$->type_id,"DIV");}
      | LP Exp RP     {$$=$2;}
      | MINUS Exp %prec UMINUS   {$$=mknode(1,UMINUS,yylineno,$2);strcpy($$->type_id,"UMINUS");}
      | NOT Exp       {$$=mknode(1,NOT,yylineno,$2);strcpy($$->type_id,"NOT");}
      | DPLUS  Exp    {$$=mknode(1,DPLUS,yylineno,$2);strcpy($$->type_id,"DPLUS");}
      | Exp DPLUS     {$$=mknode(1,DPLUS,yylineno,$1);strcpy($$->type_id,"DPLUS");}
      | ID LP Args RP {$$=mknode(1,FUNC_CALL,yylineno,$3);strcpy($$->type_id,$1);}
      | ID LP RP      {$$=mknode(0,FUNC_CALL,yylineno);strcpy($$->type_id,$1);}
      | ID            {$$=mknode(0,ID,yylineno); $$->type=ID; strcpy($$->type_id,$1);} /* type_id是ID的语义值，比如：a=1，a的语义值就是1 这里还传给Exp让其之后使用*/
      | INT           {$$=mknode(0,INT,yylineno);  $$->type=INT; $$->type_int=$1;/*感觉最上面的Exp归约，在又部，$3取的是INT的最后一个被操作过的数据，把这句代码放在keepIntIndex前面就会取到type_id的内容*/}//自定义新增，赋值给type_id，为了记录数组下标,反正原来也没有用到INT的这个属性，应该不会影响原来的功能
      | FLOAT         {$$=mknode(0,FLOAT,yylineno);$$->type_float=$1;$$->type=FLOAT;}
      | CHAR          {$$=mknode(0,CHAR,yylineno);$$->type_char=$1;$$->type=CHAR;}
      | STRING        {$$=mknode(0,STRING,yylineno);strcpy($$->type_string,$1);  splitQuote($$->type_string);  $$->type=STRING;}//获取STRING的属性，也就是语义值
      | ID ARRAYLIST  {$$=mknode(2,ARRAY_USE,yylineno,mknode(0,ID,yylineno),$2); strcpy($$->type_id,$1);}
     // | Exp LB Exp RB {$$=mknode(0,ID,yylineno); if($3->type_int>=0&&strlen($3->type_id)==0) itoa($3->type_int,$3->type_id,10);  myStrcat($1->type_id,$3->type_id); strcpy($$->type_id,$1->type_id);}  /* 多维数组的按索引访问   ID归约到Exp,后面的LB Exp RB也归约到Exp   由于是给数组的索引而不是数组赋值，所以要拼接字符串     如果数组下标是int而不是变量需要if条件里面的一个额外处理*/  
      | Exp PLUS ASSIGNOP Exp  {$$=mknode(2,PLUS_ASSIGNOP,yylineno,$1,$4);strcpy($$->type_id,"MINUS_ASSIGNOP");} //复合加法
      | Exp MINUS ASSIGNOP Exp {$$=mknode(2,MINUS_ASSIGNOP,yylineno,$1,$4);strcpy($$->type_id,"PLUS_ASSIGNOP");} //复合减法
      | Exp STAR ASSIGNOP Exp  {$$=mknode(2,STAR_ASSIGNOP,yylineno,$1,$4);strcpy($$->type_id,"STAR_ASSIGNOP");} //复合乘法
      | Exp DIV ASSIGNOP Exp   {$$=mknode(2,DIV_ASSIGNOP,yylineno,$1,$4);strcpy($$->type_id,"DIV_ASSIGNOP");} //复合除法
      ;



Args:    Exp COMMA Args    {$$=mknode(2,ARGS,yylineno,$1,$3);}
       | Exp               {$$=mknode(1,ARGS,yylineno,$1);}
       ;
       
%%

char * buffer[100];/*自定义数据缓冲,有可能要换成静态的*/

int main(int argc, char *argv[]){
	yyin=fopen(argv[1],"r");//以只读方式打开文件
	if (!yyin) return -1;//原来这里啥也没有返回，导致编译报错
	yylineno=1;
	yyparse();
	return 0;
	}

char *keepIntIndex(char *des,int src){//完成数组字符串的拼接 拼接int
        int num=src;
        itoa(num, des, 10);
        //printf("=================数组下标是：%s\n",des);
        return des;
}

void myStrcat(char *des,const char *src){//完成数组字符串的拼接 拼接标识符
        char *lb="[";
        char *rb="]";
        des=strcat(des,lb);
        des=strcat(des,src);
        des=strcat(des,rb);
        free(lb);
        free(rb);
}

void splitQuote(char *str){//C语言疑似用不了比较高级的正则表达式，导致词法分析获取的字符串多了一对引号,这里删除引号
        int len=(int)strlen(str);
        for(int i=0;i<len;i++){
            str[i]=str[i+1];
        }
        str[len-2]='\0';
        str[len-1]='\0';
}

#include<stdarg.h>
void yyerror(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "Grammar Error at Line %d Column %d: ", yylloc.first_line,yylloc.first_column);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, ".\n");
}