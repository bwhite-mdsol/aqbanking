/**********************************************************
 * This file has been automatically created by "typemaker2"
 * from the file "oh_institute_spec.xml".
 * Please do not edit this file, all changes will be lost.
 * Better edit the mentioned source file instead.
 **********************************************************/

#ifndef OH_INSTITUTE_SPEC_OH_INSTITUTE_SPEC_H
#define OH_INSTITUTE_SPEC_OH_INSTITUTE_SPEC_H


#ifdef __cplusplus
extern "C" {
#endif

/** @page P_OH_INSTITUTE_SPEC Structure OH_INSTITUTE_SPEC
<p>This page describes the properties of OH_INSTITUTE_SPEC.</p>



<h1>OH_INSTITUTE_SPEC</h1>



@anchor OH_INSTITUTE_SPEC_id
<h2>id</h2>

<p>Set this property with @ref OH_InstituteSpec_SetId(), get it with @ref OH_InstituteSpec_GetId().</p>


@anchor OH_INSTITUTE_SPEC_name
<h2>name</h2>

<p>Set this property with @ref OH_InstituteSpec_SetName(), get it with @ref OH_InstituteSpec_GetName().</p>

*/

/* needed system headers */
#include <gwenhywfar/types.h>
#include <gwenhywfar/list1.h>
#include <gwenhywfar/list2.h>
#include <gwenhywfar/db.h>

/* pre-headers */
#include <aqofxconnect/aqofxconnect.h>

typedef struct OH_INSTITUTE_SPEC OH_INSTITUTE_SPEC;
GWEN_LIST_FUNCTION_DEFS(OH_INSTITUTE_SPEC, OH_InstituteSpec)
GWEN_LIST2_FUNCTION_DEFS(OH_INSTITUTE_SPEC, OH_InstituteSpec)



/* post-headers */
#include <aqofxconnect/libofxhome/oh_institute_spec_fns.h>


/** Constructor. */
OH_INSTITUTE_SPEC *OH_InstituteSpec_new(void);

/** Destructor. */
void OH_InstituteSpec_free(OH_INSTITUTE_SPEC *p_struct);

void OH_InstituteSpec_Attach(OH_INSTITUTE_SPEC *p_struct);

OH_INSTITUTE_SPEC *OH_InstituteSpec_dup(const OH_INSTITUTE_SPEC *p_struct);

OH_INSTITUTE_SPEC *OH_InstituteSpec_copy(OH_INSTITUTE_SPEC *p_struct, const OH_INSTITUTE_SPEC *p_src);

/** Getter.
 * Use this function to get the member "id" (see @ref OH_INSTITUTE_SPEC_id)
*/
int OH_InstituteSpec_GetId(const OH_INSTITUTE_SPEC *p_struct);

/** Getter.
 * Use this function to get the member "name" (see @ref OH_INSTITUTE_SPEC_name)
*/
const char *OH_InstituteSpec_GetName(const OH_INSTITUTE_SPEC *p_struct);

/** Setter.
 * Use this function to set the member "id" (see @ref OH_INSTITUTE_SPEC_id)
*/
void OH_InstituteSpec_SetId(OH_INSTITUTE_SPEC *p_struct, int p_src);

/** Setter.
 * Use this function to set the member "name" (see @ref OH_INSTITUTE_SPEC_name)
*/
void OH_InstituteSpec_SetName(OH_INSTITUTE_SPEC *p_struct, const char *p_src);

/* list1 functions */
OH_INSTITUTE_SPEC_LIST *OH_InstituteSpec_List_dup(const OH_INSTITUTE_SPEC_LIST *p_src);

void OH_InstituteSpec_ReadDb(OH_INSTITUTE_SPEC *p_struct, GWEN_DB_NODE *p_db);

int OH_InstituteSpec_WriteDb(const OH_INSTITUTE_SPEC *p_struct, GWEN_DB_NODE *p_db);

OH_INSTITUTE_SPEC *OH_InstituteSpec_fromDb(GWEN_DB_NODE *p_db);

int OH_InstituteSpec_toDb(const OH_INSTITUTE_SPEC *p_struct, GWEN_DB_NODE *p_db);

OH_INSTITUTE_SPEC *OH_InstituteSpec_List_GetById(const OH_INSTITUTE_SPEC_LIST *p_list, int p_cmp);

/* end-headers */


#ifdef __cplusplus
}
#endif

#endif

