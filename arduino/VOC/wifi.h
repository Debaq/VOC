// wifi.h - Configuración del punto de acceso WiFi y conexión a red

#ifndef WIFI_H
#define WIFI_H

#include "config.h"
#include <WiFi.h>
#include <DNSServer.h>
#include <ArduinoJson.h>
#include "sd_card.h"

// Inicializar WiFi (intentar conexión primero, AP como respaldo)
bool initWiFi();

// Inicializar punto de acceso WiFi
void startAPMode();

// Conectar a WiFi usando las credenciales almacenadas
bool connectToWiFi();

// Cargar configuración WiFi
bool loadWiFiConfig();

// Guardar configuración WiFi
bool saveWiFiConfig();

// Actualizar configuración WiFi
bool updateWiFiConfig(const char* ssid, const char* password, bool use_static_ip = false,
                     IPAddress static_ip = IPAddress(0,0,0,0),
                     IPAddress gateway = IPAddress(0,0,0,0),
                     IPAddress subnet = IPAddress(255,255,255,0),
                     IPAddress dns1 = IPAddress(8,8,8,8),
                     IPAddress dns2 = IPAddress(8,8,4,4));

// Escanear redes WiFi disponibles
String scanNetworks();

// Variables globales externas
extern bool wifi_connected;

#endif // WIFI_H