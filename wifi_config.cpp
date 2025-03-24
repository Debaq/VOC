// wifi_config.cpp - Implementación de funciones de configuración WiFi

#include "wifi_config.h"

// Definición de variables globales
my_wifi_config_t wifi_config;
bool wifi_connected = false;

// Inicializar configuración WiFi (cargar desde SD)
bool initWiFiConfig() {
  // Valores por defecto en caso de que no exista configuración
  memset(wifi_config.ssid, 0, sizeof(wifi_config.ssid));
  memset(wifi_config.password, 0, sizeof(wifi_config.password));
  wifi_config.use_static_ip = false;
  wifi_config.static_ip = IPAddress(0, 0, 0, 0);
  wifi_config.gateway = IPAddress(0, 0, 0, 0);
  wifi_config.subnet = IPAddress(255, 255, 255, 0);
  wifi_config.dns1 = IPAddress(8, 8, 8, 8);
  wifi_config.dns2 = IPAddress(8, 8, 4, 4);
  
  if (!hasSD) {
    Serial.println("No hay tarjeta SD disponible para configuración WiFi");
    return false;
  }
  
  // Crear directorio config si no existe
  if (!SD_MMC.exists("/config")) {
    SD_MMC.mkdir("/config");
  }
  
  // Comprobar si existe el archivo de configuración
  if (!SD_MMC.exists(WIFI_CONFIG_FILE)) {
    Serial.println("Archivo de configuración WiFi no encontrado, usando valores predeterminados");
    // Crear archivo de configuración con valores predeterminados
    return saveWiFiConfig();
  }
  
  // Leer configuración desde archivo
  File configFile = SD_MMC.open(WIFI_CONFIG_FILE, FILE_READ);
  if (!configFile) {
    Serial.println("Error al abrir archivo de configuración WiFi");
    return false;
  }
  
  // Reservar memoria para documento JSON
  StaticJsonDocument<512> doc;
  
  // Deserializar JSON
  DeserializationError error = deserializeJson(doc, configFile);
  configFile.close();
  
  if (error) {
    Serial.print("Error al leer configuración WiFi: ");
    Serial.println(error.c_str());
    return false;
  }
  
  // Copiar valores a la estructura de configuración
  strlcpy(wifi_config.ssid, doc["ssid"] | "", sizeof(wifi_config.ssid));
  strlcpy(wifi_config.password, doc["password"] | "", sizeof(wifi_config.password));
  wifi_config.use_static_ip = doc["use_static_ip"] | false;
  
  if (wifi_config.use_static_ip) {
    String ip = doc["static_ip"] | "0.0.0.0";
    String gw = doc["gateway"] | "0.0.0.0";
    String sn = doc["subnet"] | "255.255.255.0";
    String dns1 = doc["dns1"] | "8.8.8.8";
    String dns2 = doc["dns2"] | "8.8.4.4";
    
    wifi_config.static_ip.fromString(ip);
    wifi_config.gateway.fromString(gw);
    wifi_config.subnet.fromString(sn);
    wifi_config.dns1.fromString(dns1);
    wifi_config.dns2.fromString(dns2);
  }
  
  Serial.println("Configuración WiFi cargada correctamente");
  Serial.print("SSID: ");
  Serial.println(wifi_config.ssid);
  
  return true;
}

// Guardar configuración WiFi en SD
bool saveWiFiConfig() {
  if (!hasSD) {
    Serial.println("No hay tarjeta SD disponible para guardar configuración WiFi");
    return false;
  }
  
  // Crear documento JSON
  StaticJsonDocument<512> doc;
  
  // Agregar valores a JSON
  doc["ssid"] = wifi_config.ssid;
  doc["password"] = wifi_config.password;
  doc["use_static_ip"] = wifi_config.use_static_ip;
  
  if (wifi_config.use_static_ip) {
    doc["static_ip"] = wifi_config.static_ip.toString();
    doc["gateway"] = wifi_config.gateway.toString();
    doc["subnet"] = wifi_config.subnet.toString();
    doc["dns1"] = wifi_config.dns1.toString();
    doc["dns2"] = wifi_config.dns2.toString();
  }
  
  // Abrir archivo para escritura
  File configFile = SD_MMC.open(WIFI_CONFIG_FILE, FILE_WRITE);
  if (!configFile) {
    Serial.println("Error al abrir archivo de configuración WiFi para escritura");
    return false;
  }
  
  // Serializar JSON a archivo
  if (serializeJson(doc, configFile) == 0) {
    Serial.println("Error al escribir configuración WiFi");
    configFile.close();
    return false;
  }
  
  configFile.close();
  Serial.println("Configuración WiFi guardada correctamente");
  return true;
}

// Actualizar configuración WiFi con nuevos valores
bool updateWiFiConfig(const char* ssid, const char* password, bool use_static_ip,
                     IPAddress static_ip, IPAddress gateway, IPAddress subnet,
                     IPAddress dns1, IPAddress dns2) {
  // Actualizar estructura de configuración
  strlcpy(wifi_config.ssid, ssid, sizeof(wifi_config.ssid));
  strlcpy(wifi_config.password, password, sizeof(wifi_config.password));
  wifi_config.use_static_ip = use_static_ip;
  
  if (use_static_ip) {
    wifi_config.static_ip = static_ip;
    wifi_config.gateway = gateway;
    wifi_config.subnet = subnet;
    wifi_config.dns1 = dns1;
    wifi_config.dns2 = dns2;
  }
  
  // Guardar configuración actualizada
  return saveWiFiConfig();
}

// Escanear redes WiFi disponibles y devolver como JSON
String scanNetworks() {
  Serial.println("Escaneando redes WiFi...");
  
  // Documento JSON para almacenar información de redes
  StaticJsonDocument<2048> doc;
  JsonArray networks = doc.createNestedArray("networks");
  
  // Escanear redes
  int numNetworks = WiFi.scanNetworks();
  
  if (numNetworks == 0) {
    Serial.println("No se encontraron redes WiFi");
  } else {
    Serial.print("Se encontraron ");
    Serial.print(numNetworks);
    Serial.println(" redes:");
    
    // Ordenar redes por intensidad de señal
    int indices[numNetworks];
    for (int i = 0; i < numNetworks; i++) {
      indices[i] = i;
    }
    
    // Ordenar por RSSI (mayor a menor)
    for (int i = 0; i < numNetworks; i++) {
      for (int j = i + 1; j < numNetworks; j++) {
        if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
          std::swap(indices[i], indices[j]);
        }
      }
    }
    
    // Agregar redes al array JSON
    for (int i = 0; i < numNetworks; i++) {
      JsonObject network = networks.createNestedObject();
      network["ssid"] = WiFi.SSID(indices[i]);
      network["rssi"] = WiFi.RSSI(indices[i]);
      network["encryption"] = (WiFi.encryptionType(indices[i]) == WIFI_AUTH_OPEN) ? false : true;
      
      // Comprobar si es la red actualmente configurada
      if (strcmp(wifi_config.ssid, WiFi.SSID(indices[i]).c_str()) == 0) {
        network["configured"] = true;
      } else {
        network["configured"] = false;
      }
    }
  }
  
  // Liberar memoria del escaneo
  WiFi.scanDelete();
  
  // Serializar y devolver como String
  String result;
  serializeJson(doc, result);
  return result;
}