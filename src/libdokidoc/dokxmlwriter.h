/* dokxmlwriter.h -
 */

#ifndef _DOKXMLWRITER_H_
#define _DOKXMLWRITER_H_

#include "libdokidoc/dokbase.h"



typedef struct _DokXmlWriter DokXmlWriter;



DokXmlWriter *dok_xml_writer_new ( FILE *file );
gboolean dok_xml_writer_close ( DokXmlWriter *writer );
void dok_xml_writer_open_node ( DokXmlWriter *writer,
                                const gchar *name,
                                const gchar *first_attr,
                                ... )
  G_GNUC_NULL_TERMINATED;
void dok_xml_writer_close_node ( DokXmlWriter *writer );



#endif
