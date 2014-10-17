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
  gchar *xmlpath;
}
  DokSourceFile;



/* DokConfig:
 */
typedef struct _DokConfig
{
  gchar *xmldir;
  GList *source_bases;
  GList *source_files;
  gchar *filter;
}
  DokConfig;



DokConfig *dok_config_load ( const gchar *fname );



#endif
