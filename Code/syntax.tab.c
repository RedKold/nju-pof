/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "./syntax.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree/tree.h"
#include "lex.yy.c"

// int yylex();

// // include it;
// extern char* yytext;
// extern int yylineno;
int yyerror(const char*s);

// Declare global variables from lexical.l
extern int struct_line_number; // for T_STRUCT
extern int lc_line_number;     // for T_LC

extern int parse_ok;

// Global AST root for semantic analysis
TreeNode* ast_root = NULL;


int error_cnt=0;
int has_error = 0;
// 错误处理函数
int yyerror(const char* msg) {
    // 检查该行是否已经有词法错误，如果有，就不报告语法错误
    extern int line_has_error[];
    if (!line_has_error[yylineno]) {
        error_cnt++;
        has_error = 1;
        // 替换常见错误信息为更友好的提示
        if (strstr(msg, "T_SEMI") != NULL) {
            // 检查是否是匿名结构体声明导致的错误
            extern FILE* yyin;
            long pos = ftell(yyin);
            // 查找当前行的开头
            long line_start = pos;
            while (line_start > 0 && fgetc(yyin) != '\n') line_start--;
            if (line_start > 0) line_start++;
            fseek(yyin, line_start, SEEK_SET);

            char buffer[256];
            int i = 0;
            char c;
            while ((c = fgetc(yyin)) != '\n' && c != EOF && i < 255) {
                buffer[i++] = c;
            }
            buffer[i] = '\0';
            fseek(yyin, pos, SEEK_SET);

            if (strstr(buffer, "struct") && (strstr(buffer, "{") || strstr(msg, "unexpected T_SEMI"))) {
                printf("Error type B at Line %d: Anonymous struct declaration.\n", yylineno);
            } else {
                printf("Error type B at Line %d: Missing \";\"\n", yylineno);
            }
        } else if (strstr(msg, "T_RB") != NULL) {
            printf("Error type B at Line %d: Missing \"]\"\n", yylineno);
        } else if (strstr(msg, "T_COMMA") != NULL && strstr(msg, "unexpected") != NULL) {
            // 处理 a[5,3] 这种情况的错误
            printf("Error type B at Line %d: Missing \"]\"\n", yylineno);
        } else if (strstr(msg, "T_ELSE") != NULL && strstr(msg, "unexpected") != NULL) {
            // 处理 if 语句后缺少分号的错误
            printf("Error type B at Line %d: Missing \";\"\n", yylineno);
        } else if (strstr(msg, "T_FLOAT") != NULL && strstr(msg, "array") != NULL || strstr(msg, "T_FLOAT") != NULL && strstr(msg, "T_INT") != NULL) {
            // 数组维度必须是整数
            printf("Error type B at Line %d: Array dimension must be integer.\n", yylineno);
        } else if (strstr(msg, "trailing") != NULL && strstr(msg, "comma") != NULL) {
            // 函数参数中的尾随逗号
            printf("Error type B at Line %d: Trailing comma in function parameters.\n", yylineno);
        } else if (strstr(msg, "anonymous") != NULL && strstr(msg, "struct") != NULL) {
            // 匿名结构体声明
            printf("Error type B at Line %d: Anonymous struct declaration.\n", yylineno);
        } else if (strstr(msg, "array") != NULL && strstr(msg, "initialization") != NULL) {
            // 数组初始化不支持
            printf("Error type B at Line %d: Array initialization not supported.\n", yylineno);
        } else if (strstr(msg, "empty") != NULL && strstr(msg, "statement") != NULL) {
            // 空的while语句体
            printf("Error type B at Line %d: Empty while statement body.\n", yylineno);
        } else {
            // 其他错误
            if (strstr(msg, "unexpected T_FLOAT, expecting T_INT")) {
                printf("Error type B at Line %d: Array dimension must be integer.\n", yylineno);
            } else if (strstr(msg, "unexpected T_RP, expecting")) {
                // 检查是否是函数参数尾随逗号
                extern FILE* yyin;
                long pos = ftell(yyin);
                fseek(yyin, -2, SEEK_CUR);
                char c = fgetc(yyin);
                fseek(yyin, pos, SEEK_SET);
                if (c == ',') {
                    printf("Error type B at Line %d: Trailing comma in function parameters.\n", yylineno);
                } else {
                    printf("Error type B at Line %d: %s\n", yylineno, msg);
                }
            } else if (strstr(msg, "unexpected T_LC")) {
                // 可能是数组初始化
                printf("Error type B at Line %d: Array initialization not supported.\n", yylineno);
            } else if (strstr(msg, "unexpected T_SEMI")) {
                // 检查是否是while语句后的空体
                extern FILE* yyin;
                long pos = ftell(yyin);
                long search_pos = pos - 30; // 搜索前30个字符
                if (search_pos < 0) search_pos = 0;
                fseek(yyin, search_pos, SEEK_SET);

                char buffer[100];
                fread(buffer, 1, pos - search_pos, yyin);
                buffer[pos - search_pos] = '\0';
                fseek(yyin, pos, SEEK_SET);

                if (strstr(buffer, "while")) {
                    printf("Error type B at Line %d: Empty while statement body.\n", yylineno);
                } else {
                    printf("Error type B at Line %d: %s\n", yylineno, msg);
                }
            } else {
                printf("Error type B at Line %d: %s\n", yylineno, msg);
            }
        }
        line_has_error[yylineno] = 1; // 标记该行有错误
    }
    return 0;
}

#line 198 "./syntax.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_SYNTAX_TAB_H_INCLUDED
# define YY_YY_SYNTAX_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_INT = 258,
    T_FLOAT = 259,
    T_ID = 260,
    T_SEMI = 261,
    T_COMMA = 262,
    T_ASSIGNOP = 263,
    T_RELOP = 264,
    T_PLUS = 265,
    T_MINUS = 266,
    T_STAR = 267,
    T_DIV = 268,
    T_AND = 269,
    T_OR = 270,
    T_DOT = 271,
    T_NOT = 272,
    T_TYPE = 273,
    T_LP = 274,
    T_RP = 275,
    T_LB = 276,
    T_RB = 277,
    T_LC = 278,
    T_RC = 279,
    T_STRUCT = 280,
    T_RETURN = 281,
    T_IF = 282,
    T_ELSE = 283,
    T_WHILE = 284,
    UMINUS = 285
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 153 "./syntax.y"

    TreeNode* node;  // 语法树节点指针
    int num;
    float real;
    char id[32];

#line 288 "./syntax.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_SYNTAX_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  16
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   387

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  31
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  23
/* YYNRULES -- Number of rules.  */
#define YYNRULES  80
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  147

#define YYUNDEFTOK  2
#define YYMAXUTOK   285


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   171,   171,   178,   189,   198,   204,   206,   211,   219,
     227,   234,   241,   251,   255,   263,   269,   277,   290,   300,
     305,   311,   317,   322,   341,   361,   384,   396,   408,   410,
     415,   421,   426,   441,   460,   470,   472,   477,   485,   492,
     497,   504,   506,   515,   527,   543,   558,   577,   583,   588,
     599,   604,   615,   620,   652,   685,   693,   701,   709,   717,
     725,   733,   741,   743,   750,   758,   803,   809,   819,   828,
     838,   846,   851,   858,   865,   869,   883,   891,   893,   899,
     904
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_INT", "T_FLOAT", "T_ID", "T_SEMI",
  "T_COMMA", "T_ASSIGNOP", "T_RELOP", "T_PLUS", "T_MINUS", "T_STAR",
  "T_DIV", "T_AND", "T_OR", "T_DOT", "T_NOT", "T_TYPE", "T_LP", "T_RP",
  "T_LB", "T_RB", "T_LC", "T_RC", "T_STRUCT", "T_RETURN", "T_IF", "T_ELSE",
  "T_WHILE", "UMINUS", "$accept", "epsilon", "program", "extDefList",
  "extDef", "extDecList", "specifier", "structSpecifier", "optTag", "tag",
  "varDec", "funDec", "varList", "paramDec", "compSt", "stmtList", "stmt",
  "defList", "def", "decList", "dec", "expr", "args", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285
};
# endif

#define YYPACT_NINF (-80)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-79)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      14,     3,   -80,    11,   -80,    26,   -80,    37,    92,   -80,
      37,    37,    -1,   -80,    12,   -80,   -80,     3,   -80,    35,
     -80,    62,    44,    47,   -80,   -80,    -8,    41,   -80,    69,
     238,    -8,   -80,   -80,    69,    54,    -8,   -80,   -80,    69,
      66,    75,   -80,   -80,    79,    82,    72,   255,   255,   255,
     281,    76,     4,   103,    81,   -80,   -80,   106,   -80,    -8,
     -80,   -80,   209,   -80,   -80,    51,    55,   311,   204,   255,
     255,   255,   255,   255,   255,   255,   133,   229,   -80,     5,
     255,   120,   123,   -80,   -80,   116,    76,   190,   255,   -80,
      69,   -80,    -2,   -80,   297,   146,   -80,   156,   353,   104,
      51,    51,    55,    55,   142,   366,   -80,   148,   173,    76,
      76,   267,   255,   255,   -80,   -80,   137,   -80,   353,   -80,
     234,   -80,   145,   -80,   -80,   144,   -80,   -80,   -80,   -80,
     -80,   -80,   325,   339,   -80,   -80,   -80,   -80,   118,   107,
     165,   149,   -80,   -80,   -80,   118,   -80
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     4,    15,     2,     6,     0,     3,     0,     0,    16,
       0,     0,    21,    20,     0,    18,     1,     0,     5,    22,
      10,     0,    13,     0,     7,     8,     2,     0,     9,     0,
       0,     2,    11,    48,     0,     0,     2,    27,    31,     0,
       0,    28,    22,    14,    72,    73,    71,     0,     0,     0,
       0,     0,    52,     0,    50,    17,    47,    32,    26,     2,
      23,    24,     0,    72,    73,    63,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    25,     0,
       0,     0,     0,    35,    41,     0,     0,     0,     0,    49,
       0,    29,     0,    68,    77,     0,    62,     0,    54,    57,
      58,    59,    60,    61,    55,    56,    70,     0,     0,     0,
       0,     0,     0,     0,    33,    34,     0,    38,    53,    51,
       0,    80,     0,    67,    66,     0,    74,    75,    69,    36,
      37,    42,     0,     0,    39,    79,    76,    65,     0,     0,
       0,    43,    45,    46,    40,     0,    44
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -80,    -3,   -80,   121,   -80,   143,   -23,   -80,   -80,   -80,
     -33,   -80,   117,   -80,   155,   -79,   -49,    22,   -80,    89,
     -80,   -28,   -47
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     4,     5,     6,     7,    21,     8,     9,    14,    15,
      22,    23,    40,    41,    84,    85,    86,    35,    36,    53,
      54,    87,    95
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      13,    52,    50,    34,    39,   120,    57,   115,    34,    10,
       2,   109,    88,    34,    -2,     1,    12,     3,   121,    65,
      66,    67,   -19,    33,    38,    30,    16,    11,    33,   110,
     129,   130,     2,    33,    94,    26,    39,    -2,    17,     3,
      98,    99,   100,   101,   102,   103,   104,   105,    83,   108,
     125,    29,   111,    51,    27,     2,    38,    52,    56,     2,
     118,    37,     3,    72,    73,    30,     3,    76,    28,    94,
      31,    76,    77,   135,    42,   136,    77,    79,    55,    63,
      64,    46,    59,    83,   132,   133,    58,    47,    90,   141,
     143,    62,    94,    48,    94,    49,   146,    19,    20,    31,
      -2,    60,    80,    81,    61,    82,    83,    83,   140,    89,
      63,    64,    46,   142,    70,    71,    72,    73,    47,   140,
      76,    63,    64,    46,    48,    77,    49,    30,    18,    47,
      31,    24,    25,    80,    81,    48,    82,    49,   106,   112,
     114,    31,   113,   134,    80,    81,    92,    82,    63,    64,
      46,    69,    70,    71,    72,    73,    47,    92,    76,    63,
      64,    46,    48,    77,    49,   -78,   123,    47,   137,   -78,
     126,   144,    43,    48,   127,    49,    91,   145,    32,   119,
     124,    68,    69,    70,    71,    72,    73,    74,    75,    76,
       0,   116,     0,     0,    77,   128,   117,     0,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    63,    64,    46,
      92,    77,    63,    64,    46,    47,     0,     0,     0,     0,
      47,    48,     0,    49,     0,     0,    48,    97,    49,    93,
     107,     0,    63,    64,    46,    92,     0,    63,    64,    46,
      47,    44,    45,    46,     0,    47,    48,     0,    49,    47,
       0,    48,     0,    49,     0,    48,     0,    49,    63,    64,
      46,     0,     0,     0,     0,     0,    47,     0,     0,     0,
       0,     0,    48,   131,    49,    68,    69,    70,    71,    72,
      73,    74,    75,    76,     0,     0,     0,     0,    77,    68,
      69,    70,    71,    72,    73,    74,    75,    76,     0,     0,
       0,     0,    77,    78,   122,    68,    69,    70,    71,    72,
      73,    74,    75,    76,     0,     0,     0,     0,    77,    68,
      69,    70,    71,    72,    73,    74,    75,    76,     0,     0,
       0,    96,    77,    68,    69,    70,    71,    72,    73,    74,
      75,    76,     0,     0,     0,   138,    77,    68,    69,    70,
      71,    72,    73,    74,    75,    76,     0,     0,     0,   139,
      77,    68,    69,    70,    71,    72,    73,    74,    75,    76,
       0,     0,     0,     0,    77,    69,    70,    71,    72,    73,
      74,     0,    76,     0,     0,     0,     0,    77
};

static const yytype_int16 yycheck[] =
{
       3,    34,    30,    26,    27,     7,    39,    86,    31,     6,
      18,     6,     8,    36,     0,     1,     5,    25,    20,    47,
      48,    49,    23,    26,    27,    21,     0,    24,    31,    24,
     109,   110,    18,    36,    62,    23,    59,     0,     1,    25,
      68,    69,    70,    71,    72,    73,    74,    75,    51,    77,
      97,     7,    80,    31,    19,    18,    59,    90,    36,    18,
      88,    20,    25,    12,    13,    21,    25,    16,     6,    97,
      23,    16,    21,   120,     5,   122,    21,     1,    24,     3,
       4,     5,     7,    86,   112,   113,    20,    11,     7,   138,
     139,    19,   120,    17,   122,    19,   145,     5,     6,    23,
      24,    22,    26,    27,    22,    29,   109,   110,     1,     6,
       3,     4,     5,     6,    10,    11,    12,    13,    11,     1,
      16,     3,     4,     5,    17,    21,    19,    21,     7,    11,
      23,    10,    11,    26,    27,    17,    29,    19,     5,    19,
      24,    23,    19,     6,    26,    27,     1,    29,     3,     4,
       5,     9,    10,    11,    12,    13,    11,     1,    16,     3,
       4,     5,    17,    21,    19,    20,    20,    11,    24,    24,
      22,     6,    29,    17,     1,    19,    59,    28,    23,    90,
      24,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      -1,     1,    -1,    -1,    21,    22,     6,    -1,     8,     9,
      10,    11,    12,    13,    14,    15,    16,     3,     4,     5,
       1,    21,     3,     4,     5,    11,    -1,    -1,    -1,    -1,
      11,    17,    -1,    19,    -1,    -1,    17,    23,    19,    20,
       1,    -1,     3,     4,     5,     1,    -1,     3,     4,     5,
      11,     3,     4,     5,    -1,    11,    17,    -1,    19,    11,
      -1,    17,    -1,    19,    -1,    17,    -1,    19,     3,     4,
       5,    -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,
      -1,    -1,    17,     6,    19,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    -1,    21,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    -1,    -1,
      -1,    -1,    21,    22,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    -1,    21,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    -1,    -1,
      -1,    20,    21,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    -1,    -1,    -1,    20,    21,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    -1,    -1,    -1,    20,
      21,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      -1,    -1,    -1,    -1,    21,     9,    10,    11,    12,    13,
      14,    -1,    16,    -1,    -1,    -1,    -1,    21
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,    18,    25,    32,    33,    34,    35,    37,    38,
       6,    24,     5,    32,    39,    40,     0,     1,    34,     5,
       6,    36,    41,    42,    34,    34,    23,    19,     6,     7,
      21,    23,    45,    32,    37,    48,    49,    20,    32,    37,
      43,    44,     5,    36,     3,     4,     5,    11,    17,    19,
      52,    48,    41,    50,    51,    24,    48,    41,    20,     7,
      22,    22,    19,     3,     4,    52,    52,    52,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    21,    22,     1,
      26,    27,    29,    32,    45,    46,    47,    52,     8,     6,
       7,    43,     1,    20,    52,    53,    20,    23,    52,    52,
      52,    52,    52,    52,    52,    52,     5,     1,    52,     6,
      24,    52,    19,    19,    24,    46,     1,     6,    52,    50,
       7,    20,     7,    20,    24,    53,    22,     1,    22,    46,
      46,     6,    52,    52,     6,    53,    53,    24,    20,    20,
       1,    47,     6,    47,     6,    28,    47
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    31,    32,    33,    33,    34,    34,    34,    34,    35,
      35,    35,    35,    36,    36,    37,    37,    38,    38,    39,
      39,    40,    41,    41,    41,    41,    42,    42,    43,    43,
      43,    43,    44,    45,    46,    46,    46,    46,    47,    47,
      47,    47,    47,    47,    47,    47,    47,    48,    48,    49,
      50,    50,    51,    51,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    53,    53,    53,    53,
      53
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     1,     1,     2,     1,     3,     3,     3,
       2,     3,     2,     1,     3,     1,     1,     5,     2,     1,
       1,     1,     1,     4,     4,     4,     4,     3,     1,     3,
       2,     1,     2,     4,     2,     1,     3,     3,     2,     3,
       2,     1,     3,     5,     7,     5,     5,     2,     1,     3,
       1,     3,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     5,     4,     4,     3,     4,
       3,     1,     1,     1,     4,     4,     3,     1,     2,     3,
       2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 171 "./syntax.y"
       { (yyval.node) = NULL; }
#line 1720 "./syntax.tab.c"
    break;

  case 3:
#line 179 "./syntax.y"
       {
         // we let TreeNote as a part of attribute
         (yyval.node) = createTreeNode(NODE_PROGRAM, "Program", 1); // 固定为第1行
         addChild((yyval.node), (yyvsp[0].node));
         // if has error, don't print it
        if(has_error == 0)
          printTree((yyval.node), 0);
         // Store AST root for semantic analysis instead of freeing
         ast_root = (yyval.node);
       }
#line 1735 "./syntax.tab.c"
    break;

  case 4:
#line 190 "./syntax.y"
       {
         // 程序级别的错误恢复
         has_error = 1;
         (yyval.node) = createTreeNode(NODE_PROGRAM, "Program", yylineno);
       }
#line 1745 "./syntax.tab.c"
    break;

  case 5:
#line 199 "./syntax.y"
          {
            (yyval.node) = createTreeNode(NODE_EXTDEF_LIST, "ExtDefList", (yyvsp[-1].node)->lineNumber);
            addChild((yyval.node), (yyvsp[-1].node));
            addChild((yyval.node), (yyvsp[0].node));
          }
#line 1755 "./syntax.tab.c"
    break;

  case 6:
#line 205 "./syntax.y"
          { (yyval.node) = NULL; }
#line 1761 "./syntax.tab.c"
    break;

  case 7:
#line 207 "./syntax.y"
          {
            has_error = 1;
            (yyval.node) = (yyvsp[0].node);
          }
#line 1770 "./syntax.tab.c"
    break;

  case 8:
#line 212 "./syntax.y"
          {
            has_error = 1;
            (yyval.node) = (yyvsp[0].node);
          }
#line 1779 "./syntax.tab.c"
    break;

  case 9:
#line 220 "./syntax.y"
      {
        (yyval.node) = createTreeNode(NODE_EXTDEF, "ExtDef", (yyvsp[-2].node)->lineNumber); // 固定为第1行
        addChild((yyval.node), (yyvsp[-2].node));
        addChild((yyval.node), (yyvsp[-1].node));
        TreeNode* semiNode = createTreeNode(NODE_SEMI, "SEMI", yylineno);
        addChild((yyval.node), semiNode);
      }
#line 1791 "./syntax.tab.c"
    break;

  case 10:
#line 228 "./syntax.y"
      {
        (yyval.node) = createTreeNode(NODE_EXTDEF, "ExtDef", (yyvsp[-1].node)->lineNumber); // 固定为第1行
        addChild((yyval.node), (yyvsp[-1].node));
        TreeNode* semiNode = createTreeNode(NODE_SEMI, "SEMI", yylineno);
        addChild((yyval.node), semiNode);
      }
#line 1802 "./syntax.tab.c"
    break;

  case 11:
#line 235 "./syntax.y"
      {
        (yyval.node) = createTreeNode(NODE_EXTDEF, "ExtDef", (yyvsp[-2].node)->lineNumber); // 固定为第1行
        addChild((yyval.node), (yyvsp[-2].node));
        addChild((yyval.node), (yyvsp[-1].node));
        addChild((yyval.node), (yyvsp[0].node));
      }
#line 1813 "./syntax.tab.c"
    break;

  case 12:
#line 242 "./syntax.y"
      {
        has_error = 1;
        (yyval.node) = NULL;
      }
#line 1822 "./syntax.tab.c"
    break;

  case 13:
#line 252 "./syntax.y"
          {
            (yyval.node) = (yyvsp[0].node);
          }
#line 1830 "./syntax.tab.c"
    break;

  case 14:
#line 256 "./syntax.y"
          {
            (yyval.node) = (yyvsp[-2].node);
            (yyvsp[-2].node)->nextSibling = (yyvsp[0].node);
          }
#line 1839 "./syntax.tab.c"
    break;

  case 15:
#line 264 "./syntax.y"
         {
           (yyval.node) = createTreeNode(NODE_SPECIFIER, "Specifier", yylineno);
           TreeNode* typeNode = createTreeNode(NODE_TYPE, yylval.id, yylineno);
           addChild((yyval.node), typeNode);
         }
#line 1849 "./syntax.tab.c"
    break;

  case 16:
#line 270 "./syntax.y"
         {
          //  printf("SPECIFIER: LINE:%d\n", $1->lineNumber);
           (yyval.node) = createTreeNode(NODE_SPECIFIER, "Specifier", (yyvsp[0].node)->lineNumber);
           addChild((yyval.node), (yyvsp[0].node));
         }
#line 1859 "./syntax.tab.c"
    break;

  case 17:
#line 278 "./syntax.y"
        {
          (yyval.node) = createTreeNode(NODE_STRUCTSPECIFIER, "StructSpecifier", struct_line_number);
          TreeNode* structNode = createTreeNode(NODE_STRUCT, "STRUCT", struct_line_number);
          addChild((yyval.node), structNode);
          addChild((yyval.node), (yyvsp[-3].node));
          TreeNode* lcNode = createTreeNode(NODE_LC, "LC", yylineno);
          addChild((yyval.node), lcNode);
          addChild((yyval.node), (yyvsp[-1].node));
          TreeNode* rcNode = createTreeNode(NODE_RC, "RC", yylineno);
          addChild((yyval.node), rcNode);
        }
#line 1875 "./syntax.tab.c"
    break;

  case 18:
#line 291 "./syntax.y"
        {
          (yyval.node) = createTreeNode(NODE_STRUCTSPECIFIER, "StructSpecifier", struct_line_number);
          TreeNode* structNode = createTreeNode(NODE_STRUCT, "STRUCT", struct_line_number);
          addChild((yyval.node), structNode);
          // 直接使用解析过程中实际获取到的标签值
          addChild((yyval.node), (yyvsp[0].node));
        }
#line 1887 "./syntax.tab.c"
    break;

  case 19:
#line 301 "./syntax.y"
        {
          (yyval.node) = createTreeNode(NODE_OPTTAG, "OptTag", struct_line_number);
          addChild((yyval.node), (yyvsp[0].node));
        }
#line 1896 "./syntax.tab.c"
    break;

  case 20:
#line 306 "./syntax.y"
        {
          (yyval.node) = NULL;
        }
#line 1904 "./syntax.tab.c"
    break;

  case 21:
#line 312 "./syntax.y"
        {
          (yyval.node) = createTreeNode(NODE_TAG, "Tag", yylineno);
          addChild((yyval.node), (yyvsp[0].node));
        }
#line 1913 "./syntax.tab.c"
    break;

  case 22:
#line 318 "./syntax.y"
      {
        (yyval.node) = createTreeNode(NODE_VARDEC, "VarDec", yylineno);
        addChild((yyval.node), (yyvsp[0].node));
      }
#line 1922 "./syntax.tab.c"
    break;

  case 23:
#line 323 "./syntax.y"
      {
        // 创建新的 VarDec 节点来包裹数组声明
        TreeNode* newVarDec = createTreeNode(NODE_VARDEC, "VarDec", yylineno);
        addChild(newVarDec, (yyvsp[-3].node));

        TreeNode* lbNode = createTreeNode(NODE_LB, "LB", yylineno);
        addChild(newVarDec, lbNode);

        TreeNode* intNode = createTreeNode(NODE_INT, "INT", yylineno);
        intNode->intVal = yylval.num;
        addChild(newVarDec, intNode);

        TreeNode* rbNode = createTreeNode(NODE_RB, "RB", yylineno);
        addChild(newVarDec, rbNode);

        (yyval.node) = newVarDec;
      }
#line 1944 "./syntax.tab.c"
    break;

  case 24:
#line 342 "./syntax.y"
      {
        printf("Error type B at Line %d: Array dimension must be integer.\n", yylineno);
        has_error = 1;

        TreeNode* newVarDec = createTreeNode(NODE_VARDEC, "VarDec", yylineno);
        addChild(newVarDec, (yyvsp[-3].node));

        TreeNode* lbNode = createTreeNode(NODE_LB, "LB", yylineno);
        addChild(newVarDec, lbNode);

        TreeNode* floatNode = createTreeNode(NODE_FLOAT, "FLOAT", yylineno);
        floatNode->floatVal = yylval.real;
        addChild(newVarDec, floatNode);

        TreeNode* rbNode = createTreeNode(NODE_RB, "RB", yylineno);
        addChild(newVarDec, rbNode);

        (yyval.node) = newVarDec;
      }
#line 1968 "./syntax.tab.c"
    break;

  case 25:
#line 362 "./syntax.y"
      {
        if ((yyvsp[-1].node)->type != NODE_INT) {
          printf("Error type B at Line %d: Array dimension must be integer.\n", yylineno);
          has_error = 1;
        }

        TreeNode* newVarDec = createTreeNode(NODE_VARDEC, "VarDec", yylineno);
        addChild(newVarDec, (yyvsp[-3].node));

        TreeNode* lbNode = createTreeNode(NODE_LB, "LB", yylineno);
        addChild(newVarDec, lbNode);

        addChild(newVarDec, (yyvsp[-1].node));

        TreeNode* rbNode = createTreeNode(NODE_RB, "RB", yylineno);
        addChild(newVarDec, rbNode);

        (yyval.node) = newVarDec;
      }
#line 1992 "./syntax.tab.c"
    break;

  case 26:
#line 385 "./syntax.y"
      {
        (yyval.node) = createTreeNode(NODE_FUNDEC, "FunDec", yylineno);
        addChild((yyval.node), (yyvsp[-3].node));
        TreeNode* lpNode = createTreeNode(NODE_LP, "LP", yylineno);
        TreeNode* rpNode = createTreeNode(NODE_RP, "RP", yylineno);
        addChild((yyval.node), lpNode);
        if ((yyvsp[-1].node) != NULL) {
            addChild((yyval.node), (yyvsp[-1].node));
        }
        addChild((yyval.node), rpNode);
      }
#line 2008 "./syntax.tab.c"
    break;

  case 27:
#line 397 "./syntax.y"
      {
        (yyval.node) = createTreeNode(NODE_FUNDEC, "FunDec", yylineno);
        addChild((yyval.node), (yyvsp[-2].node));
        TreeNode* lpNode = createTreeNode(NODE_LP, "LP", yylineno);
        TreeNode* rpNode = createTreeNode(NODE_RP, "RP", yylineno);
        addChild((yyval.node), lpNode);
        addChild((yyval.node), rpNode);
      }
#line 2021 "./syntax.tab.c"
    break;

  case 28:
#line 409 "./syntax.y"
      { (yyval.node) = (yyvsp[0].node); }
#line 2027 "./syntax.tab.c"
    break;

  case 29:
#line 411 "./syntax.y"
      {
        (yyval.node) = (yyvsp[-2].node);
        (yyvsp[-2].node)->nextSibling = (yyvsp[0].node);
      }
#line 2036 "./syntax.tab.c"
    break;

  case 30:
#line 416 "./syntax.y"
      {
        printf("Error type B at Line %d: Trailing comma in function parameters.\n", yylineno);
        has_error = 1;
        (yyval.node) = (yyvsp[-1].node);
      }
#line 2046 "./syntax.tab.c"
    break;

  case 31:
#line 422 "./syntax.y"
      { (yyval.node) = NULL; }
#line 2052 "./syntax.tab.c"
    break;

  case 32:
#line 427 "./syntax.y"
        {
          (yyval.node) = createTreeNode(NODE_PARAMDEC, "ParamDec", yylineno);
          addChild((yyval.node), (yyvsp[-1].node));
          addChild((yyval.node), (yyvsp[0].node));
        }
#line 2062 "./syntax.tab.c"
    break;

  case 33:
#line 442 "./syntax.y"
      {
        // 使用 @1 获取 T_LC 标记的行号信息
        TreeNode* lcNode = createTreeNode(NODE_LC, "LC", (yylsp[-3]).first_line);
        (yyval.node) = createTreeNode(NODE_COMPST, "CompSt", (yylsp[-3]).first_line); // 第T_LC的行号
        TreeNode* rcNode = createTreeNode(NODE_RC, "RC", (yylsp[0]).first_line);
        addChild((yyval.node), lcNode);
        if ((yyvsp[-2].node) != NULL) {
            addChild((yyval.node), (yyvsp[-2].node));
        }
        if ((yyvsp[-1].node) != NULL) {
            addChild((yyval.node), (yyvsp[-1].node));
        }
        addChild((yyval.node), rcNode);
      }
#line 2081 "./syntax.tab.c"
    break;

  case 34:
#line 461 "./syntax.y"
        {
          (yyval.node) = createTreeNode(NODE_STMTLIST, "StmtList", (yyvsp[-1].node)->lineNumber);
          if ((yyvsp[0].node) != NULL) {
              (yyvsp[-1].node)->nextSibling = (yyvsp[0].node);
              addChild((yyval.node),(yyvsp[-1].node));
          } else {
            addChild((yyval.node),(yyvsp[-1].node));
          }
        }
#line 2095 "./syntax.tab.c"
    break;

  case 35:
#line 471 "./syntax.y"
        { (yyval.node) = NULL; }
#line 2101 "./syntax.tab.c"
    break;

  case 36:
#line 473 "./syntax.y"
        {
          has_error = 0;
          (yyval.node) = (yyvsp[0].node);
        }
#line 2110 "./syntax.tab.c"
    break;

  case 37:
#line 478 "./syntax.y"
        {
          has_error = 0;
          (yyval.node) = (yyvsp[0].node);
        }
#line 2119 "./syntax.tab.c"
    break;

  case 38:
#line 486 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_STMT, "Stmt", yylineno);
      addChild((yyval.node), (yyvsp[-1].node));
      TreeNode* semiNode = createTreeNode(NODE_SEMI, "SEMI", yylineno);
      addChild((yyval.node), semiNode);
    }
#line 2130 "./syntax.tab.c"
    break;

  case 39:
#line 493 "./syntax.y"
    {
      has_error = 0;
      (yyval.node) = createTreeNode(NODE_STMT, "Stmt", yylineno);
    }
#line 2139 "./syntax.tab.c"
    break;

  case 40:
#line 498 "./syntax.y"
    {
      has_error = 0;
      (yyval.node) = createTreeNode(NODE_STMT, "Stmt", yylineno);
      TreeNode* semiNode = createTreeNode(NODE_SEMI, "SEMI", yylineno);
      addChild((yyval.node), semiNode);
    }
#line 2150 "./syntax.tab.c"
    break;

  case 41:
#line 505 "./syntax.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 2156 "./syntax.tab.c"
    break;

  case 42:
#line 507 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_STMT, "Stmt", yylineno);
      TreeNode* returnNode = createTreeNode(NODE_RETURN, "RETURN", yylineno);
      TreeNode* semiNode = createTreeNode(NODE_SEMI, "SEMI", yylineno);
      addChild((yyval.node), returnNode);
      addChild((yyval.node), (yyvsp[-1].node));
      addChild((yyval.node), semiNode);
    }
#line 2169 "./syntax.tab.c"
    break;

  case 43:
#line 516 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_STMT, "Stmt", yylineno);
      TreeNode* ifNode = createTreeNode(NODE_IF, "IF", yylineno);
      TreeNode* lpNode = createTreeNode(NODE_LP, "LP", yylineno);
      TreeNode* rpNode = createTreeNode(NODE_RP, "RP", yylineno);
      addChild((yyval.node), ifNode);
      addChild((yyval.node), lpNode);
      addChild((yyval.node), (yyvsp[-2].node));
      addChild((yyval.node), rpNode);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 2185 "./syntax.tab.c"
    break;

  case 44:
#line 528 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_STMT, "Stmt", yylineno);
      TreeNode* ifNode = createTreeNode(NODE_IF, "IF", yylineno);
      TreeNode* lpNode = createTreeNode(NODE_LP, "LP", yylineno);
      TreeNode* rpNode = createTreeNode(NODE_RP, "RP", yylineno);
      TreeNode* elseNode = createTreeNode(NODE_ELSE, "ELSE", yylineno);
      addChild((yyval.node), ifNode);
      addChild((yyval.node), lpNode);
      addChild((yyval.node), (yyvsp[-4].node));
      addChild((yyval.node), rpNode);
      addChild((yyval.node), (yyvsp[-2].node));
      addChild((yyval.node), elseNode);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 2204 "./syntax.tab.c"
    break;

  case 45:
#line 544 "./syntax.y"
    {
      printf("Error type B at Line %d: Empty while statement body.\n", yylineno);
      has_error = 0;
      (yyval.node) = createTreeNode(NODE_STMT, "Stmt", yylineno);
      TreeNode* whileNode = createTreeNode(NODE_WHILE, "WHILE", yylineno);
      TreeNode* lpNode = createTreeNode(NODE_LP, "LP", yylineno);
      TreeNode* rpNode = createTreeNode(NODE_RP, "RP", yylineno);
      TreeNode* semiNode = createTreeNode(NODE_SEMI, "SEMI", yylineno);
      addChild((yyval.node), whileNode);
      addChild((yyval.node), lpNode);
      addChild((yyval.node), (yyvsp[-2].node));
      addChild((yyval.node), rpNode);
      addChild((yyval.node), semiNode);
    }
#line 2223 "./syntax.tab.c"
    break;

  case 46:
#line 559 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_STMT, "Stmt", yylineno);
      TreeNode* whileNode = createTreeNode(NODE_WHILE, "WHILE", yylineno);
      TreeNode* lpNode = createTreeNode(NODE_LP, "LP", yylineno);
      TreeNode* rpNode = createTreeNode(NODE_RP, "RP", yylineno);
      addChild((yyval.node), whileNode);
      addChild((yyval.node), lpNode);
      addChild((yyval.node), (yyvsp[-2].node));
      addChild((yyval.node), rpNode);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 2239 "./syntax.tab.c"
    break;

  case 47:
#line 578 "./syntax.y"
       {
         (yyval.node) = createTreeNode(NODE_DEFLIST, "DefList", (yyvsp[-1].node)->lineNumber);
         addChild((yyval.node), (yyvsp[-1].node));
         addChild((yyval.node), (yyvsp[0].node));
       }
#line 2249 "./syntax.tab.c"
    break;

  case 48:
#line 584 "./syntax.y"
       { (yyval.node) = NULL; }
#line 2255 "./syntax.tab.c"
    break;

  case 49:
#line 589 "./syntax.y"
   {
     (yyval.node) = createTreeNode(NODE_DEF, "Def", yylineno);
     addChild((yyval.node), (yyvsp[-2].node));
     addChild((yyval.node), (yyvsp[-1].node));
     TreeNode* semiNode = createTreeNode(NODE_SEMI, "SEMI", yylineno);
     addChild((yyval.node), semiNode);
   }
#line 2267 "./syntax.tab.c"
    break;

  case 50:
#line 600 "./syntax.y"
       { 
        (yyval.node) = createTreeNode(NODE_DECLIST, "DecList",yylineno);
        addChild((yyval.node),(yyvsp[0].node));
       }
#line 2276 "./syntax.tab.c"
    break;

  case 51:
#line 605 "./syntax.y"
       {
         (yyval.node) = createTreeNode(NODE_DECLIST, "DecList",yylineno);
         TreeNode* commaNode = createTreeNode(NODE_COMMA, "COMMA", yylineno);
         addChild((yyval.node),(yyvsp[-2].node));
         addChild((yyval.node),commaNode);
         addChild((yyval.node),(yyvsp[0].node));
       }
#line 2288 "./syntax.tab.c"
    break;

  case 52:
#line 616 "./syntax.y"
   {
     (yyval.node) = createTreeNode(NODE_DEC, "Dec", (yyvsp[0].node)->lineNumber);
     addChild((yyval.node), (yyvsp[0].node));
   }
#line 2297 "./syntax.tab.c"
    break;

  case 53:
#line 621 "./syntax.y"
   {
     // 检查是否是数组初始化
     int isArray = 0;
     TreeNode* temp = (yyvsp[-2].node);
     while (temp) {
       if (temp->type == NODE_INT) { // varDec 中的数组维度
         isArray = 1;
         break;
       }
       temp = temp->firstChild;
     }

     if (isArray) {
       printf("Error type B at Line %d: Array initialization not supported.\n", yylineno);
       has_error = 1;
       (yyval.node) = createTreeNode(NODE_DEC, "Dec", yylineno);
       addChild((yyval.node), (yyvsp[-2].node));
     } else {
       (yyval.node) = createTreeNode(NODE_DEC, "Dec", yylineno);
       addChild((yyval.node), (yyvsp[-2].node));
       TreeNode* assignopNode = createTreeNode(NODE_ASSIGNOP, "ASSIGNOP", yylineno);
       addChild((yyval.node), assignopNode);
       addChild((yyval.node), (yyvsp[0].node));
     }
   }
#line 2327 "./syntax.tab.c"
    break;

  case 54:
#line 653 "./syntax.y"
    {
      // 检查是否是数组初始化
      int is_array = 0;
      TreeNode* temp = (yyvsp[-2].node);
      while (temp) {
        if (temp->firstChild && temp->firstChild->type == NODE_EXPR && temp->firstChild->firstChild && temp->firstChild->firstChild->type == NODE_INT) {
          is_array = 1; // 找到数组访问表达式
        }
        temp = temp->firstChild;
      }

      // 检查右手边是否是数组初始化 { ... }
      int is_init = 0;
      temp = (yyvsp[0].node);
      while (temp) {
        if (strstr(temp->name, "LC") || strstr(temp->name, "RC")) {
          is_init = 1; // 找到 LC 或 RC，可能是数组初始化
        }
        temp = temp->firstChild;
      }

      if (is_array || is_init) {
        printf("Error type B at Line %d: Array initialization not supported.\n", yylineno);
        (yyval.node) = NULL;
      } else {
        (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
        addChild((yyval.node), (yyvsp[-2].node));
        TreeNode* assignNode = createTreeNode(NODE_ASSIGNOP, "=", yylineno);
        addChild((yyval.node), assignNode);
        addChild((yyval.node), (yyvsp[0].node));
      }
    }
#line 2364 "./syntax.tab.c"
    break;

  case 55:
#line 686 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild((yyval.node), (yyvsp[-2].node));
      TreeNode* andNode = createTreeNode(NODE_AND, "&&", yylineno);
      addChild((yyval.node), andNode);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 2376 "./syntax.tab.c"
    break;

  case 56:
#line 694 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild((yyval.node), (yyvsp[-2].node));
      TreeNode* orNode = createTreeNode(NODE_OR, "||", yylineno);
      addChild((yyval.node), orNode);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 2388 "./syntax.tab.c"
    break;

  case 57:
#line 702 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild((yyval.node), (yyvsp[-2].node));
      TreeNode* relopNode = createTreeNode(NODE_RELOP, yytext, yylineno);
      addChild((yyval.node), relopNode);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 2400 "./syntax.tab.c"
    break;

  case 58:
#line 710 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild((yyval.node), (yyvsp[-2].node));
      TreeNode* plusNode = createTreeNode(NODE_PLUS, "+", yylineno);
      addChild((yyval.node), plusNode);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 2412 "./syntax.tab.c"
    break;

  case 59:
#line 718 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild((yyval.node), (yyvsp[-2].node));
      TreeNode* minusNode = createTreeNode(NODE_MINUS, "-", yylineno);
      addChild((yyval.node), minusNode);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 2424 "./syntax.tab.c"
    break;

  case 60:
#line 726 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild((yyval.node), (yyvsp[-2].node));
      TreeNode* starNode = createTreeNode(NODE_STAR, "*", yylineno);
      addChild((yyval.node), starNode);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 2436 "./syntax.tab.c"
    break;

  case 61:
#line 734 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild((yyval.node), (yyvsp[-2].node));
      TreeNode* divNode = createTreeNode(NODE_DIV, "/", yylineno);
      addChild((yyval.node), divNode);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 2448 "./syntax.tab.c"
    break;

  case 62:
#line 742 "./syntax.y"
    { (yyval.node) = (yyvsp[-1].node); }
#line 2454 "./syntax.tab.c"
    break;

  case 63:
#line 744 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      TreeNode* minusNode = createTreeNode(NODE_MINUS, "-", yylineno);
      addChild((yyval.node), minusNode);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 2465 "./syntax.tab.c"
    break;

  case 64:
#line 751 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      TreeNode* notNode = createTreeNode(NODE_NOT, "!", yylineno);
      addChild((yyval.node), notNode);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 2476 "./syntax.tab.c"
    break;

  case 65:
#line 759 "./syntax.y"
    {
      // 检查数组声明和初始化的匹配性
      // 首先获取数组声明的大小
      int declared_size = -1;
      TreeNode* temp = (yyvsp[-4].node);

      // 找到数组维度
      while (temp) {
        if (temp->type == NODE_VARDEC) {
          // varDec可能包含数组维度
          TreeNode* child = temp->firstChild;
          while (child) {
            if (child->type == NODE_INT) {
              declared_size = child->intVal;
              break;
            }
            child = child->nextSibling;
          }
        }

        temp = temp->firstChild;
      }

      // 计算初始化元素的数量
      int initialized_count = 0;
      temp = (yyvsp[-1].node);
      if (temp) {
        initialized_count = 1;
        while (temp->nextSibling) {
          initialized_count++;
          temp = temp->nextSibling;
        }
      }

      if (declared_size != -1 && declared_size != initialized_count) {
        printf("Error type B at Line %d: Array dimension %d does not match initialization size %d.\n",
               yylineno, declared_size, initialized_count);
      } else {
        printf("Error type B at Line %d: Array initialization not supported.\n", yylineno);
      }

      has_error = 1;
      (yyval.node) = NULL;
    }
#line 2525 "./syntax.tab.c"
    break;

  case 66:
#line 804 "./syntax.y"
    {
      printf("Error type B at Line %d: Array initialization not supported.\n", yylineno);
      has_error = 1;
      (yyval.node) = NULL;
    }
#line 2535 "./syntax.tab.c"
    break;

  case 67:
#line 810 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild((yyval.node), (yyvsp[-3].node));
      TreeNode* lpNode = createTreeNode(NODE_LP, "LP", yylineno);
      TreeNode* rpNode = createTreeNode(NODE_RP, "RP", yylineno);
      addChild((yyval.node), lpNode);
      addChild((yyval.node), (yyvsp[-1].node));
      addChild((yyval.node), rpNode);
    }
#line 2549 "./syntax.tab.c"
    break;

  case 68:
#line 820 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild((yyval.node), (yyvsp[-2].node));
      TreeNode* lpNode = createTreeNode(NODE_LP, "LP", yylineno);
      TreeNode* rpNode = createTreeNode(NODE_RP, "RP", yylineno);
      addChild((yyval.node), lpNode);
      addChild((yyval.node), rpNode);
    }
#line 2562 "./syntax.tab.c"
    break;

  case 69:
#line 829 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild((yyval.node), (yyvsp[-3].node));
      TreeNode* lbNode = createTreeNode(NODE_LB, "LB", yylineno);
      TreeNode* rbNode = createTreeNode(NODE_RB, "RB", yylineno);
      addChild((yyval.node),lbNode);
      addChild((yyval.node), (yyvsp[-1].node));
      addChild((yyval.node),rbNode);
    }
#line 2576 "./syntax.tab.c"
    break;

  case 70:
#line 839 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild((yyval.node), (yyvsp[-2].node));
      TreeNode* dotNode = createTreeNode(NODE_DOT, "DOT", yylineno);
      addChild((yyval.node), dotNode);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 2588 "./syntax.tab.c"
    break;

  case 71:
#line 847 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 2597 "./syntax.tab.c"
    break;

  case 72:
#line 852 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      TreeNode* intNode = createTreeNode(NODE_INT, "INT", yylineno);
      intNode->intVal = yylval.num;
      addChild((yyval.node), intNode);
    }
#line 2608 "./syntax.tab.c"
    break;

  case 73:
#line 859 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      TreeNode* floatNode = createTreeNode(NODE_FLOAT, "FLOAT", yylineno);
      floatNode->floatVal = yylval.real;
      addChild((yyval.node), floatNode);
    }
#line 2619 "./syntax.tab.c"
    break;

  case 74:
#line 866 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
    }
#line 2627 "./syntax.tab.c"
    break;

  case 75:
#line 870 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
    }
#line 2635 "./syntax.tab.c"
    break;

  case 76:
#line 884 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_ARGS, "Args", yylineno);
      addChild((yyval.node), (yyvsp[-2].node));
      TreeNode* commaNode = createTreeNode(NODE_COMMA, "COMMA", yylineno);
      addChild((yyval.node), commaNode);
      addChild((yyval.node),(yyvsp[0].node));
    }
#line 2647 "./syntax.tab.c"
    break;

  case 77:
#line 892 "./syntax.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 2653 "./syntax.tab.c"
    break;

  case 78:
#line 894 "./syntax.y"
    {
      printf("Error type B at Line %d: Trailing comma in function call arguments.\n", yylineno);
      has_error = 1;
      (yyval.node) = (yyvsp[-1].node);
    }
#line 2663 "./syntax.tab.c"
    break;

  case 79:
#line 900 "./syntax.y"
    {
      has_error = 1;
      (yyval.node) = (yyvsp[0].node);
    }
#line 2672 "./syntax.tab.c"
    break;

  case 80:
#line 905 "./syntax.y"
    {
      has_error = 1;
      (yyval.node) = NULL;
    }
#line 2681 "./syntax.tab.c"
    break;


#line 2685 "./syntax.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 911 "./syntax.y"

// Declare the AST root for use in main.c
TreeNode* ast_root;
extern int yylex();
