/* cpp.c -
 */

#include "cpp.h"
#include "lexer.h"



/* CPP:
 */
struct _CPP
{
  Lexer *lexer;
};



/* cpp_new:
 */
CPP *cpp_new ( const gchar *filename )
{
  CPP *cpp = g_new0(CPP, 1);
  cpp->lexer = lexer_new(filename);
  return cpp;
}



/* cpp_lex:
 */
void cpp_lex ( CPP *cpp )
{
  lexer_lex(cpp->lexer);
}
