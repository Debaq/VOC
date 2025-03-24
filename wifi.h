// wifi.h - Configuración del punto de acceso WiFi y DNS

#ifndef WIFI_H
#define WIFI_H

#include "config.h"
#include <WiFi.h>
#include <DNSServer.h>

// No se usa AsyncWebServer sino el servidor estándar ESP32
// Inicializar punto de acceso WiFi
void initWiFi();

// La clase CaptiveRequestHandler se debe reemplazar por la implementación
// nativa usando DNSServer

#endif // WIFI_H