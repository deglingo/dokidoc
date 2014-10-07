/* scannercgram.y -
 */

%{
  #include "scannerc.h"
%}

%define api.pure
%locations
%parse-param { DokScanner *scanner }
%lex-param { DokScanner *scanner }

%token TOK_IDENT

%%

input: /* empty */
     | TOK_IDENT input
;

%%
