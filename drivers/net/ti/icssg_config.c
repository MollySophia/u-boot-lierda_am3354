// SPDX-License-Identifier: GPL-2.0
/* ICSSG Ethernet driver
 *
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com
 */

#include <phy.h>
#include "icssg_prueth.h"
#include "icss_mii_rt.h"

/* TX IPG Values to be set for 100M and 1G link speeds.  These values are
 * in ocp_clk cycles. So need change if ocp_clk is changed for a specific
 * h/w design.
 */

/* SR1.0 IPG is in core_clk cycles */
#define MII_RT_TX_IPG_100M_SR1  0x166
#define MII_RT_TX_IPG_1G_SR1    0x18

/* SR2.0 IPG is in rgmii_clk (125MHz) clock cycles + 1 */
#define MII_RT_TX_IPG_100M      0xb2    /* FIXME: cross check */
#define MII_RT_TX_IPG_1G        0xb

void icssg_config_ipg(struct prueth *prueth, int speed, int mii)
{
	switch (speed) {
	case SPEED_1000:
		icssg_mii_update_ipg(prueth->mii_rt, mii, prueth->is_sr1 ?
				MII_RT_TX_IPG_1G_SR1 : MII_RT_TX_IPG_1G);
		break;
	case SPEED_100:
		icssg_mii_update_ipg(prueth->mii_rt, mii, prueth->is_sr1 ?
				MII_RT_TX_IPG_100M_SR1 : MII_RT_TX_IPG_100M);
		break;
	default:
		/* Other links speeds not supported */
		pr_err("Unsupported link speed\n");
		return;
	}
}

/* SR1: Set buffer sizes for the pools. There are 8 internal queues
 * implemented in firmware, but only 4 tx channels/threads in the Egress
 * direction to firmware. Need a high priority queue for management
 * messages since they shouldn't be blocked even during high traffic
 * situation. So use Q0-Q2 as data queues and Q3 as management queue
 * in the max case. However for ease of configuration, use the max
 * data queue + 1 for management message if we are not using max
 * case.
 *
 * Allocate 4 MTU buffers per data queue.  Firmware requires
 * pool sizes to be set for internal queues. Set the upper 5 queue
 * pool size to min size of 128 bytes since there are only 3 tx
 * data channels and management queue requires only minimum buffer.
 * i.e lower queues are used by driver and highest priority queue
 * from that is used for management message.
 */

static int emac_egress_buf_pool_size[] = {
	PRUETH_EMAC_BUF_POOL_SIZE_SR1, PRUETH_EMAC_BUF_POOL_SIZE_SR1,
	PRUETH_EMAC_BUF_POOL_SIZE_SR1, PRUETH_EMAC_BUF_POOL_MIN_SIZE_SR1,
	PRUETH_EMAC_BUF_POOL_MIN_SIZE_SR1, PRUETH_EMAC_BUF_POOL_MIN_SIZE_SR1,
	PRUETH_EMAC_BUF_POOL_MIN_SIZE_SR1, PRUETH_EMAC_BUF_POOL_MIN_SIZE_SR1};

void icssg_config_sr1(struct prueth *prueth)
{
	void __iomem *va;
	struct icssg_config_sr1 *config;
	int i, index;
	int slice = prueth->slice;

	va = (void __iomem *)prueth->shram.pa + slice * ICSSG_CONFIG_OFFSET_SLICE1;
	config = &prueth->config[slice];
	memset(config, 0, sizeof(*config));
	config->addr_lo = cpu_to_le32(lower_32_bits(prueth->sram_pa));
	config->addr_hi = cpu_to_le32(upper_32_bits(prueth->sram_pa));
	config->num_tx_threads = 0;
	config->rx_flow_id = ICSSG_RX_CHAN_FLOW_ID; /* flow id for host port */
	config->rx_mgr_flow_id = ICSSG_RX_MGM_CHAN_FLOW_ID; /* for mgm ch */

	for (i = PRUETH_EMAC_BUF_POOL_START_SR1; i < PRUETH_NUM_BUF_POOLS_SR1;
	     i++) {
		index =  i - PRUETH_EMAC_BUF_POOL_START_SR1;
		config->tx_buf_sz[i] =
			cpu_to_le32(emac_egress_buf_pool_size[index]);
	}

	memcpy_toio(va, &prueth->config[slice], sizeof(prueth->config[slice]));
}
