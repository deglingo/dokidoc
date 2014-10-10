/* scannercgram.y -
 */

%{
  #include "scannerc.h"
  #include <clog.h>

/* #define TODO CL_ERROR("[TODO]") */
#define TODO
%}

%define api.pure
%locations
%parse-param { DokScanner *scanner }
%lex-param { DokScanner *scanner }

%token TOK_STATIC TOK_INLINE TOK_VOID TOK_STRUCT TOK_UNION TOK_ENUM TOK_CONST TOK_TYPEDEF
%token TOK_IDENT TOK_INT TOK_STRING TOK_ELLIPSIS
%token TOK_NE

%%

input
: /* empty */    { TODO; }
  | ext_decl input    { TODO; }
;

ext_decl
  : func_def    { TODO; }
  | decl    { TODO; }
  ;

func_def
  : decl_specs declarator '{' { eat_function_body(scanner); } '}'    { TODO; }
  ;

decl
  : decl_specs ';'    { TODO; }
  | decl_specs init_decl_list ';'    { TODO; }
  ;

decl_specs
  : storage_qual_spec_list type_spec    { TODO; }
  ;

storage_qual_spec_list
  : /* empty */    { TODO; }
  | storage_spec storage_qual_spec_list    { TODO; }
  | qual_spec storage_qual_spec_list    { TODO; }
  ;

storage_spec
  : TOK_STATIC    { TODO; }
  | TOK_INLINE    { TODO; }
  | TOK_TYPEDEF    { TODO; }
  ;

qual_spec
  : TOK_CONST    { TODO; }
  ;

type_spec
  : TOK_IDENT    { TODO; }
  | TOK_VOID    { TODO; }
  | struct_spec    { TODO; }
  | enum_spec    { TODO; }
  ;

struct_spec
  : struct_or_union TOK_IDENT    { TODO; }
  | struct_or_union TOK_IDENT '{' field_decl_list '}'    { TODO; }
  | struct_or_union '{' field_decl_list '}'    { TODO; }
  ;

struct_or_union
  : TOK_STRUCT    { TODO; }
  | TOK_UNION    { TODO; }
  ;

field_decl_list
  : field_decl    { TODO; }
  | field_decl field_decl_list    { TODO; }
  ;

field_decl
  // [FIXME] decl_specs should be specifier_qualifier_list
  : decl_specs field_declarator_list ';'    { TODO; }
  | error ';'    { TODO; }
  ;

field_declarator_list
  : field_declarator    { TODO; }
  | field_declarator ',' field_declarator_list    { TODO; }
  ;

field_declarator
  : declarator    { TODO; }
  | ':' TOK_INT    { TODO; } // [FIXME] expr    { TODO; }
  | declarator ':' TOK_INT    { TODO; }
  ;

enum_spec
  : TOK_ENUM TOK_IDENT    { TODO; }
  | TOK_ENUM TOK_IDENT '{' enumerator_list '}'    { TODO; }
  | TOK_ENUM '{' enumerator_list '}'    { TODO; }
  ;

enumerator_list
  : enumerator    { TODO; }
  | enumerator ',' enumerator_list    { TODO; }
  ;

enumerator
  : /* empty */    { TODO; }
  | TOK_IDENT    { TODO; }
  | TOK_IDENT '=' TOK_INT // [FIXME]    { TODO; }
  ;

declarator
  : direct_declarator    { TODO; }
  | pointer direct_declarator    { TODO; }
  ;

pointer
  : '*'    { TODO; }
  | '*' pointer    { TODO; }
  ;

direct_declarator
  : TOK_IDENT    { TODO; }
  | '(' declarator ')'    { TODO; }
  | direct_declarator '(' param_list ')'    { TODO; }
  ;

param_list
  : param_decl    { TODO; }
  | param_decl ',' param_list    { TODO; }
  ;

param_decl
  : TOK_ELLIPSIS    { TODO; }
  | decl_specs    { TODO; }
  | decl_specs declarator    { TODO; }
  ;

init_decl_list
  : init_decl    { TODO; }
  | init_decl ',' init_decl_list    { TODO; }
  ;

init_decl
  : declarator    { TODO; }
  | declarator '=' initializer    { TODO; }
  ;

// [FIXME]
initializer
  : TOK_IDENT    { TODO; }
  ;

%%
