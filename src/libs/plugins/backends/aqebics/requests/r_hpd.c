/***************************************************************************
    begin       : Mon Mar 01 2004
    copyright   : (C) 2019 by Martin Preuss
    email       : martin@libchipcard.de

 ***************************************************************************
 *          Please see toplevel file COPYING for license details           *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif


#include "r_hpd_l.h"

#include "aqebics/aqebics_l.h"
#include "aqebics/msg/msg.h"
#include "aqebics/msg/keys.h"
#include "aqebics/msg/zip.h"
#include "aqebics/msg/xml.h"
#include "aqebics/client/user_l.h"

#include "aqebics/requests/r_download_l.h"

#include <gwenhywfar/base64.h>
#include <gwenhywfar/gui.h>
#include <gwenhywfar/httpsession.h>



int EBC_Provider_XchgHpdRequest(AB_PROVIDER *pro,
                                GWEN_HTTP_SESSION *sess,
                                AB_USER *u)
{
  int rv;
  GWEN_BUFFER *buf;

  buf=GWEN_Buffer_new(0, 1024, 0, 1);
  rv=EBC_Provider_XchgDownloadRequest(pro, sess, u, "HPD", buf, 0, NULL, NULL);
  if (rv<0 || rv>=300) {
    DBG_INFO(AQEBICS_LOGDOMAIN, "here (%d)", rv);
    GWEN_Buffer_free(buf);
    return rv;
  }
  else {
    xmlDocPtr orderDoc=NULL;
    xmlNodePtr root_node=NULL;
    xmlNodePtr node=NULL;
    xmlNodePtr nodeX=NULL;
    const char *s;
    uint32_t uflags=0;

    /* parse XML document */
    rv=EB_Xml_DocFromBuffer(GWEN_Buffer_GetStart(buf),
                            GWEN_Buffer_GetUsedBytes(buf),
                            &orderDoc);
    GWEN_Buffer_free(buf);
    if (rv<0) {
      DBG_INFO(AQEBICS_LOGDOMAIN, "here (%d)", rv);
      return rv;
    }

    /* get keys */
    root_node=xmlDocGetRootElement(orderDoc);

    /* get auth key */
    node=EB_Xml_GetNode(root_node, "ProtocolParams",
                        GWEN_PATH_FLAGS_NAMEMUSTEXIST);
    if (node==NULL) {
      DBG_ERROR(AQEBICS_LOGDOMAIN,
                "No ProtocolParams found");
      xmlFreeDoc(orderDoc);
      return GWEN_ERROR_BAD_DATA;
    }
    nodeX=EB_Xml_GetNode(node, "ClientDataDownload",
                         GWEN_PATH_FLAGS_NAMEMUSTEXIST);
    if (nodeX) {
      s=(const char *)xmlGetProp(nodeX, BAD_CAST "supported");
      if (s && strcasecmp(s, "true")==0) {
        DBG_INFO(AQEBICS_LOGDOMAIN, "Client data download supported");
        uflags|=EBC_USER_FLAGS_CLIENT_DATA_DOWNLOAD_SPP;
      }
    }

    nodeX=EB_Xml_GetNode(node, "PreValidation",
                         GWEN_PATH_FLAGS_NAMEMUSTEXIST);
    if (nodeX) {
      s=(const char *)xmlGetProp(nodeX, BAD_CAST "supported");
      if (s && strcasecmp(s, "true")==0) {
        DBG_INFO(AQEBICS_LOGDOMAIN, "Prevalidation supported");
        uflags|=EBC_USER_FLAGS_PREVALIDATION_SPP;
      }
    }

    nodeX=EB_Xml_GetNode(node, "Recovery",
                         GWEN_PATH_FLAGS_NAMEMUSTEXIST);
    if (nodeX) {
      s=(const char *)xmlGetProp(nodeX, BAD_CAST "supported");
      if (s && strcasecmp(s, "true")==0) {
        DBG_INFO(AQEBICS_LOGDOMAIN, "Recovery supported");
        uflags|=EBC_USER_FLAGS_RECOVERY_SPP;
      }
    }

    EBC_User_AddFlags(u, uflags);

    xmlFreeDoc(orderDoc);
    return 0;
  }


}



