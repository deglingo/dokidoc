/* scannercgram.y -
 */

%{
  #include "scannerc.h"
  #include <clog.h>

#define TODO CL_ERROR("[TODO]")
//#define TODO
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
  : /* empty */
  | unit           { add_decls(scanner, $1); }
  ;

unit
  : ext_decl          { $$ = $1; }
  | ext_decl unit     { $$ = dok_ast_list_merge($1, $2); }
  ;

ext_decl
  : func_def    { $$ = dok_ast_list_prepend(NULL, $1); }
  | decl        { $$ = $1; /* note: decl returns a list of decls */ }
  ;

func_def
  : decl_specs declarator '{' { eat_function_body(scanner); } '}'    { $$ = dok_ast_func_decl_new($1, $2); }
  ;

decl
  : decl_specs ';'                   { $$ = collect_decls(scanner, $1, NULL); }
  | decl_specs init_decl_list ';'    { $$ = collect_decls(scanner, $1, $2); }
  | TOK_TYPEDEF decl_specs declarator ';' { $$ = collect_typedef(scanner, $2, $3); }
  ;

decl_specs
  : storage_qual_spec_list type_spec    { $$ = fix_type($2, $1); }
  ;

storage_qual_spec_list
  : /* empty */                            { $$ = NULL; }
  | storage_spec storage_qual_spec_list    { $$ = dok_ast_list_prepend($2, $1); }
  | qual_spec storage_qual_spec_list       { $$ = dok_ast_list_prepend($2, $1); }
  ;

storage_spec
  : TOK_STATIC
  | TOK_INLINE
  ;

qual_spec
  : TOK_CONST
  ;

type_spec
  : TOK_IDENT    { $$ = dok_ast_type_name_new($1); }
  | TOK_VOID     { $$ = dok_ast_type_builtin_new(); }
  | struct_spec
  | enum_spec
  ;

struct_spec
  : struct_or_union TOK_IDENT                            { $$ = dok_ast_struct_new(); }
  | struct_or_union TOK_IDENT '{' field_decl_list '}'    { $$ = dok_ast_struct_new(); /* [FIXME] register decl */ }
  | struct_or_union '{' field_decl_list '}'    { TODO; }
  ;

struct_or_union
  : TOK_STRUCT
  | TOK_UNION
  ;

field_decl_list
  : field_decl                    { $$ = dok_ast_list_prepend(NULL, $1); }
  | field_decl field_decl_list    { $$ = dok_ast_list_prepend($2, $1); }
  ;

field_decl
  // [FIXME] decl_specs should be specifier_qualifier_list
  : decl_specs field_declarator_list ';'    { $$ = ($1 ? dok_ast_list_prepend($2, $1) : $2); }
  | error ';'                               { CL_DEBUG("ERROR: could not parse struct field"); $$ = NULL; }
  ;

field_declarator_list
  : field_declarator                              { $$ = dok_ast_list_prepend(NULL, $1); }
  | field_declarator ',' field_declarator_list    { $$ = dok_ast_list_prepend($3, $1); }
  ;

field_declarator
  : declarator
  | ':' TOK_INT    { TODO; } // [FIXME] expr    { TODO; }
  | declarator ':' TOK_INT    { $$ = $1; /* [FIXME] */ }
  ;

enum_spec
  : TOK_ENUM TOK_IDENT    { TODO; }
  | TOK_ENUM TOK_IDENT '{' enumerator_list '}'    { $$ = dok_ast_enum_new(); /* [FIXME] */ }
  | TOK_ENUM '{' enumerator_list '}'              { $$ = dok_ast_enum_new(); /* [FIXME] */ }
  ;

enumerator_list
  : enumerator                        { $$ = ($1 ? dok_ast_list_prepend(NULL, $1) : NULL); }
  | enumerator ',' enumerator_list    { $$ = ($1 ? dok_ast_list_prepend($3, $1) : $3); }
  ;

enumerator
  : /* empty */                         { $$ = NULL; }
  | TOK_IDENT
  | TOK_IDENT '=' TOK_INT // [FIXME]    { TODO; }
  ;

declarator
  : direct_declarator
  | pointer direct_declarator    { dok_declarator_set_pointer($2, $1); $$ = $2; }
  ;

pointer
  : '*'            { $$ = dok_ast_pointer_new(NULL); }
  | '*' pointer    { $$ = dok_ast_pointer_new($2); }
  ;

direct_declarator
  : TOK_IDENT                               { $$ = dok_ast_declarator_new($1); }
  | '(' declarator ')'                      { $$ = dok_ast_func_declarator_new($2); }
  | direct_declarator '(' param_list ')'    { dok_ast_declarator_set_param_list($1, $3); $$ = $1; }
  ;

param_list
  : param_decl                   { $$ = dok_ast_list_prepend(NULL, $1); }
  | param_decl ',' param_list    { $$ = dok_ast_list_prepend($3, $1); }
  ;

param_decl
  : TOK_ELLIPSIS             { $$ = dok_ast_param_decl_new(NULL, NULL); /* [FIXME] */ }
  | decl_specs               { $$ = dok_ast_param_decl_new($1, NULL); }
  | decl_specs declarator    { $$ = dok_ast_param_decl_new($1, $2); }
  ;

init_decl_list
  : init_decl                       { $$ = dok_ast_list_prepend(NULL, $1); }
  | init_decl ',' init_decl_list    { $$ = dok_ast_list_prepend($3, $1); }
  ;

init_decl
  : declarator
  | declarator '=' initializer    { $$ = $1; /* [FIXME] */ }
  ;

// [FIXME]
initializer
  : TOK_IDENT
  ;

%%
