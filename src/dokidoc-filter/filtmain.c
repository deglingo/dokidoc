/* filtmain.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/libdokidoc.h"
#include <string.h>



enum
  {
    CMD_INPUT,
    CMD_OUTPUT,
  };



/* main:
 */
gint main ( gint argc,
            gchar **argv )
{
  gint cmd;
  gchar *cfgfile;
  DokConfig *config;
  CL_DEBUG("hello!");
  /* command line */
  ASSERT(argc == 3);
  if (!strcmp(argv[1], "-i")) {
    cmd = CMD_INPUT;
  } else if (!strcmp(argv[1], "-o")) {
    cmd = CMD_OUTPUT;
  } else {
    CL_ERROR("invalid option: '%s'", argv[1]);
  }
  cfgfile = argv[2];
  config = dok_config_load(cfgfile);
  return 0;
}
