/* dokast.h -
 */

#ifndef _DOKAST_H_
#define _DOKAST_H_

#include "libdokidoc/dokbase.h"



typedef enum _DokASTType DokASTType;
typedef union _DokAST DokAST;



/* DokASTType:
 */
enum _DokASTType
  {
    DOK_AST_TYPE_DUMMY,
  };



/* DokAST:
 */
union _DokAST
{
  DokASTType type;
};



#endif
