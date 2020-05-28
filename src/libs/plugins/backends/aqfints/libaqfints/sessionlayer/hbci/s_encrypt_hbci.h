/***************************************************************************
 begin       : Sun Oct 27 2019
 copyright   : (C) 2019 by Martin Preuss
 email       : martin@libchipcard.de

 ***************************************************************************
 * This file is part of the project "AqBanking".                           *
 * Please see toplevel file COPYING of that project for license details.   *
 ***************************************************************************/

#ifndef AQFINTS_SESSION_ENCRYPT_HBCI_H
#define AQFINTS_SESSION_ENCRYPT_HBCI_H


#include "sessionlayer/session.h"
#include "parser/segment.h"




int AQFINTS_Session_EncryptMessageHbci(AQFINTS_SESSION *sess, AQFINTS_MESSAGE *message);


#endif

