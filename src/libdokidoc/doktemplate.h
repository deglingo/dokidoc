/* doktemplate.h -
 */

#ifndef _DOKTEMPLATE_H_
#define _DOKTEMPLATE_H_

#include "libdokidoc/dokbase.h"



/* DokTemplate:
 */
typedef struct _DokTemplate
{
  int dummy;
}
  DokTemplate;



DokTemplate *dok_template_new ( const gchar *tmplfile );



#endif
