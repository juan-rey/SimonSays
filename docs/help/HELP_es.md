# Guía de Ayuda de SimonSays

## Descripción general
SimonSays es un asistente de texto a voz (TTS) para Windows, creado por Juan Rey Saura, optimizado para habla rápida, bibliotecas de frases y reproducción de sonidos en línea. Soporta varios idiomas, voces configurables y funciones pensadas para una comunicación eficiente.

SimonSays usa la zona libre a la izquierda de la barra de tareas de Windows para facilitar el acceso a una decena categorías con muchas frases frecuentes (modificables) al alcance de un par de clicks. 

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
2. Haz clic izquierdo en el icono de la bandeja o elige `Mostrar` para abrir la ventana principal.
3. Escribe texto personalizado en la caja de entrada o haz clic en una frase guardada en la ventana de Categorías.
4. Pulsa el botón `Reproducir>` para hablar.
5. Durante la reproducción, el botón muestra `>` para indicar estado activo.

## Ventana de Categorías (frases)
- Ábrela con el botón `Categorías`. Puedes redimensionarla/recordar tamaño desde Configuración.
- Selecciona una categoría arriba y frases abajo.
- Un clic en una frase la reproduce automáticamente si `Decir la frase al seleccionarla` está activado; si no, carga la frase en la caja principal sin hablar.

### Gestionar categorías
- Añadir: selecciona categoría → (atajo: `F3`) diálogo `Añadir categoria` → escribe nombre → `Aceptar`.
- Renombrar: selecciona categoría → (atajo: `F4`) `Editar` → cambia nombre → `Aceptar`.
- Mover selección: `F5`/`F6` para mover al elemento anterior/siguiente.
- Eliminar: selecciona categoría → (atajo: `F8`) `Eliminar`; confirma eliminación de categoría y frases.

### Gestionar frases
- Añadir: selecciona frase → (atajo: `F3`) `Añadir frase` → escribe texto → `Aceptar`.
- Editar: selecciona frase → (atajo: `F4`) `Editar` → actualiza texto → `Aceptar`.
- Mover selección: `F5`/`F6` para mover a la frase anterior/siguiente.
- Eliminar: selecciona frase → (atajo: `F8`) `Eliminar`; confirma.

## Configuración (F2)
Abre Configuración (`F2`) para controlar:
- **Texto predeterminado**: activar/desactivar `Insert default text automatically` y personalizar frase por defecto.
- **Idioma de la interfaz**: elegir idioma de UI.
- **Voz**: seleccionar voz SAPI instalada según preferencia.
- **Volumen/Velocidad**: ajustar `Volumen (10-100)` y `Velocidad (-10 a 10)`.
- **Comportamiento de reproducción**:
  - `Decir la frase al seleccionarla`.
  - `Recordar tamaño de la ventana de categorías`.
  - `Minimizar ventana de categorías automáticamente`.
  - `Aumentar volumen de SimonSays al reproducir`.
  - `Reducir temporalmente otros sonidos al reproducir`.
- `Probar voz`: probar voz.
- `Aceptar` guarda cambios; `Cancelar` descarta.

## Menú del icono de bandeja
- `Mostrar` / `Ocultar` interfaz principal.
- `Configuración`.
- `Acerca de` muestra versión y créditos.
- `Web` abre la página del proyecto (si está configurada).
- `Salir` cierra SimonSays.

Puedes mover la ventana emergente de bandeja y otros diálogos arrastrando la barra de título. Redimensiona la ventana de Categorías arrastrando los bordes y activa `Remember categoria window size` para conservar tamaño.

## Mezclar voz y sonidos
- Usa el delimitador `♫` (`SOUND_NOTE_DELIMITER`) para insertar nombres de archivo de audio en el texto:
  - Ejemplo: `Hola ♫notification.wav♫, por favor espere.`
  - El texto fuera de delimitadores se habla; el texto dentro se trata como archivo de sonido con `PlaySound`.
- Audio soportado: `.wav`, `.mid`, `.midi`.
- Usa rutas absolutas o relativas accesibles al proceso. Empareja delimitadores para evitar interpretar texto final como audio.

## Importar y exportar categorías
- Exportar: puedes exportar todas las categorías o solo la seleccionada (atajo: `F10`).
- Importar: elige archivo; si una categoría existe, se pedirá sobrescribir (atajo: `F9`).

## Almacenamiento de valores por defecto
- Las frases por defecto se crean por idioma en `HKCU\SOFTWARE\SimonSays\frases\<Language>` al primer inicio.
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
- El diálogo `Acerca de` muestra versión, descripción y copyright.
