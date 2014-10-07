/* scannerc.h -
 */

#ifndef _SCANNERC_H_
#define _SCANNERC_H_

#include "libdokidoc/libdokidoc.h"



typedef struct _Node Node;

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


#define YYSTYPE Node *
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
int yylex ( Node **lvalp,
            Loc *llocp,
            DokScanner *scanner );
void yyerror ( Loc *locp,
               DokScanner *scanner,
               char const * );



#endif
