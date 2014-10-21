/* dokbase.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/dokbase.h"
#include "libdokidoc/doktype.h"
#include "libdokidoc/dokast.h"

#include <string.h>
#include <errno.h>
#include <glib/gstdio.h>



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



/* dok_mkdir:
 */
void dok_mkdir ( const gchar *dirname )
{
  if (g_file_test(dirname, G_FILE_TEST_EXISTS))
    {
      if (!g_file_test(dirname, G_FILE_TEST_IS_DIR))
        CL_ERROR("mkdir: file exists but is ot a directory: '%s'", dirname);
    }
  else
    {
      gchar *parent = g_path_get_dirname(dirname);
      dok_mkdir(parent);
      if (g_mkdir(dirname, 0755) != 0)
        CL_ERROR("could not create directory '%s' : %s", parent, strerror(errno));
      g_free(parent);
    }
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
