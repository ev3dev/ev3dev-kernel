// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2012 Texas Instruments Incorporated - https://www.ti.com/
 *
 * Modified from mach-omap/omap2/board-generic.c
 */
#include <linux/io.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/irqdomain.h>

#include <asm/mach/arch.h>

#include "common.h"
#include "da8xx.h"

#include "legoev3-fiq.h"
#include "irqs.h"

#define LEGOEV3_DT 1
#include <../../../drivers/lego/ev3/legoev3_i2c.h>


#define DA8XX_EHRPWM0_BASE	0x01F00000

/* Convert GPIO signal to GPIO pin number */
#define GPIO_TO_PIN(bank, gpio)	(16 * (bank) + (gpio))

/*
 * Used by FIQ handler routine to notify the OS when status has changed. See
 * arch/arm/mach-davinci/legoev3-fiq.c. This pin is TP4 in the lms2012 source
 * code, so it should be safe to use.
 */
#define EV3_FIQ_STAT_PIN	GPIO_TO_PIN(2, 7)
#define EV3_IN1_PIN6_PIN	GPIO_TO_PIN(0, 15)
#define EV3_IN1_I2C_CLK_PIN	GPIO_TO_PIN(1, 0)
#define EV3_IN2_PIN6_PIN	GPIO_TO_PIN(0, 13)
#define EV3_IN2_I2C_CLK_PIN	GPIO_TO_PIN(8, 3)
#define EV3_IN3_PIN6_PIN	GPIO_TO_PIN(1, 14)
#define EV3_IN3_I2C_CLK_PIN	GPIO_TO_PIN(1, 12)
#define EV3_IN4_PIN6_PIN	GPIO_TO_PIN(1, 15)
#define EV3_IN4_I2C_CLK_PIN	GPIO_TO_PIN(1, 11)

static struct legoev3_fiq_platform_data legoev3_in_port_i2c_platform_data = {
	.intc_mem_base		= DA8XX_CP_INTC_BASE,
	.intc_mem_size		= 0x608,
	.gpio_mem_base		= DA8XX_GPIO_BASE,
	.gpio_mem_size		= 0xD8,
	.ehrpwm_mem_base	= DA8XX_EHRPWM0_BASE,
	.ehrpwm_mem_size	= 0x1FFF,
	.timer_irq		= IRQ_DA8XX_TINT34_1,
	.ehrpwm_irq		= IRQ_DA8XX_EHRPWM0,
	.status_gpio		= EV3_FIQ_STAT_PIN,
};

static struct platform_device legoev3_in_port_i2c_fiq = {
	.name		= "legoev3-fiq",
	.id		= -1,
	.dev		= {
		.platform_data	= &legoev3_in_port_i2c_platform_data,
	},
};

static struct i2c_legoev3_platform_data legoev3_i2c3_data = {
	.sda_pin	= EV3_IN1_PIN6_PIN,
	.scl_pin	= EV3_IN1_I2C_CLK_PIN,
	.port_id	= EV3_PORT_IN1,
};

static struct platform_device legoev3_i2c3_device = {
	.name	= "i2c-legoev3",
	.dev	= {
		.platform_data	= &legoev3_i2c3_data,
	},
	.id	= 3,
};

static struct i2c_legoev3_platform_data legoev3_i2c4_data = {
	.sda_pin	= EV3_IN2_PIN6_PIN,
	.scl_pin	= EV3_IN2_I2C_CLK_PIN,
	.port_id	= EV3_PORT_IN2,
};

static struct platform_device legoev3_i2c4_device = {
	.name	= "i2c-legoev3",
	.dev	= {
		.platform_data	= &legoev3_i2c4_data,
	},
	.id	= 4,
};

static struct i2c_legoev3_platform_data legoev3_i2c5_data = {
	.sda_pin	= EV3_IN3_PIN6_PIN,
	.scl_pin	= EV3_IN3_I2C_CLK_PIN,
	.port_id	= EV3_PORT_IN3,
};

static struct platform_device legoev3_i2c5_device = {
	.name	= "i2c-legoev3",
	.dev	= {
		.platform_data	= &legoev3_i2c5_data,
	},
	.id	= 5,
};

static struct i2c_legoev3_platform_data legoev3_i2c6_data = {
	.sda_pin	= EV3_IN4_PIN6_PIN,
	.scl_pin	= EV3_IN4_I2C_CLK_PIN,
	.port_id	= EV3_PORT_IN4,
};

static struct platform_device legoev3_i2c6_device = {
	.name	= "i2c-legoev3",
	.dev	= {
		.platform_data	= &legoev3_i2c6_data,
	},
	.id	= 6,
};

static void __init legoev3_init_machine(void)
{
	int ret;

	davinci_pm_init();
	pdata_quirks_init();

	ret = platform_device_register(&legoev3_in_port_i2c_fiq);
	if (ret)
		pr_warn("%s: FIQ I2C backend registration failed: %d\n",
			__func__, ret);

	ret = platform_device_register(&legoev3_i2c3_device);
	if (ret)
		pr_warn("%s: Input port 1 I2C registration failed: %d\n",
			__func__, ret);

	ret = platform_device_register(&legoev3_i2c4_device);
	if (ret)
		pr_warn("%s: Input port 2 I2C registration failed: %d\n",
			__func__, ret);

	ret = platform_device_register(&legoev3_i2c5_device);
	if (ret)
		pr_warn("%s: Input port 3 I2C registration failed: %d\n",
			__func__, ret);

	ret = platform_device_register(&legoev3_i2c6_device);
	if (ret)
		pr_warn("%s: Input port 4 I2C registration failed: %d\n",
			__func__, ret);
}

static const char *const legoev3_dt_compat[] __initconst = {
	"lego,ev3",
	NULL,
};

DT_MACHINE_START(DA850_DT, "LEGO MINDSTORMS EV3")
	.map_io		= da850_init,
	.init_machine	= legoev3_init_machine,
	.dt_compat	= legoev3_dt_compat,
	.init_late	= davinci_init_late,
MACHINE_END
