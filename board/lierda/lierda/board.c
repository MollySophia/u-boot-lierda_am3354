// SPDX-License-Identifier: GPL-2.0+
/*
 * board.c
 *
 * Board functions for TI AM335X based boards
 *
 * Copyright (C) 2011, Texas Instruments, Incorporated - http://www.ti.com/
 */

#include <common.h>
#include <dm.h>
#include <env.h>
#include <errno.h>
#include <init.h>
#include <spl.h>
#include <serial.h>
#include <asm/arch/cpu.h>
#include <asm/arch/hardware.h>
#include <asm/arch/omap.h>
#include <asm/arch/ddr_defs.h>
#include <asm/arch/clock.h>
#include <asm/arch/clk_synthesizer.h>
#include <asm/arch/gpio.h>
#include <asm/arch/mmc_host_def.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/mem.h>
#include <asm/io.h>
#include <asm/emif.h>
#include <asm/gpio.h>
#include <asm/omap_common.h>
#include <asm/omap_sec_common.h>
#include <asm/omap_mmc.h>
#include <i2c.h>
#include <miiphy.h>
#include <cpsw.h>
#include <power/tps65217.h>
#include <power/tps65910.h>
#include <env_internal.h>
#include <watchdog.h>
#include "board.h"

DECLARE_GLOBAL_DATA_PTR;

/* GPIO that controls power to DDR on EVM-SK */
#define GPIO_TO_PIN(bank, gpio)		(32 * (bank) + (gpio))
#define GPIO_DDR_VTT_EN		GPIO_TO_PIN(0, 7)
#define ICE_GPIO_DDR_VTT_EN	GPIO_TO_PIN(0, 18)
#define GPIO_PR1_MII_CTRL	GPIO_TO_PIN(3, 4)
#define GPIO_MUX_MII_CTRL	GPIO_TO_PIN(3, 10)
#define GPIO_FET_SWITCH_CTRL	GPIO_TO_PIN(0, 7)
#define GPIO_PHY_RESET		GPIO_TO_PIN(2, 5)
#define GPIO_ETH0_MODE		GPIO_TO_PIN(0, 11)
#define GPIO_ETH1_MODE		GPIO_TO_PIN(1, 26)

static struct ctrl_dev *cdev = (struct ctrl_dev *)CTRL_DEVICE_BASE;

#define GPIO0_RISINGDETECT	(AM33XX_GPIO0_BASE + OMAP_GPIO_RISINGDETECT)
#define GPIO1_RISINGDETECT	(AM33XX_GPIO1_BASE + OMAP_GPIO_RISINGDETECT)

#define GPIO0_IRQSTATUS1	(AM33XX_GPIO0_BASE + OMAP_GPIO_IRQSTATUS1)
#define GPIO1_IRQSTATUS1	(AM33XX_GPIO1_BASE + OMAP_GPIO_IRQSTATUS1)

#define GPIO0_IRQSTATUSRAW	(AM33XX_GPIO0_BASE + 0x024)
#define GPIO1_IRQSTATUSRAW	(AM33XX_GPIO1_BASE + 0x024)

#ifndef CONFIG_DM_SERIAL
struct serial_device *default_serial_console(void)
{
		return &eserial1_device;
}
#endif

#ifndef CONFIG_SKIP_LOWLEVEL_INIT

static const struct ddr_data ddr3_beagleblack_data = {
	.datardsratio0 = MT41K256M16HA125E_RD_DQS,
	.datawdsratio0 = MT41K256M16HA125E_WR_DQS,
	.datafwsratio0 = MT41K256M16HA125E_PHY_FIFO_WE,
	.datawrsratio0 = MT41K256M16HA125E_PHY_WR_DATA,
};

static const struct cmd_control ddr3_beagleblack_cmd_ctrl_data = {
	.cmd0csratio = MT41K256M16HA125E_RATIO,
	.cmd0iclkout = MT41K256M16HA125E_INVERT_CLKOUT,

	.cmd1csratio = MT41K256M16HA125E_RATIO,
	.cmd1iclkout = MT41K256M16HA125E_INVERT_CLKOUT,

	.cmd2csratio = MT41K256M16HA125E_RATIO,
	.cmd2iclkout = MT41K256M16HA125E_INVERT_CLKOUT,
};

static struct emif_regs ddr3_beagleblack_emif_reg_data = {
	.sdram_config = MT41K256M16HA125E_EMIF_SDCFG,
	.ref_ctrl = MT41K256M16HA125E_EMIF_SDREF,
	.sdram_tim1 = MT41K256M16HA125E_EMIF_TIM1,
	.sdram_tim2 = MT41K256M16HA125E_EMIF_TIM2,
	.sdram_tim3 = MT41K256M16HA125E_EMIF_TIM3,
	.ocp_config = EMIF_OCP_CONFIG_BEAGLEBONE_BLACK,
	.zq_config = MT41K256M16HA125E_ZQ_CFG,
	.emif_ddr_phy_ctlr_1 = MT41K256M16HA125E_EMIF_READ_LATENCY,
};

#ifdef CONFIG_SPL_OS_BOOT
int spl_start_uboot(void)
{
#ifdef CONFIG_SPL_SERIAL_SUPPORT
	/* break into full u-boot on 'c' */
	if (serial_tstc() && serial_getc() == 'c')
		return 1;
#endif

#ifdef CONFIG_SPL_ENV_SUPPORT
	env_init();
	env_load();
	if (env_get_yesno("boot_os") != 1)
		return 1;
#endif

	return 0;
}
#endif

const struct dpll_params *get_dpll_ddr_params(void)
{
	int ind = get_sys_clk_index();

	return &dpll_ddr3_400MHz[ind];
}

const struct dpll_params *get_dpll_mpu_params(void)
{
	int ind = get_sys_clk_index();
	int freq = am335x_get_efuse_mpu_max_freq(cdev);

	switch (freq) {
	case MPUPLL_M_1000:
		return &dpll_mpu_opp[ind][5];
	case MPUPLL_M_800:
		return &dpll_mpu_opp[ind][4];
	case MPUPLL_M_720:
		return &dpll_mpu_opp[ind][3];
	case MPUPLL_M_600:
		return &dpll_mpu_opp[ind][2];
	case MPUPLL_M_500:
		return &dpll_mpu_opp100;
	case MPUPLL_M_300:
		return &dpll_mpu_opp[ind][0];
	}

	return &dpll_mpu_opp[ind][0];
}

void scale_vcores_generic(int freq)
{
	int sil_rev, mpu_vdd;

	/*
	 * The GP EVM, IDK and EVM SK use a TPS65910 PMIC.  For all
	 * MPU frequencies we support we use a CORE voltage of
	 * 1.10V.  For MPU voltage we need to switch based on
	 * the frequency we are running at.
	 */
#ifndef CONFIG_DM_I2C
	if (i2c_probe(TPS65910_CTRL_I2C_ADDR))
		return;
#else
	if (power_tps65910_init(0))
		return;
#endif
	/*
	 * Depending on MPU clock and PG we will need a different
	 * VDD to drive at that speed.
	 */
	sil_rev = readl(&cdev->deviceid) >> 28;
	mpu_vdd = am335x_get_tps65910_mpu_vdd(sil_rev, freq);

	/* Tell the TPS65910 to use i2c */
	tps65910_set_i2c_control();

	/* First update MPU voltage. */
	if (tps65910_voltage_update(MPU, mpu_vdd))
		return;

	/* Second, update the CORE voltage. */
	if (tps65910_voltage_update(CORE, TPS65910_OP_REG_SEL_1_1_0))
		return;

}

void gpi2c_init(void)
{
	/* When needed to be invoked prior to BSS initialization */
	static bool first_time = true;

	if (first_time) {
		enable_i2c0_pin_mux();
#ifndef CONFIG_DM_I2C
		i2c_init(CONFIG_SYS_OMAP24_I2C_SPEED,
			 CONFIG_SYS_OMAP24_I2C_SLAVE);
#endif
		first_time = false;
	}
}

void scale_vcores(void)
{
	int freq;

	gpi2c_init();
	freq = am335x_get_efuse_mpu_max_freq(cdev);

	scale_vcores_generic(freq);
}

void set_uart_mux_conf(void)
{
#if CONFIG_CONS_INDEX == 1
	enable_uart0_pin_mux();
#elif CONFIG_CONS_INDEX == 2
	enable_uart1_pin_mux();
#elif CONFIG_CONS_INDEX == 3
	enable_uart2_pin_mux();
#elif CONFIG_CONS_INDEX == 4
	enable_uart3_pin_mux();
#elif CONFIG_CONS_INDEX == 5
	enable_uart4_pin_mux();
#elif CONFIG_CONS_INDEX == 6
	enable_uart5_pin_mux();
#endif
}

void set_mux_conf_regs(void)
{
	enable_board_pin_mux();
}

const struct ctrl_ioregs ioregs_evmsk = {
	.cm0ioctl		= MT41J128MJT125_IOCTRL_VALUE,
	.cm1ioctl		= MT41J128MJT125_IOCTRL_VALUE,
	.cm2ioctl		= MT41J128MJT125_IOCTRL_VALUE,
	.dt0ioctl		= MT41J128MJT125_IOCTRL_VALUE,
	.dt1ioctl		= MT41J128MJT125_IOCTRL_VALUE,
};

const struct ctrl_ioregs ioregs_bonelt = {
	.cm0ioctl		= MT41K256M16HA125E_IOCTRL_VALUE,
	.cm1ioctl		= MT41K256M16HA125E_IOCTRL_VALUE,
	.cm2ioctl		= MT41K256M16HA125E_IOCTRL_VALUE,
	.dt0ioctl		= MT41K256M16HA125E_IOCTRL_VALUE,
	.dt1ioctl		= MT41K256M16HA125E_IOCTRL_VALUE,
};

const struct ctrl_ioregs ioregs_evm15 = {
	.cm0ioctl		= MT41J512M8RH125_IOCTRL_VALUE,
	.cm1ioctl		= MT41J512M8RH125_IOCTRL_VALUE,
	.cm2ioctl		= MT41J512M8RH125_IOCTRL_VALUE,
	.dt0ioctl		= MT41J512M8RH125_IOCTRL_VALUE,
	.dt1ioctl		= MT41J512M8RH125_IOCTRL_VALUE,
};

const struct ctrl_ioregs ioregs = {
	.cm0ioctl		= MT47H128M16RT25E_IOCTRL_VALUE,
	.cm1ioctl		= MT47H128M16RT25E_IOCTRL_VALUE,
	.cm2ioctl		= MT47H128M16RT25E_IOCTRL_VALUE,
	.dt0ioctl		= MT47H128M16RT25E_IOCTRL_VALUE,
	.dt1ioctl		= MT47H128M16RT25E_IOCTRL_VALUE,
};

void sdram_init(void)
{
	config_ddr(400, &ioregs_bonelt,
			&ddr3_beagleblack_data,
			&ddr3_beagleblack_cmd_ctrl_data,
			&ddr3_beagleblack_emif_reg_data, 0);
}
#endif

#if defined(CONFIG_CLOCK_SYNTHESIZER) && (!defined(CONFIG_SPL_BUILD) || \
	(defined(CONFIG_SPL_ETH_SUPPORT) && defined(CONFIG_SPL_BUILD)))
static void request_and_set_gpio(int gpio, char *name, int val)
{
	int ret;

	ret = gpio_request(gpio, name);
	if (ret < 0) {
		printf("%s: Unable to request %s\n", __func__, name);
		return;
	}

	ret = gpio_direction_output(gpio, 0);
	if (ret < 0) {
		printf("%s: Unable to set %s  as output\n", __func__, name);
		goto err_free_gpio;
	}

	gpio_set_value(gpio, val);

	return;

err_free_gpio:
	gpio_free(gpio);
}

#define REQUEST_AND_SET_GPIO(N)	request_and_set_gpio(N, #N, 1);
#define REQUEST_AND_CLR_GPIO(N)	request_and_set_gpio(N, #N, 0);


#endif

#if defined(CONFIG_OF_BOARD_SETUP) && defined(CONFIG_OF_CONTROL) && \
	defined(CONFIG_DM_ETH) && defined(CONFIG_DRIVER_TI_CPSW)

#define MAX_CPSW_SLAVES	2

/* At the moment, we do not want to stop booting for any failures here */
int ft_board_setup(void *fdt, bd_t *bd)
{
	const char *slave_path, *enet_name;
	int enetnode, slavenode, phynode;
	struct udevice *ethdev;
	char alias[16];
	u32 phy_id[2];
	int phy_addr;
	int i, ret;

	for (i = 0; i < MAX_CPSW_SLAVES; i++) {
		sprintf(alias, "ethernet%d", i);

		slave_path = fdt_get_alias(fdt, alias);
		if (!slave_path)
			continue;

		slavenode = fdt_path_offset(fdt, slave_path);
		if (slavenode < 0)
			continue;

		enetnode = fdt_parent_offset(fdt, slavenode);
		enet_name = fdt_get_name(fdt, enetnode, NULL);

		ethdev = eth_get_dev_by_name(enet_name);
		if (!ethdev)
			continue;

		phy_addr = cpsw_get_slave_phy_addr(ethdev, i);

		/* check for phy_id as well as phy-handle properties */
		ret = fdtdec_get_int_array_count(fdt, slavenode, "phy_id",
						 phy_id, 2);
		if (ret == 2) {
			if (phy_id[1] != phy_addr) {
				printf("fixing up phy_id for %s, old: %d, new: %d\n",
				       alias, phy_id[1], phy_addr);

				phy_id[0] = cpu_to_fdt32(phy_id[0]);
				phy_id[1] = cpu_to_fdt32(phy_addr);
				do_fixup_by_path(fdt, slave_path, "phy_id",
						 phy_id, sizeof(phy_id), 0);
			}
		} else {
			phynode = fdtdec_lookup_phandle(fdt, slavenode,
							"phy-handle");
			if (phynode < 0)
				continue;

			ret = fdtdec_get_int(fdt, phynode, "reg", -ENOENT);
			if (ret < 0)
				continue;

			if (ret != phy_addr) {
				printf("fixing up phy-handle for %s, old: %d, new: %d\n",
				       alias, ret, phy_addr);

				fdt_setprop_u32(fdt, phynode, "reg",
						cpu_to_fdt32(phy_addr));
			}
		}
	}
	return 0;
}

#endif

/*
 * Basic board specific setup.  Pinmux has been handled already.
 */
int board_init(void)
{
#if defined(CONFIG_HW_WATCHDOG)
	hw_watchdog_init();
#endif

	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;

	return 0;
}

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
	struct udevice *dev;
#if !defined(CONFIG_SPL_BUILD)
	uint8_t mac_addr[6];
	uint32_t mac_hi, mac_lo;
#endif

#ifdef CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG

	if (get_device_type() == HS_DEVICE)
		env_set("boot_fit", "1");
#endif

#if !defined(CONFIG_SPL_BUILD)
	/* try reading mac address from efuse */
	mac_lo = readl(&cdev->macid0l);
	mac_hi = readl(&cdev->macid0h);
	mac_addr[0] = mac_hi & 0xFF;
	mac_addr[1] = (mac_hi & 0xFF00) >> 8;
	mac_addr[2] = (mac_hi & 0xFF0000) >> 16;
	mac_addr[3] = (mac_hi & 0xFF000000) >> 24;
	mac_addr[4] = mac_lo & 0xFF;
	mac_addr[5] = (mac_lo & 0xFF00) >> 8;

	if (!env_get("ethaddr")) {
		printf("<ethaddr> not set. Validating first E-fuse MAC\n");

		if (is_valid_ethaddr(mac_addr))
			eth_env_set_enetaddr("ethaddr", mac_addr);
	}

	mac_lo = readl(&cdev->macid1l);
	mac_hi = readl(&cdev->macid1h);
	mac_addr[0] = mac_hi & 0xFF;
	mac_addr[1] = (mac_hi & 0xFF00) >> 8;
	mac_addr[2] = (mac_hi & 0xFF0000) >> 16;
	mac_addr[3] = (mac_hi & 0xFF000000) >> 24;
	mac_addr[4] = mac_lo & 0xFF;
	mac_addr[5] = (mac_lo & 0xFF00) >> 8;

#endif

	if (!env_get("serial#")) {
		char *board_serial = env_get("board_serial");
		char *ethaddr = env_get("ethaddr");

		if (!board_serial || !strncmp(board_serial, "unknown", 7))
			env_set("serial#", ethaddr);
		else
			env_set("serial#", board_serial);
	}

	/* Just probe the potentially supported cdce913 device */
	uclass_get_device(UCLASS_CLK, 0, &dev);

	return 0;
}
#endif

/* CPSW platdata */
#if !CONFIG_IS_ENABLED(OF_CONTROL)
struct cpsw_slave_data slave_data[] = {
	{
		.slave_reg_ofs  = CPSW_SLAVE0_OFFSET,
		.sliver_reg_ofs = CPSW_SLIVER0_OFFSET,
		.phy_addr       = 0,
	},
	{
		.slave_reg_ofs  = CPSW_SLAVE1_OFFSET,
		.sliver_reg_ofs = CPSW_SLIVER1_OFFSET,
		.phy_addr       = 1,
	},
};

struct cpsw_platform_data am335_eth_data = {
	.cpsw_base		= CPSW_BASE,
	.version		= CPSW_CTRL_VERSION_2,
	.bd_ram_ofs		= CPSW_BD_OFFSET,
	.ale_reg_ofs		= CPSW_ALE_OFFSET,
	.cpdma_reg_ofs		= CPSW_CPDMA_OFFSET,
	.mdio_div		= CPSW_MDIO_DIV,
	.host_port_reg_ofs	= CPSW_HOST_PORT_OFFSET,
	.channels		= 8,
	.slaves			= 2,
	.slave_data		= slave_data,
	.ale_entries		= 1024,
	.bd_ram_ofs		= 0x2000,
	.mac_control		= 0x20,
	.active_slave		= 0,
	.mdio_base		= 0x4a101000,
	.gmii_sel		= 0x44e10650,
	.phy_sel_compat		= "ti,am3352-cpsw-phy-sel",
	.syscon_addr		= 0x44e10630,
	.macid_sel_compat	= "cpsw,am33xx",
};

struct eth_pdata cpsw_pdata = {
	.iobase = 0x4a100000,
	.phy_interface = 0,
	.priv_pdata = &am335_eth_data,
};

U_BOOT_DEVICE(am335x_eth) = {
	.name = "eth_cpsw",
	.platdata = &cpsw_pdata,
};
#endif

#ifdef CONFIG_SPL_LOAD_FIT
int board_fit_config_name_match(const char *name)
{
	return 0;
}
#endif

#ifdef CONFIG_TI_SECURE_DEVICE
void board_fit_image_post_process(const void *fit, int node, void **p_image,
				  size_t *p_size)
{
	secure_boot_verify_image(p_image, p_size);
}
#endif

#if !CONFIG_IS_ENABLED(OF_CONTROL)
static const struct omap_hsmmc_plat am335x_mmc0_platdata = {
	.base_addr = (struct hsmmc *)OMAP_HSMMC1_BASE,
	.cfg.host_caps = MMC_MODE_HS_52MHz | MMC_MODE_HS | MMC_MODE_4BIT,
	.cfg.f_min = 400000,
	.cfg.f_max = 52000000,
	.cfg.voltages = MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_VDD_165_195,
	.cfg.b_max = CONFIG_SYS_MMC_MAX_BLK_COUNT,
};

U_BOOT_DEVICE(am335x_mmc0) = {
	.name = "omap_hsmmc",
	.platdata = &am335x_mmc0_platdata,
};

static const struct omap_hsmmc_plat am335x_mmc1_platdata = {
	.base_addr = (struct hsmmc *)OMAP_HSMMC2_BASE,
	.cfg.host_caps = MMC_MODE_HS_52MHz | MMC_MODE_HS | MMC_MODE_8BIT,
	.cfg.f_min = 400000,
	.cfg.f_max = 52000000,
	.cfg.voltages = MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_VDD_165_195,
	.cfg.b_max = CONFIG_SYS_MMC_MAX_BLK_COUNT,
};

U_BOOT_DEVICE(am335x_mmc1) = {
	.name = "omap_hsmmc",
	.platdata = &am335x_mmc1_platdata,
};
#endif
