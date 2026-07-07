# Guia d'Ajuda de SimonSays

## Visió general
SimonSays és un assistent de text a veu (TTS) per a Windows, creat per Juan Rey Saura, optimitzat per a parla ràpida, biblioteques de frases i reproducció de sons en línia. Suporta múltiples idiomes, veus personalitzables i funcions pensades per a una comunicació eficient.

SimonSays utilitza l'espai lliure a l'esquerra de la barra de tasques de Windows, proporcionant accés ràpid a una dotzena de categories de frases d'ús freqüent (i personalitzables) amb un parell de clics.

## Dreceres de teclat
- `F1`: Obrir Ajuda.
- `F2`: Obrir Configuració.
- `F3`: Configuració de Mirada / Clic per fixació.
- `F4`: Editar la selecció actual.
- `F5` / `F6`: Moure a la selecció anterior / següent en llistes.
- `F7`: Afegir després de la selecció actual (context categoria/frase).
- `F8`: Eliminar la selecció actual.
- `F9`: Importar categories.
- `F10`: Exportar categories.
- `F11` / `Ctrl -`: Allunyar el zoom (finestra de categories).
- `F12` / `Ctrl +`: Apropar el zoom (finestra de categories).
- `Ctrl 0`: Restablir el zoom.
- Botó `Reproduir>`: Iniciar reproducció.

## Inici i ús bàsic
1. Inicia SimonSays; apareix a la safata del sistema de Windows.
2. Per parlar, pots:
   - Escriure text personalitzat a la caixa d'entrada i prémer el botó `Reproduir>` (o `Enter`).
   - Fer clic en una o diverses frases desades a la finestra de Categories.
3. Durant la reproducció, el botó mostra `>` per indicar que està actiu. Pots prémer-lo de nou per aturar-lo instantàniament.

## Finestra de Categories (frases)
- Obre-la amb el botó `Categories`. Pots redimensionar-la o recordar-ne la mida des de Configuració.
- Selecciona una categoria a dalt i frases a baix.
- Un clic en una frase la reprodueix automàticament si `Parla la frase immediatament en seleccionar-la` està activat; si no, carrega la frase a la caixa d'entrada principal sense parlar.

### Gestionar categories
- Reanomenar: selecciona categoria → (drecera: `F4`) `Canvia el nom de la categoria` → canvia el nom → `D'acord`.
- Moure selecció: `F5`/`F6` per moure a l'element anterior/següent.
- Afegir: selecciona categoria → (drecera: `F7`) diàleg `Afegeix una nova categoria` → escriu el nom → `D'acord`.
- Eliminar: selecciona categoria → (drecera: `F8`) `Eliminar categoria`; confirma l'eliminació de la categoria i les seves frases.



#### Definir o canviar la icona d’una categoria
Cada categoria pot mostrar un emoji o un fitxer `.ico` personalitzat al seu botó. La icona s’especifica al diàleg Afegir/Editar utilitzant el prefix separador `##`:

```
<icona>##<nom de categoria>
```

- **Emoji**: introdueix l’emoji seguit de `##` i del nom.
  - Exemple: `🙂##Salutacions`
- **Fitxer d’icona** (`.ico`): introdueix la ruta del fitxer (absoluta o relativa a la carpeta de SimonSays) seguida de `##` i del nom.
  - Exemple: `C:\icons\smile.ico##Salutacions`
- **Sense icona**: introdueix només el nom de la categoria, sense el prefix `##`.
  - Exemple: `Salutacions`

> Nota: només s’admeten fitxers `.ico` per a icones basades en fitxer. Qualsevol altra extensió s’ignora i no es mostra cap icona.

### Gestionar frases
- Editar: selecciona frase → (drecera: `F4`) `Edita la frase` → actualitza el text → `D'acord`.
- Moure selecció: `F5`/`F6` per moure a la frase anterior/següent.
- Afegir: selecciona frase → (drecera: `F7`) `Afegeix una nova frase` → escriu el text → `D'acord`.
- Eliminar: selecciona frase → (drecera: `F8`) `Eliminar frase`; confirma.



#### Definir o canviar la icona d’una frase
Les frases segueixen la mateixa convenció de prefix d’icona que les categories, amb el separador opcional `##` abans del text de la frase:

```
<icona>##<text de la frase>
```

- **Emoji**: `👍##Sona bé!`
- **Fitxer d’icona**: `C:\icons\check.ico##Sona bé!`
- **Sense icona**: `Sona bé!`

Les frases també poden incloure un fitxer d’àudio en línia. Quan combines icona, text i fitxer d’àudio, el format complet és:

```
<icona>##<text de la frase>::<fitxer d’àudio>
```

- Exemple: `🔔##Atenció::notification.wav`

El prefix `##` i el sufix `::` són opcionals i independents; pots utilitzar qualsevol combinació:
- Només prefix d’icona: `🔔##Atenció`
- Només sufix d’àudio: `Atenció::notification.wav`
- Tots dos: `🔔##Atenció::notification.wav`
- Cap dels dos: `Atenció`

## Configuració (F2)
Obre Configuració (`F2`) per controlar:
- **Text predeterminat**: activa/desactiva `Insereix el text predeterminat automàticament` i personalitza la frase per defecte.
  - `Mostra el teclat tàctil` — obre el teclat tàctil en pantalla quan la caixa d'entrada rep el focus, posicionat a prop de la finestra principal.
- **Idioma de la interfície**: tria l'idioma de la UI (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **Veu**: selecciona una veu SAPI instal·lada segons la teva preferència.
- **Volum/Velocitat**: ajusta `Volum (10-100)` i `Velocitat (-10 a 10)`.
- `Prova la veu`: prova la veu seleccionada.
- **Finestra de categories**:
  - `Minimitza automàticament la finestra de categories` després de seleccionar.
  - `Recorda la mida de la finestra de categories`.
- **Comportament de reproducció**:
  - `Parla la frase immediatament en seleccionar-la` (auto-reproducció amb clic).
  - `Atura la reproducció anterior` — atura automàticament qualsevol reproducció en curs abans d'iniciar una nova frase.
  - `Augmenta el volum de SimonSays en reproduir` — puja el volum mestre del sistema mentre parla, restaurant-lo quan acaba la reproducció.
  - `Redueix temporalment altres sons en reproduir` — redueix el volum de les altres aplicacions mentre parla, restaurant-lo quan acaba la reproducció.
- `D'acord` desa els canvis; `Cancel·lar` els descarta.

## Mirada / Clic per fixació (F3)
Activa frases i botons sense fer servir les mans mantenint la mirada (o el cursor del ratolí) sobre ells durant un temps determinat. Aquesta funció és nova: necessita proves i comentaris; si us plau, explica'ns com funciona amb el teu seguidor ocular mitjançant l'opció `Comentaris` del menú de safata.

Configuracions compatibles:
- **Qualsevol programari de control ocular que mogui el cursor del ratolí** (Irisbond EasyClick, Tobii Dynavox Computer Control, modes de cursor del Control ocular de Windows, OptiKey, ...): el clic per fixació segueix el cursor.
- **Lectura directa de la mirada, sense necessitat de moure el cursor**: Irisbond Hiru (mode HID) i seguidors Tobii a través del seu propi programari instal·lat — verificat amb Tobii Eye Tracker 4C i Tobii Dynavox PCEye 5; s'espera que Tobii Eye Tracker 5 funcioni, però encara no s'ha provat.

Obre la configuració de Mirada / Clic per fixació (`F3`) per ajustar (els controls d'aquest diàleg es mostren en anglès):
- **Mode d'activació**: tria entre `Automatic`, que detecta la presència i el comportament del dispositiu i decideix si activar el clic per fixació mitjançant el cursor del ratolí o mitjançant detecció de mirada (si es detecta un dispositiu de seguiment ocular o programari relacionat, el clic per fixació s'activa; si no se'n detecta cap, resta desactivat), `Mouse cursor` per activar sempre el clic per fixació mitjançant el cursor, `HID eye tracker` per activar-lo sempre mitjançant detecció de mirada, o `Off` per desactivar-lo sempre, independentment dels dispositius presents.
- **Temps de fixació** (`Dwell time`): durada (en mil·lisegons) necessària per disparar un clic en mirar un element. Mentre la fixació és en curs es mostra un indicador de progrés.
- **Radi de tolerància** (`Tolerance radius`): tolerància de la mirada (en píxels) per permetre petits moviments de l'ull sense cancel·lar el clic per fixació.
- **Temps d'espera** (`Cooldown`): mil·lisegons després d'una activació durant els quals no es dispararà cap altre clic per fixació (evita clics repetits accidentals).
- **Color de progrés** (`Progress color`): color de l'indicador de progrés de la fixació (per defecte, el color d'èmfasi de Windows).
- **Detecta com uses el seguidor** (`Detect how you use the tracker`): dos botons que ajuden l'aplicació a detectar si mous el cursor amb un seguidor ocular o si simplement fas clic als botons.
- **Detectat** (`Detected`): mostra el tipus de dispositiu detectat i informació addicional sobre el programari relacionat.
- Un botó `Reset` (a baix a l'esquerra) restaura tots els ajustos de fixació als valors predeterminats.
- El botó `D'acord` desa els canvis; `Cancel·lar` els descarta.

## Menú de la icona de safata
- `Mostra` / `Amaga` la interfície principal.
- `Configuració`.
- `Gaze / Dwell-click` (Mirada / Clic per fixació).
- `Quant a` mostra la versió i els crèdits.
- `Web` obre la pàgina del projecte (si està configurada).
- `Comentaris` obre el [formulari de comentaris](https://forms.gle/KMbpEDYmwnFJUhgy8) al navegador per defecte.
- `Sortir` tanca SimonSays.

Pots moure la finestra emergent de la safata i altres diàlegs arrossegant la barra de títol; redimensiona la finestra de Categories arrossegant les vores, i activa `Recorda la mida de la finestra de categories` a Configuració per conservar la mida.

## Barrejar veu i sons
- Utilitza el delimitador `♫` (`SOUND_NOTE_DELIMITER`) per inserir noms de fitxers d'àudio al text:
  - Exemple: `Hola ♫notification.wav♫, si us plau, espera un moment.`
  - El text fora dels delimitadors es parla; el text entre delimitadors es tracta com un fitxer de so i es reprodueix en línia.
- Àudio admès: `.wav`, `.mid`, `.midi`, `.mp3`.
- Utilitza rutes absolutes o relatives accessibles al procés. Aparella delimitadors per evitar que el text final sigui tractat com a àudio.



### Ordre de cerca de fitxers de so i d’icona
Quan un nom de fitxer de so o d’icona no conté una lletra d’unitat o una ruta completa, SimonSays el cerca en l’ordre següent i utilitza la primera coincidència trobada:

1. **Carpeta AppData** — `%APPDATA%\SimonSays\` (específica de l’usuari; posa-hi els fitxers per mantenir-los separats de la instal·lació de l’aplicació).
2. **Directori de treball** — el directori de treball actual del procés (només es cerca si és diferent de la carpeta de l’executable).
3. **Carpeta de l’executable** — la carpeta que conté `SimonSays.exe`.

Si el fitxer no es troba en cap d’aquestes ubicacions, s’utilitza el so de reserva integrat.

> **Consell:** és recomanable posar els teus fitxers de so i icona personalitzats a `%APPDATA%\SimonSays\`; així sobreviuran a les actualitzacions i seran específics de l’usuari.

## Importar i exportar categories
- Exportar: pots exportar totes les categories o només la seleccionada quan se't demani. Els missatges d'èxit/error apareixen en l'idioma seleccionat (drecera: `F10`).
- Importar: tria un fitxer; si la categoria existeix, se't demanarà sobreescriure-la. Els missatges d'èxit/error apareixen en l'idioma seleccionat (drecera: `F9`).

## Resolució de problemes
- **Error d'inicialització de veu**: verifica que SAPI estigui instal·lat i que hi hagi una veu compatible.
- **No es reprodueixen sons**: confirma la ruta/extensió i els delimitadors `♫` aparellats.
- **Errors d'importació/exportació**: comprova els permisos i la ubicació del disc; reintenta.
- **Idioma UI no aplicat**: reobre Configuració i confirma la selecció d'idioma; assegura't que existeixin cadenes de localització per a l'idioma triat.

## Quant a
- El diàleg `Quant a` mostra la versió, la descripció i el copyright.
