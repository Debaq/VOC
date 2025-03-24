// wifi.cpp - Implementación de funciones WiFi y DNS

#include "wifi.h"

// Inicializar punto de acceso WiFi
void initWiFi() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(AP_IP, AP_IP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(WIFI_SSID);
  
  // Iniciar servidor DNS para redireccionar todas las peticiones
  dnsServer.start(53, "*", AP_IP);
  
  Serial.print("Punto de acceso iniciado con SSID: ");
  Serial.println(WIFI_SSID);
  Serial.print("Dirección IP: ");
  Serial.println(AP_IP);
}