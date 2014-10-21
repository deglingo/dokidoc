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



/* DokTmplFile:
 */
typedef struct _DokTmplFile
{
  gchar *tmplbase;
  gchar *tmplpath;
  GList *sources;
}
  DokTmplFile;



/* DokSourceFiler:
 */
typedef struct _DokSourceFile
{
  DokSourceBase *base;
  gchar *fullpath;
  gchar *basepath;
  gchar *xmlpath;
  DokTmplFile *template;
}
  DokSourceFile;



/* DokConfig:
 */
typedef struct _DokConfig
{
  gchar *xmldir;
  gchar *tmpldir;
  GList *source_bases;
  GList *source_files;
  GList *templates;
  gchar *filter;
}
  DokConfig;



DokConfig *dok_config_load ( const gchar *fname );



#endif
