/*
 *************************************************************************
 * Ralink Tech Inc.
 * 5F., No.36, Taiyuan St., Jhubei City,
 * Hsinchu County 302,
 * Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2010, Ralink Technology, Inc.
 *
 * This program is free software; you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation; either version 2 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * This program is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with this program; if not, write to the                         *
 * Free Software Foundation, Inc.,                                       *
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 *                                                                       *
 *************************************************************************/


#ifndef __SPECTRUM_H__
#define __SPECTRUM_H__

#include "rtmp_type.h"
#include "spectrum_def.h"


u8 GetRegulatoryMaxTxPwr(
	IN struct rtmp_adapter *pAd,
	IN u8 channel);

CHAR RTMP_GetTxPwr(
	IN struct rtmp_adapter *pAd,
	IN HTTRANSMIT_SETTING HTTxMode);

/*
	==========================================================================
	Description:
		Prepare Measurement request action frame and enqueue it into
		management queue waiting for transmition.

	Parametrs:
		1. the destination mac address of the frame.

	Return	: None.
	==========================================================================
 */
void MakeMeasurementReqFrame(
	IN struct rtmp_adapter *pAd,
	OUT PUCHAR pOutBuffer,
	OUT PULONG pFrameLen,
	IN u8 TotalLen,
	IN u8 Category,
	IN u8 Action,
	IN u8 MeasureToken,
	IN u8 MeasureReqMode,
	IN u8 MeasureReqType,
	IN UINT16 NumOfRepetitions);

/*
	==========================================================================
	Description:
		Prepare Measurement report action frame and enqueue it into
		management queue waiting for transmition.

	Parametrs:
		1. the destination mac address of the frame.

	Return	: None.
	==========================================================================
 */
void EnqueueMeasurementRep(
	IN struct rtmp_adapter *pAd,
	IN PUCHAR pDA,
	IN u8 DialogToken,
	IN u8 MeasureToken,
	IN u8 MeasureReqMode,
	IN u8 MeasureReqType,
	IN u8 ReportInfoLen,
	IN u8 * pReportInfo);

/*
	==========================================================================
	Description:
		Prepare TPC Request action frame and enqueue it into
		management queue waiting for transmition.

	Parametrs:
		1. the destination mac address of the frame.

	Return	: None.
	==========================================================================
 */
void EnqueueTPCReq(
	IN struct rtmp_adapter *pAd,
	IN PUCHAR pDA,
	IN UCHAR DialogToken);

/*
	==========================================================================
	Description:
		Prepare TPC Report action frame and enqueue it into
		management queue waiting for transmition.

	Parametrs:
		1. the destination mac address of the frame.

	Return	: None.
	==========================================================================
 */
void EnqueueTPCRep(
	IN struct rtmp_adapter *pAd,
	IN PUCHAR pDA,
	IN u8 DialogToken,
	IN u8 TxPwr,
	IN u8 LinkMargin);

#ifdef WDS_SUPPORT
/*
	==========================================================================
	Description:
		Prepare Channel Switch Announcement action frame and enqueue it into
		management queue waiting for transmition.

	Parametrs:
		1. the destination mac address of the frame.
		2. Channel switch announcement mode.
		2. a New selected channel.

	Return	: None.
	==========================================================================
 */
void EnqueueChSwAnn(
	IN struct rtmp_adapter *pAd,
	IN PUCHAR pDA,
	IN u8 ChSwMode,
	IN u8 NewCh);
#endif /* WDS_SUPPORT */

/*
	==========================================================================
	Description:
		Spectrun action frames Handler such as channel switch annoucement,
		measurement report, measurement request actions frames.

	Parametrs:
		Elme - MLME message containing the received frame

	Return	: None.
	==========================================================================
 */
void PeerSpectrumAction(
    IN struct rtmp_adapter *pAd,
    IN MLME_QUEUE_ELEM *Elem);

/*
	==========================================================================
	Description:

	Parametrs:

	Return	: None.
	==========================================================================
 */
INT Set_MeasureReq_Proc(
	IN	struct rtmp_adapter *pAd,
	IN	char *		arg);

INT Set_TpcReq_Proc(
	IN	struct rtmp_adapter *pAd,
	IN	char *		arg);

INT Set_PwrConstraint(
	IN	struct rtmp_adapter *pAd,
	IN	char *		arg);


NDIS_STATUS	MeasureReqTabInit(
	IN struct rtmp_adapter *pAd);

void MeasureReqTabExit(
	IN struct rtmp_adapter *pAd);

PMEASURE_REQ_ENTRY MeasureReqLookUp(
	IN struct rtmp_adapter *pAd,
	IN u8			DialogToken);

PMEASURE_REQ_ENTRY MeasureReqInsert(
	IN struct rtmp_adapter *pAd,
	IN u8			DialogToken);

void MeasureReqDelete(
	IN struct rtmp_adapter *pAd,
	IN u8			DialogToken);

void InsertChannelRepIE(
	IN struct rtmp_adapter *pAd,
	OUT PUCHAR pFrameBuf,
	OUT PULONG pFrameLen,
	IN char *pCountry,
	IN u8 RegulatoryClass);

void InsertTpcReportIE(
	IN struct rtmp_adapter *pAd,
	OUT PUCHAR pFrameBuf,
	OUT PULONG pFrameLen,
	IN u8 TxPwr,
	IN u8 LinkMargin);

void InsertDialogToken(
	IN struct rtmp_adapter *pAd,
	OUT PUCHAR pFrameBuf,
	OUT PULONG pFrameLen,
	IN u8 DialogToken);

NDIS_STATUS	TpcReqTabInit(
	IN struct rtmp_adapter *pAd);

void TpcReqTabExit(
	IN struct rtmp_adapter *pAd);

void NotifyChSwAnnToPeerAPs(
	IN struct rtmp_adapter *pAd,
	IN PUCHAR pRA,
	IN PUCHAR pTA,
	IN u8 ChSwMode,
	IN u8 Channel);

void RguClass_BuildBcnChList(
	IN struct rtmp_adapter *pAd,
	OUT PUCHAR pBuf,
	OUT	PULONG pBufLen);
#endif /* __SPECTRUM_H__ */

