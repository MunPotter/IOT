#include "include.h"
#include "camera_pins.h"
#include "html.h"
#include "bounded_box_design.h"

String recognize_face_matched_name[10] = {"Name0", "Name1", "Name2", "Name3", "Name4", "Name5", "Name6", "Name7", "Name8", "Name9"};

const char* ssid = "RokkhiTechTPLink";
const char* password = "RokkhiTech2021";


boolean controlState = false;


String Feedback = "";
String incomingByte ;

String Command = "", cmd = "", P1 = "", P2 = "", P3 = "", P4 = "", P5 = "", P6 = "", P7 = "", P8 = "", P9 = "";
byte ReceiveState = 0, cmdState = 1, strState = 1, questionstate = 0, equalstate = 0, semicolonstate = 0;



typedef struct {
  size_t size;
  size_t index;
  size_t count;
  int sum;
  int * values;
} ra_filter_t;

typedef struct {
  httpd_req_t *req;
  size_t len;
} jpg_chunking_t;

#define PART_BOUNDARY "123456789000000000000987654321"
static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

static ra_filter_t ra_filter;
httpd_handle_t stream_httpd = NULL;
httpd_handle_t camera_httpd = NULL;

static mtmn_config_t mtmn_config = {0};
static int8_t detection_enabled = 0;
static int8_t recognition_enabled = 0;
static int8_t is_enrolling = 0;
static face_id_list id_list = {0};
static int8_t flash_value = 0;


void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }


  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }


  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_QVGA);


  ledcAttachPin(4, 4);
  ledcSetup(4, 5000, 8);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
}

void loop() {
  delay(10000);
}


void startCameraServer() {
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();  


  httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_handler,
    .user_ctx  = NULL
  };


  httpd_uri_t status_uri = {
    .uri       = "/status",
    .method    = HTTP_GET,
    .handler   = status_handler,
    .user_ctx  = NULL
  };


  httpd_uri_t cmd_uri = {
    .uri       = "/control",
    .method    = HTTP_GET,
    .handler   = cmd_handler,
    .user_ctx  = NULL
  };


  httpd_uri_t capture_uri = {
    .uri       = "/capture",
    .method    = HTTP_GET,
    .handler   = capture_handler,
    .user_ctx  = NULL
  };


  httpd_uri_t stream_uri = {
    .uri       = "/stream",
    .method    = HTTP_GET,
    .handler   = stream_handler,
    .user_ctx  = NULL
  };


  ra_filter_init(&ra_filter, 20);

  mtmn_config.type = FAST;
  mtmn_config.min_face = 60;
  mtmn_config.pyramid = 0.707;
  mtmn_config.pyramid_times = 4;
  mtmn_config.p_threshold.score = 0.6;
  mtmn_config.p_threshold.nms = 0.7;
  mtmn_config.p_threshold.candidate_number = 20;
  mtmn_config.r_threshold.score = 0.7;
  mtmn_config.r_threshold.nms = 0.7;
  mtmn_config.r_threshold.candidate_number = 10;
  mtmn_config.o_threshold.score = 0.7;
  mtmn_config.o_threshold.nms = 0.7;
  mtmn_config.o_threshold.candidate_number = 1;

  face_id_init(&id_list, FACE_ID_SAVE_NUMBER, ENROLL_CONFIRM_TIMES);

  Serial.printf("Starting web server on port: '%d'\n", config.server_port);  //TCP Port
  if (httpd_start(&camera_httpd, &config) == ESP_OK) {

    httpd_register_uri_handler(camera_httpd, &index_uri);
    httpd_register_uri_handler(camera_httpd, &cmd_uri);
    httpd_register_uri_handler(camera_httpd, &status_uri);
    httpd_register_uri_handler(camera_httpd, &capture_uri);
  }

  config.server_port += 1;
  config.ctrl_port += 1;
  Serial.printf("Starting stream server on port: '%d'\n", config.server_port);
  if (httpd_start(&stream_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(stream_httpd, &stream_uri);
  }
}



static esp_err_t index_handler(httpd_req_t *req) {
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, (const char *)INDEX_HTML, strlen(INDEX_HTML));
}


static esp_err_t status_handler(httpd_req_t *req) {
  static char json_response[1024];

  sensor_t * s = esp_camera_sensor_get();
  char * p = json_response;
  *p++ = '{';

  p += sprintf(p, "\"framesize\":%u,", s->status.framesize);
  p += sprintf(p, "\"quality\":%u,", s->status.quality);
  p += sprintf(p, "\"brightness\":%d,", s->status.brightness);
  p += sprintf(p, "\"contrast\":%d,", s->status.contrast);
  p += sprintf(p, "\"face_detect\":%u,", detection_enabled);
  p += sprintf(p, "\"face_enroll\":%u,", is_enrolling);
  p += sprintf(p, "\"face_recognize\":%u,", recognition_enabled);
  p += sprintf(p, "\"flash\":%u", flash_value);
  *p++ = '}';
  *p++ = 0;
  httpd_resp_set_type(req, "application/json");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, json_response, strlen(json_response));
}


static esp_err_t cmd_handler(httpd_req_t *req) {
  char*  buf;
  size_t buf_len;
  char variable[128] = {0,};
  char value[128] = {0,};
  String myCmd = "";

  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) {
    buf = (char*)malloc(buf_len);

    if (!buf) {
      httpd_resp_send_500(req);
      return ESP_FAIL;
    }
    if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
      if (httpd_query_key_value(buf, "var", variable, sizeof(variable)) == ESP_OK &&
          httpd_query_key_value(buf, "val", value, sizeof(value)) == ESP_OK) {
      }
      else {
        myCmd = String(buf);
      }
    }
  } else {
    httpd_resp_send_404(req);
    return ESP_FAIL;
  }

  Feedback = ""; Command = ""; cmd = ""; P1 = ""; P2 = ""; P3 = ""; P4 = ""; P5 = ""; P6 = ""; P7 = ""; P8 = ""; P9 = "";
  ReceiveState = 0, cmdState = 1, strState = 1, questionstate = 0, equalstate = 0, semicolonstate = 0;
  if (myCmd.length() > 0) {
    myCmd = "?" + myCmd;
    for (int i = 0; i < myCmd.length(); i++) {
      getCommand(char(myCmd.charAt(i)));
    }
  }

  if (cmd.length() > 0) {
    Serial.println("");
    //Serial.println("Command: "+Command);
    Serial.println("cmd= " + cmd + " ,P1= " + P1 + " ,P2= " + P2 + " ,P3= " + P3 + " ,P4= " + P4 + " ,P5= " + P5 + " ,P6= " + P6 + " ,P7= " + P7 + " ,P8= " + P8 + " ,P9= " + P9);
    Serial.println("");


    if (cmd == "your cmd") {
      // You can do anything
      // Feedback="<font color=\"red\">Hello World</font>";
    }
    else if (cmd == "facename") {
      recognize_face_matched_name[P1.toInt()] = P2;

      int name_length = sizeof(recognize_face_matched_name) / sizeof(recognize_face_matched_name[0]);
      Feedback = "<table style=\"color:white\"><tr><td>matched_id</td><td>name</td></tr>";
      for (int i = 0; i < name_length; i++) {
        Feedback += "<tr><td>" + String(i) + "</td><td>" + recognize_face_matched_name[i] + "</td></tr>";
      }
      Feedback += "</table>";
    }
    else if (cmd == "deleteface") {
      delete_face(&id_list);
      face_id_init(&id_list, FACE_ID_SAVE_NUMBER, ENROLL_CONFIRM_TIMES);

      int name_length = sizeof(recognize_face_matched_name) / sizeof(recognize_face_matched_name[0]);
      Feedback = "<table style=\"color:white\"><tr><td>matched_id</td><td>name</td></tr>";
      for (int i = 0; i < name_length; i++) {
        Feedback += "<tr><td>" + String(i) + "</td><td>" + recognize_face_matched_name[i] + "</td></tr>";
      }
      Feedback += "</table>";
    }
    else if (cmd == "controlstate") {
      controlState = (P1.toInt() == 1);
    }
    else {
      Feedback = "Command is not defined";
    }

    if (Feedback == "") Feedback = Command;

    const char *resp = Feedback.c_str();
    httpd_resp_set_type(req, "text/html");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    return httpd_resp_send(req, resp, strlen(resp));
  }
  else {
    int val = atoi(value);
    sensor_t * s = esp_camera_sensor_get();
    int res = 0;


    if (!strcmp(variable, "framesize")) {
      if (s->pixformat == PIXFORMAT_JPEG) res = s->set_framesize(s, (framesize_t)val);
    }
    else if (!strcmp(variable, "quality")) res = s->set_quality(s, val);
    else if (!strcmp(variable, "contrast")) res = s->set_contrast(s, val);
    else if (!strcmp(variable, "brightness")) res = s->set_brightness(s, val);
    else if (!strcmp(variable, "face_detect")) {
      detection_enabled = val;
      if (!detection_enabled) {
        recognition_enabled = 0;
      }
    }
    else if (!strcmp(variable, "face_enroll")) is_enrolling = val;
    else if (!strcmp(variable, "face_recognize")) {
      recognition_enabled = val;
      if (recognition_enabled) {
        detection_enabled = val;
      }
    }
    else if (!strcmp(variable, "flash")) { //Control flash
      ledcWrite(4, val);
      flash_value = val;
    }
    else {
      res = -1;
    }

    if (res) {
      return httpd_resp_send_500(req);
    }

    if (buf) {
      Feedback = String(buf);
      const char *resp = Feedback.c_str();
      httpd_resp_set_type(req, "text/html");
      httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
      return httpd_resp_send(req, resp, strlen(resp));
    }
    else {
      httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
      return httpd_resp_send(req, NULL, 0);
    }
  }
}


static esp_err_t capture_handler(httpd_req_t *req) {
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  int64_t fr_start = esp_timer_get_time();

  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    httpd_resp_send_500(req);
    return ESP_FAIL;
  }

  httpd_resp_set_type(req, "image/jpeg");
  httpd_resp_set_hdr(req, "Content-Disposition", "inline; filename=capture.jpg");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");

  size_t out_len, out_width, out_height;
  uint8_t * out_buf;
  bool s;
  bool detected = false;
  int face_id = 0;
  if (!detection_enabled || fb->width > 400) {
    size_t fb_len = 0;
    if (fb->format == PIXFORMAT_JPEG) {
      fb_len = fb->len;
      res = httpd_resp_send(req, (const char *)fb->buf, fb->len);
    } else {
      jpg_chunking_t jchunk = {req, 0};
      res = frame2jpg_cb(fb, 80, jpg_encode_stream, &jchunk) ? ESP_OK : ESP_FAIL;
      httpd_resp_send_chunk(req, NULL, 0);
      fb_len = jchunk.len;
    }
    esp_camera_fb_return(fb);
    int64_t fr_end = esp_timer_get_time();
    Serial.printf("JPG: %uB %ums\n", (uint32_t)(fb_len), (uint32_t)((fr_end - fr_start) / 1000));
    return res;
  }

  dl_matrix3du_t *image_matrix = dl_matrix3du_alloc(1, fb->width, fb->height, 3);
  if (!image_matrix) {
    esp_camera_fb_return(fb);
    Serial.println("dl_matrix3du_alloc failed");
    httpd_resp_send_500(req);
    return ESP_FAIL;
  }

  out_buf = image_matrix->item;
  out_len = fb->width * fb->height * 3;
  out_width = fb->width;
  out_height = fb->height;

  s = fmt2rgb888(fb->buf, fb->len, fb->format, out_buf);
  esp_camera_fb_return(fb);
  if (!s) {
    dl_matrix3du_free(image_matrix);
    Serial.println("to rgb888 failed");
    httpd_resp_send_500(req);
    return ESP_FAIL;
  }

  box_array_t *net_boxes = face_detect(image_matrix, &mtmn_config);

  if (net_boxes) {
    //Serial.println("faces = " + String(net_boxes->len));
    detected = true;
    if (recognition_enabled) {
      face_id = run_face_recognition(image_matrix, net_boxes);
    }
    draw_face_boxes(image_matrix, net_boxes, face_id);
    free(net_boxes->score);
    free(net_boxes->box);
    free(net_boxes->landmark);
    free(net_boxes);
  }

  jpg_chunking_t jchunk = {req, 0};
  s = fmt2jpg_cb(out_buf, out_len, out_width, out_height, PIXFORMAT_RGB888, 90, jpg_encode_stream, &jchunk);
  dl_matrix3du_free(image_matrix);
  if (!s) {
    Serial.println("JPEG compression failed");
    return ESP_FAIL;
  }

  int64_t fr_end = esp_timer_get_time();
  Serial.printf("FACE: %uB %ums %s%d\n", (uint32_t)(jchunk.len), (uint32_t)((fr_end - fr_start) / 1000), detected ? "DETECTED " : "", face_id);
  return res;
}


static esp_err_t stream_handler(httpd_req_t *req) {
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  char * part_buf[64];
  dl_matrix3du_t *image_matrix = NULL;
  bool detected = false;
  int face_id = 0;
  int64_t fr_start = 0;
  int64_t fr_ready = 0;
  int64_t fr_face = 0;
  int64_t fr_recognize = 0;
  int64_t fr_encode = 0;

  static int64_t last_frame = 0;
  if (!last_frame) {
    last_frame = esp_timer_get_time();
  }

  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if (res != ESP_OK) {
    return res;
  }
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");

  while (true) {
    detected = false;
    face_id = 0;
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      res = ESP_FAIL;
    } else {
      fr_start = esp_timer_get_time();
      fr_ready = fr_start;
      fr_face = fr_start;
      fr_encode = fr_start;
      fr_recognize = fr_start;
      if (!detection_enabled || fb->width > 400) {
        if (fb->format != PIXFORMAT_JPEG) {
          bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
          esp_camera_fb_return(fb);
          fb = NULL;
          if (!jpeg_converted) {
            Serial.println("JPEG compression failed");
            res = ESP_FAIL;
          }
        } else {
          _jpg_buf_len = fb->len;
          _jpg_buf = fb->buf;
        }
      } else {

        image_matrix = dl_matrix3du_alloc(1, fb->width, fb->height, 3);

        if (!image_matrix) {
          Serial.println("dl_matrix3du_alloc failed");
          res = ESP_FAIL;
        } else {
          if (!fmt2rgb888(fb->buf, fb->len, fb->format, image_matrix->item)) {
            Serial.println("fmt2rgb888 failed");
            res = ESP_FAIL;
          } else {
            fr_ready = esp_timer_get_time();
            box_array_t *net_boxes = NULL;
            if (detection_enabled) {
              net_boxes = face_detect(image_matrix, &mtmn_config);
            }
            fr_face = esp_timer_get_time();
            fr_recognize = fr_face;
            if (net_boxes || fb->format != PIXFORMAT_JPEG) {
              if (net_boxes) {
                detected = true;
                if (recognition_enabled) {
                  face_id = run_face_recognition(image_matrix, net_boxes);
                }
                fr_recognize = esp_timer_get_time();
                draw_face_boxes(image_matrix, net_boxes, face_id);
                free(net_boxes->score);
                free(net_boxes->box);
                free(net_boxes->landmark);
                free(net_boxes);
              }
              if (!fmt2jpg(image_matrix->item, fb->width * fb->height * 3, fb->width, fb->height, PIXFORMAT_RGB888, 90, &_jpg_buf, &_jpg_buf_len)) {
                Serial.println("fmt2jpg failed");
                res = ESP_FAIL;
              }
              esp_camera_fb_return(fb);
              fb = NULL;
            } else {
              _jpg_buf = fb->buf;
              _jpg_buf_len = fb->len;
            }
            fr_encode = esp_timer_get_time();
          }
          dl_matrix3du_free(image_matrix);
        }
      }
    }
    if (res == ESP_OK) {
      size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if (res == ESP_OK) {
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    if (res == ESP_OK) {
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }
    if (fb) {
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    } else if (_jpg_buf) {
      free(_jpg_buf);
      _jpg_buf = NULL;
    }
    if (res != ESP_OK) {
      break;
    }
    int64_t fr_end = esp_timer_get_time();

    int64_t ready_time = (fr_ready - fr_start) / 1000;
    int64_t face_time = (fr_face - fr_ready) / 1000;
    int64_t recognize_time = (fr_recognize - fr_face) / 1000;
    int64_t encode_time = (fr_encode - fr_recognize) / 1000;
    int64_t process_time = (fr_encode - fr_start) / 1000;

    int64_t frame_time = fr_end - last_frame;
    last_frame = fr_end;
    frame_time /= 1000;
    uint32_t avg_frame_time = ra_filter_run(&ra_filter, frame_time);
    //        Serial.printf("MJPG: %uB %ums (%.1ffps), AVG: %ums (%.1ffps), %u+%u+%u+%u=%u %s%d\n",
    //            (uint32_t)(_jpg_buf_len),
    //            (uint32_t)frame_time, 1000.0 / (uint32_t)frame_time,
    //            avg_frame_time, 1000.0 / avg_frame_time,
    //            (uint32_t)ready_time, (uint32_t)face_time, (uint32_t)recognize_time, (uint32_t)encode_time, (uint32_t)process_time,
    //            (detected)?"DETECTED ":"", face_id
    //        );
  }

  last_frame = 0;
  return res;
}


static int run_face_recognition(dl_matrix3du_t *image_matrix, box_array_t *net_boxes) {
  dl_matrix3du_t *aligned_face = NULL;
  int matched_id = 0;

  aligned_face = dl_matrix3du_alloc(1, FACE_WIDTH, FACE_HEIGHT, 3);
  if (!aligned_face) {
    Serial.println("Could not allocate face recognition buffer");
    return matched_id;
  }
  if (align_face(net_boxes, image_matrix, aligned_face) == ESP_OK) {
    if (is_enrolling == 1) {
      int8_t left_sample_face = enroll_face(&id_list, aligned_face);

      if (left_sample_face == (ENROLL_CONFIRM_TIMES - 1)) {
        Serial.printf("Enrolling Face ID: %d\n", id_list.tail);
      }
      Serial.printf("Enrolling Face ID: %d sample %d\n", id_list.tail, ENROLL_CONFIRM_TIMES - left_sample_face);
      rgb_printf(image_matrix, FACE_COLOR_CYAN, "ID[%u] Sample[%u]", id_list.tail, ENROLL_CONFIRM_TIMES - left_sample_face);
      if (left_sample_face == 0) {
        is_enrolling = 0;
        Serial.printf("Enrolled Face ID: %d\n", id_list.tail);
      }
    } else {
      matched_id = recognize_face(&id_list, aligned_face);
      if (matched_id >= 0) {
        Serial.printf("Match Face ID: %u\n", matched_id);
        int name_length = sizeof(recognize_face_matched_name) / sizeof(recognize_face_matched_name[0]);


        if (matched_id < name_length)
          rgb_printf(image_matrix, FACE_COLOR_GREEN, "[%u] %s", matched_id, recognize_face_matched_name[matched_id]);
        else
          rgb_printf(image_matrix, FACE_COLOR_GREEN, "[%u] No Name", matched_id);
        //                FaceMatched(matched_id);
      } else {

        Serial.println("No Match Found");
        rgb_print(image_matrix, FACE_COLOR_RED, "Unknown Face");
        matched_id = -1;
        //                FaceNoMatched();
      }
    }
  } else {
    Serial.println("Face Not Aligned");
    rgb_print(image_matrix, FACE_COLOR_YELLOW, "Face Not Aligned");
  }

  dl_matrix3du_free(aligned_face);
  return matched_id;
}




static ra_filter_t * ra_filter_init(ra_filter_t * filter, size_t sample_size) {
  memset(filter, 0, sizeof(ra_filter_t));

  filter->values = (int *)malloc(sample_size * sizeof(int));
  if (!filter->values) {
    return NULL;
  }
  memset(filter->values, 0, sample_size * sizeof(int));

  filter->size = sample_size;
  return filter;
}

static int ra_filter_run(ra_filter_t * filter, int value) {
  if (!filter->values) {
    return value;
  }
  filter->sum -= filter->values[filter->index];
  filter->values[filter->index] = value;
  filter->sum += filter->values[filter->index];
  filter->index++;
  filter->index = filter->index % filter->size;
  if (filter->count < filter->size) {
    filter->count++;
  }
  return filter->sum / filter->count;
}


static size_t jpg_encode_stream(void * arg, size_t index, const void* data, size_t len) {
  jpg_chunking_t *j = (jpg_chunking_t *)arg;
  if (!index) {
    j->len = 0;
  }
  if (httpd_resp_send_chunk(j->req, (const char *)data, len) != ESP_OK) {
    return 0;
  }
  j->len += len;
  return len;
}


void getCommand(char c)
{
  if (c == '?') ReceiveState = 1;
  if ((c == ' ') || (c == '\r') || (c == '\n')) ReceiveState = 0;

  if (ReceiveState == 1)
  {
    Command = Command + String(c);

    if (c == '=') cmdState = 0;
    if (c == ';') strState++;

    if ((cmdState == 1) && ((c != '?') || (questionstate == 1))) cmd = cmd + String(c);
    if ((cmdState == 0) && (strState == 1) && ((c != '=') || (equalstate == 1))) P1 = P1 + String(c);
    if ((cmdState == 0) && (strState == 2) && (c != ';')) P2 = P2 + String(c);
    if ((cmdState == 0) && (strState == 3) && (c != ';')) P3 = P3 + String(c);
    if ((cmdState == 0) && (strState == 4) && (c != ';')) P4 = P4 + String(c);
    if ((cmdState == 0) && (strState == 5) && (c != ';')) P5 = P5 + String(c);
    if ((cmdState == 0) && (strState == 6) && (c != ';')) P6 = P6 + String(c);
    if ((cmdState == 0) && (strState == 7) && (c != ';')) P7 = P7 + String(c);
    if ((cmdState == 0) && (strState == 8) && (c != ';')) P8 = P8 + String(c);
    if ((cmdState == 0) && (strState >= 9) && ((c != ';') || (semicolonstate == 1))) P9 = P9 + String(c);

    if (c == '?') questionstate = 1;
    if (c == '=') equalstate = 1;
    if ((strState >= 9) && (c == ';')) semicolonstate = 1;
  }
}
