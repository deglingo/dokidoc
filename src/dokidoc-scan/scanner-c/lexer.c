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
    LEXER_STATE_INCLUDE,
    LEXER_STATE_COMMENT,
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
    { TOK_C_COMMENT_END, "^([^\\\\]|(\\\\.))*\\*/", LEXER_STATE_COMMENT, },
    { TOK_C_COMMENT_CONT, "^.*", LEXER_STATE_COMMENT, },
    { TOK_INCLUDE, "^[ \t]*((\"[^\"]+\")|(<[^>]+>))[ \t]*", LEXER_STATE_INCLUDE, },
    { TOK_NL, "^\\n", },
    { TOK_SPACE, "^([ \t]+)|([ \t]*\\\\\\n)", },
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
  gsize charno;
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
  if (!(lexer->f = fopen(filename, "r")))
    CL_ERROR("could not open file: '%s'", filename);
  lexer->buffer = g_string_new("");
  lexer->comment = g_string_new("");
  return lexer;
}



/* lexer_get_qfile:
 */
GQuark lexer_get_qfile ( Lexer *lexer )
{
  return lexer->qfilename;
}



/* _feed_buffer:
 */
static gboolean _feed_buffer ( Lexer *lexer )
{
  gint c;
  if (lexer->charno < lexer->buffer->len)
    return TRUE;
  g_string_truncate(lexer->buffer, 0);
  lexer->charno = 0;
  lexer->lineno++;
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
          CL_DEBUG("FEED: %d: '%s'", lexer->lineno, lexer->buffer->str);
          g_string_append_c(lexer->buffer, '\n');
          return TRUE;
        }
      else
        {
          g_string_append_c(lexer->buffer, c);
        }
    }
}



/* eat_c_comment:
 */
static Token *eat_c_comment ( Lexer *lexer )
{
  LexRule *rule;
  gint tok_len;
  while (1)
    {
      if (!_feed_buffer(lexer))
        CL_ERROR("EOF in comment");
      rule = lex_rule_match(LEXER_STATE_COMMENT,
                            lexer->buffer->str,
                            lexer->buffer->len - lexer->charno,
                            &tok_len);
      ASSERT(rule);
      g_string_append_len(lexer->comment, lexer->buffer->str + lexer->charno, tok_len);
      lexer->charno += tok_len;
      if (rule->token == TOK_C_COMMENT_END)
        {
          g_string_truncate(lexer->comment, lexer->comment->len - 2);
          gchar *str = g_strstrip(lexer->comment->str);
          Token *tok = token_new(TOK_COMMENT, str, strlen(str));
          g_string_truncate(lexer->comment, 0);
          return tok;
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
  Token *tok;
  /* feed the line buffer */
  if (!_feed_buffer(lexer))
    {
      ASSERT(lexer->state == LEXER_STATE_NORMAL);
      CL_DEBUG("LEX: EOF");
      /* ?? */
      loc->qfile = loc->end_qfile = lexer->qfilename;
      loc->lineno = loc->end_lineno = lexer->lineno;
      loc->charno = loc->end_charno = lexer->charno; /* -1 ? */
      return NULL;
    }
  /* loc start */
  loc->qfile = lexer->qfilename;
  loc->lineno = lexer->lineno;
  loc->charno = lexer->charno;
  /* match rule */
  if (!(rule = lex_rule_match(lexer->state,
                              lexer->buffer->str + lexer->charno,
                              lexer->buffer->len - lexer->charno,
                              &tok_len)))
    {
      CL_ERROR("no match: `%s' (state=%d)", lexer->buffer->str + lexer->charno, lexer->state);
    }
  lexer->charno += tok_len;
  switch (rule->token)
    {
    case TOK_C_COMMENT_START:
      tok = eat_c_comment(lexer);
      break;
    default:
      tok = token_new(rule->token, lexer->buffer->str + loc->charno, tok_len);
      break;
    }
  loc->end_qfile = lexer->qfilename;
  loc->end_lineno = lexer->lineno;
  loc->end_charno = lexer->charno; /* -1 ? */
  fprintf(stderr, "LEX:%s:%d: TOKEN <%d> \"%s\" (%s:%d)\n",
          g_quark_to_string(loc->qfile), loc->lineno,
          tok->type, tok->value,
          g_quark_to_string(loc->end_qfile), loc->end_lineno);
  return tok;
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
