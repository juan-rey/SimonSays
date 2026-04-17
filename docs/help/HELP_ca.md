# Guia d'Ajuda de SimonSays

## Visió general
SimonSays és un assistent de text a veu (TTS) per a Windows, creat per Juan Rey Saura, optimitzat per a parla ràpida, biblioteques de frases i reproducció de sons en línia. Suporta múltiples idiomes, veus personalitzables i funcions pensades per a una comunicació eficient.

SimonSays utilitza l'espai lliure a l'esquerra de la barra de tasques de Windows, proporcionant accés ràpid a una dotzena de categories de frases d'ús freqüent (i personalitzables) amb un parell de clics.

## Dreceres de teclat
- `F1`: Obrir Ajuda.
- `F2`: Obrir Configuració.
- `F3`: Afegir després de la selecció actual (context categoria/frase).
- `F4`: Editar la selecció actual.
- `F5` / `F6`: Moure a la selecció anterior / següent en llistes.
- `F8`: Eliminar la selecció actual.
- `F9`: Importar categories.
- `F10`: Exportar categories.
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
- Afegir: selecciona categoria → (drecera: `F3`) diàleg `Afegeix una nova categoria` → escriu el nom → `D'acord`.
- Reanomenar: selecciona categoria → (drecera: `F4`) `Canvia el nom de la categoria` → canvia el nom → `D'acord`.
- Moure selecció: `F5`/`F6` per moure a l'element anterior/següent.
- Eliminar: selecciona categoria → (drecera: `F8`) `Eliminar categoria`; confirma l'eliminació de la categoria i les seves frases.

### Gestionar frases
- Afegir: selecciona frase → (drecera: `F3`) `Afegeix una nova frase` → escriu el text → `D'acord`.
- Editar: selecciona frase → (drecera: `F4`) `Edita la frase` → actualitza el text → `D'acord`.
- Moure selecció: `F5`/`F6` per moure a la frase anterior/següent.
- Eliminar: selecciona frase → (drecera: `F8`) `Eliminar frase`; confirma.

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

## Menú de la icona de safata
- `Mostra` / `Amaga` la interfície principal.
- `Configuració`.
- `Quant a` mostra la versió i els crèdits.
- `Web` obre la pàgina del projecte (si està configurada).
- `Danos tu opinión` obre un formulari de comentaris al navegador per defecte.
- `Sortir` tanca SimonSays.

Pots moure la finestra emergent de la safata i altres diàlegs arrossegant la barra de títol; redimensiona la finestra de Categories arrossegant les vores, i activa `Recorda la mida de la finestra de categories` a Configuració per conservar la mida.

## Barrejar veu i sons
- Utilitza el delimitador `♫` (`SOUND_NOTE_DELIMITER`) per inserir noms de fitxers d'àudio al text:
  - Exemple: `Hola ♫notification.wav♫, si us plau espera.`
  - El text fora dels delimitadors es parla; el text entre delimitadors es tracta com un fitxer de so i es reprodueix en línia.
- Àudio admès: `.wav`, `.mid`, `.midi`, `.mp3`.
- Utilitza rutes absolutes o relatives accessibles al procés. Aparella delimitadors per evitar que el text final sigui tractat com a àudio.

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
