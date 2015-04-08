#include "main.h"

static Window *w_main_window;
static InverterLayer *il_inverted_face, *il_inverted_topRow, *il_inverted_bottomRow;
static struct time_layer timeLayer;
static struct date_layer dateLayer;
static BitmapLayer *bl_battery, *bl_bluetooth;
static GBitmap *gb_TIME_NUMBER[10], *gb_DATE_NUMBER[10];
static struct date_font dateFont;
static struct battery_icons batteryIcons;
static struct bluetooth_icons bluetoothIcons;
static AppTimer *display_timer;
static char *sys_locale;
static struct configurable_variables configurable;

static void update_time() {
  // create tm-structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // create long-lived buffer
  char buffer_time[] = "0000";

  // write time in buffer
  if(clock_is_24h_style() == true)
    strftime(buffer_time, sizeof("0000"), "%H%M", tick_time);
  else
    strftime(buffer_time, sizeof("0000"), "%I%M", tick_time);
  
  // set defaults
  GBitmap *gb_hour1 = gb_TIME_NUMBER[0];
  GBitmap *gb_hour2 = gb_TIME_NUMBER[0];
  GBitmap *gb_minute1 = gb_TIME_NUMBER[0];
  GBitmap *gb_minute2 = gb_TIME_NUMBER[0];
  
  // get hours1 bitmap
  switch(buffer_time[0]) {
    // 0 is default
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
  
  // get hours2 bitmap
  switch(buffer_time[1]) {
    // 0 is default
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
  
  // get minute1 bitmap
  switch(buffer_time[2]) {
    // 0 is default
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
  
  // get minute2 bitmap
  switch(buffer_time[3]) {
    // 0 is default
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
  
  // set bitmaps
  bitmap_layer_set_bitmap(timeLayer.hour1, gb_hour1);
  bitmap_layer_set_bitmap(timeLayer.hour2, gb_hour2);
  bitmap_layer_set_bitmap(timeLayer.minute1, gb_minute1);
  bitmap_layer_set_bitmap(timeLayer.minute2, gb_minute2); 
}

static void update_date() {
  // create tm-structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // create long-lived buffer
  char buffer_date[] = "00000";

  // write date in buffer
  strftime(buffer_date, sizeof("00000"), "%u%d%m", tick_time);
  
  // set defaults
  GBitmap *gb_day1 = dateFont.weekday_1_a;
  GBitmap *gb_day2 = dateFont.weekday_1_b;
  GBitmap *gb_date1 = gb_DATE_NUMBER[0];
  GBitmap *gb_date2 = gb_DATE_NUMBER[0];
  GBitmap *gb_date3 = gb_DATE_NUMBER[0];
  GBitmap *gb_date4 = gb_DATE_NUMBER[0];
  
  // get day1+day2 bitmaps
  switch(buffer_date[0]) {
    // 1 is default
    //case '1': gb_day1 = dateFont.weekday_1_a; gb_day2 = dateFont.weekday_1_b; break;
    case '2': gb_day1 = dateFont.weekday_2_a; gb_day2 = dateFont.weekday_2_b; break;
    case '3': gb_day1 = dateFont.weekday_3_a; gb_day2 = dateFont.weekday_3_b; break;
    case '4': gb_day1 = dateFont.weekday_4_a; gb_day2 = dateFont.weekday_4_b; break;
    case '5': gb_day1 = dateFont.weekday_5_a; gb_day2 = dateFont.weekday_5_b; break;
    case '6': gb_day1 = dateFont.weekday_6_a; gb_day2 = dateFont.weekday_6_b; break;
    case '7': gb_day1 = dateFont.weekday_7_a; gb_day2 = dateFont.weekday_7_b; break;
  }
  
  // get date1 bitmap
  switch(buffer_date[1]) {
    // 0 is default
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
  
  // get date2 bitmap
  switch(buffer_date[2]) {
    // 0 is default
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
  
  // get date3 bitmap
  switch(buffer_date[3]) {
    // 0 is default
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
  
  // get date4 bitmap
  switch(buffer_date[4]) {
    // 0 is default
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
  
  // set bitmaps
  bitmap_layer_set_bitmap(dateLayer.day1, gb_day1);
  bitmap_layer_set_bitmap(dateLayer.day2, gb_day2);
  bitmap_layer_set_bitmap(dateLayer.date1, gb_date1);
  bitmap_layer_set_bitmap(dateLayer.date2, gb_date2);
  bitmap_layer_set_bitmap(dateLayer.date3, gb_date3);
  bitmap_layer_set_bitmap(dateLayer.date4, gb_date4);
}

static void update_face() {  
  if (strcmp("true", configurable.invertFace) == 0)
      layer_set_hidden(inverter_layer_get_layer(il_inverted_face), true);
  else
      layer_set_hidden(inverter_layer_get_layer(il_inverted_face), false);
  
  switch(configurable.invertRow[0]) {
    case '1': 
      layer_set_hidden(inverter_layer_get_layer(il_inverted_topRow), false);
      layer_set_hidden(inverter_layer_get_layer(il_inverted_bottomRow), true);
      break;
    case '2': 
      layer_set_hidden(inverter_layer_get_layer(il_inverted_topRow), true);
      layer_set_hidden(inverter_layer_get_layer(il_inverted_bottomRow), false);
      break;
    case '0':
    default:
      layer_set_hidden(inverter_layer_get_layer(il_inverted_topRow), true);
      layer_set_hidden(inverter_layer_get_layer(il_inverted_bottomRow), true);
      break;
  }
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  // update time and date
  update_time();
  update_date();
}

static void set_battery(BatteryChargeState charge_state) {
  // get battery bitmap
  GBitmap *gb_battery = batteryIcons.icon_critical;
  
  if (charge_state.is_charging)
    gb_battery = batteryIcons.icon_charging;
  else
    switch(charge_state.charge_percent) {
      case 100: gb_battery = batteryIcons.icon_100; break;
      case 90: gb_battery = batteryIcons.icon_90; break;
      case 80: gb_battery = batteryIcons.icon_80; break;
      case 70: gb_battery = batteryIcons.icon_70; break;
      case 60: gb_battery = batteryIcons.icon_60; break;
      case 50: gb_battery = batteryIcons.icon_50; break;
      case 40: gb_battery = batteryIcons.icon_40; break;
      case 30: gb_battery = batteryIcons.icon_30; break;
      case 20: gb_battery = batteryIcons.icon_20; break;
      // icon_critical is default
      //case 10: 
      //default: gb_battery = batteryIcons.icon_critical; break;
    }
  
  // set bitmap
  bitmap_layer_set_bitmap(bl_battery, gb_battery);
}

static void set_bluetooth(bool connected, bool vibrate) {
  // get bluetooth bitmap
  if (connected) {
    bitmap_layer_set_bitmap(bl_bluetooth, bluetoothIcons.icon_connected);
    
    if (vibrate) {
      // Vibe pattern: ON for 100ms, OFF for 100ms, ON for 100ms:
      const uint32_t segments[] = { 100, 100, 100 };
      vibes_enqueue_custom_pattern( (VibePattern) {
        .durations = segments,
        .num_segments = ARRAY_LENGTH(segments),
      });
    }
  } else {
    bitmap_layer_set_bitmap(bl_bluetooth, bluetoothIcons.icon_disconnected);
    
    if (vibrate) {
      // Vibe pattern: ON for 100ms, OFF for 100ms, ON for 100ms, OFF for 100ms, ON for 100ms:
      const uint32_t segments[] = { 100, 100, 100, 100, 100 };
      vibes_enqueue_custom_pattern( (VibePattern) {
        .durations = segments,
        .num_segments = ARRAY_LENGTH(segments),
      });
    }
  }
}

static void hide_Battery_Bluetooth() {
  // hide icons
  layer_set_hidden(bitmap_layer_get_layer(bl_battery), true);
  layer_set_hidden(bitmap_layer_get_layer(bl_bluetooth), true);
}

static void display_Battery_Bluetooth() {
  // show icons
  layer_set_hidden(bitmap_layer_get_layer(bl_battery), false);
  layer_set_hidden(bitmap_layer_get_layer(bl_bluetooth), false);
  
  // wait -> cancel previous timer and register new with method to hide the icons
  app_timer_cancel(display_timer);
  display_timer = app_timer_register(DISPLAY_FOR_MILLISECONDS, hide_Battery_Bluetooth, NULL);
}

static void battery_handler(BatteryChargeState charge_state) {
  // set battery icon
  set_battery(charge_state);
  
  // show icons
  display_Battery_Bluetooth();
}

static void bluetooth_handler(bool connected) {
  // set bluetooth icon
  set_bluetooth(connected, true);
  
  // show icons
  display_Battery_Bluetooth();
}

static void tap_handler(AccelAxisType axis, int32_t direction) {
  // show icons
  display_Battery_Bluetooth();
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  char invertFace_buffer[] = "false";
  char invertRow_buffer[] = "0";
  
  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
      case INVERTFACE: snprintf(invertFace_buffer, sizeof(invertFace_buffer), "%s", t->value->cstring); break;
      case INVERTROW: snprintf(invertRow_buffer, sizeof(invertRow_buffer), "%s", t->value->cstring); break;
      default: APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key); break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }
  
  // set incoming information
  snprintf(configurable.invertFace, sizeof(invertFace_buffer), "%s", invertFace_buffer);
  persist_write_string(PERSIST_KEY_INVERTFACE, configurable.invertFace);
  snprintf(configurable.invertRow, sizeof(invertRow_buffer), "%s", invertRow_buffer);
  persist_write_string(PERSIST_KEY_INVERTROW, configurable.invertRow);
  
  // update the watchface with the new information
  update_face();
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "inbox_dropped_callback(): Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "outbox_failed_callback(): Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "outbox_sent_callback(): Outbox send success!");
}

static void create_GBitmaps() {
  // load 71x71 numbers
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
  
  // load 14x22 numbers
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
  
  // load battery icons
  batteryIcons.icon_20 = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_20);
  batteryIcons.icon_30 = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_30);
  batteryIcons.icon_40 = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_40);
  batteryIcons.icon_50 = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_50);
  batteryIcons.icon_60 = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_60);
  batteryIcons.icon_70 = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_70);
  batteryIcons.icon_80 = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_80);
  batteryIcons.icon_90 = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_90);
  batteryIcons.icon_100 = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_100);
  batteryIcons.icon_charging = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_CHARGING);
  batteryIcons.icon_critical = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_CRITICAL);
    
  // load bluetooth icons
  bluetoothIcons.icon_connected = gbitmap_create_with_resource(RESOURCE_ID_ICON_BLUETOOTH_CONNECTED);
  bluetoothIcons.icon_disconnected = gbitmap_create_with_resource(RESOURCE_ID_ICON_BLUETOOTH_DISCONNECTED);
  
  // load 14x22 weekdays (incl localisation)
  if (strcmp("de_DE", sys_locale) == 0) {
    // german
    dateFont.weekday_1_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_M);
    dateFont.weekday_1_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_O);
    dateFont.weekday_2_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_D);
    dateFont.weekday_2_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_I);
    dateFont.weekday_3_a = dateFont.weekday_1_a;
    dateFont.weekday_3_b = dateFont.weekday_2_b;
    dateFont.weekday_4_a = dateFont.weekday_2_a;
    dateFont.weekday_4_b = dateFont.weekday_1_b;
    dateFont.weekday_5_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_F);
    dateFont.weekday_5_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_R);
    dateFont.weekday_6_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_S);
    dateFont.weekday_6_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_A);
    dateFont.weekday_7_a = dateFont.weekday_6_a;
    dateFont.weekday_7_b = dateFont.weekday_1_b;
  } else if (strcmp("fr_FR", sys_locale) == 0) {
    // french
    dateFont.weekday_1_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_L);
    dateFont.weekday_1_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_U);
    dateFont.weekday_2_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_M);
    dateFont.weekday_2_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_A);
    dateFont.weekday_3_a = dateFont.weekday_2_a;
    dateFont.weekday_3_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_E);
    dateFont.weekday_4_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_J);
    dateFont.weekday_4_b = dateFont.weekday_3_b;
    dateFont.weekday_5_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_V);
    dateFont.weekday_5_b = dateFont.weekday_3_b;
    dateFont.weekday_6_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_S);
    dateFont.weekday_6_b = dateFont.weekday_2_b;
    dateFont.weekday_7_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_D);
    dateFont.weekday_7_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_I);
  } else if (strcmp("es_ES", sys_locale) == 0) {
    // spanish
    dateFont.weekday_1_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_L);
    dateFont.weekday_1_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_U);
    dateFont.weekday_2_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_M);
    dateFont.weekday_2_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_A);
    dateFont.weekday_3_a = dateFont.weekday_2_a;
    dateFont.weekday_3_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_I);
    dateFont.weekday_4_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_J);
    dateFont.weekday_4_b = dateFont.weekday_1_b;
    dateFont.weekday_5_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_V);
    dateFont.weekday_5_b = dateFont.weekday_3_b;
    dateFont.weekday_6_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_S);
    dateFont.weekday_6_b = dateFont.weekday_2_b;
    dateFont.weekday_7_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_D);
    dateFont.weekday_7_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_O);
  } else {
    // default: english
    dateFont.weekday_1_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_M);
    dateFont.weekday_1_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_O);
    dateFont.weekday_2_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_T);
    dateFont.weekday_2_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_U);
    dateFont.weekday_3_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_W);
    dateFont.weekday_3_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_E);
    dateFont.weekday_4_a = dateFont.weekday_2_a;
    dateFont.weekday_4_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_H);
    dateFont.weekday_5_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_F);
    dateFont.weekday_5_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_R);
    dateFont.weekday_6_a = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_S);
    dateFont.weekday_6_b = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_A);
    dateFont.weekday_7_a = dateFont.weekday_6_a;
    dateFont.weekday_7_b = dateFont.weekday_2_b;
  }
  dateFont.dot = gbitmap_create_with_resource(RESOURCE_ID_FONT_14x22_DOT);
}

static void destroy_GBitmaps() {
  // destroy 71x71 numbers
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
  
  // destroy 14x22 numbers
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
  
  // destroy battery icons
  gbitmap_destroy(batteryIcons.icon_20);
  gbitmap_destroy(batteryIcons.icon_30);
  gbitmap_destroy(batteryIcons.icon_40);
  gbitmap_destroy(batteryIcons.icon_50);
  gbitmap_destroy(batteryIcons.icon_60);
  gbitmap_destroy(batteryIcons.icon_70);
  gbitmap_destroy(batteryIcons.icon_80);
  gbitmap_destroy(batteryIcons.icon_90);
  gbitmap_destroy(batteryIcons.icon_100);
  gbitmap_destroy(batteryIcons.icon_charging);
  gbitmap_destroy(batteryIcons.icon_critical);
  
  // destroy bluetooth icons
  gbitmap_destroy(bluetoothIcons.icon_connected);
  gbitmap_destroy(bluetoothIcons.icon_disconnected);
  
  // destroy only loaded 14x22 numbers
  if (strcmp("de_DE", sys_locale) == 0) {
    // german
    gbitmap_destroy(dateFont.weekday_1_a);
    gbitmap_destroy(dateFont.weekday_1_b);
    gbitmap_destroy(dateFont.weekday_2_a);
    gbitmap_destroy(dateFont.weekday_2_b);
    gbitmap_destroy(dateFont.weekday_5_a);
    gbitmap_destroy(dateFont.weekday_5_b);
    gbitmap_destroy(dateFont.weekday_6_a);
    gbitmap_destroy(dateFont.weekday_6_b);
  } else if (strcmp("fr_FR", sys_locale) == 0 || strcmp("es_ES", sys_locale) == 0) {
    // french oder spanish
    gbitmap_destroy(dateFont.weekday_1_a);
    gbitmap_destroy(dateFont.weekday_1_b);
    gbitmap_destroy(dateFont.weekday_2_a);
    gbitmap_destroy(dateFont.weekday_2_b);
    gbitmap_destroy(dateFont.weekday_3_b);
    gbitmap_destroy(dateFont.weekday_4_a);
    gbitmap_destroy(dateFont.weekday_5_a);
    gbitmap_destroy(dateFont.weekday_6_a);
    gbitmap_destroy(dateFont.weekday_7_a);
    gbitmap_destroy(dateFont.weekday_7_b);
  } else {
    // default: english
    gbitmap_destroy(dateFont.weekday_1_a);
    gbitmap_destroy(dateFont.weekday_1_b);
    gbitmap_destroy(dateFont.weekday_2_a);
    gbitmap_destroy(dateFont.weekday_2_b);
    gbitmap_destroy(dateFont.weekday_3_a);
    gbitmap_destroy(dateFont.weekday_3_b);
    gbitmap_destroy(dateFont.weekday_4_b);
    gbitmap_destroy(dateFont.weekday_5_a);
    gbitmap_destroy(dateFont.weekday_5_b);
    gbitmap_destroy(dateFont.weekday_6_a);
    gbitmap_destroy(dateFont.weekday_6_b);
  }
  gbitmap_destroy(dateFont.dot);
}

static void create_BitmapLayers() {
  // create BitmapLayer
  timeLayer.hour1 = bitmap_layer_create(GRect( 0, 24, 71, 71));
  timeLayer.hour2 = bitmap_layer_create(GRect(73, 24, 71, 71));
  timeLayer.minute1 = bitmap_layer_create(GRect( 0, 97, 71, 71));
  timeLayer.minute2 = bitmap_layer_create(GRect(73, 97, 71, 71));
  
  dateLayer.day1 = bitmap_layer_create(GRect(16, 0, 14, 22));
  dateLayer.day2 = bitmap_layer_create(GRect(32, 0, 14, 22));
  dateLayer.date1 = bitmap_layer_create(GRect(57, 0, 14, 22));
  dateLayer.date2 = bitmap_layer_create(GRect(73, 0, 14, 22));
  dateLayer.dot1 = bitmap_layer_create(GRect(88, 0, 4, 22));
  dateLayer.date3 = bitmap_layer_create(GRect(93, 0, 14, 22));
  dateLayer.date4 = bitmap_layer_create(GRect(109, 0, 14, 22));
  dateLayer.dot2 = bitmap_layer_create(GRect(124, 0, 4, 22));
  
  bl_battery = bitmap_layer_create(GRect(132, 0, 12, 22));
  bl_bluetooth = bitmap_layer_create(GRect(0, 0, 12, 22));
  
  // set defaults
  bitmap_layer_set_bitmap(timeLayer.hour1, gb_TIME_NUMBER[0]);
  bitmap_layer_set_bitmap(timeLayer.hour2, gb_TIME_NUMBER[0]);
  bitmap_layer_set_bitmap(timeLayer.minute1, gb_TIME_NUMBER[0]);
  bitmap_layer_set_bitmap(timeLayer.minute2, gb_TIME_NUMBER[0]);
  
  bitmap_layer_set_bitmap(dateLayer.day1, dateFont.weekday_1_a);
  bitmap_layer_set_bitmap(dateLayer.day2, dateFont.weekday_1_b);
  bitmap_layer_set_bitmap(dateLayer.date1, gb_DATE_NUMBER[0]);
  bitmap_layer_set_bitmap(dateLayer.date2, gb_DATE_NUMBER[0]);
  bitmap_layer_set_bitmap(dateLayer.dot1, dateFont.dot);
  bitmap_layer_set_bitmap(dateLayer.date3, gb_DATE_NUMBER[0]);
  bitmap_layer_set_bitmap(dateLayer.date4, gb_DATE_NUMBER[0]);
  bitmap_layer_set_bitmap(dateLayer.dot2, dateFont.dot);
  
  bitmap_layer_set_bitmap(bl_battery, batteryIcons.icon_critical);
  bitmap_layer_set_bitmap(bl_bluetooth, bluetoothIcons.icon_disconnected);
  
  // hide at start
  layer_set_hidden(bitmap_layer_get_layer(bl_battery), true);
  layer_set_hidden(bitmap_layer_get_layer(bl_bluetooth), true);
}

static void add_BitmapLayers() {
  // add BitmapLayers
  layer_add_child(window_get_root_layer(w_main_window), bitmap_layer_get_layer(timeLayer.hour1));
  layer_add_child(window_get_root_layer(w_main_window), bitmap_layer_get_layer(timeLayer.hour2));
  layer_add_child(window_get_root_layer(w_main_window), bitmap_layer_get_layer(timeLayer.minute1));
  layer_add_child(window_get_root_layer(w_main_window), bitmap_layer_get_layer(timeLayer.minute2));
  
  layer_add_child(window_get_root_layer(w_main_window), bitmap_layer_get_layer(dateLayer.day1));
  layer_add_child(window_get_root_layer(w_main_window), bitmap_layer_get_layer(dateLayer.day2));
  layer_add_child(window_get_root_layer(w_main_window), bitmap_layer_get_layer(dateLayer.date1));
  layer_add_child(window_get_root_layer(w_main_window), bitmap_layer_get_layer(dateLayer.date2));
  layer_add_child(window_get_root_layer(w_main_window), bitmap_layer_get_layer(dateLayer.dot1));
  layer_add_child(window_get_root_layer(w_main_window), bitmap_layer_get_layer(dateLayer.date3));
  layer_add_child(window_get_root_layer(w_main_window), bitmap_layer_get_layer(dateLayer.date4));
  layer_add_child(window_get_root_layer(w_main_window), bitmap_layer_get_layer(dateLayer.dot2));
  
  layer_add_child(window_get_root_layer(w_main_window), bitmap_layer_get_layer(bl_battery));
  layer_add_child(window_get_root_layer(w_main_window), bitmap_layer_get_layer(bl_bluetooth));
}

static void destroy_BitmapLayers() {
  // destroy BitmapLayers
  bitmap_layer_destroy(timeLayer.hour1);
  bitmap_layer_destroy(timeLayer.hour2);
  bitmap_layer_destroy(timeLayer.minute1);
  bitmap_layer_destroy(timeLayer.minute2);
  
  bitmap_layer_destroy(dateLayer.day1);
  bitmap_layer_destroy(dateLayer.day2);
  bitmap_layer_destroy(dateLayer.date1);
  bitmap_layer_destroy(dateLayer.date2);
  bitmap_layer_destroy(dateLayer.dot1);
  bitmap_layer_destroy(dateLayer.date3);
  bitmap_layer_destroy(dateLayer.date4);
  bitmap_layer_destroy(dateLayer.dot2);
  
  bitmap_layer_destroy(bl_battery);
  bitmap_layer_destroy(bl_bluetooth);
}

static void create_InverterLayers() {
  // create InverterLayers
  il_inverted_face = inverter_layer_create(GRect(0, 0, 144, 186));
  il_inverted_topRow = inverter_layer_create(GRect(0, 24, 144, 73));
  il_inverted_bottomRow = inverter_layer_create(GRect(0, 95, 144, 73));
  
  // add InverterLayers
  layer_add_child(window_get_root_layer(w_main_window), inverter_layer_get_layer(il_inverted_face));
  layer_add_child(window_get_root_layer(w_main_window), inverter_layer_get_layer(il_inverted_topRow));
  layer_add_child(window_get_root_layer(w_main_window), inverter_layer_get_layer(il_inverted_bottomRow));
  
  // set visibility defaults
  layer_set_hidden(inverter_layer_get_layer(il_inverted_topRow), true);
  layer_set_hidden(inverter_layer_get_layer(il_inverted_bottomRow), true);
}

static void destroy_InverterLayers() {
  // destroy InverterLayers
  inverter_layer_destroy(il_inverted_face);
  inverter_layer_destroy(il_inverted_topRow);
  inverter_layer_destroy(il_inverted_bottomRow);
}

static void window_load(Window *window) {
  // load localisation
  sys_locale = setlocale(LC_ALL, "");
  
  // create GBitmaps
  create_GBitmaps();
  
  // create BitmapLayers and set defaults
  create_BitmapLayers();
  
  // add BitmapLayers
  add_BitmapLayers();
  
  // create InverterLayers and set visibility defaults
  create_InverterLayers();
  
  // first call to show correct time, date, design and icons at start
  update_time();
  update_date();
  set_battery(battery_state_service_peek());
  set_bluetooth(bluetooth_connection_service_peek(), false);
  update_face();
  
  // show battery and bluetooth one time at start
  display_Battery_Bluetooth();
}

static void window_unload(Window *window) {
  // destroy BitmapLayers
  destroy_BitmapLayers();
  
  // destroy GBitmaps
  destroy_GBitmaps();
  
  // destroy InverterLayers
  destroy_InverterLayers();
}

static void initHandler() {
  // subscribe TickHandler
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // subscribe BatteryHandler
  battery_state_service_subscribe(battery_handler);
  
  // subscribe BluetoothHandler
  bluetooth_connection_service_subscribe(bluetooth_handler);
  
  // subscribe TapHandler
  accel_tap_service_subscribe(tap_handler);
}

static void deinitHandler() {
  // unsubscribe TickHandler
  tick_timer_service_unsubscribe();
  
  // unsubscribe BatteryHandler
  battery_state_service_unsubscribe();
  
  // unsubscribe BluetoothHandler
  bluetooth_connection_service_unsubscribe();
  
  // unsubscribe TapHandler
  accel_tap_service_unsubscribe();
}

static void initAppMessage() {
  // set variable defaults
  snprintf(configurable.invertFace, sizeof("false"), "false");
  snprintf(configurable.invertRow, sizeof("0"), "0");
  
  if (persist_exists(PERSIST_KEY_INVERTFACE))
    persist_read_string(PERSIST_KEY_INVERTFACE, configurable.invertFace, persist_get_size(PERSIST_KEY_INVERTFACE)); 
  else
    persist_write_string(PERSIST_KEY_INVERTFACE, configurable.invertFace);
  
  if (persist_exists(PERSIST_KEY_INVERTROW))
    persist_read_string(PERSIST_KEY_INVERTROW, configurable.invertRow, persist_get_size(PERSIST_KEY_INVERTROW)); 
  else
    persist_write_string(PERSIST_KEY_INVERTROW, configurable.invertRow);
  
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

static void init() {
  // create Window
  w_main_window = window_create();
  
  // set background color for Window
  window_set_background_color(w_main_window, GColorWhite);

  // initialize AppMessage
  initAppMessage();
  
  // subscribe Handlers
  initHandler();

  // set Window Handlers
  window_set_window_handlers(w_main_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });

  // show Window with animated = true
  window_stack_push(w_main_window, true);
}

static void deinit() {
  // unsubscribe Handlers
  deinitHandler();
  
  // destroy Window
  window_destroy(w_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}