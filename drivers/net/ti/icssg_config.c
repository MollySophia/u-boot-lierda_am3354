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
