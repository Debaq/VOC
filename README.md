# Otoscopio Educativo con ESP32-CAM (VOC)

![Otoscopio Educativo](https://i.imgur.com/placeholder-otoscopio.jpg)

## Visión General

El Otoscopio Educativo VOC (Visual Otoscopic Camera) es un dispositivo médico educativo que combina un otoscopio tradicional con tecnología de cámara digital basada en ESP32-CAM, permitiendo que tanto el evaluador como el docente visualicen simultáneamente el canal auditivo y tímpano del paciente durante las prácticas clínicas.

### Características Principales

- **Visualización Dual:** Sistema óptico con prisma y espejo 50/50 que permite la visualización simultánea directa y por cámara.
- **Transmisión en Tiempo Real:** Streaming de video en vivo a través de WiFi.
- **Captura de Imágenes:** Capacidad para tomar y almacenar fotografías de alta resolución.
- **Interfaz Web Responsiva:** Accesible desde cualquier dispositivo con navegador web.
- **Portal Cautivo:** Conexión WiFi directa sin necesidad de configuración adicional.
- **Almacenamiento en SD:** Guarda imágenes para su revisión posterior o documentación clínica.
- **Administración Web:** Interfaz para gestionar configuraciones y actualizar el firmware.

## Principio de Funcionamiento

![Diagrama de Funcionamiento](https://i.imgur.com/placeholder-diagrama.jpg)

El sistema utiliza un prisma con espejo 50/50 que divide la luz proveniente del canal auditivo:
- 50% de la luz se dirige al ocular para la visualización directa por el evaluador.
- El otro 50% se dirige hacia la cámara ESP32-CAM, que transmite la imagen en tiempo real a través de WiFi.

Esta configuración óptica permite que tanto el estudiante (evaluador) como el profesor puedan observar simultáneamente la misma estructura anatómica, facilitando la enseñanza práctica de técnicas de exploración otoscópica.

## Componentes del Hardware

- **ESP32-CAM:** Módulo con microcontrolador ESP32 y cámara OV2640.
- **Prisma con Espejo 50/50:** Componente óptico que divide la luz.
- **Carcasa de Otoscopio:** Adaptada para integrar el sistema óptico y electrónico.
- **Tarjeta MicroSD:** Para almacenamiento de imágenes capturadas.
- **Batería LiPo:** Alimentación portátil del dispositivo.
- **LED de Iluminación:** Proporciona luz para la exploración.
- **Conos Desechables:** Compatibles con estándares médicos.

## Arquitectura del Software

El sistema está desarrollado con las siguientes capacidades:

```
ESP32-CAM Portal (VOC)
│
├── Módulo de Cámara
│   ├── Inicialización y configuración
│   ├── Captura de imágenes
│   └── Streaming de video
│
├── Sistema de Archivos (SD)
│   ├── Almacenamiento de imágenes
│   ├── Gestión de archivos web
│   └── Configuración del sistema
│
├── Conectividad WiFi
│   ├── Punto de acceso
│   └── Servidor DNS (portal cautivo)
│
└── Servidor Web
    ├── Interfaz de usuario
    │   ├── Visualización en tiempo real
    │   ├── Galería de imágenes
    │   └── Controles de cámara
    │
    └── Área administrativa
        ├── Edición de archivos web
        └── Actualización OTA
```

## Instrucciones de Uso

### Paso 1: Encendido y Conexión

1. Encienda el dispositivo mediante el interruptor ubicado en la base.
2. Espere a que el LED indicador cambie a verde (aproximadamente 5 segundos).
3. En su dispositivo (teléfono, tablet o computadora), busque y conéctese a la red WiFi:
   - **SSID:** VOC
   - **Contraseña:** No requiere (red abierta)
4. Se abrirá automáticamente el portal web o navegue a la dirección: `http://192.168.4.1`

### Paso 2: Interfaz de Usuario

![Interfaz de Usuario](https://i.imgur.com/placeholder-interfaz.jpg)

La interfaz presenta tres secciones principales:

- **Cámara:** Visualización en tiempo real con controles para tomar fotos y ajustar parámetros.
- **Galería:** Acceso a imágenes capturadas previamente.
- **Admin:** Área para configuraciones avanzadas (requiere autenticación).

### Paso 3: Exploración Otoscópica

1. Coloque un cono desechable en el extremo del otoscopio.
2. Proceda con la técnica estándar de exploración otoscópica:
   - Tracción del pabellón auricular hacia arriba y atrás (adultos).
   - Tracción del pabellón auricular hacia abajo y atrás (niños).
3. El evaluador observa directamente a través del ocular.
4. El docente y demás estudiantes visualizan en tiempo real la misma imagen a través de sus dispositivos.
5. Capture imágenes según sea necesario con el botón "Tomar Foto".

### Área Administrativa

Para acceder al área administrativa:
- **Usuario:** admin
- **Contraseña:** admin123

En esta sección puede:
- Editar archivos de la interfaz web
- Actualizar el firmware (OTA)
- Modificar configuraciones avanzadas

## Especificaciones Técnicas

- **Resolución Máxima:** 1600x1200 (UXGA)
- **Alcance WiFi:** ~30 metros (línea de vista)
- **Formato de Imagen:** JPEG
- **Duración de Batería:** ~3 horas de uso continuo
- **Capacidad de Almacenamiento:** Dependiente de la tarjeta SD (máximo 32GB)
- **Dimensiones:** 140mm x 30mm (largo x diámetro)
- **Peso:** 180g (incluida batería)

## Diagrama de Bloques del Sistema

```
┌──────────────────┐      ┌───────────────┐
│                  │      │               │
│  Prisma 50/50    │◄────►│  Canal        │
│                  │      │  Auditivo     │
└───────┬──────────┘      └───────────────┘
        │
        ├─────────────┐
        │             │
┌───────▼──────┐      │
│              │      │
│  ESP32-CAM   │      │
│              │      │
└───────┬──────┘      │
        │             │
┌───────▼──────┐      │
│              │      │
│  Servidor    │      │
│  WiFi        │      │
│              │      │
└───────┬──────┘      │
        │             │
        │       ┌─────▼─────┐
┌───────▼──────┐│           │
│              ││  Visor    │
│  Dispositivo ││  Directo  │
│  Cliente     ││           │
│              ││           │
└──────────────┘└───────────┘
```

## Instalación y Configuración

### Requisitos

- Arduino IDE (1.8.x o superior)
- Paquete ESP32 para Arduino
- Bibliotecas adicionales:
  - ESP32 Camera
  - ArduinoJson
  - Update

### Compilación e Instalación

1. Clone el repositorio o descargue los archivos fuente.
2. Abra el archivo `VOC.ino` en Arduino IDE.
3. Seleccione la placa "ESP32 Wrover Module" o "AI Thinker ESP32-CAM".
4. Configure los pines según la documentación.
5. Compile y cargue el firmware.

### Preparación de la Tarjeta SD

Antes de usar el dispositivo, formatee una tarjeta microSD (FAT32) y cree las siguientes carpetas:
- `/photos`
- `/web`

## Resolución de Problemas

| Problema | Posible Causa | Solución |
|----------|--------------|----------|
| No enciende el dispositivo | Batería descargada | Recargue la batería |
| No se conecta al WiFi | Interferencia | Acérquese al dispositivo |
| Imagen borrosa | Lente sucia o desenfocada | Limpie la lente o ajuste el enfoque |
| No captura fotos | Tarjeta SD llena o ausente | Verifique la tarjeta SD |
| Error al actualizar firmware | Conexión interrumpida | Intente nuevamente con conexión estable |

## Contribuir al Proyecto

El Otoscopio Educativo VOC es un proyecto abierto. Contribuciones son bienvenidas:
- Reporte de errores
- Sugerencias de mejoras
- Pull requests con nuevas características
- Documentación adicional

## Licencia

Este proyecto está licenciado bajo la Licencia MIT - ver el archivo LICENSE para más detalles.


---

*Desarrollado para mejorar la educación médica en técnicas de exploración otoscópica.*
