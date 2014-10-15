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


#define YYSTYPE DokAST *
#define YYLTYPE DokLocation

/* [fixme] how to handle filename ? */
#define YYLLOC_DEFAULT(cur, rhs, n) do {                    \
    if (n) {                                                \
      (cur).qfile = YYRHSLOC(rhs, 1).qfile;                 \
      (cur).lineno = YYRHSLOC(rhs, 1).lineno;               \
      (cur).charno = YYRHSLOC(rhs, 1).charno;               \
      (cur).end_qfile = YYRHSLOC(rhs, n).end_qfile;         \
      (cur).end_lineno = YYRHSLOC(rhs, n).end_lineno;       \
      (cur).end_charno = YYRHSLOC(rhs, n).end_charno;       \
    } else {                                                \
      (cur).qfile = (cur).end_qfile =                       \
        YYRHSLOC(rhs, 0).end_qfile;                         \
      (cur).lineno = (cur).end_lineno =                     \
        YYRHSLOC(rhs, 0).end_lineno;                        \
      (cur).charno = (cur).end_charno =                     \
        YYRHSLOC(rhs, 0).end_charno;                        \
    }                                                       \
  } while (0)

int yyparse ( DokScanner *scanner );
int yylex ( DokAST **lvalp,
            DokLocation *llocp,
            DokScanner *scanner );
void yyerror ( DokLocation *locp,
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
void add_decls ( DokScanner *scanner,
                 DokAST *decls );



#endif
