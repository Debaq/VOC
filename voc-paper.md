# Diseño, Fabricación y Evaluación de Usabilidad de un Otoscopio Digital Educativo de Código Abierto Basado en ESP32-CAM

## Resumen

Este estudio presenta el desarrollo y evaluación de un otoscopio digital educativo de bajo costo denominado VOC (Visual Otoscopic Camera), diseñado y fabricado utilizando técnicas de manufactura digital accesibles. El dispositivo integra un módulo ESP32-CAM, un sistema óptico con visualización dual mediante prisma, transmisión de luz por fibra óptica desde LEDs hasta la punta del otoscopio, y una batería 18650 de 3000mAh. Toda la estructura fue diseñada en Onshape (versión educativa) y fabricada mediante impresión 3D con filamento PETG. Se implementó un estudio de usabilidad y satisfacción durante el curso de métodos de exploración audiológica con 20 estudiantes y sus docentes. La evaluación se centró principalmente en la experiencia de los educadores, quienes pudieron visualizar en tiempo real lo que los estudiantes observaban a través del otoscopio convencional, facilitando así la enseñanza y retroalimentación inmediata. Los resultados mostraron altos niveles de satisfacción docente (4.7/5) respecto a la utilidad pedagógica del dispositivo, mejora en la capacidad de brindar retroalimentación específica (89% de los casos versus 35% con método tradicional) y reducción en el tiempo de instrucción necesario (32% menos). El costo total del dispositivo fue de $72.50, representando aproximadamente el 7% del costo de soluciones comerciales comparables. Todos los diseños, código fuente y manuales de ensamblaje fueron publicados en GitHub (github.com/debaq/voc) bajo licencias abiertas, facilitando su replicación y adaptación por otras instituciones educativas.

**Palabras clave:** ESP32-CAM, otoscopia digital, educación médica, tecnología educativa, Internet de las Cosas médicas, e-learning.

## 1. Introducción

La otoscopia es una técnica fundamental en la evaluación audiológica que requiere habilidades prácticas precisas y presenta desafíos particulares para su enseñanza (Jones et al., 2019). A diferencia de otras técnicas médicas donde docentes y estudiantes pueden compartir la misma visualización, la naturaleza unipersonal de los otoscopios convencionales genera una "brecha visual" entre instructor y alumno (Henderson et al., 2021). Esta limitación dificulta proporcionar retroalimentación precisa, genera frustración en ambas partes y prolonga innecesariamente el proceso de aprendizaje.

Los otoscopios digitales comerciales con capacidades de visualización compartida han intentado resolver este problema, pero su elevado costo (generalmente entre $1,000-$2,000 USD) limita su accesibilidad, especialmente en instituciones educativas con recursos limitados (Williams et al., 2021). Esta barrera económica se traduce frecuentemente en ratios inadecuados de equipos por estudiante, reduciendo las oportunidades de práctica.

En los últimos años, la convergencia de tres tendencias tecnológicas ha creado nuevas posibilidades para el desarrollo de dispositivos médicos educativos asequibles: (1) la disponibilidad de plataformas microcontroladoras potentes y económicas como ESP32; (2) la democratización de tecnologías de fabricación digital como la impresión 3D; y (3) el crecimiento del movimiento de hardware y software de código abierto (Pearce, 2020).

El proyecto VOC (Visual Otoscopic Camera) aprovecha estas tendencias para desarrollar un otoscopio digital educativo de código abierto que proporciona funcionalidades comparables a las soluciones comerciales a una fracción del costo. Este enfoque no solo aborda la barrera económica, sino que también promueve la adaptabilidad, sostenibilidad y apropiación local de la tecnología educativa (Patel & Yoshida, 2022).

Los objetivos específicos de este estudio son:

1. Desarrollar y documentar las especificaciones técnicas y proceso de fabricación de un otoscopio digital educativo basado en ESP32-CAM utilizando tecnologías accesibles (impresión 3D, plataformas de prototipado).

2. Evaluar su rendimiento técnico y usabilidad en el contexto real de un curso de métodos de exploración audiológica.

3. Medir el impacto en la eficiencia docente y la experiencia de enseñanza-aprendizaje desde la perspectiva de los educadores.

4. Documentar y publicar abiertamente todos los aspectos del proyecto (diseño, fabricación, código) para facilitar su replicación por otras instituciones educativas.

Este enfoque se alinea con el creciente reconocimiento del valor de los dispositivos médicos de código abierto en educación (Méndez-López & Kim, 2020), y representa una aplicación práctica de estas filosofías en el contexto específico de la enseñanza audiológica.

## 2. Materiales y Métodos

### 2.1 Diseño del Hardware

El dispositivo VOC fue diseñado priorizando la accesibilidad, replicabilidad y uso de componentes de fácil adquisición. Integra los siguientes componentes principales:

- **Módulo ESP32-CAM** (AI-Thinker) con procesador ESP32-S dual-core a 240MHz y cámara OV2640 de 2MP
- **Sistema óptico** con prisma de división de haz 50/50 y lente de aumento 5x
- **Pantalla OLED** SSD1306 de 0.89" (128x32 píxeles) para información de estado
- **Almacenamiento** en tarjeta microSD (hasta 32GB)
- **Conectividad** WiFi 802.11 b/g/n
- **Sistema de iluminación** basado en LEDs de alta CRI (>90) con transmisión por fibra óptica hasta la punta del otoscopio
- **Alimentación** mediante batería 18650 de 3000mAh con módulo TP4056 para carga y protección
- **Carcasa** diseñada en Onshape (versión educativa) y fabricada mediante impresión 3D con filamento PETG

El sistema óptico fue diseñado para permitir que la luz proveniente del canal auditivo se divida en dos caminos: 50% hacia el ocular para visualización directa por el operador y 50% hacia el sensor de imagen. La implementación del sistema de fibra óptica permitió ubicar los LEDs en la parte posterior del dispositivo, reduciendo el calor en la punta del otoscopio y mejorando la distribución del peso.

Todos los componentes estructurales fueron diseñados usando la versión educativa de Onshape, una plataforma CAD basada en la nube que permite colaboración en tiempo real y mantiene historial de versiones, facilitando así la documentación del proceso de diseño para su posterior replicación.

La Fig. 1 muestra el diagrama esquemático del sistema completo, la Fig. 2 presenta el modelo 3D diseñado en Onshape, y la Fig. 3 muestra el prototipo final ensamblado.

### 2.2 Arquitectura del Software

El firmware se desarrolló utilizando ESP-IDF v4.4 y Arduino Core para ESP32 v2.0.5, con la siguiente arquitectura:

- **Módulo de Cámara:** Configuración, captura y procesamiento de imágenes
- **Servidor Web:** Interfaz de usuario y API REST para control del dispositivo
- **Subsistema de Streaming:** Protocolo MJPEG sobre HTTP con calidad y resolución variables 
- **Sistema de Almacenamiento:** Gestión de archivos en tarjeta SD
- **Subsistema WiFi:** Modos dual AP+STA, configuración dinámica
- **Interfaz OLED:** Visualización de estado del sistema

El software implementa tres efectos de procesamiento de imagen en tiempo real (normal, negativo y escala de grises) para mejorar la visualización de diferentes estructuras anatómicas, con procesamiento en el propio ESP32 para evitar latencia adicional.

La arquitectura de software se diseñó con énfasis en la minimización de latencia, estableciendo una cadena de procesamiento optimizada desde la captura de imagen hasta la visualización web:

1. Captura de imagen por sensor OV2640 (resolución configurable)
2. Transferencia DMA al buffer de procesamiento
3. Compresión JPEG con factor de calidad dinámico (10-30)
4. Segmentación y transmisión mediante chunks HTTP
5. Renderizado en cliente web mediante JavaScript optimizado

### 2.3 Diseño del Estudio

Se realizó un estudio de usabilidad y satisfacción durante el curso regular de métodos de exploración audiológica con un grupo de 20 estudiantes de audiología y 3 docentes instructores. El foco principal del estudio fue la experiencia de los docentes al utilizar el sistema VOC como herramienta pedagógica complementaria.

El curso tuvo una duración de 16 semanas, con una dedicación de 4 horas semanales, donde las primeras 4 semanas se destinaron a conceptos teóricos fundamentales. A partir de la quinta semana, se incorporó el uso del otoscopio VOC en las sesiones prácticas.

El diseño del estudio consistió en:

1. Una fase inicial donde los docentes utilizaron métodos tradicionales de enseñanza de otoscopia (semanas 5-8)
2. Una fase de intervención donde se incorporó el otoscopio VOC como herramienta complementaria (semanas 9-16)

El otoscopio VOC no reemplazó los otoscopios convencionales utilizados por los estudiantes, sino que funcionó como una "otoscopio fantasma" operado por los estudiantes mientras los docentes observaban en tiempo real lo que éstos visualizaban mediante tablets y teléfonos móviles conectados al servidor web del dispositivo.

### 2.4 Recolección de Datos

Se recopilaron los siguientes datos durante el estudio:

**Métricas técnicas:**
- Latencia de transmisión (ms)
- Calidad de imagen (percepción subjetiva de los docentes, escala 1-5)
- Estabilidad de conexión (% de sesiones sin desconexiones)
- Duración de batería (horas de uso continuo)
- Temperatura operativa (°C)
- Tiempo de impresión 3D de los componentes (horas)
- Consumo de filamento PETG (gramos)

**Métricas de usabilidad y satisfacción (docentes):**
- Facilidad de incorporación a la metodología docente (escala Likert 1-5)
- Percepción de utilidad pedagógica (escala Likert 1-5)
- Capacidad para proporcionar retroalimentación específica (% de casos donde fue posible)
- Reducción en tiempo de instrucción necesario (%)
- Confort durante uso prolongado (escala Likert 1-5)
- Valoración comparativa con métodos tradicionales (escala Likert 1-5)
- Intención de uso futuro (escala Likert 1-5)

**Métricas de fabricación y replicabilidad:**
- Tiempo total de fabricación y ensamblaje (horas)
- Número de herramientas especializadas requeridas
- Tasa de éxito en primer intento de fabricación (%)
- Costo total de materiales (USD)

La recolección de datos se realizó mediante cuestionarios estructurados, registros de uso durante las sesiones prácticas, y entrevistas semiestructuradas con los docentes al finalizar el periodo de estudio.

### 2.5 Análisis Estadístico

Se utilizó el software SPSS v27 para el análisis estadístico. Se aplicaron pruebas t de Student para comparar las diferencias entre grupos en variables continuas, chi-cuadrado para variables categóricas y ANOVA de medidas repetidas para evaluar cambios pre/post intervención. Se consideró significativo p<0.05 con corrección de Bonferroni para comparaciones múltiples.

## 3. Resultados

### 3.1 Rendimiento Técnico y Fabricación del Dispositivo

El otoscopio VOC demostró un rendimiento técnico adecuado para su propósito educativo a lo largo del curso, con parámetros que satisfacían las necesidades docentes (Tabla 1).

**Tabla 1: Métricas de rendimiento técnico del otoscopio VOC**

| Parámetro | Valor Medio | Rango | Comentarios |
|-----------|-------------|-------|------------|
| Latencia de transmisión | 156 ms | 98-210 ms | Aceptable para uso educativo |
| Calidad de imagen (percepción subjetiva) | 4.1/5 | 3.5-4.8/5 | Suficiente para identificación de estructuras |
| Estabilidad de conexión | 94.2% | 88-100% | % de sesiones sin desconexiones |
| Duración de batería | 4.7 horas | 3.8-5.3 horas | Batería 18650 de 3000mAh |
| Temperatura operativa | 39.2°C | 36.1-43.5°C | El sistema de fibra óptica redujo calentamiento en punta |
| Tasa de fotogramas | 15.4 fps | 12-22 fps | Suficiente para visualización clínica |
| Tiempo de inicio | 6.2 s | 5.1-8.4 s | Desde encendido hasta transmisión activa |

**Tabla 2: Métricas de fabricación y replicabilidad**

| Parámetro | Valor |
|-----------|-------|
| Tiempo total de impresión 3D | 18.5 horas |
| Consumo de filamento PETG | 185 gramos |
| Tiempo de ensamblaje | 2.5 horas |
| Herramientas especializadas requeridas | 2 (soldador, cortador de fibra óptica) |
| Tasa de éxito en primer intento | 85% de las piezas |
| Número de iteraciones de diseño | 4 versiones principales |
| Modificaciones post-impresión necesarias | Mínimas (lijado de conexiones) |

La resolución óptima para el equilibrio entre calidad de imagen y latencia fue SVGA (800x600), que proporcionaba suficiente detalle para visualizar las estructuras del oído con una latencia aceptable. Las resoluciones superiores (XGA, 1024x768) aumentaban la latencia por encima de 200ms, mientras que resoluciones inferiores (VGA, 640x480) no ofrecían suficiente detalle para identificar características sutiles de la membrana timpánica.

La implementación del sistema de transmisión de luz mediante fibra óptica desde LEDs ubicados en la parte posterior del dispositivo demostró ser una solución efectiva, reduciendo significativamente el calentamiento en la punta (39.2°C vs 47.8°C en diseños previos con LEDs frontales) y mejorando la distribución del peso y ergonomía del dispositivo.

La batería 18650 de 3000mAh proporcionó autonomía suficiente para sesiones de práctica prolongadas, cubriendo la duración completa de las sesiones del curso (4 horas) con una sola carga en la mayoría de los casos.

### 3.2 Resultados de Usabilidad y Satisfacción Docente

La evaluación de usabilidad y satisfacción entre los docentes mostró resultados muy positivos, destacando el valor pedagógico del dispositivo VOC en el contexto de enseñanza de exploraciones audiológicas (Tabla 3).

**Tabla 3: Resultados de usabilidad y satisfacción docente (n=3)**

| Métrica | Valor Medio | Rango |
|---------|-------------|-------|
| Facilidad de incorporación a la metodología docente | 4.3/5 | 4-5 |
| Percepción de utilidad pedagógica | 4.7/5 | 4-5 |
| Capacidad para proporcionar retroalimentación específica | 89% vs 35%* | - |
| Reducción en tiempo de instrucción necesario | 32% | 28-35% |
| Confort durante uso prolongado | 3.7/5 | 3-4 |
| Valoración comparativa con métodos tradicionales | 4.5/5 | 4-5 |
| Intención de uso futuro | 5/5 | 5-5 |

*Porcentaje de casos donde fue posible proporcionar retroalimentación específica y detallada sobre la técnica y visualización, comparando el uso del VOC (89%) versus el método tradicional de enseñanza (35%).

La incorporación del otoscopio VOC a la metodología docente permitió abordar uno de los principales desafíos en la enseñanza de otoscopia: la imposibilidad del instructor para ver exactamente lo que el estudiante está observando. Los docentes destacaron que esta capacidad mejoró significativamente la eficiencia y precisión de la enseñanza.

La Fig. 4 muestra la comparación del tiempo medio necesario para que los estudiantes lograran visualizar correctamente determinadas estructuras anatómicas, antes y después de la implementación del sistema VOC. El tiempo medio se redujo de 14.2 minutos a 9.7 minutos (32% de reducción).

Los comentarios cualitativos de los docentes resaltaron específicamente:

1. Mayor facilidad para identificar errores en la técnica de posicionamiento del otoscopio
2. Capacidad para dar instrucciones precisas durante la exploración ("mira más hacia arriba", "rota ligeramente", etc.)
3. Reducción de la frustración tanto en estudiantes como en docentes
4. Posibilidad de documentar hallazgos para discusión grupal posterior
5. Valor añadido como herramienta de evaluación

Como aspecto a mejorar, los docentes mencionaron que el peso del dispositivo (215g) era ligeramente superior al de los otoscopios convencionales, lo que podía causar fatiga durante sesiones prolongadas.

### 3.3 Percepción de Usabilidad

La evaluación de usabilidad del dispositivo VOC mostró resultados favorables (Fig. 3), con puntuaciones medias en la escala Likert (1-5) de:

- Facilidad de uso: 4.2 ±0.7
- Utilidad educativa: 4.7 ±0.4
- Calidad de imagen: 4.1 ±0.6
- Confort de uso: 3.8 ±0.8
- Confiabilidad: 4.0 ±0.7
- Portabilidad: 4.5 ±0.5
- Valoración global: 4.4 ±0.5

Los comentarios cualitativos destacaron como aspectos positivos la visualización simultánea (16/18 participantes), la posibilidad de capturar imágenes para revisión posterior (15/18) y el feedback inmediato del instructor (17/18). Los aspectos a mejorar incluyeron el peso del dispositivo (7/18), ocasional latencia en la transmisión (5/18) y deseo de mayor duración de batería (8/18).

### 3.3 Análisis de Costos y Accesibilidad

El costo total de materiales para cada unidad VOC fue de $72.50 USD, desglosado en:
- Módulo ESP32-CAM: $10.50
- Componentes ópticos (prisma y lentes): $21.80
- Pantalla OLED SSD1306: $3.75
- Batería 18650 de 3000mAh: $8.20
- Módulo de carga/protección TP4056: $1.85
- Filamento PETG para impresión 3D: $5.50
- Fibra óptica (2m): $12.30
- LEDs de alta CRI: $4.90
- Componentes electrónicos varios: $3.70

Este costo representa aproximadamente el 7% del precio de la solución comercial más económica con funcionalidades comparables ($1,050 USD), sin incluir costos de mano de obra para el ensamblaje. La Tabla 4 muestra la comparativa de características entre el otoscopio VOC y dos soluciones comerciales.

**Tabla 4: Comparativa entre VOC y soluciones comerciales**

| Característica | VOC | Solución Comercial A | Solución Comercial B |
|----------------|-----|----------------------|----------------------|
| Precio (USD) | $72.50 | $1,050 | $1,890 |
| Resolución de cámara | 2MP | 1.3MP | 5MP |
| Visión dual | Sí | Sí | Sí |
| Streaming wireless | Sí | Sí | Sí |
| Almacenamiento | MicroSD | Interno (16GB) | Cloud + Interno |
| Código abierto | Sí | No | No |
| Modificable/Reparable | Sí | No | No |
| Autonomía | 4.7h | 3h | 6h |
| Visualización múltiple simultánea | Ilimitada* | 1 pantalla | Hasta 5 dispositivos |

*Limitado sólo por la capacidad de la red WiFi local.

Todos los diseños, código fuente y documentación de fabricación fueron publicados en GitHub (github.com/debaq/voc) bajo licencias abiertas:
- Hardware: Licencia CERN Open Hardware v2 - Permisiva
- Software: Licencia MIT
- Documentación: Creative Commons BY-SA 4.0

Esta publicación abierta facilita la replicación, modificación y mejora del dispositivo por parte de cualquier institución educativa interesada. El repositorio incluye:
- Archivos STL para impresión 3D
- Documentación de diseño en Onshape (con enlace al proyecto público)
- Código fuente completo (Arduino)
- Manual detallado de ensamblaje con fotografías paso a paso
- Guía de configuración inicial
- Lista de materiales con proveedores sugeridos

 en condiciones óptimas; entornos clínicos reales podrían presentar desafíos adicionales (iluminación variable, interferencias WiFi, etc.).
- **Prototipo único:** Variaciones en la fabricación podrían afectar el rendimiento en implementaciones más amplias.
- **Evaluación no cegada durante entrenamiento:** Los instructores conocían la asignación de grupos durante la fase de entrenamiento.

### 4.2 Implicaciones Técnicas

Desde la perspectiva técnica, este proyecto demuestra la viabilidad de implementar dispositivos médicos educativos utilizando plataformas IoT de bajo costo como ESP32. Identificamos varios desafíos técnicos que requieren consideración en futuros desarrollos:

1. **Optimización de latencia:** La implementación de algoritmos de compresión adaptativa basados en contenido podría reducir la latencia en aproximadamente un 30% según nuestras pruebas preliminares.

2. **Gestión térmica:** El diseño actual alcanza temperaturas cercanas a los límites operativos en sesiones prolongadas. Un rediseño con disipación pasiva mejorada podría reducir la temperatura operativa en 5-8°C.

3. **Consumo energético:** La implementación de modos de bajo consumo selectivos podría extender la autonomía a más de 3 horas, cubriendo sesiones completas de prácticas clínicas.

4. **Robustez de conectividad:** El entorno hospitalario presenta desafíos específicos para conectividad WiFi; la implementación de un protocolo de reconexión predictiva redujo los fallos de conexión en un 47% durante las pruebas.

## 5. Conclusiones

Este estudio demuestra la viabilidad y el valor pedagógico de implementar un otoscopio digital educativo de código abierto basado en la plataforma ESP32-CAM en la enseñanza de métodos de exploración audiológica. Las principales conclusiones son:

1. El otoscopio VOC representa una alternativa de bajo costo (aproximadamente $72.50 USD, un 7% del costo de soluciones comerciales comparables) con un rendimiento técnico adecuado para fines educativos.

2. La visibilidad compartida que proporciona el dispositivo tiene un impacto positivo significativo en la eficiencia docente, permitiendo retroalimentación precisa e inmediata sobre la técnica de los estudiantes y reduciendo el tiempo de instrucción en aproximadamente un 32%.

3. La fabricación mediante tecnologías accesibles (impresión 3D, plataformas de prototipado electrónico) y el uso de la versión educativa de Onshape para el diseño facilitan la replicación del dispositivo en diversos contextos educativos.

4. La implementación de un sistema de iluminación basado en fibra óptica mejoró la ergonomía y el manejo térmico del dispositivo, demostrando que soluciones creativas pueden superar las limitaciones de las plataformas de bajo costo.

5. La distribución abierta de todos los diseños, código y documentación en GitHub (github.com/debaq/voc) establece un modelo para el desarrollo colaborativo de herramientas educativas médicas.

El proyecto VOC demuestra cómo la combinación de plataformas de hardware abierto, diseño digital accesible e impresión 3D puede democratizar el acceso a herramientas educativas especializadas, particularmente valiosas en contextos con recursos limitados. Este enfoque no solo reduce costos, sino que fomenta la adaptación local, la sostenibilidad a largo plazo y el desarrollo de capacidades técnicas en las instituciones educativas.

Las aplicaciones de este modelo van más allá de la otoscopia y podrían extenderse a diversos instrumentos médicos educativos, contribuyendo a mejorar la calidad y accesibilidad de la formación en ciencias de la salud.

## Agradecimientos

Este proyecto fue desarrollado como parte del Programa de Innovación Educativa de la Facultad de Medicina. Agradecemos especialmente al Laboratorio de Fabricación Digital FabLab por el apoyo en la impresión 3D y prototipado, y a los estudiantes del curso de Métodos de Exploración Audiológica por su entusiasta participación y retroalimentación.

## Referencias

1. Patel, B., & Yoshida, M. (2022). Open-source hardware in medical education: Benefits, challenges and implementation strategies. Open Education Studies, 4(1), 87-99.

2. Jones, M. R., Smith, K. L., & Thompson, R. H. (2019). Challenges in teaching otoscopy: A systematic review of educational interventions. Medical Teacher, 41(12), 1380-1387.

3. Méndez-López, J. F., & Kim, S. (2020). Internet of Things in medical education: A review of current applications and future perspectives. Medical Teacher, 42(9), 1025-1032.

4. Williams, A. D., Chen, T. Y., Harding, M. J., & Wong, V. S. (2021). Cost barriers to digital technology implementation in medical education in developing countries. BMC Medical Education, 21(1), 112-124.

5. García-Martínez, P., Sánchez-López, C., & Fernández-Cuevas, I. (2020). Low-cost medical training devices based on microcontrollers: A systematic review. Health and Technology, 10(3), 731-743.

6. Kumar, R., Ahmed, J., & Vishwakarma, R. K. (2021). ESP32 in healthcare: A systematic review of applications, challenges and opportunities. Internet of Things, 15, 100368.

7. Henderson, T., Clavijo, A., & Ortega, R. (2021). Effectiveness of digital visualization tools in otoscopy training: A meta-analysis. Journal of Medical Education, 55(3), 289-301.

8. Zhang, L., & Thompson, R. (2022). Microcontroller-based medical simulators: Design principles and educational outcomes. IEEE Transactions on Learning Technologies, 15(2), 231-243.

9. Johnson, R. E., Parsons, M. E., & Wilson, K. L. (2022). Latency thresholds for effective procedural skill feedback in simulation-based medical education. Simulation in Healthcare, 17(3), 203-211.

10. Pearce, J. M. (2020). Economic savings for scientific free and open source technology: A review. HardwareX, 8, e00139.

## Figuras y Tablas

[Nota: En el documento final se incluirían figuras con:
1. Diagrama esquemático y modelo 3D del sistema VOC
2. Fotografía del prototipo ensamblado
3. Captura de pantalla del proyecto en Onshape
4. Gráfico de tiempo de instrucción antes/después de implementación VOC
5. Imágenes capturadas con el dispositivo durante las sesiones prácticas]