// camera.cpp - Implementación de funciones de la cámara

#include "camera.h"
#include "sd_card.h"

// Variables externas declaradas en config.h
extern uint8_t tempBuffer[BUFFER_SIZE];

// Inicialización de la cámara
bool initCamera() {
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
  
  // Configuración inicial con mejor calidad
  config.frame_size = FRAMESIZE_SVGA;
  config.jpeg_quality = 12; // 0-63 (menor valor = mayor calidad)
  config.fb_count = 1;
  
  // Inicializar la cámara
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("El inicio de la cámara falló con error 0x%x", err);
    return false;
  }
  
  // Ajustes adicionales de la cámara
  sensor_t * s = esp_camera_sensor_get();
  s->set_brightness(s, 0);     // -2 a 2
  s->set_contrast(s, 0);       // -2 a 2
  s->set_saturation(s, 0);     // -2 a 2
  s->set_special_effect(s, 0); // 0 = No Effect, 1 = Negative, 2 = Grayscale...
  s->set_whitebal(s, 1);       // 0 = disable, 1 = enable
  s->set_awb_gain(s, 1);       // 0 = disable, 1 = enable
  s->set_wb_mode(s, 0);        // 0 a 4 - Auto, Sunny, Cloudy, Office, Home
  s->set_exposure_ctrl(s, 1);  // 0 = disable, 1 = enable
  s->set_aec2(s, 0);           // 0 = disable, 1 = enable
  s->set_gain_ctrl(s, 1);      // 0 = disable, 1 = enable
  s->set_agc_gain(s, 0);       // 0 a 30
  s->set_gainceiling(s, (gainceiling_t)0);  // 0 a 6
  s->set_bpc(s, 0);            // 0 = disable, 1 = enable
  s->set_wpc(s, 1);            // 0 = disable, 1 = enable
  s->set_raw_gma(s, 1);        // 0 = disable, 1 = enable
  s->set_lenc(s, 1);           // 0 = disable, 1 = enable
  s->set_hmirror(s, 0);        // 0 = disable, 1 = enable
  s->set_vflip(s, 0);          // 0 = disable, 1 = enable
  s->set_dcw(s, 1);            // 0 = disable, 1 = enable
  
  return true;
}

// Capturar una foto y guardarla en la SD
String capturePhotoSaveSD() {
  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();
  if(!fb) {
    Serial.println("Fallo en la captura de la cámara");
    return "";
  }
  
  String filename = "/photos/photo_" + String(photoCount) + ".jpg";
  File photoFile = SD_MMC.open(filename, FILE_WRITE);
  if(!photoFile) {
    Serial.println("Error al abrir el archivo en la SD");
    esp_camera_fb_return(fb);
    return "";
  }
  
  photoFile.write(fb->buf, fb->len);
  photoFile.close();
  esp_camera_fb_return(fb);
  
  photoCount++;
  Serial.println("Foto guardada: " + filename);
  return filename;
}

// Callback para enviar las imágenes JPEG por chunks
static size_t jpg_encode_stream(void * arg, size_t index, const void* data, size_t len) {
  jpg_chunking_t *j = (jpg_chunking_t *)arg;
  if(!index) {
    httpd_resp_set_type(j->req, "image/jpeg");
    httpd_resp_set_hdr(j->req, "Content-Disposition", "inline; filename=capture.jpg");
  }
  if(httpd_resp_send_chunk(j->req, (const char *)data, len) != ESP_OK) {
    return 0;
  }
  j->len += len;
  return len;
}