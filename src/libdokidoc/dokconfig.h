/* dokconfig.h -
 */

#ifndef _DOKCONFIG_H_
#define _DOKCONFIG_H_

#include "libdokidoc/dokbase.h"



/* DokConfig:
 */
typedef struct _DokConfig
{
  int dummy;
}
  DokConfig;



DokConfig *dok_config_load ( const gchar *fname );



#endif
