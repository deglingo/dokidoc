/* lexer.h -
 */

#ifndef _LEXER_H_
#define _LEXER_H_

#include "base.h"



typedef struct _Lexer Lexer;
typedef struct _Token Token;



/* Token:
 */
struct _Token
{
  gint type;
  gchar *value;
};



Lexer *lexer_new ( const gchar *filename );
Token *lexer_lex ( Lexer *lexer );



#endif
