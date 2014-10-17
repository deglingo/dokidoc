/* filtmain.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/libdokidoc.h"

#include <string.h>
#include <errno.h>



enum
  {
    CMD_READ,
    CMD_WRITE,
  };



/* process_read:
 */
static void process_read ( const gchar *tmplfile )
{
  FILE *f;
  CL_DEBUG("reading template: '%s'", tmplfile);
  if (!(f = fopen(tmplfile, "r")))
    {
      CL_DEBUG("ERROR: could not open file: '%s' (%s)", tmplfile, strerror(errno));
      return;
    }
  CL_ERROR("[TODO]");
}



/* main:
 */
gint main ( gint argc,
            gchar **argv )
{
  gchar *tmplfile;
  gint cmd;
  /* [FIXME] parse command line */
  ASSERT(argc == 3);
  tmplfile = argv[2];
  if (!strcmp(argv[1], "read"))
    cmd = CMD_READ;
  else if (!strcmp(argv[1], "write"))
    cmd = CMD_WRITE;
  else
    CL_ERROR("invalid command: %s", argv[1]);
  /* process */
  switch (cmd) {
  case CMD_READ: process_read(tmplfile); break;
  default: CL_ERROR("[TODO]");
  }
  return 0;
}
