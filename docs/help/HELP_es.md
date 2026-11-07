# Guía de ayuda de SimonSays

## Descripción general
SimonSays es un asistente de texto a voz (TTS) para Windows creado por Juan Rey Saura. Nació como una herramienta para personas con ELA y otras personas que ya no pueden hablar, para que pudieran comunicarse rápidamente sin dejar lo que estaban haciendo en el ordenador. Con el tiempo quedó claro que el mismo diseño sirve como herramienta general de CAA (comunicación aumentativa y alternativa) para cualquier persona a la que le cueste hablar o no pueda hacerlo; por ejemplo, por disartria, una laringectomía, apraxia del habla, un ictus o una lesión cerebral. Está optimizado para hablar rápido, usar bibliotecas de frases y reproducir sonidos dentro del texto. Admite varios idiomas, voces configurables y funciones pensadas para una comunicación eficiente.

SimonSays usa el espacio libre de la barra de tareas de Windows y da acceso rápido a una decena de categorías de frases de uso frecuente (y personalizables) con solo un par de clics. Como nunca tapa tus otras ventanas, puedes seguir en el navegador, en tus documentos o en una videollamada y hablar siempre que lo necesites: con el teclado, con el ratón o sin manos, con un seguidor ocular.

Casos de uso:
- Uso intensivo de varias aplicaciones de Windows sin perder de vista la comunicación.
- Tableros creados por logopedas para personas con discapacidad del habla.
- Reuniones en línea con [Virtual CABLE](https://vb-audio.com/Cable/) (consulta [Usar SimonSays en reuniones en línea](#usar-simonsays-en-reuniones-en-línea)).

SimonSays no pretende sustituir a programas de comunicación completos como Mind Express, TD Communicator, Grid 3 u OptiKey, ni la valoración y el apoyo profesional que suelen acompañarlos. Hace un trabajo más concreto: hablar rápidamente desde la barra de tareas mientras sigues trabajando en otras aplicaciones. Puede funcionar junto a esos programas; por ejemplo, OptiKey u otro software de control ocular puede mover el cursor mientras SimonSays dice tus frases.

SimonSays es de código abierto y gratuito (libre como en libertad, no solo como en precio), y seguirá siéndolo: cualquiera puede usarlo, estudiarlo, adaptarlo y compartirlo. No tiene publicidad ni recoge datos.

SimonSays se acerca a la versión 1.0. Antes de ese hito necesita que haya gente que lo pruebe en su día a día, con sus propias voces, tableros y seguidores oculares: si algo falla, resulta incómodo o falta, cuéntanoslo (consulta [Comentarios, soporte y privacidad](#comentarios-soporte-y-privacidad)). Cada comentario ayuda.

## Contenido
- [Primeros pasos](#primeros-pasos)
- [Ventana de categorías](#ventana-de-categorías)
- [Frases predeterminadas](#frases-predeterminadas)
- [Atajos](#atajos)
- [Configuración (F2)](#configuración-f2)
- [Mirada / Clic por fijación (F3)](#mirada--clic-por-fijación-f3)
- [Usar SimonSays en reuniones en línea](#usar-simonsays-en-reuniones-en-línea)
- [Menú del icono de la bandeja](#menú-del-icono-de-la-bandeja)
- Editar tableros (avanzado):
  - [Editar categorías y frases](#editar-categorías-y-frases) (y [qué editor usar](#qué-editor-usar))
  - [Mezclar voz y sonidos](#mezclar-voz-y-sonidos)
  - [Personalizar el aspecto (estilos de tablero y de categoría)](#personalizar-el-aspecto-estilos-de-tablero-y-de-categoría)
  - [Importar y exportar categorías](#importar-y-exportar-categorías)
  - [Deshacer cambios](#deshacer-cambios)
  - [Dónde guarda SimonSays tus archivos](#dónde-guarda-simonsays-tus-archivos)
- [Solución de problemas](#solución-de-problemas)
- [Comentarios, soporte y privacidad](#comentarios-soporte-y-privacidad)

## Primeros pasos
1. Inicia SimonSays. Aparece en un espacio libre de la barra de tareas de Windows y añade un icono a la bandeja del sistema. La barra de tareas puede estar abajo, arriba, a la izquierda o a la derecha de la pantalla:
   - Cuando los iconos de la barra de tareas están centrados, SimonSays se coloca al principio de la barra (en el extremo izquierdo, o arriba en una barra lateral). Cuando están alineados a la izquierda, se coloca justo antes de la bandeja del sistema.
   - En una barra de tareas pequeña (estrecha), la barra de SimonSays es más baja, con una letra más pequeña y los dos botones de acceso rápido uno al lado del otro. En una barra de tareas a la izquierda o a la derecha de la pantalla, los controles se apilan uno debajo de otro.
   - La ventana de categorías se abre junto a la barra de tareas, y la flecha del botón `Categorías` indica hacia dónde se abre.
   - Si mueves o cambias el tamaño de la barra de tareas mientras SimonSays está en marcha, la sigue en unos segundos.
2. Para hablar, puedes:
   - Hacer clic en el cuadro de texto de la barra (o fijar la mirada en él): se abre un cuadro de entrada más grande junto a la barra de tareas. Escribe el texto y pulsa `Enter` para decirlo; el cuadro se cierra y el foco pasa a `Reproducir>`, así que `Espacio` lo vuelve a decir (o lo detiene). `Esc` cierra el cuadro sin hablar y te devuelve a la ventana que estabas usando; el texto se queda en la barra, donde `Reproducir>` lo vuelve a decir.
   - Hacer clic en una o varias frases guardadas en la ventana de categorías.
3. Durante la reproducción, el botón muestra `>` para indicar que está sonando. Puedes pulsarlo de nuevo para detenerlo al instante.

![La barra de SimonSays en la barra de tareas](img/bar.png)

Junto al botón `Categorías`, dos pequeños **botones de acceso rápido** dan acceso con un clic a dos cosas a las que, si no, llegarías con un atajo o con el menú de la bandeja:

- 📂 abre un tablero guardado en tu ordenador; es la misma importación que `F9`, pero empieza en tu carpeta de tableros (`%LocalAppData%\SimonSays\boards\`), donde se guardan los tableros exportados y descargados (consulta [Importar y exportar categorías](#importar-y-exportar-categorías)).
- ⚙️ abre la configuración, igual que `F2`.

Los dos pueden ocultarse con `Mostrar botones de acceso rápido y menús` en la configuración (`F2`), lo que deja más espacio para el cuadro de entrada. La misma opción desactiva también el menú contextual (clic derecho) de la ventana de categorías.

Si SimonSays está oculto, haz clic en su icono de la bandeja para volver a mostrarlo.

## Ventana de categorías
![La ventana de categorías con las frases predeterminadas](img/categories.png)

- Ábrela con el botón `Categorías`. Cambia su tamaño, o activa `Recordar tamaño de la ventana de categorías` en la configuración para conservarlo.
- Selecciona una categoría en la lista de arriba y frases en la lista de abajo.
- Un clic en una frase la dice automáticamente si `Decir la frase al seleccionarla` está activado; si no, la frase se carga en el cuadro de entrada principal sin hablar.
- Haz doble clic en una zona vacía o en el borde de la ventana para ajustar su tamaño de forma que quepan todas sus categorías y frases.
- Arrastra una zona vacía de la ventana para moverla, y pulsa `Esc` para ocultarla.
- Acerca y aleja el zoom con `F12` y `F11` (consulta [Atajos](#atajos)).
- Haz clic derecho en un botón o en una zona vacía (o pulsa `Mayús + F10` o la tecla de menú) para abrir un menú con `Pegar`, `Reducir`, `Ampliar`, `Restablecer zoom`, `Ayuda`, `Configuración`, `Mirada / Clic por fijación...`, `Ocultar` y `Salir de SimonSays...`, cada uno con su atajo. La primera entrada muestra el título del tablero (`Tablero sin título` si no tiene); elígela para ver los créditos del tablero, su idioma y cuántas categorías y frases contiene. El menú solo aparece mientras `Mostrar botones de acceso rápido y menús` está activado en la configuración.
- En un botón de categoría o de frase, el menú ofrece además `Editar...`, `Mover a la izquierda`, `Mover a la derecha`, `Agregar nueva categoría...` o `Agregar nueva frase...` y `Eliminar categoría...` o `Eliminar frase...` para ese botón, sin tener que seleccionarlo antes; en una categoría, `Exportar categoría...` pregunta si guardar solo esa categoría o todo el tablero. Los atajos `F4` a `F8` se muestran cuando el botón es tu selección actual, porque esas teclas actúan sobre la selección. En una zona vacía, el menú ofrece `Agregar nueva categoría...`, que añade una al final; también es la forma de empezar un tablero vacío.
- La franja entre las categorías y las frases muestra un recordatorio de los atajos principales (o el rótulo del tablero, si tiene uno).

![El menú contextual de un botón de frase](img/menu.png)

## Frases predeterminadas
SimonSays incluye un conjunto de categorías y frases listo para usar en cada uno de sus 18 idiomas, ordenado para que las frases que más se necesitan requieran menos selecciones:

- 👋🏻 Saludos Frecuentes
- 🗣️ Frases de Conversación
- 🚨 Frases de Emergencia
- 🤲 Cuidado y Confort
- 🏥 Salud
- 🏠 Familia y Hogar
- 👥 Social
- ⏰ Tiempo y Fecha
- 💼 Frases de Negocios
- ✈️ Frases de Viaje
- 🔊 Sonidos

Cada idioma tiene sus propias categorías y frases: cuando cambias el idioma en la configuración (`F2`), la ventana de categorías pasa al tablero de ese idioma, y los cambios que hagas en un idioma no afectan a los demás.

Las frases se dicen con las voces instaladas en Windows. Si no hay ninguna voz instalada para tu idioma, las frases se muestran igualmente, pero pueden decirse con otra voz (consulta [Solución de problemas](#solución-de-problemas)).

Puedes cambiar las frases predeterminadas o recuperarlas más adelante; consulta [Editar categorías y frases](#editar-categorías-y-frases) y [Restaurar las frases predeterminadas](#restaurar-las-frases-predeterminadas).

## Atajos
- `F1`: Abrir la ayuda
- `F2`: Abrir la configuración
- `F3`: Configuración de mirada / clic por fijación
- `Enter`: Decir el texto del cuadro de entrada
- `Esc`: Ocultar la ventana de categorías
- `F11` / `Ctrl -`: Alejar el zoom (ventana de categorías)
- `F12` / `Ctrl +`: Acercar el zoom (ventana de categorías)
- `Ctrl + 0`: Restablecer el zoom
- `Mayús + F10` o la tecla de menú: Abrir el menú de la ventana de categorías (igual que el clic derecho)
- `Espacio` sobre `Reproducir>`: Volver a decir el texto, o detenerlo mientras suena

Los atajos para editar categorías, frases y tableros se indican en [Editar categorías y frases](#editar-categorías-y-frases).

## Configuración (F2)
Abre la configuración (`F2`) para ajustar lo siguiente. La configuración tiene dos columnas.

![La ventana de configuración](img/settings.png)

**Columna izquierda**
- **Texto predeterminado**: activa o desactiva `Insertar texto predeterminado automáticamente` y personaliza la frase predeterminada.
  - `Mostrar teclado táctil`: abre el teclado táctil en pantalla cuando se abre el cuadro de entrada; se coloca al lado del cuadro de entrada, en el lado opuesto a la barra de tareas.
- `Mostrar botones de acceso rápido y menús`: muestra los botones 📂 (abrir un tablero) y ⚙️ (configuración) junto al botón `Categorías` de la ventana principal (consulta [Primeros pasos](#primeros-pasos)) y el menú contextual de la ventana de categorías (consulta [Ventana de categorías](#ventana-de-categorías)). Desactívala para ocultarlos todos y dar más espacio al cuadro de entrada.
- **Ventana de categorías**:
  - `Minimizar ventana de categorías automáticamente` después de seleccionar.
  - `Recordar tamaño de la ventana de categorías`.
  - `Autoajustar tamaño de la ventana de categorías`: ajusta la ventana de categorías a sus categorías y frases cada vez que se cargan; por ejemplo, al iniciar, al cambiar de idioma o después de una importación.
- `Mirada / Clic por fijación...`: abre la configuración de [Mirada / Clic por fijación](#mirada--clic-por-fijación-f3) encima de la configuración, que sigue abierta con tus cambios. Los cambios que hagas allí se guardan al pulsar `Aceptar` en esa ventana, aunque después pulses `Cancelar` en la configuración.

**Columna derecha**
- **Idioma**: elige el idioma de la interfaz (English, Español, العربية, Euskara, Català, 简体中文, Français, Galego, Deutsch, עברית, हिन्दी, Italiano, 日本語, 한국어, Português, Português (Brasil), Русский, Valencià). Cada idioma tiene también sus propias categorías y frases (consulta [Frases predeterminadas](#frases-predeterminadas)).
- **Voz**: elige una voz SAPI instalada que coincida con tu idioma.
- **Volumen/Velocidad**: ajusta `Volumen (10-100)` y `Velocidad (-10 a 10)`.
- `Probar voz`: escucha la voz seleccionada (a través del dispositivo elegido en `Salida de sonido`).
- `Salida de sonido`: por dónde reproduce SimonSays su voz y sus sonidos: `Predeterminado de Windows` (lo habitual) o cualquier dispositivo conectado. Los cables de audio virtuales aparecen primero, que es lo que necesitas para las [reuniones en línea](#usar-simonsays-en-reuniones-en-línea). Si el dispositivo elegido no está conectado, SimonSays suena por el dispositivo predeterminado de Windows y te avisa una vez; la elección se conserva, se muestra como `(no conectado)` y se vuelve a usar en cuanto el dispositivo vuelve.
- `Ajustes de escucha...`: abre los dispositivos de grabación de Windows, donde puedes activar **Escuchar este dispositivo** para un cable virtual (consulta [Usar SimonSays en reuniones en línea](#usar-simonsays-en-reuniones-en-línea)).
- **Comportamiento de reproducción**:
  - `Decir la frase al seleccionarla` (decir automáticamente con un clic).
  - `Detener reproducción anterior`: detiene automáticamente cualquier reproducción en curso antes de empezar una frase nueva.
  - `Aumentar volumen de SimonSays al reproducir`: sube el volumen del dispositivo por el que suena SimonSays (consulta `Salida de sonido`) mientras habla y restaura el nivel anterior cuando termina.
  - `Reducir temporalmente otros sonidos al reproducir`: baja el volumen de las demás aplicaciones mientras habla y lo restaura cuando termina.

**Abajo**
- `Ayuda` (esquina inferior izquierda, o `F1` con la configuración abierta) abre esta ayuda en el idioma seleccionado en la lista `Idioma`; la configuración sigue abierta con tus cambios.
- `Aceptar` guarda los cambios; `Cancelar` los descarta.

De forma predeterminada, `Decir la frase al seleccionarla`, `Minimizar ventana de categorías automáticamente`, `Autoajustar tamaño de la ventana de categorías`, `Aumentar volumen de SimonSays al reproducir` y `Reducir temporalmente otros sonidos al reproducir` están activados, y `Detener reproducción anterior` está desactivado.

## Mirada / Clic por fijación (F3)
Activa frases y botones sin usar las manos manteniendo la mirada (o el cursor del ratón) sobre ellos durante un tiempo determinado. Esta función es nueva: necesita pruebas y comentarios; cuéntanos cómo funciona con tu seguidor mediante la opción `Danos tu opinión` del menú de la bandeja.

Configuraciones compatibles:
- **Cualquier software de control ocular que mueva el cursor del ratón** (Irisbond EasyClick, Tobii Dynavox Computer Control, los modos de cursor del Control ocular de Windows, OptiKey): el clic por fijación sigue al cursor.
- **Lectura directa de la mirada (sin necesidad de mover el cursor)**: Irisbond Hiru (modo HID) y los seguidores Tobii a través de su propio software; verificado con Tobii Eye Tracker 4C, Tobii Eye Tracker 5 y Tobii Dynavox PCEye 5.
- **Tobii Eye Tracker 5**: su software actual solo instala un motor de 64 bits, así que SimonSays lo lee a través de un pequeño programa auxiliar, `SimonSaysGaze64.exe`, instalado junto a él. El auxiliar solo se inicia cuando hace falta, no tiene ventana, se cierra con SimonSays y mantiene la mirada en tu ordenador: nunca se guarda ni se envía a ningún sitio.
- **Licencia de Tobii Eye Tracker 4C y Eye Tracker 5**: la licencia de usuario final de Tobii para estos seguidores no permite el uso médico, incluido el uso en hospitales y otros entornos sanitarios. Los dispositivos de Tobii Dynavox, como el PCEye 5, tienen sus propias condiciones de licencia.
- **Software de Tobii Eye Tracker 4C**: SimonSays lee el 4C a través de Tobii Core. Si conectas un Eye Tracker 5 al mismo ordenador, Windows instala Tobii Experience, que desactiva Tobii Core, y el 4C deja de funcionar. Para volver a usar el 4C, reinstala Tobii Core; la versión recomendada es `Tobii_Eye_Tracking_Core_v2.13.1.7294_x86.exe`, de la [página de primeros pasos de Tobii](https://gaming.tobii.com/getstarted/).

Dónde funciona el clic por fijación:
- La barra de SimonSays, la ventana de categorías y sus botones de frases y categorías.
- Los botones de las ventanas y preguntas de SimonSays: la configuración, la ventana de Mirada / Clic por fijación, los diálogos de agregar, editar y vista previa, y todas las confirmaciones, como `Sí` / `No` o `Aceptar` / `Cancelar`. Estos botones están en tu idioma y empiezan a responder a la fijación unos tres cuartos de segundo después de abrirse la ventana, para que una pregunta que aparece justo donde estás mirando no pueda responderse sola.
- Cuando se lee la mirada directamente, el botón que estás mirando se ilumina igual que bajo el ratón, así que puedes ver lo que vas a seleccionar antes de que empiece el progreso. Un parpadeo mantiene el resaltado; apartar la mirada lo quita.

![La ventana de Mirada / Clic por fijación](img/dwell.png)

Abre la configuración de Mirada / Clic por fijación (`F3`, el menú de la bandeja o el botón `Mirada / Clic por fijación...` de la configuración) para ajustar:
- **Modo de activación**: elige una de estas opciones:
  - `Automático`: detecta los dispositivos presentes y activa el clic por fijación con el cursor del ratón o con la detección de la mirada, según corresponda. El software de Tobii en segundo plano (siempre en marcha una vez instalado) no basta para activar la fijación con el cursor; un seguidor Tobii solo se usa mientras está siguiendo la mirada de verdad.
  - `Cursor del ratón`: activa siempre el clic por fijación con el cursor del ratón.
  - `Seguidor ocular HID`: activa siempre el clic por fijación con la detección de la mirada (Irisbond Hiru en modo HID, o seguidores Tobii). Solo tu mirada activa los botones: mientras no llega ninguna mirada (seguimiento apagado o seguidor desconectado), no se activa nada por fijación y el ratón funciona con normalidad.
  - `Desactivado`: desactiva siempre el clic por fijación.
  - **Retardo de enganche**: cuánto tiempo, en milisegundos, tiene que posarse tu mirada (o el cursor) sobre un botón antes de que empiece la barra de progreso (250 de forma predeterminada). Así, al pasar la mirada por encima de los botones no se ve ningún progreso; el botón se activa tras el retardo de enganche más el tiempo de fijación. Ponlo a 0 para que el progreso empiece de inmediato.
  - **Tiempo de fijación**: la duración (en milisegundos) necesaria para hacer clic al mirar un elemento (500 de forma predeterminada). Mientras dura la fijación se muestra un indicador de progreso.
  - **Radio de tolerancia**: la tolerancia de la mirada (en píxeles) que permite pequeños movimientos del ojo sin dejar de contar la fijación.
  - **Tiempo de espera**: milisegundos después de una activación durante los que el clic por fijación no puede volver a activarse (evita clics repetidos por accidente).
  - **Color de progreso**: el color del indicador de progreso de la fijación (de forma predeterminada, el color de énfasis de Windows).
  - **Apartarse antes de que la fijación vuelva a activar** (activado de forma predeterminada): después de un clic por fijación, ningún botón puede volver a activarse por fijación hasta que el puntero o la mirada se alejen de ese punto (más allá del radio de tolerancia) o apartes la vista un momento. Quedarse sobre un botón lo activa una sola vez, así que una frase larga no se repite antes de terminar. Los clics del ratón no se ven afectados.
  - **Detectar cómo usas el seguidor**: dos botones que ayudan a la aplicación a detectar si mueves el cursor del ratón con un seguidor ocular o si simplemente haces clic en los botones.
  - **Detectado**: muestra el tipo de dispositivo detectado y más información sobre el software relacionado.
  - `Guardar informe de diagnóstico...` (debajo de Detectado): escribe un informe sobre tu seguidor ocular y su software en `hid_dump.txt` y abre su carpeta, para que puedas enviarlo al soporte.
  - Un botón `Restablecer` (abajo a la izquierda) devuelve todos los ajustes de fijación a sus valores predeterminados.
  - Un botón `Aceptar` guarda los cambios; `Cancelar` los descarta.

## Usar SimonSays en reuniones en línea
SimonSays puede hablar directamente en una videollamada (Teams, Zoom, Google Meet…), de modo que los demás participantes oyen tus frases como si salieran de tu micrófono. Necesitas un cable de audio virtual, como el gratuito [VB-Audio Virtual Cable](https://vb-audio.com/Cable/):

1. Instala el cable de audio virtual y reinicia Windows si te lo pide.
2. Envía el sonido de SimonSays al cable: abre la configuración (`F2`) y elige `CABLE Input` en `Salida de sonido` (los cables virtuales aparecen primero). Pulsa `Probar voz` para comprobarlo: ya no deberías oírlo por los altavoces, porque ahora va al cable.
3. En tu aplicación de reuniones, elige `CABLE Output` como micrófono.
4. Para oír también tú a SimonSays, pulsa `Ajustes de escucha...` en la configuración. Se abren los dispositivos de grabación de Windows: selecciona `CABLE Output`, abre **Propiedades → Escuchar**, marca **Escuchar este dispositivo**, elige tus altavoces o auriculares en **Reproducir a través de este dispositivo** y pulsa `Aceptar`. Solo tienes que hacerlo una vez.

SimonSays no puede activar **Escuchar este dispositivo** por ti: es un ajuste del lado de grabación del cable que Windows solo te deja cambiar a ti, así que `Ajustes de escucha...` te lleva directamente a la ventana correcta.

Conviene saber:
- Deja el dispositivo de reproducción predeterminado de Windows en tus altavoces o auriculares, no en `CABLE Input`. Si **Escuchar este dispositivo** reproduce en "Dispositivo de reproducción predeterminado" mientras ese dispositivo predeterminado es el cable, el sonido vuelve a entrar en el cable.
- Si en la reunión se oyen tus frases pero no los efectos de sonido (aplausos, un timbre), la supresión de ruido de la aplicación de reuniones los está eliminando; baja o desactiva la supresión de ruido en la aplicación.
- Si el cable se desinstala o se desconecta, SimonSays suena por el dispositivo predeterminado de Windows y te avisa una vez; vuelve al cable en cuanto esté disponible de nuevo.
- Para tener frases preparadas para una llamada, añade la categoría **Plantilla de reunión**: pulsa `F9` o el botón 📂 y abre el tablero con el nombre en tu idioma (`SimonSays_Plantilla_de_reunión_(Español).ssc` en español). Tiene saludos, "¿Me oyes?", una breve explicación de que hablas con un programa, sí / no, de acuerdo / en desacuerdo, pedir que repitan o hablen más despacio, y despedidas. Edítala para añadir frases para tu propia reunión.
- Como alternativa al paso 2, puedes seguir dirigiendo SimonSays con el ajuste por aplicación de Windows (**Configuración → Sistema → Sonido → Mezclador de volumen**), dejando `Salida de sonido` en `Predeterminado de Windows`. SimonSays solo aparece en esa lista después de haber reproducido algo.

## Menú del icono de la bandeja
- `Mostrar` / `Ocultar` la interfaz principal
- `Configuración`
- `Mirada / Clic por fijación...`
- `Web` abre la página web de SimonSays
- `Acerca de` muestra la versión y los créditos
- `Danos tu opinión` abre un [formulario de comentarios](https://forms.gle/KMbpEDYmwnFJUhgy8) en el navegador predeterminado
- `Salir` cierra SimonSays, después de preguntar

Puedes mover la ventana emergente de la bandeja y otros diálogos arrastrando su barra de título; cambia el tamaño de la ventana de categorías arrastrando sus bordes, y activa `Recordar tamaño de la ventana de categorías` en la configuración para conservarlo.

## Editar categorías y frases
Esta sección y las siguientes son para cuidadores, logopedas y usuarios avanzados que cambian las categorías, frases, sonidos y aspecto de un tablero. Cada cambio se guarda al instante (consulta [Deshacer cambios](#deshacer-cambios)).

### Qué editor usar
- **En SimonSays**: cambios del día a día; cambiar el nombre, añadir, mover o eliminar categorías y frases, ponerles un emoji, añadir sonidos, pegar una lista de frases o ajustar un color. El resto de esta sección explica cómo.
- **En [SimonSays Web](https://github.com/juan-rey/simonsays-web)**: el trabajo grande con tableros, que allí es mucho más fácil: tableros hechos con pictogramas de CAA (puede buscar en la biblioteca de pictogramas de ARASAAC), imágenes de icono personalizadas y tableros con estilo, con colores, tamaños y disposiciones. Se recomienda especialmente para diseñar un tablero **Fitzgerald Key**, en el que cada grupo de palabras tiene su propio color. Es un editor visual que abre y guarda tableros `.ssc`/`.ssz` directamente en el navegador, sin subirlos; cuando termines, importa el tablero en SimonSays con `F9` o con el botón 📂. También es la forma de usar los tableros de SimonSays en otras plataformas.

Atajos de edición (ventana de categorías):
- `F4`: Editar la selección actual
- `F5` / `F6`: Ir a la selección anterior / siguiente en las listas
- `F7`: Añadir después de la selección actual (categoría o frase, según el contexto)
- `F8`: Eliminar la selección actual
- `F9`: Importar categorías
- `F10`: Exportar categorías
- `Ctrl + F4`: Editar el estilo del tablero (consulta [Personalizar el aspecto](#personalizar-el-aspecto-estilos-de-tablero-y-de-categoría))
- `Ctrl + F8`: Eliminar todas las categorías y sus frases (pide confirmación dos veces)
- `Ctrl + V` o `Mayús + Insert`: Añadir el texto copiado como frases, una por párrafo (consulta [Gestionar frases](#gestionar-frases))
- Arrastrar un archivo `.txt`, `.ssc` o `.ssz` a la ventana: añade un `.txt` como categoría nueva, o importa un tablero igual que con `F9` (consulta [Gestionar categorías](#gestionar-categorías))

> **Consejo:** exporta tu tablero (`F10`) antes de usar `Ctrl + F8` o de importar un tablero completo (`F9`), para poder recuperarlo.

### Gestionar categorías
- Cambiar el nombre: selecciona la categoría → (atajo: `F4`) `Cambiar nombre de categoría` → cambia el nombre → `Aceptar`.
- Mover la selección: `F5`/`F6` para ir al elemento anterior/siguiente al recorrer las listas.
- Añadir: selecciona una categoría → (atajo: `F7`) diálogo `Agregar nueva categoría` → escribe el nombre → `Aceptar`.
- Eliminar: selecciona la categoría → (atajo: `F8`) `Eliminar categoría`; confirma que quieres eliminar la categoría y sus frases.
- Añadir desde un archivo de texto: arrastra un archivo `.txt` a la ventana de categorías → se crea una categoría con el nombre del archivo y una frase por párrafo (los párrafos se separan con una línea en blanco) → revisa la vista previa → `Aceptar`. Puedes soltar varios archivos a la vez; cada uno tiene su propia vista previa. Se rechaza un archivo cuyo nombre coincida con una categoría existente.
- Abrir un tablero arrastrándolo: arrastra un archivo `.ssc` o `.ssz` a la ventana de categorías → SimonSays pregunta si quieres importarlo; con `Sí` se importa igual que con `F9` (consulta [Importar y exportar categorías](#importar-y-exportar-categorías)). Si el archivo contiene un tablero completo, sustituye al actual, y antes se guarda una copia del tablero actual como `backup.ssz`. Puedes soltar a la vez archivos de texto y de tablero; cada uno se procesa por turno. Abrir un archivo `.ssc` o `.ssz` desde el Explorador de archivos (doble clic) hace la misma pregunta.

#### Poner o cambiar el icono de una categoría
Cada categoría puede mostrar un emoji o un archivo de imagen personalizado (`.ico`, `.png` o `.jpg`) en su botón. Indica el icono en el diálogo de agregar o editar con el prefijo separador `##`:

```
<icono>##<nombre de la categoría>
```

- **Emoji**: escribe el emoji (o los emojis) seguido de `##` y el nombre.
  - Ejemplo: `🙂##Saludos`
- **Archivo de icono** (`.ico`, `.png`, `.jpg`): escribe el nombre del archivo o la ruta completa seguidos de `##` y el nombre. Un nombre de archivo sin carpeta se busca como se describe en [Orden de búsqueda de archivos de sonido e icono](#orden-de-búsqueda-de-archivos-de-sonido-e-icono).
  - Ejemplo: `C:\icons\smile.png##Saludos`
- **Sin icono**: escribe solo el nombre de la categoría, sin prefijo `##`.
  - Ejemplo: `Saludos`

> Nota: para los iconos de archivo solo se admiten archivos `.ico`, `.png` y `.jpg`. Las demás extensiones se ignoran y no se muestra ningún icono. Las imágenes PNG conservan su transparencia; las imágenes muy grandes se reducen automáticamente al cargarlas.

### Gestionar frases
- Editar: selecciona la frase → (atajo: `F4`) `Editar frase` → actualiza el texto → `Aceptar`.
- Mover la selección: `F5`/`F6` para ir a la frase anterior/siguiente.
- Añadir: selecciona una frase → (atajo: `F7`) `Agregar nueva frase` → escribe el texto → `Aceptar`.
- Eliminar: selecciona la frase → (atajo: `F8`) `Eliminar frase`; confirma.
- Añadir varias a la vez: copia un texto con las frases separadas por una línea en blanco → selecciona una frase (o una categoría) → `Ctrl + V` → revisa la vista previa → `Aceptar`. Las frases se añaden después de la frase seleccionada, o al final de la categoría. Los saltos de línea dentro de un párrafo se convierten en espacios, y los marcadores de icono `##` y de audio `::` funcionan igual que en el diálogo `Agregar nueva frase`.
- Añadir sonidos: arrastra uno o varios archivos `.wav` o `.mp3` a la ventana de categorías → revisa la vista previa → `Aceptar`. Cada archivo se convierte en una frase de sonido con el nombre del archivo, añadida después de la frase seleccionada (o al final de la categoría seleccionada), y el archivo se copia en la carpeta de recursos del tablero para que viaje con él cuando lo exportes. Nunca se sobrescribe un archivo existente: si ya hay un sonido distinto con el mismo nombre, el nuevo se guarda como `nombre (2).wav`.

#### Poner o cambiar el icono de una frase
Las frases siguen la misma convención de prefijo de icono que las categorías, con el separador opcional `##` delante del texto de la frase:

```
<icono>##<texto de la frase>
```

- **Emoji**: `👍##¡Me parece bien!`
- **Archivo de icono**: `C:\icons\check.png##¡Me parece bien!`
- **Sin icono**: `¡Me parece bien!`

Las frases pueden incluir un archivo de audio (consulta [Mezclar voz y sonidos](#mezclar-voz-y-sonidos)). Para combinar icono, texto y audio, usa el formato:

```
<icono>##<texto de la frase>::<archivo de audio>
```

- Ejemplo: `🔔##Atención::alert.mp3`

El prefijo `##` y el sufijo `::` son opcionales e independientes; puedes usar cualquier combinación:
- Solo prefijo de icono: `🔔##Atención`
- Solo sufijo de audio: `Atención::alert.mp3`
- Ambos: `🔔##Atención::alert.mp3`
- Ninguno: `Atención`

## Mezclar voz y sonidos
- Usa el delimitador `♫` para insertar nombres de archivos de audio en el texto:
  - Ejemplo: `Hola ♫doorbell.mp3♫, espera un momento.`
  - El texto fuera de los delimitadores se dice; el texto de dentro se trata como un archivo de sonido y se reproduce en ese punto.
- Audio admitido: `.wav` y `.mp3`. Los archivos MIDI (`.mid`, `.midi`) se aceptan en una frase, pero no suenan; conviértelos a `.wav` o `.mp3`.
- Usa rutas absolutas o relativas a las que el programa pueda acceder. Asegúrate de que los delimitadores van en pareja, para que el texto final no se trate como audio.
- Para escribir `♫`, cópialo de esta ayuda, o selecciona una frase que tenga un sonido: su marcador `♫…♫` se coloca en el cuadro de entrada, donde puedes añadir texto alrededor.

### Sonidos incluidos con SimonSays
Estos sonidos se instalan con SimonSays y pueden usarse por su nombre en cualquier texto o frase; por ejemplo, `♫laugh.mp3♫` en el cuadro de entrada, o `¡Bien hecho!::applause.mp3` para un botón de frase con el rótulo "¡Bien hecho!" que reproduce aplausos:

`ahem.mp3`, `alert.mp3`, `applause.mp3`, `aww.mp3`, `beep.mp3`, `boo.mp3`, `call_bell.mp3`, `censor.mp3`, `doorbell.mp3`, `drumroll.mp3`, `evil_laugh.mp3`, `fail.mp3`, `hmmmm.mp3`, `kisses.mp3`, `laugh.mp3`, `mmm-hmm.mp3`, `nuh-uh.mp3`, `phone_ring.mp3`, `rimshot.mp3`, `scream.mp3`, `shots.mp3`, `sigh.mp3`

### Orden de búsqueda de archivos de sonido e icono
Cuando el nombre de un archivo de sonido o de icono no incluye una letra de unidad ni una ruta completa, SimonSays lo busca en este orden y usa la primera coincidencia:

1. **La subcarpeta de recursos del tablero actual**: si el estilo del tablero define `title` o `resource-folder`, su propia subcarpeta dentro de la carpeta de recursos (consulta [Personalizar el aspecto](#personalizar-el-aspecto-estilos-de-tablero-y-de-categoría)).
2. **Carpeta de recursos**: `%LocalAppData%\SimonSays\resources\` (el lugar predeterminado para los iconos y sonidos de los tableros; los paquetes `.ssz` importados instalan aquí sus archivos).
3. **Carpeta de datos de SimonSays**: `%LocalAppData%\SimonSays\` (se mantiene como alternativa para los archivos que versiones anteriores dejaban aquí, antes de que existiera la carpeta de recursos).
4. **Directorio de trabajo**: el directorio de trabajo actual del proceso (solo se busca si es distinto de la carpeta del ejecutable).
5. **Carpeta del ejecutable**: la carpeta que contiene `SimonSays.exe`.

Si el archivo no se encuentra en ninguna de estas ubicaciones, se usa el sonido de reserva integrado (un icono simplemente no se muestra).

> **Consejo:** te recomendamos colocar tus archivos de sonido e icono en `%LocalAppData%\SimonSays\resources\`; sobreviven a las actualizaciones de la aplicación y son propios de cada usuario. Un tablero con un estilo `title` o `resource-folder` tiene allí automáticamente su propia subcarpeta, lo que separa sus archivos de los de otros tableros.

## Personalizar el aspecto (estilos de tablero y de categoría)
Puedes cambiar los colores, el tamaño y el estilo de la ventana de categorías y de sus botones; por ejemplo, para crear tableros de comunicación con códigos de color como Fitzgerald Key. Los estilos son opcionales: sin ellos, la ventana mantiene su aspecto predeterminado.

> **Consejo:** todo lo que sigue puede escribirse en SimonSays, lo que resulta práctico para pequeños retoques. Para diseñar un tablero entero con estilo o con pictogramas, se recomienda [SimonSays Web](https://github.com/juan-rey/simonsays-web): ves el resultado mientras trabajas y escribe estos estilos por ti (consulta [Qué editor usar](#qué-editor-usar)).

Un estilo es una lista de pares `propiedad:valor;`. Los valores pueden ser:
- **Colores**: hexadecimal `#RRGGBB`, por ejemplo `#FFD966`.
- **Tamaños**: un número de píxeles (`96`) o un porcentaje del predeterminado (`120%`).
- **Palabras clave**: por ejemplo `left`, `right`, `top`, `bottom`, `smart`, `normal`, `bold`.
- **Texto**: un nombre de fuente, o el texto del rótulo, del título o de los créditos.

Todo lo que la aplicación no reconoce (una propiedad mal escrita o un valor incorrecto) simplemente se ignora, así que un estilo nunca estropea la ventana.

### Dar estilo a una categoría (diálogo Editar)
Selecciona una categoría y pulsa `F4` (Editar); después añade `::` y un estilo tras el nombre:

```
<icono>##<nombre de la categoría>::<propiedad>:<valor>;<propiedad>:<valor>;
```

- Ejemplo: `🧍##Personas::background:#FFD966;` pone en amarillo el botón Personas. El color de la categoría **se aplica también a las frases que contiene**, así que una sola línea colorea todo un grupo de palabras: la base de un tablero Fitzgerald Key.
- Un estilo de categoría puede definir `background` y `text-color` para el botón de la categoría, y cualquier propiedad de frase (ver más abajo) con el prefijo `phrase-`, por ejemplo `phrase-corner-radius:8;`.

### Dar estilo a todo el tablero (`$$board`)
El aspecto general (el fondo de la ventana, los separadores y los valores predeterminados de todos los botones de categoría y de frase) es un **estilo de tablero**. Se guarda con el nombre de categoría reservado `$$board` (no puedes usar nombres con `$$` para tus propias categorías).

- **Edítalo** pulsando `Ctrl + F4` en la ventana de categorías. El editor muestra un `propiedad:valor;` por línea. Al confirmar con `Aceptar`, se aplica el nuevo estilo y SimonSays pregunta "¿Quieres revertir los cambios?": elige `Aceptar` para volver al estilo anterior, o `Cancelar` para conservar el nuevo.
- **Consigue uno** importando un archivo de categorías con estilo (`.ssc` / `.ssz`). Cuando importas un archivo que contiene un estilo de tablero, SimonSays pregunta antes de sustituir el tuyo y muestra el `title` y los `credits` del tablero (si los tiene) en el mensaje de importación. Un archivo cuyo estilo de tablero solo define `title`, `credits`, `language` o `resource-folder` no cambia nada visible, así que SimonSays no pregunta: si contiene una sola categoría, se añade y tu aspecto se mantiene; si contiene un tablero completo, sustituye a tu tablero, aspecto incluido, como cualquier tablero completo.

### Propiedades de estilo
- **Toda la ventana**: `background`, `separator-color`, `text-color`, `font-name`, `font-size`, `text-weight`, `caption` (rótulo que se muestra entre los separadores), `title` y `credits` (se muestran al importar el tablero), `language` (el idioma para el que se hizo el tablero, por ejemplo `language:Spanish`; SimonSays no importa el tablero mientras el idioma configurado sea otro, y te dice a qué idioma cambiar en la configuración, `F2`).
- **Todos los botones de categoría** (prefijo `category-`) y **todos los botones de frase** (prefijo `phrase-`): `background`, `text-color`, `width`, `height`, `corner-radius`, `border-width`, `margin`, `icon-position` (`left`/`right`/`top`/`bottom`/`center`), `icon-size`, `font-name`, `font-size`, `text-weight`, `text-layout`.

Con `icon-position` en `center`, el icono se coloca en el centro del botón y el texto se oculta (una tecla solo con icono); usa `top` si quieres el icono arriba y el nombre debajo. Si quieres texto sobre un icono centrado, añade un `text-layout` explícito (por ejemplo `text-layout:bottom`) y se mostrará en esa posición.

`text-layout` admite una palabra clave horizontal (`left`/`center`/`right`) y/o una vertical (`top`/`middle`/`bottom`), o `smart`. El valor predeterminado es smart: el texto se centra en el espacio que deja libre el icono (icono arriba → rótulo centrado debajo; centrado en todo el botón cuando no hay icono).

### Ejemplo: un tablero Fitzgerald Key con códigos de color
Para colorear seis grupos de palabras, edita cada categoría (`F4`) y dale un color de fondo. Estas son las cadenas exactas que escribes en el diálogo de edición:

```
🧍##Personas::background:#FFD966;
🏃##Acciones::background:#93C47D;
📏##Descriptivos::background:#6FABDC;
🍎##Sustantivos::background:#F6B26B;
🕒##Miscelánea::background:#FFFFFF;
👋##Social::background:#B4A7D6;
```

Cada color colorea también las frases de esa categoría, así que todo el tablero queda codificado por colores. Los retoques de todo el tablero (como poner el emoji de cada categoría arriba con su nombre debajo, y colorear los separadores) vienen de un estilo de tablero (`Ctrl + F4`) como:

```
category-icon-position:top;separator-color:#B4A7D6;
```

Para un tablero Fitzgerald Key completo, con pictogramas en cada tecla y las frases organizadas por grupos de palabras, créalo en [SimonSays Web](https://github.com/juan-rey/simonsays-web) e impórtalo con `F9`.

Los estilos viajan con tus categorías: al exportar (`F10`) se incluyen, y al importar (`F9`) se traen, así que un tablero con estilo puede compartirse como un único archivo.

## Importar y exportar categorías
Los tableros se comparten como archivos `.ssc` (categorías y frases) o `.ssz` (lo mismo, más los archivos de sonido e icono que usan, en un único archivo). También puedes crearlos y editarlos visualmente en el navegador con [SimonSays Web](https://github.com/juan-rey/simonsays-web).

- **Exportar** (`F10`): cuando se te pregunte, elige exportar todas las categorías o solo la seleccionada; el diálogo de guardar se abre de forma predeterminada en `%LocalAppData%\SimonSays\boards\`, aunque puedes guardar en cualquier sitio. SimonSays guarda automáticamente un `.ssz` cuando tus categorías o frases usan archivos de sonido o de icono que puede encontrar (los sonidos `.mid`/`.midi` no se incluyen). Exportar todas las categorías incluye el estilo del tablero; exportar una categoría incluye solo el estilo de esa categoría. Los mensajes de éxito o error aparecen en el idioma seleccionado.
- **Importar** (`F9`): elige un archivo `.ssc` o `.ssz`.
  - Un archivo con **varias categorías o con un estilo de tablero** es un tablero completo: **sustituye todas las categorías y frases del idioma actual**. Antes, tu tablero anterior se guarda como `backup.ssz` (consulta [Deshacer cambios](#deshacer-cambios)).
  - Un archivo con **una sola categoría** se añade a tu tablero; si ya existe una categoría con el mismo nombre, se te preguntará si quieres sobrescribirla. Lo mismo ocurre cuando el archivo solo da nombre a su tablero (`title`, `credits` o `language`, sin colores, tamaños ni fuentes): esos datos nunca sustituyen tu tablero ni su aspecto.
  - Los archivos de sonido e icono de un `.ssz` se instalan en la carpeta de recursos, y la ventana de categorías ajusta su tamaño al tablero importado. Los mensajes de éxito o error aparecen en el idioma seleccionado.

## Deshacer cambios
SimonSays guarda cada cambio al instante y no tiene un comando general de Deshacer. Estas son las formas de volver atrás:

- **Deshacer una importación**: antes de que una importación sustituya tu tablero, SimonSays guarda el tablero anterior como `backup.ssz` en `%LocalAppData%\SimonSays\boards\`. Para deshacer la importación, pulsa `F9` y elige `backup.ssz`: tu tablero anterior vuelve tal como estaba, categorías y aspecto, sin más preguntas. Solo se conserva el último tablero sustituido: cada nueva importación de un tablero completo sobrescribe la copia.
- **Deshacer un cambio del estilo del tablero**: después de editar el estilo del tablero con `Ctrl + F4`, SimonSays pregunta "¿Quieres revertir los cambios?"; elige `Aceptar` para volver al estilo anterior.
- **Deshacer un cambio de categoría o de frase**: los cambios hechos con `F4`, `F7` o `F8` no pueden deshacerse automáticamente. Antes de hacer cambios grandes en un tablero que te importa, expórtalo (`F10`); importar ese archivo más adelante (`F9`) te devuelve a ese punto.

### Restaurar las frases predeterminadas
SimonSays nunca sobrescribe tus propias categorías y frases, así que las nuevas frases predeterminadas de una actualización solo aparecen en los idiomas que aún no has usado. Puedes volver a las categorías y frases integradas en cualquier momento; por ejemplo, para empezar de nuevo o para recibir las frases nuevas de una actualización. Esto **sustituye todo el tablero del idioma actual**:

1. Haz clic en el botón 📂 junto a `Categorías` (o pulsa `F9` en la ventana de categorías). Se abre la carpeta de tableros.
2. Elige el tablero de frases predeterminadas de tu idioma; en español, `SimonSays_frases_predeterminadas_(Español).ssc` (en los demás idiomas tiene la misma forma, en ese idioma; por ejemplo, `SimonSays_default_phrases_(English).ssc`).
3. Confirma la importación. Antes, tu tablero actual se guarda como `backup.ssz`, así que todavía puedes recuperarlo (consulta [Deshacer cambios](#deshacer-cambios)).

Este tablero contiene siempre las frases predeterminadas de la versión de SimonSays que estás usando: SimonSays lo actualiza en la carpeta de tableros al iniciarse y al cambiar de idioma. Si quieres conservar tu propio tablero, expórtalo antes (`F10`) con el nombre que prefieras; `backup.ssz` se sobrescribe en la siguiente importación.

Si el archivo no está en la carpeta de tableros, queda un último recurso: cierra SimonSays (menú de la bandeja → `Salir`), abre el Editor del Registro (`regedit`), elimina la clave con el nombre de tu idioma en inglés (por ejemplo `Spanish`) en `HKEY_CURRENT_USER\SOFTWARE\SimonSays\Phrases\` y vuelve a iniciar SimonSays. Las categorías y frases predeterminadas de ese idioma se crean de nuevo, sin copia de seguridad.

Para `Portuguese (Brazil)`: si también existe una clave `Portuguese`, SimonSays copia ese tablero en `Portuguese (Brazil)` en lugar de crear las frases predeterminadas brasileñas, así que elimina también la clave `Portuguese` (expórtala antes si la usas).

## Dónde guarda SimonSays tus archivos
- **Las categorías, las frases y la configuración** se guardan en el registro de Windows, en `HKEY_CURRENT_USER\SOFTWARE\SimonSays` (el tablero de cada idioma, en `Phrases\<Language>`).
- `%LocalAppData%\SimonSays\resources\`: tus archivos de sonido e icono, y los instalados desde tableros `.ssz`; un tablero cuyo estilo define `title` o `resource-folder` tiene su propia subcarpeta.
- `%LocalAppData%\SimonSays\boards\`: la carpeta predeterminada para los tableros exportados, la copia automática `backup.ssz` y los tableros que vienen con SimonSays: las frases predeterminadas de cada idioma, y categorías listas para usar que puedes añadir a tu tablero con `F9` o con el botón 📂: Sí / No, Dolor y cuerpo, Deletrear, Juegos, Preguntas, Cuidado diario, una Plantilla de reunión y una Prueba de audición, con el nombre en tu idioma, y `Spelling_NATO`, `Alert_tones` y `Morse_code` para cualquier idioma. Cada una añade una categoría sin sustituir tu tablero. Los tableros de tu idioma se copian allí al iniciar y cuando cambias el idioma en la configuración, siempre que el tablero predeterminado de tu idioma falte o haya cambiado, y sustituyen las copias de la carpeta; por eso, guarda con otro nombre los tableros que edites.
- `%LocalAppData%\SimonSays\debug\`: archivos de diagnóstico (como `hid_dump.txt`) que el soporte puede pedirte.

Para abrir una de estas carpetas, pega su ruta (por ejemplo `%LocalAppData%\SimonSays`) en la barra de direcciones del Explorador de archivos.

## Solución de problemas
- **Falla la inicialización de la voz**: comprueba que SAPI está instalado y que hay una voz compatible.
- **No hay voz para mi idioma, o las frases se dicen con otro acento**: SimonSays usa las voces instaladas en Windows. Añade una voz para tu idioma en la configuración de Windows (**Hora e idioma → Voz → Administrar voces**) y elígela después en la configuración de SimonSays (`F2`). Puede que algunas voces "naturales" más recientes de Windows no estén disponibles para otras aplicaciones.
- **No se oyen los sonidos**: comprueba la ruta y la extensión del archivo, y que los delimitadores `♫` van en pareja.
- **La barra de tareas ya no se oculta automáticamente con SimonSays abierto**: es intencionado. SimonSays está en la barra de tareas, así que la mantiene visible mientras funciona y vuelve a activar "Ocultar automáticamente la barra de tareas" cuando lo cierras. Si SimonSays se cerró de forma inesperada (por ejemplo, por un corte de luz), se vuelve a activar la próxima vez que cierres SimonSays.
- **El botón Widgets tapa la barra de SimonSays** (Windows 11): desactiva Widgets en **Configuración → Personalización → Barra de tareas** de Windows. SimonSays ofrece abrir esa página una vez, la primera vez que detecta Widgets.
- **La ventana de categorías es demasiado pequeña o demasiado grande**: haz doble clic en una zona vacía o en el borde de la ventana para ajustarla a su contenido, o usa el zoom con `F11` / `F12`.
- **Han desaparecido mis categorías o frases**: comprueba el idioma en la configuración (`F2`); cada idioma tiene su propio tablero. Si una importación sustituyó tu tablero, consulta [Deshacer cambios](#deshacer-cambios).
- **"SimonSays no ha podido guardar tus categorías y frases"**: tu cambio se sigue mostrando, pero puede perderse al cerrar SimonSays. Exporta tu tablero con `F10` enseguida para conservar una copia; si se repite, comunícalo. El mensaje aparece una sola vez, no en cada cambio, para que nunca estorbe el uso del tablero.
- **Errores de importación o exportación**: revisa los permisos del archivo y la ubicación en el disco; vuelve a intentarlo.
- **No se aplican los cambios de idioma o de voz**: vuelve a abrir la configuración y confirma la selección. Si los cambios siguen sin aplicarse, reinicia SimonSays y vuelve a intentarlo.
- **Informar de problemas con el seguidor ocular**: si el soporte te pide un diagnóstico, abre la ventana de Mirada / Clic por fijación (`F3`) y haz clic en `Guardar informe de diagnóstico...`. El informe (`hid_dump.txt`) se guarda en `%LocalAppData%\SimonSays\debug\` y se abre esa carpeta. Solo se crea cuando lo pides.

## Comentarios, soporte y privacidad
- El diálogo `Acerca de` (menú de la bandeja) muestra la versión, la descripción y el copyright.
- Comparte sugerencias, problemas o cómo funciona SimonSays con tu seguidor ocular mediante la opción `Danos tu opinión` del menú de la bandeja, que abre un [formulario de comentarios](https://forms.gle/KMbpEDYmwnFJUhgy8).
- Para informar de errores y obtener soporte técnico, visita el [repositorio de SimonSays en GitHub](https://github.com/juan-rey/SimonSays).
- SimonSays es libre y de código abierto, con la Open Software License 3.0 (OSL-3.0). No muestra publicidad ni recoge tus datos.
