// wifi_config.h - Configuración y almacenamiento de credenciales WiFi

#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include "config.h"
#include "sd_card.h"
#include <ArduinoJson.h>

// Ruta del archivo de configuración WiFi en la SD
#define WIFI_CONFIG_FILE "/config/wifi_config.json"

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
} my_wifi_config_t;  // Cambiado de wifi_config_t

// Variables globales
extern my_wifi_config_t wifi_config;  // Cambiado pero mantiene el mismo nombre de variable
extern bool wifi_connected;

// Inicializar configuración WiFi (cargar desde SD)
bool initWiFiConfig();

// Guardar configuración WiFi en SD
bool saveWiFiConfig();

// Actualizar configuración WiFi con nuevos valores
bool updateWiFiConfig(const char* ssid, const char* password, bool use_static_ip = false,
                     IPAddress static_ip = IPAddress(0,0,0,0), 
                     IPAddress gateway = IPAddress(0,0,0,0),
                     IPAddress subnet = IPAddress(255,255,255,0),
                     IPAddress dns1 = IPAddress(8,8,8,8),
                     IPAddress dns2 = IPAddress(8,8,4,4));

// Escanear redes WiFi disponibles
String scanNetworks();

#endif // WIFI_CONFIG_H