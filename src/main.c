#include <pebble.h>

static Window *w_main_window;
static InverterLayer *il_inverted;/*, *il_inverted2;*/
static BitmapLayer *bl_hour1, *bl_hour2, *bl_minute1, *bl_minute2;
static BitmapLayer *bl_day1, *bl_day2, *bl_date1, *bl_date2, *bl_dot1, *bl_date3, *bl_date4, *bl_dot2;
static BitmapLayer *bl_battery, *bl_bluetooth;
static GBitmap *gb_TIME_NUMBER[10], *gb_DATE_NUMBER[10];
static GBitmap *gb_DATE_FONT_WEEKDAY_1_a, *gb_DATE_FONT_WEEKDAY_1_b, *gb_DATE_FONT_WEEKDAY_2_a, *gb_DATE_FONT_WEEKDAY_2_b,
               *gb_DATE_FONT_WEEKDAY_3_a, *gb_DATE_FONT_WEEKDAY_3_b, *gb_DATE_FONT_WEEKDAY_4_a, *gb_DATE_FONT_WEEKDAY_4_b,
               *gb_DATE_FONT_WEEKDAY_5_a, *gb_DATE_FONT_WEEKDAY_5_b, *gb_DATE_FONT_WEEKDAY_6_a, *gb_DATE_FONT_WEEKDAY_6_b,
               *gb_DATE_FONT_WEEKDAY_7_a, *gb_DATE_FONT_WEEKDAY_7_b, *gb_DATE_FONT_DOT;
static GBitmap *gb_ICON_BATTERY_20, *gb_ICON_BATTERY_30, *gb_ICON_BATTERY_40, *gb_ICON_BATTERY_50,
               *gb_ICON_BATTERY_60, *gb_ICON_BATTERY_70, *gb_ICON_BATTERY_80, *gb_ICON_BATTERY_90,
               *gb_ICON_BATTERY_100, *gb_ICON_BATTERY_CHARGING, *gb_ICON_BATTERY_CRITICAL;
static GBitmap *gb_ICON_BLUETOOTH_CONNECTED, *gb_ICON_BLUETOOTH_DISCONNECTED;
static char *sys_locale;
static AppTimer *display_timer;
static uint32_t DISPLAY_FOR_MILLISECONDS = 5000;

static void update_time() {
  // tm-Struktur erstellen
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // 'long-lived' Puffer erstellen
  static char buffer_time[] = "0000";

  // Momentane Zeit in den Puffer schreiben
  if(clock_is_24h_style() == true)
    strftime(buffer_time, sizeof("0000"), "%H%M", tick_time);
  else
    strftime(buffer_time, sizeof("0000"), "%I%M", tick_time);
  
  GBitmap *gb_hour1 = gb_TIME_NUMBER[0];
  GBitmap *gb_hour2 = gb_TIME_NUMBER[0];
  GBitmap *gb_minute1 = gb_TIME_NUMBER[0];
  GBitmap *gb_minute2 = gb_TIME_NUMBER[0];
  
  // Bitmap für hours1 bestimmen
  switch(buffer_time[0]) {
    // 0 ist Standardwert
    //case '0': gb_hour1 = gb_TIME_NUMBER[0]; break;
    case '1': gb_hour1 = gb_TIME_NUMBER[1]; break;
    case '2': gb_hour1 = gb_TIME_NUMBER[2]; break;
    case '3': gb_hour1 = gb_TIME_NUMBER[3]; break;
    case '4': gb_hour1 = gb_TIME_NUMBER[4]; break;
    case '5': gb_hour1 = gb_TIME_NUMBER[5]; break;
    case '6': gb_hour1 = gb_TIME_NUMBER[6]; break;
    case '7': gb_hour1 = gb_TIME_NUMBER[7]; break;
    case '8': gb_hour1 = gb_TIME_NUMBER[8]; break;
    case '9': gb_hour1 = gb_TIME_NUMBER[9]; break;
  }
  
  // Bitmap für hours2 bestimmen
  switch(buffer_time[1]) {
    // 0 ist Standardwert
    //case '0': gb_hour2 = gb_TIME_NUMBER[0]; break;
    case '1': gb_hour2 = gb_TIME_NUMBER[1]; break;
    case '2': gb_hour2 = gb_TIME_NUMBER[2]; break;
    case '3': gb_hour2 = gb_TIME_NUMBER[3]; break;
    case '4': gb_hour2 = gb_TIME_NUMBER[4]; break;
    case '5': gb_hour2 = gb_TIME_NUMBER[5]; break;
    case '6': gb_hour2 = gb_TIME_NUMBER[6]; break;
    case '7': gb_hour2 = gb_TIME_NUMBER[7]; break;
    case '8': gb_hour2 = gb_TIME_NUMBER[8]; break;
    case '9': gb_hour2 = gb_TIME_NUMBER[9]; break;
  }
  
  // Bitmap für minute1 bestimmen
  switch(buffer_time[2]) {
    // 0 ist Standardwert
    //case '0': gb_minute1 = gb_TIME_NUMBER[0]; break;
    case '1': gb_minute1 = gb_TIME_NUMBER[1]; break;
    case '2': gb_minute1 = gb_TIME_NUMBER[2]; break;
    case '3': gb_minute1 = gb_TIME_NUMBER[3]; break;
    case '4': gb_minute1 = gb_TIME_NUMBER[4]; break;
    case '5': gb_minute1 = gb_TIME_NUMBER[5]; break;
    case '6': gb_minute1 = gb_TIME_NUMBER[6]; break;
    case '7': gb_minute1 = gb_TIME_NUMBER[7]; break;
    case '8': gb_minute1 = gb_TIME_NUMBER[8]; break;
    case '9': gb_minute1 = gb_TIME_NUMBER[9]; break;
  }
  
  // Bitmap für minute2 bestimmen
  switch(buffer_time[3]) {
    // 0 ist Standardwert
    //case '0': gb_minute2 = gb_TIME_NUMBER[0]; break;
    case '1': gb_minute2 = gb_TIME_NUMBER[1]; break;
    case '2': gb_minute2 = gb_TIME_NUMBER[2]; break;
    case '3': gb_minute2 = gb_TIME_NUMBER[3]; break;
    case '4': gb_minute2 = gb_TIME_NUMBER[4]; break;
    case '5': gb_minute2 = gb_TIME_NUMBER[5]; break;
    case '6': gb_minute2 = gb_TIME_NUMBER[6]; break;
    case '7': gb_minute2 = gb_TIME_NUMBER[7]; break;
    case '8': gb_minute2 = gb_TIME_NUMBER[8]; break;
    case '9': gb_minute2 = gb_TIME_NUMBER[9]; break;
  }
  
  // Bitmaps setzen
  bitmap_layer_set_bitmap(bl_hour1, gb_hour1);
  bitmap_layer_set_bitmap(bl_hour2, gb_hour2);
  bitmap_layer_set_bitmap(bl_minute1, gb_minute1);
  bitmap_layer_set_bitmap(bl_minute2, gb_minute2); 
}

static void update_date() {
  // tm-Struktur erstellen
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // 'long-lived' Puffer erstellen
  static char buffer_date[] = "00000";

  // Momentanes Datum in den Puffer schreiben
  strftime(buffer_date, sizeof("00000"), "%u%d%m", tick_time);
  
  GBitmap *gb_day1 = gb_DATE_FONT_WEEKDAY_1_a;
  GBitmap *gb_day2 = gb_DATE_FONT_WEEKDAY_1_b;
  GBitmap *gb_date1 = gb_DATE_NUMBER[0];
  GBitmap *gb_date2 = gb_DATE_NUMBER[0];
  GBitmap *gb_date3 = gb_DATE_NUMBER[0];
  GBitmap *gb_date4 = gb_DATE_NUMBER[0];
  
  // Bitmap für day1+day2 bestimmen
  switch(buffer_date[0]) {
    // 1 ist Standardwert
    //case '1': gb_day1 = gb_DATE_FONT_WEEKDAY_1_a; gb_day2 = gb_DATE_FONT_WEEKDAY_1_b; break;
    case '2': gb_day1 = gb_DATE_FONT_WEEKDAY_2_a; gb_day2 = gb_DATE_FONT_WEEKDAY_2_b; break;
    case '3': gb_day1 = gb_DATE_FONT_WEEKDAY_3_a; gb_day2 = gb_DATE_FONT_WEEKDAY_3_b; break;
    case '4': gb_day1 = gb_DATE_FONT_WEEKDAY_4_a; gb_day2 = gb_DATE_FONT_WEEKDAY_4_b; break;
    case '5': gb_day1 = gb_DATE_FONT_WEEKDAY_5_a; gb_day2 = gb_DATE_FONT_WEEKDAY_5_b; break;
    case '6': gb_day1 = gb_DATE_FONT_WEEKDAY_6_a; gb_day2 = gb_DATE_FONT_WEEKDAY_6_b; break;
    case '7': gb_day1 = gb_DATE_FONT_WEEKDAY_7_a; gb_day2 = gb_DATE_FONT_WEEKDAY_7_b; break;
  }
  
  // Bitmap für date1 bestimmen
  switch(buffer_date[1]) {
    // 0 ist Standardwert
    //case '0': gb_date1 = gb_DATE_NUMBER[0]; break;
    case '1': gb_date1 = gb_DATE_NUMBER[1]; break;
    case '2': gb_date1 = gb_DATE_NUMBER[2]; break;
    case '3': gb_date1 = gb_DATE_NUMBER[3]; break;
    case '4': gb_date1 = gb_DATE_NUMBER[4]; break;
    case '5': gb_date1 = gb_DATE_NUMBER[5]; break;
    case '6': gb_date1 = gb_DATE_NUMBER[6]; break;
    case '7': gb_date1 = gb_DATE_NUMBER[7]; break;
    case '8': gb_date1 = gb_DATE_NUMBER[8]; break;
    case '9': gb_date1 = gb_DATE_NUMBER[9]; break;
  }
  
  // Bitmap für date2 bestimmen
  switch(buffer_date[2]) {
    // 0 ist Standardwert
    //case '0': gb_date2 = gb_DATE_NUMBER[0]; break;
    case '1': gb_date2 = gb_DATE_NUMBER[1]; break;
    case '2': gb_date2 = gb_DATE_NUMBER[2]; break;
    case '3': gb_date2 = gb_DATE_NUMBER[3]; break;
    case '4': gb_date2 = gb_DATE_NUMBER[4]; break;
    case '5': gb_date2 = gb_DATE_NUMBER[5]; break;
    case '6': gb_date2 = gb_DATE_NUMBER[6]; break;
    case '7': gb_date2 = gb_DATE_NUMBER[7]; break;
    case '8': gb_date2 = gb_DATE_NUMBER[8]; break;
    case '9': gb_date2 = gb_DATE_NUMBER[9]; break;
  }
  
  // Bitmap für date3 bestimmen
  switch(buffer_date[3]) {
    // 0 ist Standardwert
    //case '0': gb_date3 = gb_DATE_NUMBER[0]; break;
    case '1': gb_date3 = gb_DATE_NUMBER[1]; break;
    case '2': gb_date3 = gb_DATE_NUMBER[2]; break;
    case '3': gb_date3 = gb_DATE_NUMBER[3]; break;
    case '4': gb_date3 = gb_DATE_NUMBER[4]; break;
    case '5': gb_date3 = gb_DATE_NUMBER[5]; break;
    case '6': gb_date3 = gb_DATE_NUMBER[6]; break;
    case '7': gb_date3 = gb_DATE_NUMBER[7]; break;
    case '8': gb_date3 = gb_DATE_NUMBER[8]; break;
    case '9': gb_date3 = gb_DATE_NUMBER[9]; break;
  }
  
  // Bitmap für date4 bestimmen
  switch(buffer_date[4]) {
    // 0 ist Standardwert
    //case '0': gb_date4 = gb_DATE_NUMBER[0]; break;
    case '1': gb_date4 = gb_DATE_NUMBER[1]; break;
    case '2': gb_date4 = gb_DATE_NUMBER[2]; break;
    case '3': gb_date4 = gb_DATE_NUMBER[3]; break;
    case '4': gb_date4 = gb_DATE_NUMBER[4]; break;
    case '5': gb_date4 = gb_DATE_NUMBER[5]; break;
    case '6': gb_date4 = gb_DATE_NUMBER[6]; break;
    case '7': gb_date4 = gb_DATE_NUMBER[7]; break;
    case '8': gb_date4 = gb_DATE_NUMBER[8]; break;
    case '9': gb_date4 = gb_DATE_NUMBER[9]; break;
  }
  
  // Bitmaps setzen
  bitmap_layer_set_bitmap(bl_day1, gb_day1);
  bitmap_layer_set_bitmap(bl_day2, gb_day2);
  bitmap_layer_set_bitmap(bl_date1, gb_date1);
  bitmap_layer_set_bitmap(bl_date2, gb_date2);
  bitmap_layer_set_bitmap(bl_date3, gb_date3);
  bitmap_layer_set_bitmap(bl_date4, gb_date4);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
  update_date();
}

static void set_battery(BatteryChargeState charge_state) {
  // Bitmap für battery bestimmen
  GBitmap *gb_battery = gb_ICON_BATTERY_CRITICAL;
  
  if (charge_state.is_charging)
    gb_battery = gb_ICON_BATTERY_CHARGING;
  else
    switch(charge_state.charge_percent) {
      case 100: gb_battery = gb_ICON_BATTERY_100; break;
      case 90: gb_battery = gb_ICON_BATTERY_90; break;
      case 80: gb_battery = gb_ICON_BATTERY_80; break;
      case 70: gb_battery = gb_ICON_BATTERY_70; break;
      case 60: gb_battery = gb_ICON_BATTERY_60; break;
      case 50: gb_battery = gb_ICON_BATTERY_50; break;
      case 40: gb_battery = gb_ICON_BATTERY_40; break;
      case 30: gb_battery = gb_ICON_BATTERY_30; break;
      case 20: gb_battery = gb_ICON_BATTERY_20; break;
      case 10: 
      default: gb_battery = gb_ICON_BATTERY_CRITICAL; break;
    }
  
  // Bitmap setzen
  bitmap_layer_set_bitmap(bl_battery, gb_battery);
}

static void set_bluetooth(bool connected, bool vibrate) {
  if (connected) {
    bitmap_layer_set_bitmap(bl_bluetooth, gb_ICON_BLUETOOTH_CONNECTED);
    
    if (vibrate) {
      // Vibe pattern: ON for 100ms, OFF for 100ms, ON for 100ms:
      static const uint32_t segments[] = { 100, 100, 100 };
      vibes_enqueue_custom_pattern((VibePattern) {
        .durations = segments,
        .num_segments = ARRAY_LENGTH(segments),
      });
    }
  } else {
    bitmap_layer_set_bitmap(bl_bluetooth, gb_ICON_BLUETOOTH_DISCONNECTED);
    
    if (vibrate) {
      // Vibe pattern: ON for 100ms, OFF for 100ms, ON for 100ms, OFF for 100ms, ON for 100ms:
      static const uint32_t segments[] = { 100, 100, 100, 100, 100 };
      vibes_enqueue_custom_pattern((VibePattern) {
        .durations = segments,
        .num_segments = ARRAY_LENGTH(segments),
      });
    }
  }
}

static void hide_Battery_Bluetooth() {
  // Wieder verstecken
  layer_set_hidden(bitmap_layer_get_layer(bl_battery), true);
  layer_set_hidden(bitmap_layer_get_layer(bl_bluetooth), true);
}

static void display_Battery_Bluetooth() {
  // Symbole einblenden
  layer_set_hidden(bitmap_layer_get_layer(bl_battery), false);
  layer_set_hidden(bitmap_layer_get_layer(bl_bluetooth), false);
  
  // Warten -> ggf Timer abbrechen und wieder erstellen mit anschließender Methode zum Verstecken
  app_timer_cancel(display_timer);
  display_timer = app_timer_register(DISPLAY_FOR_MILLISECONDS, hide_Battery_Bluetooth, NULL);
}

static void battery_handler(BatteryChargeState charge_state) {
  // Weiterleiten
  set_battery(charge_state);
  
  // Symbole anzeigen
  display_Battery_Bluetooth();
}

static void bluetooth_handler(bool connected) {
  // Weiterleiten
  set_bluetooth(connected, true);
  
  // Symbole anzeigen
  display_Battery_Bluetooth();
}

static void tap_handler(AccelAxisType axis, int32_t direction) {
  // Weiterleiten
  display_Battery_Bluetooth();
}

static void create_GBitmaps() {
  // 71x71 Zahlen laden
  gb_TIME_NUMBER[0] = gbitmap_create_with_resource(RESOURCE_ID_NUMBER_71x71_0);
  gb_TIME_NUMBER[1] = gbitmap_create_with_resource(RESOURCE_ID_NUMBER_71x71_1);
  gb_TIME_NUMBER[2] = gbitmap_create_with_resource(RESOURCE_ID_NUMBER_71x71_2);
  gb_TIME_NUMBER[3] = gbitmap_create_with_resource(RESOURCE_ID_NUMBER_71x71_3);
  gb_TIME_NUMBER[4] = gbitmap_create_with_resource(RESOURCE_ID_NUMBER_71x71_4);
  gb_TIME_NUMBER[5] = gbitmap_create_with_resource(RESOURCE_ID_NUMBER_71x71_5);
  gb_TIME_NUMBER[6] = gbitmap_create_with_resource(RESOURCE_ID_NUMBER_71x71_6);
  gb_TIME_NUMBER[7] = gbitmap_create_with_resource(RESOURCE_ID_NUMBER_71x71_7);
  gb_TIME_NUMBER[8] = gbitmap_create_with_resource(RESOURCE_ID_NUMBER_71x71_8);
  gb_TIME_NUMBER[9] = gbitmap_create_with_resource(RESOURCE_ID_NUMBER_71x71_9);
  
  // 14x22 Zahlen laden
  gb_DATE_NUMBER[0] = gbitmap_create_with_resource(RESOURCE_ID_NUMBER_14x22_0);
  gb_DATE_NUMBER[1] = gbitmap_create_with_resource(RESOURCE_ID_NUMBER_14x22_1);
  gb_DATE_NUMBER[2] = gbitmap_create_with_resource(RESOURCE_ID_NUMBER_14x22_2);
  gb_DATE_NUMBER[3] = gbitmap_create_with_resource(RESOURCE_ID_NUMBER_14x22_3);
  gb_DATE_NUMBER[4] = gbitmap_create_with_resource(RESOURCE_ID_NUMBER_14x22_4);
  gb_DATE_NUMBER[5] = gbitmap_create_with_resource(RESOURCE_ID_NUMBER_14x22_5);
  gb_DATE_NUMBER[6] = gbitmap_create_with_resource(RESOURCE_ID_NUMBER_14x22_6);
  gb_DATE_NUMBER[7] = gbitmap_create_with_resource(RESOURCE_ID_NUMBER_14x22_7);
  gb_DATE_NUMBER[8] = gbitmap_create_with_resource(RESOURCE_ID_NUMBER_14x22_8);
  gb_DATE_NUMBER[9] = gbitmap_create_with_resource(RESOURCE_ID_NUMBER_14x22_9);
  
  // Batterie laden
  gb_ICON_BATTERY_20 = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_20);
  gb_ICON_BATTERY_30 = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_30);
  gb_ICON_BATTERY_40 = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_40);
  gb_ICON_BATTERY_50 = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_50);
  gb_ICON_BATTERY_60 = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_60);
  gb_ICON_BATTERY_70 = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_70);
  gb_ICON_BATTERY_80 = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_80);
  gb_ICON_BATTERY_90 = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_90);
  gb_ICON_BATTERY_100 = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_100);
  gb_ICON_BATTERY_CHARGING = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_CHARGING);
  gb_ICON_BATTERY_CRITICAL = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_CRITICAL);
    
  // Bluetooth Icons laden
  gb_ICON_BLUETOOTH_CONNECTED = gbitmap_create_with_resource(RESOURCE_ID_ICON_BLUETOOTH_CONNECTED);
  gb_ICON_BLUETOOTH_DISCONNECTED = gbitmap_create_with_resource(RESOURCE_ID_ICON_BLUETOOTH_DISCONNECTED);
  
  // 14x22 Wochentage laden (inkl localisation)
  if (strcmp("de_DE", sys_locale) == 0) {
    // Deutsch
    gb_DATE_FONT_WEEKDAY_1_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_M);
    gb_DATE_FONT_WEEKDAY_1_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_O);
    gb_DATE_FONT_WEEKDAY_2_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_D);
    gb_DATE_FONT_WEEKDAY_2_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_I);
    gb_DATE_FONT_WEEKDAY_3_a = gb_DATE_FONT_WEEKDAY_1_a;
    gb_DATE_FONT_WEEKDAY_3_b = gb_DATE_FONT_WEEKDAY_2_b;
    gb_DATE_FONT_WEEKDAY_4_a = gb_DATE_FONT_WEEKDAY_2_a;
    gb_DATE_FONT_WEEKDAY_4_b = gb_DATE_FONT_WEEKDAY_1_b;
    gb_DATE_FONT_WEEKDAY_5_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_F);
    gb_DATE_FONT_WEEKDAY_5_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_R);
    gb_DATE_FONT_WEEKDAY_6_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_S);
    gb_DATE_FONT_WEEKDAY_6_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_A);
    gb_DATE_FONT_WEEKDAY_7_a = gb_DATE_FONT_WEEKDAY_6_a;
    gb_DATE_FONT_WEEKDAY_7_b = gb_DATE_FONT_WEEKDAY_1_b;
  } else if (strcmp("fr_FR", sys_locale) == 0) {
    // Franzoesisch
    gb_DATE_FONT_WEEKDAY_1_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_L);
    gb_DATE_FONT_WEEKDAY_1_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_U);
    gb_DATE_FONT_WEEKDAY_2_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_M);
    gb_DATE_FONT_WEEKDAY_2_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_A);
    gb_DATE_FONT_WEEKDAY_3_a = gb_DATE_FONT_WEEKDAY_2_a;
    gb_DATE_FONT_WEEKDAY_3_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_E);
    gb_DATE_FONT_WEEKDAY_4_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_J);
    gb_DATE_FONT_WEEKDAY_4_b = gb_DATE_FONT_WEEKDAY_3_b;
    gb_DATE_FONT_WEEKDAY_5_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_V);
    gb_DATE_FONT_WEEKDAY_5_b = gb_DATE_FONT_WEEKDAY_3_b;
    gb_DATE_FONT_WEEKDAY_6_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_S);
    gb_DATE_FONT_WEEKDAY_6_b = gb_DATE_FONT_WEEKDAY_2_b;
    gb_DATE_FONT_WEEKDAY_7_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_D);
    gb_DATE_FONT_WEEKDAY_7_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_I);
  } else if (strcmp("es_ES", sys_locale) == 0) {
    // Spanisch
    gb_DATE_FONT_WEEKDAY_1_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_L);
    gb_DATE_FONT_WEEKDAY_1_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_U);
    gb_DATE_FONT_WEEKDAY_2_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_M);
    gb_DATE_FONT_WEEKDAY_2_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_A);
    gb_DATE_FONT_WEEKDAY_3_a = gb_DATE_FONT_WEEKDAY_2_a;
    gb_DATE_FONT_WEEKDAY_3_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_I);
    gb_DATE_FONT_WEEKDAY_4_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_J);
    gb_DATE_FONT_WEEKDAY_4_b = gb_DATE_FONT_WEEKDAY_1_b;
    gb_DATE_FONT_WEEKDAY_5_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_V);
    gb_DATE_FONT_WEEKDAY_5_b = gb_DATE_FONT_WEEKDAY_3_b;
    gb_DATE_FONT_WEEKDAY_6_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_S);
    gb_DATE_FONT_WEEKDAY_6_b = gb_DATE_FONT_WEEKDAY_2_b;
    gb_DATE_FONT_WEEKDAY_7_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_D);
    gb_DATE_FONT_WEEKDAY_7_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_O);
  } else {
    // Standard: Englisch
    gb_DATE_FONT_WEEKDAY_1_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_M);
    gb_DATE_FONT_WEEKDAY_1_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_O);
    gb_DATE_FONT_WEEKDAY_2_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_T);
    gb_DATE_FONT_WEEKDAY_2_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_U);
    gb_DATE_FONT_WEEKDAY_3_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_W);
    gb_DATE_FONT_WEEKDAY_3_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_E);
    gb_DATE_FONT_WEEKDAY_4_a = gb_DATE_FONT_WEEKDAY_2_a;
    gb_DATE_FONT_WEEKDAY_4_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_H);
    gb_DATE_FONT_WEEKDAY_5_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_F);
    gb_DATE_FONT_WEEKDAY_5_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_R);
    gb_DATE_FONT_WEEKDAY_6_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_S);
    gb_DATE_FONT_WEEKDAY_6_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_A);
    gb_DATE_FONT_WEEKDAY_7_a = gb_DATE_FONT_WEEKDAY_6_a;
    gb_DATE_FONT_WEEKDAY_7_b = gb_DATE_FONT_WEEKDAY_2_b;
  }
  gb_DATE_FONT_DOT = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_DOT);
}

static void destroy_GBitmaps() {
  // 71x71 Zahlen zerstoeren
  gbitmap_destroy(gb_TIME_NUMBER[0]);
  gbitmap_destroy(gb_TIME_NUMBER[1]);
  gbitmap_destroy(gb_TIME_NUMBER[2]);
  gbitmap_destroy(gb_TIME_NUMBER[3]);
  gbitmap_destroy(gb_TIME_NUMBER[4]);
  gbitmap_destroy(gb_TIME_NUMBER[5]);
  gbitmap_destroy(gb_TIME_NUMBER[6]);
  gbitmap_destroy(gb_TIME_NUMBER[7]);
  gbitmap_destroy(gb_TIME_NUMBER[8]);
  gbitmap_destroy(gb_TIME_NUMBER[9]);
  
  // 14x22 Zahlen zerstoeren
  gbitmap_destroy(gb_DATE_NUMBER[0]);
  gbitmap_destroy(gb_DATE_NUMBER[1]);
  gbitmap_destroy(gb_DATE_NUMBER[2]);
  gbitmap_destroy(gb_DATE_NUMBER[3]);
  gbitmap_destroy(gb_DATE_NUMBER[4]);
  gbitmap_destroy(gb_DATE_NUMBER[5]);
  gbitmap_destroy(gb_DATE_NUMBER[6]);
  gbitmap_destroy(gb_DATE_NUMBER[7]);
  gbitmap_destroy(gb_DATE_NUMBER[8]);
  gbitmap_destroy(gb_DATE_NUMBER[9]);
  
  // Batterie zerstoeren
  gbitmap_destroy(gb_ICON_BATTERY_20);
  gbitmap_destroy(gb_ICON_BATTERY_30);
  gbitmap_destroy(gb_ICON_BATTERY_40);
  gbitmap_destroy(gb_ICON_BATTERY_50);
  gbitmap_destroy(gb_ICON_BATTERY_60);
  gbitmap_destroy(gb_ICON_BATTERY_70);
  gbitmap_destroy(gb_ICON_BATTERY_80);
  gbitmap_destroy(gb_ICON_BATTERY_90);
  gbitmap_destroy(gb_ICON_BATTERY_100);
  gbitmap_destroy(gb_ICON_BATTERY_CHARGING);
  gbitmap_destroy(gb_ICON_BATTERY_CRITICAL);
  
  // Bluetooth Icons zerstoeren
  gbitmap_destroy(gb_ICON_BLUETOOTH_CONNECTED);
  gbitmap_destroy(gb_ICON_BLUETOOTH_DISCONNECTED);
  
  // 14x22 Wochentage zerstoeren
  if (strcmp("de_DE", sys_locale) == 0) {
    // Deutsch
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_1_a);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_1_b);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_2_a);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_2_b);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_5_a);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_5_b);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_6_a);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_6_b);
  } else if (strcmp("fr_FR", sys_locale) == 0 || strcmp("es_ES", sys_locale) == 0) {
    // Franzoesisch oder Spanisch
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_1_a);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_1_b);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_2_a);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_2_b);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_3_b);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_4_a);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_5_a);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_6_a);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_7_a);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_7_b);
  } else {
    // Standard: Englisch
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_1_a);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_1_b);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_2_a);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_2_b);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_3_a);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_3_b);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_4_b);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_5_a);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_5_b);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_6_a);
    gbitmap_destroy(gb_DATE_FONT_WEEKDAY_6_b);
  }
  gbitmap_destroy(gb_DATE_FONT_DOT);
}

static void create_BitmapLayers() {
  // BitmapLayer erstellen
  bl_hour1 = bitmap_layer_create(GRect( 0, 24, 71, 71));
  bl_hour2 = bitmap_layer_create(GRect(73, 24, 71, 71));
  bl_minute1 = bitmap_layer_create(GRect( 0, 97, 71, 71));
  bl_minute2 = bitmap_layer_create(GRect(73, 97, 71, 71));
  
  bl_day1 = bitmap_layer_create(GRect(16, 0, 14, 22));
  bl_day2 = bitmap_layer_create(GRect(32, 0, 14, 22));
  bl_date1 = bitmap_layer_create(GRect(57, 0, 14, 22));
  bl_date2 = bitmap_layer_create(GRect(73, 0, 14, 22));
  bl_dot1 = bitmap_layer_create(GRect(88, 0, 4, 22));
  bl_date3 = bitmap_layer_create(GRect(93, 0, 14, 22));
  bl_date4 = bitmap_layer_create(GRect(109, 0, 14, 22));
  bl_dot2 = bitmap_layer_create(GRect(124, 0, 4, 22));
  
  bl_battery = bitmap_layer_create(GRect(132, 0, 12, 22));
  bl_bluetooth = bitmap_layer_create(GRect(0, 0, 12, 22));
  
  // Standards setzen
  bitmap_layer_set_bitmap(bl_hour1, gb_TIME_NUMBER[0]);
  bitmap_layer_set_bitmap(bl_hour2, gb_TIME_NUMBER[0]);
  bitmap_layer_set_bitmap(bl_minute1, gb_TIME_NUMBER[0]);
  bitmap_layer_set_bitmap(bl_minute2, gb_TIME_NUMBER[0]);
  
  bitmap_layer_set_bitmap(bl_day1, gb_DATE_FONT_WEEKDAY_1_a);
  bitmap_layer_set_bitmap(bl_day2, gb_DATE_FONT_WEEKDAY_1_b);
  bitmap_layer_set_bitmap(bl_date1, gb_DATE_NUMBER[0]);
  bitmap_layer_set_bitmap(bl_date2, gb_DATE_NUMBER[0]);
  bitmap_layer_set_bitmap(bl_dot1, gb_DATE_FONT_DOT);
  bitmap_layer_set_bitmap(bl_date3, gb_DATE_NUMBER[0]);
  bitmap_layer_set_bitmap(bl_date4, gb_DATE_NUMBER[0]);
  bitmap_layer_set_bitmap(bl_dot2, gb_DATE_FONT_DOT);
  
  bitmap_layer_set_bitmap(bl_battery, gb_ICON_BATTERY_CRITICAL);
  bitmap_layer_set_bitmap(bl_bluetooth, gb_ICON_BLUETOOTH_DISCONNECTED);
  
  // Standardmaessig verstecken
  layer_set_hidden(bitmap_layer_get_layer(bl_battery), true);
  layer_set_hidden(bitmap_layer_get_layer(bl_bluetooth), true);
}

static void destroy_BitmapLayers() {
  bitmap_layer_destroy(bl_hour1);
  bitmap_layer_destroy(bl_hour2);
  bitmap_layer_destroy(bl_minute1);
  bitmap_layer_destroy(bl_minute2);
  
  bitmap_layer_destroy(bl_day1);
  bitmap_layer_destroy(bl_day2);
  bitmap_layer_destroy(bl_date1);
  bitmap_layer_destroy(bl_date2);
  bitmap_layer_destroy(bl_dot1);
  bitmap_layer_destroy(bl_date3);
  bitmap_layer_destroy(bl_date4);
  bitmap_layer_destroy(bl_dot2);
  
  bitmap_layer_destroy(bl_battery);
  bitmap_layer_destroy(bl_bluetooth);
}

static void main_window_load(Window *window) {
  // Localisation laden
  sys_locale = setlocale(LC_ALL, "de_DE");
  
  // GBitmaps erstellen
  create_GBitmaps();
  
  // BitmapLayer erstellen und die Standards setzen
  create_BitmapLayers();
  
  // BitmapLayer hinzufuegen
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bl_hour1));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bl_hour2));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bl_minute1));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bl_minute2));
  
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bl_day1));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bl_day2));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bl_date1));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bl_date2));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bl_dot1));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bl_date3));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bl_date4));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bl_dot2));
  
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bl_battery));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bl_bluetooth));
  
  // Invertierte Ebene erstellen und hinzufügen
  il_inverted = inverter_layer_create(GRect(0, 0, 144, 186));
  layer_add_child(window_get_root_layer(window), inverter_layer_get_layer(il_inverted));
//  il_inverted2 = inverter_layer_create(GRect(0, 24, 144, 73)); // Obere Reihe
//  il_inverted2 = inverter_layer_create(GRect(0, 95, 144, 73)); // Untere Reihe
//  layer_add_child(window_get_root_layer(window), inverter_layer_get_layer(il_inverted2)); 
  
  // Erster Aufruf, damit von Anfang an dargestllt wird
  update_time();
  update_date();
  set_battery(battery_state_service_peek());
  set_bluetooth(bluetooth_connection_service_peek(), false);
  
  // Batterie und Bluetooth einmal anzeigen
  display_Battery_Bluetooth();
}

static void main_window_unload(Window *window) {
  // BitmapLayer zerstoeren
  destroy_BitmapLayers();
  
  // GBitmaps zerstoeren
  destroy_GBitmaps();
  
  // Invertierte Ebene zerstoeren
  inverter_layer_destroy(il_inverted);
}

static void initHandler() {
  // TickHandler registrieren
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // BatteryHandler registrieren
  battery_state_service_subscribe(battery_handler);
  
  // BluetoothHandler registrieren
  bluetooth_connection_service_subscribe(bluetooth_handler);
  
  // TapHandler registrieren
  accel_tap_service_subscribe(tap_handler);
}

static void deinitHandler() { 
  // TickHandler entfernen
  tick_timer_service_unsubscribe();
  
  // BatteryHandler entfernen
  battery_state_service_unsubscribe();
  
  // BluetoothHandler entfernen
  bluetooth_connection_service_unsubscribe();
  
  // TapHandler entfernen
  accel_tap_service_unsubscribe();
}

static void init() {  
  // Main Window Element erstellen
  w_main_window = window_create();
  
  // Window Hintergrundfarbe setzen
  window_set_background_color(w_main_window, GColorWhite);

  // Handler verknuepfen, die die Elemente verwalten
  window_set_window_handlers(w_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Window auf der Uhr anzeigen, mit animated=true
  window_stack_push(w_main_window, true);
  
  // Handler registrieren
  initHandler();
}

static void deinit() { 
  // Handler entfernen
  deinitHandler();
  
  // Window zerstoeren
  window_destroy(w_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}