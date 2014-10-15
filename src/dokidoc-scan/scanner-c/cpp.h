/* cpp.h -
 */

#ifndef _CPP_H_
#define _CPP_H_

#include "base.h"



typedef struct _CPP CPP;



CPP *cpp_new ( const gchar *filename );
Token *cpp_lex ( CPP *cpp,
                 DokLocation *loc );
void cpp_unlex ( CPP *cpp,
                 Token *token,
                 DokLocation *loc );



#endif
