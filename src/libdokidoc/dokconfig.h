/* dokconfig.h -
 */

#ifndef _DOKCONFIG_H_
#define _DOKCONFIG_H_

#include "libdokidoc/dokbase.h"



/* DokSourceBase:
 */
typedef struct _DokSourceBase
{
  gchar *path;
}
  DokSourceBase;



/* DokSourceFiler:
 */
typedef struct _DokSourceFile
{
  DokSourceBase *base;
  gchar *fullpath;
  gchar *basepath;
}
  DokSourceFile;



/* DokConfig:
 */
typedef struct _DokConfig
{
  GList *source_bases;
  GList *source_files;
}
  DokConfig;



DokConfig *dok_config_load ( const gchar *fname );



#endif
