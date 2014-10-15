/* doktreexmldumper.h -
 */

#ifndef _DOKTREEXMLDUMPER_H_
#define _DOKTREEXMLDUMPER_H_

#include "libdokidoc/doktreevisitor.h"
#include "libdokidoc/doktree.h"



DokVisitor *dok_tree_xml_dumper_new ( void );
xmlNodePtr dok_tree_xml_dumper_get_root ( DokVisitor *visitor );



#endif
