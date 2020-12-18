/* SPDX-License-Identifier: GPL-2.0 */

/* PRU-ICSS MII_RT register definitions
 *
 * Copyright (C) 2015-2020 Texas Instruments Incorporated - http://www.ti.com
 */

#ifndef __NET_PRUSS_MII_RT_H__
#define __NET_PRUSS_MII_RT_H__

#include <regmap.h>

#define RGMII_CFG_OFFSET	4

/* Constant to choose between MII0 and MII1 */
#define ICSS_MII0	0
#define ICSS_MII1	1

/* RGMII CFG Register bits */
#define RGMII_CFG_GIG_EN_MII0	BIT(17)
#define RGMII_CFG_GIG_EN_MII1	BIT(21)
#define RGMII_CFG_FULL_DUPLEX_MII0	BIT(18)
#define RGMII_CFG_FULL_DUPLEX_MII1	BIT(22)

/* PRUSS_MII_RT Registers */
#define PRUSS_MII_RT_RXCFG0		0x0
#define PRUSS_MII_RT_RXCFG1		0x4
#define PRUSS_MII_RT_TXCFG0		0x10
#define PRUSS_MII_RT_TXCFG1		0x14
#define PRUSS_MII_RT_TX_CRC0		0x20
#define PRUSS_MII_RT_TX_CRC1		0x24
#define PRUSS_MII_RT_TX_IPG0		0x30
#define PRUSS_MII_RT_TX_IPG1		0x34
#define PRUSS_MII_RT_PRS0		0x38
#define PRUSS_MII_RT_PRS1		0x3c
#define PRUSS_MII_RT_RX_FRMS0		0x40
#define PRUSS_MII_RT_RX_FRMS1		0x44
#define PRUSS_MII_RT_RX_PCNT0		0x48
#define PRUSS_MII_RT_RX_PCNT1		0x4c
#define PRUSS_MII_RT_RX_ERR0		0x50
#define PRUSS_MII_RT_RX_ERR1		0x54

static inline void icssg_mii_update_ipg(struct regmap *mii_rt, int mii, u32 ipg)
{
	u32 val;

	if (mii == ICSS_MII0) {
		regmap_write(mii_rt, PRUSS_MII_RT_TX_IPG0, ipg);
	} else {
		/* Errata workaround: IEP1 is not read by h/w unless IEP0 is written */
		regmap_read(mii_rt, PRUSS_MII_RT_TX_IPG0, &val);
		regmap_write(mii_rt, PRUSS_MII_RT_TX_IPG1, ipg);
		regmap_write(mii_rt, PRUSS_MII_RT_TX_IPG0, val);
	}
}

static inline void icssg_update_rgmii_cfg(struct regmap *miig_rt, bool gig_en,
					  bool full_duplex, int mii)
{
	u32 gig_en_mask, gig_val = 0, full_duplex_mask, full_duplex_val = 0;

	gig_en_mask = (mii == ICSS_MII0) ? RGMII_CFG_GIG_EN_MII0 :
					RGMII_CFG_GIG_EN_MII1;
	if (gig_en)
		gig_val = gig_en_mask;
	regmap_update_bits(miig_rt, RGMII_CFG_OFFSET, gig_en_mask, gig_val);

	full_duplex_mask = (mii == ICSS_MII0) ? RGMII_CFG_FULL_DUPLEX_MII0 :
					   RGMII_CFG_FULL_DUPLEX_MII1;
	if (full_duplex)
		full_duplex_val = full_duplex_mask;
	regmap_update_bits(miig_rt, RGMII_CFG_OFFSET, full_duplex_mask,
			   full_duplex_val);
}

#endif /* __NET_PRUSS_MII_RT_H__ */
