/* dokbase.h -
 */

#ifndef _DOKBASE_H_
#define _DOKBASE_H_

#include <glib.h>
#include <stdio.h>

#include <libxml/tree.h>
#include <libxml/xmlreader.h>



typedef struct _DokLocation
{
  GQuark qfile;
  gint lineno;
  gint charno;
  GQuark end_qfile;
  gint end_lineno;
  gint end_charno;
}
  DokLocation;



void libdokidoc_init ( void );



#endif
