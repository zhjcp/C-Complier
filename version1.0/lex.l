%{
#include "parser.tab.h"
#include "string.h"
#include "def.h"
int yycolumn=1;
#define YY_USER_ACTION    	yylloc.first_line=yylloc.last_line=yylineno; \
	yylloc.first_column=yycolumn;	yylloc.last_column=yycolumn+yyleng-1; yycolumn+=yyleng;
typedef union {
	//用联合将这多种类型统一起来
	int  type_int;
	int  type_float;
	char type_char; //char在定义时还包括
	char type_string[50]; //预设长度为50的字符串数组|如果用指针而不是数组，要动态分配内存,比较麻烦，好像会导致内存泄露
	char type_id[32]; //type_id表示标识符的名字
	struct node *ptr;
} YYLVAL;
#define YYSTYPE YYLVAL

%}
%option yylineno

id   [A-Za-z][A-Za-z0-9]*  
int    [0-9]+
float  ([0-9]*\.[0-9]+)|([0-9]+\.)
char    '.'
string  \"(\\.|[^\"])*\"
annotation1  \/\/[^\n]*
annotation2  \/\*(\s|.|\r|\n)*?\*\/

%%
{annotation1} {printf("%s%s\n","行注释：",yytext); } /* 行注释 */  
{annotation2} {printf("%s%s\n","块注释：",yytext); } /* 块注释 */
{int}        {yylval.type_int=atoi(yytext);         return INT;} /* int值 */
{float}      {yylval.type_float=atof(yytext);       return FLOAT;} /* float值 */
{char}       {yylval.type_char=yytext[1];           return CHAR;} /* char值 yytext长度为3 'x' 索引=2处是字符*/
{string}     {strcpy(yylval.type_string,yytext);    return STRING;} /* string的值 正则(?<=").*(?=") 好像无法在这里使用*/


"int"       	  {strcpy(yylval.type_id,  yytext);   return TYPE;} /* int类型 */
"float"      	  {strcpy(yylval.type_id,  yytext);   return TYPE;} /* float类型 */
"char"      	  {strcpy(yylval.type_id,  yytext);   return TYPE;} /* char类型 */
"char *"          {strcpy(yylval.type_id,  yytext);   return TYPE;} /* string类型 C语言中是没有专门的字符串类型的，但为了表现字符串的效果，我伪造了一个字符串类型（本质上仍然是char指针）*/
"const"     	  {strcpy(yylval.type_id,  yytext);   return CONST;} /* 常数类型 */

"return"     {  return RETURN;}
"if"         {  return IF;}
"else"       {  return ELSE;}
"while"      {  return WHILE;}
"for"        {  return FOR;}
"switch"     {  return SWITCH;}
"case"       {  return CASE;}
"default"    {  return DEFAULT;}
"break"      {  strcpy(yylval.type_id,  yytext); return BREAK;}
"continue"   {  strcpy(yylval.type_id,  yytext); return CONTINUE;}

{id}        {   strcpy(yylval.type_id,  yytext); return ID;/*由于关键字的形式也符合表示符的规则，所以把关键字的处理全部放在标识符的前面，优先识别*/}
";"			{   return SEMI;}
":"         {   return COLON;}
","			{   return COMMA;}
">"|"<"|">="|"<="|"=="|"!="   {   strcpy(yylval.type_id, yytext);;return RELOP;}
"="			{   return ASSIGNOP;}
"+"			{   return PLUS;}
"++"        {   return SELFPLUS;}
"-"			{   return MINUS;}
"--"        {   return SELFMINUS;}
"*"			{   return STAR;}
"/"			{   return DIV;}
"&&"        {   return AND;}
"||"        {   return OR;}
"!"			{   return NOT;}
"("			{   return LP;}
")"			{   return RP;}
"["			{   return LB;}
"]"			{   return RB;}
"{"			{   return LC;}
"}"			{   return RC;}
[\n]        {yycolumn=1;}   
[ \r\t]     {}   
.			{printf("Error type A :Mysterious character \"%s\"\n\t at Line %d\n",yytext,yylineno);}

%%
/* 原53行：作为实验内容，还需要考虑识别出2种形式的注释注释部分时，直接舍弃  只是我们之后需要补充的功能*/
/* 和bison联用时，不需要这部分
void main()
{
yylex();
return 0;
}

*/
int yywrap()
{
return 1;
}