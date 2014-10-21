/* filtmain.c -
 */

#include "libdokidoc/private.h"
#include "libdokidoc/libdokidoc.h"

#include <string.h>
#include <errno.h>



/* process:
 */
static void process ( DokConfig *config )
{
  gchar *dest = "sgml/book.sgml";
  FILE *f;
  xmlNodePtr root, title, chap;
  xmlDocPtr doc;
  CL_DEBUG("[TODO] SGML...");
  /* */
  root = xmlNewNode(NULL, BAD_CAST "book");
  xmlNewProp(root, BAD_CAST "xml:id", BAD_CAST "the_book");
  xmlNewProp(root, BAD_CAST "xmlns", BAD_CAST "http://docbook.org/ns/docbook");
  xmlNewProp(root, BAD_CAST "version", BAD_CAST "5.0");
  title = xmlNewChild(root, NULL, BAD_CAST "title", BAD_CAST "TITLE");
  chap = xmlNewChild(root, NULL, BAD_CAST "chapter", NULL);
  xmlNewProp(chap, BAD_CAST "xml:id", BAD_CAST "chap_1");
  xmlNewChild(chap, NULL, BAD_CAST "title", BAD_CAST "Chapter 1");
  xmlNewChild(chap, NULL, BAD_CAST "para", BAD_CAST "Paragraph 1");
  xmlNewChild(chap, NULL, BAD_CAST "para", BAD_CAST "Paragraph 2");
  /* */
  doc = xmlNewDoc(BAD_CAST "1.0");
  xmlDocSetRootElement(doc, root);
  xmlDocFormatDump(stderr, doc, 1);
  /* [FIXME] */
  dok_mkdir("./sgml");
  if (!(f = fopen(dest, "w")))
    CL_ERROR("could not open '%s' : %s", dest, strerror(errno));
  xmlDocFormatDump(f, doc, 1);
  fclose(f);
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
