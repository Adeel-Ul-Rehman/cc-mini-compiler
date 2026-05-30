/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "ast.h"
#include "symbol_table.h"

extern int yylex();
extern int yylineno;
void yyerror(const char *s);

ASTNode *program_root;
extern SymbolTable *symtab;

int semantic_errors = 0;

const char* get_type_name(ASTNode *type) {
    if (!type || type->type != NODE_TYPE) return "unknown";
    return type->data.sval;
}

void semantic_error(const char *msg) {
    fprintf(stderr, "Semantic Error at line %d: %s\n", yylineno, msg);
    semantic_errors = 1;
}

// Helper to get type from a factor node (literal or variable)
const char* get_factor_type(ASTNode *node) {
    if (!node) return "unknown";
    switch (node->type) {
        case NODE_INT_LIT: return "int";
        case NODE_FLOAT_LIT: return "float";
        case NODE_BOOL_LIT: return "int";
        case NODE_STRING_LIT: return "string";
        case NODE_VAR: {
            char *name = node->data.sval;
            Symbol *sym = lookup_symbol(symtab, name);
            if (sym && sym->type) return get_type_name(sym->type);
            return "unknown";
        }
        default: return "unknown";
    }
}

#line 116 "parser.c"

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

#include "parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TYPE_VOID = 3,                  /* TYPE_VOID  */
  YYSYMBOL_TYPE_INT = 4,                   /* TYPE_INT  */
  YYSYMBOL_TYPE_FLOAT = 5,                 /* TYPE_FLOAT  */
  YYSYMBOL_TYPE_BOOL = 6,                  /* TYPE_BOOL  */
  YYSYMBOL_IF = 7,                         /* IF  */
  YYSYMBOL_ELSE = 8,                       /* ELSE  */
  YYSYMBOL_WHILE = 9,                      /* WHILE  */
  YYSYMBOL_FOR = 10,                       /* FOR  */
  YYSYMBOL_RETURN = 11,                    /* RETURN  */
  YYSYMBOL_INPUT = 12,                     /* INPUT  */
  YYSYMBOL_OUTPUT = 13,                    /* OUTPUT  */
  YYSYMBOL_EQ = 14,                        /* EQ  */
  YYSYMBOL_NE = 15,                        /* NE  */
  YYSYMBOL_LE = 16,                        /* LE  */
  YYSYMBOL_GE = 17,                        /* GE  */
  YYSYMBOL_AND = 18,                       /* AND  */
  YYSYMBOL_OR = 19,                        /* OR  */
  YYSYMBOL_ASSIGN = 20,                    /* ASSIGN  */
  YYSYMBOL_PLUS = 21,                      /* PLUS  */
  YYSYMBOL_MINUS = 22,                     /* MINUS  */
  YYSYMBOL_STAR = 23,                      /* STAR  */
  YYSYMBOL_SLASH = 24,                     /* SLASH  */
  YYSYMBOL_LT = 25,                        /* LT  */
  YYSYMBOL_GT = 26,                        /* GT  */
  YYSYMBOL_LPAREN = 27,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 28,                    /* RPAREN  */
  YYSYMBOL_LBRACE = 29,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 30,                    /* RBRACE  */
  YYSYMBOL_SEMICOLON = 31,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 32,                     /* COMMA  */
  YYSYMBOL_INT_LIT = 33,                   /* INT_LIT  */
  YYSYMBOL_BOOL_LIT = 34,                  /* BOOL_LIT  */
  YYSYMBOL_FLOAT_LIT = 35,                 /* FLOAT_LIT  */
  YYSYMBOL_IDENTIFIER = 36,                /* IDENTIFIER  */
  YYSYMBOL_STRING_LIT = 37,                /* STRING_LIT  */
  YYSYMBOL_YYACCEPT = 38,                  /* $accept  */
  YYSYMBOL_program = 39,                   /* program  */
  YYSYMBOL_top_list = 40,                  /* top_list  */
  YYSYMBOL_top = 41,                       /* top  */
  YYSYMBOL_decl = 42,                      /* decl  */
  YYSYMBOL_function = 43,                  /* function  */
  YYSYMBOL_param_opt = 44,                 /* param_opt  */
  YYSYMBOL_param_list = 45,                /* param_list  */
  YYSYMBOL_block = 46,                     /* block  */
  YYSYMBOL_47_1 = 47,                      /* $@1  */
  YYSYMBOL_block_item_list = 48,           /* block_item_list  */
  YYSYMBOL_block_item = 49,                /* block_item  */
  YYSYMBOL_stmt = 50,                      /* stmt  */
  YYSYMBOL_assign_stmt = 51,               /* assign_stmt  */
  YYSYMBOL_if_stmt = 52,                   /* if_stmt  */
  YYSYMBOL_while_stmt = 53,                /* while_stmt  */
  YYSYMBOL_for_stmt = 54,                  /* for_stmt  */
  YYSYMBOL_return_stmt = 55,               /* return_stmt  */
  YYSYMBOL_input_stmt = 56,                /* input_stmt  */
  YYSYMBOL_output_stmt = 57,               /* output_stmt  */
  YYSYMBOL_expr = 58,                      /* expr  */
  YYSYMBOL_logical_or = 59,                /* logical_or  */
  YYSYMBOL_logical_and = 60,               /* logical_and  */
  YYSYMBOL_equality = 61,                  /* equality  */
  YYSYMBOL_relational = 62,                /* relational  */
  YYSYMBOL_additive = 63,                  /* additive  */
  YYSYMBOL_multiplicative = 64,            /* multiplicative  */
  YYSYMBOL_factor = 65,                    /* factor  */
  YYSYMBOL_type_spec = 66                  /* type_spec  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




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

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
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
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   111

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  38
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  64
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  121

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   292


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    82,    82,    86,    87,    91,    92,    96,   104,   135,
     146,   147,   151,   155,   162,   162,   166,   167,   171,   172,
     181,   182,   183,   184,   185,   186,   187,   188,   192,   223,
     224,   228,   232,   237,   241,   253,   257,   261,   262,   266,
     267,   271,   272,   273,   277,   278,   279,   280,   281,   285,
     286,   287,   291,   292,   293,   297,   298,   299,   300,   312,
     313,   317,   318,   319,   320
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TYPE_VOID",
  "TYPE_INT", "TYPE_FLOAT", "TYPE_BOOL", "IF", "ELSE", "WHILE", "FOR",
  "RETURN", "INPUT", "OUTPUT", "EQ", "NE", "LE", "GE", "AND", "OR",
  "ASSIGN", "PLUS", "MINUS", "STAR", "SLASH", "LT", "GT", "LPAREN",
  "RPAREN", "LBRACE", "RBRACE", "SEMICOLON", "COMMA", "INT_LIT",
  "BOOL_LIT", "FLOAT_LIT", "IDENTIFIER", "STRING_LIT", "$accept",
  "program", "top_list", "top", "decl", "function", "param_opt",
  "param_list", "block", "$@1", "block_item_list", "block_item", "stmt",
  "assign_stmt", "if_stmt", "while_stmt", "for_stmt", "return_stmt",
  "input_stmt", "output_stmt", "expr", "logical_or", "logical_and",
  "equality", "relational", "additive", "multiplicative", "factor",
  "type_spec", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-90)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -90,    16,    59,   -90,   -90,   -90,   -90,   -90,   -90,   -90,
     -90,   -28,    14,    13,    59,   -90,    13,   -90,   -90,   -90,
     -90,   -90,    -9,     7,    10,     9,    35,    32,    34,   -90,
       5,     6,    23,    44,   -90,    13,    13,    13,    13,    13,
      13,    13,    13,    13,    13,    13,    13,    48,    59,   -90,
     -90,    10,     9,    35,    35,    32,    32,    32,    32,    34,
      34,   -90,   -90,   -90,   -90,    46,   -90,   -90,     0,    15,
      56,    57,    13,    58,    60,   -90,    66,   -90,   -90,   -90,
     -90,    61,   -90,   -90,   -90,    63,    64,    65,    52,    13,
      13,    53,   -90,    54,    13,    13,   -90,   -90,   -90,   -90,
      -6,    69,    70,    68,    72,    73,   -90,     8,     8,    13,
     -90,   -90,    83,   -90,    71,     8,    53,   -90,    75,     8,
     -90
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     2,     1,    61,    62,    63,    64,     4,     5,
       6,     0,     0,     0,    10,     7,     0,    55,    57,    56,
      58,    59,     0,    36,    37,    39,    41,    44,    49,    52,
       0,    11,     0,     0,     8,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    12,
      60,    38,    40,    42,    43,    47,    48,    45,    46,    50,
      51,    53,    54,    14,     9,     0,    16,    13,     0,     0,
       0,     0,     0,     0,     0,    15,     0,    18,    27,    17,
      19,     0,    21,    22,    23,     0,     0,     0,     0,     0,
       0,     0,    33,     0,     0,     0,    20,    24,    25,    26,
       0,     0,     0,     0,     0,     0,    28,     0,     0,     0,
      34,    35,    30,    31,     0,     0,     0,    29,     0,     0,
      32
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -90,   -90,   -90,   -90,    36,   -90,   -90,   -90,    62,   -90,
     -90,   -90,   -76,   -89,   -90,   -90,   -90,   -90,   -90,   -90,
     -16,   -90,    76,    74,    33,    27,    37,    30,   -13
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,     8,     9,    10,    30,    31,    78,    66,
      68,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      22,    23,    24,    25,    26,    27,    28,    29,    11
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      33,    32,   103,     4,     5,     6,     7,    69,    12,    70,
      71,    72,    73,    74,    13,    69,     3,    70,    71,    72,
      73,    74,    34,    37,    38,    15,    35,   118,    36,    63,
      75,   112,   113,    47,    13,    65,    76,    63,    48,   117,
      16,    14,    89,   120,    76,    15,    17,    18,    19,    20,
      21,    39,    40,    43,    44,    88,    92,    45,    46,    49,
      41,    42,     4,     5,     6,     7,    55,    56,    57,    58,
      53,    54,    50,   101,   102,    61,    62,    63,   105,   106,
      59,    60,    67,    90,    91,    93,    95,    94,   100,    76,
     104,   115,    96,   114,    97,    98,    99,   107,   108,   109,
     110,   111,   116,   119,    77,     0,     0,     0,     0,    64,
      52,    51
};

static const yytype_int8 yycheck[] =
{
      16,    14,    91,     3,     4,     5,     6,     7,    36,     9,
      10,    11,    12,    13,    20,     7,     0,     9,    10,    11,
      12,    13,    31,    14,    15,    31,    19,   116,    18,    29,
      30,   107,   108,    28,    20,    48,    36,    29,    32,   115,
      27,    27,    27,   119,    36,    31,    33,    34,    35,    36,
      37,    16,    17,    21,    22,    68,    72,    23,    24,    36,
      25,    26,     3,     4,     5,     6,    39,    40,    41,    42,
      37,    38,    28,    89,    90,    45,    46,    29,    94,    95,
      43,    44,    36,    27,    27,    27,    20,    27,    36,    36,
      36,     8,    31,   109,    31,    31,    31,    28,    28,    31,
      28,    28,    31,    28,    68,    -1,    -1,    -1,    -1,    47,
      36,    35
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    39,    40,     0,     3,     4,     5,     6,    41,    42,
      43,    66,    36,    20,    27,    31,    27,    33,    34,    35,
      36,    37,    58,    59,    60,    61,    62,    63,    64,    65,
      44,    45,    66,    58,    31,    19,    18,    14,    15,    16,
      17,    25,    26,    21,    22,    23,    24,    28,    32,    36,
      28,    60,    61,    62,    62,    63,    63,    63,    63,    64,
      64,    65,    65,    29,    46,    66,    47,    36,    48,     7,
       9,    10,    11,    12,    13,    30,    36,    42,    46,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    66,    27,
      27,    27,    58,    27,    27,    20,    31,    31,    31,    31,
      36,    58,    58,    51,    36,    58,    58,    28,    28,    31,
      28,    28,    50,    50,    58,     8,    31,    50,    51,    28,
      50
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    38,    39,    40,    40,    41,    41,    42,    42,    43,
      44,    44,    45,    45,    47,    46,    48,    48,    49,    49,
      50,    50,    50,    50,    50,    50,    50,    50,    51,    52,
      52,    53,    54,    55,    56,    57,    58,    59,    59,    60,
      60,    61,    61,    61,    62,    62,    62,    62,    62,    63,
      63,    63,    64,    64,    64,    65,    65,    65,    65,    65,
      65,    66,    66,    66,    66
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     3,     5,     6,
       0,     1,     2,     4,     0,     4,     0,     2,     1,     1,
       2,     1,     1,     1,     2,     2,     2,     1,     3,     7,
       5,     5,     9,     2,     4,     4,     1,     1,     3,     1,
       3,     1,     3,     3,     1,     3,     3,     3,     3,     1,
       3,     3,     1,     3,     3,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
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
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
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
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
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
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

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
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
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
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  case 2: /* program: top_list  */
#line 82 "parser.y"
               { program_root = ast_program(NULL, (yyvsp[0].node)); }
#line 1249 "parser.c"
    break;

  case 3: /* top_list: %empty  */
#line 86 "parser.y"
                              { (yyval.node) = NULL; }
#line 1255 "parser.c"
    break;

  case 4: /* top_list: top_list top  */
#line 87 "parser.y"
                              { (yyval.node) = ast_func_list((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1261 "parser.c"
    break;

  case 5: /* top: decl  */
#line 91 "parser.y"
                              { (yyval.node) = (yyvsp[0].node); }
#line 1267 "parser.c"
    break;

  case 6: /* top: function  */
#line 92 "parser.y"
                              { (yyval.node) = (yyvsp[0].node); }
#line 1273 "parser.c"
    break;

  case 7: /* decl: type_spec IDENTIFIER SEMICOLON  */
#line 96 "parser.y"
                                   {
        if (lookup_symbol_current_scope(symtab, (yyvsp[-1].sval))) {
            semantic_error("Variable already declared in this scope");
        } else {
            insert_symbol(symtab, (yyvsp[-1].sval), (yyvsp[-2].node));
        }
        (yyval.node) = ast_declaration((yyvsp[-2].node), (yyvsp[-1].sval), NULL);
    }
#line 1286 "parser.c"
    break;

  case 8: /* decl: type_spec IDENTIFIER ASSIGN expr SEMICOLON  */
#line 104 "parser.y"
                                                 {
        const char *spec = get_type_name((yyvsp[-4].node));
        // For initialization, get type from expression
        const char *expr_type = "unknown";
        if ((yyvsp[-1].node)) {
            if ((yyvsp[-1].node)->type == NODE_FLOAT_LIT) expr_type = "float";
            else if ((yyvsp[-1].node)->type == NODE_INT_LIT) expr_type = "int";
            else if ((yyvsp[-1].node)->type == NODE_BOOL_LIT) expr_type = "int";
            else if ((yyvsp[-1].node)->type == NODE_STRING_LIT) expr_type = "string";
            else if ((yyvsp[-1].node)->type == NODE_VAR) {
                Symbol *sym = lookup_symbol(symtab, (yyvsp[-1].node)->data.sval);
                if (sym && sym->type) expr_type = get_type_name(sym->type);
            }
            else expr_type = ast_get_type((yyvsp[-1].node));
        }
        if (strcmp(spec, expr_type) != 0) {
            char buf[256];
            snprintf(buf, sizeof(buf), "type mismatch in initialization of '%s' (%s vs %s)", (yyvsp[-3].sval), spec, expr_type);
            semantic_error(buf);
        } else {
            if (lookup_symbol_current_scope(symtab, (yyvsp[-3].sval))) {
                semantic_error("Variable already declared in this scope");
            } else {
                insert_symbol(symtab, (yyvsp[-3].sval), (yyvsp[-4].node));
            }
        }
        (yyval.node) = ast_declaration((yyvsp[-4].node), (yyvsp[-3].sval), (yyvsp[-1].node));
    }
#line 1319 "parser.c"
    break;

  case 9: /* function: type_spec IDENTIFIER LPAREN param_opt RPAREN block  */
#line 135 "parser.y"
                                                       {
        if (lookup_symbol_current_scope(symtab, (yyvsp[-4].sval))) {
            semantic_error("Function already declared in this scope");
        } else {
            insert_symbol(symtab, (yyvsp[-4].sval), (yyvsp[-5].node));
        }
        (yyval.node) = ast_function((yyvsp[-5].node), (yyvsp[-4].sval), (yyvsp[-2].node), (yyvsp[0].node));
    }
#line 1332 "parser.c"
    break;

  case 10: /* param_opt: %empty  */
#line 146 "parser.y"
                     { (yyval.node) = NULL; }
#line 1338 "parser.c"
    break;

  case 11: /* param_opt: param_list  */
#line 147 "parser.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1344 "parser.c"
    break;

  case 12: /* param_list: type_spec IDENTIFIER  */
#line 151 "parser.y"
                         {
        insert_symbol(symtab, (yyvsp[0].sval), (yyvsp[-1].node));
        (yyval.node) = ast_param_list(NULL, (yyvsp[-1].node), (yyvsp[0].sval));
    }
#line 1353 "parser.c"
    break;

  case 13: /* param_list: param_list COMMA type_spec IDENTIFIER  */
#line 155 "parser.y"
                                            {
        insert_symbol(symtab, (yyvsp[0].sval), (yyvsp[-1].node));
        (yyval.node) = ast_param_list((yyvsp[-3].node), (yyvsp[-1].node), (yyvsp[0].sval));
    }
#line 1362 "parser.c"
    break;

  case 14: /* $@1: %empty  */
#line 162 "parser.y"
           { enter_scope(symtab); }
#line 1368 "parser.c"
    break;

  case 15: /* block: LBRACE $@1 block_item_list RBRACE  */
#line 162 "parser.y"
                                                           { exit_scope(symtab); (yyval.node) = ast_block((yyvsp[-1].node)); }
#line 1374 "parser.c"
    break;

  case 16: /* block_item_list: %empty  */
#line 166 "parser.y"
                              { (yyval.node) = NULL; }
#line 1380 "parser.c"
    break;

  case 17: /* block_item_list: block_item_list block_item  */
#line 167 "parser.y"
                                 { (yyval.node) = ast_stmt_list((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1386 "parser.c"
    break;

  case 18: /* block_item: decl  */
#line 171 "parser.y"
                              { (yyval.node) = (yyvsp[0].node); }
#line 1392 "parser.c"
    break;

  case 19: /* block_item: stmt  */
#line 172 "parser.y"
                              { (yyval.node) = (yyvsp[0].node); }
#line 1398 "parser.c"
    break;

  case 20: /* stmt: assign_stmt SEMICOLON  */
#line 181 "parser.y"
                              { (yyval.node) = (yyvsp[-1].node); }
#line 1404 "parser.c"
    break;

  case 21: /* stmt: if_stmt  */
#line 182 "parser.y"
                              { (yyval.node) = (yyvsp[0].node); }
#line 1410 "parser.c"
    break;

  case 22: /* stmt: while_stmt  */
#line 183 "parser.y"
                              { (yyval.node) = (yyvsp[0].node); }
#line 1416 "parser.c"
    break;

  case 23: /* stmt: for_stmt  */
#line 184 "parser.y"
                              { (yyval.node) = (yyvsp[0].node); }
#line 1422 "parser.c"
    break;

  case 24: /* stmt: return_stmt SEMICOLON  */
#line 185 "parser.y"
                              { (yyval.node) = (yyvsp[-1].node); }
#line 1428 "parser.c"
    break;

  case 25: /* stmt: input_stmt SEMICOLON  */
#line 186 "parser.y"
                              { (yyval.node) = (yyvsp[-1].node); }
#line 1434 "parser.c"
    break;

  case 26: /* stmt: output_stmt SEMICOLON  */
#line 187 "parser.y"
                              { (yyval.node) = (yyvsp[-1].node); }
#line 1440 "parser.c"
    break;

  case 27: /* stmt: block  */
#line 188 "parser.y"
                              { (yyval.node) = (yyvsp[0].node); }
#line 1446 "parser.c"
    break;

  case 28: /* assign_stmt: IDENTIFIER ASSIGN expr  */
#line 192 "parser.y"
                           {
        Symbol *sym = lookup_symbol(symtab, (yyvsp[-2].sval));
        if (!sym) {
            char buf[256];
            snprintf(buf, sizeof(buf), "variable '%s' not declared", (yyvsp[-2].sval));
            semantic_error(buf);
        } else {
            const char *var_type = get_type_name(sym->type);
            const char *expr_type = "unknown";
            if ((yyvsp[0].node)) {
                if ((yyvsp[0].node)->type == NODE_FLOAT_LIT) expr_type = "float";
                else if ((yyvsp[0].node)->type == NODE_INT_LIT) expr_type = "int";
                else if ((yyvsp[0].node)->type == NODE_BOOL_LIT) expr_type = "int";
                else if ((yyvsp[0].node)->type == NODE_STRING_LIT) expr_type = "string";
                else if ((yyvsp[0].node)->type == NODE_VAR) {
                    Symbol *s = lookup_symbol(symtab, (yyvsp[0].node)->data.sval);
                    if (s && s->type) expr_type = get_type_name(s->type);
                }
                else expr_type = ast_get_type((yyvsp[0].node));
            }
            if (strcmp(var_type, expr_type) != 0) {
                char buf[256];
                snprintf(buf, sizeof(buf), "type mismatch assigning '%s' to '%s'", expr_type, var_type);
                semantic_error(buf);
            }
        }
        (yyval.node) = ast_assignment((yyvsp[-2].sval), (yyvsp[0].node));
    }
#line 1479 "parser.c"
    break;

  case 29: /* if_stmt: IF LPAREN expr RPAREN stmt ELSE stmt  */
#line 223 "parser.y"
                                           { (yyval.node) = ast_if((yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1485 "parser.c"
    break;

  case 30: /* if_stmt: IF LPAREN expr RPAREN stmt  */
#line 224 "parser.y"
                                           { (yyval.node) = ast_if((yyvsp[-2].node), (yyvsp[0].node), NULL); }
#line 1491 "parser.c"
    break;

  case 31: /* while_stmt: WHILE LPAREN expr RPAREN stmt  */
#line 228 "parser.y"
                                    { (yyval.node) = ast_while((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1497 "parser.c"
    break;

  case 32: /* for_stmt: FOR LPAREN assign_stmt SEMICOLON expr SEMICOLON assign_stmt RPAREN stmt  */
#line 233 "parser.y"
        { (yyval.node) = ast_for((yyvsp[-6].node), (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1503 "parser.c"
    break;

  case 33: /* return_stmt: RETURN expr  */
#line 237 "parser.y"
                { (yyval.node) = ast_return((yyvsp[0].node)); }
#line 1509 "parser.c"
    break;

  case 34: /* input_stmt: INPUT LPAREN IDENTIFIER RPAREN  */
#line 241 "parser.y"
                                   {
        Symbol *sym = lookup_symbol(symtab, (yyvsp[-1].sval));
        if (!sym) {
            char buf[256];
            snprintf(buf, sizeof(buf), "variable '%s' not declared", (yyvsp[-1].sval));
            semantic_error(buf);
        }
        (yyval.node) = ast_input((yyvsp[-1].sval));
    }
#line 1523 "parser.c"
    break;

  case 35: /* output_stmt: OUTPUT LPAREN expr RPAREN  */
#line 253 "parser.y"
                              { (yyval.node) = ast_output((yyvsp[-1].node)); }
#line 1529 "parser.c"
    break;

  case 36: /* expr: logical_or  */
#line 257 "parser.y"
                 { (yyval.node) = (yyvsp[0].node); ast_set_type((yyval.node), ast_get_type((yyvsp[0].node))); }
#line 1535 "parser.c"
    break;

  case 37: /* logical_or: logical_and  */
#line 261 "parser.y"
                                { (yyval.node) = (yyvsp[0].node); ast_set_type((yyval.node), ast_get_type((yyvsp[0].node))); }
#line 1541 "parser.c"
    break;

  case 38: /* logical_or: logical_or OR logical_and  */
#line 262 "parser.y"
                                { (yyval.node) = ast_binary("||", (yyvsp[-2].node), (yyvsp[0].node)); ast_set_type((yyval.node), "int"); }
#line 1547 "parser.c"
    break;

  case 39: /* logical_and: equality  */
#line 266 "parser.y"
                                 { (yyval.node) = (yyvsp[0].node); ast_set_type((yyval.node), ast_get_type((yyvsp[0].node))); }
#line 1553 "parser.c"
    break;

  case 40: /* logical_and: logical_and AND equality  */
#line 267 "parser.y"
                                 { (yyval.node) = ast_binary("&&", (yyvsp[-2].node), (yyvsp[0].node)); ast_set_type((yyval.node), "int"); }
#line 1559 "parser.c"
    break;

  case 41: /* equality: relational  */
#line 271 "parser.y"
                                 { (yyval.node) = (yyvsp[0].node); ast_set_type((yyval.node), ast_get_type((yyvsp[0].node))); }
#line 1565 "parser.c"
    break;

  case 42: /* equality: equality EQ relational  */
#line 272 "parser.y"
                                 { (yyval.node) = ast_binary("==", (yyvsp[-2].node), (yyvsp[0].node)); ast_set_type((yyval.node), "int"); }
#line 1571 "parser.c"
    break;

  case 43: /* equality: equality NE relational  */
#line 273 "parser.y"
                                 { (yyval.node) = ast_binary("!=", (yyvsp[-2].node), (yyvsp[0].node)); ast_set_type((yyval.node), "int"); }
#line 1577 "parser.c"
    break;

  case 44: /* relational: additive  */
#line 277 "parser.y"
                                 { (yyval.node) = (yyvsp[0].node); ast_set_type((yyval.node), ast_get_type((yyvsp[0].node))); }
#line 1583 "parser.c"
    break;

  case 45: /* relational: relational LT additive  */
#line 278 "parser.y"
                                 { (yyval.node) = ast_binary("<", (yyvsp[-2].node), (yyvsp[0].node)); ast_set_type((yyval.node), "int"); }
#line 1589 "parser.c"
    break;

  case 46: /* relational: relational GT additive  */
#line 279 "parser.y"
                                 { (yyval.node) = ast_binary(">", (yyvsp[-2].node), (yyvsp[0].node)); ast_set_type((yyval.node), "int"); }
#line 1595 "parser.c"
    break;

  case 47: /* relational: relational LE additive  */
#line 280 "parser.y"
                                 { (yyval.node) = ast_binary("<=", (yyvsp[-2].node), (yyvsp[0].node)); ast_set_type((yyval.node), "int"); }
#line 1601 "parser.c"
    break;

  case 48: /* relational: relational GE additive  */
#line 281 "parser.y"
                                 { (yyval.node) = ast_binary(">=", (yyvsp[-2].node), (yyvsp[0].node)); ast_set_type((yyval.node), "int"); }
#line 1607 "parser.c"
    break;

  case 49: /* additive: multiplicative  */
#line 285 "parser.y"
                                 { (yyval.node) = (yyvsp[0].node); ast_set_type((yyval.node), ast_get_type((yyvsp[0].node))); }
#line 1613 "parser.c"
    break;

  case 50: /* additive: additive PLUS multiplicative  */
#line 286 "parser.y"
                                     { (yyval.node) = ast_binary("+", (yyvsp[-2].node), (yyvsp[0].node)); ast_set_type((yyval.node), "int"); }
#line 1619 "parser.c"
    break;

  case 51: /* additive: additive MINUS multiplicative  */
#line 287 "parser.y"
                                     { (yyval.node) = ast_binary("-", (yyvsp[-2].node), (yyvsp[0].node)); ast_set_type((yyval.node), "int"); }
#line 1625 "parser.c"
    break;

  case 52: /* multiplicative: factor  */
#line 291 "parser.y"
                                 { (yyval.node) = (yyvsp[0].node); ast_set_type((yyval.node), ast_get_type((yyvsp[0].node))); }
#line 1631 "parser.c"
    break;

  case 53: /* multiplicative: multiplicative STAR factor  */
#line 292 "parser.y"
                                 { (yyval.node) = ast_binary("*", (yyvsp[-2].node), (yyvsp[0].node)); ast_set_type((yyval.node), "int"); }
#line 1637 "parser.c"
    break;

  case 54: /* multiplicative: multiplicative SLASH factor  */
#line 293 "parser.y"
                                  { (yyval.node) = ast_binary("/", (yyvsp[-2].node), (yyvsp[0].node)); ast_set_type((yyval.node), "int"); }
#line 1643 "parser.c"
    break;

  case 55: /* factor: INT_LIT  */
#line 297 "parser.y"
                       { (yyval.node) = ast_int_lit((yyvsp[0].ival)); ast_set_type((yyval.node), "int"); }
#line 1649 "parser.c"
    break;

  case 56: /* factor: FLOAT_LIT  */
#line 298 "parser.y"
                       { (yyval.node) = ast_float_lit((yyvsp[0].fval)); ast_set_type((yyval.node), "float"); }
#line 1655 "parser.c"
    break;

  case 57: /* factor: BOOL_LIT  */
#line 299 "parser.y"
                       { (yyval.node) = ast_bool_lit((yyvsp[0].ival)); ast_set_type((yyval.node), "int"); }
#line 1661 "parser.c"
    break;

  case 58: /* factor: IDENTIFIER  */
#line 300 "parser.y"
                       {
        (yyval.node) = ast_var((yyvsp[0].sval));
        Symbol *sym = lookup_symbol(symtab, (yyvsp[0].sval));
        if (!sym) {
            char buf[256];
            snprintf(buf, sizeof(buf), "variable '%s' not declared", (yyvsp[0].sval));
            semantic_error(buf);
            ast_set_type((yyval.node), "unknown");
        } else {
            ast_set_type((yyval.node), get_type_name(sym->type));
        }
    }
#line 1678 "parser.c"
    break;

  case 59: /* factor: STRING_LIT  */
#line 312 "parser.y"
                       { (yyval.node) = ast_string_lit((yyvsp[0].sval)); ast_set_type((yyval.node), "string"); }
#line 1684 "parser.c"
    break;

  case 60: /* factor: LPAREN expr RPAREN  */
#line 313 "parser.y"
                          { (yyval.node) = (yyvsp[-1].node); ast_set_type((yyval.node), ast_get_type((yyvsp[-1].node))); }
#line 1690 "parser.c"
    break;

  case 61: /* type_spec: TYPE_VOID  */
#line 317 "parser.y"
                { (yyval.node) = ast_type("void"); }
#line 1696 "parser.c"
    break;

  case 62: /* type_spec: TYPE_INT  */
#line 318 "parser.y"
                  { (yyval.node) = ast_type("int"); }
#line 1702 "parser.c"
    break;

  case 63: /* type_spec: TYPE_FLOAT  */
#line 319 "parser.y"
                 { (yyval.node) = ast_type("float"); }
#line 1708 "parser.c"
    break;

  case 64: /* type_spec: TYPE_BOOL  */
#line 320 "parser.y"
                 { (yyval.node) = ast_type("bool"); }
#line 1714 "parser.c"
    break;


#line 1718 "parser.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

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
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
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
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 323 "parser.y"


void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error at line %d: %s\n", yylineno, s);
    semantic_errors = 1;
}
