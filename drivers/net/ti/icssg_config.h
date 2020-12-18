/* SPDX-License-Identifier: GPL-2.0 */
/* Texas Instruments ICSSG Ethernet driver
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 *
 */

#ifndef __NET_TI_ICSSG_CONFIG_H
#define __NET_TI_ICSSG_CONFIG_H

/*------------------------ SR1.0 related --------------------------*/

/* Port queue size in MSMC from firmware
 * PORTQSZ_HP .set (0x1800)
 * PORTQSZ_HP2 .set (PORTQSZ_HP+128) ;include barrier area
 * 0x1880 x 8 bytes per slice  (port)
 */

#define MSMC_RAM_SIZE_SR1	(SZ_64K + SZ_32K + SZ_2K) /* 0x1880 x 8 x 2 */

#define PRUETH_MAX_RX_MGM_DESC	8
#define PRUETH_MAX_RX_FLOWS_SR1	4	/* excluding default flow */
#define PRUETH_RX_FLOW_DATA_SR1	3       /* highest priority flow */
#define PRUETH_MAX_RX_MGM_FLOWS	2	/* excluding default flow */
#define PRUETH_RX_MGM_FLOW_RESPONSE	0
#define PRUETH_RX_MGM_FLOW_TIMESTAMP	1
#define PRUETH_RX_MGM_FLOW_OTHER	2

#define PRUETH_NUM_BUF_POOLS_SR1	16
#define PRUETH_EMAC_BUF_POOL_START_SR1	8
#define PRUETH_EMAC_BUF_POOL_MIN_SIZE_SR1	128
#define PRUETH_EMAC_BUF_SIZE_SR1	1536
#define PRUETH_EMAC_NUM_BUF_SR1		4
#define PRUETH_EMAC_BUF_POOL_SIZE_SR1	(PRUETH_EMAC_NUM_BUF_SR1 * \
					 PRUETH_EMAC_BUF_SIZE_SR1)
/* Config area lies in shared RAM */
#define ICSSG_CONFIG_OFFSET_SLICE0   0
#define ICSSG_CONFIG_OFFSET_SLICE1   0x8000

struct icssg_config_sr1 {
	__le32 status;	/* Firmware status */
	__le32 addr_lo;	/* MSMC Buffer pool base address low. */
	__le32 addr_hi;	/* MSMC Buffer pool base address high. Must be 0 */
	__le32 tx_buf_sz[16];	/* Array of buffer pool sizes */
	__le32 num_tx_threads;	/* Number of active egress threads, 1 to 4 */
	__le32 tx_rate_lim_en;	/* Bitmask: Egress rate limit en per thread */
	__le32 rx_flow_id;	/* RX flow id for first rx ring */
	__le32 rx_mgr_flow_id;	/* RX flow id for the first management ring */
	__le32 flags;		/* TBD */
	__le32 n_burst;		/* for debug */
	__le32 rtu_status;	/* RTU status */
	__le32 info;		/* reserved */
} __packed;

/* Shutdown command to stop processing at firmware.
 * Command format : 0x8101ss00. ss - sequence number. Currently not used
 * by driver.
 */
#define ICSSG_SHUTDOWN_CMD		0x81010000

/* pstate speed/duplex command to set speed and duplex settings
 * in firmware.
 * Command format : 0x8102ssPN. ss - sequence number: currently not
 * used by driver, P - port number: For switch, N - Speed/Duplex state
 * - Possible values of N:
 * 0x0 - 10Mbps/Half duplex ;
 * 0x8 - 10Mbps/Full duplex ;
 * 0x2 - 100Mbps/Half duplex;
 * 0xa - 100Mbps/Full duplex;
 * 0xc - 1Gbps/Full duplex;
 * NOTE: The above are same as bits [3..1](slice 0) or bits [8..6](slice 1) of
 * RGMII CFG register. So suggested to read the register to populate the command
 * bits.
 */
#define ICSSG_PSTATE_SPEED_DUPLEX_CMD	0x81020000
#define ICSSG_PSTATE_FULL_DUPLEX	BIT(3)
#define ICSSG_PSTATE_SPEED_100		BIT(1)
#define ICSSG_PSTATE_SPEED_1000		BIT(2)

/* Flow IDs used in config structure to firmware. Should match with
 * flow_id in struct dma for rx channels.
 */
#define ICSSG_RX_CHAN_FLOW_ID		0 /* flow id for host port */
#define ICSSG_RX_MGM_CHAN_FLOW_ID	1 /* flow id for command response */

#endif /* __NET_TI_ICSSG_CONFIG_H */
