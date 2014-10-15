/* dokconfig.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/dokconfig.h"



/* dok_config_load:
 */
DokConfig *dok_config_load ( const gchar *fname )
{
  DokConfig *config = g_new0(DokConfig, 1);
  CL_DEBUG("loading config file: '%s'", fname);
  return config;
}
