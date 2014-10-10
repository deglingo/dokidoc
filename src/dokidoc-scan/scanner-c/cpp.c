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
};



/* cpp_new:
 */
CPP *cpp_new ( const gchar *filename )
{
  CPP *cpp = g_new0(CPP, 1);
  cpp->lexer = lexer_new(filename);
  return cpp;
}



/* _parse_directive:
 */
static void _parse_directive ( CPP *cpp )
{
  Token *tok;
  /* skip whites */
  do {
    tok = lexer_lex(cpp->lexer);
  } while (tok->type == TOK_SPACE);
  ASSERT(tok->type == TOK_IDENT);
  if (!strcmp(tok->value, "include"))
    {
      /* [FIXME] */
      tok = lexer_lex_include(cpp->lexer);
      ASSERT(tok->type == TOK_INCLUDE);
      CL_DEBUG("include: '%s'", tok->value);
      do {
        tok = lexer_lex(cpp->lexer);
        ASSERT(tok->type == TOK_NL || tok->type == TOK_COMMENT);
      } while (tok->type != TOK_NL);
    }
  else
    {
      CL_DEBUG("[TODO] unknown directive: '%s'", tok->value);
      do {
        tok = lexer_lex(cpp->lexer);
      } while (tok->type != TOK_NL);
    }
}



/* cpp_lex:
 */
Token *cpp_lex ( CPP *cpp )
{
  Token *tok;
  if (cpp->unlex_token)
    {
      tok = cpp->unlex_token;
      cpp->unlex_token = NULL;
      return tok;
    }
  while (1)
    {
      if (!(tok = lexer_lex(cpp->lexer)))
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
                 Token *token )
{
  /* [fixme] */
  ASSERT(!cpp->unlex_token);
  cpp->unlex_token = token;
}
