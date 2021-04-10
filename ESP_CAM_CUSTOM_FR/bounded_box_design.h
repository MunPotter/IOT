int pan_center = 90; // center the pan servo
int tilt_center = 90; // center the tilt servo

static void rgb_print(dl_matrix3du_t *image_matrix, uint32_t color, const char * str) {
  fb_data_t fb;
  fb.width = image_matrix->w;
  fb.height = image_matrix->h;
  fb.data = image_matrix->item;
  fb.bytes_per_pixel = 3;
  fb.format = FB_BGR888;
  fb_gfx_print(&fb, (fb.width - (strlen(str) * 14)) / 2, 10, color, str);
}

static int rgb_printf(dl_matrix3du_t *image_matrix, uint32_t color, const char *format, ...) {
  char loc_buf[64];
  char * temp = loc_buf;
  int len;
  va_list arg;
  va_list copy;
  va_start(arg, format);
  va_copy(copy, arg);
  len = vsnprintf(loc_buf, sizeof(loc_buf), format, arg);
  va_end(copy);
  if (len >= sizeof(loc_buf)) {
    temp = (char*)malloc(len + 1);
    if (temp == NULL) {
      return 0;
    }
  }
  vsnprintf(temp, len + 1, format, arg);
  va_end(arg);
  rgb_print(image_matrix, color, temp);
  if (len > 64) {
    free(temp);
  }
  return len;
}


static void draw_face_boxes(dl_matrix3du_t *image_matrix, box_array_t *boxes, int face_id) {
  int x, y, w, h, i, half_width, half_height;
  uint32_t color = FACE_COLOR_GREEN;
  fb_data_t fb;
  fb.width = image_matrix->w;
  fb.height = image_matrix->h;
  fb.data = image_matrix->item;
  fb.bytes_per_pixel = 3;
  fb.format = FB_BGR888;
  for (i = 0; i < boxes->len; i++) {

    // Convoluted way of finding face centre...
    x = ((int)boxes->box[i].box_p[0]);
    w = (int)boxes->box[i].box_p[2] - x + 1;
    half_width = w / 2;
    int face_center_pan = x + half_width; // image frame face centre x co-ordinate

    y = (int)boxes->box[i].box_p[1];
    h = (int)boxes->box[i].box_p[3] - y + 1;
    half_height = h / 2;
    int face_center_tilt = y + half_height;  // image frame face centre y co-ordinate

    //    assume QVGA 320x240
    //        int sensor_width = 320;
    //        int sensor_height = 240;
    //        int lens_fov = 45
    //        float diagonal = sqrt(sq(sensor_width) + sq(sensor_height)); // pixels along the diagonal
    //        float pixels_per_degree = diagonal / lens_fov;
    //        400/45 = 8.89

    float move_to_x = pan_center + ((-160 + face_center_pan) / 8.89) ;
    float move_to_y = tilt_center + ((-120 + face_center_tilt) / 8.89) ;

    pan_center = (pan_center + move_to_x) / 2;
    Serial.println(pan_center);
    //    ledcAnalogWrite(2, pan_center); // channel, 0-180

    tilt_center = (tilt_center + move_to_y) / 2;
    int reversed_tilt_center = map(tilt_center, 0, 180, 180, 0);
    //    ledcAnalogWrite(4, reversed_tilt_center); // channel, 0-180

    fb_gfx_drawFastHLine(&fb, x, y, w, color);
    fb_gfx_drawFastHLine(&fb, x, y + h - 1, w, color);
    fb_gfx_drawFastVLine(&fb, x, y, h, color);
    fb_gfx_drawFastVLine(&fb, x + w - 1, y, h, color);
  }
}
