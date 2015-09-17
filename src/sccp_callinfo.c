/*!
 * \file        sccp_callinfo.c
 * \brief       SCCP CallInfo Class
 * \brief       SCCP CallInfo Header
 * \author      Diederik de Groot <ddegroot [at] users.sf.net>
 * \note        This program is free software and may be modified and distributed under the terms of the GNU Public License.
 *              See the LICENSE file at the top of the source tree.
 *
 * $date$
 * $revision$
 */

/*!
 * \remarks
 * Purpose:     SCCP CallInfo
 * When to use: 
 * Relations:   
 */

#include <config.h>
#include "common.h"
#include "sccp_utils.h"
#include <stdarg.h>

SCCP_FILE_VERSION(__FILE__, "$Revision$");

/*!
 * \brief SCCP CallInfo Structure
 */
struct sccp_callinfo {
	char calledPartyName[StationMaxNameSize];								/*!< Called Party Name */
	char calledPartyNumber[StationMaxDirnumSize];								/*!< Called Party Number */
	char cdpnVoiceMailbox[StationMaxDirnumSize];								/*!< Called Party Voicemail Box */

	char callingPartyName[StationMaxNameSize];								/*!< Calling Party Name */
	char callingPartyNumber[StationMaxDirnumSize];								/*!< Calling Party Number */
	char cgpnVoiceMailbox[StationMaxDirnumSize];								/*!< Calling Party Voicemail Box */

	char originalCalledPartyName[StationMaxNameSize];							/*!< Original Calling Party Name */
	char originalCalledPartyNumber[StationMaxDirnumSize];							/*!< Original Calling Party ID */
	char originalCdpnVoiceMailbox[StationMaxDirnumSize];							/*!< Original Called Party VoiceMail Box */

	char originalCallingPartyName[StationMaxNameSize];							/*!< Original Calling Party Name */
	char originalCallingPartyNumber[StationMaxDirnumSize];							/*!< Original Calling Party ID */

	char lastRedirectingPartyName[StationMaxNameSize];							/*!< Original Called Party Name */
	char lastRedirectingPartyNumber[StationMaxDirnumSize];							/*!< Original Called Party ID */
	char lastRedirectingVoiceMailbox[StationMaxDirnumSize];							/*!< Last Redirecting VoiceMail Box */

	char huntPilotName[StationMaxNameSize];									/*!< Hunt Pilot Name */
	char huntPilotNumber[StationMaxDirnumSize];								/*!< Hunt Pilot Number */

	uint32_t originalCdpnRedirectReason;									/*!< Original Called Party Redirect Reason */
	uint32_t lastRedirectingReason;										/*!< Last Redirecting Reason */
	int presentation;											/*!< Should this callerinfo be shown (privacy) */

	struct valid {
		unsigned int cdpnVoiceMailbox:1;								/*!< TRUE if the name information is valid/present */
		unsigned int calledParty:1;									/*!< TRUE if the name information is valid/present */
		unsigned int cgpnVoiceMailbox:1;								/*!< TRUE if the name information is valid/present */
		unsigned int callingParty:1;									/*!< TRUE if the name information is valid/present */
		unsigned int originalCdpnVoiceMailbox:1;							/*!< TRUE if the name information is valid/present */
		unsigned int originalCalledParty:1;								/*!< TRUE if the name information is valid/present */
		unsigned int originalCallingParty:1;								/*!< TRUE if the name information is valid/present */
		unsigned int lastRedirectingVoiceMailbox:1;							/*!< TRUE if the name information is valid/present */
		unsigned int lastRedirectingParty:1;								/*!< TRUE if the name information is valid/present */
		unsigned int huntPilot:1;									/*!< TRUE if the name information is valid/present */
	} valid;

	sccp_mutex_t lock;
};														/*!< SCCP CallInfo Structure */

enum sccp_callinfo_type {
	_CALLINFO_STRING,
	_CALLINFO_REASON,
	_CALLINFO_PRESENTATION,
};

struct sccp_callinfo_entry_type {
	const enum sccp_callinfo_type type;
	const int fieldOffset;
	const int validOffset;
} static const sccp_callinfo_entry_types[] = {
	[SCCP_CALLINFO_CALLEDPARTY_NAME] 		= {_CALLINFO_STRING, offsetof(sccp_callinfo_t,calledPartyName), -1},
	[SCCP_CALLINFO_CALLEDPARTY_NUMBER] 		= {_CALLINFO_STRING, offsetof(sccp_callinfo_t,calledPartyNumber), 1},
	[SCCP_CALLINFO_CALLEDPARTY_VOICEMAIL] 		= {_CALLINFO_STRING, offsetof(sccp_callinfo_t,cdpnVoiceMailbox), 0},
	[SCCP_CALLINFO_CALLINGPARTY_NAME] 		= {_CALLINFO_STRING, offsetof(sccp_callinfo_t,callingPartyName), -1},
	[SCCP_CALLINFO_CALLINGPARTY_NUMBER] 		= {_CALLINFO_STRING, offsetof(sccp_callinfo_t,callingPartyNumber), 3},
	[SCCP_CALLINFO_CALLINGPARTY_VOICEMAIL] 		= {_CALLINFO_STRING, offsetof(sccp_callinfo_t,cgpnVoiceMailbox), 2},
	[SCCP_CALLINFO_ORIG_CALLEDPARTY_NAME] 		= {_CALLINFO_STRING, offsetof(sccp_callinfo_t,originalCalledPartyName), -1},
	[SCCP_CALLINFO_ORIG_CALLEDPARTY_NUMBER] 	= {_CALLINFO_STRING, offsetof(sccp_callinfo_t,originalCalledPartyNumber), 5},
	[SCCP_CALLINFO_ORIG_CALLEDPARTY_VOICEMAIL] 	= {_CALLINFO_STRING, offsetof(sccp_callinfo_t,originalCdpnVoiceMailbox), 4},
	[SCCP_CALLINFO_ORIG_CALLINGPARTY_NAME] 		= {_CALLINFO_STRING, offsetof(sccp_callinfo_t,originalCallingPartyName), -1},
	[SCCP_CALLINFO_ORIG_CALLINGPARTY_NUMBER] 	= {_CALLINFO_STRING, offsetof(sccp_callinfo_t,originalCallingPartyNumber), 6},
	[SCCP_CALLINFO_LAST_REDIRECTINGPARTY_NAME] 	= {_CALLINFO_STRING, offsetof(sccp_callinfo_t,lastRedirectingPartyName), -1},
	[SCCP_CALLINFO_LAST_REDIRECTINGPARTY_NUMBER] 	= {_CALLINFO_STRING, offsetof(sccp_callinfo_t,lastRedirectingPartyNumber), 8},
	[SCCP_CALLINFO_LAST_REDIRECTINGPARTY_VOICEMAIL]	= {_CALLINFO_STRING, offsetof(sccp_callinfo_t,lastRedirectingVoiceMailbox), 7},
	[SCCP_CALLINFO_HUNT_PILOT_NAME] 		= {_CALLINFO_STRING, offsetof(sccp_callinfo_t,huntPilotName), -1},
	[SCCP_CALLINFO_HUNT_PILOT_NUMBER] 		= {_CALLINFO_STRING, offsetof(sccp_callinfo_t,huntPilotNumber), -1},
	[SCCP_CALLINFO_ORIG_CALLEDPARTY_REDIRECT_REASON]= {_CALLINFO_REASON, offsetof(sccp_callinfo_t,originalCdpnRedirectReason), -1},
	[SCCP_CALLINFO_LAST_REDIRECT_REASON] 		= {_CALLINFO_REASON, offsetof(sccp_callinfo_t,lastRedirectingReason), -1},
	[SCCP_CALLINFO_PRESENTATION] 			= {_CALLINFO_PRESENTATION, offsetof(sccp_callinfo_t,presentation), -1},
};

static const enum sccp_callinfo_range {
	sccp_callinfo_range_char,
	sccp_callinfo_range_int,
	sccp_callinfo_range_presentation,
} sccp_callinfo_ranges[] = {
	[SCCP_CALLINFO_CALLEDPARTY_NAME ... SCCP_CALLINFO_HUNT_PILOT_NUMBER] = sccp_callinfo_range_char,	/* yes this is valid ;-) */
	[SCCP_CALLINFO_ORIG_CALLEDPARTY_REDIRECT_REASON ... SCCP_CALLINFO_LAST_REDIRECT_REASON] = sccp_callinfo_range_int,
	[SCCP_CALLINFO_PRESENTATION] = sccp_callinfo_range_presentation,	
};

sccp_callinfo_t *const sccp_callinfo_ctor(void)
{
	sccp_callinfo_t *const ci = sccp_malloc(sizeof(sccp_callinfo_t));
	if (!ci) {
		pbx_log(LOG_ERROR, "SCCP: No memory to allocate callinfo object. Failing\n");
		return NULL;
	}
	sccp_mutex_init(&ci->lock);
	return ci;
}

sccp_callinfo_t *const sccp_callinfo_dtor(sccp_callinfo_t *ci)
{
	assert(ci != NULL);
	sccp_mutex_destroy(&ci->lock);
	sccp_free(ci);
	return NULL;
}

boolean_t sccp_callinfo_copy(const sccp_callinfo_t * const src_ci, sccp_callinfo_t * const dst_ci)
{
	/* observing locking order. not locking both callinfo objects at the same time, using a tmp as go between */
	if (src_ci && dst_ci) {
		sccp_callinfo_t tmp_ci = {{ 0 }};
		
	 	sccp_mutex_lock(&((sccp_callinfo_t * const)src_ci)->lock);
		memcpy(&tmp_ci, src_ci, sizeof(sccp_callinfo_t));
		sccp_mutex_unlock(&((sccp_callinfo_t * const)src_ci)->lock);

	 	sccp_mutex_lock(&dst_ci->lock);
		memcpy(dst_ci, &tmp_ci, sizeof(sccp_callinfo_t));
		sccp_mutex_unlock(&dst_ci->lock);
		
		return TRUE;
	}
	return FALSE;
}

int sccp_callinfo_set(sccp_callinfo_t * ci, sccp_callinfo_key_t key, ...) 
{
 	assert(ci != NULL);

 	sccp_callinfo_key_t curkey = SCCP_CALLINFO_NONE;
 	int changes = 0;

 	sccp_mutex_lock(&ci->lock);
	va_list ap;
	va_start(ap, key);
	
	for (curkey = key; curkey > SCCP_CALLINFO_NONE && curkey < SCCP_CALLINFO_KEY_SENTINEL; curkey = va_arg(ap, sccp_callinfo_key_t)) 
	{
		struct sccp_callinfo_entry_type entry = sccp_callinfo_entry_types[curkey];
		void *destPtr = ci + entry.fieldOffset;
		switch (entry.type) {
			case _CALLINFO_REASON:
				*(int*)destPtr = va_arg(ap, int);
				changes++;
				break;
				
			case _CALLINFO_PRESENTATION:
				*(sccp_calleridpresence_t*)destPtr = va_arg(ap, sccp_calleridpresence_t);
				changes++;
				break;
			case _CALLINFO_STRING:
				{
					uint valid = 0;
					char *value = va_arg(ap, char *);
					unsigned int *validPtr = entry.validOffset ? (((unsigned int*)&ci->valid) + entry.validOffset) : NULL;	// cast bitfieldpointer into array of uint

					if (!sccp_strlen_zero(value)) {
						valid = 1;
					}
					if (!strncmp(destPtr, value, StationMaxDirnumSize)) {
						sccp_copy_string(destPtr, va_arg(ap, char *), StationMaxDirnumSize);
						changes++;
						if (validPtr && *validPtr != valid) {
							*validPtr = valid;
							changes++;
						}
					}
				}
				break;
		}
	}
	
	va_end(ap);
 	sccp_mutex_unlock(&ci->lock);
	return changes;
}

gcc_inline static boolean_t sccp_callinfo_getStr(sccp_callinfo_t * ci, sccp_callinfo_key_t key, char **const value)
{
	//assert(ci != NULL);
	
	char *srcPtr = NULL;							/* use to set the destination of the value */
	unsigned int *validPtr = NULL;						/* array to validation bitfield */
	
	switch(key) {
		case SCCP_CALLINFO_CALLEDPARTY_VOICEMAIL:
			srcPtr = ci->cdpnVoiceMailbox;
			validPtr = (((unsigned int*)&ci->valid) + 0);		/* cast pointer to bitfiled into array */
			break;
		case SCCP_CALLINFO_CALLEDPARTY_NAME:
			srcPtr = ci->calledPartyName;
			break;
		case SCCP_CALLINFO_CALLEDPARTY_NUMBER:
			srcPtr = ci->calledPartyNumber;
			validPtr = (((unsigned int*)&ci->valid) + 1);
			break;
			
		case SCCP_CALLINFO_CALLINGPARTY_VOICEMAIL:
			srcPtr = ci->cgpnVoiceMailbox;
			validPtr = (((unsigned int*)&ci->valid) + 2);
			break;
		case SCCP_CALLINFO_CALLINGPARTY_NAME:
			srcPtr = ci->callingPartyName;
			break;
		case SCCP_CALLINFO_CALLINGPARTY_NUMBER:
			srcPtr = ci->callingPartyNumber;
			validPtr = (((unsigned int*)&ci->valid) + 3);
			break;
			
		case SCCP_CALLINFO_ORIG_CALLEDPARTY_VOICEMAIL:
			srcPtr = ci->cdpnVoiceMailbox;
			validPtr = (((unsigned int*)&ci->valid) + 4);
			break;
		case SCCP_CALLINFO_ORIG_CALLEDPARTY_NAME:
			srcPtr = ci->calledPartyName;
			break;
		case SCCP_CALLINFO_ORIG_CALLEDPARTY_NUMBER:
			srcPtr = ci->calledPartyNumber;
			validPtr = (((unsigned int*)&ci->valid) + 5);
			break;
			
		case SCCP_CALLINFO_ORIG_CALLINGPARTY_NAME:
			srcPtr = ci->callingPartyName;
			break;
		case SCCP_CALLINFO_ORIG_CALLINGPARTY_NUMBER:
			srcPtr = ci->callingPartyNumber;
			validPtr = (((unsigned int*)&ci->valid) + 6);
			break;
			
		case SCCP_CALLINFO_LAST_REDIRECTINGPARTY_VOICEMAIL:
			srcPtr = ci->lastRedirectingVoiceMailbox;
			validPtr = (((unsigned int*)&ci->valid) + 7);
			break;
		case SCCP_CALLINFO_LAST_REDIRECTINGPARTY_NAME:
			srcPtr = ci->lastRedirectingPartyName;
			break;
		case SCCP_CALLINFO_LAST_REDIRECTINGPARTY_NUMBER:
			srcPtr = ci->lastRedirectingPartyNumber;
			validPtr = (((unsigned int*)&ci->valid) + 8);
			break;

		case SCCP_CALLINFO_HUNT_PILOT_NAME:
			srcPtr = ci->huntPilotName;
			break;
		case SCCP_CALLINFO_HUNT_PILOT_NUMBER:
			srcPtr = ci->huntPilotNumber;
			validPtr = (((unsigned int*)&ci->valid) + 9);
			break;

		default:
			pbx_log(LOG_ERROR, "SCCP: (CallInfo_getStr) unknown key %d\n",key);
			return FALSE;
	}
	if (srcPtr) {
		if (validPtr) {
			if (*validPtr == 1) {
				sccp_copy_string(*value, srcPtr, StationMaxDirnumSize);
			} else {
				// not valid -> return empty string
				**value = '\0';
				return FALSE;
			}
		} else {
			sccp_copy_string(*value, srcPtr, StationMaxDirnumSize);
		}
	}
	return TRUE;
}

gcc_inline static boolean_t sccp_callinfo_getReason(sccp_callinfo_t * ci, sccp_callinfo_key_t key, int *const reason)
{
	//assert(ci != NULL);

	switch(key) {
		case SCCP_CALLINFO_ORIG_CALLEDPARTY_REDIRECT_REASON:
			*reason = ci->originalCdpnRedirectReason;
			break;
		case SCCP_CALLINFO_LAST_REDIRECT_REASON:
			*reason = ci->lastRedirectingReason;
			break;
		default:
			pbx_log(LOG_ERROR, "SCCP: (CallInfo_setStr) unknown key %d\n",key);
			return FALSE;
	}
	return TRUE;
}

gcc_inline static boolean_t sccp_callinfo_getPresentation(sccp_callinfo_t * ci, sccp_calleridpresence_t *const presentation)
{
	//assert(ci != NULL);
	*presentation = ci->presentation;
	return TRUE;
}

int sccp_callinfo_get(sccp_callinfo_t * ci, sccp_callinfo_key_t key, ...) 
{
 	assert(ci != NULL);

 	sccp_callinfo_key_t curkey = SCCP_CALLINFO_NONE;
 	int changes = 0;
 	
 	sccp_mutex_lock(&ci->lock);
	va_list ap;
	va_start(ap, key);
	for (curkey = key; curkey > SCCP_CALLINFO_NONE && curkey < SCCP_CALLINFO_KEY_SENTINEL; curkey = va_arg(ap, sccp_callinfo_key_t)) 
	{
		switch(sccp_callinfo_ranges[curkey]) {
			case sccp_callinfo_range_char:
				changes |= sccp_callinfo_getStr(ci, curkey, va_arg(ap, char **)) ? 1 : 0;
				break;
			case sccp_callinfo_range_int:
				changes |= sccp_callinfo_getReason(ci, curkey, va_arg(ap, int *)) ? 1 : 0;
				break;
			case sccp_callinfo_range_presentation:
				changes |= sccp_callinfo_getPresentation(ci, va_arg(ap, sccp_calleridpresence_t *))  ? 1 : 0;
				break;
		}
	}
	va_end(ap);
 	sccp_mutex_unlock(&ci->lock);
	return changes;
}

int sccp_callinfo_setCalledParty(sccp_callinfo_t * ci, const char name[StationMaxDirnumSize], const char number[StationMaxDirnumSize], const char voicemail[StationMaxDirnumSize])
{
	assert(ci != NULL);
	return sccp_callinfo_set(ci, 
		SCCP_CALLINFO_CALLEDPARTY_NAME, name, 
		SCCP_CALLINFO_CALLEDPARTY_NUMBER, number, 
		SCCP_CALLINFO_CALLEDPARTY_VOICEMAIL, voicemail, 
		SCCP_CALLINFO_KEY_SENTINEL);
}

int sccp_callinfo_setCallingParty(sccp_callinfo_t * ci, const char name[StationMaxDirnumSize], const char number[StationMaxDirnumSize], const char voicemail[StationMaxDirnumSize])
{
	assert(ci != NULL);
	return sccp_callinfo_set(ci, 
		SCCP_CALLINFO_CALLINGPARTY_NAME, name, 
		SCCP_CALLINFO_CALLINGPARTY_NUMBER, number, 
		SCCP_CALLINFO_CALLINGPARTY_VOICEMAIL, voicemail, 
		SCCP_CALLINFO_KEY_SENTINEL);
}

int sccp_callinfo_setOrigCalledParty(sccp_callinfo_t * ci, const char name[StationMaxDirnumSize], const char number[StationMaxDirnumSize], const char voicemail[StationMaxDirnumSize], const int reason)
{
	assert(ci != NULL);
	return sccp_callinfo_set(ci, 
		SCCP_CALLINFO_ORIG_CALLEDPARTY_NAME, name,
		SCCP_CALLINFO_ORIG_CALLEDPARTY_NUMBER, number,
		SCCP_CALLINFO_ORIG_CALLEDPARTY_VOICEMAIL, voicemail,
		SCCP_CALLINFO_ORIG_CALLEDPARTY_REDIRECT_REASON, reason,
		SCCP_CALLINFO_KEY_SENTINEL);
}

int sccp_callinfo_setOrigCallingParty(sccp_callinfo_t * ci, const char name[StationMaxDirnumSize], const char number[StationMaxDirnumSize])
{
	assert(ci != NULL);
	return sccp_callinfo_set(ci, 
		SCCP_CALLINFO_ORIG_CALLINGPARTY_NAME, name,
		SCCP_CALLINFO_ORIG_CALLINGPARTY_NUMBER, number,
		SCCP_CALLINFO_KEY_SENTINEL);
}

int sccp_callinfo_setLastRedirectingParty(sccp_callinfo_t * ci, const char name[StationMaxDirnumSize], const char number[StationMaxDirnumSize], const char voicemail[StationMaxDirnumSize], const int reason)
{
	assert(ci != NULL);
	return sccp_callinfo_set(ci, 
		SCCP_CALLINFO_LAST_REDIRECTINGPARTY_NAME, name,
		SCCP_CALLINFO_LAST_REDIRECTINGPARTY_NUMBER, number,
		SCCP_CALLINFO_LAST_REDIRECTINGPARTY_VOICEMAIL, voicemail,
		SCCP_CALLINFO_LAST_REDIRECT_REASON, reason,
		SCCP_CALLINFO_KEY_SENTINEL);
}

/*
boolean_t sccp_callinfo_sendCallInfo(sccp_callinfo_t *ci, constDevicePtr d)
{
	assert(ci != NULL && d != NULL);
	if (device->protocol && device->protocol->sendCallInfo) {
		instance = sccp_device_find_index_for_line(device, channel->line->name);
		device->protocol->sendCallInfo(device, channel, instance);
	}
	return TRUE;
}
*/

boolean_t sccp_callinfo_getCallInfoStr(sccp_callinfo_t *ci, pbx_str_t ** const buf)
{
	assert(ci != NULL);
 	sccp_mutex_lock(&ci->lock);
	pbx_str_append(buf, 0, " - calledParty: %s <%s>%s%s%s\n", ci->calledPartyName, ci->calledPartyNumber, (ci->cdpnVoiceMailbox) ? " voicemail: " : "", ci->cdpnVoiceMailbox, (ci->valid.calledParty) ? ", valid" : ", invalid");
	pbx_str_append(buf, 0, " - callingParty: %s <%s>%s%s%s\n", ci->callingPartyName, ci->callingPartyNumber, (ci->cgpnVoiceMailbox) ? " voicemail: " : "", ci->cgpnVoiceMailbox, (ci->valid.callingParty) ? ", valid" : ", invalid");
	pbx_str_append(buf, 0, " - originalCalledParty: %s <%s>%s%s%s, reason: %d\n", ci->originalCalledPartyName, ci->originalCalledPartyNumber, (ci->originalCdpnVoiceMailbox) ? " voicemail: " : "" , ci->originalCdpnVoiceMailbox, (ci->valid.originalCalledParty) ? ", valid" : ", invalid", ci->originalCdpnRedirectReason);
	pbx_str_append(buf, 0, " - originalCallingParty: %s <%s>%s\n", ci->originalCallingPartyName, ci->originalCallingPartyNumber, (ci->valid.originalCallingParty) ? ", valid" : ", invalid");
	pbx_str_append(buf, 0, " - lastRedirectingParty: %s <%s>%s%s%s, reason: %d\n", ci->lastRedirectingPartyName, ci->lastRedirectingPartyNumber, (ci->lastRedirectingVoiceMailbox) ? " voicemail: " : "", ci->lastRedirectingVoiceMailbox, (ci->valid.lastRedirectingParty) ? ", valid" : ", invalid", ci->lastRedirectingReason);
	pbx_str_append(buf, 0, " - huntPilot: %s <%s>%s\n", ci->huntPilotName, ci->huntPilotNumber, (ci->valid.huntPilot) ? ", valid" : ", invalid");
	pbx_str_append(buf, 0, " - presentation: %s\n\n", sccp_calleridpresence2str(ci->presentation));
 	sccp_mutex_unlock(&ci->lock);
	return TRUE;
}

void sccp_callinfo_print2log(sccp_callinfo_t *ci)
{
	assert(ci != NULL);
	pbx_str_t *buf = pbx_str_alloca(DEFAULT_PBX_STR_BUFFERSIZE);
	sccp_callinfo_getCallInfoStr(ci, &buf);
}

#if 0
/*!
 * \brief Reset Caller Id Presentation
 * \param channel SCCP Channel
 */
void sccp_channel_reset_calleridPresenceParameter(sccp_channel_t * channel)
{
	channel->callInfo.presentation = CALLERID_PRESENCE_ALLOWED;
	if (iPbx.set_callerid_presence) {
		iPbx.set_callerid_presence(channel);
	}
}

/*!
 * \brief Set Caller Id Presentation
 * \param channel SCCP Channel
 * \param presenceParameter SCCP CallerID Presence ENUM
 */
void sccp_channel_set_calleridPresenceParameter(sccp_channel_t * channel, sccp_calleridpresence_t presenceParameter)
{
	channel->callInfo.presentation = presenceParameter;
	if (iPbx.set_callerid_presence) {
		iPbx.set_callerid_presence(channel);
	}
}

#endif
// kate: indent-width 8; replace-tabs off; indent-mode cstyle; auto-insert-doxygen on; line-numbers on; tab-indents on; keep-extra-spaces off; auto-brackets off;
