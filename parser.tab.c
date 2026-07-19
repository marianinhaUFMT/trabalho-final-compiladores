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
#include <string.h>
#include "tab_simb.h"

int  yylex(void);
void yyerror(const char *s);

extern int linha_atual;

int erros_encontrados = 0;
int main_definida = 0;

/* acumulador temporario usado para montar a assinatura de uma funcao
   (prototipo ou implementacao) antes de gravar/validar na TS de funcoes */
static SimboloFuncao temp_func;

/* acumulador temporario dos argumentos de uma chamada de funcao */
static char  chamada_nome[MAX_NOME];
static Tipo  chamada_args[MAX_PARAMS];
static int   chamada_n_args;

static void erro_semantico(const char *msg) {
    fprintf(stderr, "[ERRO SEMANTICO] linha %d: %s\n", linha_atual, msg);
    erros_encontrados++;
}

/* compatibilidade simples entre tipos (int/float sao mutuamente compativeis) */
static int compativel(Tipo a, Tipo b) {
    if (a == b) return 1;
    if ((a == T_INT || a == T_FLOAT) && (b == T_INT || b == T_FLOAT)) return 1;
    return 0;
}

static void inicia_chamada(const char *nome) {
    strncpy(chamada_nome, nome, MAX_NOME - 1);
    chamada_nome[MAX_NOME - 1] = '\0';
    chamada_n_args = 0;
}

static void empilha_arg(Tipo t) {
    if (chamada_n_args < MAX_PARAMS) {
        chamada_args[chamada_n_args++] = t;
    }
}

static Tipo finaliza_chamada(void) {
    SimboloFuncao *f = ts_func_busca(chamada_nome);
    if (f == NULL || !f->prototipada) {
        char msg[160];
        sprintf(msg, "chamada da funcao '%s' nao declarada (sem prototipo)", chamada_nome);
        erro_semantico(msg);
        return T_INDEFINIDO;
    }
    if (chamada_n_args != f->n_params) {
        char msg[160];
        sprintf(msg, "funcao '%s' espera %d parametro(s), mas recebeu %d",
                chamada_nome, f->n_params, chamada_n_args);
        erro_semantico(msg);
        return f->tipo_retorno;
    }
    for (int i = 0; i < chamada_n_args; i++) {
        if (!compativel(chamada_args[i], f->params[i].tipo)) {
            char msg[192];
            sprintf(msg, "parametro %d da chamada de '%s' incompativel: esperado %s, recebido %s",
                    i + 1, chamada_nome, tipo_para_str(f->params[i].tipo), tipo_para_str(chamada_args[i]));
            erro_semantico(msg);
        }
    }
    return f->tipo_retorno;
}

#line 145 "parser.tab.c"

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

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INT = 3,                        /* INT  */
  YYSYMBOL_FLOAT = 4,                      /* FLOAT  */
  YYSYMBOL_CHAR = 5,                       /* CHAR  */
  YYSYMBOL_BOOL = 6,                       /* BOOL  */
  YYSYMBOL_STRING = 7,                     /* STRING  */
  YYSYMBOL_VARIABLE = 8,                   /* VARIABLE  */
  YYSYMBOL_INTEGER = 9,                    /* INTEGER  */
  YYSYMBOL_FLOAT_CONST = 10,               /* FLOAT_CONST  */
  YYSYMBOL_CHAR_CONST = 11,                /* CHAR_CONST  */
  YYSYMBOL_STRING_CONST = 12,              /* STRING_CONST  */
  YYSYMBOL_IF = 13,                        /* IF  */
  YYSYMBOL_ELSE = 14,                      /* ELSE  */
  YYSYMBOL_WHILE = 15,                     /* WHILE  */
  YYSYMBOL_READ = 16,                      /* READ  */
  YYSYMBOL_WRITE = 17,                     /* WRITE  */
  YYSYMBOL_RETURN = 18,                    /* RETURN  */
  YYSYMBOL_TRUE = 19,                      /* TRUE  */
  YYSYMBOL_FALSE = 20,                     /* FALSE  */
  YYSYMBOL_MAIN = 21,                      /* MAIN  */
  YYSYMBOL_EQ = 22,                        /* EQ  */
  YYSYMBOL_NE = 23,                        /* NE  */
  YYSYMBOL_LT = 24,                        /* LT  */
  YYSYMBOL_LE = 25,                        /* LE  */
  YYSYMBOL_GT = 26,                        /* GT  */
  YYSYMBOL_GE = 27,                        /* GE  */
  YYSYMBOL_NOT = 28,                       /* NOT  */
  YYSYMBOL_29_ = 29,                       /* '+'  */
  YYSYMBOL_30_ = 30,                       /* '-'  */
  YYSYMBOL_31_ = 31,                       /* '*'  */
  YYSYMBOL_32_ = 32,                       /* '/'  */
  YYSYMBOL_33_ = 33,                       /* ';'  */
  YYSYMBOL_34_ = 34,                       /* '('  */
  YYSYMBOL_35_ = 35,                       /* ')'  */
  YYSYMBOL_36_ = 36,                       /* ','  */
  YYSYMBOL_37_ = 37,                       /* '{'  */
  YYSYMBOL_38_ = 38,                       /* '}'  */
  YYSYMBOL_39_ = 39,                       /* '='  */
  YYSYMBOL_YYACCEPT = 40,                  /* $accept  */
  YYSYMBOL_programa = 41,                  /* programa  */
  YYSYMBOL_lista_declaracoes_globais = 42, /* lista_declaracoes_globais  */
  YYSYMBOL_declaracao_global = 43,         /* declaracao_global  */
  YYSYMBOL_tipo = 44,                      /* tipo  */
  YYSYMBOL_declaracao_variavel = 45,       /* declaracao_variavel  */
  YYSYMBOL_assinatura_funcao = 46,         /* assinatura_funcao  */
  YYSYMBOL_47_1 = 47,                      /* $@1  */
  YYSYMBOL_lista_params_opt = 48,          /* lista_params_opt  */
  YYSYMBOL_lista_params = 49,              /* lista_params  */
  YYSYMBOL_prototipo_funcao = 50,          /* prototipo_funcao  */
  YYSYMBOL_definicao_funcao = 51,          /* definicao_funcao  */
  YYSYMBOL_52_2 = 52,                      /* $@2  */
  YYSYMBOL_funcao_main = 53,               /* funcao_main  */
  YYSYMBOL_54_3 = 54,                      /* $@3  */
  YYSYMBOL_lista_comandos = 55,            /* lista_comandos  */
  YYSYMBOL_comando = 56,                   /* comando  */
  YYSYMBOL_atribuicao = 57,                /* atribuicao  */
  YYSYMBOL_condicional = 58,               /* condicional  */
  YYSYMBOL_laco = 59,                      /* laco  */
  YYSYMBOL_leitura = 60,                   /* leitura  */
  YYSYMBOL_escrita = 61,                   /* escrita  */
  YYSYMBOL_comando_return = 62,            /* comando_return  */
  YYSYMBOL_chamada_funcao = 63,            /* chamada_funcao  */
  YYSYMBOL_64_4 = 64,                      /* $@4  */
  YYSYMBOL_lista_args_opt = 65,            /* lista_args_opt  */
  YYSYMBOL_lista_args = 66,                /* lista_args  */
  YYSYMBOL_expr = 67                       /* expr  */
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
#define YYLAST   239

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  70
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  138

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   283


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
      34,    35,    31,    29,    36,    30,     2,    32,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    33,
       2,    39,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    37,     2,    38,     2,     2,     2,     2,
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
      25,    26,    27,    28
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    98,    98,   102,   103,   107,   108,   109,   113,   114,
     115,   116,   117,   123,   143,   142,   153,   154,   158,   162,
     171,   194,   193,   253,   252,   278,   279,   283,   284,   285,
     286,   287,   288,   289,   290,   294,   313,   314,   318,   322,
     336,   344,   345,   346,   350,   369,   369,   377,   378,   382,
     383,   389,   390,   391,   392,   393,   394,   395,   407,   408,
     410,   412,   414,   416,   417,   418,   419,   420,   421,   422,
     423
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
  "\"end of file\"", "error", "\"invalid token\"", "INT", "FLOAT", "CHAR",
  "BOOL", "STRING", "VARIABLE", "INTEGER", "FLOAT_CONST", "CHAR_CONST",
  "STRING_CONST", "IF", "ELSE", "WHILE", "READ", "WRITE", "RETURN", "TRUE",
  "FALSE", "MAIN", "EQ", "NE", "LT", "LE", "GT", "GE", "NOT", "'+'", "'-'",
  "'*'", "'/'", "';'", "'('", "')'", "','", "'{'", "'}'", "'='", "$accept",
  "programa", "lista_declaracoes_globais", "declaracao_global", "tipo",
  "declaracao_variavel", "assinatura_funcao", "$@1", "lista_params_opt",
  "lista_params", "prototipo_funcao", "definicao_funcao", "$@2",
  "funcao_main", "$@3", "lista_comandos", "comando", "atribuicao",
  "condicional", "laco", "leitura", "escrita", "comando_return",
  "chamada_funcao", "$@4", "lista_args_opt", "lista_args", "expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-53)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -53,    27,   179,   -53,     8,   -53,   -53,   -53,   -53,   -53,
      22,   -53,    10,   -53,   -53,   -53,    -3,   -21,   -53,     7,
      18,   -53,   -53,   -53,   -53,   232,    44,    17,   -53,    37,
      21,    47,   -23,    24,    50,    51,    58,    -2,   -53,    91,
     -53,   -53,   -53,   -53,   -53,   -53,   -53,   -53,    74,   -53,
     -53,   -53,   232,   -53,    -2,    -2,    -2,   100,    11,    75,
     -53,   -53,   -53,   -53,   -53,   -53,    -2,    -2,   -53,   180,
      90,   -53,    73,   126,    -2,   192,    71,   146,   101,   102,
     104,   110,   111,   -53,   166,    -2,    -2,    -2,    -2,    -2,
      -2,    -2,    -2,    -2,    -2,   -53,   -53,   -53,   113,    99,
      41,   -53,   107,   112,   117,   118,   127,   128,   141,   -53,
     202,   202,    89,    89,    89,    89,    -7,    -7,   -53,   -53,
     -53,    -2,   -53,   -53,   -53,   -53,   -53,   -53,   -53,    41,
     109,   125,   144,   -53,   122,   -53,   149,   -53
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     0,     1,     8,     9,    10,    11,    12,     3,
       0,     5,    21,     6,     7,     2,     0,     0,    20,     0,
       0,    13,    14,    26,    23,    17,     0,     0,     8,     0,
       0,    16,     0,     0,     0,     0,     0,     0,    22,     0,
      27,    25,    28,    29,    30,    31,    32,    33,     0,    26,
      19,    15,     0,    45,     0,     0,     0,     0,     0,    57,
      51,    52,    53,    54,    55,    56,     0,     0,    58,     0,
       0,    34,     0,     0,    48,     0,     0,     0,     0,     0,
       0,     0,     0,    69,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    44,    24,    18,     0,    47,
      50,    35,     0,     0,     0,     0,     0,     0,     0,    70,
      63,    64,    65,    66,    67,    68,    59,    60,    61,    62,
      46,     0,    26,    26,    39,    40,    41,    42,    43,    49,
       0,     0,    36,    38,     0,    26,     0,    37
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -53,   -53,   -53,   -53,     3,   177,   -53,   -53,   -53,   -53,
     -53,   -53,   -53,   -53,   -53,   -48,   -53,   -53,   -53,   -53,
     -53,   -53,   -53,   -26,   -53,   -53,   -53,   -52
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,     9,    39,    40,    12,    25,    30,    31,
      13,    14,    19,    15,    27,    26,    41,    42,    43,    44,
      45,    46,    47,    68,    74,    98,    99,    69
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      48,    72,    75,    76,    77,    10,    59,    60,    61,    62,
      63,    53,    21,    22,    83,    84,    54,    64,    65,    79,
      80,    81,   100,    82,    93,    94,    66,     3,    29,    16,
      17,    20,    67,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,    18,    23,    50,    48,    28,     5,     6,
       7,     8,    32,    24,    49,    73,    51,    33,    55,    34,
      35,    36,    37,    85,    86,    87,    88,    89,    90,   129,
      91,    92,    93,    94,   130,   131,    28,     5,     6,     7,
       8,    32,    38,    52,    56,    57,    33,   136,    34,    35,
      36,    37,    58,    85,    86,    87,    88,    89,    90,    70,
      91,    92,    93,    94,    48,    48,   102,    71,    78,    53,
      48,    96,    28,     5,     6,     7,     8,    32,    91,    92,
      93,    94,    33,    21,    34,    35,    36,    37,    28,     5,
       6,     7,     8,    32,    97,   121,   104,   105,    33,   106,
      34,    35,    36,    37,   122,   107,   108,   132,   120,   123,
     124,   125,    28,     5,     6,     7,     8,    32,   134,   135,
     126,   127,    33,   133,    34,    35,    36,    37,    85,    86,
      87,    88,    89,    90,   128,    91,    92,    93,    94,    11,
       0,   103,     4,     5,     6,     7,     8,   137,    85,    86,
      87,    88,    89,    90,     0,    91,    92,    93,    94,     0,
       0,   109,    85,    86,    87,    88,    89,    90,     0,    91,
      92,    93,    94,    95,    85,    86,    87,    88,    89,    90,
       0,    91,    92,    93,    94,   101,    87,    88,    89,    90,
       0,    91,    92,    93,    94,    28,     5,     6,     7,     8
};

static const yytype_int16 yycheck[] =
{
      26,    49,    54,    55,    56,     2,     8,     9,    10,    11,
      12,    34,    33,    34,    66,    67,    39,    19,    20,     8,
       9,    10,    74,    12,    31,    32,    28,     0,    25,    21,
       8,    34,    34,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    33,    37,     8,    72,     3,     4,     5,
       6,     7,     8,    35,    37,    52,    35,    13,    34,    15,
      16,    17,    18,    22,    23,    24,    25,    26,    27,   121,
      29,    30,    31,    32,   122,   123,     3,     4,     5,     6,
       7,     8,    38,    36,    34,    34,    13,   135,    15,    16,
      17,    18,    34,    22,    23,    24,    25,    26,    27,     8,
      29,    30,    31,    32,   130,   131,    35,    33,     8,    34,
     136,    38,     3,     4,     5,     6,     7,     8,    29,    30,
      31,    32,    13,    33,    15,    16,    17,    18,     3,     4,
       5,     6,     7,     8,     8,    36,    35,    35,    13,    35,
      15,    16,    17,    18,    37,    35,    35,    38,    35,    37,
      33,    33,     3,     4,     5,     6,     7,     8,    14,    37,
      33,    33,    13,    38,    15,    16,    17,    18,    22,    23,
      24,    25,    26,    27,    33,    29,    30,    31,    32,     2,
      -1,    35,     3,     4,     5,     6,     7,    38,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    31,    32,    -1,
      -1,    35,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    31,    32,    33,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    31,    32,    33,    24,    25,    26,    27,
      -1,    29,    30,    31,    32,     3,     4,     5,     6,     7
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    41,    42,     0,     3,     4,     5,     6,     7,    43,
      44,    45,    46,    50,    51,    53,    21,     8,    33,    52,
      34,    33,    34,    37,    35,    47,    55,    54,     3,    44,
      48,    49,     8,    13,    15,    16,    17,    18,    38,    44,
      45,    56,    57,    58,    59,    60,    61,    62,    63,    37,
       8,    35,    36,    34,    39,    34,    34,    34,    34,     8,
       9,    10,    11,    12,    19,    20,    28,    34,    63,    67,
       8,    33,    55,    44,    64,    67,    67,    67,     8,     8,
       9,    10,    12,    67,    67,    22,    23,    24,    25,    26,
      27,    29,    30,    31,    32,    33,    38,     8,    65,    66,
      67,    33,    35,    35,    35,    35,    35,    35,    35,    35,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      35,    36,    37,    37,    33,    33,    33,    33,    33,    67,
      55,    55,    38,    38,    14,    37,    55,    38
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    40,    41,    42,    42,    43,    43,    43,    44,    44,
      44,    44,    44,    45,    47,    46,    48,    48,    49,    49,
      50,    52,    51,    54,    53,    55,    55,    56,    56,    56,
      56,    56,    56,    56,    56,    57,    58,    58,    59,    60,
      61,    61,    61,    61,    62,    64,    63,    65,    65,    66,
      66,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     2,     0,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     0,     6,     1,     0,     4,     2,
       2,     0,     5,     0,     8,     2,     0,     1,     1,     1,
       1,     1,     1,     1,     2,     4,     7,    11,     7,     5,
       5,     5,     5,     5,     3,     0,     5,     1,     0,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       3
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
  case 8: /* tipo: INT  */
#line 113 "parser.y"
                { (yyval.c).tipo = T_INT; }
#line 1307 "parser.tab.c"
    break;

  case 9: /* tipo: FLOAT  */
#line 114 "parser.y"
                { (yyval.c).tipo = T_FLOAT; }
#line 1313 "parser.tab.c"
    break;

  case 10: /* tipo: CHAR  */
#line 115 "parser.y"
                { (yyval.c).tipo = T_CHAR; }
#line 1319 "parser.tab.c"
    break;

  case 11: /* tipo: BOOL  */
#line 116 "parser.y"
                { (yyval.c).tipo = T_BOOL; }
#line 1325 "parser.tab.c"
    break;

  case 12: /* tipo: STRING  */
#line 117 "parser.y"
                { (yyval.c).tipo = T_STRING; }
#line 1331 "parser.tab.c"
    break;

  case 13: /* declaracao_variavel: tipo VARIABLE ';'  */
#line 124 "parser.y"
        {
            TabelaVariaveis *ts = dentro_de_funcao ? &ts_local : &ts_global;
            if (!ts_var_insere(ts, (yyvsp[-1].c).str, (yyvsp[-2].c).tipo)) {
                char msg[160];
                sprintf(msg, "variavel '%s' ja declarada neste escopo", (yyvsp[-1].c).str);
                erro_semantico(msg);
            }
        }
#line 1344 "parser.tab.c"
    break;

  case 14: /* $@1: %empty  */
#line 143 "parser.y"
        {
            strncpy(temp_func.nome, (yyvsp[-1].c).str, MAX_NOME - 1);
            temp_func.nome[MAX_NOME - 1] = '\0';
            temp_func.tipo_retorno = (yyvsp[-2].c).tipo;
            temp_func.n_params = 0;
        }
#line 1355 "parser.tab.c"
    break;

  case 18: /* lista_params: lista_params ',' tipo VARIABLE  */
#line 159 "parser.y"
        {
            ts_func_adiciona_param(&temp_func, (yyvsp[0].c).str, (yyvsp[-1].c).tipo);
        }
#line 1363 "parser.tab.c"
    break;

  case 19: /* lista_params: tipo VARIABLE  */
#line 163 "parser.y"
        {
            ts_func_adiciona_param(&temp_func, (yyvsp[0].c).str, (yyvsp[-1].c).tipo);
        }
#line 1371 "parser.tab.c"
    break;

  case 20: /* prototipo_funcao: assinatura_funcao ';'  */
#line 172 "parser.y"
        {
            SimboloFuncao *existente = ts_func_busca(temp_func.nome);
            if (existente != NULL && existente->prototipada) {
                char msg[160];
                sprintf(msg, "funcao '%s' ja possui prototipo (redefinicao nao permitida)", temp_func.nome);
                erro_semantico(msg);
            } else {
                SimboloFuncao *f = ts_func_insere_prototipo(temp_func.nome, temp_func.tipo_retorno);
                f->tipo_retorno = temp_func.tipo_retorno;
                f->n_params = 0;
                for (int i = 0; i < temp_func.n_params; i++) {
                    ts_func_adiciona_param(f, temp_func.params[i].nome, temp_func.params[i].tipo);
                }
                f->prototipada = 1;
            }
        }
#line 1392 "parser.tab.c"
    break;

  case 21: /* $@2: %empty  */
#line 194 "parser.y"
        {
            SimboloFuncao *f = ts_func_busca(temp_func.nome);
            if (f == NULL || !f->prototipada) {
                char msg[160];
                sprintf(msg, "funcao '%s' implementada sem prototipo previo", temp_func.nome);
                erro_semantico(msg);
                f = ts_func_insere_prototipo(temp_func.nome, temp_func.tipo_retorno);
                f->n_params = 0;
                for (int i = 0; i < temp_func.n_params; i++) {
                    ts_func_adiciona_param(f, temp_func.params[i].nome, temp_func.params[i].tipo);
                }
                f->prototipada = 1;
            } else {
                int assinatura_ok = (f->tipo_retorno == temp_func.tipo_retorno) &&
                                     (f->n_params == temp_func.n_params);
                if (assinatura_ok) {
                    for (int i = 0; i < f->n_params; i++) {
                        if (f->params[i].tipo != temp_func.params[i].tipo) { assinatura_ok = 0; break; }
                    }
                }
                if (!assinatura_ok) {
                    char msg[192];
                    sprintf(msg, "implementacao de '%s' nao corresponde ao prototipo declarado", temp_func.nome);
                    erro_semantico(msg);
                }
                if (f->implementada) {
                    char msg[160];
                    sprintf(msg, "funcao '%s' ja foi implementada (redefinicao nao permitida)", temp_func.nome);
                    erro_semantico(msg);
                }
            }

            f->implementada = 1;
            funcao_atual = f;
            dentro_de_funcao = 1;
            ts_var_init(&ts_local);
            /* parametros da funcao se comportam como variaveis locais, ja inicializadas */
            for (int i = 0; i < temp_func.n_params; i++) {
                ts_var_insere(&ts_local, temp_func.params[i].nome, temp_func.params[i].tipo);
                SimboloVar *pv = ts_var_busca(&ts_local, temp_func.params[i].nome);
                if (pv) pv->inicializada = 1;
            }
        }
#line 1440 "parser.tab.c"
    break;

  case 22: /* definicao_funcao: assinatura_funcao $@2 '{' lista_comandos '}'  */
#line 238 "parser.y"
        {
            if (!funcao_atual->tem_return) {
                char msg[160];
                sprintf(msg, "funcao '%s' nao possui comando 'return' (retorno obrigatorio)", funcao_atual->nome);
                erro_semantico(msg);
            }
            dentro_de_funcao = 0;
            funcao_atual = NULL;
        }
#line 1454 "parser.tab.c"
    break;

  case 23: /* $@3: %empty  */
#line 253 "parser.y"
        {
            if (main_definida) {
                erro_semantico("funcao 'main' ja foi definida");
            }
            main_definida = 1;
            SimboloFuncao *f = ts_func_insere_prototipo("main", T_INT);
            f->prototipada = 1;
            f->implementada = 1;
            funcao_atual = f;
            dentro_de_funcao = 1;
            ts_var_init(&ts_local);
        }
#line 1471 "parser.tab.c"
    break;

  case 24: /* funcao_main: INT MAIN '(' ')' $@3 '{' lista_comandos '}'  */
#line 266 "parser.y"
        {
            if (!funcao_atual->tem_return) {
                erro_semantico("funcao 'main' nao possui comando 'return' (retorno obrigatorio)");
            }
            dentro_de_funcao = 0;
            funcao_atual = NULL;
        }
#line 1483 "parser.tab.c"
    break;

  case 35: /* atribuicao: VARIABLE '=' expr ';'  */
#line 295 "parser.y"
        {
            SimboloVar *v = busca_variavel((yyvsp[-3].c).str);
            if (v == NULL) {
                char msg[160];
                sprintf(msg, "variavel '%s' nao declarada", (yyvsp[-3].c).str);
                erro_semantico(msg);
            } else if (!compativel(v->tipo, (yyvsp[-1].c).tipo)) {
                char msg[192];
                sprintf(msg, "atribuicao incompativel: variavel '%s' e do tipo %s, mas expressao e do tipo %s",
                        (yyvsp[-3].c).str, tipo_para_str(v->tipo), tipo_para_str((yyvsp[-1].c).tipo));
                erro_semantico(msg);
            } else {
                v->inicializada = 1;
            }
        }
#line 1503 "parser.tab.c"
    break;

  case 39: /* leitura: READ '(' VARIABLE ')' ';'  */
#line 323 "parser.y"
        {
            SimboloVar *v = busca_variavel((yyvsp[-2].c).str);
            if (v == NULL) {
                char msg[160];
                sprintf(msg, "variavel '%s' nao declarada", (yyvsp[-2].c).str);
                erro_semantico(msg);
            } else {
                v->inicializada = 1;
            }
        }
#line 1518 "parser.tab.c"
    break;

  case 40: /* escrita: WRITE '(' VARIABLE ')' ';'  */
#line 337 "parser.y"
        {
            if (busca_variavel((yyvsp[-2].c).str) == NULL) {
                char msg[160];
                sprintf(msg, "variavel '%s' nao declarada", (yyvsp[-2].c).str);
                erro_semantico(msg);
            }
        }
#line 1530 "parser.tab.c"
    break;

  case 44: /* comando_return: RETURN expr ';'  */
#line 351 "parser.y"
        {
            if (funcao_atual == NULL) {
                erro_semantico("comando 'return' fora do escopo de uma funcao");
            } else {
                funcao_atual->tem_return = 1;
                if (!compativel(funcao_atual->tipo_retorno, (yyvsp[-1].c).tipo)) {
                    char msg[192];
                    sprintf(msg, "tipo de retorno incompativel: funcao '%s' retorna %s, mas expressao e do tipo %s",
                            funcao_atual->nome, tipo_para_str(funcao_atual->tipo_retorno), tipo_para_str((yyvsp[-1].c).tipo));
                    erro_semantico(msg);
                }
            }
        }
#line 1548 "parser.tab.c"
    break;

  case 45: /* $@4: %empty  */
#line 369 "parser.y"
                     { inicia_chamada((yyvsp[-1].c).str); }
#line 1554 "parser.tab.c"
    break;

  case 46: /* chamada_funcao: VARIABLE '(' $@4 lista_args_opt ')'  */
#line 370 "parser.y"
        {
            (yyval.c).tipo = finaliza_chamada();
            strncpy((yyval.c).str, chamada_nome, sizeof((yyval.c).str) - 1);
        }
#line 1563 "parser.tab.c"
    break;

  case 49: /* lista_args: lista_args ',' expr  */
#line 382 "parser.y"
                            { empilha_arg((yyvsp[0].c).tipo); }
#line 1569 "parser.tab.c"
    break;

  case 50: /* lista_args: expr  */
#line 383 "parser.y"
                            { empilha_arg((yyvsp[0].c).tipo); }
#line 1575 "parser.tab.c"
    break;

  case 51: /* expr: INTEGER  */
#line 389 "parser.y"
                           { (yyval.c).tipo = T_INT; }
#line 1581 "parser.tab.c"
    break;

  case 52: /* expr: FLOAT_CONST  */
#line 390 "parser.y"
                            { (yyval.c).tipo = T_FLOAT; }
#line 1587 "parser.tab.c"
    break;

  case 53: /* expr: CHAR_CONST  */
#line 391 "parser.y"
                            { (yyval.c).tipo = T_CHAR; }
#line 1593 "parser.tab.c"
    break;

  case 54: /* expr: STRING_CONST  */
#line 392 "parser.y"
                            { (yyval.c).tipo = T_STRING; }
#line 1599 "parser.tab.c"
    break;

  case 55: /* expr: TRUE  */
#line 393 "parser.y"
                            { (yyval.c).tipo = T_BOOL; }
#line 1605 "parser.tab.c"
    break;

  case 56: /* expr: FALSE  */
#line 394 "parser.y"
                            { (yyval.c).tipo = T_BOOL; }
#line 1611 "parser.tab.c"
    break;

  case 57: /* expr: VARIABLE  */
#line 396 "parser.y"
        {
            SimboloVar *v = busca_variavel((yyvsp[0].c).str);
            if (v == NULL) {
                char msg[160];
                sprintf(msg, "variavel '%s' nao declarada", (yyvsp[0].c).str);
                erro_semantico(msg);
                (yyval.c).tipo = T_INDEFINIDO;
            } else {
                (yyval.c).tipo = v->tipo;
            }
        }
#line 1627 "parser.tab.c"
    break;

  case 58: /* expr: chamada_funcao  */
#line 407 "parser.y"
                            { (yyval.c).tipo = (yyvsp[0].c).tipo; }
#line 1633 "parser.tab.c"
    break;

  case 59: /* expr: expr '+' expr  */
#line 408 "parser.y"
                            { (yyval.c).tipo = compativel((yyvsp[-2].c).tipo, (yyvsp[0].c).tipo) ? (yyvsp[-2].c).tipo : T_INDEFINIDO;
                               if ((yyval.c).tipo == T_INDEFINIDO) erro_semantico("operandos de '+' com tipos incompativeis"); }
#line 1640 "parser.tab.c"
    break;

  case 60: /* expr: expr '-' expr  */
#line 410 "parser.y"
                            { (yyval.c).tipo = compativel((yyvsp[-2].c).tipo, (yyvsp[0].c).tipo) ? (yyvsp[-2].c).tipo : T_INDEFINIDO;
                               if ((yyval.c).tipo == T_INDEFINIDO) erro_semantico("operandos de '-' com tipos incompativeis"); }
#line 1647 "parser.tab.c"
    break;

  case 61: /* expr: expr '*' expr  */
#line 412 "parser.y"
                            { (yyval.c).tipo = compativel((yyvsp[-2].c).tipo, (yyvsp[0].c).tipo) ? (yyvsp[-2].c).tipo : T_INDEFINIDO;
                               if ((yyval.c).tipo == T_INDEFINIDO) erro_semantico("operandos de '*' com tipos incompativeis"); }
#line 1654 "parser.tab.c"
    break;

  case 62: /* expr: expr '/' expr  */
#line 414 "parser.y"
                            { (yyval.c).tipo = compativel((yyvsp[-2].c).tipo, (yyvsp[0].c).tipo) ? (yyvsp[-2].c).tipo : T_INDEFINIDO;
                               if ((yyval.c).tipo == T_INDEFINIDO) erro_semantico("operandos de '/' com tipos incompativeis"); }
#line 1661 "parser.tab.c"
    break;

  case 63: /* expr: expr EQ expr  */
#line 416 "parser.y"
                            { (yyval.c).tipo = T_BOOL; }
#line 1667 "parser.tab.c"
    break;

  case 64: /* expr: expr NE expr  */
#line 417 "parser.y"
                            { (yyval.c).tipo = T_BOOL; }
#line 1673 "parser.tab.c"
    break;

  case 65: /* expr: expr LT expr  */
#line 418 "parser.y"
                            { (yyval.c).tipo = T_BOOL; }
#line 1679 "parser.tab.c"
    break;

  case 66: /* expr: expr LE expr  */
#line 419 "parser.y"
                            { (yyval.c).tipo = T_BOOL; }
#line 1685 "parser.tab.c"
    break;

  case 67: /* expr: expr GT expr  */
#line 420 "parser.y"
                            { (yyval.c).tipo = T_BOOL; }
#line 1691 "parser.tab.c"
    break;

  case 68: /* expr: expr GE expr  */
#line 421 "parser.y"
                            { (yyval.c).tipo = T_BOOL; }
#line 1697 "parser.tab.c"
    break;

  case 69: /* expr: NOT expr  */
#line 422 "parser.y"
                            { (yyval.c).tipo = T_BOOL; }
#line 1703 "parser.tab.c"
    break;

  case 70: /* expr: '(' expr ')'  */
#line 423 "parser.y"
                            { (yyval.c) = (yyvsp[-1].c); }
#line 1709 "parser.tab.c"
    break;


#line 1713 "parser.tab.c"

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

#line 426 "parser.y"


void yyerror(const char *s) {
    fprintf(stderr, "[ERRO SINTATICO] linha %d: %s\n", linha_atual, s);
    erros_encontrados++;
}
