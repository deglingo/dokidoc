/* doktype.h -
 */

#ifndef _DOKTYPE_H_
#define _DOKTYPE_H_

#include "libdokidoc/dokbase.h"



typedef guint DokType;
typedef struct _DokTypeInfo DokTypeInfo;
typedef struct _DokTypeInstance DokTypeInstance;
typedef struct _DokTypeClass DokTypeClass;



/* DokTypeInfo:
 */
struct _DokTypeInfo
{
  guint instance_size;
  guint class_size;
};



/* DokTypeInstance:
 */
struct _DokTypeInstance
{
  DokTypeClass *dok_type_class;
};



/* DokTypeClass:
 */
struct _DokTypeClass
{
  DokType dok_type;
};



#define DOK_TYPE_INSTANCE_GET_TYPE(obj) (DOK_TYPE_CLASS_GET_TYPE(DOK_TYPE_INSTANCE_GET_CLASS(obj)))
#define DOK_TYPE_INSTANCE_GET_CLASS(obj) (((DokTypeInstance *)(obj))->dok_type_class)
#define DOK_TYPE_CLASS_GET_TYPE(cls) (((DokTypeClass *)(cls))->dok_type)

#define DOK_TYPE_INSTANCE_CAST(obj, type, s_type) ((s_type *)(dok_type_instance_cast((obj), (type))))
#define DOK_TYPE_INSTANCE_CHECK(obj, type) (dok_type_instance_check((obj), (type)))



void _dok_type_init ( void );

DokType dok_type_register ( const gchar *name,
                            DokType parent,
                            DokTypeInfo *info );
const gchar *dok_type_name ( DokType type );
DokType dok_type_from_name ( const gchar *name );
gboolean dok_type_isa ( DokType type1,
                        DokType type2 );

DokTypeInstance *dok_type_instance_new ( DokType type );
gpointer dok_type_instance_cast ( gpointer obj,
                                  DokType type );
gboolean dok_type_instance_check ( gpointer obj,
                                   DokType type );



#endif
