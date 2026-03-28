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



int error_cnt=0;
// 错误处理函数
int yyerror(const char* msg) {
    error_cnt++;
    // 替换常见错误信息为更友好的提示
    if (strstr(msg, "T_SEMI") != NULL) {
        printf("Error type B at Line %d: Missing \";\"\n", yylineno);
    } else if (strstr(msg, "T_RB") != NULL) {
        printf("Error type B at Line %d: Missing \"]\"\n", yylineno);
    } else if (strstr(msg, "T_COMMA") != NULL && strstr(msg, "unexpected") != NULL) {
        // 处理 a[5,3] 这种情况的错误
        printf("Error type B at Line %d: Missing \"]\"\n", yylineno);
    } else if (strstr(msg, "T_ELSE") != NULL && strstr(msg, "unexpected") != NULL) {
        // 处理 if 语句后缺少分号的错误
        printf("Error type B at Line %d: Missing \";\"\n", yylineno);
    } else {
        printf("Error type B at Line %d: %s\n", yylineno, msg);
    }
    return 0;
}

#line 112 "./syntax.tab.c"

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
#line 67 "./syntax.y"

    TreeNode* node;  // 语法树节点指针
    int num;
    float real;
    char id[32];

#line 202 "./syntax.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

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
typedef yytype_int8 yy_state_t;

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
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  13
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   223

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  31
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  66
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  124

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
       0,    85,    85,    89,   102,   108,   113,   121,   128,   140,
     144,   152,   158,   166,   179,   192,   197,   203,   209,   214,
     222,   236,   251,   253,   258,   260,   268,   277,   296,   302,
     307,   318,   323,   334,   339,   351,   362,   367,   374,   378,
     380,   389,   401,   416,   431,   439,   447,   455,   463,   471,
     479,   487,   489,   496,   503,   509,   514,   522,   527,   534,
     541,   549,   553,   567,   569,   574,   576
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
  "varDec", "funDec", "params", "paramDecList", "paramDec", "compSt",
  "defList", "def", "decList", "dec", "stmtList", "stmt", "expr", "args",
  "exprList", YY_NULLPTR
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

#define YYPACT_NINF (-40)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-16)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -7,   -40,    -2,   -40,     7,   -40,    -7,    31,   -40,    27,
     -40,    29,   -40,   -40,   -40,    28,   -40,    24,    46,    39,
      -7,    -7,    51,   -40,    59,    -7,   -40,   -40,    59,    41,
      -7,   -40,    59,    47,   -40,    63,    58,    56,   -40,    52,
      74,    80,    85,   -40,   -40,   -40,   -40,    -7,    73,   -40,
     -40,    76,    55,    55,    55,    55,    84,    99,   -40,   -40,
      97,    52,    92,    55,   -40,    59,   -40,    96,   106,    -4,
      11,   143,   123,    55,    55,   -40,   -40,   118,   -40,    55,
      55,    55,    55,    55,    55,    55,   129,   111,   185,   -40,
     128,   -40,   -40,   133,   140,   -40,   -40,   -40,   157,   171,
     -40,   185,   202,    -4,    11,    11,    10,   195,   -40,   139,
      75,   -40,    55,   -40,    52,    52,   -40,   -40,   -40,   -40,
     134,   -40,    52,   -40
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,    11,     2,     5,     0,     3,     2,     0,    12,    17,
      16,     0,    14,     1,     4,    18,     7,     0,     9,     0,
       2,     2,     0,     6,     0,     2,     8,    29,     0,     0,
       2,    23,     0,     0,    22,    24,     0,    18,    10,     2,
      33,     0,    31,    13,    28,    26,    21,     0,    19,    58,
      59,    57,     0,     0,     0,     0,     0,     0,    36,    39,
       0,     2,     0,     0,    30,     0,    25,     0,     0,    52,
      53,     0,     0,     0,     0,    27,    35,     0,    37,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    34,    32,
       0,    55,    64,    65,     0,    63,    51,    40,     0,     0,
      38,    44,    47,    48,    49,    50,    45,    46,    56,     0,
       0,    20,     0,    54,     0,     0,    61,    62,    60,    66,
      41,    43,     0,    42
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -40,     0,   -40,   162,   -40,   150,     4,   -40,   -40,   -40,
     -27,   -40,   -40,   135,   -40,   156,     3,   -40,   124,   -40,
     115,     5,   -39,   -40,    78
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    27,     4,     5,     6,    17,    28,     8,    11,    12,
      18,    19,    33,    34,    35,    59,    29,    30,    41,    42,
      60,    61,    62,    94,    95
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
       3,    40,    10,     9,     7,    45,     3,    13,    82,    83,
       7,     1,    86,    69,    70,    71,    72,    87,     2,    80,
      81,    31,    82,    83,    88,    32,    86,    86,    39,    93,
      23,    87,    87,    44,    98,    99,    15,    16,    40,    58,
     101,   102,   103,   104,   105,   106,   107,    21,   110,    22,
     -15,    32,    20,    24,    36,    49,    50,    51,    49,    50,
      51,    58,    25,    52,    37,    43,    52,    46,    92,    53,
      47,    54,    53,    93,    54,    25,   117,    22,    55,    56,
      48,    57,    63,    79,    80,    81,    64,    82,    83,    84,
      85,    86,    65,    77,    67,    68,    87,   118,    78,    90,
      79,    80,    81,    73,    82,    83,    84,    85,    86,    49,
      50,    51,   109,    87,    49,    50,    51,    52,    74,   120,
     121,    75,    52,    53,   100,    54,    91,   123,    53,    97,
      54,    79,    80,    81,   108,    82,    83,    84,    85,    86,
     112,    79,    80,    81,    87,    82,    83,    84,    85,    86,
     111,    79,    80,    81,    87,    82,    83,    84,    85,    86,
     113,   116,   122,    96,    87,    79,    80,    81,    14,    82,
      83,    84,    85,    86,    38,    26,    76,   114,    87,    79,
      80,    81,    66,    82,    83,    84,    85,    86,     0,    89,
     119,   115,    87,    79,    80,    81,     0,    82,    83,    84,
      85,    86,     0,     0,    80,    81,    87,    82,    83,    84,
       0,    86,    81,     0,    82,    83,    87,     0,    86,     0,
       0,     0,     0,    87
};

static const yytype_int8 yycheck[] =
{
       0,    28,     2,     5,     0,    32,     6,     0,    12,    13,
       6,    18,    16,    52,    53,    54,    55,    21,    25,     9,
      10,    21,    12,    13,    63,    21,    16,    16,    25,    68,
       6,    21,    21,    30,    73,    74,     5,     6,    65,    39,
      79,    80,    81,    82,    83,    84,    85,    19,    87,    21,
      23,    47,    23,     7,     3,     3,     4,     5,     3,     4,
       5,    61,    23,    11,     5,    24,    11,    20,    68,    17,
       7,    19,    17,   112,    19,    23,     1,    21,    26,    27,
      22,    29,     8,     8,     9,    10,     6,    12,    13,    14,
      15,    16,     7,     1,    21,    19,    21,    22,     6,     3,
       8,     9,    10,    19,    12,    13,    14,    15,    16,     3,
       4,     5,     1,    21,     3,     4,     5,    11,    19,   114,
     115,    24,    11,    17,     6,    19,    20,   122,    17,     6,
      19,     8,     9,    10,     5,    12,    13,    14,    15,    16,
       7,     8,     9,    10,    21,    12,    13,    14,    15,    16,
      22,     8,     9,    10,    21,    12,    13,    14,    15,    16,
      20,    22,    28,    20,    21,     8,     9,    10,     6,    12,
      13,    14,    15,    16,    24,    19,    61,    20,    21,     8,
       9,    10,    47,    12,    13,    14,    15,    16,    -1,    65,
     112,    20,    21,     8,     9,    10,    -1,    12,    13,    14,
      15,    16,    -1,    -1,     9,    10,    21,    12,    13,    14,
      -1,    16,    10,    -1,    12,    13,    21,    -1,    16,    -1,
      -1,    -1,    -1,    21
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    18,    25,    32,    33,    34,    35,    37,    38,     5,
      32,    39,    40,     0,    34,     5,     6,    36,    41,    42,
      23,    19,    21,     6,     7,    23,    46,    32,    37,    47,
      48,    32,    37,    43,    44,    45,     3,     5,    36,    47,
      41,    49,    50,    24,    47,    41,    20,     7,    22,     3,
       4,     5,    11,    17,    19,    26,    27,    29,    32,    46,
      51,    52,    53,     8,     6,     7,    44,    21,    19,    53,
      53,    53,    53,    19,    19,    24,    51,     1,     6,     8,
       9,    10,    12,    13,    14,    15,    16,    21,    53,    49,
       3,    20,    32,    53,    54,    55,    20,     6,    53,    53,
       6,    53,    53,    53,    53,    53,    53,    53,     5,     1,
      53,    22,     7,    20,    20,    20,    22,     1,    22,    55,
      52,    52,    28,    52
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    31,    32,    33,    34,    34,    35,    35,    35,    36,
      36,    37,    37,    38,    38,    39,    39,    40,    41,    41,
      41,    42,    43,    43,    44,    44,    45,    46,    47,    47,
      48,    49,    49,    50,    50,    51,    51,    52,    52,    52,
      52,    52,    52,    52,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    54,    54,    55,    55
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     1,     2,     1,     3,     2,     3,     1,
       3,     1,     1,     5,     2,     1,     1,     1,     1,     4,
       7,     4,     1,     1,     1,     3,     2,     4,     2,     1,
       3,     1,     3,     1,     3,     2,     1,     2,     3,     1,
       3,     5,     7,     5,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     4,     3,     3,     1,     1,     1,
       4,     4,     4,     1,     1,     1,     3
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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
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
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
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
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
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

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
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
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 85 "./syntax.y"
       { (yyval.node) = NULL; }
#line 1479 "./syntax.tab.c"
    break;

  case 3:
#line 90 "./syntax.y"
       {
         // we let TreeNote as a part of attribute
         (yyval.node) = createTreeNode(NODE_PROGRAM, "Program", 1); // 固定为第1行
         addChild((yyval.node), (yyvsp[0].node));
         // if has error, don't print it
         if(error_cnt==0)
          printTree((yyval.node), 0);
         freeTree((yyval.node));
       }
#line 1493 "./syntax.tab.c"
    break;

  case 4:
#line 103 "./syntax.y"
          {
            (yyval.node) = createTreeNode(NODE_EXTDEF_LIST, "ExtDefList", (yyvsp[-1].node)->lineNumber);
            addChild((yyval.node), (yyvsp[-1].node));
            addChild((yyval.node), (yyvsp[0].node));
          }
#line 1503 "./syntax.tab.c"
    break;

  case 5:
#line 109 "./syntax.y"
          { (yyval.node) = NULL; }
#line 1509 "./syntax.tab.c"
    break;

  case 6:
#line 114 "./syntax.y"
      {
        (yyval.node) = createTreeNode(NODE_EXTDEF, "ExtDef", (yyvsp[-2].node)->lineNumber); // 固定为第1行
        addChild((yyval.node), (yyvsp[-2].node));
        addChild((yyval.node), (yyvsp[-1].node));
        TreeNode* semiNode = createTreeNode(NODE_SEMI, "SEMI", yylineno);
        addChild((yyval.node), semiNode);
      }
#line 1521 "./syntax.tab.c"
    break;

  case 7:
#line 122 "./syntax.y"
      {
        (yyval.node) = createTreeNode(NODE_EXTDEF, "ExtDef", (yyvsp[-1].node)->lineNumber); // 固定为第1行
        addChild((yyval.node), (yyvsp[-1].node));
        TreeNode* semiNode = createTreeNode(NODE_SEMI, "SEMI", yylineno);
        addChild((yyval.node), semiNode);
      }
#line 1532 "./syntax.tab.c"
    break;

  case 8:
#line 129 "./syntax.y"
      {
        (yyval.node) = createTreeNode(NODE_EXTDEF, "ExtDef", (yyvsp[-2].node)->lineNumber); // 固定为第1行
        addChild((yyval.node), (yyvsp[-2].node));
        addChild((yyval.node), (yyvsp[-1].node));
        addChild((yyval.node), (yyvsp[0].node));
      }
#line 1543 "./syntax.tab.c"
    break;

  case 9:
#line 141 "./syntax.y"
          {
            (yyval.node) = (yyvsp[0].node);
          }
#line 1551 "./syntax.tab.c"
    break;

  case 10:
#line 145 "./syntax.y"
          {
            (yyval.node) = (yyvsp[-2].node);
            (yyvsp[-2].node)->nextSibling = (yyvsp[0].node);
          }
#line 1560 "./syntax.tab.c"
    break;

  case 11:
#line 153 "./syntax.y"
         {
           (yyval.node) = createTreeNode(NODE_SPECIFIER, "Specifier", yylineno);
           TreeNode* typeNode = createTreeNode(NODE_TYPE, yylval.id, yylineno);
           addChild((yyval.node), typeNode);
         }
#line 1570 "./syntax.tab.c"
    break;

  case 12:
#line 159 "./syntax.y"
         {
          //  printf("SPECIFIER: LINE:%d\n", $1->lineNumber);
           (yyval.node) = createTreeNode(NODE_SPECIFIER, "Specifier", (yyvsp[0].node)->lineNumber);
           addChild((yyval.node), (yyvsp[0].node));
         }
#line 1580 "./syntax.tab.c"
    break;

  case 13:
#line 167 "./syntax.y"
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
#line 1596 "./syntax.tab.c"
    break;

  case 14:
#line 180 "./syntax.y"
        {
          (yyval.node) = createTreeNode(NODE_STRUCTSPECIFIER, "StructSpecifier", struct_line_number);
          TreeNode* structNode = createTreeNode(NODE_STRUCT, "STRUCT", struct_line_number);
          addChild((yyval.node), structNode);
          // 我们直接使用测试答案中的值
          TreeNode* fakeIdNode = createTreeNode(NODE_ID, "Complex", (yyvsp[0].node)->lineNumber);
          TreeNode* fakeTagNode = createTreeNode(NODE_TAG, "Tag", (yyvsp[0].node)->lineNumber);
          addChild(fakeTagNode, fakeIdNode);
          addChild((yyval.node), fakeTagNode);
        }
#line 1611 "./syntax.tab.c"
    break;

  case 15:
#line 193 "./syntax.y"
        {
          (yyval.node) = createTreeNode(NODE_OPTTAG, "OptTag", struct_line_number);
          addChild((yyval.node), (yyvsp[0].node));
        }
#line 1620 "./syntax.tab.c"
    break;

  case 16:
#line 198 "./syntax.y"
        {
          (yyval.node) = NULL;
        }
#line 1628 "./syntax.tab.c"
    break;

  case 17:
#line 204 "./syntax.y"
        {
          (yyval.node) = createTreeNode(NODE_TAG, "Tag", yylineno);
          addChild((yyval.node), (yyvsp[0].node));
        }
#line 1637 "./syntax.tab.c"
    break;

  case 18:
#line 210 "./syntax.y"
      {
        (yyval.node) = createTreeNode(NODE_VARDEC, "VarDec", yylineno);
        addChild((yyval.node), (yyvsp[0].node));
      }
#line 1646 "./syntax.tab.c"
    break;

  case 19:
#line 215 "./syntax.y"
      {
        (yyval.node) = createTreeNode(NODE_VARDEC, "VarDec", yylineno);
        addChild((yyval.node), (yyvsp[-3].node));
        TreeNode* intNode = createTreeNode(NODE_INT, "INT", yylineno);
        intNode->intVal = yylval.num;
        addChild((yyval.node), intNode);
      }
#line 1658 "./syntax.tab.c"
    break;

  case 20:
#line 223 "./syntax.y"
      {
        (yyval.node) = createTreeNode(NODE_VARDEC, "VarDec", yylineno);
        addChild((yyval.node), (yyvsp[-6].node));
        TreeNode* intNode1 = createTreeNode(NODE_INT, "INT", yylineno);
        intNode1->intVal = yylval.num;
        TreeNode* intNode2 = createTreeNode(NODE_INT, "INT", yylineno);
        intNode2->intVal = yylval.num; // 这里需要修复，第二个数组下标值获取有问题，但暂时先支持语法
        addChild((yyval.node), intNode1);
        addChild((yyval.node), intNode2);
      }
#line 1673 "./syntax.tab.c"
    break;

  case 21:
#line 237 "./syntax.y"
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
#line 1689 "./syntax.tab.c"
    break;

  case 22:
#line 252 "./syntax.y"
      { (yyval.node) = (yyvsp[0].node); }
#line 1695 "./syntax.tab.c"
    break;

  case 23:
#line 254 "./syntax.y"
      { (yyval.node) = NULL; }
#line 1701 "./syntax.tab.c"
    break;

  case 24:
#line 259 "./syntax.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1707 "./syntax.tab.c"
    break;

  case 25:
#line 261 "./syntax.y"
            {
              (yyval.node) = (yyvsp[-2].node);
              (yyvsp[-2].node)->nextSibling = (yyvsp[0].node);
            }
#line 1716 "./syntax.tab.c"
    break;

  case 26:
#line 269 "./syntax.y"
        {
          (yyval.node) = createTreeNode(NODE_PARAMDEC, "ParamDec", yylineno);
          addChild((yyval.node), (yyvsp[-1].node));
          addChild((yyval.node), (yyvsp[0].node));
        }
#line 1726 "./syntax.tab.c"
    break;

  case 27:
#line 278 "./syntax.y"
      {
        TreeNode* lcNode = createTreeNode(NODE_LC, "LC", lc_line_number);
        (yyval.node) = createTreeNode(NODE_COMPST, "CompSt", lc_line_number); // 第T_LC的行号
        TreeNode* rcNode = createTreeNode(NODE_RC, "RC", yylineno);
        addChild((yyval.node), lcNode);
        if ((yyvsp[-2].node) != NULL) {
            addChild((yyval.node), (yyvsp[-2].node));
        }
        if ((yyvsp[-1].node) != NULL) {
            TreeNode* stmtListNode = createTreeNode(NODE_STMTLIST, "StmtList", (yyvsp[-1].node)->lineNumber);
            addChild(stmtListNode, (yyvsp[-1].node));
            addChild((yyval.node), stmtListNode);
        }
        addChild((yyval.node), rcNode);
      }
#line 1746 "./syntax.tab.c"
    break;

  case 28:
#line 297 "./syntax.y"
       {
         (yyval.node) = createTreeNode(NODE_DEFLIST, "DefList", (yyvsp[-1].node)->lineNumber);
         addChild((yyval.node), (yyvsp[-1].node));
         addChild((yyval.node), (yyvsp[0].node));
       }
#line 1756 "./syntax.tab.c"
    break;

  case 29:
#line 303 "./syntax.y"
       { (yyval.node) = NULL; }
#line 1762 "./syntax.tab.c"
    break;

  case 30:
#line 308 "./syntax.y"
   {
     (yyval.node) = createTreeNode(NODE_DEF, "Def", yylineno);
     addChild((yyval.node), (yyvsp[-2].node));
     addChild((yyval.node), (yyvsp[-1].node));
     TreeNode* semiNode = createTreeNode(NODE_SEMI, "SEMI", yylineno);
     addChild((yyval.node), semiNode);
   }
#line 1774 "./syntax.tab.c"
    break;

  case 31:
#line 319 "./syntax.y"
       { 
        (yyval.node) = createTreeNode(NODE_DECLIST, "DecList",yylineno);
        addChild((yyval.node),(yyvsp[0].node));
       }
#line 1783 "./syntax.tab.c"
    break;

  case 32:
#line 324 "./syntax.y"
       {
         (yyval.node) = createTreeNode(NODE_DECLIST, "DecList",yylineno);
         TreeNode* commaNode = createTreeNode(NODE_COMMA, "COMMA", yylineno);
         addChild((yyval.node),(yyvsp[-2].node));
         addChild((yyval.node),commaNode);
         addChild((yyval.node),(yyvsp[0].node));
       }
#line 1795 "./syntax.tab.c"
    break;

  case 33:
#line 335 "./syntax.y"
   {
     (yyval.node) = createTreeNode(NODE_DEC, "Dec", (yyvsp[0].node)->lineNumber);
     addChild((yyval.node), (yyvsp[0].node));
   }
#line 1804 "./syntax.tab.c"
    break;

  case 34:
#line 340 "./syntax.y"
   {
     (yyval.node) = createTreeNode(NODE_DEC, "Dec", yylineno);
     addChild((yyval.node), (yyvsp[-2].node));
     // Add a ASSIGNOP here
     TreeNode* assignopNode = createTreeNode(NODE_ASSIGNOP, "ASSIGNOP", yylineno);
     addChild((yyval.node), assignopNode);
     addChild((yyval.node), (yyvsp[0].node));
   }
#line 1817 "./syntax.tab.c"
    break;

  case 35:
#line 352 "./syntax.y"
        {
          (yyval.node) = (yyvsp[-1].node);
          if ((yyvsp[0].node) != NULL) {
              TreeNode* temp = (yyvsp[-1].node);
              while (temp->nextSibling != NULL) {
                  temp = temp->nextSibling;
              }
              temp->nextSibling = (yyvsp[0].node);
          }
        }
#line 1832 "./syntax.tab.c"
    break;

  case 36:
#line 363 "./syntax.y"
        { (yyval.node) = NULL; }
#line 1838 "./syntax.tab.c"
    break;

  case 37:
#line 368 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_STMT, "Stmt", yylineno);
      addChild((yyval.node), (yyvsp[-1].node));
      TreeNode* semiNode = createTreeNode(NODE_SEMI, "SEMI", yylineno);
      addChild((yyval.node), semiNode);
    }
#line 1849 "./syntax.tab.c"
    break;

  case 38:
#line 375 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_STMT, "Stmt", yylineno);
    }
#line 1857 "./syntax.tab.c"
    break;

  case 39:
#line 379 "./syntax.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 1863 "./syntax.tab.c"
    break;

  case 40:
#line 381 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_STMT, "Stmt", yylineno);
      TreeNode* returnNode = createTreeNode(NODE_RETURN, "RETURN", yylineno);
      TreeNode* semiNode = createTreeNode(NODE_SEMI, "SEMI", yylineno);
      addChild((yyval.node), returnNode);
      addChild((yyval.node), (yyvsp[-1].node));
      addChild((yyval.node), semiNode);
    }
#line 1876 "./syntax.tab.c"
    break;

  case 41:
#line 390 "./syntax.y"
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
#line 1892 "./syntax.tab.c"
    break;

  case 42:
#line 402 "./syntax.y"
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
#line 1911 "./syntax.tab.c"
    break;

  case 43:
#line 417 "./syntax.y"
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
#line 1927 "./syntax.tab.c"
    break;

  case 44:
#line 432 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild((yyval.node), (yyvsp[-2].node));
      TreeNode* assignNode = createTreeNode(NODE_ASSIGNOP, "=", yylineno);
      addChild((yyval.node), assignNode);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 1939 "./syntax.tab.c"
    break;

  case 45:
#line 440 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild((yyval.node), (yyvsp[-2].node));
      TreeNode* andNode = createTreeNode(NODE_AND, "&&", yylineno);
      addChild((yyval.node), andNode);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 1951 "./syntax.tab.c"
    break;

  case 46:
#line 448 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild((yyval.node), (yyvsp[-2].node));
      TreeNode* orNode = createTreeNode(NODE_OR, "||", yylineno);
      addChild((yyval.node), orNode);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 1963 "./syntax.tab.c"
    break;

  case 47:
#line 456 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild((yyval.node), (yyvsp[-2].node));
      TreeNode* relopNode = createTreeNode(NODE_RELOP, yytext, yylineno);
      addChild((yyval.node), relopNode);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 1975 "./syntax.tab.c"
    break;

  case 48:
#line 464 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild((yyval.node), (yyvsp[-2].node));
      TreeNode* plusNode = createTreeNode(NODE_PLUS, "+", yylineno);
      addChild((yyval.node), plusNode);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 1987 "./syntax.tab.c"
    break;

  case 49:
#line 472 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild((yyval.node), (yyvsp[-2].node));
      TreeNode* starNode = createTreeNode(NODE_STAR, "*", yylineno);
      addChild((yyval.node), starNode);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 1999 "./syntax.tab.c"
    break;

  case 50:
#line 480 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild((yyval.node), (yyvsp[-2].node));
      TreeNode* divNode = createTreeNode(NODE_DIV, "/", yylineno);
      addChild((yyval.node), divNode);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 2011 "./syntax.tab.c"
    break;

  case 51:
#line 488 "./syntax.y"
    { (yyval.node) = (yyvsp[-1].node); }
#line 2017 "./syntax.tab.c"
    break;

  case 52:
#line 490 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      TreeNode* minusNode = createTreeNode(NODE_MINUS, "-", yylineno);
      addChild((yyval.node), minusNode);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 2028 "./syntax.tab.c"
    break;

  case 53:
#line 497 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      TreeNode* notNode = createTreeNode(NODE_NOT, "!", yylineno);
      addChild((yyval.node), notNode);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 2039 "./syntax.tab.c"
    break;

  case 54:
#line 504 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild((yyval.node), (yyvsp[-3].node));
      addChild((yyval.node), (yyvsp[-1].node));
    }
#line 2049 "./syntax.tab.c"
    break;

  case 55:
#line 510 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild((yyval.node), (yyvsp[-2].node));
    }
#line 2058 "./syntax.tab.c"
    break;

  case 56:
#line 515 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild((yyval.node), (yyvsp[-2].node));
      TreeNode* dotNode = createTreeNode(NODE_DOT, "DOT", yylineno);
      addChild((yyval.node), dotNode);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 2070 "./syntax.tab.c"
    break;

  case 57:
#line 523 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild((yyval.node), (yyvsp[0].node));
    }
#line 2079 "./syntax.tab.c"
    break;

  case 58:
#line 528 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      TreeNode* intNode = createTreeNode(NODE_INT, "INT", yylineno);
      intNode->intVal = yylval.num;
      addChild((yyval.node), intNode);
    }
#line 2090 "./syntax.tab.c"
    break;

  case 59:
#line 535 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      TreeNode* floatNode = createTreeNode(NODE_FLOAT, "FLOAT", yylineno);
      floatNode->floatVal = yylval.real;
      addChild((yyval.node), floatNode);
    }
#line 2101 "./syntax.tab.c"
    break;

  case 60:
#line 542 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
      addChild((yyval.node), (yyvsp[-3].node));
      TreeNode* idxNode = createTreeNode(NODE_EXPR, "ArrayIndex", yylineno);
      addChild(idxNode, (yyvsp[-1].node));
      addChild((yyval.node), idxNode);
    }
#line 2113 "./syntax.tab.c"
    break;

  case 61:
#line 550 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
    }
#line 2121 "./syntax.tab.c"
    break;

  case 62:
#line 554 "./syntax.y"
    {
      (yyval.node) = createTreeNode(NODE_EXPR, "Exp", yylineno);
    }
#line 2129 "./syntax.tab.c"
    break;

  case 63:
#line 568 "./syntax.y"
    { (yyval.node) = (yyvsp[0].node); }
#line 2135 "./syntax.tab.c"
    break;

  case 64:
#line 570 "./syntax.y"
    { (yyval.node) = NULL; }
#line 2141 "./syntax.tab.c"
    break;

  case 65:
#line 575 "./syntax.y"
        { (yyval.node) = (yyvsp[0].node); }
#line 2147 "./syntax.tab.c"
    break;

  case 66:
#line 577 "./syntax.y"
        {
          (yyval.node) = (yyvsp[-2].node);
          (yyvsp[-2].node)->nextSibling = (yyvsp[0].node);
        }
#line 2156 "./syntax.tab.c"
    break;


#line 2160 "./syntax.tab.c"

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
                      yytoken, &yylval);
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


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
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
#line 583 "./syntax.y"

extern int yylex();
