/***************************************************************************
 begin       : Thu Jan 16 2020
 copyright   : (C) 2020 by Martin Preuss
 email       : martin@libchipcard.de

 ***************************************************************************
 *          Please see toplevel file COPYING for license details           *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif


/* plugin headers */
#include "io_network.h"
#include "aqofxconnect/user.h"

/* aqbanking headers */
#include "aqbanking/i18n_l.h"
#include <aqbanking/backendsupport/httpsession.h>

/* gwenhywfar headers */
#include <gwenhywfar/gui.h>

/* system headers */
#include <stdio.h>
#include <errno.h>




/* ------------------------------------------------------------------------------------------------
 * forward declarations
 * ------------------------------------------------------------------------------------------------
 */



static int _createConnection(AB_PROVIDER *pro, AB_USER *u, GWEN_HTTP_SESSION **pSess);
static void _probablyWriteToLogFile(const char *sEnvVar, const char *sCaption, const uint8_t *p, uint32_t len);



/* ------------------------------------------------------------------------------------------------
 * implementations
 * ------------------------------------------------------------------------------------------------
 */



int AO_Provider_SendAndReceive(AB_PROVIDER *pro, AB_USER *u, const uint8_t *p, unsigned int plen, GWEN_BUFFER **pRbuf)
{
  GWEN_HTTP_SESSION *sess=NULL;
  GWEN_BUFFER *rbuf;
  int rv;
  const char *sEnvVar;

  sEnvVar=getenv("AQOFX_LOG_COMM");

  _probablyWriteToLogFile(sEnvVar, "Sending", p, plen);

  /* setup connection */
  rv=_createConnection(pro, u, &sess);
  DBG_ERROR(AQOFXCONNECT_LOGDOMAIN, "RBW Connect (%d)", rv);
  if (rv) {
    DBG_ERROR(AQOFXCONNECT_LOGDOMAIN, "Could not create connection");
    GWEN_Gui_ProgressLog2(0, GWEN_LoggerLevel_Error, I18N("Could not create connection (%d)"), rv);
    return rv;
  }

  /* send request */
  GWEN_Gui_ProgressLog(0, GWEN_LoggerLevel_Info, I18N("Sending request..."));
  rv=GWEN_HttpSession_SendPacket(sess, "POST", p, plen);
  DBG_ERROR(AQOFXCONNECT_LOGDOMAIN, "RBW POST (%d)", rv);
  if (rv<0) {
    DBG_ERROR(AQOFXCONNECT_LOGDOMAIN, "Error sending packet (%d)", rv);
    GWEN_Gui_ProgressLog(0, GWEN_LoggerLevel_Error, I18N("Network error while sending request"));
    GWEN_HttpSession_Fini(sess);
    GWEN_HttpSession_free(sess);
    return rv;
  }

  /* wait for response */
  GWEN_Gui_ProgressLog(0, GWEN_LoggerLevel_Info, I18N("Waiting for response..."));
  rbuf=GWEN_Buffer_new(0, 1024, 0, 1);
  rv=GWEN_HttpSession_RecvPacket(sess, rbuf);
  DBG_ERROR(AQOFXCONNECT_LOGDOMAIN, "RBW Recv (%d)", rv);
  if (rv<0) {
    DBG_ERROR(AQOFXCONNECT_LOGDOMAIN, "Error receiving packet (%d)", rv);
    GWEN_Gui_ProgressLog(0, GWEN_LoggerLevel_Error, I18N("Network error while waiting for response"));
    GWEN_Buffer_free(rbuf);
    GWEN_HttpSession_Fini(sess);
    GWEN_HttpSession_free(sess);
    return rv;
  }
  else if (!(rv>=200 && rv<=299)) {
    /* not a HTTP: ok code */
    DBG_INFO(AQOFXCONNECT_LOGDOMAIN, "here (%d)", rv);
    GWEN_Buffer_free(rbuf);
    GWEN_HttpSession_Fini(sess);
    GWEN_HttpSession_free(sess);
    return rv;
  }

  /* disconnect (ignore result) */
  GWEN_HttpSession_Fini(sess);
  GWEN_HttpSession_free(sess);

  /* found a response, transform it */
  *pRbuf=rbuf;

  _probablyWriteToLogFile(sEnvVar, "Received", (const uint8_t *) GWEN_Buffer_GetStart(rbuf),
                          GWEN_Buffer_GetUsedBytes(rbuf));

  return 0;
}



int _createConnection(AB_PROVIDER *pro, AB_USER *u, GWEN_HTTP_SESSION **pSess)
{
  int rv;
  GWEN_HTTP_SESSION *sess;
  const char *addr;
  const char *s;

  DBG_ERROR(AQOFXCONNECT_LOGDOMAIN, "RBW _createConnection");

  /* take bank addr from user */
  addr=AO_User_GetServerAddr(u);
  if (!(addr && *addr)) {
    DBG_ERROR(AQOFXCONNECT_LOGDOMAIN, "User has no valid address settings");
    return GWEN_ERROR_INVALID;
  }
  DBG_ERROR(AQOFXCONNECT_LOGDOMAIN, "RBW addr %s", addr);

  sess=AB_HttpSession_new(pro, u, addr, "https", 443);

  /* setup session */
  GWEN_HttpSession_AddFlags(sess, GWEN_HTTP_SESSION_FLAGS_NO_CACHE);

  GWEN_HttpSession_SetHttpContentType(sess, "application/x-ofx");

  DBG_ERROR(AQOFXCONNECT_LOGDOMAIN, "RBW HttpVMajor %d", AO_User_GetHttpVMajor(u));
  GWEN_HttpSession_SetHttpVMajor(sess, AO_User_GetHttpVMajor(u));
  DBG_ERROR(AQOFXCONNECT_LOGDOMAIN, "RBW HttpVMinor %d", AO_User_GetHttpVMinor(u));
  GWEN_HttpSession_SetHttpVMinor(sess, AO_User_GetHttpVMinor(u));

  s=AO_User_GetHttpUserAgent(u);
  DBG_ERROR(AQOFXCONNECT_LOGDOMAIN, "RBW userAgent %s", (s && *s)?s:"AqBanking")
  GWEN_HttpSession_SetHttpUserAgent(sess, (s && *s)?s:"AqBanking");

  /* init session */
  rv=GWEN_HttpSession_Init(sess);
  if (rv<0) {
    DBG_INFO(AQOFXCONNECT_LOGDOMAIN, "here (%d)", rv);
    GWEN_HttpSession_free(sess);
    return rv;
  }

  *pSess=sess;
  return 0;
}



void _probablyWriteToLogFile(const char *sEnvVar, const char *sCaption, const uint8_t *p, uint32_t len)
{
  if (sEnvVar) {
    FILE *f;

    if (strcasecmp(sEnvVar, "1")==0)
      sEnvVar="/tmp/ofx.log";
    DBG_ERROR(AQOFXCONNECT_LOGDOMAIN, "Saving OFX log to \"%s\" ...", sEnvVar);
    GWEN_Gui_ProgressLog2(0, GWEN_LoggerLevel_Warning, I18N("Saving communication log to %s"), sEnvVar);

    f=fopen(sEnvVar, "a+");
    if (!f) {
      DBG_ERROR(AQOFXCONNECT_LOGDOMAIN, "fopen: %s", strerror(errno));
    }
    else {
      fprintf(f, "\n\n%s:\n", sCaption);
      fprintf(f, "-------------------------------------\n");
      if (fwrite(p, len, 1, f)!=1) {
        DBG_ERROR(AQOFXCONNECT_LOGDOMAIN, "fwrite: %s", strerror(errno));
      }
      if (fclose(f)) {
        DBG_ERROR(AQOFXCONNECT_LOGDOMAIN, "fclose: %s", strerror(errno));
      }
    }
  }
}




