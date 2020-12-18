/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Texas Instruments K3 AM65 Ethernet Switch SubSystem Driver
 *
 * Copyright (C) 2019, Texas Instruments, Incorporated
 *
 */

#ifndef __NET_TI_ICSSG_PRUETH_H
#define __NET_TI_ICSSG_PRUETH_H

#include <common.h>
#include <asm/io.h>
#include <clk.h>
#include <dm.h>
#include <dm/lists.h>
#include <dm/ofnode.h>
#include <dm/device.h>
#include <dma-uclass.h>
#include <regmap.h>

void icssg_class_set_mac_addr(struct regmap *miig_rt, int slice, u8 *mac);
void icssg_class_disable(struct regmap *miig_rt, int slice);
void icssg_class_default(struct regmap *miig_rt, int slice);
void icssg_class_promiscuous(struct regmap *miig_rt, int slice);

/* Config area lies in shared RAM */
#define ICSSG_CONFIG_OFFSET_SLICE0   0
#define ICSSG_CONFIG_OFFSET_SLICE1   0x8000

/* Load time Fiwmware Configuration */
struct icssg_config {
	__le32 status;	/* Firmware status */
	__le32 addr_lo;	/* MSMC Buffer pool base address low. */
	__le32 addr_hi;	/* MSMC Buffer pool base address high. Must be 0 */
	__le32 tx_buf_sz[16];	/* Array of buffer pool sizes */
	__le32 num_tx_threads;	/* Number of active egress threads, 1 to 4 */
	__le32 tx_rate_lim_en;	/* Bitmask: Egress rate limit enable per thread */
	__le32 rx_flow_id;	/* RX flow id for first rx ring */
	__le32 rx_mgr_flow_id;	/* RX flow id for the first management ring */
	__le32 flags;		/* TBD */
	__le32 n_burst;		/* for debug */
	__le32 rtu_status;	/* RTU status */
	__le32 info;		/* reserved */
} __packed;

enum prueth_mac {
	PRUETH_MAC0 = 0,
	PRUETH_MAC1,
	PRUETH_NUM_MACS,
};

enum prueth_port {
	PRUETH_PORT_HOST = 0,	/* host side port */
	PRUETH_PORT_MII0,	/* physical port MII 0 */
	PRUETH_PORT_MII1,	/* physical port MII 1 */
};

/**
 * enum pruss_pru_id - PRU core identifiers
 */
enum pruss_pru_id {
	PRUSS_PRU0 = 0,
	PRUSS_PRU1,
	PRUSS_NUM_PRUS,
};

struct prueth {
	struct udevice		*dev;
	struct regmap		*miig_rt;
	struct regmap		*mii_rt;
	fdt_addr_t		mdio_base;
	phys_addr_t		pruss_shrdram2;
	phys_addr_t		tmaddr;
	struct mii_dev		*bus;
	u32			port_id;
	u32			sram_pa;
	struct phy_device	*phydev;
	bool			has_phy;
	ofnode			phy_node;
	u32			phy_addr;
	ofnode			eth_node[PRUETH_NUM_MACS];
	struct icssg_config	config[PRUSS_NUM_PRUS];
	u32			mdio_freq;
	int			phy_interface;
	struct			clk mdiofck;
	struct dma		dma_tx;
	struct dma		dma_rx;
	struct dma		dma_rx_mgm;
	u32			rx_next;
	u32			rx_pend;
	int			slice;
};

/* config helpers */
void icssg_config_ipg(struct prueth *prueth, int speed, int mii);

#endif /* __NET_TI_ICSSG_PRUETH_H */
