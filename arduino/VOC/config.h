// config.h - Archivo de configuración global para ESP32-CAM

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <WiFi.h>
#include "SD_MMC.h"
#include "esp_camera.h"
#include "esp_http_server.h"  // Añadido para httpd_handle_t
#include <DNSServer.h>        // Añadido para DNSServer
#include <ArduinoJson.h>      // Añadido para manejo de JSON

// Configuración de la red WiFi
#define WIFI_SSID "VOC"                // Nombre de la red WiFi
#define AP_IP IPAddress(192, 168, 4, 1) // Dirección IP del punto de acceso
#define WEB_PORT 80                    // Puerto para el servidor web
#define WIFI_CONNECTION_TIMEOUT 10000   // Tiempo de espera para conexión WiFi (ms)

// Configuración de la pantalla OLED
#define OLED_SDA 14                    // Pin SDA para I2C (cambiar según conexión)
#define OLED_SCL 15                    // Pin SCL para I2C (cambiar según conexión)
#define OLED_ADDRESS 0x3C              // Dirección I2C de la pantalla OLED
#define OLED_RESET -1                  // Pin de reset para OLED (normalmente -1)
#define SCREEN_WIDTH 128               // Ancho de la pantalla OLED en píxeles
#define SCREEN_HEIGHT 32               // Alto de la pantalla OLED en píxeles (32 para 0.89")

// Credenciales para el área administrativa
#define ADMIN_USER "admin"
#define ADMIN_PASSWORD "admin123"      // Cámbialo por una contraseña segura

// Definición de pines para la cámara AI-THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// Rutas de archivos en la SD
#define HTML_INDEX "/web/index.html"
#define HTML_ADMIN "/web/admin.html"
#define HTML_WIFI "/web/wifi.html"
#define CSS_MAIN "/web/styles.css"
#define JS_MAIN "/web/script.js"
#define HTML_FALLBACK "/web/fallback.html"
#define PHOTOS_DIR "/photos"
#define WEB_DIR "/web"
#define CONFIG_DIR "/config"
#define WIFI_CONFIG_FILE "/config/wifi_config.json"

// Buffer para almacenar archivos temporales
#define BUFFER_SIZE 4096

// Variables globales (declaradas como extern para usarlas en múltiples archivos)
extern bool hasSD;
extern size_t photoCount;
extern String lastPhotoFilename;
extern httpd_handle_t camera_httpd;
extern httpd_handle_t stream_httpd;
extern DNSServer dnsServer;
extern uint8_t tempBuffer[BUFFER_SIZE];
extern bool isAPMode;

// Estructura para almacenar configuración WiFi
typedef struct {
  char ssid[32];
  char password[64];
  bool use_static_ip;
  IPAddress static_ip;
  IPAddress gateway;
  IPAddress subnet;
  IPAddress dns1;
  IPAddress dns2;
} my_wifi_config_t;  // Cambiado a my_wifi_config_t para evitar conflictos

// Variable para la configuración WiFi
extern my_wifi_config_t my_wifi_config;  // Cambiado a my_wifi_config

// Estructura para respuesta HTTP básica
typedef struct {
  httpd_req_t *req;
  size_t len;
} jpg_chunking_t;

#endif // CONFIG_H