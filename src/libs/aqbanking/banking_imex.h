/***************************************************************************
 $RCSfile$
 -------------------
 cvs         : $Id$
 begin       : Mon Mar 01 2004
 copyright   : (C) 2004 by Martin Preuss
 email       : martin@libchipcard.de

 ***************************************************************************
 * This file is part of the project "AqBanking".                           *
 * Please see toplevel file COPYING of that project for license details.   *
 ***************************************************************************/


#ifndef AQBANKING_BANKING_IMEX_H
#define AQBANKING_BANKING_IMEX_H

#include <aqbanking/imexporter.h>


#ifdef __cplusplus
extern "C" {
#endif


/** @addtogroup G_AB_IMEXPORTER
 */
/*@{*/


/** @name Plugin Handling
 *
 */
/*@{*/
/**
 * Returns a list2 of available importers and exporters.
 * You must free this list after using it via
 * @ref GWEN_PluginDescription_List2_freeAll.
 * Please note that a simple @ref GWEN_PluginDescription_List2_free would
 * not suffice, since that would only free the list but not the objects
 * stored within the list !
 * @param ab pointer to the AB_BANKING object
 */
AQBANKING_API
GWEN_PLUGIN_DESCRIPTION_LIST2 *AB_Banking_GetImExporterDescrs(AB_BANKING *ab);

/**
 * Loads an importer/exporter backend with the given name. You can use
 * @ref AB_Banking_GetImExporterDescrs to retrieve a list of available
 * im-/exporters.
 * AqBanking remains the owner of the object returned (if any), so you
 * <b>must not</b> free it.
 */
AQBANKING_API 
AB_IMEXPORTER *AB_Banking_GetImExporter(AB_BANKING *ab, const char *name);

/**
 * <p>
 * Loads all available profiles for the given importer/exporter.
 * This includes global profiles as well as local ones.
 * </p>
 * <p>
 * Local profiles overwrite global ones, allowing the user to customize the
 * profiles. Local profiles are expected in a folder below the user
 * local folder (e.g. "$HOME/.banking"). The local profile folder for the
 * CSV plugin is in "$HOME/.banking/imexporters/csv/profiles".
 * </p>
 * <p>
 * The GWEN_DB returned contains one group for every loaded profile. Every
 * group has the name of the profile it contains. Every group contains at
 * least a variable called <i>name</i> which contains the name of the
 * profile, too. The remaining content of each group is completely defined by
 * the importer/exporter.
 * </p>
 * <p>
 * You can use @ref GWEN_DB_GetFirstGroup and @ref GWEN_DB_GetNextGroup
 * to browse the profiles.
 * </p>
 * <p>
 * The caller becomes the new owner of the object returned (if any).
 * This makes him/her responsible for freeing it via
 *  @ref GWEN_DB_Group_free.
 * </p>
 * <p>
 * You can use any of the subgroups below the returned one as argument
 * to @ref AB_ImExporter_Import.
 * </p>
 * @param ab pointer to the AB_BANKING object
 * @param name name of the importer whose profiles are to be read
 */
AQBANKING_API
GWEN_DB_NODE *AB_Banking_GetImExporterProfiles(AB_BANKING *ab,
                                               const char *name);
/*@}*/


/**
 * This function tries to fill missing fields in a given imexporter context.
 * It tries to find the online banking accounts for all account info objects in
 * the context and copies missing information (like IBAN, BIC, owner name etc).
 *
 * @param ab pointer to the AB_BANKING object
 * @param iec pointer to the imexporter context to fill
 * @return 0 if all accounts were found, 1 if there was at least 1 unknown account
 */
AQBANKING_API
int AB_Banking_FillGapsInImExporterContext(AB_BANKING *ab, AB_IMEXPORTER_CONTEXT *iec);


AQBANKING_API
int AB_Banking_ExportToBuffer(AB_BANKING *ab,
			      AB_IMEXPORTER_CONTEXT *ctx,
			      const char *exporterName,
                              const char *profileName,
			      GWEN_BUFFER *buf,
			      uint32_t guiid);


/*@}*/ /* addtogroup */

#ifdef __cplusplus
}
#endif

#endif

