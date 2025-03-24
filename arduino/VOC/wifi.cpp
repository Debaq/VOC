// wifi.cpp - Implementación de funciones WiFi y DNS

#include "wifi.h"
#include "oled.h"

// Variables globales
my_wifi_config_t my_wifi_config;
bool isAPMode = true;
bool wifi_connected = false;

// Inicializar WiFi (intentar conexión primero, AP como respaldo)
bool initWiFi() {
  // Cargar configuración WiFi
  if (!loadWiFiConfig()) {
    Serial.println("Error al cargar configuración WiFi, iniciando modo AP");
    startAPMode();
    return false;
  }
  
  // Si hay credenciales WiFi, intentar conectar
  if (strlen(my_wifi_config.ssid) > 0) {
    Serial.printf("Intentando conectar a WiFi: %s\n", my_wifi_config.ssid);
    
    if (connectToWiFi()) {
      Serial.println("Conexión WiFi establecida");
      isAPMode = false;
      wifi_connected = true;
      
      // Mostrar IP en pantalla OLED
      displayIP(WiFi.localIP(), my_wifi_config.ssid);
      return true;
    } else {
      Serial.println("Fallo en la conexión WiFi, iniciando modo AP");
    }
  } else {
    Serial.println("No hay credenciales WiFi almacenadas, iniciando modo AP");
  }
  
  // Si la conexión falla o no hay credenciales, iniciar modo AP
  startAPMode();
  return false;
}

// Inicializar punto de acceso WiFi
void startAPMode() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(AP_IP, AP_IP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(WIFI_SSID);
  
  // Iniciar servidor DNS para redireccionar todas las peticiones
  dnsServer.start(53, "*", AP_IP);
  
  Serial.print("Punto de acceso iniciado con SSID: ");
  Serial.println(WIFI_SSID);
  Serial.print("Dirección IP: ");
  Serial.println(AP_IP);
  
  isAPMode = true;
  wifi_connected = false;
  
  // Mostrar información en la pantalla OLED
  displayAPMode(AP_IP, WIFI_SSID);
}

// Conectar a WiFi usando las credenciales almacenadas
bool connectToWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(my_wifi_config.ssid, my_wifi_config.password);
  
  // Configurar IP estática si está habilitada
  if (my_wifi_config.use_static_ip) {
    WiFi.config(my_wifi_config.static_ip, my_wifi_config.gateway, 
               my_wifi_config.subnet, my_wifi_config.dns1);
  }
  
  // Mostrar "Conectando..." en la pantalla OLED
  displayText("Conectando a:\n" + String(my_wifi_config.ssid), 1, true);
  
  // Esperar conexión con timeout
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
    // Verificar timeout
    if (millis() - startTime > WIFI_CONNECTION_TIMEOUT) {
      Serial.println("\nTimeout al conectar a WiFi");
      return false;
    }
  }
  
  Serial.println();
  Serial.print("Conectado a WiFi, IP: ");
  Serial.println(WiFi.localIP());
  
  return true;
}

// Cargar configuración WiFi
bool loadWiFiConfig() {
  // Valores por defecto en caso de que no exista configuración
  memset(my_wifi_config.ssid, 0, sizeof(my_wifi_config.ssid));
  memset(my_wifi_config.password, 0, sizeof(my_wifi_config.password));
  my_wifi_config.use_static_ip = false;
  my_wifi_config.static_ip = IPAddress(0, 0, 0, 0);
  my_wifi_config.gateway = IPAddress(0, 0, 0, 0);
  my_wifi_config.subnet = IPAddress(255, 255, 255, 0);
  my_wifi_config.dns1 = IPAddress(8, 8, 8, 8);
  my_wifi_config.dns2 = IPAddress(8, 8, 4, 4);
  
  if (!hasSD) {
    Serial.println("No hay tarjeta SD disponible para configuración WiFi");
    return false;
  }
  
  // Crear directorio config si no existe
  if (!SD_MMC.exists(CONFIG_DIR)) {
    SD_MMC.mkdir(CONFIG_DIR);
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
  strlcpy(my_wifi_config.ssid, doc["ssid"] | "", sizeof(my_wifi_config.ssid));
  strlcpy(my_wifi_config.password, doc["password"] | "", sizeof(my_wifi_config.password));
  my_wifi_config.use_static_ip = doc["use_static_ip"] | false;
  
  if (my_wifi_config.use_static_ip) {
    String ip = doc["static_ip"] | "0.0.0.0";
    String gw = doc["gateway"] | "0.0.0.0";
    String sn = doc["subnet"] | "255.255.255.0";
    String dns1 = doc["dns1"] | "8.8.8.8";
    String dns2 = doc["dns2"] | "8.8.4.4";
    
    my_wifi_config.static_ip.fromString(ip);
    my_wifi_config.gateway.fromString(gw);
    my_wifi_config.subnet.fromString(sn);
    my_wifi_config.dns1.fromString(dns1);
    my_wifi_config.dns2.fromString(dns2);
  }
  
  Serial.println("Configuración WiFi cargada correctamente");
  if (strlen(my_wifi_config.ssid) > 0) {
    Serial.print("SSID: ");
    Serial.println(my_wifi_config.ssid);
  } else {
    Serial.println("No hay SSID configurado");
  }
  
  return true;
}

// Guardar configuración WiFi
bool saveWiFiConfig() {
  if (!hasSD) {
    Serial.println("No hay tarjeta SD disponible para guardar configuración WiFi");
    return false;
  }
  
  // Crear directorio config si no existe
  if (!SD_MMC.exists(CONFIG_DIR)) {
    SD_MMC.mkdir(CONFIG_DIR);
  }
  
  // Crear documento JSON
  StaticJsonDocument<512> doc;
  
  // Agregar valores a JSON
  doc["ssid"] = my_wifi_config.ssid;
  doc["password"] = my_wifi_config.password;
  doc["use_static_ip"] = my_wifi_config.use_static_ip;
  
  if (my_wifi_config.use_static_ip) {
    doc["static_ip"] = my_wifi_config.static_ip.toString();
    doc["gateway"] = my_wifi_config.gateway.toString();
    doc["subnet"] = my_wifi_config.subnet.toString();
    doc["dns1"] = my_wifi_config.dns1.toString();
    doc["dns2"] = my_wifi_config.dns2.toString();
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

// Actualizar configuración WiFi
bool updateWiFiConfig(const char* ssid, const char* password, bool use_static_ip,
                     IPAddress static_ip, IPAddress gateway, IPAddress subnet,
                     IPAddress dns1, IPAddress dns2) {
  // Actualizar estructura de configuración
  strlcpy(my_wifi_config.ssid, ssid, sizeof(my_wifi_config.ssid));
  strlcpy(my_wifi_config.password, password, sizeof(my_wifi_config.password));
  my_wifi_config.use_static_ip = use_static_ip;
  
  if (use_static_ip) {
    my_wifi_config.static_ip = static_ip;
    my_wifi_config.gateway = gateway;
    my_wifi_config.subnet = subnet;
    my_wifi_config.dns1 = dns1;
    my_wifi_config.dns2 = dns2;
  }
  
  // Guardar configuración actualizada
  return saveWiFiConfig();
}

// Escanear redes WiFi disponibles
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
      if (strcmp(my_wifi_config.ssid, WiFi.SSID(indices[i]).c_str()) == 0) {
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