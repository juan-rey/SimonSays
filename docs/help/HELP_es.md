# Guía de Ayuda de SimonSays

## Descripción general
SimonSays es un asistente de texto a voz (TTS) para Windows, creado por Juan Rey Saura, optimizado para habla rápida, bibliotecas de frases y reproducción de sonidos en línea. Soporta varios idiomas, voces configurables y funciones pensadas para una comunicación eficiente.

## Atajos
- `F1`: abrir Ayuda.
- `F2`: abrir Configuración.
- `F3`: añadir después de la selección actual (contexto categoría/frase).
- `F4`: editar selección actual.
- `F5` / `F6`: mover a la selección anterior / siguiente en listas.
- `F8`: eliminar selección actual.
- `F9`: importar categorías.
- `F10`: exportar categorías.
- Botón `Play>`: iniciar reproducción (clic).

## Inicio y uso básico
1. Inicia SimonSays; aparece en la bandeja de la barra de tareas de Windows.
2. Haz clic izquierdo en el icono de la bandeja o elige `Show` para abrir la ventana principal.
3. Escribe texto personalizado en la caja de entrada o haz clic en una frase guardada en la ventana de Categorías.
4. Pulsa el botón `Play>` (o atajo si está configurado) para hablar.
5. Durante la reproducción, el botón muestra `>` para indicar estado activo.

## Ventana de Categorías (frases)
- Ábrela con el botón `Categories`. Puedes redimensionarla/recordar tamaño desde Configuración.
- Selecciona una categoría arriba y frases abajo.
- Un clic en una frase la reproduce automáticamente si `Speak phrase immediately when selecting` está activado; si no, carga la frase en la caja principal sin hablar.

### Gestionar categorías
- Añadir: selecciona categoría → (atajo: `F3`) diálogo `Add category` → escribe nombre → `OK`.
- Renombrar: selecciona categoría → (atajo: `F4`) `Edit` → cambia nombre → `OK`.
- Mover selección: `F5`/`F6` para mover al elemento anterior/siguiente.
- Eliminar: selecciona categoría → (atajo: `F8`) `Delete`; confirma eliminación de categoría y frases.

### Gestionar frases
- Añadir: selecciona frase → (atajo: `F3`) `Add phrase` → escribe texto → `OK`.
- Editar: selecciona frase → (atajo: `F4`) `Edit` → actualiza texto → `OK`.
- Mover selección: `F5`/`F6` para mover a la frase anterior/siguiente.
- Eliminar: selecciona frase → (atajo: `F8`) `Delete`; confirma.

## Configuración (F2)
Abre Configuración (`F2`) para controlar:
- **Texto predeterminado**: activar/desactivar `Insert default text automatically` y personalizar frase por defecto.
- **Idioma de la interfaz**: elegir idioma de UI.
- **Voz**: seleccionar voz SAPI instalada según preferencia.
- **Volumen/Velocidad**: ajustar `Volume (10-100)` y `Rate (-10 to 10)`.
- **Comportamiento de reproducción**:
  - `Speak phrase immediately when selecting`.
  - `Remember category window size`.
  - `Minimize category window automatically`.
  - `Increase SimonSays volume when playing`.
  - `Temporarily reduce other audio when playing`.
- `Test Voice`: probar voz.
- `OK` guarda cambios; `Cancel` descarta.

## Menú del icono de bandeja
- `Show` / `Hide` interfaz principal.
- `Settings`.
- `About` muestra versión y créditos.
- `Web` abre la página del proyecto (si está configurada).
- `Exit` cierra SimonSays.

Puedes mover la ventana emergente de bandeja y otros diálogos arrastrando la barra de título. Redimensiona la ventana de Categorías arrastrando los bordes y activa `Remember category window size` para conservar tamaño.

## Mezclar voz y sonidos
- Usa el delimitador `♫` (`SOUND_NOTE_DELIMITER`) para insertar nombres de archivo de audio en el texto:
  - Ejemplo: `Hello ♫notification.wav♫, please wait.`
  - El texto fuera de delimitadores se habla; el texto dentro se trata como archivo de sonido con `PlaySound`.
- Audio soportado: `.wav`, `.mid`, `.midi`.
- Usa rutas absolutas o relativas accesibles al proceso. Empareja delimitadores para evitar interpretar texto final como audio.

## Importar y exportar categorías
- Exportar: puedes exportar todas las categorías o solo la seleccionada (atajo: `F10`).
- Importar: elige archivo; si una categoría existe, se pedirá sobrescribir (atajo: `F9`).

## Almacenamiento de valores por defecto
- Las frases por defecto se crean por idioma en `HKCU\SOFTWARE\SimonSays\Phrases\<Language>` al primer inicio.
- Para reinstalar valores por defecto de un idioma: cierra SimonSays → elimina la clave del idioma → reinicia SimonSays.

## Comportamiento de barra de tareas y ventana
- SimonSays soporta actualmente barra de tareas inferior.
- La ventana de Categorías puede minimizarse automáticamente o recordar tamaño según Configuración.

## Solución de problemas
- **Fallo de inicialización de voz**: verifica SAPI y voces compatibles.
- **No se reproducen sonidos**: confirma ruta/extensión y delimitadores `♫`.
- **Errores de importación/exportación**: revisa permisos/ruta de disco y reintenta.
- **Idioma UI no aplicado**: reabre Configuración y confirma idioma.

## Acerca de
- El diálogo `About` muestra versión, descripción y copyright.
