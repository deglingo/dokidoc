/* cpp.c -
 */

#include "cpp.h"
#include "lexer.h"
#include "scannerc.h"
#include "scannercgram.h"
#include <string.h>



/* CPP:
 */
struct _CPP
{
  Lexer *lexer;
  Token *unlex_token;
  DokLocation unlex_loc;
};



/* cpp_new:
 */
CPP *cpp_new ( const gchar *filename )
{
  CPP *cpp = g_new0(CPP, 1);
  cpp->lexer = lexer_new(filename);
  return cpp;
}



/* cpp_get_qfile:
 */
GQuark cpp_get_qfile ( CPP *cpp )
{
  return lexer_get_qfile(cpp->lexer);
}



/* _parse_directive:
 */
static void _parse_directive ( CPP *cpp )
{
  Token *tok;
  DokLocation loc = { 0, };
  /* skip whites */
  do {
    tok = lexer_lex(cpp->lexer, &loc);
  } while (tok->type == TOK_SPACE);
  ASSERT(tok->type == TOK_IDENT);
  if (!strcmp(tok->value, "include"))
    {
      /* [FIXME] */
      tok = lexer_lex_include(cpp->lexer, &loc);
      ASSERT(tok->type == TOK_INCLUDE);
      CL_DEBUG("include: '%s'", tok->value);
      do {
        tok = lexer_lex(cpp->lexer, &loc);
        ASSERT(tok->type == TOK_NL || tok->type == TOK_COMMENT);
      } while (tok->type != TOK_NL);
    }
  else
    {
      CL_DEBUG("[TODO] unknown directive: '%s'", tok->value);
      do {
        tok = lexer_lex(cpp->lexer, &loc);
      } while (tok->type != TOK_NL);
    }
}



/* cpp_lex:
 */
Token *cpp_lex ( CPP *cpp,
                 DokLocation *loc )
{
  Token *tok;
  if (cpp->unlex_token)
    {
      tok = cpp->unlex_token;
      *loc = cpp->unlex_loc;
      cpp->unlex_token = NULL;
      return tok;
    }
  while (1)
    {
      if (!(tok = lexer_lex(cpp->lexer, loc)))
        return NULL;
      switch (tok->type)
        {
        case TOK_COMMENT:
          return tok;
        case TOK_SHARP:
          /* [FIXME] check BOL */
          _parse_directive(cpp);
          continue;
        case TOK_NL:
        case TOK_SPACE:
          continue; /* [fixme] */
        case TOK_IDENT:
          /* [FIXME] expansion */
          if (!strcmp(tok->value, "G_GNUC_NULL_TERMINATED"))
            continue;
          else
            return tok;
        default:
          return tok;
        }
    }
}



/* cpp_unlex:
 */
void cpp_unlex ( CPP *cpp,
                 Token *token,
                 DokLocation *loc )
{
  /* [fixme] */
  ASSERT(!cpp->unlex_token);
  cpp->unlex_token = token;
  cpp->unlex_loc = *loc;
}
