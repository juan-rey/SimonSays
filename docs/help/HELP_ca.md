# Guia d'ajuda de SimonSays

## Visió general
SimonSays és un assistent de text a veu (TTS) per a Windows creat per Juan Rey Saura. Va néixer com una eina per a persones amb ELA i altres persones que ja no poden parlar, perquè poguessin comunicar-se ràpidament sense deixar el que estaven fent a l'ordinador. Amb el temps va quedar clar que el mateix disseny serveix com a eina general de CAA (comunicació augmentativa i alternativa) per a qualsevol persona a qui costi parlar o que no pugui fer-ho; per exemple, per disàrtria, una laringectomia, apràxia de la parla, un ictus o una lesió cerebral. Està optimitzat per parlar ràpid, fer servir biblioteques de frases i reproduir sons dins del text. Admet diversos idiomes, veus configurables i funcions pensades per a una comunicació eficient.

SimonSays fa servir l'espai lliure de la barra de tasques de Windows i dona accés ràpid a una dotzena de categories de frases d'ús freqüent (i personalitzables) amb només un parell de clics. Com que mai no tapa les altres finestres, pots continuar al navegador, als teus documents o en una videotrucada i parlar sempre que ho necessitis: amb el teclat, amb el ratolí o sense mans, amb un seguidor ocular.

Casos d'ús:
- Ús intensiu de diverses aplicacions de Windows sense perdre de vista la comunicació.
- Taulers creats per logopedes per a persones amb discapacitat de la parla.
- Reunions en línia amb [Virtual CABLE](https://vb-audio.com/Cable/) (consulta [Fer servir SimonSays en reunions en línia](#fer-servir-simonsays-en-reunions-en-línia)).

SimonSays no pretén substituir programes de comunicació complets com Mind Express, TD Communicator, Grid 3 o OptiKey, ni la valoració i el suport professional que acostumen a acompanyar-los. Fa una feina més concreta: parlar ràpidament des de la barra de tasques mentre continues treballant en altres aplicacions. Pot funcionar al costat d'aquests programes; per exemple, OptiKey o un altre programari de control ocular pot moure el cursor mentre SimonSays diu les teves frases.

SimonSays és de codi obert i gratuït (lliure com en llibertat, no només com en preu), i continuarà sent-ho: tothom el pot fer servir, estudiar, adaptar i compartir. No té publicitat ni recull dades.

SimonSays s'acosta a la versió 1.0. Abans d'aquesta fita necessita gent que el provi en el seu dia a dia, amb les seves veus, taulers i seguidors oculars: si alguna cosa falla, resulta incòmoda o hi falta, explica-ho (consulta [Comentaris, suport i privadesa](#comentaris-suport-i-privadesa)). Cada comentari ajuda.

## Contingut
- [Primers passos](#primers-passos)
- [Finestra de categories](#finestra-de-categories)
- [Frases predeterminades](#frases-predeterminades)
- [Dreceres](#dreceres)
- [Configuració (F2)](#configuració-f2)
- [Mirada / Clic per fixació (F3)](#mirada--clic-per-fixació-f3)
- [Fer servir SimonSays en reunions en línia](#fer-servir-simonsays-en-reunions-en-línia)
- [Menú de la icona de la safata](#menú-de-la-icona-de-la-safata)
- Editar taulers (avançat):
  - [Editar categories i frases](#editar-categories-i-frases) (i [quin editor fer servir](#quin-editor-fer-servir))
  - [Barrejar veu i sons](#barrejar-veu-i-sons)
  - [Personalitzar l'aspecte (estils de tauler i de categoria)](#personalitzar-laspecte-estils-de-tauler-i-de-categoria)
  - [Importar i exportar categories](#importar-i-exportar-categories)
  - [Desfer canvis](#desfer-canvis)
  - [On desa SimonSays els teus fitxers](#on-desa-simonsays-els-teus-fitxers)
- [Resolució de problemes](#resolució-de-problemes)
- [Comentaris, suport i privadesa](#comentaris-suport-i-privadesa)

## Primers passos
1. Inicia SimonSays. Apareix en un espai lliure de la barra de tasques de Windows i afegeix una icona a la safata del sistema. La barra de tasques pot ser a baix, a dalt, a l'esquerra o a la dreta de la pantalla:
   - Quan les icones de la barra de tasques estan centrades, SimonSays se situa al principi de la barra (a l'extrem esquerre, o a dalt en una barra lateral). Quan estan alineades a l'esquerra, se situa just abans de la safata del sistema.
   - En una barra de tasques petita (estreta), la barra de SimonSays és més baixa, amb una lletra més petita i els dos botons d'accés ràpid l'un al costat de l'altre. En una barra de tasques a l'esquerra o a la dreta de la pantalla, els controls s'apilen l'un sota l'altre.
   - La finestra de categories s'obre al costat de la barra de tasques, i la fletxa del botó `Categories` indica cap a on s'obre.
   - Si mous la barra de tasques o en canvies la mida mentre SimonSays està en marxa, la segueix en pocs segons.
2. Per parlar, pots:
   - Fer clic al quadre de text de la barra (o fixar-hi la mirada): s'obre un quadre d'entrada més gran al costat de la barra de tasques. Escriu el text i prem `Enter` per dir-lo; el quadre es tanca i el focus passa a `Reproduir>`, de manera que `Espai` el torna a dir (o l'atura). `Esc` tanca el quadre sense parlar i et torna a la finestra que feies servir; el text queda a la barra, on `Reproduir>` el torna a dir.
   - Fer clic en una o diverses frases desades a la finestra de categories.
3. Durant la reproducció, el botó mostra `>` per indicar que està sonant. El pots tornar a prémer per aturar-lo a l'instant.

![La barra de SimonSays a la barra de tasques](img/bar.png)

Al costat del botó `Categories`, dos petits **botons d'accés ràpid** donen accés amb un clic a dues coses a les quals, si no, arribaries amb una drecera o amb el menú de la safata:

- 📂 obre un tauler desat a l'ordinador; és la mateixa importació que `F9`, però comença a la carpeta de taulers (`%LocalAppData%\SimonSays\boards\`), on es desen els taulers exportats i baixats (consulta [Importar i exportar categories](#importar-i-exportar-categories)).
- ⚙️ obre la configuració, igual que `F2`.

Tots dos es poden amagar amb `Mostra els botons i el menú d'accés ràpid` a la configuració (`F2`), cosa que deixa més espai per al quadre d'entrada. La mateixa opció desactiva també el menú contextual (clic dret) de la finestra de categories.

Si SimonSays està amagat, fes clic a la seva icona de la safata per tornar-lo a mostrar.

## Finestra de categories
![La finestra de categories amb les frases predeterminades](img/categories.png)

- Obre-la amb el botó `Categories`. Canvia'n la mida, o activa `Recorda la mida de la finestra de categories` a la configuració per conservar-la.
- Selecciona una categoria a la llista de dalt i frases a la llista de baix.
- Un clic en una frase la diu automàticament si `Parla la frase immediatament en seleccionar-la` està activat; si no, la frase es carrega al quadre d'entrada principal sense parlar.
- Fes doble clic en una zona buida o a la vora de la finestra per ajustar-ne la mida de manera que hi càpiguen totes les categories i frases.
- Arrossega una zona buida de la finestra per moure-la, i prem `Esc` per amagar-la.
- Amplia i redueix el zoom amb `F12` i `F11` (consulta [Dreceres](#dreceres)).
- Fes clic dret en un botó o en una zona buida (o prem `Maj + F10` o la tecla de menú) per obrir un menú amb `Enganxa`, `Redueix`, `Amplia`, `Restableix el zoom`, `Ajuda`, `Configuració`, `Mirada / Clic per fixació...`, `Amaga` i `Surt de SimonSays...`, cadascun amb la seva drecera. La primera entrada mostra el títol del tauler (`Tauler sense títol` si no en té); tria-la per veure els crèdits del tauler, el seu idioma i quantes categories i frases conté. El menú només apareix mentre `Mostra els botons i el menú d'accés ràpid` està activat a la configuració.
- En un botó de categoria o de frase, el menú ofereix a més `Edita...`, `Mou a l'esquerra`, `Mou a la dreta`, `Afegeix una nova categoria...` o `Afegeix una nova frase...` i `Eliminar categoria...` o `Eliminar frase...` per a aquest botó, sense haver-lo de seleccionar abans; en una categoria, `Exporta la categoria...` pregunta si cal desar només aquesta categoria o tot el tauler. Les dreceres `F4` a `F8` es mostren quan el botó és la teva selecció actual, perquè aquestes tecles actuen sobre la selecció. En una zona buida, el menú ofereix `Afegeix una nova categoria...`, que n'afegeix una al final; també és la manera de començar un tauler buit.
- La franja entre les categories i les frases mostra un recordatori de les dreceres principals (o el rètol del tauler, si en té).

![El menú contextual d'un botó de frase](img/menu.png)

## Frases predeterminades
SimonSays inclou un conjunt de categories i frases a punt per fer servir en cadascun dels seus 18 idiomes, ordenat perquè les frases que més es necessiten requereixin menys seleccions:

- 👋🏻 Salutacions Freqüents
- 🗣️ Frases de Conversa
- 🚨 Frases d'Emergència
- 🤲 Cura i Confort
- 🏥 Frases de Salut
- 🏠 Família i Casa
- 👥 Frases Socials
- ⏰ Hora i Data
- 💼 Frases de Negocis
- ✈️ Frases de Viatge
- 🔊 Sons

Cada idioma té les seves pròpies categories i frases: quan canvies l'idioma a la configuració (`F2`), la finestra de categories passa al tauler d'aquest idioma, i els canvis que facis en un idioma no afecten els altres.

Les frases es diuen amb les veus instal·lades a Windows. Si no hi ha cap veu instal·lada per al teu idioma, les frases es mostren igualment, però es poden dir amb una altra veu (consulta [Resolució de problemes](#resolució-de-problemes)).

Pots canviar les frases predeterminades o recuperar-les més endavant; consulta [Editar categories i frases](#editar-categories-i-frases) i [Restaurar les frases predeterminades](#restaurar-les-frases-predeterminades).

## Dreceres
- `F1`: Obrir l'ajuda
- `F2`: Obrir la configuració
- `F3`: Configuració de mirada / clic per fixació
- `Enter`: Dir el text del quadre d'entrada
- `Esc`: Amagar la finestra de categories
- `F11` / `Ctrl -`: Reduir el zoom (finestra de categories)
- `F12` / `Ctrl +`: Ampliar el zoom (finestra de categories)
- `Ctrl + 0`: Restablir el zoom
- `Maj + F10` o la tecla de menú: Obrir el menú de la finestra de categories (igual que el clic dret)
- `Espai` sobre `Reproduir>`: Tornar a dir el text, o aturar-lo mentre sona

Les dreceres per editar categories, frases i taulers s'indiquen a [Editar categories i frases](#editar-categories-i-frases).

## Configuració (F2)
Obre la configuració (`F2`) per ajustar el següent. La configuració té dues columnes.

![La finestra de configuració](img/settings.png)

**Columna esquerra**
- **Text predeterminat**: activa o desactiva `Insereix el text predeterminat automàticament` i personalitza la frase predeterminada.
  - `Mostra el teclat tàctil`: obre el teclat tàctil en pantalla quan s'obre el quadre d'entrada; se situa al costat del quadre d'entrada, al costat contrari de la barra de tasques.
- `Mostra els botons i el menú d'accés ràpid`: mostra els botons 📂 (obrir un tauler) i ⚙️ (configuració) al costat del botó `Categories` de la finestra principal (consulta [Primers passos](#primers-passos)) i el menú contextual de la finestra de categories (consulta [Finestra de categories](#finestra-de-categories)). Desactiva-la per amagar-los tots i donar més espai al quadre d'entrada.
- **Finestra de categories**:
  - `Minimitza automàticament la finestra de categories` després de seleccionar.
  - `Recorda la mida de la finestra de categories`.
  - `Ajusta automàticament la finestra de categories al contingut`: ajusta la finestra de categories a les seves categories i frases cada vegada que es carreguen; per exemple, en iniciar, en canviar d'idioma o després d'una importació.
- `Mirada / Clic per fixació...`: obre la configuració de [Mirada / Clic per fixació](#mirada--clic-per-fixació-f3) damunt de la configuració, que continua oberta amb els teus canvis. Els canvis que hi facis es desen en prémer `D'acord` en aquella finestra, encara que després premis `Cancel·lar` a la configuració.

**Columna dreta**
- **Idioma**: tria l'idioma de la interfície (English, Español, العربية, Euskara, Català, 简体中文, Français, Galego, Deutsch, עברית, हिन्दी, Italiano, 日本語, 한국어, Português, Português (Brasil), Русский, Valencià). Cada idioma té també les seves pròpies categories i frases (consulta [Frases predeterminades](#frases-predeterminades)).
- **Veu**: tria una veu SAPI instal·lada que coincideixi amb el teu idioma.
- **Volum/Velocitat**: ajusta `Volum (10-100)` i `Velocitat (-10 a 10)`.
- `Prova la veu`: escolta la veu seleccionada (a través del dispositiu triat a `Sortida de so`).
- `Sortida de so`: per on reprodueix SimonSays la seva veu i els seus sons: `Predeterminat del Windows` (l'habitual) o qualsevol dispositiu connectat. Els cables d'àudio virtuals apareixen primer, que és el que necessites per a les [reunions en línia](#fer-servir-simonsays-en-reunions-en-línia). Si el dispositiu triat no està connectat, SimonSays sona pel dispositiu predeterminat de Windows i t'avisa una vegada; l'elecció es conserva, es mostra com a `(no connectat)` i es torna a fer servir tan bon punt el dispositiu torna.
- `Configuració d'escolta...`: obre els dispositius d'enregistrament de Windows, on pots activar **Escolta aquest dispositiu** per a un cable virtual (consulta [Fer servir SimonSays en reunions en línia](#fer-servir-simonsays-en-reunions-en-línia)).
- **Comportament de la reproducció**:
  - `Parla la frase immediatament en seleccionar-la` (dir automàticament amb un clic).
  - `Atura la reproducció anterior`: atura automàticament qualsevol reproducció en curs abans de començar una frase nova.
  - `Augmenta el volum de SimonSays en reproduir`: apuja el volum del dispositiu pel qual sona SimonSays (consulta `Sortida de so`) mentre parla i restaura el nivell anterior quan acaba.
  - `Redueix temporalment altres sons en reproduir`: abaixa el volum de les altres aplicacions mentre parla i el restaura quan acaba.

**A baix**
- `Ajuda` (cantonada inferior esquerra, o `F1` amb la configuració oberta) obre aquesta ajuda en l'idioma seleccionat a la llista `Idioma`; la configuració continua oberta amb els teus canvis.
- `D'acord` desa els canvis; `Cancel·lar` els descarta.

De manera predeterminada, `Parla la frase immediatament en seleccionar-la`, `Minimitza automàticament la finestra de categories`, `Ajusta automàticament la finestra de categories al contingut`, `Augmenta el volum de SimonSays en reproduir` i `Redueix temporalment altres sons en reproduir` estan activats, i `Atura la reproducció anterior` està desactivat.

## Mirada / Clic per fixació (F3)
Activa frases i botons sense fer servir les mans mantenint la mirada (o el cursor del ratolí) damunt seu durant un temps determinat. Aquesta funció és nova: necessita proves i comentaris; explica'ns com funciona amb el teu seguidor mitjançant l'opció `Dona'ns la teua opinió` del menú de la safata.

Configuracions compatibles:
- **Qualsevol programari de control ocular que mogui el cursor del ratolí** (Irisbond EasyClick, Tobii Dynavox Computer Control, els modes de cursor del Control ocular de Windows, OptiKey): el clic per fixació segueix el cursor.
- **Lectura directa de la mirada (sense haver de moure el cursor)**: Irisbond Hiru (mode HID) i els seguidors Tobii a través del seu propi programari; verificat amb Tobii Eye Tracker 4C, Tobii Eye Tracker 5 i Tobii Dynavox PCEye 5.
- **Tobii Eye Tracker 5**: el seu programari actual només instal·la un motor de 64 bits, de manera que SimonSays el llegeix a través d'un petit programa auxiliar, `SimonSaysGaze64.exe`, instal·lat al seu costat. L'auxiliar només s'inicia quan cal, no té finestra, es tanca amb SimonSays i manté la mirada al teu ordinador: mai no es desa ni s'envia enlloc.
- **Llicència de Tobii Eye Tracker 4C i Eye Tracker 5**: la llicència d'usuari final de Tobii per a aquests seguidors no permet l'ús mèdic, inclòs l'ús en hospitals i altres entorns sanitaris. Els dispositius de Tobii Dynavox, com el PCEye 5, tenen les seves pròpies condicions de llicència.
- **Programari del Tobii Eye Tracker 4C**: SimonSays llegeix el 4C a través de Tobii Core. Si connectes un Eye Tracker 5 al mateix ordinador, Windows instal·la Tobii Experience, que desactiva Tobii Core, i el 4C deixa de funcionar. Per tornar a fer servir el 4C, reinstal·la Tobii Core; la versió recomanada és `Tobii_Eye_Tracking_Core_v2.13.1.7294_x86.exe`, de la [pàgina de primers passos de Tobii](https://gaming.tobii.com/getstarted/).

On funciona el clic per fixació:
- La barra de SimonSays, la finestra de categories i els seus botons de frases i categories.
- Els botons de les finestres i preguntes de SimonSays: la configuració, la finestra de Mirada / Clic per fixació, els diàlegs d'afegir, editar i previsualitzar, i totes les confirmacions, com `Sí` / `No` o `D'acord` / `Cancel·lar`. Aquests botons són en el teu idioma i comencen a respondre a la fixació uns tres quarts de segon després d'obrir-se la finestra, perquè una pregunta que apareix just on mires no es pugui respondre sola.
- Quan es llegeix la mirada directament, el botó que mires s'il·lumina igual que sota el ratolí, de manera que pots veure el que seleccionaràs abans que comenci el progrés. Un parpelleig manté el ressaltat; apartar la mirada el treu.

![La finestra de Mirada / Clic per fixació](img/dwell.png)

Obre la configuració de Mirada / Clic per fixació (`F3`, el menú de la safata o el botó `Mirada / Clic per fixació...` de la configuració) per ajustar:
- **Mode d'activació**: tria una d'aquestes opcions:
  - `Automàtic`: detecta els dispositius presents i activa el clic per fixació amb el cursor del ratolí o amb la detecció de la mirada, segons correspongui. El programari de Tobii en segon pla (sempre en marxa un cop instal·lat) no és suficient per activar la fixació amb el cursor; un seguidor Tobii només es fa servir mentre segueix la mirada de veritat.
  - `Cursor del ratolí`: activa sempre el clic per fixació amb el cursor del ratolí.
  - `Seguidor ocular HID`: activa sempre el clic per fixació amb la detecció de la mirada (Irisbond Hiru en mode HID, o seguidors Tobii). Només la teva mirada activa els botons: mentre no arriba cap mirada (seguiment apagat o seguidor desconnectat), no s'activa res per fixació i el ratolí funciona amb normalitat.
  - `Desactivat`: desactiva sempre el clic per fixació.
  - **Retard d'enganxament**: quant de temps, en mil·lisegons, ha de reposar la mirada (o el cursor) sobre un botó abans que comenci la barra de progrés (150 de manera predeterminada). Així, en passar la mirada per damunt dels botons no es veu cap progrés; el botó s'activa després del retard d'enganxament més el temps de fixació. Posa-ho a 0 perquè el progrés comenci immediatament.
  - **Temps de fixació**: la durada (en mil·lisegons) necessària per fer clic en mirar un element (500 de manera predeterminada). Mentre dura la fixació es mostra un indicador de progrés.
  - **Radi de tolerància**: la tolerància de la mirada (en píxels) que permet petits moviments de l'ull sense deixar de comptar la fixació.
  - **Temps d'espera**: mil·lisegons després d'una activació durant els quals el clic per fixació no es pot tornar a activar (evita clics repetits per accident).
  - **Color de progrés**: el color de l'indicador de progrés de la fixació (de manera predeterminada, el color d'èmfasi de Windows).
  - **Apartar-se abans que la fixació torne a activar** (activat de manera predeterminada): després d'un clic per fixació, cap botó no es pot tornar a activar per fixació fins que el punter o la mirada s'allunyen d'aquell punt (més enllà del radi de tolerància) o apartes la vista un moment. Quedar-se sobre un botó l'activa una sola vegada, de manera que una frase llarga no es repeteix abans d'acabar. Els clics del ratolí no se'n veuen afectats.
  - **Detecta com fas servir el seguidor**: dos botons que ajuden l'aplicació a detectar si mous el cursor del ratolí amb un seguidor ocular o si simplement fas clic als botons.
  - **Detectat**: mostra el tipus de dispositiu detectat i més informació sobre el programari relacionat.
  - `Desa l'informe de diagnòstic...` (sota Detectat): escriu un informe sobre el teu seguidor ocular i el seu programari a `hid_dump.txt` i n'obre la carpeta, perquè el puguis enviar al suport.
  - Un botó `Restableix` (a baix a l'esquerra) torna tots els ajustos de fixació als valors predeterminats.
  - Un botó `D'acord` desa els canvis; `Cancel·lar` els descarta.

## Fer servir SimonSays en reunions en línia
SimonSays pot parlar directament en una videotrucada (Teams, Zoom, Google Meet…), de manera que els altres participants senten les teves frases com si sortissin del teu micròfon. Necessites un cable d'àudio virtual, com el gratuït [VB-Audio Virtual Cable](https://vb-audio.com/Cable/):

1. Instal·la el cable d'àudio virtual i reinicia Windows si t'ho demana.
2. Envia el so de SimonSays al cable: obre la configuració (`F2`) i tria `CABLE Input` a `Sortida de so` (els cables virtuals apareixen primer). Prem `Prova la veu` per comprovar-ho: ja no l'hauries de sentir pels altaveus, perquè ara va al cable.
3. A la teva aplicació de reunions, tria `CABLE Output` com a micròfon.
4. Per sentir també tu SimonSays, prem `Configuració d'escolta...` a la configuració. S'obren els dispositius d'enregistrament de Windows: selecciona `CABLE Output`, obre **Propietats → Escolta**, marca **Escolta aquest dispositiu**, tria els teus altaveus o auriculars a **Reprodueix mitjançant aquest dispositiu** i prem `D'acord`. Només ho has de fer una vegada.

SimonSays no pot activar **Escolta aquest dispositiu** per tu: és un ajust del costat d'enregistrament del cable que Windows només et deixa canviar a tu, de manera que `Configuració d'escolta...` et porta directament a la finestra correcta.

Convé saber:
- Deixa el dispositiu de reproducció predeterminat de Windows als teus altaveus o auriculars, no a `CABLE Input`. Si **Escolta aquest dispositiu** reprodueix a "Dispositiu de reproducció predeterminat" mentre aquest dispositiu predeterminat és el cable, el so torna a entrar al cable.
- Si a la reunió se senten les teves frases però no els efectes de so (aplaudiments, un timbre), la supressió de soroll de l'aplicació de reunions els elimina; abaixa o desactiva la supressió de soroll a l'aplicació.
- Si el cable es desinstal·la o es desconnecta, SimonSays sona pel dispositiu predeterminat de Windows i t'avisa una vegada; torna al cable tan bon punt torna a estar disponible.
- Per tenir frases preparades per a una trucada, afegeix la categoria **Plantilla de reunió**: prem `F9` o el botó 📂 i obre el tauler amb el nom en el teu idioma (`SimonSays_Plantilla_de_reunió_(Català).ssc` en català). Té salutacions, "Em sents?", una breu explicació que parles amb un programa, sí / no, d'acord / en desacord, demanar que repeteixin o parlin més a poc a poc, i comiats. Edita-la per afegir frases per a la teva reunió.
- Com a alternativa al pas 2, pots continuar dirigint SimonSays amb l'ajust per aplicació de Windows (**Configuració → Sistema → So → Mesclador de volum**), deixant `Sortida de so` a `Predeterminat del Windows`. SimonSays només apareix en aquesta llista després d'haver reproduït alguna cosa.

## Menú de la icona de la safata
- `Mostra` / `Amaga` la interfície principal
- `Configuració`
- `Mirada / Clic per fixació...`
- `Web` obre la pàgina web de SimonSays
- `Quant a` mostra la versió i els crèdits
- `Dona'ns la teua opinió` obre un [formulari de comentaris](https://forms.gle/KMbpEDYmwnFJUhgy8) al navegador predeterminat
- `Sortir` tanca SimonSays, després de preguntar

Pots moure la finestra emergent de la safata i altres diàlegs arrossegant la seva barra de títol; canvia la mida de la finestra de categories arrossegant-ne les vores, i activa `Recorda la mida de la finestra de categories` a la configuració per conservar-la.

## Editar categories i frases
Aquesta secció i les següents són per a cuidadors, logopedes i usuaris avançats que canvien les categories, frases, sons i aspecte d'un tauler. Cada canvi es desa a l'instant (consulta [Desfer canvis](#desfer-canvis)).

### Quin editor fer servir
- **A SimonSays**: canvis del dia a dia; canviar el nom, afegir, moure o eliminar categories i frases, posar-los un emoji, afegir sons, enganxar una llista de frases o ajustar un color. La resta d'aquesta secció explica com.
- **A [SimonSays Web](https://github.com/juan-rey/simonsays-web)**: la feina grossa amb taulers, que allà és molt més fàcil: taulers fets amb pictogrames de CAA (pot cercar a la biblioteca de pictogrames d'ARASAAC), imatges d'icona personalitzades i taulers amb estil, amb colors, mides i disposicions. Es recomana especialment per dissenyar un tauler **Fitzgerald Key**, en què cada grup de paraules té el seu propi color. És un editor visual que obre i desa taulers `.ssc`/`.ssz` directament al navegador, sense pujar-los; quan acabis, importa el tauler a SimonSays amb `F9` o amb el botó 📂. També és la manera de fer servir els taulers de SimonSays en altres plataformes.

Dreceres d'edició (finestra de categories):
- `F4`: Editar la selecció actual
- `F5` / `F6`: Anar a la selecció anterior / següent de les llistes
- `F7`: Afegir després de la selecció actual (categoria o frase, segons el context)
- `F8`: Eliminar la selecció actual
- `F9`: Importar categories
- `F10`: Exportar categories
- `Ctrl + F4`: Editar l'estil del tauler (consulta [Personalitzar l'aspecte](#personalitzar-laspecte-estils-de-tauler-i-de-categoria))
- `Ctrl + F8`: Eliminar totes les categories i les seves frases (demana confirmació dues vegades)
- `Ctrl + V` o `Maj + Insert`: Afegir el text copiat com a frases, una per paràgraf (consulta [Gestionar frases](#gestionar-frases))
- Arrossegar un fitxer `.txt`, `.ssc` o `.ssz` a la finestra: afegeix un `.txt` com a categoria nova, o importa un tauler igual que amb `F9` (consulta [Gestionar categories](#gestionar-categories))

> **Consell:** exporta el teu tauler (`F10`) abans de fer servir `Ctrl + F8` o d'importar un tauler complet (`F9`), per poder recuperar-lo.

### Gestionar categories
- Canviar el nom: selecciona la categoria → (drecera: `F4`) `Canvia el nom de la categoria` → canvia el nom → `D'acord`.
- Moure la selecció: `F5`/`F6` per anar a l'element anterior/següent en recórrer les llistes.
- Afegir: selecciona una categoria → (drecera: `F7`) diàleg `Afegeix una nova categoria` → escriu el nom → `D'acord`.
- Eliminar: selecciona la categoria → (drecera: `F8`) `Eliminar categoria`; confirma que vols eliminar la categoria i les seves frases.
- Afegir des d'un fitxer de text: arrossega un fitxer `.txt` a la finestra de categories → es crea una categoria amb el nom del fitxer i una frase per paràgraf (els paràgrafs se separen amb una línia en blanc) → revisa la previsualització → `D'acord`. Pots deixar anar diversos fitxers alhora; cadascun té la seva pròpia previsualització. Es rebutja un fitxer el nom del qual coincideixi amb una categoria existent.
- Obrir un tauler arrossegant-lo: arrossega un fitxer `.ssc` o `.ssz` a la finestra de categories → SimonSays pregunta si el vols importar; amb `Sí` s'importa igual que amb `F9` (consulta [Importar i exportar categories](#importar-i-exportar-categories)). Si el fitxer conté un tauler complet, substitueix l'actual, i abans es desa una còpia del tauler actual com a `backup.ssz`. Pots deixar anar alhora fitxers de text i de tauler; cadascun es processa per torns. Obrir un fitxer `.ssc` o `.ssz` des de l'Explorador de fitxers (doble clic) fa la mateixa pregunta.

#### Posar o canviar la icona d'una categoria
Cada categoria pot mostrar un emoji o un fitxer d'imatge personalitzat (`.ico`, `.png` o `.jpg`) al seu botó. Indica la icona al diàleg d'afegir o editar amb el prefix separador `##`:

```
<icona>##<nom de la categoria>
```

- **Emoji**: escriu l'emoji (o els emojis) seguit de `##` i el nom.
  - Exemple: `🙂##Salutacions`
- **Fitxer d'icona** (`.ico`, `.png`, `.jpg`): escriu el nom del fitxer o el camí complet seguits de `##` i el nom. Un nom de fitxer sense carpeta es cerca tal com es descriu a [Ordre de cerca dels fitxers de so i d'icona](#ordre-de-cerca-dels-fitxers-de-so-i-dicona).
  - Exemple: `C:\icons\smile.png##Salutacions`
- **Sense icona**: escriu només el nom de la categoria, sense prefix `##`.
  - Exemple: `Salutacions`

> Nota: per a les icones de fitxer només s'admeten fitxers `.ico`, `.png` i `.jpg`. Les altres extensions s'ignoren i no es mostra cap icona. Les imatges PNG conserven la transparència; les imatges molt grans es redueixen automàticament en carregar-les.

### Gestionar frases
- Editar: selecciona la frase → (drecera: `F4`) `Edita la frase` → actualitza el text → `D'acord`.
- Moure la selecció: `F5`/`F6` per anar a la frase anterior/següent.
- Afegir: selecciona una frase → (drecera: `F7`) `Afegeix una nova frase` → escriu el text → `D'acord`.
- Eliminar: selecciona la frase → (drecera: `F8`) `Eliminar frase`; confirma.
- Afegir-ne diverses alhora: copia un text amb les frases separades per una línia en blanc → selecciona una frase (o una categoria) → `Ctrl + V` → revisa la previsualització → `D'acord`. Les frases s'afegeixen després de la frase seleccionada, o al final de la categoria. Els salts de línia dins d'un paràgraf es converteixen en espais, i els marcadors d'icona `##` i d'àudio `::` funcionen igual que al diàleg `Afegeix una nova frase`.
- Afegir sons: arrossega un o diversos fitxers `.wav` o `.mp3` a la finestra de categories → revisa la previsualització → `D'acord`. Cada fitxer es converteix en una frase de so amb el nom del fitxer, afegida després de la frase seleccionada (o al final de la categoria seleccionada), i el fitxer es copia a la carpeta de recursos del tauler perquè viatgi amb ell quan l'exportis. Mai no se sobreescriu un fitxer existent: si ja hi ha un so diferent amb el mateix nom, el nou es desa com a `nom (2).wav`.

#### Posar o canviar la icona d'una frase
Les frases segueixen la mateixa convenció de prefix d'icona que les categories, amb el separador opcional `##` davant del text de la frase:

```
<icona>##<text de la frase>
```

- **Emoji**: `👍##Em sembla bé!`
- **Fitxer d'icona**: `C:\icons\check.png##Em sembla bé!`
- **Sense icona**: `Em sembla bé!`

Les frases poden incloure un fitxer d'àudio (consulta [Barrejar veu i sons](#barrejar-veu-i-sons)). Per combinar icona, text i àudio, fes servir el format:

```
<icona>##<text de la frase>::<fitxer d'àudio>
```

- Exemple: `🔔##Atenció::alert.mp3`

El prefix `##` i el sufix `::` són opcionals i independents; pots fer servir qualsevol combinació:
- Només prefix d'icona: `🔔##Atenció`
- Només sufix d'àudio: `Atenció::alert.mp3`
- Tots dos: `🔔##Atenció::alert.mp3`
- Cap: `Atenció`

## Barrejar veu i sons
- Fes servir el delimitador `♫` per inserir noms de fitxers d'àudio al text:
  - Exemple: `Hola ♫doorbell.mp3♫, espera un moment.`
  - El text fora dels delimitadors es diu; el text de dins es tracta com un fitxer de so i es reprodueix en aquell punt.
- Àudio admès: `.wav` i `.mp3`. Els fitxers MIDI (`.mid`, `.midi`) s'accepten en una frase, però no sonen; converteix-los a `.wav` o `.mp3`.
- Fes servir camins absoluts o relatius als quals el programa pugui accedir. Assegura't que els delimitadors van en parella, perquè el text final no es tracti com a àudio.
- Per escriure `♫`, copia'l d'aquesta ajuda, o selecciona una frase que tingui un so: el seu marcador `♫…♫` es col·loca al quadre d'entrada, on pots afegir-hi text al voltant.

### Sons inclosos amb SimonSays
Aquests sons s'instal·len amb SimonSays i es poden fer servir pel seu nom en qualsevol text o frase; per exemple, `♫laugh.mp3♫` al quadre d'entrada, o `Ben fet!::applause.mp3` per a un botó de frase amb el rètol "Ben fet!" que reprodueix aplaudiments:

`ahem.mp3`, `alert.mp3`, `applause.mp3`, `aww.mp3`, `beep.mp3`, `boo.mp3`, `call_bell.mp3`, `censor.mp3`, `doorbell.mp3`, `drumroll.mp3`, `evil_laugh.mp3`, `fail.mp3`, `hmmmm.mp3`, `kisses.mp3`, `laugh.mp3`, `mmm-hmm.mp3`, `nuh-uh.mp3`, `phone_ring.mp3`, `rimshot.mp3`, `scream.mp3`, `shots.mp3`, `sigh.mp3`

### Ordre de cerca dels fitxers de so i d'icona
Quan el nom d'un fitxer de so o d'icona no inclou una lletra d'unitat ni un camí complet, SimonSays el cerca en aquest ordre i fa servir la primera coincidència:

1. **La subcarpeta de recursos del tauler actual**: si l'estil del tauler defineix `title` o `resource-folder`, la seva pròpia subcarpeta dins de la carpeta de recursos (consulta [Personalitzar l'aspecte](#personalitzar-laspecte-estils-de-tauler-i-de-categoria)).
2. **Carpeta de recursos**: `%LocalAppData%\SimonSays\resources\` (el lloc predeterminat per a les icones i els sons dels taulers; els paquets `.ssz` importats hi instal·len els seus fitxers).
3. **Carpeta de dades de SimonSays**: `%LocalAppData%\SimonSays\` (es manté com a alternativa per als fitxers que versions anteriors hi deixaven, abans que existís la carpeta de recursos).
4. **Directori de treball**: el directori de treball actual del procés (només es cerca si és diferent de la carpeta de l'executable).
5. **Carpeta de l'executable**: la carpeta que conté `SimonSays.exe`.

Si el fitxer no es troba en cap d'aquestes ubicacions, es fa servir el so de reserva integrat (una icona simplement no es mostra).

> **Consell:** et recomanem que posis els teus fitxers de so i d'icona a `%LocalAppData%\SimonSays\resources\`; sobreviuen a les actualitzacions de l'aplicació i són propis de cada usuari. Un tauler amb un estil `title` o `resource-folder` hi té automàticament la seva pròpia subcarpeta, cosa que en separa els fitxers dels d'altres taulers.

## Personalitzar l'aspecte (estils de tauler i de categoria)
Pots canviar els colors, la mida i l'estil de la finestra de categories i dels seus botons; per exemple, per crear taulers de comunicació amb codis de color com Fitzgerald Key. Els estils són opcionals: sense estils, la finestra manté l'aspecte predeterminat.

> **Consell:** tot el que segueix es pot escriure a SimonSays, cosa pràctica per a petits retocs. Per dissenyar un tauler sencer amb estil o amb pictogrames, es recomana [SimonSays Web](https://github.com/juan-rey/simonsays-web): veus el resultat mentre treballes i escriu aquests estils per tu (consulta [Quin editor fer servir](#quin-editor-fer-servir)).

Un estil és una llista de parells `propietat:valor;`. Els valors poden ser:
- **Colors**: hexadecimal `#RRGGBB`, per exemple `#FFD966`.
- **Mides**: un nombre de píxels (`96`) o un percentatge del predeterminat (`120%`).
- **Paraules clau**: per exemple `left`, `right`, `top`, `bottom`, `smart`, `normal`, `bold`.
- **Text**: un nom de font, o el text del rètol, del títol o dels crèdits.

Tot el que l'aplicació no reconeix (una propietat mal escrita o un valor incorrecte) simplement s'ignora, de manera que un estil mai no espatlla la finestra.

### Donar estil a una categoria (diàleg Edita)
Selecciona una categoria i prem `F4` (Edita); després afegeix `::` i un estil darrere del nom:

```
<icona>##<nom de la categoria>::<propietat>:<valor>;<propietat>:<valor>;
```

- Exemple: `🧍##Persones::background:#FFD966;` posa en groc el botó Persones. El color de la categoria **s'aplica també a les frases que conté**, de manera que una sola línia acoloreix tot un grup de paraules: la base d'un tauler Fitzgerald Key.
- Un estil de categoria pot definir `background` i `text-color` per al botó de la categoria, i qualsevol propietat de frase (vegeu més avall) amb el prefix `phrase-`, per exemple `phrase-corner-radius:8;`.

### Donar estil a tot el tauler (`$$board`)
L'aspecte general (el fons de la finestra, els separadors i els valors predeterminats de tots els botons de categoria i de frase) és un **estil de tauler**. Es desa amb el nom de categoria reservat `$$board` (no pots fer servir noms amb `$$` per a les teves categories).

- **Edita'l** prement `Ctrl + F4` a la finestra de categories. L'editor mostra un `propietat:valor;` per línia. En confirmar amb `D'acord`, s'aplica el nou estil i SimonSays pregunta "Vols revertir els canvis?": tria `D'acord` per tornar a l'estil anterior, o `Cancel·lar` per conservar el nou.
- **Aconsegueix-ne un** important un fitxer de categories amb estil (`.ssc` / `.ssz`). Quan importes un fitxer que conté un estil de tauler, SimonSays pregunta abans de substituir el teu i mostra el `title` i els `credits` del tauler (si en té) al missatge d'importació. Un fitxer l'estil de tauler del qual només defineix `title`, `credits`, `language` o `resource-folder` no canvia res visible, de manera que SimonSays no pregunta: si conté una sola categoria, s'afegeix i el teu aspecte es manté; si conté un tauler complet, substitueix el teu tauler, aspecte inclòs, com qualsevol tauler complet.

### Propietats d'estil
- **Tota la finestra**: `background`, `separator-color`, `text-color`, `font-name`, `font-size`, `text-weight`, `caption` (rètol que es mostra entre els separadors), `title` i `credits` (es mostren en importar el tauler), `language` (l'idioma per al qual es va fer el tauler, per exemple `language:Catalan`; SimonSays no importa el tauler mentre l'idioma configurat sigui un altre, i et diu a quin idioma has de canviar a la configuració, `F2`).
- **Tots els botons de categoria** (prefix `category-`) i **tots els botons de frase** (prefix `phrase-`): `background`, `text-color`, `width`, `height`, `corner-radius`, `border-width`, `margin`, `icon-position` (`left`/`right`/`top`/`bottom`/`center`), `icon-size`, `font-name`, `font-size`, `text-weight`, `text-layout`.

Amb `icon-position` a `center`, la icona se situa al centre del botó i el text s'amaga (una tecla només amb icona); fes servir `top` si vols la icona a dalt i el nom a sota. Si vols text sobre una icona centrada, afegeix un `text-layout` explícit (per exemple `text-layout:bottom`) i es mostrarà en aquella posició.

`text-layout` admet una paraula clau horitzontal (`left`/`center`/`right`) i/o una de vertical (`top`/`middle`/`bottom`), o `smart`. El valor predeterminat és smart: el text se centra a l'espai que deixa lliure la icona (icona a dalt → rètol centrat a sota; centrat a tot el botó quan no hi ha icona).

### Exemple: un tauler Fitzgerald Key amb codis de color
Per acolorir sis grups de paraules, edita cada categoria (`F4`) i dona-li un color de fons. Aquestes són les cadenes exactes que escrius al diàleg d'edició:

```
🧍##Persones::background:#FFD966;
🏃##Accions::background:#93C47D;
📏##Descriptius::background:#6FABDC;
🍎##Substantius::background:#F6B26B;
🕒##Miscel·lània::background:#FFFFFF;
👋##Social::background:#B4A7D6;
```

Cada color acoloreix també les frases d'aquella categoria, de manera que tot el tauler queda codificat per colors. Els retocs de tot el tauler (com posar l'emoji de cada categoria a dalt amb el nom a sota, i acolorir els separadors) vénen d'un estil de tauler (`Ctrl + F4`) com:

```
category-icon-position:top;separator-color:#B4A7D6;
```

Per a un tauler Fitzgerald Key complet, amb pictogrames a cada tecla i les frases organitzades per grups de paraules, crea'l a [SimonSays Web](https://github.com/juan-rey/simonsays-web) i importa'l amb `F9`.

Els estils viatgen amb les teves categories: en exportar (`F10`) s'hi inclouen, i en importar (`F9`) s'hi porten, de manera que un tauler amb estil es pot compartir com un sol fitxer.

## Importar i exportar categories
Els taulers es comparteixen com a fitxers `.ssc` (categories i frases) o `.ssz` (el mateix, més els fitxers de so i d'icona que fan servir, en un sol fitxer). També els pots crear i editar visualment al navegador amb [SimonSays Web](https://github.com/juan-rey/simonsays-web).

- **Exportar** (`F10`): quan se't pregunti, tria exportar totes les categories o només la seleccionada; el diàleg de desar s'obre de manera predeterminada a `%LocalAppData%\SimonSays\boards\`, tot i que pots desar a qualsevol lloc. SimonSays desa automàticament un `.ssz` quan les teves categories o frases fan servir fitxers de so o d'icona que pot trobar (els sons `.mid`/`.midi` no s'hi inclouen). Exportar totes les categories inclou l'estil del tauler; exportar una categoria només inclou l'estil d'aquella categoria. Els missatges d'èxit o d'error apareixen en l'idioma seleccionat.
- **Importar** (`F9`): tria un fitxer `.ssc` o `.ssz`.
  - Un fitxer amb **diverses categories o amb un estil de tauler** és un tauler complet: **substitueix totes les categories i frases de l'idioma actual**. Abans, el teu tauler anterior es desa com a `backup.ssz` (consulta [Desfer canvis](#desfer-canvis)).
  - Un fitxer amb **una sola categoria** s'afegeix al teu tauler; si ja existeix una categoria amb el mateix nom, se't preguntarà si la vols sobreescriure. El mateix passa quan el fitxer només dona nom al seu tauler (`title`, `credits` o `language`, sense colors, mides ni fonts): aquestes dades mai no substitueixen el teu tauler ni el seu aspecte.
  - Els fitxers de so i d'icona d'un `.ssz` s'instal·len a la carpeta de recursos, i la finestra de categories ajusta la mida al tauler importat. Els missatges d'èxit o d'error apareixen en l'idioma seleccionat.

## Desfer canvis
SimonSays desa cada canvi a l'instant i no té una ordre general de Desfer. Aquestes són les maneres de tornar enrere:

- **Desfer una importació**: abans que una importació substitueixi el teu tauler, SimonSays desa el tauler anterior com a `backup.ssz` a `%LocalAppData%\SimonSays\boards\`. Per desfer la importació, prem `F9` i tria `backup.ssz`: el teu tauler anterior torna tal com era, categories i aspecte, sense més preguntes. Només es conserva l'últim tauler substituït: cada nova importació d'un tauler complet sobreescriu la còpia.
- **Desfer un canvi de l'estil del tauler**: després d'editar l'estil del tauler amb `Ctrl + F4`, SimonSays pregunta "Vols revertir els canvis?"; tria `D'acord` per tornar a l'estil anterior.
- **Desfer un canvi de categoria o de frase**: els canvis fets amb `F4`, `F7` o `F8` no es poden desfer automàticament. Abans de fer canvis grans en un tauler que t'importa, exporta'l (`F10`); importar aquell fitxer més endavant (`F9`) et torna a aquell punt.

### Restaurar les frases predeterminades
SimonSays mai no sobreescriu les teves categories i frases, de manera que les noves frases predeterminades d'una actualització només apareixen en els idiomes que encara no has fet servir. Pots tornar a les categories i frases integrades en qualsevol moment; per exemple, per començar de nou o per rebre les frases noves d'una actualització. Això **substitueix tot el tauler de l'idioma actual**:

1. Fes clic al botó 📂 al costat de `Categories` (o prem `F9` a la finestra de categories). S'obre la carpeta de taulers.
2. Tria el tauler de frases predeterminades del teu idioma; en català, `SimonSays_Frases_predeterminades_(Català).ssc` (en els altres idiomes té la mateixa forma, en aquell idioma; per exemple, `SimonSays_default_phrases_(English).ssc`).
3. Confirma la importació. Abans, el teu tauler actual es desa com a `backup.ssz`, de manera que encara el pots recuperar (consulta [Desfer canvis](#desfer-canvis)).

Aquest tauler conté sempre les frases predeterminades de la versió de SimonSays que fas servir: SimonSays l'actualitza a la carpeta de taulers en iniciar-se i en canviar d'idioma. Si vols conservar el teu tauler, exporta'l abans (`F10`) amb el nom que prefereixis; `backup.ssz` se sobreescriu a la importació següent.

Si el fitxer no és a la carpeta de taulers, queda un últim recurs: tanca SimonSays (menú de la safata → `Sortir`), obre l'Editor del Registre (`regedit`), elimina la clau amb el nom del teu idioma en anglès (per exemple `Catalan`) a `HKEY_CURRENT_USER\SOFTWARE\SimonSays\Phrases\` i torna a iniciar SimonSays. Les categories i frases predeterminades d'aquest idioma es tornen a crear, sense còpia de seguretat.

Per a `Portuguese (Brazil)`: si també existeix una clau `Portuguese`, SimonSays copia aquell tauler a `Portuguese (Brazil)` en lloc de crear les frases predeterminades brasileres, de manera que elimina també la clau `Portuguese` (exporta-la abans si la fas servir).

## On desa SimonSays els teus fitxers
- **Les categories, les frases i la configuració** es desen al registre de Windows, a `HKEY_CURRENT_USER\SOFTWARE\SimonSays` (el tauler de cada idioma, a `Phrases\<Language>`).
- `%LocalAppData%\SimonSays\resources\`: els teus fitxers de so i d'icona, i els instal·lats des de taulers `.ssz`; un tauler l'estil del qual defineix `title` o `resource-folder` hi té la seva pròpia subcarpeta.
- `%LocalAppData%\SimonSays\boards\`: la carpeta predeterminada per als taulers exportats, la còpia automàtica `backup.ssz` i els taulers que vénen amb SimonSays: les frases predeterminades de cada idioma, i categories a punt per fer servir que pots afegir al teu tauler amb `F9` o amb el botó 📂: Sí / No, Dolor i cos, Lletrejar, Jocs, Preguntes, Cura diària, una Plantilla de reunió i una Prova d'audició, amb el nom en el teu idioma, i `Spelling_NATO`, `Alert_tones` i `Morse_code` per a qualsevol idioma. Cadascuna afegeix una categoria sense substituir el teu tauler. Els taulers del teu idioma s'hi copien en iniciar i quan canvies l'idioma a la configuració, sempre que el tauler predeterminat del teu idioma falti o hagi canviat, i substitueixen les còpies de la carpeta; per això, desa amb un altre nom els taulers que editis.
- `%LocalAppData%\SimonSays\debug\`: fitxers de diagnòstic (com `hid_dump.txt`) que el suport et pot demanar.

Per obrir una d'aquestes carpetes, enganxa'n el camí (per exemple `%LocalAppData%\SimonSays`) a la barra d'adreces de l'Explorador de fitxers.

## Resolució de problemes
- **Falla la inicialització de la veu**: comprova que SAPI està instal·lat i que hi ha una veu compatible.
- **No hi ha veu per al meu idioma, o les frases es diuen amb un altre accent**: SimonSays fa servir les veus instal·lades a Windows. Afegeix una veu per al teu idioma a la configuració de Windows (**Hora i idioma → Veu → Gestiona les veus**) i tria-la després a la configuració de SimonSays (`F2`). És possible que algunes veus "naturals" més recents de Windows no estiguin disponibles per a altres aplicacions.
- **No se senten els sons**: comprova el camí i l'extensió del fitxer, i que els delimitadors `♫` van en parella.
- **La barra de tasques ja no s'amaga automàticament amb SimonSays obert**: és intencionat. SimonSays és a la barra de tasques, de manera que la manté visible mentre funciona i torna a activar "Amaga automàticament la barra de tasques" quan el tanques. Si SimonSays es va tancar de manera inesperada (per exemple, per un tall de llum), es torna a activar la propera vegada que tanquis SimonSays.
- **El botó Widgets tapa la barra de SimonSays** (Windows 11): desactiva Widgets a **Configuració → Personalització → Barra de tasques** de Windows. SimonSays ofereix obrir aquesta pàgina una vegada, la primera vegada que detecta Widgets.
- **La finestra de categories és massa petita o massa gran**: fes doble clic en una zona buida o a la vora de la finestra per ajustar-la al contingut, o fes servir el zoom amb `F11` / `F12`.
- **Han desaparegut les meves categories o frases**: comprova l'idioma a la configuració (`F2`); cada idioma té el seu propi tauler. Si una importació va substituir el teu tauler, consulta [Desfer canvis](#desfer-canvis).
- **"SimonSays no ha pogut desar les teves categories i frases"**: el teu canvi encara es mostra, però es pot perdre en tancar SimonSays. Exporta el teu tauler amb `F10` de seguida per conservar-ne una còpia; si es repeteix, comunica-ho. El missatge només apareix una vegada, no a cada canvi, perquè mai no destorbi l'ús del tauler.
- **Errors d'importació o d'exportació**: revisa els permisos del fitxer i la ubicació al disc; torna-ho a provar.
- **No s'apliquen els canvis d'idioma o de veu**: torna a obrir la configuració i confirma la selecció. Si els canvis continuen sense aplicar-se, reinicia SimonSays i torna-ho a provar.
- **Informar de problemes amb el seguidor ocular**: si el suport et demana un diagnòstic, obre la finestra de Mirada / Clic per fixació (`F3`) i fes clic a `Desa l'informe de diagnòstic...`. L'informe (`hid_dump.txt`) es desa a `%LocalAppData%\SimonSays\debug\` i s'obre aquella carpeta. Només es crea quan el demanes.

## Comentaris, suport i privadesa
- El diàleg `Quant a` (menú de la safata) mostra la versió, la descripció i el copyright.
- Comparteix suggeriments, problemes o com funciona SimonSays amb el teu seguidor ocular mitjançant l'opció `Dona'ns la teua opinió` del menú de la safata, que obre un [formulari de comentaris](https://forms.gle/KMbpEDYmwnFJUhgy8).
- Per informar d'errors i obtenir suport tècnic, visita el [repositori de SimonSays a GitHub](https://github.com/juan-rey/SimonSays).
- SimonSays és lliure i de codi obert, amb la Open Software License 3.0 (OSL-3.0). No mostra publicitat ni recull les teves dades.
