// web_server.h - Servidor web y manejadores HTTP

#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "config.h"
#include "esp_http_server.h"
#include "esp_timer.h"
#include "auth.h"
#include "camera.h"
#include "sd_card.h"
#include "wifi.h"
#include <Update.h>
#include <ESPmDNS.h>

// Inicializar servidores HTTP
void startCameraServer();

// Manejadores HTTP
static esp_err_t index_handler(httpd_req_t *req);
static esp_err_t stream_handler(httpd_req_t *req);
static esp_err_t cmd_handler(httpd_req_t *req);
static esp_err_t capture_handler(httpd_req_t *req);
static esp_err_t list_handler(httpd_req_t *req);
static esp_err_t sd_file_handler(httpd_req_t *req);
static esp_err_t css_handler(httpd_req_t *req);
static esp_err_t js_handler(httpd_req_t *req);
static esp_err_t admin_handler(httpd_req_t *req);
static esp_err_t wifi_handler(httpd_req_t *req);
static esp_err_t update_web_handler(httpd_req_t *req);
static esp_err_t ota_update_handler(httpd_req_t *req);
static esp_err_t wifi_scan_handler(httpd_req_t *req);
static esp_err_t wifi_config_handler(httpd_req_t *req);

#endif // WEB_SERVER_H