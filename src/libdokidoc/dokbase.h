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

void dok_mkdir ( const gchar *dirname );

gsize g_printf_string_upper_boundf ( const gchar *format,
                                     ... )
  G_GNUC_PRINTF(1, 2);

#define DOK_APRINTF(dest, format, v...) do {    \
    gsize _dok_aprintf_size =                   \
      g_printf_string_upper_boundf(format, v);  \
    *dest = g_alloca(_dok_aprintf_size+1);      \
    sprintf(*dest, format, v);                  \
  } while (0)



#endif
