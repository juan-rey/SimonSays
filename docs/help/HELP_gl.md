# Guía de Axuda de SimonSays

## Visión xeral
SimonSays é un asistente de texto a voz (TTS) para Windows, creado por Juan Rey Saura, optimizado para fala rápida, bibliotecas de frases e reprodución de sons en liña. Soporta múltiples idiomas, voces configurables e funcións pensadas para unha comunicación eficiente.

SimonSays utiliza o espazo libre á esquerda da barra de tarefas de Windows, proporcionando acceso rápido a unha ducia de categorías de frases de uso frecuente (e personalizables) con un par de clics.

## Atallos
- `F1`: Abrir Axuda.
- `F2`: Abrir Configuración.
- `F3`: Engadir despois da selección actual (contexto categoría/frase).
- `F4`: Editar a selección actual.
- `F5` / `F6`: Mover á selección anterior / seguinte nas listas.
- `F8`: Eliminar a selección actual.
- `F9`: Importar categorías.
- `F10`: Exportar categorías.
- Botón `Reproducir>`: Iniciar reprodución.

## Inicio e uso básico
1. Inicia SimonSays; aparece na bandexa do sistema de Windows.
2. Para falar, podes:
   - Escribir texto personalizado na caixa de entrada e premer o botón `Reproducir>` (ou `Enter`).
   - Facer clic nunha ou varias frases gardadas na xanela de Categorías.
3. Durante a reprodución, o botón mostra `>` para indicar o estado activo. Podes premelo de novo para deter a reprodución ao instante.

## Xanela de Categorías (frases)
- Ábrela co botón `Categorías`. Podes redimensionala ou recordar o seu tamaño desde Configuración.
- Selecciona unha categoría arriba e frases abaixo.
- Un clic nunha frase reprodúcea automaticamente se `Falar a frase inmediatamente ao seleccionala` está activado; se non, carga a frase na caixa de entrada principal sen falar.

### Xestionar categorías
- Engadir: selecciona categoría → (atallo: `F3`) diálogo `Engadir nova categoría` → escribe o nome → `Aceptar`.
- Renomear: selecciona categoría → (atallo: `F4`) `Cambiar o nome da categoría` → cambia o nome → `Aceptar`.
- Mover selección: `F5`/`F6` para mover ao elemento anterior/seguinte.
- Eliminar: selecciona categoría → (atallo: `F8`) `Eliminar categoría`; confirma a eliminación da categoría e as súas frases.



#### Configurar ou cambiar a icona dunha categoría
Cada categoría pode mostrar un emoji ou un ficheiro `.ico` personalizado no seu botón. A icona especifícase no diálogo Engadir/Editar usando o prefixo separador `##`:

```
<icona>##<nome da categoría>
```

- **Emoji**: escribe o emoji seguido de `##` e do nome.
  - Exemplo: `🙂##Saúdos`
- **Ficheiro de icona** (`.ico`): escribe a ruta do ficheiro (absoluta ou relativa ao cartafol de SimonSays) seguida de `##` e do nome.
  - Exemplo: `C:\icons\smile.ico##Saúdos`
- **Sen icona**: escribe só o nome da categoría, sen o prefixo `##`.
  - Exemplo: `Saúdos`

> Nota: só se admiten ficheiros `.ico` para iconas baseadas en ficheiro. Calquera outra extensión ignórase e non se mostra ningunha icona.

### Xestionar frases
- Engadir: selecciona frase → (atallo: `F3`) `Engadir nova frase` → escribe o texto → `Aceptar`.
- Editar: selecciona frase → (atallo: `F4`) `Editar a frase` → actualiza o texto → `Aceptar`.
- Mover selección: `F5`/`F6` para mover á frase anterior/seguinte.
- Eliminar: selecciona frase → (atallo: `F8`) `Eliminar frase`; confirma.



#### Configurar ou cambiar a icona dunha frase
As frases seguen a mesma convención de prefixo de icona ca as categorías, co separador opcional `##` antes do texto da frase:

```
<icona>##<texto da frase>
```

- **Emoji**: `👍##Soa ben!`
- **Ficheiro de icona**: `C:\icons\check.ico##Soa ben!`
- **Sen icona**: `Soa ben!`

As frases tamén poden incluír un ficheiro de audio en liña. Ao combinar icona, texto e ficheiro de audio, o formato completo é:

```
<icona>##<texto da frase>::<ficheiro de audio>
```

- Exemplo: `🔔##Atención::notification.wav`

O prefixo `##` e o sufixo `::` son opcionais e independentes; podes usar calquera combinación:
- Só prefixo de icona: `🔔##Atención`
- Só sufixo de audio: `Atención::notification.wav`
- Ambos: `🔔##Atención::notification.wav`
- Ningún: `Atención`

## Configuración (F2)
Abre Configuración (`F2`) para controlar:
- **Texto predeterminado**: activar/desactivar `Inserir automaticamente o texto predeterminado` e personalizar a frase por defecto.
  - `Mostrar teclado táctil` — abre o teclado táctil en pantalla cando a caixa de entrada recibe o foco, posicionado preto da xanela principal.
- **Idioma da interface**: escoller idioma de UI (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **Voz**: seleccionar a voz SAPI instalada segundo a túa preferencia.
- **Volume/Velocidade**: axustar `Volume (10-100)` e `Velocidade (-10 a 10)`.
- `Probar voz`: probar a voz seleccionada.
- **Xanela de categorías**:
  - `Minimizar automaticamente a xanela de categorías` tras seleccionar.
  - `Lembrar o tamaño da xanela de categorías`.
- **Comportamento de reprodución**:
  - `Falar a frase inmediatamente ao seleccionala` (auto-reprodución ao facer clic).
  - `Deter a reprodución anterior` — detén automaticamente calquera reprodución en curso antes de iniciar unha nova frase.
  - `Aumentar o volume de SimonSays ao reproducir` — sobe o volume mestre do sistema mentres fala, restaurándoo cando remata a reprodución.
  - `Reducir temporalmente outros sons ao reproducir` — reduce o volume das demais aplicacións mentres fala, restaurándoo cando remata a reprodución.
- `Aceptar` garda cambios; `Cancelar` descarta.

## Menú da icona da bandexa
- `Mostrar` / `Ocultar` a interface principal.
- `Configuración`.
- `Sobre` mostra a versión e os créditos.
- `Web` abre a páxina do proxecto (se está configurada).
- `Comentarios` abre un [formulario de comentarios](https://forms.gle/KMbpEDYmwnFJUhgy8) no navegador predeterminado.
- `Saír` pecha SimonSays.

Podes mover a xanela emerxente da bandexa e outros diálogos arrastrando a barra de título; redimensiona a xanela de Categorías arrastrando os bordes, e activa `Lembrar o tamaño da xanela de categorías` en Configuración para conservar o tamaño.

## Mesturar fala e sons
- Usa o delimitador `♫` (`SOUND_NOTE_DELIMITER`) para inserir nomes de arquivo de audio no texto:
  - Exemplo: `Ola ♫notification.wav♫, por favor espere.`
  - O texto fóra dos delimitadores fálase; o texto dentro trátase como arquivo de son e reprodúcese en liña.
- Audio soportado: `.wav`, `.mid`, `.midi`, `.mp3`.
- Usa rutas absolutas ou relativas accesibles ao proceso. Emparella delimitadores para evitar interpretar texto final como audio.



### Orde de busca de ficheiros de son e icona
Cando un nome de ficheiro de son ou icona non contén unha letra de unidade nin unha ruta completa, SimonSays búscao na seguinte orde e usa a primeira coincidencia atopada:

1. **Cartafol AppData** — `%APPDATA%\SimonSays\` (específico do usuario; coloca aquí os ficheiros para mantelos separados da instalación da aplicación).
2. **Directorio de traballo** — o directorio de traballo actual do proceso (só se busca se é diferente do cartafol do executábel).
3. **Cartafol do executábel** — o cartafol que contén `SimonSays.exe`.

Se o ficheiro non se atopa en ningunha destas localizacións, úsase o son de reserva integrado.

> **Consello:** recoméndase colocar os teus ficheiros de son e icona personalizados en `%APPDATA%\SimonSays\`; así sobrevivirán ás actualizacións e serán específicos do usuario.

## Importar e exportar categorías
- Exportar: podes exportar todas as categorías ou só a seleccionada cando se che pregunte. As mensaxes de éxito/erro aparecen no idioma seleccionado (atallo: `F10`).
- Importar: escolle arquivo; se a categoría existe, pediráselle sobrescribir. As mensaxes de éxito/erro aparecen no idioma seleccionado (atallo: `F9`).

## Almacenamento de valores por defecto
- As frases por defecto créanse por idioma na clave de rexistro `HKCU\SOFTWARE\SimonSays\Phrases\<Language>` no primeiro inicio.
- Para reinstalar valores por defecto dun idioma: pecha SimonSays → elimina a clave do idioma → reinicia SimonSays.

## Solución de problemas
- **Fallo de inicialización de voz**: verifica que SAPI estea instalado e que haxa unha voz compatible.
- **Non se reproducen sons**: confirma ruta/extensión e delimitadores `♫` emparellados.
- **Erros de importación/exportación**: revisa os permisos e a localización do disco; volve intentalo.
- **Idioma UI non aplicado**: reabre Configuración e confirma idioma; asegúrate de que existan cadeas de localización para o idioma elixido.

## Sobre
- O diálogo `Sobre` mostra versión, descrición e copyright.
