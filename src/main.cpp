#include <lvgl.h>
#include <TFT_eSPI.h>
#include <ui.h>

// defining variables
#define POT_PIN 15         // Potentiometer Pins
#define MAX_POT_VALUE 4095 // Maximum Value the Potentiometer can get
#define TANK_CAPACITY 6

// defining classes for ui value specification
extern lv_obj_t *ui_fuelLevelValue;
extern lv_obj_t *ui_batteryLevelValue;
extern lv_obj_t *ui_batteryLevelBarValue;

/*Change to your screen resolution*/
static const uint16_t screenWidth = 240;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char *buf)
{
  Serial.printf(buf);
  Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  uint16_t touchX = 0, touchY = 0;

  bool touched = false; // tft.getTouch( &touchX, &touchY, 600 );

  if (!touched)
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    data->point.x = touchX;
    data->point.y = touchY;

    Serial.print("Data x ");
    Serial.println(touchX);

    Serial.print("Data y ");
    Serial.println(touchY);
  }
}

void setup()
{
  Serial.begin(115200); /* prepare for possible serial debug */

  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");

  lv_init();

#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

  tft.begin();        /* TFT init */
  tft.setRotation(3); /* Landscape orientation, flipped */

  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 10);

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  ui_init();

  Serial.println("Setup done");
}

void loop()
{
  // Fuel Level Value input
  float potValue = analogRead(POT_PIN);
  float scaledPotValueMax = MAX_POT_VALUE / 1000;
  float scaledPotValue = potValue / 1000;
  int fuelValue = map(potValue, 0, MAX_POT_VALUE, 0, 100);
  float remainingValue = map(scaledPotValue, 0, scaledPotValueMax, 0, TANK_CAPACITY);
  Serial.printf("Fuel Value: %dL\n", fuelValue);
  Serial.printf("Remaining Value: %.2f\n", remainingValue);

  lv_arc_set_value(ui_fuelLevelValue, fuelValue);
  lv_label_set_text_fmt(ui_remainingValue, "%.2f", remainingValue);

  lv_timer_handler(); /* let the GUI do its work */
  delay(5);
}
