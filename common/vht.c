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


#include "rt_config.h"


/*
	IEEE 802.11AC D2.0 sec 22.3.14
	Channelization, Table 22-21

	A VHT channel is specified by the four PLME MIB fields
	(Fields to specify VHT channels).

	dot11CurrentChannelBandwidth:
		Channel bandwidth. Possible values are
			cbw20, cbw40, cbw80, cbw160 and cbw80p80.
	dot11CurrentChannelCenterFrequencyIndex1:
		In 20 MHz, 40 MHz, 80 MHz and 160 MHz channels, denotes the channel
			center frequency.
		In 80+80 MHz channels, denotes the center frequency of the frequency
			segment 1, which is the frequency segment containing the primary
			channel..
		Valid range = 1, �K, 200.
	dot11CurrentChannelCenterFrequencyIndex2:
		In 80+80 MHz channels, denotes the center frequency of the frequency
			segment 2, which is the frequency segment that does not contain the
			primary channel.
			Valid range = 1, �K, 200.
		Undefined for 20 MHz, 40 MHz, 80 MHz and 160 MHz channels.
	dot11CurrentPrimaryChannel:
		Denotes the location of the primary 20 MHz channel.
		Valid range = 1, �K, 200.


	Formula:
	 A channel center frequency of 5.000 GHz shall be indicated by
	 	dot11ChannelStartingFactor = 8000, and
		dot11CurrentPrimaryChannel = 200.

	 Channel starting frequency
	 	= dot11ChannelStartingFactor �� 0500 kHz.

	Channel center frequency [MHz]
		= Channel starting frequency + 5 * dot11CurrentChannelCenterFrequencyIndex

	Primary 20 MHz channel center frequency [MHz]
		= Channel starting frequency + 5 * dot11CurrentPrimaryChannel

	ex:  a channel specified by:
		dot11CurrentChannelBandwidth = 80 MHz
		dot11CurrentChannelCenterFrequencyIndex1 = 42
		dot11CurrentPrimaryChannel = 36

		=>is an 80 MHz channel with a center frequency of 5210 MHz and
			the primary 20 MHz channel centered at 5180 MHz.

*/
struct vht_ch_layout{
	UCHAR ch_low_bnd;
	UCHAR ch_up_bnd;
	UCHAR cent_freq_idx;
};

static struct vht_ch_layout vht_ch_80M[]={
	{36, 48, 42},
	{52, 64, 58},
	{100,112, 106},
	{116, 128, 122},
	{132, 144, 138},
	{149, 161, 155},
	{0, 0 ,0},
};




void dump_vht_cap(struct rtmp_adapter*pAd, VHT_CAP_IE *vht_ie)
{
	VHT_CAP_INFO *vht_cap = &vht_ie->vht_cap;
	VHT_MCS_SET *vht_mcs = &vht_ie->mcs_set;

	DBGPRINT(RT_DEBUG_OFF, ("Dump VHT_CAP IE\n"));
	hex_dump("VHT CAP IE Raw Data", (UCHAR *)vht_ie, sizeof(VHT_CAP_IE));

	DBGPRINT(RT_DEBUG_OFF, ("VHT Capabilities Info Field\n"));
	DBGPRINT(RT_DEBUG_OFF, ("\tMaximum MPDU Length=%d\n", vht_cap->max_mpdu_len));
	DBGPRINT(RT_DEBUG_OFF, ("\tSupported Channel Width=%d\n", vht_cap->ch_width));
	DBGPRINT(RT_DEBUG_OFF, ("\tRxLDPC=%d\n", vht_cap->rx_ldpc));
	DBGPRINT(RT_DEBUG_OFF, ("\tShortGI_80M=%d\n", vht_cap->sgi_80M));
	DBGPRINT(RT_DEBUG_OFF, ("\tShortGI_160M=%d\n", vht_cap->sgi_160M));
	DBGPRINT(RT_DEBUG_OFF, ("\tTxSTBC=%d\n", vht_cap->tx_stbc));
	DBGPRINT(RT_DEBUG_OFF, ("\tRxSTBC=%d\n", vht_cap->rx_stbc));
	DBGPRINT(RT_DEBUG_OFF, ("\tSU BeamformerCap=%d\n", vht_cap->bfer_cap_su));
	DBGPRINT(RT_DEBUG_OFF, ("\tSU BeamformeeCap=%d\n", vht_cap->bfee_cap_su));
	DBGPRINT(RT_DEBUG_OFF, ("\tCompressedSteeringNumOfBeamformerAnt=%d\n", vht_cap->cmp_st_num_bfer));
	DBGPRINT(RT_DEBUG_OFF, ("\tNumber of Sounding Dimensions=%d\n", vht_cap->num_snd_dimension));
	DBGPRINT(RT_DEBUG_OFF, ("\tMU BeamformerCap=%d\n", vht_cap->bfer_cap_mu));
	DBGPRINT(RT_DEBUG_OFF, ("\tMU BeamformeeCap=%d\n", vht_cap->bfee_cap_mu));
	DBGPRINT(RT_DEBUG_OFF, ("\tVHT TXOP PS=%d\n", vht_cap->vht_txop_ps));
	DBGPRINT(RT_DEBUG_OFF, ("\t+HTC-VHT Capable=%d\n", vht_cap->htc_vht_cap));
	DBGPRINT(RT_DEBUG_OFF, ("\tMaximum A-MPDU Length Exponent=%d\n", vht_cap->max_ampdu_exp));
	DBGPRINT(RT_DEBUG_OFF, ("\tVHT LinkAdaptation Capable=%d\n", vht_cap->vht_link_adapt));

	DBGPRINT(RT_DEBUG_OFF, ("VHT Supported MCS Set Field\n"));
	DBGPRINT(RT_DEBUG_OFF, ("\tRx Highest SupDataRate=%d\n", vht_mcs->rx_high_rate));
	DBGPRINT(RT_DEBUG_OFF, ("\tRxMCS Map_1SS=%d\n", vht_mcs->rx_mcs_map.mcs_ss1));
	DBGPRINT(RT_DEBUG_OFF, ("\tRxMCS Map_2SS=%d\n", vht_mcs->rx_mcs_map.mcs_ss2));
	DBGPRINT(RT_DEBUG_OFF, ("\tTx Highest SupDataRate=%d\n", vht_mcs->tx_high_rate));
	DBGPRINT(RT_DEBUG_OFF, ("\tTxMCS Map_1SS=%d\n", vht_mcs->tx_mcs_map.mcs_ss1));
	DBGPRINT(RT_DEBUG_OFF, ("\tTxMCS Map_2SS=%d\n", vht_mcs->tx_mcs_map.mcs_ss2));
}


void dump_vht_op(struct rtmp_adapter*pAd, VHT_OP_IE *vht_ie)
{
	VHT_OP_INFO *vht_op = &vht_ie->vht_op_info;
	VHT_MCS_MAP *vht_mcs = &vht_ie->basic_mcs_set;

	DBGPRINT(RT_DEBUG_OFF, ("Dump VHT_OP IE\n"));
	hex_dump("VHT OP IE Raw Data", (UCHAR *)vht_ie, sizeof(VHT_OP_IE));

	DBGPRINT(RT_DEBUG_OFF, ("VHT Operation Info Field\n"));
	DBGPRINT(RT_DEBUG_OFF, ("\tChannelWidth=%d\n", vht_op->ch_width));
	DBGPRINT(RT_DEBUG_OFF, ("\tChannelCenterFrequency Seg 1=%d\n", vht_op->center_freq_1));
	DBGPRINT(RT_DEBUG_OFF, ("\tChannelCenterFrequency Seg 1=%d\n", vht_op->center_freq_2));

	DBGPRINT(RT_DEBUG_OFF, ("VHT Basic MCS Set Field\n"));
	DBGPRINT(RT_DEBUG_OFF, ("\tRxMCS Map_1SS=%d\n", vht_mcs->mcs_ss1));
	DBGPRINT(RT_DEBUG_OFF, ("\tRxMCS Map_2SS=%d\n", vht_mcs->mcs_ss2));
}


/*
	Get BBP Channel Index by RF channel info
	return value: 0~3
*/
UCHAR vht_prim_ch_idx(UCHAR vht_cent_ch, UCHAR prim_ch)
{
	INT idx = 0;
	UCHAR bbp_idx = 0;

	if (vht_cent_ch == prim_ch)
		goto done;

	while (vht_ch_80M[idx].ch_up_bnd != 0)
	{
		if (vht_cent_ch == vht_ch_80M[idx].cent_freq_idx)
		{
			if (prim_ch == vht_ch_80M[idx].ch_up_bnd)
				bbp_idx = 3;
			else if (prim_ch == vht_ch_80M[idx].ch_low_bnd)
				bbp_idx = 0;
			else {
				bbp_idx = prim_ch > vht_cent_ch ? 2 : 1;
			}
			break;
		}
		idx++;
	}

done:
	DBGPRINT(RT_DEBUG_TRACE, ("%s():(VhtCentCh=%d, PrimCh=%d) =>BbpChIdx=%d\n",
				__FUNCTION__, vht_cent_ch, prim_ch, bbp_idx));
	return bbp_idx;
}


/*
	Currently we only consider about VHT 80MHz!
*/
UCHAR vht_cent_ch_freq(struct rtmp_adapter*pAd, UCHAR prim_ch)
{
	INT idx = 0;


	if (pAd->CommonCfg.vht_bw < VHT_BW_80 || prim_ch < 36)
	{
		pAd->CommonCfg.vht_cent_ch = 0;
		pAd->CommonCfg.vht_cent_ch2 = 0;
		return prim_ch;
	}

	while (vht_ch_80M[idx].ch_up_bnd != 0)
	{
		if (prim_ch >= vht_ch_80M[idx].ch_low_bnd &&
			prim_ch <= vht_ch_80M[idx].ch_up_bnd)
		{
			pAd->CommonCfg.vht_cent_ch = vht_ch_80M[idx].cent_freq_idx;
			return vht_ch_80M[idx].cent_freq_idx;
		}
		idx++;
	}

	return prim_ch;
}


INT vht_mode_adjust(struct rtmp_adapter*pAd, MAC_TABLE_ENTRY *pEntry, VHT_CAP_IE *cap, VHT_OP_IE *op)
{
	pEntry->MaxHTPhyMode.field.MODE = MODE_VHT;
	pAd->CommonCfg.AddHTInfo.AddHtInfo2.NonGfPresent = 1;
	pAd->MacTab.fAnyStationNonGF = TRUE;

	if (op->vht_op_info.ch_width >= 1 && pEntry->MaxHTPhyMode.field.BW == BW_40)
	{
		pEntry->MaxHTPhyMode.field.BW= BW_80;
		pEntry->MaxHTPhyMode.field.ShortGI = (cap->vht_cap.sgi_80M);
		pEntry->MaxHTPhyMode.field.STBC = (cap->vht_cap.rx_stbc > 1 ? 1 : 0);
	}

	return TRUE;
}


INT get_vht_op_ch_width(struct rtmp_adapter*pAd)
{

	return TRUE;
}


/********************************************************************
	Procedures for 802.11 AC Information elements
********************************************************************/
/*
	Defined in IEEE 802.11AC

	Appeared in Beacon, ProbResp frames
*/
INT build_quiet_channel(struct rtmp_adapter*pAd, UCHAR *buf)
{
	INT len = 0;


	return len;
}


/*
	Defined in IEEE 802.11AC

	Appeared in Beacon, ProbResp frames
*/
INT build_ext_bss_load(struct rtmp_adapter*pAd, UCHAR *buf)
{
	INT len = 0;


	return len;
}


/*
	Defined in IEEE 802.11AC

	Appeared in Beacon, ProbResp frames
*/
INT build_ext_pwr_constraint(struct rtmp_adapter*pAd, UCHAR *buf)
{
	INT len = 0;


	return len;
}


/*
	Defined in IEEE 802.11AC

	Appeared in Beacon, ProbResp frames
*/
INT build_vht_pwr_envelope(struct rtmp_adapter*pAd, UCHAR *buf)
{
	INT len = 0;


	return len;
}


/*
	Defined in IEEE 802.11AC

	Appeared in Beacon, (Re)AssocResp, ProbResp frames
*/
INT build_vht_op_ie(struct rtmp_adapter*pAd, UCHAR *buf)
{
	VHT_OP_IE vht_op;

	NdisZeroMemory((UCHAR *)&vht_op, sizeof(VHT_OP_IE));
	vht_op.vht_op_info.ch_width = (pAd->CommonCfg.vht_bw == VHT_BW_80 ? 1: 0);
	switch (vht_op.vht_op_info.ch_width)
	{
		case 0:
			vht_op.vht_op_info.center_freq_1 = 0;
			vht_op.vht_op_info.center_freq_2 = 0;
			break;
		case 1:
		case 2:
			vht_op.vht_op_info.center_freq_1 = pAd->CommonCfg.vht_cent_ch;
			vht_op.vht_op_info.center_freq_2 = 0;
			break;
		case 3:
			vht_op.vht_op_info.center_freq_1 = pAd->CommonCfg.vht_cent_ch;
			vht_op.vht_op_info.center_freq_2 = pAd->CommonCfg.vht_cent_ch2;
			break;
	}

	vht_op.basic_mcs_set.mcs_ss1 = 3;
	vht_op.basic_mcs_set.mcs_ss2 = 3;
	vht_op.basic_mcs_set.mcs_ss3 = 3;
	vht_op.basic_mcs_set.mcs_ss4 = 3;
	vht_op.basic_mcs_set.mcs_ss5 = 3;
	vht_op.basic_mcs_set.mcs_ss6 = 3;
	vht_op.basic_mcs_set.mcs_ss7 = 3;
	vht_op.basic_mcs_set.mcs_ss8 = 3;
	switch  (pAd->CommonCfg.RxStream)
	{
		case 2:
			vht_op.basic_mcs_set.mcs_ss2 = VHT_MCS_CAP_7;
		case 1:
#if	defined(MT76x0) || defined(MT76x2)
			if (IS_MT76x0(pAd) || IS_MT76x2(pAd))
			{
				/*
					MT7650E2 support VHT_MCS8 & VHT_MCS9.
				*/
				vht_op.basic_mcs_set.mcs_ss1 = VHT_MCS_CAP_9;
			}
			else
#endif
			vht_op.basic_mcs_set.mcs_ss1 = VHT_MCS_CAP_7;
			break;
	}

	NdisMoveMemory((UCHAR *)buf, (UCHAR *)&vht_op, sizeof(VHT_OP_IE));

	return sizeof(VHT_OP_IE);
}


/*
	Defined in IEEE 802.11AC

	Appeared in Beacon, (Re)AssocReq, (Re)AssocResp, ProbReq/Resp frames
*/
INT build_vht_cap_ie(struct rtmp_adapter*pAd, UCHAR *buf)
{
	VHT_CAP_IE vht_cap_ie;

	NdisZeroMemory((UCHAR *)&vht_cap_ie,  sizeof(VHT_CAP_IE));
	vht_cap_ie.vht_cap.max_mpdu_len = 0; // TODO: Ask Jerry about hardware limitation.
	vht_cap_ie.vht_cap.ch_width = 0; /* not support 160 or 80 + 80 MHz */
	vht_cap_ie.vht_cap.sgi_80M = pAd->CommonCfg.vht_sgi_80;
	vht_cap_ie.vht_cap.htc_vht_cap = 1;
	vht_cap_ie.vht_cap.max_ampdu_exp = 3; // TODO: Ask Jerry about the hardware limitation, currently set as 64K


	vht_cap_ie.vht_cap.tx_stbc = 0;
	vht_cap_ie.vht_cap.rx_stbc = 0;
	if (pAd->CommonCfg.vht_stbc)
	{
		if (pAd->CommonCfg.TxStream >= 2)
			vht_cap_ie.vht_cap.tx_stbc = 1;
		else
			vht_cap_ie.vht_cap.tx_stbc = 0;

		if (pAd->CommonCfg.RxStream >= 1)
			vht_cap_ie.vht_cap.rx_stbc = 1; // TODO: is it depends on the number of our antennas?
		else
			vht_cap_ie.vht_cap.rx_stbc = 0;
	}

	vht_cap_ie.vht_cap.tx_ant_consistency = 1;
	vht_cap_ie.vht_cap.rx_ant_consistency = 1;

	vht_cap_ie.mcs_set.rx_mcs_map.mcs_ss1 = VHT_MCS_CAP_NA;
	vht_cap_ie.mcs_set.rx_mcs_map.mcs_ss2 = VHT_MCS_CAP_NA;
	vht_cap_ie.mcs_set.rx_mcs_map.mcs_ss3 = VHT_MCS_CAP_NA;
	vht_cap_ie.mcs_set.rx_mcs_map.mcs_ss4 = VHT_MCS_CAP_NA;
	vht_cap_ie.mcs_set.rx_mcs_map.mcs_ss5 = VHT_MCS_CAP_NA;
	vht_cap_ie.mcs_set.rx_mcs_map.mcs_ss6 = VHT_MCS_CAP_NA;
	vht_cap_ie.mcs_set.rx_mcs_map.mcs_ss7 = VHT_MCS_CAP_NA;
	vht_cap_ie.mcs_set.rx_mcs_map.mcs_ss8 = VHT_MCS_CAP_NA;

	vht_cap_ie.mcs_set.tx_mcs_map.mcs_ss1 = VHT_MCS_CAP_NA;
	vht_cap_ie.mcs_set.tx_mcs_map.mcs_ss2 = VHT_MCS_CAP_NA;
	vht_cap_ie.mcs_set.tx_mcs_map.mcs_ss3 = VHT_MCS_CAP_NA;
	vht_cap_ie.mcs_set.tx_mcs_map.mcs_ss4 = VHT_MCS_CAP_NA;
	vht_cap_ie.mcs_set.tx_mcs_map.mcs_ss5 = VHT_MCS_CAP_NA;
	vht_cap_ie.mcs_set.tx_mcs_map.mcs_ss6 = VHT_MCS_CAP_NA;
	vht_cap_ie.mcs_set.tx_mcs_map.mcs_ss7 = VHT_MCS_CAP_NA;
	vht_cap_ie.mcs_set.tx_mcs_map.mcs_ss8 = VHT_MCS_CAP_NA;


	switch  (pAd->CommonCfg.RxStream)
	{
		case 1:
			vht_cap_ie.mcs_set.rx_high_rate = 292;
#if	defined(MT76x0) || defined(MT76x2)
			if (IS_MT76x0(pAd) || IS_MT76x2(pAd))
			{
				/*
					MT7650E2 support VHT_MCS8 & VHT_MCS9.
				*/
				vht_cap_ie.mcs_set.rx_mcs_map.mcs_ss1 = VHT_MCS_CAP_9;
			}
			else
#endif /* MT76x0 */
			vht_cap_ie.mcs_set.rx_mcs_map.mcs_ss1 = VHT_MCS_CAP_7;
			break;
		case 2:
			vht_cap_ie.mcs_set.rx_high_rate = 585;
			vht_cap_ie.mcs_set.rx_mcs_map.mcs_ss1 = VHT_MCS_CAP_7;
			vht_cap_ie.mcs_set.rx_mcs_map.mcs_ss2 = VHT_MCS_CAP_7;
			break;
		default:
			vht_cap_ie.mcs_set.rx_high_rate = 0;
			break;
	}

	switch (pAd->CommonCfg.TxStream)
	{
		case 1:
			vht_cap_ie.mcs_set.tx_high_rate = 292;
#if	defined(MT76x0) || defined(MT76x2)
			if (IS_MT76x0(pAd) || IS_MT76x2(pAd))
			{
				/*
					MT7650E2 support VHT_MCS8 & VHT_MCS9.
				*/
				vht_cap_ie.mcs_set.tx_mcs_map.mcs_ss1 = VHT_MCS_CAP_9;
			}
			else
#endif /* MT76x0 */
			vht_cap_ie.mcs_set.tx_mcs_map.mcs_ss1 = VHT_MCS_CAP_7;
			break;
		case 2:
			vht_cap_ie.mcs_set.tx_high_rate = 585;
			vht_cap_ie.mcs_set.tx_mcs_map.mcs_ss1 = VHT_MCS_CAP_7;
			vht_cap_ie.mcs_set.tx_mcs_map.mcs_ss2 = VHT_MCS_CAP_7;
			break;
		default:
			vht_cap_ie.mcs_set.tx_high_rate = 0;
			break;
	}

	NdisMoveMemory(buf, (UCHAR *)&vht_cap_ie, sizeof(VHT_CAP_IE));

	return sizeof(VHT_CAP_IE);
}


INT build_vht_ies(struct rtmp_adapter*pAd, UCHAR *buf, UCHAR frm)
{
	INT len = 0;
	EID_STRUCT eid_hdr;


	eid_hdr.Eid = IE_VHT_CAP;
	eid_hdr.Len = sizeof(VHT_CAP_IE);
	NdisMoveMemory(buf, (UCHAR *)&eid_hdr, 2);
	len = 2;

	len += build_vht_cap_ie(pAd, (UCHAR *)(buf + len));
	if (frm == SUBTYPE_BEACON || frm == SUBTYPE_PROBE_RSP ||
		frm == SUBTYPE_ASSOC_RSP || frm == SUBTYPE_REASSOC_RSP)
	{
		eid_hdr.Eid = IE_VHT_OP;
		eid_hdr.Len = sizeof(VHT_OP_IE);
		NdisMoveMemory((UCHAR *)(buf + len), (UCHAR *)&eid_hdr, 2);
		len +=2;

		len += build_vht_op_ie(pAd, (UCHAR *)(buf + len));
	}

	return len;
}

