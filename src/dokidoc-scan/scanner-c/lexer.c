/* lexer.c -
 */

#include "lexer.h"
#include "scannercgram.h"
#include <stdio.h>



/* [fixme] internal tokens */
enum
  {
    TOK_C_COMMENT = 1024,
  };



/* LexRule:
 */
typedef struct _LexRule
{
  gint token;
  gchar *pattern;
  GRegex *regex;
}
  LexRule;



/* LEX_RULES:
 */
static LexRule LEX_RULES[] =
  {
    { TOK_IDENT, "^[a-zA-Z_][a-zA-Z0-9_]*", },
    { 0, },
  };



/* Lexer:
 */
struct _Lexer
{
  gchar *filename;
  FILE *f;
  GString *buffer;
  gsize charno;
};



/* lex_rules_init:
 */
static void lex_rules_init ( void )
{
  static gboolean done = FALSE;
  LexRule *rule;
  GError *error = NULL;
  if (done)
    return;
  done = TRUE;
  for (rule = LEX_RULES; rule->token; rule++)
    {
      if (!(rule->regex = g_regex_new(rule->pattern, G_REGEX_OPTIMIZE, 0, &error)))
        CL_ERROR("regex error: %s", error->message);
    }  
}



/* lexer_new:
 */
Lexer *lexer_new ( const gchar *filename )
{
  Lexer *lexer = g_new0(Lexer, 1);
  lex_rules_init();
  lexer->filename = g_strdup(filename);
  if (!(lexer->f = fopen(filename, "r")))
    CL_ERROR("could not open file: '%s'", filename);
  lexer->buffer = g_string_new("");
  return lexer;
}



/* _feed_buffer:
 */
static gboolean _feed_buffer ( Lexer *lexer )
{
  gint c;
  g_string_truncate(lexer->buffer, 0);
  lexer->charno = 0;
  while (1)
    {
      c = fgetc(lexer->f);
      if (c == 0)
        {
          CL_ERROR("nul byte in file");
        }
      else if (c == '\f' || c == '\r')
        {
          CL_ERROR("[TODO] char 0x%x", c);
        }
      else if (c == EOF)
        {
          if (lexer->charno == 0)
            return FALSE;
          else
            return TRUE;          
        }
      else if (c == '\n')
        {
          return TRUE;
        }
      else if (c == '\\')
        {
          c = fgetc(lexer->f);
          if (c == EOF) {
            CL_ERROR("[TODO]");
          } else if (c == '\n') {
            g_string_append_c(lexer->buffer, ' ');
          } else {
            g_string_append_c(lexer->buffer, '\\');
            g_string_append_c(lexer->buffer, c);
          }
        }
      else
        {
          g_string_append_c(lexer->buffer, c);
        }
    }
}



/* lexer_lex:
 */
Token *lexer_lex ( Lexer *lexer )
{
  while (1)
    {
      /* feed the line buffer */
      while (lexer->charno >= lexer->buffer->len)
        if (!_feed_buffer(lexer))
          return NULL;
      CL_DEBUG("[TODO]");
      return NULL;
    }
}
