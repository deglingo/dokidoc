/* doktemplate.h -
 */

#ifndef _DOKTEMPLATE_H_
#define _DOKTEMPLATE_H_

#include "libdokidoc/dokbase.h"



/* DokTemplate:
 */
typedef struct _DokTemplate
{
  gchar *path;
  GPtrArray *nodes;
}
  DokTemplate;



DokTemplate *dok_template_new ( const gchar *tmplfile );
void dok_template_add_node ( DokTemplate *tmpl,
                             const gchar *type,
                             const gchar *fqname );
void dok_template_dump ( DokTemplate *tmpl,
                         FILE *f );



#endif
