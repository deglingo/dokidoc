/* dokxmlwriter.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/dokxmlwriter.h"



/* DokXmlWriter:
 */
struct _DokXmlWriter
{
  FILE *file;
  GSList *context;
};



/* Node:
 */
typedef struct _Node
{
  gchar *name;
}
  Node;



static void _write ( DokXmlWriter *writer,
                     const gchar *text )
{
  fprintf(writer->file, "%s", text);
}



/* dok_xml_writer_new:
 */
DokXmlWriter *dok_xml_writer_new ( FILE *file )
{
  DokXmlWriter *writer = g_new0(DokXmlWriter, 1);
  writer->file = file;
  _write(writer, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"); /* [FIXME] */
  return writer;
}



/* dok_xml_writer_close:
 */
gboolean dok_xml_writer_close ( DokXmlWriter *writer )
{
  CL_ERROR("[TODO]");
  return FALSE;
}



/* dok_xml_writer_open_node:
 */
void dok_xml_writer_open_node ( DokXmlWriter *writer,
                                const gchar *name,
                                const gchar *first_attr,
                                ... )
{
  CL_ERROR("[TODO]");
}



/* dok_xml_writer_close_node:
 */
void dok_xml_writer_close_node ( DokXmlWriter *writer )
{
  CL_ERROR("[TODO]");
}
