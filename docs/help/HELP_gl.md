# Guía de axuda de SimonSays

## Descrición xeral
SimonSays é un asistente de texto a voz (TTS) para Windows creado por Juan Rey Saura. Naceu como unha ferramenta para persoas con ELA e outras persoas que xa non poden falar, para que puidesen comunicarse rapidamente sen deixar o que estaban a facer no ordenador. Co tempo quedou claro que o mesmo deseño serve como ferramenta xeral de CAA (comunicación aumentativa e alternativa) para calquera persoa á que lle custe falar ou non poida facelo; por exemplo, por disartria, unha laringectomía, apraxia da fala, un ictus ou unha lesión cerebral. Está optimizado para falar rápido, usar bibliotecas de frases e reproducir sons dentro do texto. Admite varios idiomas, voces configurables e funcións pensadas para unha comunicación eficiente.

SimonSays usa o espazo libre da barra de tarefas de Windows e dá acceso rápido a unha ducia de categorías de frases de uso frecuente (e personalizables) con só un par de clics. Como nunca tapa as túas outras xanelas, podes seguir no navegador, nos teus documentos ou nunha videochamada e falar sempre que o necesites: co teclado, co rato ou sen mans, cun seguidor ocular.

Casos de uso:
- Uso intensivo de varias aplicacións de Windows sen perder de vista a comunicación.
- Taboleiros creados por logopedas para persoas con discapacidade da fala.
- Reunións en liña con [Virtual CABLE](https://vb-audio.com/Cable/) (consulta [Usar SimonSays en reunións en liña](#usar-simonsays-en-reunións-en-liña)).

SimonSays non pretende substituír programas de comunicación completos como Mind Express, TD Communicator, Grid 3 ou OptiKey, nin a valoración e o apoio profesional que adoitan acompañalos. Fai un traballo máis concreto: falar rapidamente desde a barra de tarefas mentres segues a traballar noutras aplicacións. Pode funcionar xunto a eses programas; por exemplo, OptiKey ou outro software de control ocular pode mover o cursor mentres SimonSays di as túas frases.

SimonSays é de código aberto e gratuíto (libre como en liberdade, non só como en prezo), e seguirá séndoo: calquera pode usalo, estudalo, adaptalo e compartilo. Non ten publicidade nin recolle datos.

SimonSays achégase á versión 1.0. Antes dese fito necesita xente que o probe no seu día a día, coas súas propias voces, taboleiros e seguidores oculares: se algo falla, resulta incómodo ou falta, cóntanolo (consulta [Comentarios, soporte e privacidade](#comentarios-soporte-e-privacidade)). Cada comentario axuda.

## Contido
- [Primeiros pasos](#primeiros-pasos)
- [Xanela de categorías](#xanela-de-categorías)
- [Frases predeterminadas](#frases-predeterminadas)
- [Atallos](#atallos)
- [Configuración (F2)](#configuración-f2)
- [Mirada / Clic por fixación (F3)](#mirada--clic-por-fixación-f3)
- [Usar SimonSays en reunións en liña](#usar-simonsays-en-reunións-en-liña)
- [Menú da icona da bandexa](#menú-da-icona-da-bandexa)
- Editar taboleiros (avanzado):
  - [Editar categorías e frases](#editar-categorías-e-frases) (e [que editor usar](#que-editor-usar))
  - [Mesturar voz e sons](#mesturar-voz-e-sons)
  - [Personalizar o aspecto (estilos de taboleiro e de categoría)](#personalizar-o-aspecto-estilos-de-taboleiro-e-de-categoría)
  - [Importar e exportar categorías](#importar-e-exportar-categorías)
  - [Desfacer cambios](#desfacer-cambios)
  - [Onde garda SimonSays os teus ficheiros](#onde-garda-simonsays-os-teus-ficheiros)
- [Solución de problemas](#solución-de-problemas)
- [Comentarios, soporte e privacidade](#comentarios-soporte-e-privacidade)

## Primeiros pasos
1. Inicia SimonSays. Aparece nun espazo libre da barra de tarefas de Windows e engade unha icona á bandexa do sistema. A barra de tarefas pode estar abaixo, arriba, á esquerda ou á dereita da pantalla:
   - Cando as iconas da barra de tarefas están centradas, SimonSays colócase ao principio da barra (no extremo esquerdo, ou arriba nunha barra lateral). Cando están aliñadas á esquerda, colócase xusto antes da bandexa do sistema.
   - Nunha barra de tarefas pequena (estreita), a barra de SimonSays é máis baixa, cunha letra máis pequena e os dous botóns de acceso rápido un ao lado do outro. Nunha barra de tarefas á esquerda ou á dereita da pantalla, os controis apílanse un debaixo do outro.
   - A xanela de categorías ábrese xunto á barra de tarefas, e a frecha do botón `Categorías` indica cara a onde se abre.
   - Se moves ou cambias o tamaño da barra de tarefas mentres SimonSays está en marcha, séguea en poucos segundos.
2. Para falar, podes:
   - Facer clic na caixa de texto da barra (ou fixar a mirada nela): ábrese unha caixa de entrada máis grande xunto á barra de tarefas. Escribe o texto e preme `Enter` para dicilo; a caixa péchase e o foco pasa a `Reproducir>`, así que `Espazo` volve dicilo (ou deteno). `Esc` pecha a caixa sen falar e lévate de volta á xanela que estabas a usar; o texto queda na barra, onde `Reproducir>` volve dicilo.
   - Facer clic nunha ou varias frases gardadas na xanela de categorías.
3. Durante a reprodución, o botón mostra `>` para indicar que está a soar. Podes premelo de novo para detelo ao instante.

![A barra de SimonSays na barra de tarefas](img/bar.png)

Xunto ao botón `Categorías`, dous pequenos **botóns de acceso rápido** dan acceso cun clic a dúas cousas ás que, se non, chegarías cun atallo ou co menú da bandexa:

- 📂 abre un taboleiro gardado no teu ordenador; é a mesma importación que `F9`, pero empeza no teu cartafol de taboleiros (`%LocalAppData%\SimonSays\boards\`), onde se gardan os taboleiros exportados e descargados (consulta [Importar e exportar categorías](#importar-e-exportar-categorías)).
- ⚙️ abre a configuración, igual que `F2`.

Os dous pódense ocultar con `Mostrar os botóns e o menú de acceso rápido` na configuración (`F2`), o que deixa máis espazo para a caixa de entrada. A mesma opción desactiva tamén o menú contextual (clic dereito) da xanela de categorías.

Se SimonSays está oculto, fai clic na súa icona da bandexa para mostralo de novo.

## Xanela de categorías
![A xanela de categorías coas frases predeterminadas](img/categories.png)

- Ábrea co botón `Categorías`. Cambia o seu tamaño, ou activa `Lembrar o tamaño da xanela de categorías` na configuración para conservalo.
- Selecciona unha categoría na lista de arriba e frases na lista de abaixo.
- Un clic nunha frase dina automaticamente se `Falar a frase inmediatamente ao seleccionala` está activado; se non, a frase cárgase na caixa de entrada principal sen falar.
- Fai dobre clic nunha zona baleira ou no bordo da xanela para axustar o seu tamaño de xeito que caiban todas as súas categorías e frases.
- Arrastra unha zona baleira da xanela para movela, e preme `Esc` para ocultala.
- Achega e afasta o zoom con `F12` e `F11` (consulta [Atallos](#atallos)).
- Fai clic dereito nun botón ou nunha zona baleira (ou preme `Maiús + F10` ou a tecla de menú) para abrir un menú con `Pegar`, `Reducir`, `Ampliar`, `Restablecer o zoom`, `Axuda`, `Configuración`, `Mirada / Clic por fixación...`, `Ocultar` e `Saír de SimonSays...`, cada un co seu atallo. A primeira entrada mostra o título do taboleiro (`Taboleiro sen título` se non ten); escóllea para ver os créditos do taboleiro, o seu idioma e cantas categorías e frases contén. O menú só aparece mentres `Mostrar os botóns e o menú de acceso rápido` está activado na configuración.
- Nun botón de categoría ou de frase, o menú ofrece ademais `Editar...`, `Mover á esquerda`, `Mover á dereita`, `Engadir nova categoría...` ou `Engadir nova frase...` e `Eliminar categoría...` ou `Eliminar frase...` para ese botón, sen ter que seleccionalo antes; nunha categoría, `Exportar categoría...` pregunta se gardar só esa categoría ou todo o taboleiro. Os atallos `F4` a `F8` móstranse cando o botón é a túa selección actual, porque esas teclas actúan sobre a selección. Nunha zona baleira, o menú ofrece `Engadir nova categoría...`, que engade unha ao final; tamén é a forma de comezar un taboleiro baleiro.
- A franxa entre as categorías e as frases mostra un recordatorio dos atallos principais (ou o rótulo do taboleiro, se ten un).

![O menú contextual dun botón de frase](img/menu.png)

## Frases predeterminadas
SimonSays inclúe un conxunto de categorías e frases listo para usar en cada un dos seus 18 idiomas, ordenado para que as frases que máis se necesitan requiran menos seleccións:

- 👋🏻 Saúdos Frecuentes
- 🗣️ Frases de Conversa
- 🚨 Frases de Emerxencia
- 🤲 Coidado e Confort
- 🏥 Frases de Saúde
- 🏠 Familia e Fogar
- 👥 Frases Sociais
- ⏰ Hora e Data
- 💼 Frases de Negocios
- ✈️ Frases de Viaxe
- 🔊 Sons

Cada idioma ten as súas propias categorías e frases: cando cambias o idioma na configuración (`F2`), a xanela de categorías pasa ao taboleiro dese idioma, e os cambios que fagas nun idioma non afectan aos demais.

As frases dinse coas voces instaladas en Windows. Se non hai ningunha voz instalada para o teu idioma, as frases móstranse igualmente, pero poden dicirse con outra voz (consulta [Solución de problemas](#solución-de-problemas)).

Podes cambiar as frases predeterminadas ou recuperalas máis adiante; consulta [Editar categorías e frases](#editar-categorías-e-frases) e [Restaurar as frases predeterminadas](#restaurar-as-frases-predeterminadas).

## Atallos
- `F1`: Abrir a axuda
- `F2`: Abrir a configuración
- `F3`: Configuración de mirada / clic por fixación
- `Enter`: Dicir o texto da caixa de entrada
- `Esc`: Ocultar a xanela de categorías
- `F11` / `Ctrl -`: Afastar o zoom (xanela de categorías)
- `F12` / `Ctrl +`: Achegar o zoom (xanela de categorías)
- `Ctrl + 0`: Restablecer o zoom
- `Maiús + F10` ou a tecla de menú: Abrir o menú da xanela de categorías (igual que o clic dereito)
- `Espazo` sobre `Reproducir>`: Volver dicir o texto, ou detelo mentres soa

Os atallos para editar categorías, frases e taboleiros indícanse en [Editar categorías e frases](#editar-categorías-e-frases).

## Configuración (F2)
Abre a configuración (`F2`) para axustar o seguinte. A configuración ten dúas columnas.

![A xanela de configuración](img/settings.png)

**Columna esquerda**
- **Texto predeterminado**: activa ou desactiva `Inserir automaticamente o texto predeterminado` e personaliza a frase predeterminada.
  - `Mostrar teclado táctil`: abre o teclado táctil en pantalla cando se abre a caixa de entrada; colócase ao lado da caixa de entrada, no lado oposto á barra de tarefas.
- `Mostrar os botóns e o menú de acceso rápido`: mostra os botóns 📂 (abrir un taboleiro) e ⚙️ (configuración) xunto ao botón `Categorías` da xanela principal (consulta [Primeiros pasos](#primeiros-pasos)) e o menú contextual da xanela de categorías (consulta [Xanela de categorías](#xanela-de-categorías)). Desactívaa para ocultalos todos e darlle máis espazo á caixa de entrada.
- **Xanela de categorías**:
  - `Minimizar automaticamente a xanela de categorías` despois de seleccionar.
  - `Lembrar o tamaño da xanela de categorías`.
  - `Axustar automaticamente a xanela de categorías ao contido`: axusta a xanela de categorías ás súas categorías e frases cada vez que se cargan; por exemplo, ao iniciar, ao cambiar de idioma ou despois dunha importación.
- `Mirada / Clic por fixación...`: abre a configuración de [Mirada / Clic por fixación](#mirada--clic-por-fixación-f3) enriba da configuración, que segue aberta cos teus cambios. Os cambios que fagas alí gárdanse ao premer `Aceptar` nesa xanela, aínda que despois premas `Cancelar` na configuración.

**Columna dereita**
- **Idioma**: escolle o idioma da interface (English, Español, العربية, Euskara, Català, 简体中文, Français, Galego, Deutsch, עברית, हिन्दी, Italiano, 日本語, 한국어, Português, Português (Brasil), Русский, Valencià). Cada idioma ten tamén as súas propias categorías e frases (consulta [Frases predeterminadas](#frases-predeterminadas)).
- **Voz**: escolle unha voz SAPI instalada que coincida co teu idioma.
- **Volume/Velocidade**: axusta `Volume (10-100)` e `Velocidade (-10 a 10)`.
- `Probar voz`: escoita a voz seleccionada (a través do dispositivo escollido en `Saída de son`).
- `Saída de son`: por onde reproduce SimonSays a súa voz e os seus sons: `Predeterminado de Windows` (o habitual) ou calquera dispositivo conectado. Os cables de audio virtuais aparecen primeiro, que é o que necesitas para as [reunións en liña](#usar-simonsays-en-reunións-en-liña). Se o dispositivo escollido non está conectado, SimonSays soa polo dispositivo predeterminado de Windows e avísate unha vez; a elección consérvase, móstrase como `(non conectado)` e volve usarse en canto o dispositivo volve.
- `Axustes de escoita...`: abre os dispositivos de gravación de Windows, onde podes activar **Escoitar este dispositivo** para un cable virtual (consulta [Usar SimonSays en reunións en liña](#usar-simonsays-en-reunións-en-liña)).
- **Comportamento da reprodución**:
  - `Falar a frase inmediatamente ao seleccionala` (dicir automaticamente cun clic).
  - `Deter a reprodución anterior`: detén automaticamente calquera reprodución en curso antes de comezar unha frase nova.
  - `Aumentar o volume de SimonSays ao reproducir`: sobe o volume do dispositivo polo que soa SimonSays (consulta `Saída de son`) mentres fala e restaura o nivel anterior cando remata.
  - `Reducir temporalmente outros sons ao reproducir`: baixa o volume das demais aplicacións mentres fala e restáurao cando remata.

**Abaixo**
- `Axuda` (esquina inferior esquerda, ou `F1` coa configuración aberta) abre esta axuda no idioma seleccionado na lista `Idioma`; a configuración segue aberta cos teus cambios.
- `Aceptar` garda os cambios; `Cancelar` descártaos.

De forma predeterminada, `Falar a frase inmediatamente ao seleccionala`, `Minimizar automaticamente a xanela de categorías`, `Axustar automaticamente a xanela de categorías ao contido`, `Aumentar o volume de SimonSays ao reproducir` e `Reducir temporalmente outros sons ao reproducir` están activados, e `Deter a reprodución anterior` está desactivado.

## Mirada / Clic por fixación (F3)
Activa frases e botóns sen usar as mans mantendo a mirada (ou o cursor do rato) sobre eles durante un tempo determinado. Esta función é nova: necesita probas e comentarios; cóntanos como funciona co teu seguidor mediante a opción `Dános a túa opinión` do menú da bandexa.

Configuracións compatibles:
- **Calquera software de control ocular que mova o cursor do rato** (Irisbond EasyClick, Tobii Dynavox Computer Control, os modos de cursor do Control ocular de Windows, OptiKey): o clic por fixación segue o cursor.
- **Lectura directa da mirada (sen necesidade de mover o cursor)**: Irisbond Hiru (modo HID) e os seguidores Tobii a través do seu propio software; verificado con Tobii Eye Tracker 4C, Tobii Eye Tracker 5 e Tobii Dynavox PCEye 5.
- **Tobii Eye Tracker 5**: o seu software actual só instala un motor de 64 bits, así que SimonSays leo a través dun pequeno programa auxiliar, `SimonSaysGaze64.exe`, instalado xunto a el. O auxiliar só se inicia cando fai falta, non ten xanela, péchase con SimonSays e mantén a mirada no teu ordenador: nunca se garda nin se envía a ningún sitio.
- **Licenza de Tobii Eye Tracker 4C e Eye Tracker 5**: a licenza de usuario final de Tobii para estes seguidores non permite o uso médico, incluído o uso en hospitais e outros contornos sanitarios. Os dispositivos de Tobii Dynavox, como o PCEye 5, teñen as súas propias condicións de licenza.
- **Software do Tobii Eye Tracker 4C**: SimonSays le o 4C a través de Tobii Core. Se conectas un Eye Tracker 5 ao mesmo ordenador, Windows instala Tobii Experience, que desactiva Tobii Core, e o 4C deixa de funcionar. Para volver usar o 4C, reinstala Tobii Core; a versión recomendada é `Tobii_Eye_Tracking_Core_v2.13.1.7294_x86.exe`, da [páxina de primeiros pasos de Tobii](https://gaming.tobii.com/getstarted/).

Onde funciona o clic por fixación:
- A barra de SimonSays, a xanela de categorías e os seus botóns de frases e categorías.
- Os botóns das xanelas e preguntas de SimonSays: a configuración, a xanela de Mirada / Clic por fixación, os diálogos de engadir, editar e vista previa, e todas as confirmacións, como `Si` / `Non` ou `Aceptar` / `Cancelar`. Estes botóns están no teu idioma e comezan a responder á fixación uns tres cuartos de segundo despois de abrirse a xanela, para que unha pregunta que aparece xusto onde estás a mirar non poida responderse soa.
- Cando se le a mirada directamente, o botón que estás a mirar ilumínase igual que baixo o rato, así que podes ver o que vas seleccionar antes de que comece o progreso. Un pestanexo mantén o resaltado; apartar a mirada quítao.

![A xanela de Mirada / Clic por fixación](img/dwell.png)

Abre a configuración de Mirada / Clic por fixación (`F3`, o menú da bandexa ou o botón `Mirada / Clic por fixación...` da configuración) para axustar:
- **Modo de activación**: escolle unha destas opcións:
  - `Automático`: detecta os dispositivos presentes e activa o clic por fixación co cursor do rato ou coa detección da mirada, segundo corresponda. O software de Tobii en segundo plano (sempre en marcha unha vez instalado) non abonda para activar a fixación co cursor; un seguidor Tobii só se usa mentres está a seguir a mirada de verdade.
  - `Cursor do rato`: activa sempre o clic por fixación co cursor do rato.
  - `Seguidor ocular HID`: activa sempre o clic por fixación coa detección da mirada (Irisbond Hiru en modo HID, ou seguidores Tobii). Só a túa mirada activa os botóns: mentres non chega ningunha mirada (seguimento apagado ou seguidor desconectado), non se activa nada por fixación e o rato funciona con normalidade.
  - `Desactivado`: desactiva sempre o clic por fixación.
  - **Atraso de enganche**: canto tempo, en milisegundos, ten que pousarse a túa mirada (ou o cursor) sobre un botón antes de que comece a barra de progreso (250 de forma predeterminada). Así, ao pasar a mirada por riba dos botóns non se ve ningún progreso; o botón actívase tras o atraso de enganche máis o tempo de fixación. Ponno a 0 para que o progreso comece de inmediato.
  - **Tempo de fixación**: a duración (en milisegundos) necesaria para facer clic ao mirar un elemento (500 de forma predeterminada). Mentres dura a fixación móstrase un indicador de progreso.
  - **Raio de tolerancia**: a tolerancia da mirada (en píxeles) que permite pequenos movementos do ollo sen deixar de contar a fixación.
  - **Tempo de espera**: milisegundos despois dunha activación durante os que o clic por fixación non pode volver activarse (evita clics repetidos por accidente).
  - **Cor de progreso**: a cor do indicador de progreso da fixación (de forma predeterminada, a cor de énfase de Windows).
  - **Afastarse antes de que a fixación volva activar** (activado de forma predeterminada): despois dun clic por fixación, ningún botón pode volver activarse por fixación ata que o punteiro ou a mirada se afasten dese punto (máis alá do raio de tolerancia) ou apartes a vista un momento. Quedar sobre un botón actívao unha soa vez, así que unha frase longa non se repite antes de rematar. Os clics do rato non se ven afectados.
  - **Detectar como usas o seguidor**: dous botóns que axudan á aplicación a detectar se moves o cursor do rato cun seguidor ocular ou se simplemente fas clic nos botóns.
  - **Detectado**: mostra o tipo de dispositivo detectado e máis información sobre o software relacionado.
  - `Gardar informe de diagnóstico...` (debaixo de Detectado): escribe un informe sobre o teu seguidor ocular e o seu software en `hid_dump.txt` e abre o seu cartafol, para que poidas envialo ao soporte.
  - Un botón `Restablecer` (abaixo á esquerda) devolve todos os axustes de fixación aos seus valores predeterminados.
  - Un botón `Aceptar` garda os cambios; `Cancelar` descártaos.

## Usar SimonSays en reunións en liña
SimonSays pode falar directamente nunha videochamada (Teams, Zoom, Google Meet…), de xeito que os demais participantes oen as túas frases coma se saísen do teu micrófono. Necesitas un cable de audio virtual, como o gratuíto [VB-Audio Virtual Cable](https://vb-audio.com/Cable/):

1. Instala o cable de audio virtual e reinicia Windows se cho pide.
2. Envía o son de SimonSays ao cable: abre a configuración (`F2`) e escolle `CABLE Input` en `Saída de son` (os cables virtuais aparecen primeiro). Preme `Probar voz` para comprobalo: xa non deberías oílo polos altofalantes, porque agora vai ao cable.
3. Na túa aplicación de reunións, escolle `CABLE Output` como micrófono.
4. Para oír ti tamén SimonSays, preme `Axustes de escoita...` na configuración. Ábrense os dispositivos de gravación de Windows: selecciona `CABLE Output`, abre **Propiedades → Escoitar**, marca **Escoitar este dispositivo**, escolle os teus altofalantes ou auriculares en **Reproducir a través deste dispositivo** e preme `Aceptar`. Só tes que facelo unha vez.

SimonSays non pode activar **Escoitar este dispositivo** por ti: é un axuste do lado de gravación do cable que Windows só che deixa cambiar a ti, así que `Axustes de escoita...` lévate directamente á xanela correcta.

Convén saber:
- Deixa o dispositivo de reprodución predeterminado de Windows nos teus altofalantes ou auriculares, non en `CABLE Input`. Se **Escoitar este dispositivo** reproduce en "Dispositivo de reprodución predeterminado" mentres ese dispositivo predeterminado é o cable, o son volve entrar no cable.
- Se na reunión se oen as túas frases pero non os efectos de son (aplausos, un timbre), a supresión de ruído da aplicación de reunións estaos eliminando; baixa ou desactiva a supresión de ruído na aplicación.
- Se o cable se desinstala ou se desconecta, SimonSays soa polo dispositivo predeterminado de Windows e avísate unha vez; volve ao cable en canto estea dispoñible de novo.
- Para ter frases preparadas para unha chamada, engade a categoría **Modelo de reunión**: preme `F9` ou o botón 📂 e abre o taboleiro co nome no teu idioma (`SimonSays_Modelo_de_reunión_(Galego).ssc` en galego). Ten saúdos, "Óesme?", unha breve explicación de que falas cun programa, si / non, de acordo / en desacordo, pedir que repitan ou falen máis amodo, e despedidas. Edítaa para engadir frases para a túa propia reunión.
- Como alternativa ao paso 2, podes seguir dirixindo SimonSays co axuste por aplicación de Windows (**Configuración → Sistema → Son → Mesturador de volume**), deixando `Saída de son` en `Predeterminado de Windows`. SimonSays só aparece nesa lista despois de reproducir algo.

## Menú da icona da bandexa
- `Mostrar` / `Ocultar` a interface principal
- `Configuración`
- `Mirada / Clic por fixación...`
- `Web` abre a páxina web de SimonSays
- `Sobre` mostra a versión e os créditos
- `Dános a túa opinión` abre un [formulario de comentarios](https://forms.gle/KMbpEDYmwnFJUhgy8) no navegador predeterminado
- `Saír` pecha SimonSays, despois de preguntar

Podes mover a xanela emerxente da bandexa e outros diálogos arrastrando a súa barra de título; cambia o tamaño da xanela de categorías arrastrando os seus bordos, e activa `Lembrar o tamaño da xanela de categorías` na configuración para conservalo.

## Editar categorías e frases
Esta sección e as seguintes son para coidadores, logopedas e usuarios avanzados que cambian as categorías, frases, sons e aspecto dun taboleiro. Cada cambio gárdase ao instante (consulta [Desfacer cambios](#desfacer-cambios)).

### Que editor usar
- **En SimonSays**: cambios do día a día; cambiar o nome, engadir, mover ou eliminar categorías e frases, poñerlles un emoji, engadir sons, pegar unha lista de frases ou axustar unha cor. O resto desta sección explica como.
- **En [SimonSays Web](https://github.com/juan-rey/simonsays-web)**: o traballo grande con taboleiros, que alí é moito máis doado: taboleiros feitos con pictogramas de CAA (pode buscar na biblioteca de pictogramas de ARASAAC), imaxes de icona personalizadas e taboleiros con estilo, con cores, tamaños e disposicións. Recoméndase especialmente para deseñar un taboleiro **Fitzgerald Key**, no que cada grupo de palabras ten a súa propia cor. É un editor visual que abre e garda taboleiros `.ssc`/`.ssz` directamente no navegador, sen subilos; cando remates, importa o taboleiro en SimonSays con `F9` ou co botón 📂. Tamén é a forma de usar os taboleiros de SimonSays noutras plataformas.

Atallos de edición (xanela de categorías):
- `F4`: Editar a selección actual
- `F5` / `F6`: Ir á selección anterior / seguinte nas listas
- `F7`: Engadir despois da selección actual (categoría ou frase, segundo o contexto)
- `F8`: Eliminar a selección actual
- `F9`: Importar categorías
- `F10`: Exportar categorías
- `Ctrl + F4`: Editar o estilo do taboleiro (consulta [Personalizar o aspecto](#personalizar-o-aspecto-estilos-de-taboleiro-e-de-categoría))
- `Ctrl + F8`: Eliminar todas as categorías e as súas frases (pide confirmación dúas veces)
- `Ctrl + V` ou `Maiús + Insert`: Engadir o texto copiado como frases, unha por parágrafo (consulta [Xestionar frases](#xestionar-frases))
- Arrastrar un ficheiro `.txt`, `.ssc` ou `.ssz` á xanela: engade un `.txt` como categoría nova, ou importa un taboleiro igual que con `F9` (consulta [Xestionar categorías](#xestionar-categorías))

> **Consello:** exporta o teu taboleiro (`F10`) antes de usar `Ctrl + F8` ou de importar un taboleiro completo (`F9`), para poder recuperalo.

### Xestionar categorías
- Cambiar o nome: selecciona a categoría → (atallo: `F4`) `Cambiar o nome da categoría` → cambia o nome → `Aceptar`.
- Mover a selección: `F5`/`F6` para ir ao elemento anterior/seguinte ao percorrer as listas.
- Engadir: selecciona unha categoría → (atallo: `F7`) diálogo `Engadir nova categoría` → escribe o nome → `Aceptar`.
- Eliminar: selecciona a categoría → (atallo: `F8`) `Eliminar categoría`; confirma que queres eliminar a categoría e as súas frases.
- Engadir desde un ficheiro de texto: arrastra un ficheiro `.txt` á xanela de categorías → créase unha categoría co nome do ficheiro e unha frase por parágrafo (os parágrafos sepáranse cunha liña en branco) → revisa a vista previa → `Aceptar`. Podes soltar varios ficheiros á vez; cada un ten a súa propia vista previa. Rexéitase un ficheiro cuxo nome coincida cunha categoría existente.
- Abrir un taboleiro arrastrándoo: arrastra un ficheiro `.ssc` ou `.ssz` á xanela de categorías → SimonSays pregunta se queres importalo; con `Si` impórtase igual que con `F9` (consulta [Importar e exportar categorías](#importar-e-exportar-categorías)). Se o ficheiro contén un taboleiro completo, substitúe o actual, e antes gárdase unha copia do taboleiro actual como `backup.ssz`. Podes soltar á vez ficheiros de texto e de taboleiro; cada un procésase por quendas. Abrir un ficheiro `.ssc` ou `.ssz` desde o Explorador de ficheiros (dobre clic) fai a mesma pregunta.

#### Poñer ou cambiar a icona dunha categoría
Cada categoría pode mostrar un emoji ou un ficheiro de imaxe personalizado (`.ico`, `.png` ou `.jpg`) no seu botón. Indica a icona no diálogo de engadir ou editar co prefixo separador `##`:

```
<icona>##<nome da categoría>
```

- **Emoji**: escribe o emoji (ou os emojis) seguido de `##` e o nome.
  - Exemplo: `🙂##Saúdos`
- **Ficheiro de icona** (`.ico`, `.png`, `.jpg`): escribe o nome do ficheiro ou a ruta completa seguidos de `##` e o nome. Un nome de ficheiro sen cartafol búscase como se describe en [Orde de busca dos ficheiros de son e de icona](#orde-de-busca-dos-ficheiros-de-son-e-de-icona).
  - Exemplo: `C:\icons\smile.png##Saúdos`
- **Sen icona**: escribe só o nome da categoría, sen prefixo `##`.
  - Exemplo: `Saúdos`

> Nota: para as iconas de ficheiro só se admiten ficheiros `.ico`, `.png` e `.jpg`. As demais extensións ignóranse e non se mostra ningunha icona. As imaxes PNG conservan a súa transparencia; as imaxes moi grandes redúcense automaticamente ao cargalas.

### Xestionar frases
- Editar: selecciona a frase → (atallo: `F4`) `Editar a frase` → actualiza o texto → `Aceptar`.
- Mover a selección: `F5`/`F6` para ir á frase anterior/seguinte.
- Engadir: selecciona unha frase → (atallo: `F7`) `Engadir nova frase` → escribe o texto → `Aceptar`.
- Eliminar: selecciona a frase → (atallo: `F8`) `Eliminar frase`; confirma.
- Engadir varias á vez: copia un texto coas frases separadas por unha liña en branco → selecciona unha frase (ou unha categoría) → `Ctrl + V` → revisa a vista previa → `Aceptar`. As frases engádense despois da frase seleccionada, ou ao final da categoría. Os saltos de liña dentro dun parágrafo convértense en espazos, e os marcadores de icona `##` e de audio `::` funcionan igual que no diálogo `Engadir nova frase`.
- Engadir sons: arrastra un ou varios ficheiros `.wav` ou `.mp3` á xanela de categorías → revisa a vista previa → `Aceptar`. Cada ficheiro convértese nunha frase de son co nome do ficheiro, engadida despois da frase seleccionada (ou ao final da categoría seleccionada), e o ficheiro cópiase no cartafol de recursos do taboleiro para que viaxe con el cando o exportes. Nunca se sobrescribe un ficheiro existente: se xa hai un son distinto co mesmo nome, o novo gárdase como `nome (2).wav`.

#### Poñer ou cambiar a icona dunha frase
As frases seguen a mesma convención de prefixo de icona que as categorías, co separador opcional `##` diante do texto da frase:

```
<icona>##<texto da frase>
```

- **Emoji**: `👍##Paréceme ben!`
- **Ficheiro de icona**: `C:\icons\check.png##Paréceme ben!`
- **Sen icona**: `Paréceme ben!`

As frases poden incluír un ficheiro de audio (consulta [Mesturar voz e sons](#mesturar-voz-e-sons)). Para combinar icona, texto e audio, usa o formato:

```
<icona>##<texto da frase>::<ficheiro de audio>
```

- Exemplo: `🔔##Atención::alert.mp3`

O prefixo `##` e o sufixo `::` son opcionais e independentes; podes usar calquera combinación:
- Só prefixo de icona: `🔔##Atención`
- Só sufixo de audio: `Atención::alert.mp3`
- Ambos: `🔔##Atención::alert.mp3`
- Ningún: `Atención`

## Mesturar voz e sons
- Usa o delimitador `♫` para inserir nomes de ficheiros de audio no texto:
  - Exemplo: `Ola ♫doorbell.mp3♫, agarda un momento.`
  - O texto fóra dos delimitadores dise; o texto de dentro trátase como un ficheiro de son e reprodúcese nese punto.
- Audio admitido: `.wav` e `.mp3`. Os ficheiros MIDI (`.mid`, `.midi`) acéptanse nunha frase, pero non soan; convérteos a `.wav` ou `.mp3`.
- Usa rutas absolutas ou relativas ás que o programa poida acceder. Asegúrate de que os delimitadores van en parella, para que o texto final non se trate como audio.
- Para escribir `♫`, cópiao desta axuda, ou selecciona unha frase que teña un son: o seu marcador `♫…♫` colócase na caixa de entrada, onde podes engadir texto arredor.

### Sons incluídos con SimonSays
Estes sons instálanse con SimonSays e poden usarse polo seu nome en calquera texto ou frase; por exemplo, `♫laugh.mp3♫` na caixa de entrada, ou `Moi ben!::applause.mp3` para un botón de frase co rótulo "Moi ben!" que reproduce aplausos:

`ahem.mp3`, `alert.mp3`, `applause.mp3`, `aww.mp3`, `beep.mp3`, `boo.mp3`, `call_bell.mp3`, `censor.mp3`, `doorbell.mp3`, `drumroll.mp3`, `evil_laugh.mp3`, `fail.mp3`, `hmmmm.mp3`, `kisses.mp3`, `laugh.mp3`, `mmm-hmm.mp3`, `nuh-uh.mp3`, `phone_ring.mp3`, `rimshot.mp3`, `scream.mp3`, `shots.mp3`, `sigh.mp3`

### Orde de busca dos ficheiros de son e de icona
Cando o nome dun ficheiro de son ou de icona non inclúe unha letra de unidade nin unha ruta completa, SimonSays búscao nesta orde e usa a primeira coincidencia:

1. **O subcartafol de recursos do taboleiro actual**: se o estilo do taboleiro define `title` ou `resource-folder`, o seu propio subcartafol dentro do cartafol de recursos (consulta [Personalizar o aspecto](#personalizar-o-aspecto-estilos-de-taboleiro-e-de-categoría)).
2. **Cartafol de recursos**: `%LocalAppData%\SimonSays\resources\` (o lugar predeterminado para as iconas e os sons dos taboleiros; os paquetes `.ssz` importados instalan aquí os seus ficheiros).
3. **Cartafol de datos de SimonSays**: `%LocalAppData%\SimonSays\` (mantense como alternativa para os ficheiros que versións anteriores deixaban aquí, antes de que existise o cartafol de recursos).
4. **Directorio de traballo**: o directorio de traballo actual do proceso (só se busca se é distinto do cartafol do executable).
5. **Cartafol do executable**: o cartafol que contén `SimonSays.exe`.

Se o ficheiro non se atopa en ningunha destas localizacións, úsase o son de reserva integrado (unha icona simplemente non se mostra).

> **Consello:** recomendámosche colocar os teus ficheiros de son e de icona en `%LocalAppData%\SimonSays\resources\`; sobreviven ás actualizacións da aplicación e son propios de cada usuario. Un taboleiro cun estilo `title` ou `resource-folder` ten alí automaticamente o seu propio subcartafol, o que separa os seus ficheiros dos doutros taboleiros.

## Personalizar o aspecto (estilos de taboleiro e de categoría)
Podes cambiar as cores, o tamaño e o estilo da xanela de categorías e dos seus botóns; por exemplo, para crear taboleiros de comunicación con códigos de cor como Fitzgerald Key. Os estilos son opcionais: sen eles, a xanela mantén o seu aspecto predeterminado.

> **Consello:** todo o que segue pode escribirse en SimonSays, o que resulta práctico para pequenos retoques. Para deseñar un taboleiro enteiro con estilo ou con pictogramas, recoméndase [SimonSays Web](https://github.com/juan-rey/simonsays-web): ves o resultado mentres traballas e escribe estes estilos por ti (consulta [Que editor usar](#que-editor-usar)).

Un estilo é unha lista de pares `propiedade:valor;`. Os valores poden ser:
- **Cores**: hexadecimal `#RRGGBB`, por exemplo `#FFD966`.
- **Tamaños**: un número de píxeles (`96`) ou unha porcentaxe do predeterminado (`120%`).
- **Palabras clave**: por exemplo `left`, `right`, `top`, `bottom`, `smart`, `normal`, `bold`.
- **Texto**: un nome de fonte, ou o texto do rótulo, do título ou dos créditos.

Todo o que a aplicación non recoñece (unha propiedade mal escrita ou un valor incorrecto) simplemente ignórase, así que un estilo nunca estraga a xanela.

### Dar estilo a unha categoría (diálogo Editar)
Selecciona unha categoría e preme `F4` (Editar); despois engade `::` e un estilo tras o nome:

```
<icona>##<nome da categoría>::<propiedade>:<valor>;<propiedade>:<valor>;
```

- Exemplo: `🧍##Persoas::background:#FFD966;` pon en amarelo o botón Persoas. A cor da categoría **aplícase tamén ás frases que contén**, así que unha soa liña colorea todo un grupo de palabras: a base dun taboleiro Fitzgerald Key.
- Un estilo de categoría pode definir `background` e `text-color` para o botón da categoría, e calquera propiedade de frase (ver máis abaixo) co prefixo `phrase-`, por exemplo `phrase-corner-radius:8;`.

### Dar estilo a todo o taboleiro (`$$board`)
O aspecto xeral (o fondo da xanela, os separadores e os valores predeterminados de todos os botóns de categoría e de frase) é un **estilo de taboleiro**. Gárdase co nome de categoría reservado `$$board` (non podes usar nomes con `$$` para as túas propias categorías).

- **Edítao** premendo `Ctrl + F4` na xanela de categorías. O editor mostra un `propiedade:valor;` por liña. Ao confirmar con `Aceptar`, aplícase o novo estilo e SimonSays pregunta "Queres reverter os cambios?": escolle `Aceptar` para volver ao estilo anterior, ou `Cancelar` para conservar o novo.
- **Consegue un** importando un ficheiro de categorías con estilo (`.ssc` / `.ssz`). Cando importas un ficheiro que contén un estilo de taboleiro, SimonSays pregunta antes de substituír o teu e mostra o `title` e os `credits` do taboleiro (se os ten) na mensaxe de importación. Un ficheiro cuxo estilo de taboleiro só define `title`, `credits`, `language` ou `resource-folder` non cambia nada visible, así que SimonSays non pregunta: se contén unha soa categoría, engádese e o teu aspecto mantense; se contén un taboleiro completo, substitúe o teu taboleiro, aspecto incluído, como calquera taboleiro completo.

### Propiedades de estilo
- **Toda a xanela**: `background`, `separator-color`, `text-color`, `font-name`, `font-size`, `text-weight`, `caption` (rótulo que se mostra entre os separadores), `title` e `credits` (móstranse ao importar o taboleiro), `language` (o idioma para o que se fixo o taboleiro, por exemplo `language:Galician`; SimonSays non importa o taboleiro mentres o idioma configurado sexa outro, e dille a que idioma cambiar na configuración, `F2`).
- **Todos os botóns de categoría** (prefixo `category-`) e **todos os botóns de frase** (prefixo `phrase-`): `background`, `text-color`, `width`, `height`, `corner-radius`, `border-width`, `margin`, `icon-position` (`left`/`right`/`top`/`bottom`/`center`), `icon-size`, `font-name`, `font-size`, `text-weight`, `text-layout`.

Con `icon-position` en `center`, a icona colócase no centro do botón e o texto ocúltase (unha tecla só con icona); usa `top` se queres a icona arriba e o nome debaixo. Se queres texto sobre unha icona centrada, engade un `text-layout` explícito (por exemplo `text-layout:bottom`) e mostrarase nesa posición.

`text-layout` admite unha palabra clave horizontal (`left`/`center`/`right`) e/ou unha vertical (`top`/`middle`/`bottom`), ou `smart`. O valor predeterminado é smart: o texto céntrase no espazo que deixa libre a icona (icona arriba → rótulo centrado debaixo; centrado en todo o botón cando non hai icona).

### Exemplo: un taboleiro Fitzgerald Key con códigos de cor
Para colorear seis grupos de palabras, edita cada categoría (`F4`) e dálle unha cor de fondo. Estas son as cadeas exactas que escribes no diálogo de edición:

```
🧍##Persoas::background:#FFD966;
🏃##Accións::background:#93C47D;
📏##Descritivos::background:#6FABDC;
🍎##Substantivos::background:#F6B26B;
🕒##Miscelánea::background:#FFFFFF;
👋##Social::background:#B4A7D6;
```

Cada cor colorea tamén as frases desa categoría, así que todo o taboleiro queda codificado por cores. Os retoques de todo o taboleiro (como poñer o emoji de cada categoría arriba co seu nome debaixo, e colorear os separadores) veñen dun estilo de taboleiro (`Ctrl + F4`) como:

```
category-icon-position:top;separator-color:#B4A7D6;
```

Para un taboleiro Fitzgerald Key completo, con pictogramas en cada tecla e as frases organizadas por grupos de palabras, créao en [SimonSays Web](https://github.com/juan-rey/simonsays-web) e impórtao con `F9`.

Os estilos viaxan coas túas categorías: ao exportar (`F10`) inclúense, e ao importar (`F9`) tráense, así que un taboleiro con estilo pode compartirse como un único ficheiro.

## Importar e exportar categorías
Os taboleiros compártense como ficheiros `.ssc` (categorías e frases) ou `.ssz` (o mesmo, máis os ficheiros de son e de icona que usan, nun único ficheiro). Tamén podes crealos e editalos visualmente no navegador con [SimonSays Web](https://github.com/juan-rey/simonsays-web).

- **Exportar** (`F10`): cando se che pregunte, escolle exportar todas as categorías ou só a seleccionada; o diálogo de gardar ábrese de forma predeterminada en `%LocalAppData%\SimonSays\boards\`, aínda que podes gardar en calquera sitio. SimonSays garda automaticamente un `.ssz` cando as túas categorías ou frases usan ficheiros de son ou de icona que pode atopar (os sons `.mid`/`.midi` non se inclúen). Exportar todas as categorías inclúe o estilo do taboleiro; exportar unha categoría inclúe só o estilo desa categoría. As mensaxes de éxito ou de erro aparecen no idioma seleccionado.
- **Importar** (`F9`): escolle un ficheiro `.ssc` ou `.ssz`.
  - Un ficheiro con **varias categorías ou cun estilo de taboleiro** é un taboleiro completo: **substitúe todas as categorías e frases do idioma actual**. Antes, o teu taboleiro anterior gárdase como `backup.ssz` (consulta [Desfacer cambios](#desfacer-cambios)).
  - Un ficheiro cunha **soa categoría** engádese ao teu taboleiro; se xa existe unha categoría co mesmo nome, preguntaráseche se queres sobrescribila. O mesmo ocorre cando o ficheiro só lle dá nome ao seu taboleiro (`title`, `credits` ou `language`, sen cores, tamaños nin fontes): eses datos nunca substitúen o teu taboleiro nin o seu aspecto.
  - Os ficheiros de son e de icona dun `.ssz` instálanse no cartafol de recursos, e a xanela de categorías axusta o seu tamaño ao taboleiro importado. As mensaxes de éxito ou de erro aparecen no idioma seleccionado.

## Desfacer cambios
SimonSays garda cada cambio ao instante e non ten un comando xeral de Desfacer. Estas son as formas de volver atrás:

- **Desfacer unha importación**: antes de que unha importación substitúa o teu taboleiro, SimonSays garda o taboleiro anterior como `backup.ssz` en `%LocalAppData%\SimonSays\boards\`. Para desfacer a importación, preme `F9` e escolle `backup.ssz`: o teu taboleiro anterior volve tal como estaba, categorías e aspecto, sen máis preguntas. Só se conserva o último taboleiro substituído: cada nova importación dun taboleiro completo sobrescribe a copia.
- **Desfacer un cambio do estilo do taboleiro**: despois de editar o estilo do taboleiro con `Ctrl + F4`, SimonSays pregunta "Queres reverter os cambios?"; escolle `Aceptar` para volver ao estilo anterior.
- **Desfacer un cambio de categoría ou de frase**: os cambios feitos con `F4`, `F7` ou `F8` non se poden desfacer automaticamente. Antes de facer cambios grandes nun taboleiro que che importa, expórtao (`F10`); importar ese ficheiro máis adiante (`F9`) lévate de volta a ese punto.

### Restaurar as frases predeterminadas
SimonSays nunca sobrescribe as túas propias categorías e frases, así que as novas frases predeterminadas dunha actualización só aparecen nos idiomas que aínda non usaches. Podes volver ás categorías e frases integradas en calquera momento; por exemplo, para empezar de novo ou para recibir as frases novas dunha actualización. Isto **substitúe todo o taboleiro do idioma actual**:

1. Fai clic no botón 📂 xunto a `Categorías` (ou preme `F9` na xanela de categorías). Ábrese o cartafol de taboleiros.
2. Escolle o taboleiro de frases predeterminadas do teu idioma; en galego, `SimonSays_Frases_predeterminadas_(Galego).ssc` (nos demais idiomas ten a mesma forma, nese idioma; por exemplo, `SimonSays_default_phrases_(English).ssc`).
3. Confirma a importación. Antes, o teu taboleiro actual gárdase como `backup.ssz`, así que aínda podes recuperalo (consulta [Desfacer cambios](#desfacer-cambios)).

Este taboleiro contén sempre as frases predeterminadas da versión de SimonSays que estás a usar: SimonSays actualízao no cartafol de taboleiros ao iniciarse e ao cambiar de idioma. Se queres conservar o teu propio taboleiro, expórtao antes (`F10`) co nome que prefiras; `backup.ssz` sobrescríbese na seguinte importación.

Se o ficheiro non está no cartafol de taboleiros, queda un último recurso: pecha SimonSays (menú da bandexa → `Saír`), abre o Editor do Rexistro (`regedit`), elimina a clave co nome do teu idioma en inglés (por exemplo `Galician`) en `HKEY_CURRENT_USER\SOFTWARE\SimonSays\Phrases\` e volve iniciar SimonSays. As categorías e frases predeterminadas dese idioma créanse de novo, sen copia de seguridade.

Para `Portuguese (Brazil)`: se tamén existe unha clave `Portuguese`, SimonSays copia ese taboleiro en `Portuguese (Brazil)` en lugar de crear as frases predeterminadas brasileiras, así que elimina tamén a clave `Portuguese` (expórtaa antes se a usas).

## Onde garda SimonSays os teus ficheiros
- **As categorías, as frases e a configuración** gárdanse no rexistro de Windows, en `HKEY_CURRENT_USER\SOFTWARE\SimonSays` (o taboleiro de cada idioma, en `Phrases\<Language>`).
- `%LocalAppData%\SimonSays\resources\`: os teus ficheiros de son e de icona, e os instalados desde taboleiros `.ssz`; un taboleiro cuxo estilo define `title` ou `resource-folder` ten o seu propio subcartafol.
- `%LocalAppData%\SimonSays\boards\`: o cartafol predeterminado para os taboleiros exportados, a copia automática `backup.ssz` e os taboleiros que veñen con SimonSays: as frases predeterminadas de cada idioma, e categorías listas para usar que podes engadir ao teu taboleiro con `F9` ou co botón 📂: Si / Non, Dor e corpo, Soletrear, Xogos, Preguntas, Coidado diario, un Modelo de reunión e unha Proba de audición, co nome no teu idioma, e `Spelling_NATO`, `Alert_tones` e `Morse_code` para calquera idioma. Cada unha engade unha categoría sen substituír o teu taboleiro. Os taboleiros do teu idioma cópianse alí ao iniciar e cando cambias o idioma na configuración, sempre que o taboleiro predeterminado do teu idioma falte ou cambiase, e substitúen as copias do cartafol; por iso, garda cun nome distinto os taboleiros que edites.
- `%LocalAppData%\SimonSays\debug\`: ficheiros de diagnóstico (como `hid_dump.txt`) que o soporte pode pedirche.

Para abrir un destes cartafoles, pega a súa ruta (por exemplo `%LocalAppData%\SimonSays`) na barra de enderezos do Explorador de ficheiros.

## Solución de problemas
- **Falla a inicialización da voz**: comproba que SAPI está instalado e que hai unha voz compatible.
- **Non hai voz para o meu idioma, ou as frases dinse con outro acento**: SimonSays usa as voces instaladas en Windows. Engade unha voz para o teu idioma na configuración de Windows (**Hora e idioma → Voz → Xestionar voces**) e escóllea despois na configuración de SimonSays (`F2`). Pode que algunhas voces "naturais" máis recentes de Windows non estean dispoñibles para outras aplicacións.
- **Non se oen os sons**: comproba a ruta e a extensión do ficheiro, e que os delimitadores `♫` van en parella.
- **A barra de tarefas xa non se oculta automaticamente con SimonSays aberto**: é intencionado. SimonSays está na barra de tarefas, así que a mantén visible mentres funciona e volve activar "Ocultar automaticamente a barra de tarefas" cando o pechas. Se SimonSays se pechou de forma inesperada (por exemplo, por un corte de luz), vólvese activar a próxima vez que peches SimonSays.
- **O botón Widgets tapa a barra de SimonSays** (Windows 11): desactiva Widgets en **Configuración → Personalización → Barra de tarefas** de Windows. SimonSays ofrece abrir esa páxina unha vez, a primeira vez que detecta Widgets.
- **A xanela de categorías é demasiado pequena ou demasiado grande**: fai dobre clic nunha zona baleira ou no bordo da xanela para axustala ao seu contido, ou usa o zoom con `F11` / `F12`.
- **Desapareceron as miñas categorías ou frases**: comproba o idioma na configuración (`F2`); cada idioma ten o seu propio taboleiro. Se unha importación substituíu o teu taboleiro, consulta [Desfacer cambios](#desfacer-cambios).
- **"SimonSays non puido gardar as túas categorías e frases"**: o teu cambio segue a mostrarse, pero pode perderse ao pechar SimonSays. Exporta o teu taboleiro con `F10` axiña para conservar unha copia; se se repite, comunícao. A mensaxe aparece unha soa vez, non en cada cambio, para que nunca estorbe o uso do taboleiro.
- **Erros de importación ou exportación**: revisa os permisos do ficheiro e a localización no disco; téntao de novo.
- **Non se aplican os cambios de idioma ou de voz**: volve abrir a configuración e confirma a selección. Se os cambios seguen sen aplicarse, reinicia SimonSays e téntao de novo.
- **Informar de problemas co seguidor ocular**: se o soporte che pide un diagnóstico, abre a xanela de Mirada / Clic por fixación (`F3`) e fai clic en `Gardar informe de diagnóstico...`. O informe (`hid_dump.txt`) gárdase en `%LocalAppData%\SimonSays\debug\` e ábrese ese cartafol. Só se crea cando o pides.

## Comentarios, soporte e privacidade
- O diálogo `Sobre` (menú da bandexa) mostra a versión, a descrición e o copyright.
- Comparte suxestións, problemas ou como funciona SimonSays co teu seguidor ocular mediante a opción `Dános a túa opinión` do menú da bandexa, que abre un [formulario de comentarios](https://forms.gle/KMbpEDYmwnFJUhgy8).
- Para informar de erros e obter soporte técnico, visita o [repositorio de SimonSays en GitHub](https://github.com/juan-rey/SimonSays).
- SimonSays é libre e de código aberto, coa Open Software License 3.0 (OSL-3.0). Non mostra publicidade nin recolle os teus datos.
