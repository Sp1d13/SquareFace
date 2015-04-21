#include <pebble.h>

#define KEY_INVERTFACE 10
#define KEY_INVERTROW 11
#define KEY_FORMATDATE 12
#define DISPLAY_FOR_MILLISECONDS 5000
  
enum {
  INVERTFACE = 0x0,
  INVERTROW = 0x1,
  FORMATDATE = 0x2
};

struct time_layer {
	BitmapLayer *hour1, *hour2, *minute1, *minute2;
};

struct date_layer {
  BitmapLayer *day1, *day2, *date1, *date2, *dot1, *date3, *date4, *dot2;
};

struct date_font {
  GBitmap *weekday_1_a, *weekday_1_b, *weekday_2_a, *weekday_2_b,
          *weekday_3_a, *weekday_3_b, *weekday_4_a, *weekday_4_b,
          *weekday_5_a, *weekday_5_b, *weekday_6_a, *weekday_6_b,
          *weekday_7_a, *weekday_7_b, *dot;
};

struct battery_icons {
  GBitmap *icon_20, *icon_30, *icon_40, *icon_50,
          *icon_60, *icon_70, *icon_80, *icon_90,
          *icon_100, *icon_charging, *icon_critical;
};

struct bluetooth_icons {
  GBitmap *icon_connected, *icon_disconnected;
};

struct configurable_variables {
  int32_t invertFace, invertRow, formatDate;
};