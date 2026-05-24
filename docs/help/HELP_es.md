# Guía de Ayuda de SimonSays

## Descripción general
SimonSays es un asistente de texto a voz (TTS) para Windows, creado por Juan Rey Saura, optimizado para habla rápida, bibliotecas de frases y reproducción de sonidos en línea. Soporta varios idiomas, voces configurables y funciones pensadas para una comunicación eficiente.

SimonSays usa la zona libre a la izquierda de la barra de tareas de Windows para facilitar el acceso a una decena de categorías con muchas frases frecuentes (modificables) al alcance de un par de clics.

## Atajos
- `F1`: Abrir Ayuda.
- `F2`: Abrir Configuración.
- `F3`: Añadir después de la selección actual (contexto categoría/frase).
- `F4`: Editar selección actual.
- `F5` / `F6`: Mover a la selección anterior / siguiente en listas.
- `F8`: Eliminar selección actual.
- `F9`: Importar categorías.
- `F10`: Exportar categorías.
- Botón `Reproducir>`: Iniciar reproducción.

## Inicio y uso básico
1. Inicia SimonSays; aparece en la bandeja de la barra de tareas de Windows.
2. Para hablar, puedes:
   - Escribir texto personalizado en la caja de entrada y pulsar el botón `Reproducir>` (o `Enter`).
   - Hacer clic en una o varias frases guardadas en la ventana de Categorías.
3. Durante la reproducción, el botón muestra `>` para indicar estado activo. Puedes pulsarlo de nuevo para detener la reproducción al instante.

## Ventana de Categorías (frases)
- Ábrela con el botón `Categorías`. Puedes redimensionarla o recordar su tamaño desde Configuración.
- Selecciona una categoría arriba y frases abajo.
- Un clic en una frase la reproduce automáticamente si `Decir la frase al seleccionarla` está activado; si no, carga la frase en la caja principal sin hablar.

### Gestionar categorías
- Añadir: selecciona categoría → (atajo: `F3`) diálogo `Agregar nueva categoría` → escribe nombre → `Aceptar`.
- Renombrar: selecciona categoría → (atajo: `F4`) `Cambiar nombre de categoría` → cambia nombre → `Aceptar`.
- Mover selección: `F5`/`F6` para mover al elemento anterior/siguiente.
- Eliminar: selecciona categoría → (atajo: `F8`) `Eliminar categoría`; confirma eliminación de categoría y frases.



#### Configurar o cambiar el icono de una categoría
Cada categoría puede mostrar un emoji o un archivo personalizado `.ico` en su botón. El icono se especifica en el diálogo Agregar/Editar usando el prefijo separador `##`:

```
<icono>##<nombre de categoría>
```

- **Emoji**: escribe el/los emoji seguidos de `##` y el nombre.
  - Ejemplo: `🙂##Saludos`
- **Archivo de icono** (`.ico`): escribe la ruta del archivo (absoluta o relativa a la carpeta de SimonSays) seguida de `##` y el nombre.
  - Ejemplo: `C:\icons\smile.ico##Saludos`
- **Sin icono**: escribe solo el nombre de la categoría sin prefijo `##`.
  - Ejemplo: `Saludos`

> Nota: solo se admiten archivos `.ico` para iconos basados en archivo. Cualquier otra extensión se ignora y no se muestra icono.

### Gestionar frases
- Añadir: selecciona frase → (atajo: `F3`) `Agregar nueva frase` → escribe texto → `Aceptar`.
- Editar: selecciona frase → (atajo: `F4`) `Editar frase` → actualiza texto → `Aceptar`.
- Mover selección: `F5`/`F6` para mover a la frase anterior/siguiente.
- Eliminar: selecciona frase → (atajo: `F8`) `Eliminar frase`; confirma.



#### Configurar o cambiar el icono de una frase
Las frases siguen la misma convención de prefijo de icono que las categorías, con el separador opcional `##` antes del texto de la frase:

```
<icono>##<texto de frase>
```

- **Emoji**: `👍##¡Suena bien!`
- **Archivo de icono**: `C:\icons\check.ico##¡Suena bien!`
- **Sin icono**: `¡Suena bien!`

Las frases también pueden incluir un archivo de audio en línea. Al combinar icono, texto y archivo de audio, el formato completo es:

```
<icono>##<texto de frase>::<archivo de audio>
```

- Ejemplo: `🔔##Atención::notification.wav`

El prefijo `##` y el sufijo `::` son opcionales e independientes; puedes usar cualquier combinación:
- Solo prefijo de icono: `🔔##Atención`
- Solo sufijo de audio: `Atención::notification.wav`
- Ambos: `🔔##Atención::notification.wav`
- Ninguno: `Atención`

## Configuración (F2)
Abre Configuración (`F2`) para controlar:
- **Texto predeterminado**: activar/desactivar `Insertar texto predeterminado automáticamente` y personalizar frase por defecto.
  - `Mostrar teclado táctil` — abre el teclado táctil en pantalla cuando la caja de entrada recibe el foco, posicionado cerca de la ventana principal.
- **Idioma de la interfaz**: elegir idioma de UI (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **Voz**: seleccionar voz SAPI instalada según preferencia.
- **Volumen/Velocidad**: ajustar `Volumen (10-100)` y `Velocidad (-10 a 10)`.
- `Probar voz`: probar la voz seleccionada.
- **Ventana de categorías**:
  - `Minimizar ventana de categorías automáticamente` tras seleccionar.
  - `Recordar tamaño de la ventana de categorías`.
- **Comportamiento de reproducción**:
  - `Decir la frase al seleccionarla` (auto-reproducción al hacer clic).
  - `Detener reproducción anterior` — detiene automáticamente cualquier reproducción en curso antes de iniciar una nueva frase.
  - `Aumentar volumen de SimonSays al reproducir` — sube el volumen maestro del sistema mientras habla, restaurándolo cuando termina la reproducción.
  - `Reducir temporalmente otros sonidos al reproducir` — reduce el volumen de las demás aplicaciones mientras habla, restaurándolo cuando termina la reproducción.
- `Aceptar` guarda cambios; `Cancelar` descarta.

## Menú del icono de bandeja
- `Mostrar` / `Ocultar` interfaz principal.
- `Configuración`.
- `Acerca de` muestra versión y créditos.
- `Web` abre la página del proyecto (si está configurada).
- `Danos tu opinión` abre el [formulario de comentarios](https://forms.gle/KMbpEDYmwnFJUhgy8) en el navegador predeterminado.
- `Salir` cierra SimonSays.

Puedes mover la ventana emergente de bandeja y otros diálogos arrastrando la barra de título; redimensiona la ventana de Categorías arrastrando los bordes y activa `Recordar tamaño de la ventana de categorías` en Configuración para conservar el tamaño.

## Mezclar voz y sonidos
- Usa el delimitador `♫` (`SOUND_NOTE_DELIMITER`) para insertar nombres de archivo de audio en el texto:
  - Ejemplo: `Hola ♫notification.wav♫, por favor espere.`
  - El texto fuera de delimitadores se habla; el texto dentro se trata como archivo de sonido y se reproduce en línea.
- Audio soportado: `.wav`, `.mid`, `.midi`, `.mp3`.
- Usa rutas absolutas o relativas accesibles al proceso. Empareja delimitadores para evitar interpretar texto final como audio.



### Orden de búsqueda de archivos de sonido e icono
Cuando un nombre de archivo de sonido o icono no contiene una letra de unidad o una ruta completa, SimonSays lo busca en este orden, usando la primera coincidencia:

1. **Carpeta AppData** — `%APPDATA%\SimonSays\` (específica del usuario; coloca aquí tus archivos para separarlos de la instalación).
2. **Directorio de trabajo** — el directorio de trabajo actual del proceso (solo se busca si es distinto de la carpeta del ejecutable).
3. **Carpeta del ejecutable** — la carpeta que contiene `SimonSays.exe`.

Si no se encuentra el archivo en estas ubicaciones, se usa el sonido de reserva integrado.

> **Consejo:** se recomienda colocar tus sonidos e iconos personalizados en `%APPDATA%\SimonSays\`; sobrevivirán a las actualizaciones y son específicos del usuario.

## Importar y exportar categorías
- Exportar: puedes exportar todas las categorías o solo la seleccionada cuando se te pregunte. Los mensajes de éxito/error aparecen en el idioma seleccionado (atajo: `F10`).
- Importar: elige archivo; si una categoría existe, se pedirá sobrescribir. Los mensajes de éxito/error aparecen en el idioma seleccionado (atajo: `F9`).

## Solución de problemas
- **Fallo de inicialización de voz**: verifica que SAPI esté instalado y que haya una voz compatible.
- **No se reproducen sonidos**: confirma ruta/extensión y delimitadores `♫` emparejados.
- **Errores de importación/exportación**: revisa permisos y ubicación de disco; reintenta.
- **Idioma UI no aplicado**: reabre Configuración y confirma idioma; asegúrate de que existan cadenas de localización para el idioma elegido.

## Acerca de
- El diálogo `Acerca de` muestra versión, descripción y copyright.
