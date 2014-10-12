/* dokbase.h -
 */

#ifndef _DOKBASE_H_
#define _DOKBASE_H_

#include <glib.h>
#include <stdio.h>

#include <libxml/tree.h>



typedef struct _DokLocation
{
  GQuark qfile;
  gint lineno;
  gint end_lineno;
}
  DokLocation;



void libdokidoc_init ( void );



#endif
