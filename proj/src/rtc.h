#pragma once

#include "i8042.h"

#define RTC_ADDR_REG        0x70
#define RTC_DATA_REG        0x71

#define RTC_SECONDS         0x0
#define RTC_SECONDS_ALARM   0x1
#define RTC_MINUTES         0x2
#define RTC_MINUTES_ALARM   0x3
#define RTC_HOURS           0x4
#define RTC_HOURS_ALARM     0x5
#define RTC_DAY             0x7
#define RTC_MONTH           0x8
#define RTC_YEAR            0x9
#define RTC_REG_A           0xA
#define RTC_REG_B           0xB
#define RTC_REG_C           0xC
#define RTC_REG_D           0xD

/* RTC_REG_A BITS */

#define RTC_RS0_BIT         BIT(0)
#define RTC_RS1_BIT         BIT(1)
#define RTC_RS2_BIT         BIT(2)
#define RTC_RS3_BIT         BIT(3)
#define RTC_DV0_BIT         BIT(4)
#define RTC_DV1_BIT         BIT(5)
#define RTC_DV2_BIT         BIT(6)
#define RTC_UIP             BIT(7)

/* RTC_REG_B BITS */

#define RTC_DSE                 BIT(0)
#define RTC_MODE       			BIT(1)
#define RTC_DM         	 		BIT(2)
#define RTC_SQWE                BIT(3)
#define RTC_UIE                 BIT(4)
#define RTC_AIE                 BIT(5)
#define RTC_PIE                 BIT(6)
#define RTC_SET                 BIT(7)

/* RTC_REG_C BITS */

#define RTC_UF          BIT(4)
#define RTC_AF          BIT(5)
#define RTC_PF          BIT(6)
#define RTC_IRQF        BIT(7)

/**
 * RTC_REG_D BITS
 */

#define RTC_VRT_BIT         BIT(7)
#define RTC_IRQ             8

typedef struct {

	unsigned long seconds;
	unsigned long minutes;
	unsigned long hours;

	unsigned long day;
	unsigned long month;
	unsigned long year;

} rtc_time_t;

void rtc_wait();

int rtc_check_alarm();
void rtc_enable_alarm();
void rtc_disable_alarm();

int rtc_subscribe();
void rtc_unsubscribe();

rtc_time_t rtc_current_date();
