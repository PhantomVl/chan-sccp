
/*!
 * \file 	common.h
 * \brief 	SCCP Common Include File
 * \author 	Diederik de Groot <dkgroot [at] talon.nl>
 * \note	This file is automatically generated by configure
 * \note 	Do not change this file. Change will be lost when running configure
 */

#    ifndef CHAN_SCCP_COMMON_H
#define CHAN_SCCP_COMMON_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include <sys/signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <assert.h>
#include <poll.h>

#include "sccp_lock.h"
#include "sccp_dllists.h"
#include "chan_sccp.h"
#include "sccp_event.h"
#include "pbx_impl/pbx_impl.h"

#include "sccp_pbx.h"
#include "sccp_protocol.h"
#include "sccp_socket.h"
#include "sccp_device.h"
#include "sccp_line.h"
#include "sccp_channel.h"
#include "sccp_features.h"
#include "sccp_utils.h"
#include "sccp_indicate.h"
#include "sccp_hint.h"
#include "sccp_actions.h"
#include "sccp_featureButton.h"
#include "sccp_mwi.h"
#include "sccp_config.h"
#include "sccp_conference.h"
#include "sccp_labels.h"
#include "sccp_softkeys.h"
#include "sccp_conference.h"
#include "sccp_features.h"
#include "sccp_adv_features.h"
#include "sccp_cli.h"
#include "sccp_appfunctions.h"
#include "sccp_management.h"
#include "sccp_rtp.h"

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif
#    endif										// CHAN_SCCP_COMMON_H
