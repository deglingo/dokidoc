/* filtmain.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/libdokidoc.h"



/* process:
 */
static void process ( DokConfig *config )
{
  CL_DEBUG("[TODO] SGML...");
}



/* main:
 */
gint main ( gint argc,
            gchar **argv )
{
  gchar *cfgfile;
  DokConfig *config;
  libdokidoc_init();
  CL_DEBUG("hello!");
  /* [TODO] command line */
  ASSERT(argc == 2);
  cfgfile = argv[1];
  config = dok_config_load(cfgfile);
  process(config);
  return 0;
}
