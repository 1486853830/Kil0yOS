#include "drivers/rtc.h"
#include "drivers/io.h"

#define CMOS_INDEX  0x70
#define CMOS_DATA   0x71

#define RTC_SEC     0x00
#define RTC_MIN     0x02
#define RTC_HOUR    0x04
#define RTC_DAY     0x07
#define RTC_MONTH   0x08
#define RTC_YEAR    0x09
#define RTC_CENTURY 0x32
#define RTC_STAT_A  0x0A
#define RTC_STAT_B  0x0B

#define RTC_B_24H   0x02
#define RTC_B_BIN   0x04

static uint8_t cmos_read(uint8_t reg) {
    outb(CMOS_INDEX, reg);
    return inb(CMOS_DATA);
}

static uint8_t bcd_to_bin(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

void rtc_init(void) {
}

int rtc_read(rtc_time_t* time) {
    if (!time) return -1;

    /* Wait for update in progress to clear */
    while (cmos_read(RTC_STAT_A) & 0x80);

    uint8_t second  = cmos_read(RTC_SEC);
    uint8_t minute  = cmos_read(RTC_MIN);
    uint8_t hour    = cmos_read(RTC_HOUR);
    uint8_t day     = cmos_read(RTC_DAY);
    uint8_t month   = cmos_read(RTC_MONTH);
    uint8_t year    = cmos_read(RTC_YEAR);
    uint8_t century = cmos_read(RTC_CENTURY);

    uint8_t stat_b = cmos_read(RTC_STAT_B);

    /* Convert BCD to binary if needed */
    if (!(stat_b & RTC_B_BIN)) {
        second  = bcd_to_bin(second);
        minute  = bcd_to_bin(minute);
        hour    = bcd_to_bin(hour & 0x7F);
        day     = bcd_to_bin(day);
        month   = bcd_to_bin(month);
        year    = bcd_to_bin(year);
        if (century != 0 && century != 0xFF) {
            century = bcd_to_bin(century);
        }
    }

    /* Handle 12-hour format */
    if (!(stat_b & RTC_B_24H)) {
        if (hour & 0x80) {
            hour = ((hour & 0x7F) % 12) + 12;
        } else {
            hour = (hour & 0x7F) % 12;
        }
    }

    time->second  = second;
    time->minute  = minute;
    time->hour    = hour;
    time->day     = day;
    time->month   = month;

    if (century == 0 || century == 0xFF) {
        time->year = 2000 + year;
    } else {
        time->year = century * 100 + year;
    }

    time->century = (time->year / 100);

    return 0;
}
