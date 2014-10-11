/* dokbase.c -
 */

#include "libdokidoc/dokbase.h"
#include "libdokidoc/dokast.h"



/* libdokidoc_init:
 */
void libdokidoc_init ( void )
{
  static gboolean done = FALSE;
  if (done)
    return;
  done = TRUE;
  _dok_ast_init();
}
