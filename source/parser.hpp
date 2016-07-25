/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
     LET = 258,
     LOG = 259,
     ASSERT = 260,
     EQUALS = 261,
     PLUS = 262,
     MULT = 263,
     DIV = 264,
     INTDIV = 265,
     MOD = 266,
     EXP = 267,
     SUB = 268,
     PLUS_MINUS = 269,
     ID = 270,
     STRING = 271,
     NUMBER = 272,
     NOT_EQUALS = 273,
     GREATER_EQUAL = 274,
     LESS_EQUAL = 275
   };
#endif
/* Tokens.  */
#define LET 258
#define LOG 259
#define ASSERT 260
#define EQUALS 261
#define PLUS 262
#define MULT 263
#define DIV 264
#define INTDIV 265
#define MOD 266
#define EXP 267
#define SUB 268
#define PLUS_MINUS 269
#define ID 270
#define STRING 271
#define NUMBER 272
#define NOT_EQUALS 273
#define GREATER_EQUAL 274
#define LESS_EQUAL 275




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 22 "parser.y"
{
	Function * function;
	Parameter * parameter;
	Statement * statement;
	Expression * expression;

	std::vector<std::shared_ptr<Parameter>> * parameterList;
	std::vector<IdParameter> * idParameterList;
	std::vector<std::shared_ptr<Statement>> * statementList;
	std::vector<std::shared_ptr<Expression>> * expressionList;

	float number;
	std::string * id;
}
/* Line 1529 of yacc.c.  */
#line 104 "parser.hpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

