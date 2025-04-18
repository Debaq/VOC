// web_server.cpp - Implementación del servidor web y manejadores HTTP

#include "web_server.h"

// Variables globales para los servidores HTTP
extern httpd_handle_t camera_httpd;
extern httpd_handle_t stream_httpd;
extern String lastPhotoFilename;

// Manejador HTTP para la página principal (portal cautivo)
static esp_err_t index_handler(httpd_req_t *req) {
  httpd_resp_set_type(req, "text/html");
  
  if (hasSD && fileExists(HTML_INDEX)) {
    // Leer archivo HTML desde la SD
    File file = SD_MMC.open(HTML_INDEX, FILE_READ);
    if (file) {
      // Enviar el archivo en chunks
      size_t bytesRead;
      while ((bytesRead = file.read(tempBuffer, BUFFER_SIZE)) > 0) {
        httpd_resp_send_chunk(req, (const char*)tempBuffer, bytesRead);
      }
      file.close();
      httpd_resp_send_chunk(req, NULL, 0);
      return ESP_OK;
    }
  }
  
  // Si no se pudo leer el archivo, mostrar página de respaldo
  if (hasSD && fileExists(HTML_FALLBACK)) {
    String html = readFile(HTML_FALLBACK);
    return httpd_resp_sendstr(req, html.c_str());
  } else {
    // HTML de respaldo incorporado como último recurso
    String fallbackHtml = "<!DOCTYPE html><html><head><title>ESP32-CAM</title></head><body>";
    fallbackHtml += "<h1>ESP32-CAM Portal</h1><p>Error: No se pudo cargar la interfaz</p>";
    fallbackHtml += "<img src='/stream' width='640' height='480'>";
    fallbackHtml += "</body></html>";
    return httpd_resp_sendstr(req, fallbackHtml.c_str());
  }
}

// Manejador para servir archivos CSS
static esp_err_t css_handler(httpd_req_t *req) {
  httpd_resp_set_type(req, "text/css");
  
  if (hasSD && fileExists(CSS_MAIN)) {
    File file = SD_MMC.open(CSS_MAIN, FILE_READ);
    if (file) {
      // Enviar el archivo en chunks
      size_t bytesRead;
      while ((bytesRead = file.read(tempBuffer, BUFFER_SIZE)) > 0) {
        httpd_resp_send_chunk(req, (const char*)tempBuffer, bytesRead);
      }
      file.close();
      httpd_resp_send_chunk(req, NULL, 0);
      return ESP_OK;
    }
  }
  
  return httpd_resp_sendstr(req, "/* Error: CSS file not found */");
}

// Manejador para servir archivos JavaScript
static esp_err_t js_handler(httpd_req_t *req) {
  httpd_resp_set_type(req, "application/javascript");
  
  if (hasSD && fileExists(JS_MAIN)) {
    File file = SD_MMC.open(JS_MAIN, FILE_READ);
    if (file) {
      // Enviar el archivo en chunks
      size_t bytesRead;
      while ((bytesRead = file.read(tempBuffer, BUFFER_SIZE)) > 0) {
        httpd_resp_send_chunk(req, (const char*)tempBuffer, bytesRead);
      }
      file.close();
      httpd_resp_send_chunk(req, NULL, 0);
      return ESP_OK;
    }
  }
  
  return httpd_resp_sendstr(req, "/* Error: JavaScript file not found */");
}

// Manejador para el área de administración
static esp_err_t admin_handler(httpd_req_t *req) {
  // Verificar autenticación
  if (!isAuthenticated(req)) {
    return ESP_OK; // Ya se envió respuesta de error 401
  }
  
  httpd_resp_set_type(req, "text/html");
  
  if (hasSD && fileExists(HTML_ADMIN)) {
    File file = SD_MMC.open(HTML_ADMIN, FILE_READ);
    if (file) {
      // Enviar el archivo en chunks
      size_t bytesRead;
      while ((bytesRead = file.read(tempBuffer, BUFFER_SIZE)) > 0) {
        httpd_resp_send_chunk(req, (const char*)tempBuffer, bytesRead);
      }
      file.close();
      httpd_resp_send_chunk(req, NULL, 0);
      return ESP_OK;
    }
  }
  
  // HTML de respaldo para admin
  String adminHtml = "<!DOCTYPE html><html><head><title>ESP32-CAM Admin</title></head><body>";
  adminHtml += "<h1>Área Administrativa</h1><p>Error: No se pudo cargar la interfaz administrativa</p>";
  adminHtml += "</body></html>";
  return httpd_resp_sendstr(req, adminHtml.c_str());
}

// Manejador para la configuración WiFi
static esp_err_t wifi_handler(httpd_req_t *req) {
  httpd_resp_set_type(req, "text/html");
  
  if (hasSD && fileExists(HTML_WIFI)) {
    File file = SD_MMC.open(HTML_WIFI, FILE_READ);
    if (file) {
      // Enviar el archivo en chunks
      size_t bytesRead;
      while ((bytesRead = file.read(tempBuffer, BUFFER_SIZE)) > 0) {
        httpd_resp_send_chunk(req, (const char*)tempBuffer, bytesRead);
      }
      file.close();
      httpd_resp_send_chunk(req, NULL, 0);
      return ESP_OK;
    }
  }
  
  // HTML de respaldo para configuración WiFi
  String wifiHtml = "<!DOCTYPE html><html><head><title>ESP32-CAM WiFi Config</title></head><body>";
  wifiHtml += "<h1>Configuración WiFi</h1>";
  wifiHtml += "<form action='/save-wifi' method='post'>";
  wifiHtml += "<label>SSID: <input type='text' name='ssid'></label><br>";
  wifiHtml += "<label>Password: <input type='password' name='password'></label><br>";
  wifiHtml += "<input type='submit' value='Guardar'>";
  wifiHtml += "</form>";
  wifiHtml += "<p><a href='/'>Volver al inicio</a></p>";
  wifiHtml += "</body></html>";
  return httpd_resp_sendstr(req, wifiHtml.c_str());
}

static esp_err_t stream_handler(httpd_req_t *req) {
  Serial.println("Solicitud de stream recibida");
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  char * part_buf[64];
  
  static int64_t last_frame = 0;
  if(!last_frame) {
    last_frame = esp_timer_get_time();
  }

  res = httpd_resp_set_type(req, "multipart/x-mixed-replace;boundary=frame");
  if(res != ESP_OK) {
    Serial.println("Error al configurar el tipo de respuesta HTTP");
    return res;
  }

  Serial.println("Iniciando bucle de streaming");
  
  int frame_count = 0;
  while(true) {
    Serial.print(".");  // Indicador de actividad menos verboso
    if (frame_count % 10 == 0) Serial.println(); // Nueva línea cada 10 frames
    
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Error al capturar frame para streaming");
      res = ESP_FAIL;
    } else {
      frame_count++;
      
      if(fb->format != PIXFORMAT_JPEG) {
        bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
        esp_camera_fb_return(fb);
        fb = NULL;
        if(!jpeg_converted) {
          Serial.println("Error al convertir a JPEG");
          res = ESP_FAIL;
        }
      } else {
        _jpg_buf_len = fb->len;
        _jpg_buf = fb->buf;
      }
    }
    
    if(res == ESP_OK) {
      size_t hlen = snprintf((char *)part_buf, 64, "\r\n--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n", _jpg_buf_len);

      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    
    if(res == ESP_OK) {
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    
    if(fb) {
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    } else if(_jpg_buf) {
      free(_jpg_buf);
      _jpg_buf = NULL;
    }
    
    if(res != ESP_OK) {
      Serial.println("Error en stream, finalizando");
      break;
    }
    
    // Limitar velocidad para pruebas y evitar sobrecarga
    delay(100);
  }
  
  Serial.println("Stream finalizado");
  return res;
}

// Manejador HTTP para cambiar ajustes de la cámara
static esp_err_t cmd_handler(httpd_req_t *req) {
  char*  buf;
  size_t buf_len;
  char variable[32] = {0,};
  char value[32] = {0,};

  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) {
    buf = (char*)malloc(buf_len);
    if(!buf) {
      httpd_resp_send_500(req);
      return ESP_FAIL;
    }
    if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
      if (httpd_query_key_value(buf, "var", variable, sizeof(variable)) == ESP_OK &&
          httpd_query_key_value(buf, "val", value, sizeof(value)) == ESP_OK) {
      } else {
        free(buf);
        httpd_resp_send_404(req);
        return ESP_FAIL;
      }
    } else {
      free(buf);
      httpd_resp_send_404(req);
      return ESP_FAIL;
    }
    free(buf);
  } else {
    httpd_resp_send_404(req);
    return ESP_FAIL;
  }

  sensor_t * s = esp_camera_sensor_get();
  int res = 0;
  
  // Procesar comandos específicos
  if(!strcmp(variable, "resolution")) {
    if(!strcmp(value, "VGA"))
      res = s->set_framesize(s, FRAMESIZE_VGA);
    else if(!strcmp(value, "SVGA"))
      res = s->set_framesize(s, FRAMESIZE_SVGA);
    else if(!strcmp(value, "XGA"))
      res = s->set_framesize(s, FRAMESIZE_XGA);
  }
  else if(!strcmp(variable, "special_effect")) {
    res = s->set_special_effect(s, atoi(value));
  }
  
  // Ajustes adicionales pueden ser agregados aquí
  
  const char * resp = "OK";
  httpd_resp_set_type(req, "text/html");
  if(res){
    return httpd_resp_send_500(req);
  }
  
  return httpd_resp_send(req, resp, strlen(resp));
}

// Manejador HTTP para capturar foto
static esp_err_t capture_handler(httpd_req_t *req) {
  lastPhotoFilename = capturePhotoSaveSD();
  
  httpd_resp_set_type(req, "text/plain");
  if(lastPhotoFilename != "") {
    return httpd_resp_sendstr(req, lastPhotoFilename.c_str());
  } else {
    return httpd_resp_send_500(req);
  }
}

// Manejador HTTP para listar fotos
static esp_err_t list_handler(httpd_req_t *req) {
  httpd_resp_set_type(req, "application/json");
  
  String json = "[";
  File root = SD_MMC.open(PHOTOS_DIR);
  if(!root || !root.isDirectory()) {
    root.close();
    return httpd_resp_sendstr(req, "[]");
  }
  
  int count = 0;
  File file = root.openNextFile();
  while(file) {
    if(!file.isDirectory() && String(file.name()).endsWith(".jpg")) {
      if(count > 0) json += ",";
      String filename = String(file.name());
      String filepath = "/photos/" + filename;
      json += "{\"name\":\"" + filename + "\",";
      json += "\"path\":\"" + filepath + "\",";
      json += "\"thumb\":\"" + filepath + "\",";
      json += "\"size\":" + String(file.size()) + "}";
      count++;
    }
    file.close();
    file = root.openNextFile();
  }
  root.close();
  
  json += "]";
  return httpd_resp_sendstr(req, json.c_str());
}

// Manejador HTTP para servir archivos de la SD
static esp_err_t sd_file_handler(httpd_req_t *req) {
  char *filepath = (char*)req->uri;
  
  // Saltar el / inicial
  filepath++;
  
  // Verificar si el archivo existe
  if(!SD_MMC.exists(filepath)) {
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "File not found");
    return ESP_FAIL;
  }
  
  // Abrir archivo
  File file = SD_MMC.open(filepath, FILE_READ);
  if(!file) {
    httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to open file");
    return ESP_FAIL;
  }
  
  // Establecer el tipo de contenido según la extensión
  if(String(filepath).endsWith(".jpg")) {
    httpd_resp_set_type(req, "image/jpeg");
  } else if(String(filepath).endsWith(".png")) {
    httpd_resp_set_type(req, "image/png");
  } else {
    httpd_resp_set_type(req, "application/octet-stream");
  }
  
  // Enviar el archivo en chunks
  size_t bytesRead;
  while((bytesRead = file.read(tempBuffer, BUFFER_SIZE)) > 0) {
    if(httpd_resp_send_chunk(req, (const char*)tempBuffer, bytesRead) != ESP_OK) {
      file.close();
      return ESP_FAIL;
    }
  }
  
  // Cerrar el archivo y enviar chunk vacío para finalizar
  file.close();
  httpd_resp_send_chunk(req, NULL, 0);
  return ESP_OK;
}

// Manejador para escanear redes WiFi
static esp_err_t wifi_scan_handler(httpd_req_t *req) {
  httpd_resp_set_type(req, "application/json");
  
  String networks = scanNetworks();
  return httpd_resp_sendstr(req, networks.c_str());
}

// Manejador para guardar configuración WiFi
static esp_err_t wifi_config_handler(httpd_req_t *req) {
  char* buf;
  size_t buf_len;
  char ssid[32] = {0,};
  char password[64] = {0,};
  
  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) {
    buf = (char*)malloc(buf_len);
    if(!buf) {
      httpd_resp_send_500(req);
      return ESP_FAIL;
    }
    
    if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
      if (httpd_query_key_value(buf, "ssid", ssid, sizeof(ssid)) == ESP_OK &&
          httpd_query_key_value(buf, "password", password, sizeof(password)) == ESP_OK) {
        
        // Actualizar configuración WiFi
        if (updateWiFiConfig(ssid, password)) {
          free(buf);
          
          // Responder con éxito y solicitar reinicio
          httpd_resp_set_type(req, "application/json");
          httpd_resp_sendstr(req, "{\"success\":true,\"message\":\"Configuración WiFi guardada. El dispositivo se reiniciará para aplicar los cambios.\"}");
          
          // Programar reinicio después de enviar la respuesta
          delay(2000);
          ESP.restart();
          
          return ESP_OK;
        } else {
          free(buf);
          httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to save WiFi configuration");
          return ESP_FAIL;
        }
      }
    }
    
    free(buf);
  }
  
  // Si llegamos aquí, significa que faltan parámetros o hay un error en la solicitud
  httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Missing or invalid parameters");
  return ESP_FAIL;
}

// Manejador para actualizar archivos web (requiere autenticación)
static esp_err_t update_web_handler(httpd_req_t *req) {
  // Verificar autenticación
  if (!isAuthenticated(req)) {
    return ESP_OK; // Ya se envió respuesta de error 401
  }
  
  int content_len = req->content_len;
  if (content_len <= 0) {
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Empty content");
    return ESP_FAIL;
  }
  
  // Obtener el nombre del archivo a actualizar
  char*  buf;
  size_t buf_len;
  char filepath[64] = {0,};
  
  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) {
    buf = (char*)malloc(buf_len);
    if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
      if (httpd_query_key_value(buf, "file", filepath, sizeof(filepath)) != ESP_OK) {
        free(buf);
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "File parameter required");
        return ESP_FAIL;
      }
    }
    free(buf);
  } else {
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "File parameter required");
    return ESP_FAIL;
  }
  
  // Validar la ruta del archivo (debe estar en /web/)
  String path = String(filepath);
  if (!path.startsWith(WEB_DIR)) {
    path = String(WEB_DIR) + "/" + path;
  }
  
  // Abrir archivo para escritura
  File file = SD_MMC.open(path.c_str(), FILE_WRITE);
  if (!file) {
    httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to open file for writing");
    return ESP_FAIL;
  }
  
  // Leer y escribir el contenido en chunks
  int received = 0;
  int remaining = content_len;
  while (remaining > 0) {
    int toRead = remaining < BUFFER_SIZE ? remaining : BUFFER_SIZE;
    int read_len = httpd_req_recv(req, (char*)tempBuffer, toRead);
    
    if (read_len <= 0) {
      if (read_len == HTTPD_SOCK_ERR_TIMEOUT) {
        continue; // Reintentar en caso de timeout
      }
      httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to receive data");
      file.close();
      return ESP_FAIL;
    }
    
    // Escribir el chunk recibido al archivo
    if (file.write(tempBuffer, read_len) != read_len) {
      httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to write file");
      file.close();
      return ESP_FAIL;
    }
    
    received += read_len;
    remaining -= read_len;
  }
  
  file.close();
  
  // Responder con éxito
  httpd_resp_set_type(req, "application/json");
  String response = "{\"success\":true,\"file\":\"" + path + "\"}";
  httpd_resp_sendstr(req, response.c_str());
  return ESP_OK;
}

// Manejador para actualización OTA (requiere autenticación)
static esp_err_t ota_update_handler(httpd_req_t *req) {
  // Verificar autenticación
  if (!isAuthenticated(req)) {
    return ESP_OK; // Ya se envió respuesta de error 401
  }
  
  int content_len = req->content_len;
  if (content_len <= 0) {
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Empty firmware");
    return ESP_FAIL;
  }
  
  // Preparar para la actualización OTA
  if (!Update.begin(content_len)) {
    httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "OTA begin failed");
    return ESP_FAIL;
  }
  
  // Recibir y escribir el firmware en chunks
  int received = 0;
  int remaining = content_len;
  while (remaining > 0) {
    int toRead = remaining < BUFFER_SIZE ? remaining : BUFFER_SIZE;
    int read_len = httpd_req_recv(req, (char*)tempBuffer, toRead);
    
    if (read_len <= 0) {
      if (read_len == HTTPD_SOCK_ERR_TIMEOUT) {
        continue; // Reintentar en caso de timeout
      }
      httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to receive firmware");
      return ESP_FAIL;
    }
    
    // Escribir el chunk al sistema OTA
    if (Update.write(tempBuffer, read_len) != read_len) {
      httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to write firmware");
      return ESP_FAIL;
    }
    
    received += read_len;
    remaining -= read_len;
  }
  
  if (!Update.end(true)) {
    httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "OTA end failed");
    return ESP_FAIL;
  }
  
  // Responder con éxito
  httpd_resp_set_type(req, "application/json");
  httpd_resp_sendstr(req, "{\"success\":true,\"message\":\"Firmware actualizado. Reiniciando...\"}");
  
  // Programar reinicio después de enviar la respuesta
  delay(1000);
  ESP.restart();
  
  return ESP_OK;
}

// Manejador HTTP para página de prueba simple
static esp_err_t test_handler(httpd_req_t *req) {
  const char* test_html = "<!DOCTYPE html><html><head><title>Test ESP32-CAM</title></head><body>"
                          "<h1>Prueba de Streaming</h1>"
                          "<img src=\"http://192.168.4.1:81/stream\" width=\"640\" height=\"480\">"
                          "</body></html>";
  
  httpd_resp_set_type(req, "text/html");
  httpd_resp_send(req, test_html, strlen(test_html));
  return ESP_OK;
}


// Inicializar servidores HTTP
void startCameraServer() {
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = WEB_PORT;
  config.ctrl_port = WEB_PORT;
  config.max_uri_handlers = 15;
  config.max_resp_headers = 8;
  config.stack_size = 10240;
  
  // Servidor para páginas web y comandos
  Serial.printf("Iniciando servidor web en puerto: %d\n", config.server_port);
  if (httpd_start(&camera_httpd, &config) == ESP_OK) {
    // Ruta principal (portal cautivo)
    httpd_uri_t index_uri = {
      .uri       = "/",
      .method    = HTTP_GET,
      .handler   = index_handler,
      .user_ctx  = NULL
    };
    httpd_register_uri_handler(camera_httpd, &index_uri);
    


// Página de prueba simple
httpd_uri_t test_uri = {
  .uri       = "/test",
  .method    = HTTP_GET,
  .handler   = test_handler,
  .user_ctx  = NULL
};
httpd_register_uri_handler(camera_httpd, &test_uri);


    // Capturar foto
    httpd_uri_t capture_uri = {
      .uri       = "/capture",
      .method    = HTTP_GET,
      .handler   = capture_handler,
      .user_ctx  = NULL
    };
    httpd_register_uri_handler(camera_httpd, &capture_uri);
    
    // Comandos de control de cámara
    httpd_uri_t cmd_uri = {
      .uri       = "/control",
      .method    = HTTP_GET,
      .handler   = cmd_handler,
      .user_ctx  = NULL
    };
    httpd_register_uri_handler(camera_httpd, &cmd_uri);
    
    // Listar fotos
    httpd_uri_t list_uri = {
      .uri       = "/list",
      .method    = HTTP_GET,
      .handler   = list_handler,
      .user_ctx  = NULL
    };
    httpd_register_uri_handler(camera_httpd, &list_uri);
    
    // Servir archivos de la SD desde carpeta photos
    httpd_uri_t photos_uri = {
      .uri       = "/photos/*",
      .method    = HTTP_GET,
      .handler   = sd_file_handler,
      .user_ctx  = NULL
    };
    httpd_register_uri_handler(camera_httpd, &photos_uri);
    
    // Servir CSS
    httpd_uri_t css_uri = {
      .uri       = "/styles.css",
      .method    = HTTP_GET,
      .handler   = css_handler,
      .user_ctx  = NULL
    };
    httpd_register_uri_handler(camera_httpd, &css_uri);
    
    // Servir JavaScript
    httpd_uri_t js_uri = {
      .uri       = "/script.js",
      .method    = HTTP_GET,
      .handler   = js_handler,
      .user_ctx  = NULL
    };
    httpd_register_uri_handler(camera_httpd, &js_uri);
    
    // Área administrativa
    httpd_uri_t admin_uri = {
      .uri       = "/admin",
      .method    = HTTP_GET,
      .handler   = admin_handler,
      .user_ctx  = NULL
    };
    httpd_register_uri_handler(camera_httpd, &admin_uri);
    
    // Configuración WiFi
    httpd_uri_t wifi_uri = {
      .uri       = "/wifi",
      .method    = HTTP_GET,
      .handler   = wifi_handler,
      .user_ctx  = NULL
    };
    httpd_register_uri_handler(camera_httpd, &wifi_uri);
    
    // Escanear redes WiFi
    httpd_uri_t wifi_scan_uri = {
      .uri       = "/scan-wifi",
      .method    = HTTP_GET,
      .handler   = wifi_scan_handler,
      .user_ctx  = NULL
    };
    httpd_register_uri_handler(camera_httpd, &wifi_scan_uri);
    
    // Configurar WiFi
    httpd_uri_t wifi_config_uri = {
      .uri       = "/config-wifi",
      .method    = HTTP_GET,
      .handler   = wifi_config_handler,
      .user_ctx  = NULL
    };
    httpd_register_uri_handler(camera_httpd, &wifi_config_uri);
    
    // Actualizar archivos web
    httpd_uri_t update_web_uri = {
      .uri       = "/update-web",
      .method    = HTTP_POST,
      .handler   = update_web_handler,
      .user_ctx  = NULL
    };
    httpd_register_uri_handler(camera_httpd, &update_web_uri);
    
    // Actualización OTA
    httpd_uri_t ota_uri = {
      .uri       = "/update-ota",
      .method    = HTTP_POST,
      .handler   = ota_update_handler,
      .user_ctx  = NULL
    };
    httpd_register_uri_handler(camera_httpd, &ota_uri);
  }
  
  config.server_port += 1;
  config.ctrl_port += 1;
  
  // Servidor para streaming de video
  Serial.printf("Iniciando servidor de streaming en puerto: %d\n", config.server_port);
  if (httpd_start(&stream_httpd, &config) == ESP_OK) {
    httpd_uri_t stream_uri = {
      .uri       = "/stream",
      .method    = HTTP_GET,
      .handler   = stream_handler,
      .user_ctx  = NULL
    };
    httpd_register_uri_handler(stream_httpd, &stream_uri);
  }
}