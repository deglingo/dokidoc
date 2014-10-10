/* base.h -
 */

#ifndef _BASE_H_
#define _BASE_H_

#include "libdokidoc/libdokidoc.h"
#include <clog.h>



#define ASSERT(expr) do {                           \
    if (!(expr)) {                                  \
      CL_ERROR("ASSERTION FAILED: `" #expr "'");    \
    }                                               \
  } while (0)
typedef struct _Token Token;



/* Token:
 */
struct _Token
{
  gint type;
  gchar *value;
};



#endif
