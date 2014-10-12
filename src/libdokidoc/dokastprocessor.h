/* dokastdumper.h -
 */

#ifndef _DOKASTPROCESSOR_H_
#define _DOKASTPROCESSOR_H_

#include "libdokidoc/dokastvisitor.h"
#include "libdokidoc/doktree.h"



DokVisitor *dok_ast_processor_new ( void );
DokTree *dok_ast_processor_get_tree ( DokVisitor *processor );



#endif
