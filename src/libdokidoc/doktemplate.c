/* doktemplate.c -
 */

#include "libdokidoc/doktemplate.h"



/* dok_template_new:
 */
DokTemplate *dok_template_new ( const gchar *tmplfile )
{
  DokTemplate *tmpl;
  tmpl = g_new0(DokTemplate, 1);
  return tmpl;
}
