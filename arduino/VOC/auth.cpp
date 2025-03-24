// auth.cpp - Implementación de funciones de autenticación

#include "auth.h"

// Función simple para codificar en base64 (para autenticación)
String base64Encode(String input) {
  const char* base64chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  String encoded = "";
  int i = 0;
  int j = 0;
  uint8_t char_array_3[3];
  uint8_t char_array_4[4];
  
  while (i < input.length()) {
    char_array_3[j++] = input.charAt(i++);
    if (j == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;
      
      for(j = 0; j < 4; j++)
        encoded += base64chars[char_array_4[j]];
      j = 0;
    }
  }
  
  if (j) {
    for(i = j; i < 3; i++)
      char_array_3[i] = '\0';
    
    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;
    
    for (i = 0; i < j + 1; i++)
      encoded += base64chars[char_array_4[i]];
    
    while((j++ < 3))
      encoded += '=';
  }
  
  return encoded;
}

// Función auxiliar para verificar credenciales de admin
bool isAuthenticated(httpd_req_t *req) {
  char*  buf;
  size_t buf_len;
  
  // Obtener encabezado de autorización
  buf_len = httpd_req_get_hdr_value_len(req, "Authorization") + 1;
  if (buf_len > 1) {
    buf = (char*)malloc(buf_len);
    if (httpd_req_get_hdr_value_str(req, "Authorization", buf, buf_len) == ESP_OK) {
      // Verificar si es Basic Auth
      if (strncmp(buf, "Basic ", 6) == 0) {
        // Decodificar base64 (implementación simple)
        String auth = String(buf + 6);
        String expected = base64Encode(String(ADMIN_USER) + ":" + String(ADMIN_PASSWORD));
        free(buf);
        return (auth.equals(expected));
      }
    }
    free(buf);
  }
  
  // Si no hay encabezado o no coincide, solicitar autenticación
  httpd_resp_set_status(req, "401 Unauthorized");
  httpd_resp_set_type(req, "text/html");
  httpd_resp_set_hdr(req, "WWW-Authenticate", "Basic realm=\"ESP32-CAM Admin Area\"");
  httpd_resp_sendstr(req, "Unauthorized");
  return false;
}