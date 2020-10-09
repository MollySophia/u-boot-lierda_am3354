// SPDX-License-Identifier: GPL-2.0+
/*
 * J7200 specific clock platform data
 *
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
 */
#include "k3-clk.h"

static const char * const gluelogic_hfosc0_clkout_parents[] = {
	"osc_19_2_mhz",
	"osc_20_mhz",
	"osc_24_mhz",
	"osc_25_mhz",
	"osc_26_mhz",
	"osc_27_mhz",
};

static const char * const main_pll_hfosc_sel_out1_parents[] = {
	"gluelogic_hfosc0_clkout",
};

static const char * const mcu_ospi0_iclk_sel_out0_parents[] = {
	"fss_mcu_0_ospi_0_ospi_oclk_clk",
};

static const char * const wkup_fref_clksel_out0_parents[] = {
	"gluelogic_hfosc0_clkout",
	"j7vc_wakeup_16ff_wkup_0_wkup_rcosc_12p5m_clk",
};

static const char * const k3_pll_ctrl_wrap_wkup_0_sysclkout_clk_parents[] = {
	"wkup_fref_clksel_out0",
	"hsdiv1_16fft_mcu_0_hsdivout0_clk",
};

static const char * const mcu_ospi_ref_clk_sel_out0_parents[] = {
	"hsdiv4_16fft_mcu_1_hsdivout4_clk",
	"hsdiv4_16fft_mcu_2_hsdivout4_clk",
};

static const char * const mcuusart_clk_sel_out0_parents[] = {
	"hsdiv4_16fft_mcu_1_hsdivout3_clk",
	"postdiv2_16fft_main_1_hsdivout5_clk",
};

static const char * const wkup_i2c0_mcupll_bypass_clksel_out0_parents[] = {
	"hsdiv4_16fft_mcu_1_hsdivout3_clk",
	"gluelogic_hfosc0_clkout",
};

static const char * const main_pll_hfosc_sel_out0_parents[] = {
	"gluelogic_hfosc0_clkout",
};

static const char * const main_pll_hfosc_sel_out12_parents[] = {
	"gluelogic_hfosc0_clkout",
};

static const char * const main_pll_hfosc_sel_out3_parents[] = {
	"gluelogic_hfosc0_clkout",
};

static const char * const main_pll_hfosc_sel_out7_parents[] = {
	"gluelogic_hfosc0_clkout",
};

static const char * const main_pll_hfosc_sel_out8_parents[] = {
	"gluelogic_hfosc0_clkout",
};

static const char * const k3_pll_ctrl_wrap_main_0_sysclkout_clk_parents[] = {
	"main_pll_hfosc_sel_out0",
	"hsdiv4_16fft_main_0_hsdivout0_clk",
};

static const char * const emmcsd_refclk_sel_out0_parents[] = {
	"hsdiv4_16fft_main_0_hsdivout2_clk",
	"hsdiv4_16fft_main_1_hsdivout2_clk",
	"hsdiv4_16fft_main_3_hsdivout2_clk",
	"hsdiv4_16fft_main_3_hsdivout2_clk",
};

static const char * const emmcsd_refclk_sel_out1_parents[] = {
	"hsdiv4_16fft_main_0_hsdivout2_clk",
	"hsdiv4_16fft_main_1_hsdivout2_clk",
	"hsdiv4_16fft_main_3_hsdivout2_clk",
	"hsdiv4_16fft_main_3_hsdivout2_clk",
};

static const char * const gtc_clk_mux_out0_parents[] = {
	"hsdiv4_16fft_main_3_hsdivout1_clk",
	"postdiv2_16fft_main_0_hsdivout6_clk",
	"hsdiv4_16fft_mcu_2_hsdivout1_clk",
	"k3_pll_ctrl_wrap_main_0_chip_div1_clk_clk",
};

static const struct clk_data clk_list[] = {
	CLK_FIXED_RATE("osc_27_mhz", 27000000, 0),
	CLK_FIXED_RATE("osc_26_mhz", 26000000, 0),
	CLK_FIXED_RATE("osc_25_mhz", 25000000, 0),
	CLK_FIXED_RATE("osc_24_mhz", 24000000, 0),
	CLK_FIXED_RATE("osc_20_mhz", 20000000, 0),
	CLK_FIXED_RATE("osc_19_2_mhz", 19200000, 0),
	CLK_MUX("gluelogic_hfosc0_clkout", gluelogic_hfosc0_clkout_parents, 6, 0x43000030, 0, 3, 0),
	CLK_MUX("main_pll_hfosc_sel_out1", main_pll_hfosc_sel_out1_parents, 1, 0x43008084, 0, 1, 0),
	CLK_FIXED_RATE("fss_mcu_0_ospi_0_ospi_oclk_clk", 0, 0),
	CLK_FIXED_RATE("j7vc_wakeup_16ff_wkup_0_wkup_rcosc_12p5m_clk", 12500000, 0),
	CLK_MUX("mcu_ospi0_iclk_sel_out0", mcu_ospi0_iclk_sel_out0_parents, 1, 0x40f08030, 4, 1, 0),
	CLK_PLL_DEFFREQ("pllfracf_ssmod_16fft_main_1_foutvcop_clk", "main_pll_hfosc_sel_out1", 0x681000, 0, 1920000000),
	CLK_DIV("pllfracf_ssmod_16fft_main_1_foutpostdiv_clk_subdiv", "pllfracf_ssmod_16fft_main_1_foutvcop_clk", 0x680038, 24, 3, 0),
	CLK_DIV("pllfracf_ssmod_16fft_main_1_foutpostdiv_clk", "pllfracf_ssmod_16fft_main_1_foutpostdiv_clk_subdiv", 0x680038, 16, 3, 0),
	CLK_DIV("postdiv2_16fft_main_1_hsdivout5_clk", "pllfracf_ssmod_16fft_main_1_foutpostdiv_clk", 0x681094, 0, 7, 0),
	CLK_MUX("wkup_fref_clksel_out0", wkup_fref_clksel_out0_parents, 2, 0x43008050, 8, 1, 0),
	CLK_PLL("pllfracf_ssmod_16fft_mcu_0_foutvcop_clk", "wkup_fref_clksel_out0", 0x40d00000, 0),
	CLK_PLL("pllfracf_ssmod_16fft_mcu_1_foutvcop_clk", "wkup_fref_clksel_out0", 0x40d01000, 0),
	CLK_PLL("pllfracf_ssmod_16fft_mcu_2_foutvcop_clk", "wkup_fref_clksel_out0", 0x40d02000, 0),
	CLK_DIV("hsdiv1_16fft_mcu_0_hsdivout0_clk", "pllfracf_ssmod_16fft_mcu_0_foutvcop_clk", 0x40d00080, 0, 7, 0),
	CLK_DIV("hsdiv4_16fft_mcu_1_hsdivout3_clk", "pllfracf_ssmod_16fft_mcu_1_foutvcop_clk", 0x40d0108c, 0, 7, 0),
	CLK_DIV("hsdiv4_16fft_mcu_1_hsdivout4_clk", "pllfracf_ssmod_16fft_mcu_1_foutvcop_clk", 0x40d01090, 0, 7, 0),
	CLK_DIV("hsdiv4_16fft_mcu_2_hsdivout4_clk", "pllfracf_ssmod_16fft_mcu_2_foutvcop_clk", 0x40d02090, 0, 7, 0),
	CLK_MUX_PLLCTRL("k3_pll_ctrl_wrap_wkup_0_sysclkout_clk", k3_pll_ctrl_wrap_wkup_0_sysclkout_clk_parents, 2, 0x42010000, 0),
	CLK_DIV("k3_pll_ctrl_wrap_wkup_0_chip_div1_clk_clk", "k3_pll_ctrl_wrap_wkup_0_sysclkout_clk", 0x42010118, 0, 5, 0),
	CLK_MUX("mcu_ospi_ref_clk_sel_out0", mcu_ospi_ref_clk_sel_out0_parents, 2, 0x40f08030, 0, 1, 0),
	CLK_MUX("mcuusart_clk_sel_out0", mcuusart_clk_sel_out0_parents, 2, 0x40f081c0, 0, 1, 0),
	CLK_MUX("wkup_i2c0_mcupll_bypass_clksel_out0", wkup_i2c0_mcupll_bypass_clksel_out0_parents, 2, 0x43008060, 0, 1, 0),
	CLK_MUX("main_pll_hfosc_sel_out0", main_pll_hfosc_sel_out0_parents, 1, 0x43008080, 0, 1, 0),
	CLK_MUX("main_pll_hfosc_sel_out12", main_pll_hfosc_sel_out12_parents, 1, 0x430080b0, 0, 1, 0),
	CLK_MUX("main_pll_hfosc_sel_out3", main_pll_hfosc_sel_out3_parents, 1, 0x4300808c, 0, 1, 0),
	CLK_MUX("main_pll_hfosc_sel_out7", main_pll_hfosc_sel_out7_parents, 1, 0x4300809c, 0, 1, 0),
	CLK_MUX("main_pll_hfosc_sel_out8", main_pll_hfosc_sel_out8_parents, 1, 0x430080a0, 0, 1, 0),
	CLK_FIXED_RATE("emmcsd4ss_main_0_emmcsdss_io_clk_o", 0, 0),
	CLK_DIV_DEFFREQ("hsdiv4_16fft_main_1_hsdivout0_clk", "pllfracf_ssmod_16fft_main_1_foutvcop_clk", 0x681080, 0, 7, 0, 192000000),
	CLK_DIV("hsdiv4_16fft_main_1_hsdivout2_clk", "pllfracf_ssmod_16fft_main_1_foutvcop_clk", 0x681088, 0, 7, 0),
	CLK_DIV("hsdiv4_16fft_mcu_2_hsdivout1_clk", "pllfracf_ssmod_16fft_mcu_2_foutvcop_clk", 0x40d02084, 0, 7, 0),
	CLK_PLL("pllfracf_ssmod_16fft_main_0_foutvcop_clk", "main_pll_hfosc_sel_out0", 0x680000, 0),
	CLK_DIV("pllfracf_ssmod_16fft_main_0_foutpostdiv_clk_subdiv", "pllfracf_ssmod_16fft_main_0_foutvcop_clk", 0x680038, 24, 3, 0),
	CLK_DIV("pllfracf_ssmod_16fft_main_0_foutpostdiv_clk", "pllfracf_ssmod_16fft_main_0_foutpostdiv_clk_subdiv", 0x680038, 16, 3, 0),
	CLK_PLL("pllfracf_ssmod_16fft_main_12_foutvcop_clk", "main_pll_hfosc_sel_out12", 0x68c000, 0),
	CLK_PLL("pllfracf_ssmod_16fft_main_3_foutvcop_clk", "main_pll_hfosc_sel_out3", 0x683000, 0),
	CLK_PLL("pllfracf_ssmod_16fft_main_7_foutvcop_clk", "main_pll_hfosc_sel_out7", 0x687000, 0),
	CLK_PLL("pllfracf_ssmod_16fft_main_8_foutvcop_clk", "main_pll_hfosc_sel_out8", 0x688000, 0),
	CLK_DIV("postdiv2_16fft_main_0_hsdivout6_clk", "pllfracf_ssmod_16fft_main_0_foutpostdiv_clk", 0x680098, 0, 7, 0),
	CLK_DIV_DEFFREQ("usart_programmable_clock_divider_out0", "hsdiv4_16fft_main_1_hsdivout0_clk", 0x1081c0, 0, 2, 0, 48000000),
	CLK_DIV("hsdiv0_16fft_main_12_hsdivout0_clk", "pllfracf_ssmod_16fft_main_12_foutvcop_clk", 0x68c080, 0, 7, 0),
	CLK_DIV("hsdiv0_16fft_main_7_hsdivout0_clk", "pllfracf_ssmod_16fft_main_7_foutvcop_clk", 0x687080, 0, 7, 0),
	CLK_DIV("hsdiv0_16fft_main_8_hsdivout0_clk", "pllfracf_ssmod_16fft_main_8_foutvcop_clk", 0x688080, 0, 7, 0),
	CLK_DIV("hsdiv4_16fft_main_0_hsdivout0_clk", "pllfracf_ssmod_16fft_main_0_foutvcop_clk", 0x680080, 0, 7, 0),
	CLK_DIV("hsdiv4_16fft_main_0_hsdivout1_clk", "pllfracf_ssmod_16fft_main_0_foutvcop_clk", 0x680084, 0, 7, 0),
	CLK_DIV("hsdiv4_16fft_main_0_hsdivout2_clk", "pllfracf_ssmod_16fft_main_0_foutvcop_clk", 0x680088, 0, 7, 0),
	CLK_DIV("hsdiv4_16fft_main_0_hsdivout3_clk", "pllfracf_ssmod_16fft_main_0_foutvcop_clk", 0x68008c, 0, 7, 0),
	CLK_DIV("hsdiv4_16fft_main_0_hsdivout4_clk", "pllfracf_ssmod_16fft_main_0_foutvcop_clk", 0x680090, 0, 7, 0),
	CLK_DIV("hsdiv4_16fft_main_3_hsdivout1_clk", "pllfracf_ssmod_16fft_main_3_foutvcop_clk", 0x683084, 0, 7, 0),
	CLK_DIV("hsdiv4_16fft_main_3_hsdivout2_clk", "pllfracf_ssmod_16fft_main_3_foutvcop_clk", 0x683088, 0, 7, 0),
	CLK_MUX_PLLCTRL("k3_pll_ctrl_wrap_main_0_sysclkout_clk", k3_pll_ctrl_wrap_main_0_sysclkout_clk_parents, 2, 0x410000, 0),
	CLK_DIV("k3_pll_ctrl_wrap_main_0_chip_div1_clk_clk", "k3_pll_ctrl_wrap_main_0_sysclkout_clk", 0x410118, 0, 5, 0),
	CLK_MUX("emmcsd_refclk_sel_out0", emmcsd_refclk_sel_out0_parents, 4, 0x1080b0, 0, 2, 0),
	CLK_MUX("emmcsd_refclk_sel_out1", emmcsd_refclk_sel_out1_parents, 4, 0x1080b4, 0, 2, 0),
	CLK_MUX("gtc_clk_mux_out0", gtc_clk_mux_out0_parents, 4, 0x108030, 0, 4, 0),
	CLK_DIV("k3_pll_ctrl_wrap_main_0_chip_div24_clk_clk", "k3_pll_ctrl_wrap_main_0_sysclkout_clk", 0x41011c, 0, 5, 0),
	CLK_DIV("k3_pll_ctrl_wrap_wkup_0_chip_div24_clk_clk", "k3_pll_ctrl_wrap_wkup_0_sysclkout_clk", 0x4201011c, 0, 5, 0),
};

static const struct dev_clk soc_dev_clk_data[] = {
	DEV_CLK(4, 0, "hsdiv0_16fft_main_8_hsdivout0_clk"),
	DEV_CLK(4, 1, "hsdiv0_16fft_main_7_hsdivout0_clk"),
	DEV_CLK(4, 2, "k3_pll_ctrl_wrap_main_0_chip_div1_clk_clk"),
	DEV_CLK(8, 5, "hsdiv0_16fft_main_12_hsdivout0_clk"),
	DEV_CLK(8, 0, "k3_pll_ctrl_wrap_main_0_chip_div1_clk_clk"),
	DEV_CLK(30, 4, "hsdiv4_16fft_main_0_hsdivout1_clk"),
	DEV_CLK(30, 7, "hsdiv4_16fft_main_0_hsdivout2_clk"),
	DEV_CLK(30, 2, "hsdiv4_16fft_main_0_hsdivout3_clk"),
	DEV_CLK(30, 8, "hsdiv4_16fft_main_0_hsdivout4_clk"),
	DEV_CLK(30, 6, "gluelogic_hfosc0_clkout"),
	DEV_CLK(30, 11, "k3_pll_ctrl_wrap_main_0_chip_div1_clk_clk"),
	DEV_CLK(30, 9, "gluelogic_hfosc0_clkout"),
	DEV_CLK(30, 12, "j7vc_wakeup_16ff_wkup_0_wkup_rcosc_12p5m_clk"),
	DEV_CLK(30, 0, "k3_pll_ctrl_wrap_main_0_chip_div1_clk_clk"),
	DEV_CLK(30, 5, "k3_pll_ctrl_wrap_main_0_chip_div1_clk_clk"),
	DEV_CLK(61, 1, "gtc_clk_mux_out0"),
	DEV_CLK(61, 2, "hsdiv4_16fft_main_3_hsdivout1_clk"),
	DEV_CLK(61, 3, "postdiv2_16fft_main_0_hsdivout6_clk"),
	DEV_CLK(61, 16, "hsdiv4_16fft_mcu_2_hsdivout1_clk"),
	DEV_CLK(61, 17, "k3_pll_ctrl_wrap_main_0_chip_div1_clk_clk"),
	DEV_CLK(61, 0, "k3_pll_ctrl_wrap_main_0_chip_div1_clk_clk"),
	DEV_CLK(91, 0, "k3_pll_ctrl_wrap_main_0_chip_div1_clk_clk"),
	DEV_CLK(91, 3, "emmcsd_refclk_sel_out0"),
	DEV_CLK(91, 4, "hsdiv4_16fft_main_0_hsdivout2_clk"),
	DEV_CLK(91, 5, "hsdiv4_16fft_main_1_hsdivout2_clk"),
	DEV_CLK(91, 6, "hsdiv4_16fft_main_3_hsdivout2_clk"),
	DEV_CLK(91, 7, "hsdiv4_16fft_main_3_hsdivout2_clk"),
	DEV_CLK(92, 0, "emmcsd4ss_main_0_emmcsdss_io_clk_o"),
	DEV_CLK(92, 1, "k3_pll_ctrl_wrap_main_0_chip_div1_clk_clk"),
	DEV_CLK(92, 2, "emmcsd_refclk_sel_out1"),
	DEV_CLK(92, 3, "hsdiv4_16fft_main_0_hsdivout2_clk"),
	DEV_CLK(92, 4, "hsdiv4_16fft_main_1_hsdivout2_clk"),
	DEV_CLK(92, 5, "hsdiv4_16fft_main_3_hsdivout2_clk"),
	DEV_CLK(92, 6, "hsdiv4_16fft_main_3_hsdivout2_clk"),
	DEV_CLK(103, 3, "k3_pll_ctrl_wrap_wkup_0_chip_div1_clk_clk"),
	DEV_CLK(103, 5, "mcu_ospi0_iclk_sel_out0"),
	DEV_CLK(103, 7, "fss_mcu_0_ospi_0_ospi_oclk_clk"),
	DEV_CLK(103, 8, "k3_pll_ctrl_wrap_wkup_0_chip_div1_clk_clk"),
	DEV_CLK(103, 0, "mcu_ospi_ref_clk_sel_out0"),
	DEV_CLK(103, 1, "hsdiv4_16fft_mcu_1_hsdivout4_clk"),
	DEV_CLK(103, 2, "hsdiv4_16fft_mcu_2_hsdivout4_clk"),
	DEV_CLK(104, 1, "k3_pll_ctrl_wrap_wkup_0_chip_div1_clk_clk"),
	DEV_CLK(104, 7, "k3_pll_ctrl_wrap_wkup_0_chip_div1_clk_clk"),
	DEV_CLK(104, 0, "hsdiv4_16fft_mcu_1_hsdivout4_clk"),
	DEV_CLK(133, 1, "k3_pll_ctrl_wrap_main_0_chip_div1_clk_clk"),
	DEV_CLK(133, 0, "k3_pll_ctrl_wrap_main_0_chip_div1_clk_clk"),
	DEV_CLK(138, 1, "k3_pll_ctrl_wrap_wkup_0_chip_div1_clk_clk"),
	DEV_CLK(138, 0, "k3_pll_ctrl_wrap_wkup_0_chip_div1_clk_clk"),
	DEV_CLK(146, 2, "usart_programmable_clock_divider_out0"),
	DEV_CLK(146, 3, "k3_pll_ctrl_wrap_main_0_chip_div1_clk_clk"),
	DEV_CLK(149, 2, "mcuusart_clk_sel_out0"),
	DEV_CLK(149, 3, "hsdiv4_16fft_mcu_1_hsdivout3_clk"),
	DEV_CLK(149, 4, "postdiv2_16fft_main_1_hsdivout5_clk"),
	DEV_CLK(149, 5, "k3_pll_ctrl_wrap_wkup_0_chip_div1_clk_clk"),
	DEV_CLK(154, 0, "j7vc_wakeup_16ff_wkup_0_wkup_rcosc_12p5m_clk"),
	DEV_CLK(154, 2, "gluelogic_hfosc0_clkout"),
	DEV_CLK(154, 1, "k3_pll_ctrl_wrap_wkup_0_chip_div1_clk_clk"),
	DEV_CLK(197, 2, "k3_pll_ctrl_wrap_wkup_0_chip_div1_clk_clk"),
	DEV_CLK(197, 1, "wkup_i2c0_mcupll_bypass_clksel_out0"),
	DEV_CLK(202, 2, "hsdiv0_16fft_main_8_hsdivout0_clk"),
	DEV_CLK(203, 0, "hsdiv0_16fft_main_8_hsdivout0_clk"),
};

const struct ti_k3_clk_platdata j7200_clk_platdata = {
	.clk_list = clk_list,
	.clk_list_cnt = 63,
	.soc_dev_clk_data = soc_dev_clk_data,
	.soc_dev_clk_data_cnt = 61,
};
