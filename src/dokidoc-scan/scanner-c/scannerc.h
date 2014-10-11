/* scannerc.h -
 */

#ifndef _SCANNERC_H_
#define _SCANNERC_H_

#include "libdokidoc/libdokidoc.h"



/* [fixme] internal tokens */
enum
  {
    TOK_C_COMMENT_START = 1024,
    TOK_C_COMMENT_CONT,
    TOK_C_COMMENT_END,
    TOK_COMMENT,
    TOK_INCLUDE,
    TOK_NL,
    TOK_SPACE,
    TOK_SHARP,
    TOK_ARROW,
  };



typedef struct _Loc
{
  gchar *filename;
  gssize lineno;
  gssize charno;
  gchar *end_filename;
  gssize end_lineno;
  gssize end_charno;
}
  Loc;


#define YYSTYPE DokAST *
#define YYLTYPE Loc

/* [fixme] how to handle filename ? */
#define YYLLOC_DEFAULT(cur, rhs, n) do {                    \
    if (n) {                                                \
      (cur).filename = YYRHSLOC(rhs, 1).filename;           \
      (cur).lineno = YYRHSLOC(rhs, 1).lineno;               \
      (cur).charno = YYRHSLOC(rhs, 1).charno;               \
      (cur).end_filename = YYRHSLOC(rhs, n).end_filename;   \
      (cur).end_lineno = YYRHSLOC(rhs, n).end_lineno;       \
      (cur).end_charno = YYRHSLOC(rhs, n).end_charno;       \
    } else {                                                \
      (cur).filename = (cur).end_filename =                 \
        YYRHSLOC(rhs, 0).end_filename;                      \
      (cur).lineno = (cur).end_lineno =                     \
        YYRHSLOC(rhs, 0).end_lineno;                        \
      (cur).charno = (cur).end_charno =                     \
        YYRHSLOC(rhs, 0).end_charno;                        \
    }                                                       \
  } while (0)

int yyparse ( DokScanner *scanner );
int yylex ( DokAST **lvalp,
            Loc *llocp,
            DokScanner *scanner );
void yyerror ( Loc *locp,
               DokScanner *scanner,
               char const * );

void eat_function_body ( DokScanner *scanner );
DokAST *collect_decls ( DokScanner *scanner,
                        DokAST *type_specs,
                        DokAST *declarators );
DokAST *collect_typedef ( DokScanner *scanner,
                          DokAST *type_specs,
                          DokAST *declarator );
DokAST *fix_type ( DokAST *type,
                   DokAST *specs );



#endif
