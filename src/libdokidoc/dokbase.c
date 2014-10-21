/* dokbase.c -
 */

#include "libdokidoc/dokbase.h"
#include "libdokidoc/doktype.h"
#include "libdokidoc/dokast.h"



/* libdokidoc_init:
 */
void libdokidoc_init ( void )
{
  static gboolean done = FALSE;
  if (done)
    return;
  done = TRUE;
  _dok_type_init();
  _dok_ast_init();
}



/* g_printf_string_upper_boundf:
 */
gsize g_printf_string_upper_boundf ( const gchar *format,
                                     ... )
{
  gsize size;
  va_list args;
  va_start(args, format);
  size = g_printf_string_upper_bound(format, args);
  va_end(args);
  return size;
}
