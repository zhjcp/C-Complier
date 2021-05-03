
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INT = 258,
     ID = 259,
     RELOP = 260,
     TYPE = 261,
     CONST_TYPE = 262,
     FLOAT = 263,
     CHAR = 264,
     STRING = 265,
     BREAK = 266,
     CONTINUE = 267,
     DPLUS = 268,
     LP = 269,
     RP = 270,
     LB = 271,
     RB = 272,
     LC = 273,
     RC = 274,
     SEMI = 275,
     COMMA = 276,
     COLON = 277,
     CONST = 278,
     PLUS = 279,
     SELFPLUS = 280,
     MINUS = 281,
     SELFMINUS = 282,
     STAR = 283,
     DIV = 284,
     ASSIGNOP = 285,
     PLUS_ASSIGNOP = 286,
     MINUS_ASSIGNOP = 287,
     STAR_ASSIGNOP = 288,
     DIV_ASSIGNOP = 289,
     AND = 290,
     OR = 291,
     NOT = 292,
     IF = 293,
     ELSE = 294,
     WHILE = 295,
     RETURN = 296,
     FOR = 297,
     SWITCH = 298,
     CASE = 299,
     DEFAULT = 300,
     ARRAY = 301,
     ARRAY_USE = 302,
     EXT_DEF_LIST = 303,
     EXT_VAR_DEF = 304,
     EXT_VARCONST_DEF = 305,
     FUNC_DEF = 306,
     FUNC_DEC = 307,
     EXT_DEC_LIST = 308,
     ARRAY_LIST = 309,
     ARRAY_LIST_CALL = 310,
     PARAM_LIST = 311,
     PARAM_DEC = 312,
     VAR_DEF = 313,
     DEC_LIST = 314,
     DEF_LIST = 315,
     COMP_STM = 316,
     STM_LIST = 317,
     EXP_STMT = 318,
     SKIP_SEMI = 319,
     IF_THEN = 320,
     IF_THEN_ELSE = 321,
     CASE_THEN = 322,
     DEFAULT_THEN = 323,
     FUNC_CALL = 324,
     ARGS = 325,
     FUNCTION = 326,
     PARAM = 327,
     ARG = 328,
     CALL = 329,
     LABEL = 330,
     GOTO = 331,
     JLT = 332,
     JLE = 333,
     JGT = 334,
     JGE = 335,
     EQ = 336,
     NEQ = 337,
     UMINUS = 338,
     LOWER_THEN_ELSE = 339
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 19 "parser.y"

	int    type_int;
	float  type_float;
        char   type_char; //char
        char   type_string[100]; //string
	char   type_id[32];
	struct ASTNode *ptr;



/* Line 1676 of yacc.c  */
#line 147 "parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYLTYPE yylloc;

