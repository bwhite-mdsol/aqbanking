/***************************************************************************
 begin       : Sun Aug 04 2019
 copyright   : (C) 2019 by Martin Preuss
 email       : martin@libchipcard.de

 ***************************************************************************
 * This file is part of the project "AqBanking".                           *
 * Please see toplevel file COPYING of that project for license details.   *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "sessionlayer/session.h"




int AQFINTS_Session_PinTan_PrepareSignSeg(AQFINTS_SESSION *sess,
                                          const AQFINTS_KEYNAME *keyName,
                                          const char *ctrlRef,
                                          GWEN_DB_NODE *cfg)
{
  char sdate[9];
  char stime[7];
  struct tm *lt;
  time_t tt;

  /* some preparations */
  tt=time(0);
  lt=localtime(&tt);

  /* create date */
  if (!strftime(sdate, sizeof(sdate), "%Y%m%d", lt)) {
    DBG_INFO(AQHBCI_LOGDOMAIN, "Date string too long");
    return GWEN_ERROR_INTERNAL;
  }
  /* create time */
  if (!strftime(stime, sizeof(stime), "%H%M%S", lt)) {
    DBG_INFO(AQHBCI_LOGDOMAIN, "Date string too long");
    return GWEN_ERROR_INTERNAL;
  }

  /* store info */
  GWEN_DB_SetIntValue(cfg, GWEN_DB_FLAGS_DEFAULT, "function", 2);
  GWEN_DB_SetCharValue(cfg, GWEN_DB_FLAGS_DEFAULT, "ctrlref", ctrlRef);
  GWEN_DB_SetIntValue(cfg, GWEN_DB_FLAGS_DEFAULT, "area", 1);
  GWEN_DB_SetIntValue(cfg, GWEN_DB_FLAGS_DEFAULT, "role", 1);

  GWEN_DB_SetIntValue(cfg, GWEN_DB_FLAGS_DEFAULT, "SecDetails/dir", 1); /* 1 client, 2=server */
  GWEN_DB_SetCharValue(cfg, GWEN_DB_FLAGS_DEFAULT, "SecDetails/secId", AQFINTS_KeyName_GetSystemId(keyName));
  GWEN_DB_SetIntValue(cfg, GWEN_DB_FLAGS_DEFAULT, "signseq", AQFINTS_KeyName_GetSignatureCounter(keyName));

  GWEN_DB_SetCharValue(cfg, GWEN_DB_FLAGS_DEFAULT, "SecStamp/date", sdate);
  GWEN_DB_SetCharValue(cfg, GWEN_DB_FLAGS_DEFAULT, "SecStamp/time", stime);

  /* hashAlgo */
  GWEN_DB_SetIntValue(cfg, GWEN_DB_FLAGS_DEFAULT, "hashAlgo/purpose", 1);
  GWEN_DB_SetIntValue(cfg, GWEN_DB_FLAGS_DEFAULT, "hashAlgo/algo", 999);
  GWEN_DB_SetIntValue(cfg, GWEN_DB_FLAGS_DEFAULT, "hashAlgo/pname", 1);

  /* signAlgo */
  GWEN_DB_SetIntValue(cfg, GWEN_DB_FLAGS_DEFAULT, "signAlgo/purpose", 6);
  GWEN_DB_SetIntValue(cfg, GWEN_DB_FLAGS_DEFAULT, "signAlgo/algo", 10);
  GWEN_DB_SetIntValue(cfg, GWEN_DB_FLAGS_DEFAULT, "signAlgo/mode", 16);


  GWEN_DB_SetIntValue(cfg, GWEN_DB_FLAGS_DEFAULT, "key/country", AQFINTS_KeyName_GetCountry(keyName));
  GWEN_DB_SetCharValue(cfg, GWEN_DB_FLAGS_DEFAULT, "key/bankcode", AQFINTS_KeyName_GetBankCode(keyName));
  GWEN_DB_SetCharValue(cfg, GWEN_DB_FLAGS_DEFAULT, "key/userid", AQFINTS_KeyName_GetUserId(keyName));
  GWEN_DB_SetCharValue(cfg, GWEN_DB_FLAGS_DEFAULT, "key/keytype", AQFINTS_KeyName_GetKeyType(keyName));
  GWEN_DB_SetIntValue(cfg, GWEN_DB_FLAGS_DEFAULT, "key/keynum", AQFINTS_KeyName_GetKeyNumber(keyName));
  GWEN_DB_SetIntValue(cfg, GWEN_DB_FLAGS_DEFAULT, "key/keyversion", AQFINTS_KeyName_GetKeyVersion(keyName));

  GWEN_DB_SetCharValue(cfg, GWEN_DB_FLAGS_DEFAULT, "secProfile/code", AQFINTS_Session_SetSecProfileCode(session));
  GWEN_DB_SetIntValue(cfg, GWEN_DB_FLAGS_DEFAULT, "secProfile/version", AQFINTS_Session_SetSecProfileCode(session));

  return 0;
}

