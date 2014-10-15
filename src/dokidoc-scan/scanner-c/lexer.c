/* lexer.c -
 */

#include "lexer.h"
#include "scannercgram.h"
#include "scannerc.h"
#include <stdio.h>
#include <string.h>



/* LexerState:
 */
typedef enum _LexerState
  {
    LEXER_STATE_NORMAL = 0,
    LEXER_STATE_COMMENT,
    LEXER_STATE_INCLUDE,
  }
  LexerState;



/* LexRule:
 */
typedef struct _LexRule
{
  gint token;
  gchar *pattern;
  LexerState state;
  GRegex *regex;
}
  LexRule;



/* LEX_RULES:
 */
static LexRule LEX_RULES[] =
  {
    { TOK_C_COMMENT_START, "^/\\*", },
    { TOK_C_COMMENT_END, "^([^\\\\]|(\\\\.))*?\\*/", LEXER_STATE_COMMENT, },
    { TOK_C_COMMENT_CONT, "^([^\\\\]|(\\\\.))*", LEXER_STATE_COMMENT, },
    { TOK_INCLUDE, "^[ \t]*((\"[^\"]+\")|(<[^>]+>))[ \t]*", LEXER_STATE_INCLUDE, },
    { TOK_NL, "^\\n", },
    { TOK_SPACE, "^[ \t]+", },
    { TOK_SHARP, "^#", },
    { TOK_IDENT, "^[a-zA-Z_][a-zA-Z0-9_]*", },
    { TOK_INT, "^[0-9]+", },
    { TOK_STRING, "^\"([^\\\\]|(\\\\.))*\"", },
    { TOK_ELLIPSIS, "^\\.\\.\\.", },
    { TOK_ARROW, "^->", },
    { TOK_NE, "^!=", },
    { '(', "^\\(", },
    { ')', "^\\)", },
    { '{', "^\\{", },
    { '}', "^\\}", },
    { '[', "^\\[", },
    { ']', "^\\]", },
    { ';', "^;", },
    { '=', "^=", },
    { '&', "^&", },
    { '^', "^\\^", },
    { '!', "^!", },
    { '.', "^\\.", },
    { ',', "^,", },
    { '+', "^\\+", },
    { '-', "^-", },
    { '*', "^\\*", },
    { '<', "^<", },
    { '>', "^>", },
    { '?', "^\\?", },
    { ':', "^:", },
    { 0, },
  };



/* Lexer:
 */
struct _Lexer
{
  gchar *filename;
  GQuark qfilename;
  FILE *f;
  GString *buffer;
  gint lineno;
  gint next_lineno;
  gsize charno;
  gboolean in_comment;
  GString *comment;
  LexerState state;
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
        CL_ERROR("regex error: '%s' - %s", rule->pattern, error->message);
    }  
}



/* lex_rule_match:
 */
static LexRule *lex_rule_match ( LexerState state,
                                 const gchar *line,
                                 gsize len,
                                 gint *tok_len )
{
  LexRule *rule;
  GMatchInfo *match = NULL;
  GError *error = NULL;
  for (rule = LEX_RULES; rule->token; rule++)
    {
      if (rule->state != state)
        continue;
      if (g_regex_match_full(rule->regex, line, len, 0, 0, &match, &error))
        {
          gint start;
          g_match_info_fetch_pos(match, 0, &start, tok_len);
          ASSERT(start == 0);
          g_match_info_free(match);
          return rule;
        }
      else
        {
          g_match_info_free(match);
        }
    }
  return NULL;
}



/* token_new:
 */
static Token *token_new ( gint type,
                          const gchar *value,
                          gssize value_len )
{
  Token *tok = g_new0(Token, 1);
  tok->type = type;
  if (value)
    tok->value = g_strndup(value, value_len);
  else
    tok->value = NULL;
  return tok;
}



/* lexer_new:
 */
Lexer *lexer_new ( const gchar *filename )
{
  Lexer *lexer = g_new0(Lexer, 1);
  char *path;
  lex_rules_init();
  path = realpath(filename, NULL);
  lexer->filename = g_strdup(path);
  free(path);
  lexer->qfilename = g_quark_from_string(lexer->filename);
  lexer->next_lineno = 1;
  if (!(lexer->f = fopen(filename, "r")))
    CL_ERROR("could not open file: '%s'", filename);
  lexer->buffer = g_string_new("");
  lexer->comment = g_string_new("");
  return lexer;
}



/* _feed_buffer:
 */
static gboolean _feed_buffer ( Lexer *lexer )
{
  gint c;
  g_string_truncate(lexer->buffer, 0);
  lexer->charno = 0;
  lexer->lineno = lexer->next_lineno;
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
          lexer->next_lineno++;
          g_string_append_c(lexer->buffer, '\n');
          return TRUE;
        }
      else if (c == '\\')
        {
          c = fgetc(lexer->f);
          if (c == EOF) {
            CL_ERROR("[TODO]");
          } else if (c == '\n') {
            lexer->next_lineno++;
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
Token *lexer_lex ( Lexer *lexer,
                   DokLocation *loc )
{
  LexRule *rule;
  gint tok_len;
  gchar *buf;
  Token *tok;
  /* [fixme] */
  memset(loc, 0, sizeof(DokLocation));
  while (1)
    {
      gint charno;
      /* feed the line buffer */
      while (lexer->charno >= lexer->buffer->len)
        if (!_feed_buffer(lexer))
          {
            ASSERT(lexer->state == LEXER_STATE_NORMAL);
            return NULL;
          }
      /* match rule */
      if (!(rule = lex_rule_match(lexer->state,
                                  lexer->buffer->str + lexer->charno,
                                  lexer->buffer->len - lexer->charno,
                                  &tok_len)))
        {
          CL_ERROR("no match: `%s' (state=%d)", lexer->buffer->str + lexer->charno, lexer->state);
        }
      charno = lexer->charno;
      buf = lexer->buffer->str + lexer->charno;
      lexer->charno += tok_len;
      switch (rule->token)
        {
        case TOK_C_COMMENT_START:
          g_string_append_len(lexer->comment, buf, tok_len);
          lexer->state = LEXER_STATE_COMMENT;
          continue;
        case TOK_C_COMMENT_CONT:
          g_string_append_len(lexer->comment, buf, tok_len);
          continue;
        case TOK_C_COMMENT_END:
          g_string_append_len(lexer->comment, buf, tok_len);
          tok = token_new(TOK_COMMENT, lexer->comment->str, lexer->comment->len);
          g_string_truncate(lexer->comment, 0);
          lexer->state = LEXER_STATE_NORMAL;
          return tok;
        default:
          loc->qfile = loc->end_qfile = lexer->qfilename;
          loc->lineno = loc->end_lineno = lexer->lineno;
          loc->charno = charno;
          loc->end_charno = lexer->charno - 1;
          tok = token_new(rule->token, buf, tok_len);
          return tok;
        }
    }
}



/* lexer_lex_include:
 */
Token *lexer_lex_include ( Lexer *lexer,
                           DokLocation *loc )
{
  LexerState state = lexer->state;
  Token *tok;
  lexer->state = LEXER_STATE_INCLUDE;
  tok = lexer_lex(lexer, loc);
  ASSERT(tok);
  lexer->state = state;
  return tok;
}
