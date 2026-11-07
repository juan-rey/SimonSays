# Guida di SimonSays

## Panoramica
SimonSays è un assistente di sintesi vocale (TTS) per Windows creato da Juan Rey Saura. È nato come strumento per le persone con SLA e per altre persone che non possono più parlare, perché potessero comunicare rapidamente senza lasciare ciò che stavano facendo al computer. Col tempo è diventato chiaro che lo stesso progetto funziona come strumento generale di CAA (comunicazione aumentativa e alternativa) per chiunque trovi difficile o impossibile parlare; per esempio a causa di disartria, laringectomia, aprassia del linguaggio, ictus o lesione cerebrale. È ottimizzato per parlare velocemente, usare librerie di frasi e riprodurre suoni all'interno del testo. Supporta più lingue, voci configurabili e funzioni pensate per una comunicazione efficiente.

SimonSays usa lo spazio libero della barra delle applicazioni di Windows e offre un accesso rapido a una decina di categorie di frasi di uso frequente (e personalizzabili) con un paio di clic. Poiché non copre mai le altre finestre, puoi restare nel browser, nei tuoi documenti o in una videochiamata e parlare ogni volta che ne hai bisogno: con la tastiera, con il mouse o senza mani, con un tracker oculare.

Casi d'uso:
- Uso intensivo di più applicazioni Windows senza perdere di vista la comunicazione.
- Tavole create da logopedisti per persone con disabilità del linguaggio.
- Riunioni online con [Virtual CABLE](https://vb-audio.com/Cable/) (vedi [Usare SimonSays nelle riunioni online](#usare-simonsays-nelle-riunioni-online)).

SimonSays non vuole sostituire programmi di comunicazione completi come Mind Express, TD Communicator, Grid 3 o OptiKey, né la valutazione e il supporto professionale che di solito li accompagnano. Svolge un compito più mirato: parlare velocemente dalla barra delle applicazioni mentre continui a lavorare in altre applicazioni. Può funzionare accanto a quei programmi; per esempio, OptiKey o un altro software di controllo oculare può muovere il cursore mentre SimonSays pronuncia le tue frasi.

SimonSays è open source e gratuito (libero come in libertà, non solo come in prezzo), e resterà tale: chiunque può usarlo, studiarlo, adattarlo e condividerlo. Non ha pubblicità e non raccoglie dati.

SimonSays si avvicina alla versione 1.0. Prima di questo traguardo ha bisogno di persone che lo provino nella vita di tutti i giorni, con le proprie voci, tavole e tracker oculari: se qualcosa non funziona, è scomodo o manca, faccelo sapere (vedi [Commenti, supporto e privacy](#commenti-supporto-e-privacy)). Ogni segnalazione aiuta.

## Indice
- [Primi passi](#primi-passi)
- [Finestra delle categorie](#finestra-delle-categorie)
- [Frasi predefinite](#frasi-predefinite)
- [Scorciatoie](#scorciatoie)
- [Impostazioni (F2)](#impostazioni-f2)
- [Sguardo / Clic a fissazione (F3)](#sguardo--clic-a-fissazione-f3)
- [Usare SimonSays nelle riunioni online](#usare-simonsays-nelle-riunioni-online)
- [Menu dell'area di notifica](#menu-dellarea-di-notifica)
- Modificare le tavole (avanzato):
  - [Modificare categorie e frasi](#modificare-categorie-e-frasi) (e [quale editor usare](#quale-editor-usare))
  - [Mescolare voce e suoni](#mescolare-voce-e-suoni)
  - [Personalizzare l'aspetto (stili della tavola e delle categorie)](#personalizzare-laspetto-stili-della-tavola-e-delle-categorie)
  - [Importare ed esportare categorie](#importare-ed-esportare-categorie)
  - [Annullare le modifiche](#annullare-le-modifiche)
  - [Dove SimonSays salva i tuoi file](#dove-simonsays-salva-i-tuoi-file)
- [Risoluzione dei problemi](#risoluzione-dei-problemi)
- [Commenti, supporto e privacy](#commenti-supporto-e-privacy)

## Primi passi
1. Avvia SimonSays. Compare in uno spazio libero della barra delle applicazioni di Windows e aggiunge un'icona all'area di notifica. La barra delle applicazioni può trovarsi in basso, in alto, a sinistra o a destra dello schermo:
   - Quando le icone della barra delle applicazioni sono centrate, SimonSays si colloca all'inizio della barra (all'estremità sinistra, o in alto su una barra laterale). Quando sono allineate a sinistra, si colloca subito prima dell'area di notifica.
   - Su una barra delle applicazioni piccola (sottile), la barra di SimonSays è più bassa, con un carattere più piccolo e i due pulsanti di accesso rapido affiancati. Su una barra delle applicazioni a sinistra o a destra dello schermo, i controlli sono impilati uno sotto l'altro.
   - La finestra delle categorie si apre accanto alla barra delle applicazioni, e la freccia del pulsante `Categorie` indica da che parte si apre.
   - Se sposti o ridimensioni la barra delle applicazioni mentre SimonSays è in esecuzione, la segue entro pochi secondi.
2. Per parlare, puoi:
   - Fare clic sulla casella di testo della barra (o fissarla con lo sguardo): si apre una casella di inserimento più grande accanto alla barra delle applicazioni. Scrivi il testo e premi `Invio` per pronunciarlo; la casella si chiude e lo stato attivo passa a `Riproduci>`, così `Spazio` lo ripete (o lo ferma). `Esc` chiude la casella senza parlare e ti riporta alla finestra che stavi usando; il testo resta nella barra, dove `Riproduci>` lo ripete.
   - Fare clic su una o più frasi salvate nella finestra delle categorie.
3. Durante la riproduzione, il pulsante mostra `>` per indicare che è in corso. Puoi premerlo di nuovo per fermarla subito.

![La barra di SimonSays sulla barra delle applicazioni](img/bar.png)

Accanto al pulsante `Categorie`, due piccoli **pulsanti di accesso rapido** danno accesso con un clic a due cose che altrimenti raggiungeresti con una scorciatoia o con il menu dell'area di notifica:

- 📂 apre una tavola salvata sul tuo computer; è la stessa importazione di `F9`, ma parte dalla tua cartella delle tavole (`%LocalAppData%\SimonSays\boards\`), dove si trovano le tavole esportate e scaricate (vedi [Importare ed esportare categorie](#importare-ed-esportare-categorie)).
- ⚙️ apre le impostazioni, come `F2`.

Entrambi possono essere nascosti con `Mostra i pulsanti e il menu di accesso rapido` nelle impostazioni (`F2`), lasciando più spazio alla casella di inserimento. La stessa opzione disattiva anche il menu contestuale (clic destro) della finestra delle categorie.

Se SimonSays è nascosto, fai clic sulla sua icona nell'area di notifica per mostrarlo di nuovo.

## Finestra delle categorie
![La finestra delle categorie con le frasi predefinite](img/categories.png)

- Aprila con il pulsante `Categorie`. Ridimensionala, oppure attiva `Ricorda la dimensione della finestra delle categorie` nelle impostazioni per conservarne la dimensione.
- Seleziona una categoria nell'elenco in alto e le frasi nell'elenco in basso.
- Un clic su una frase la pronuncia automaticamente se `Pronuncia subito la frase quando la selezioni` è attivo; altrimenti la frase viene caricata nella casella di inserimento principale senza essere pronunciata.
- Fai doppio clic su un'area vuota o sul bordo della finestra per ridimensionarla in modo che tutte le sue categorie e frasi ci stiano.
- Trascina un'area vuota della finestra per spostarla, e premi `Esc` per nasconderla.
- Ingrandisci e riduci lo zoom con `F12` e `F11` (vedi [Scorciatoie](#scorciatoie)).
- Fai clic destro su un pulsante o su un'area vuota (o premi `Maiusc + F10` o il tasto Menu) per aprire un menu con `Incolla`, `Riduci`, `Ingrandisci`, `Ripristina zoom`, `Aiuto`, `Impostazioni`, `Sguardo / Clic a fissazione...`, `Nascondi` e `Esci da SimonSays...`, ciascuno con la sua scorciatoia. La prima voce mostra il titolo della tavola (`Tavola senza titolo` se non ne ha uno); sceglila per vedere i riconoscimenti della tavola, la sua lingua e quante categorie e frasi contiene. Il menu compare solo quando `Mostra i pulsanti e il menu di accesso rapido` è attivo nelle impostazioni.
- Su un pulsante di categoria o di frase, il menu offre anche `Modifica...`, `Sposta a sinistra`, `Sposta a destra`, `Aggiungi nuova categoria...` o `Aggiungi nuova frase...` e `Elimina categoria...` o `Elimina frase...` per quel pulsante, senza doverlo prima selezionare; su una categoria, `Esporta categoria...` chiede se salvare solo quella categoria o tutta la tavola. Le scorciatoie da `F4` a `F8` sono mostrate quando il pulsante è la tua selezione corrente, perché quei tasti agiscono sulla selezione. Su un'area vuota il menu offre `Aggiungi nuova categoria...`, che ne aggiunge una alla fine; è anche il modo per iniziare una tavola vuota.
- La striscia tra le categorie e le frasi mostra un promemoria delle scorciatoie principali (o la didascalia della tavola, se ne ha una).

![Il menu contestuale di un pulsante di frase](img/menu.png)

## Frasi predefinite
SimonSays include un insieme di categorie e frasi pronte all'uso in ciascuna delle sue 18 lingue, ordinato in modo che le frasi più necessarie richiedano meno selezioni:

- 👋🏻 Saluti Frequenti
- 🗣️ Frasi di Conversazione
- 🚨 Frasi di Emergenza
- 🤲 Cura & Comfort
- 🏥 Salute
- 🏠 Famiglia & Casa
- 👥 Sociale
- ⏰ Tempo & Data
- 💼 Frasi di Affari
- ✈️ Frasi di Viaggio
- 🔊 Suoni

Ogni lingua ha le sue categorie e frasi: quando cambi lingua nelle impostazioni (`F2`), la finestra delle categorie passa alla tavola di quella lingua, e le modifiche fatte in una lingua non toccano le altre.

Le frasi vengono pronunciate con le voci installate in Windows. Se non è installata alcuna voce per la tua lingua, le frasi compaiono comunque, ma possono essere pronunciate con un'altra voce (vedi [Risoluzione dei problemi](#risoluzione-dei-problemi)).

Puoi modificare le frasi predefinite o recuperarle in seguito; vedi [Modificare categorie e frasi](#modificare-categorie-e-frasi) e [Ripristinare le frasi predefinite](#ripristinare-le-frasi-predefinite).

## Scorciatoie
- `F1`: Apri la guida
- `F2`: Apri le impostazioni
- `F3`: Impostazioni di sguardo / clic a fissazione
- `Invio`: Pronuncia il testo della casella di inserimento
- `Esc`: Nascondi la finestra delle categorie
- `F11` / `Ctrl -`: Riduci lo zoom (finestra delle categorie)
- `F12` / `Ctrl +`: Ingrandisci lo zoom (finestra delle categorie)
- `Ctrl + 0`: Ripristina lo zoom
- `Maiusc + F10` o il tasto Menu: Apri il menu della finestra delle categorie (come il clic destro)
- `Spazio` su `Riproduci>`: Pronuncia di nuovo il testo, o fermalo mentre è in riproduzione

Le scorciatoie per modificare categorie, frasi e tavole sono elencate in [Modificare categorie e frasi](#modificare-categorie-e-frasi).

## Impostazioni (F2)
Apri le impostazioni (`F2`) per regolare quanto segue. Le impostazioni hanno due colonne.

![La finestra delle impostazioni](img/settings.png)

**Colonna sinistra**
- **Testo predefinito**: attiva o disattiva `Inserisci automaticamente il testo predefinito` e personalizza la frase predefinita.
  - `Mostra la tastiera touch`: apre la tastiera touch sullo schermo quando si apre la casella di inserimento; viene posizionata accanto alla casella, dal lato opposto alla barra delle applicazioni.
- `Mostra i pulsanti e il menu di accesso rapido`: mostra i pulsanti 📂 (apri una tavola) e ⚙️ (impostazioni) accanto al pulsante `Categorie` della finestra principale (vedi [Primi passi](#primi-passi)) e il menu contestuale della finestra delle categorie (vedi [Finestra delle categorie](#finestra-delle-categorie)). Disattivala per nasconderli tutti e lasciare più spazio alla casella di inserimento.
- **Finestra delle categorie**:
  - `Riduci automaticamente la finestra delle categorie` dopo la selezione.
  - `Ricorda la dimensione della finestra delle categorie`.
  - `Ridimensiona automaticamente la finestra delle categorie`: adatta la finestra delle categorie alle sue categorie e frasi ogni volta che vengono caricate; per esempio all'avvio, dopo un cambio di lingua o dopo un'importazione.
- `Sguardo / Clic a fissazione...`: apre le impostazioni di [Sguardo / Clic a fissazione](#sguardo--clic-a-fissazione-f3) sopra le impostazioni, che restano aperte con le tue modifiche. Le modifiche fatte lì vengono salvate quando premi `OK` in quella finestra, anche se poi premi `Annulla` nelle impostazioni.

**Colonna destra**
- **Lingua**: scegli la lingua dell'interfaccia (English, Español, العربية, Euskara, Català, 简体中文, Français, Galego, Deutsch, עברית, हिन्दी, Italiano, 日本語, 한국어, Português, Português (Brasil), Русский, Valencià). Ogni lingua ha anche le sue categorie e frasi (vedi [Frasi predefinite](#frasi-predefinite)).
- **Voce**: scegli una voce SAPI installata adatta alla tua lingua.
- **Volume/Velocità**: regola `Volume (10-100)` e `Velocità (-10 a 10)`.
- `Prova voce`: ascolta la voce selezionata (attraverso il dispositivo scelto in `Uscita audio`).
- `Uscita audio`: dove SimonSays riproduce la sua voce e i suoi suoni: `Predefinito di Windows` (la scelta abituale) o qualsiasi dispositivo collegato. I cavi audio virtuali sono elencati per primi, ed è ciò che serve per le [riunioni online](#usare-simonsays-nelle-riunioni-online). Se il dispositivo scelto non è collegato, SimonSays suona attraverso il dispositivo predefinito di Windows e ti avvisa una volta; la scelta viene conservata, mostrata come `(non collegato)` e usata di nuovo appena il dispositivo torna disponibile.
- `Impostazioni di ascolto...`: apre i dispositivi di registrazione di Windows, dove puoi attivare **Ascolta questo dispositivo** per un cavo virtuale (vedi [Usare SimonSays nelle riunioni online](#usare-simonsays-nelle-riunioni-online)).
- **Comportamento della riproduzione**:
  - `Pronuncia subito la frase quando la selezioni` (pronuncia automatica con un clic).
  - `Ferma la riproduzione precedente`: ferma automaticamente qualsiasi riproduzione in corso prima di iniziare una nuova frase.
  - `Aumenta il volume di SimonSays durante la riproduzione`: alza il volume del dispositivo su cui suona SimonSays (vedi `Uscita audio`) mentre parla e ripristina il livello precedente alla fine.
  - `Riduci temporaneamente gli altri audio durante la riproduzione`: abbassa il volume delle altre applicazioni mentre parla e lo ripristina alla fine.

**In basso**
- `Aiuto` (angolo in basso a sinistra, o `F1` con le impostazioni aperte) apre questa guida nella lingua selezionata nell'elenco `Lingua`; le impostazioni restano aperte con le tue modifiche.
- `OK` salva le modifiche; `Annulla` le scarta.

Per impostazione predefinita, `Pronuncia subito la frase quando la selezioni`, `Riduci automaticamente la finestra delle categorie`, `Ridimensiona automaticamente la finestra delle categorie`, `Aumenta il volume di SimonSays durante la riproduzione` e `Riduci temporaneamente gli altri audio durante la riproduzione` sono attivi, e `Ferma la riproduzione precedente` è disattivato.

## Sguardo / Clic a fissazione (F3)
Attiva frasi e pulsanti senza usare le mani tenendo lo sguardo (o il cursore del mouse) su di essi per un tempo stabilito. Questa funzione è nuova: ha bisogno di prove e commenti; raccontaci come funziona con il tuo tracker tramite l'opzione `Invia feedback` del menu dell'area di notifica.

Configurazioni supportate:
- **Qualsiasi software di controllo oculare che muove il cursore del mouse** (Irisbond EasyClick, Tobii Dynavox Computer Control, le modalità cursore del Controllo oculare di Windows, OptiKey): il clic a fissazione segue il cursore.
- **Lettura diretta dello sguardo (senza bisogno di muovere il cursore)**: Irisbond Hiru (modalità HID) e i tracker Tobii tramite il loro software; verificato con Tobii Eye Tracker 4C, Tobii Eye Tracker 5 e Tobii Dynavox PCEye 5.
- **Tobii Eye Tracker 5**: il suo software attuale installa solo un motore a 64 bit, quindi SimonSays lo legge tramite un piccolo programma ausiliario, `SimonSaysGaze64.exe`, installato accanto a esso. L'ausiliario si avvia solo quando serve, non ha finestre, si chiude insieme a SimonSays e mantiene lo sguardo sul tuo computer: non viene mai salvato né inviato da nessuna parte.
- **Licenza di Tobii Eye Tracker 4C ed Eye Tracker 5**: la licenza per l'utente finale di Tobii per questi tracker non consente l'uso medico, compreso l'uso in ospedali e altri contesti sanitari. I dispositivi Tobii Dynavox, come il PCEye 5, hanno le proprie condizioni di licenza.
- **Software di Tobii Eye Tracker 4C**: SimonSays legge il 4C tramite Tobii Core. Se colleghi un Eye Tracker 5 allo stesso computer, Windows installa Tobii Experience, che disattiva Tobii Core, e il 4C smette di funzionare. Per usare di nuovo il 4C, reinstalla Tobii Core; la versione consigliata è `Tobii_Eye_Tracking_Core_v2.13.1.7294_x86.exe`, dalla [pagina introduttiva di Tobii](https://gaming.tobii.com/getstarted/).

Dove funziona il clic a fissazione:
- La barra di SimonSays, la finestra delle categorie e i suoi pulsanti di frasi e categorie.
- I pulsanti delle finestre e delle domande di SimonSays: le impostazioni, la finestra Sguardo / Clic a fissazione, le finestre di aggiunta, modifica e anteprima, e tutte le conferme come `Sì` / `No` o `OK` / `Annulla`. Questi pulsanti sono nella tua lingua e iniziano a rispondere alla fissazione circa tre quarti di secondo dopo l'apertura della finestra, così una domanda che compare proprio dove stai guardando non può rispondersi da sola.
- Quando lo sguardo viene letto direttamente, il pulsante che stai guardando si illumina come sotto il mouse, così puoi vedere cosa stai per selezionare prima che inizi l'avanzamento. Un battito di ciglia mantiene l'evidenziazione; distogliere lo sguardo la toglie.

![La finestra Sguardo / Clic a fissazione](img/dwell.png)

Apri le impostazioni di Sguardo / Clic a fissazione (`F3`, il menu dell'area di notifica o il pulsante `Sguardo / Clic a fissazione...` nelle impostazioni) per regolare:
- **Modalità di attivazione**: scegli una delle seguenti opzioni:
  - `Automatico`: rileva i dispositivi presenti e attiva il clic a fissazione con il cursore del mouse o con il rilevamento dello sguardo, a seconda dei casi. Il software Tobii in background (sempre in esecuzione dopo l'installazione) da solo non attiva la fissazione con il cursore; un tracker Tobii viene usato solo mentre sta davvero seguendo lo sguardo.
  - `Cursore del mouse`: attiva sempre il clic a fissazione con il cursore del mouse.
  - `Puntatore oculare HID`: attiva sempre il clic a fissazione con il rilevamento dello sguardo (Irisbond Hiru in modalità HID, o tracker Tobii). Solo il tuo sguardo attiva i pulsanti: finché non arriva alcuno sguardo (tracciamento spento o tracker scollegato), nulla si attiva per fissazione e il mouse funziona normalmente.
  - `Disattivato`: disattiva sempre il clic a fissazione.
  - **Ritardo di aggancio**: per quanto tempo, in millisecondi, il tuo sguardo (o il cursore) deve restare su un pulsante prima che inizi la barra di avanzamento (250 per impostazione predefinita). Così, passando con lo sguardo sui pulsanti non si vede alcun avanzamento; il pulsante si attiva dopo il ritardo di aggancio più il tempo di fissazione. Impostalo a 0 per far partire subito l'avanzamento.
  - **Tempo di fissazione**: la durata (in millisecondi) necessaria per fare clic guardando un elemento (500 per impostazione predefinita). Durante la fissazione viene mostrato un indicatore di avanzamento.
  - **Raggio di tolleranza**: la tolleranza dello sguardo (in pixel) che consente piccoli movimenti dell'occhio senza interrompere la fissazione.
  - **Tempo di attesa**: millisecondi dopo un'attivazione durante i quali il clic a fissazione non può attivarsi (evita clic ripetuti per errore).
  - **Colore di avanzamento**: il colore dell'indicatore di avanzamento della fissazione (per impostazione predefinita, il colore principale di Windows).
  - **Allontanarsi prima che la fissazione riattivi** (attivo per impostazione predefinita): dopo un clic a fissazione, nessun pulsante può essere riattivato per fissazione finché il puntatore o lo sguardo non si allontana da quel punto (oltre il raggio di tolleranza) o finché non distogli lo sguardo per un momento. Restare su un pulsante lo attiva una sola volta, così una frase lunga non viene ripetuta prima di finire. I clic del mouse non sono interessati.
  - **Rileva come usi il tracker**: due pulsanti che aiutano l'applicazione a capire se muovi il cursore del mouse con un tracker oculare o se semplicemente fai clic sui pulsanti.
  - **Rilevato**: mostra il tipo di dispositivo rilevato e altre informazioni sul software collegato.
  - `Salva rapporto di diagnostica...` (sotto Rilevato): scrive un rapporto sul tuo tracker oculare e sul suo software in `hid_dump.txt` e ne apre la cartella, così puoi inviarlo al supporto.
  - Un pulsante `Reimposta` (in basso a sinistra) riporta tutte le impostazioni di fissazione ai valori predefiniti.
  - Un pulsante `OK` salva le modifiche; `Annulla` le scarta.

## Usare SimonSays nelle riunioni online
SimonSays può parlare direttamente in una videochiamata (Teams, Zoom, Google Meet…), così gli altri partecipanti sentono le tue frasi come se uscissero dal tuo microfono. Ti serve un cavo audio virtuale, come il gratuito [VB-Audio Virtual Cable](https://vb-audio.com/Cable/):

1. Installa il cavo audio virtuale e riavvia Windows se richiesto.
2. Invia l'audio di SimonSays al cavo: apri le impostazioni (`F2`) e scegli `CABLE Input` in `Uscita audio` (i cavi virtuali sono elencati per primi). Premi `Prova voce` per verificarlo: non dovresti più sentirlo dagli altoparlanti, perché ora va nel cavo.
3. Nella tua applicazione per riunioni, scegli `CABLE Output` come microfono.
4. Per sentire anche tu SimonSays, premi `Impostazioni di ascolto...` nelle impostazioni. Si aprono i dispositivi di registrazione di Windows: seleziona `CABLE Output`, apri **Proprietà → Ascolto**, spunta **Ascolta questo dispositivo**, scegli i tuoi altoparlanti o le cuffie in **Riproduci tramite questo dispositivo** e premi `OK`. Devi farlo una sola volta.

SimonSays non può attivare **Ascolta questo dispositivo** al posto tuo: è un'impostazione del lato di registrazione del cavo che Windows permette di cambiare solo a te, per questo `Impostazioni di ascolto...` ti porta direttamente alla finestra giusta.

Buono a sapersi:
- Lascia il dispositivo di riproduzione predefinito di Windows sui tuoi altoparlanti o sulle cuffie, non su `CABLE Input`. Se **Ascolta questo dispositivo** riproduce su "Dispositivo di riproduzione predefinito" mentre quel dispositivo predefinito è il cavo, l'audio rientra nel cavo.
- Se nella riunione si sentono le tue frasi ma non gli effetti sonori (applausi, un campanello), la soppressione del rumore dell'applicazione per riunioni li sta eliminando; abbassa o disattiva la soppressione del rumore nell'applicazione.
- Se il cavo viene disinstallato o scollegato, SimonSays suona attraverso il dispositivo predefinito di Windows e ti avvisa una volta; torna al cavo appena è di nuovo disponibile.
- Per avere frasi pronte per una chiamata, aggiungi la categoria **Modello per riunioni**: premi `F9` o il pulsante 📂 e apri la tavola con il nome nella tua lingua (`SimonSays_Modello_per_riunioni_(Italiano).ssc` in italiano). Contiene saluti, "Mi senti?", una breve spiegazione che parli tramite un programma, sì / no, d'accordo / non d'accordo, richieste di ripetere o di parlare più lentamente, e congedi. Modificala per aggiungere frasi per la tua riunione.
- In alternativa al passaggio 2, puoi ancora indirizzare SimonSays con l'impostazione per app di Windows (**Impostazioni → Sistema → Audio → Mixer volume**), lasciando `Uscita audio` su `Predefinito di Windows`. SimonSays compare in quell'elenco solo dopo aver riprodotto qualcosa.

## Menu dell'area di notifica
- `Mostra` / `Nascondi` l'interfaccia principale
- `Impostazioni`
- `Sguardo / Clic a fissazione...`
- `Web` apre la pagina web di SimonSays
- `Informazioni` mostra la versione e i riconoscimenti
- `Invia feedback` apre un [modulo per i commenti](https://forms.gle/KMbpEDYmwnFJUhgy8) nel browser predefinito
- `Esci` chiude SimonSays, dopo aver chiesto conferma

Puoi spostare la finestra a comparsa dell'area di notifica e le altre finestre di dialogo trascinandone la barra del titolo; ridimensiona la finestra delle categorie trascinandone i bordi e attiva `Ricorda la dimensione della finestra delle categorie` nelle impostazioni per conservarne la dimensione.

## Modificare categorie e frasi
Questa sezione e le successive sono per caregiver, logopedisti e utenti esperti che cambiano le categorie, le frasi, i suoni e l'aspetto di una tavola. Ogni modifica viene salvata subito (vedi [Annullare le modifiche](#annullare-le-modifiche)).

### Quale editor usare
- **In SimonSays**: le modifiche di tutti i giorni; rinominare, aggiungere, spostare o eliminare categorie e frasi, dare loro un emoji, aggiungere suoni, incollare un elenco di frasi o regolare un colore. Il resto di questa sezione spiega come.
- **In [SimonSays Web](https://github.com/juan-rey/simonsays-web)**: il lavoro più impegnativo sulle tavole, che lì è molto più facile: tavole costruite con pittogrammi di CAA (può cercare nella libreria di pittogrammi ARASAAC), immagini personalizzate per le icone e tavole con stile, con colori, dimensioni e disposizioni. È consigliato soprattutto per progettare una tavola **Fitzgerald Key**, in cui ogni gruppo di parole ha il suo colore. È un editor visuale che apre e salva tavole `.ssc`/`.ssz` direttamente nel browser, senza caricarle online; quando hai finito, importa la tavola in SimonSays con `F9` o con il pulsante 📂. È anche il modo per usare le tavole di SimonSays su altre piattaforme.

Scorciatoie di modifica (finestra delle categorie):
- `F4`: Modifica la selezione corrente
- `F5` / `F6`: Vai alla selezione precedente / successiva negli elenchi
- `F7`: Aggiungi dopo la selezione corrente (categoria o frase, a seconda del contesto)
- `F8`: Elimina la selezione corrente
- `F9`: Importa categorie
- `F10`: Esporta categorie
- `Ctrl + F4`: Modifica lo stile della tavola (vedi [Personalizzare l'aspetto](#personalizzare-laspetto-stili-della-tavola-e-delle-categorie))
- `Ctrl + F8`: Elimina tutte le categorie e le loro frasi (chiede conferma due volte)
- `Ctrl + V` o `Maiusc + Ins`: Aggiungi il testo copiato come frasi, una per paragrafo (vedi [Gestire le frasi](#gestire-le-frasi))
- Trascinare un file `.txt`, `.ssc` o `.ssz` sulla finestra: aggiunge un `.txt` come nuova categoria, oppure importa una tavola come con `F9` (vedi [Gestire le categorie](#gestire-le-categorie))

> **Suggerimento:** esporta la tua tavola (`F10`) prima di usare `Ctrl + F8` o di importare una tavola completa (`F9`), così potrai recuperarla.

### Gestire le categorie
- Rinominare: seleziona la categoria → (scorciatoia: `F4`) `Cambia nome categoria` → cambia il nome → `OK`.
- Spostare la selezione: `F5`/`F6` per andare all'elemento precedente/successivo scorrendo gli elenchi.
- Aggiungere: seleziona una categoria → (scorciatoia: `F7`) finestra `Aggiungi nuova categoria` → scrivi il nome → `OK`.
- Eliminare: seleziona la categoria → (scorciatoia: `F8`) `Elimina categoria`; conferma l'eliminazione della categoria e delle sue frasi.
- Aggiungere da un file di testo: trascina un file `.txt` sulla finestra delle categorie → viene creata una categoria con il nome del file e una frase per paragrafo (i paragrafi sono separati da una riga vuota) → controlla l'anteprima → `OK`. Puoi trascinare più file insieme; ognuno ha la sua anteprima. Un file il cui nome coincide con una categoria esistente viene rifiutato.
- Aprire una tavola trascinandola: trascina un file `.ssc` o `.ssz` sulla finestra delle categorie → SimonSays chiede se importarlo; con `Sì` viene importato come con `F9` (vedi [Importare ed esportare categorie](#importare-ed-esportare-categorie)). Se il file contiene una tavola completa, sostituisce quella attuale, e prima viene salvata una copia della tavola attuale come `backup.ssz`. Puoi trascinare insieme file di testo e file di tavola; ciascuno viene gestito a turno. Aprire un file `.ssc` o `.ssz` da Esplora file (doppio clic) pone la stessa domanda.

#### Impostare o cambiare l'icona di una categoria
Ogni categoria può mostrare sul suo pulsante un emoji o un file immagine personalizzato (`.ico`, `.png` o `.jpg`). Indica l'icona nella finestra di aggiunta o modifica con il prefisso separatore `##`:

```
<icona>##<nome della categoria>
```

- **Emoji**: scrivi l'emoji (o gli emoji) seguito da `##` e dal nome.
  - Esempio: `🙂##Saluti`
- **File di icona** (`.ico`, `.png`, `.jpg`): scrivi il nome del file o il percorso completo seguito da `##` e dal nome. Un nome di file senza cartella viene cercato come descritto in [Ordine di ricerca dei file audio e delle icone](#ordine-di-ricerca-dei-file-audio-e-delle-icone).
  - Esempio: `C:\icons\smile.png##Saluti`
- **Nessuna icona**: scrivi solo il nome della categoria, senza prefisso `##`.
  - Esempio: `Saluti`

> Nota: per le icone da file sono supportati solo i file `.ico`, `.png` e `.jpg`. Le altre estensioni vengono ignorate e non viene mostrata alcuna icona. Le immagini PNG mantengono la trasparenza; le immagini molto grandi vengono ridotte automaticamente al caricamento.

### Gestire le frasi
- Modificare: seleziona la frase → (scorciatoia: `F4`) `Modifica frase` → aggiorna il testo → `OK`.
- Spostare la selezione: `F5`/`F6` per andare alla frase precedente/successiva.
- Aggiungere: seleziona una frase → (scorciatoia: `F7`) `Aggiungi nuova frase` → scrivi il testo → `OK`.
- Eliminare: seleziona la frase → (scorciatoia: `F8`) `Elimina frase`; conferma.
- Aggiungerne molte insieme: copia un testo con le frasi separate da una riga vuota → seleziona una frase (o una categoria) → `Ctrl + V` → controlla l'anteprima → `OK`. Le frasi vengono aggiunte dopo la frase selezionata, o alla fine della categoria. Gli a capo all'interno di un paragrafo diventano spazi, e i marcatori di icona `##` e di audio `::` funzionano come nella finestra `Aggiungi nuova frase`.
- Aggiungere suoni: trascina uno o più file `.wav` o `.mp3` sulla finestra delle categorie → controlla l'anteprima → `OK`. Ogni file diventa una frase sonora con il nome del file, aggiunta dopo la frase selezionata (o alla fine della categoria selezionata), e il file viene copiato nella cartella delle risorse della tavola così da accompagnarla quando la esporti. Un file esistente non viene mai sovrascritto: se c'è già un suono diverso con lo stesso nome, il nuovo viene salvato come `nome (2).wav`.

#### Impostare o cambiare l'icona di una frase
Le frasi seguono la stessa convenzione del prefisso di icona delle categorie, con il separatore facoltativo `##` prima del testo della frase:

```
<icona>##<testo della frase>
```

- **Emoji**: `👍##Mi va bene!`
- **File di icona**: `C:\icons\check.png##Mi va bene!`
- **Nessuna icona**: `Mi va bene!`

Le frasi possono includere un file audio (vedi [Mescolare voce e suoni](#mescolare-voce-e-suoni)). Per combinare icona, testo e audio usa il formato:

```
<icona>##<testo della frase>::<file audio>
```

- Esempio: `🔔##Attenzione::alert.mp3`

Il prefisso `##` e il suffisso `::` sono facoltativi e indipendenti; puoi usare qualsiasi combinazione:
- Solo prefisso di icona: `🔔##Attenzione`
- Solo suffisso audio: `Attenzione::alert.mp3`
- Entrambi: `🔔##Attenzione::alert.mp3`
- Nessuno: `Attenzione`

## Mescolare voce e suoni
- Usa il delimitatore `♫` per inserire nomi di file audio nel testo:
  - Esempio: `Ciao ♫doorbell.mp3♫, aspetta un momento.`
  - Il testo fuori dai delimitatori viene pronunciato; il testo all'interno viene trattato come un file audio e riprodotto in quel punto.
- Audio supportato: `.wav` e `.mp3`. I file MIDI (`.mid`, `.midi`) sono accettati in una frase ma non vengono riprodotti; convertili in `.wav` o `.mp3`.
- Usa percorsi assoluti o relativi accessibili al programma. Assicurati che i delimitatori siano in coppia, così il testo finale non viene trattato come audio.
- Per scrivere `♫`, copialo da questa guida, oppure seleziona una frase che contiene un suono: il suo marcatore `♫…♫` viene inserito nella casella di inserimento, dove puoi aggiungere testo attorno.

### Suoni inclusi in SimonSays
Questi suoni vengono installati con SimonSays e si possono usare per nome in qualsiasi testo o frase; per esempio `♫laugh.mp3♫` nella casella di inserimento, oppure `Ben fatto!::applause.mp3` per un pulsante di frase con l'etichetta "Ben fatto!" che riproduce un applauso:

`ahem.mp3`, `alert.mp3`, `applause.mp3`, `aww.mp3`, `beep.mp3`, `boo.mp3`, `call_bell.mp3`, `censor.mp3`, `doorbell.mp3`, `drumroll.mp3`, `evil_laugh.mp3`, `fail.mp3`, `hmmmm.mp3`, `kisses.mp3`, `laugh.mp3`, `mmm-hmm.mp3`, `nuh-uh.mp3`, `phone_ring.mp3`, `rimshot.mp3`, `scream.mp3`, `shots.mp3`, `sigh.mp3`

### Ordine di ricerca dei file audio e delle icone
Quando il nome di un file audio o di un'icona non contiene una lettera di unità o un percorso completo, SimonSays lo cerca in quest'ordine e usa la prima corrispondenza trovata:

1. **La sottocartella delle risorse della tavola corrente**: se lo stile della tavola imposta `title` o `resource-folder`, la sua sottocartella all'interno della cartella delle risorse (vedi [Personalizzare l'aspetto](#personalizzare-laspetto-stili-della-tavola-e-delle-categorie)).
2. **Cartella delle risorse**: `%LocalAppData%\SimonSays\resources\` (la posizione predefinita per le icone e i suoni delle tavole; i pacchetti `.ssz` importati installano qui i loro file).
3. **Cartella dei dati di SimonSays**: `%LocalAppData%\SimonSays\` (mantenuta come riserva per i file che le versioni precedenti mettevano qui, prima che esistesse la cartella delle risorse).
4. **Directory di lavoro**: la directory di lavoro corrente del processo (cercata solo se diversa dalla cartella dell'eseguibile).
5. **Cartella dell'eseguibile**: la cartella che contiene `SimonSays.exe`.

Se il file non viene trovato in nessuna di queste posizioni, viene usato il suono di riserva integrato (un'icona semplicemente non viene mostrata).

> **Suggerimento:** ti consigliamo di mettere i tuoi file audio e le tue icone in `%LocalAppData%\SimonSays\resources\`; sopravvivono agli aggiornamenti dell'applicazione e sono specifici di ogni utente. Una tavola con uno stile `title` o `resource-folder` ottiene lì automaticamente la sua sottocartella, che tiene i suoi file separati da quelli delle altre tavole.

## Personalizzare l'aspetto (stili della tavola e delle categorie)
Puoi cambiare colori, dimensioni e stile della finestra delle categorie e dei suoi pulsanti; per esempio per creare tavole di comunicazione con codici colore come Fitzgerald Key. Gli stili sono facoltativi: senza di essi la finestra mantiene il suo aspetto predefinito.

> **Suggerimento:** tutto ciò che segue si può scrivere in SimonSays, comodo per piccoli ritocchi. Per progettare un'intera tavola con stile o con pittogrammi è consigliato [SimonSays Web](https://github.com/juan-rey/simonsays-web): vedi il risultato mentre lavori e scrive questi stili per te (vedi [Quale editor usare](#quale-editor-usare)).

Uno stile è un elenco di coppie `proprietà:valore;`. I valori possono essere:
- **Colori**: esadecimale `#RRGGBB`, per esempio `#FFD966`.
- **Dimensioni**: un numero di pixel (`96`) o una percentuale del valore predefinito (`120%`).
- **Parole chiave**: per esempio `left`, `right`, `top`, `bottom`, `smart`, `normal`, `bold`.
- **Testo**: il nome di un carattere, oppure il testo della didascalia, del titolo o dei riconoscimenti.

Tutto ciò che l'applicazione non riconosce (una proprietà scritta male o un valore errato) viene semplicemente ignorato, quindi uno stile non rovina mai la finestra.

### Dare uno stile a una categoria (finestra Modifica)
Seleziona una categoria e premi `F4` (Modifica), poi aggiungi `::` e uno stile dopo il nome:

```
<icona>##<nome della categoria>::<proprietà>:<valore>;<proprietà>:<valore>;
```

- Esempio: `🧍##Persone::background:#FFD966;` colora di giallo il pulsante Persone. Il colore della categoria **si applica anche alle frasi che contiene**, così una sola riga colora un intero gruppo di parole: la base di una tavola Fitzgerald Key.
- Lo stile di una categoria può impostare `background` e `text-color` per il pulsante della categoria, e qualsiasi proprietà delle frasi (vedi sotto) con il prefisso `phrase-`, per esempio `phrase-corner-radius:8;`.

### Dare uno stile a tutta la tavola (`$$board`)
L'aspetto generale (lo sfondo della finestra, i separatori e i valori predefiniti di tutti i pulsanti di categoria e di frase) è uno **stile della tavola**. Viene salvato con il nome di categoria riservato `$$board` (non puoi usare nomi con `$$` per le tue categorie).

- **Modificalo** premendo `Ctrl + F4` nella finestra delle categorie. L'editor mostra un `proprietà:valore;` per riga. Quando confermi con `OK`, il nuovo stile viene applicato e SimonSays chiede "Vuoi annullare le modifiche?": scegli `OK` per tornare allo stile precedente, o `Annulla` per mantenere quello nuovo.
- **Ottienine uno** importando un file di categorie con stile (`.ssc` / `.ssz`). Quando importi un file che contiene uno stile della tavola, SimonSays chiede prima di sostituire il tuo e mostra il `title` e i `credits` della tavola (se presenti) nel messaggio di importazione. Un file il cui stile della tavola imposta solo `title`, `credits`, `language` o `resource-folder` non cambia nulla di visibile, quindi SimonSays non chiede nulla: se contiene una sola categoria, questa viene aggiunta e il tuo aspetto resta; se contiene una tavola completa, sostituisce la tua tavola, aspetto compreso, come qualsiasi tavola completa.

### Proprietà di stile
- **Tutta la finestra**: `background`, `separator-color`, `text-color`, `font-name`, `font-size`, `text-weight`, `caption` (didascalia mostrata tra i separatori), `title` e `credits` (mostrati quando la tavola viene importata), `language` (la lingua per cui è stata creata la tavola, per esempio `language:Italian`; SimonSays si rifiuta di importare la tavola finché la lingua impostata è un'altra, e ti dice a quale lingua passare nelle impostazioni, `F2`).
- **Tutti i pulsanti di categoria** (prefisso `category-`) e **tutti i pulsanti di frase** (prefisso `phrase-`): `background`, `text-color`, `width`, `height`, `corner-radius`, `border-width`, `margin`, `icon-position` (`left`/`right`/`top`/`bottom`/`center`), `icon-size`, `font-name`, `font-size`, `text-weight`, `text-layout`.

Con `icon-position` impostato su `center`, l'icona si trova al centro del pulsante e il testo viene nascosto (un tasto solo icona); usa `top` se vuoi l'icona sopra e il nome sotto. Se vuoi del testo sopra un'icona centrata, aggiungi un `text-layout` esplicito (per esempio `text-layout:bottom`) e verrà mostrato in quella posizione.

`text-layout` accetta una parola chiave orizzontale (`left`/`center`/`right`) e/o una verticale (`top`/`middle`/`bottom`), oppure `smart`. Il valore predefinito è smart: il testo viene centrato nello spazio lasciato libero dall'icona (icona sopra → didascalia centrata sotto; centrato in tutto il pulsante quando non c'è icona).

### Esempio: una tavola Fitzgerald Key a colori
Per colorare sei gruppi di parole, modifica ogni categoria (`F4`) e assegnale un colore di sfondo. Queste sono le stringhe esatte da scrivere nella finestra di modifica:

```
🧍##Persone::background:#FFD966;
🏃##Azioni::background:#93C47D;
📏##Descrittivi::background:#6FABDC;
🍎##Nomi::background:#F6B26B;
🕒##Varie::background:#FFFFFF;
👋##Sociale::background:#B4A7D6;
```

Ogni colore colora anche le frasi di quella categoria, così tutta la tavola risulta codificata per colori. I ritocchi dell'intera tavola (come mettere l'emoji di ogni categoria in alto con il nome sotto, e colorare i separatori) vengono da uno stile della tavola (`Ctrl + F4`) come:

```
category-icon-position:top;separator-color:#B4A7D6;
```

Per una tavola Fitzgerald Key completa, con pittogrammi su ogni tasto e le frasi disposte per gruppi di parole, creala in [SimonSays Web](https://github.com/juan-rey/simonsays-web) e importala con `F9`.

Gli stili viaggiano con le tue categorie: l'esportazione (`F10`) li include e l'importazione (`F9`) li porta con sé, quindi una tavola con stile si può condividere come un unico file.

## Importare ed esportare categorie
Le tavole si condividono come file `.ssc` (categorie e frasi) o `.ssz` (lo stesso, più i file audio e di icona che usano, in un unico file). Puoi anche crearle e modificarle visivamente nel browser con [SimonSays Web](https://github.com/juan-rey/simonsays-web).

- **Esporta** (`F10`): quando richiesto, scegli se esportare tutte le categorie o solo quella selezionata; la finestra di salvataggio si apre per impostazione predefinita in `%LocalAppData%\SimonSays\boards\`, ma puoi salvare ovunque. SimonSays salva automaticamente un `.ssz` quando le tue categorie o frasi usano file audio o di icona che riesce a trovare (i suoni `.mid`/`.midi` non sono inclusi). Esportare tutte le categorie include lo stile della tavola; esportarne una include solo lo stile di quella categoria. I messaggi di esito compaiono nella lingua selezionata.
- **Importa** (`F9`): scegli un file `.ssc` o `.ssz`.
  - Un file con **più categorie o con uno stile della tavola** è una tavola completa: **sostituisce tutte le categorie e frasi della lingua corrente**. La tua tavola precedente viene prima salvata come `backup.ssz` (vedi [Annullare le modifiche](#annullare-le-modifiche)).
  - Un file con **una sola categoria** viene aggiunto alla tua tavola; se esiste già una categoria con lo stesso nome, ti verrà chiesto se sovrascriverla. Lo stesso vale quando il file dà solo un nome alla sua tavola (`title`, `credits` o `language`, senza colori, dimensioni o caratteri): queste informazioni non sostituiscono mai la tua tavola né il suo aspetto.
  - I file audio e di icona contenuti in un `.ssz` vengono installati nella cartella delle risorse, e la finestra delle categorie si adatta alla tavola importata. I messaggi di esito compaiono nella lingua selezionata.

## Annullare le modifiche
SimonSays salva subito ogni modifica e non ha un comando Annulla generale. Questi sono i modi per tornare indietro:

- **Annullare un'importazione**: prima che un'importazione sostituisca la tua tavola, SimonSays salva la tavola precedente come `backup.ssz` in `%LocalAppData%\SimonSays\boards\`. Per annullare l'importazione, premi `F9` e scegli `backup.ssz`: la tavola precedente torna com'era, categorie e aspetto, senza altre domande. Viene conservata solo l'ultima tavola sostituita: ogni nuova importazione di una tavola completa sovrascrive la copia.
- **Annullare una modifica dello stile della tavola**: dopo aver modificato lo stile della tavola con `Ctrl + F4`, SimonSays chiede "Vuoi annullare le modifiche?"; scegli `OK` per tornare allo stile precedente.
- **Annullare una modifica di categoria o di frase**: le modifiche fatte con `F4`, `F7` o `F8` non si possono annullare automaticamente. Prima di grandi modifiche a una tavola a cui tieni, esportala (`F10`); importare quel file in seguito (`F9`) ti riporta a quel punto.

### Ripristinare le frasi predefinite
SimonSays non sovrascrive mai le tue categorie e frasi, quindi le nuove frasi predefinite di un aggiornamento compaiono solo nelle lingue che non hai ancora usato. Puoi tornare alle categorie e frasi integrate in qualsiasi momento; per esempio per ricominciare da capo o per avere le nuove frasi di un aggiornamento. Questo **sostituisce l'intera tavola della lingua corrente**:

1. Fai clic sul pulsante 📂 accanto a `Categorie` (o premi `F9` nella finestra delle categorie). Si apre la cartella delle tavole.
2. Scegli la tavola delle frasi predefinite della tua lingua; in italiano, `SimonSays_frasi_predefinite_(Italiano).ssc` (nelle altre lingue ha la stessa forma, in quella lingua; per esempio `SimonSays_default_phrases_(English).ssc`).
3. Conferma l'importazione. La tua tavola attuale viene prima salvata come `backup.ssz`, quindi puoi ancora recuperarla (vedi [Annullare le modifiche](#annullare-le-modifiche)).

Questa tavola contiene sempre le frasi predefinite della versione di SimonSays che stai usando: SimonSays la aggiorna nella cartella delle tavole all'avvio e quando cambi lingua. Se vuoi conservare la tua tavola, esportala prima (`F10`) con un nome a tua scelta; `backup.ssz` viene sovrascritto alla successiva importazione.

Se il file manca dalla cartella delle tavole, resta un'ultima possibilità: chiudi SimonSays (menu dell'area di notifica → `Esci`), apri l'Editor del Registro di sistema (`regedit`), elimina la chiave con il nome inglese della tua lingua (per esempio `Italian`) in `HKEY_CURRENT_USER\SOFTWARE\SimonSays\Phrases\` e avvia di nuovo SimonSays. Le categorie e frasi predefinite di quella lingua vengono ricreate, senza copia di sicurezza.

Per `Portuguese (Brazil)`: se esiste anche una chiave `Portuguese`, SimonSays copia quella tavola in `Portuguese (Brazil)` invece di creare le frasi predefinite brasiliane, quindi elimina anche la chiave `Portuguese` (esportala prima se la usi).

## Dove SimonSays salva i tuoi file
- **Categorie, frasi e impostazioni** sono salvate nel registro di Windows in `HKEY_CURRENT_USER\SOFTWARE\SimonSays` (la tavola di ogni lingua in `Phrases\<Language>`).
- `%LocalAppData%\SimonSays\resources\`: i tuoi file audio e di icona, e quelli installati dalle tavole `.ssz`; una tavola il cui stile imposta `title` o `resource-folder` ha la sua sottocartella.
- `%LocalAppData%\SimonSays\boards\`: la cartella predefinita per le tavole esportate, la copia automatica `backup.ssz` e le tavole fornite con SimonSays: le frasi predefinite di ogni lingua, più categorie pronte che puoi aggiungere alla tua tavola con `F9` o con il pulsante 📂: Sì / No, Dolore e corpo, Compitare, Giochi, Domande, Cura quotidiana, un Modello per riunioni e una Prova dell'udito, con il nome nella tua lingua, e `Spelling_NATO`, `Alert_tones` e `Morse_code` per qualsiasi lingua. Ognuna aggiunge una categoria senza sostituire la tua tavola. Le tavole della tua lingua vengono copiate lì all'avvio e quando cambi lingua nelle impostazioni, ogni volta che la tavola predefinita della tua lingua manca o è cambiata, sostituendo le copie nella cartella; quindi salva con un nuovo nome le tavole che modifichi.
- `%LocalAppData%\SimonSays\debug\`: file di diagnostica (come `hid_dump.txt`) che il supporto potrebbe chiederti.

Per aprire una di queste cartelle, incolla il suo percorso (per esempio `%LocalAppData%\SimonSays`) nella barra degli indirizzi di Esplora file.

## Risoluzione dei problemi
- **L'inizializzazione della voce non riesce**: verifica che SAPI sia installato e che sia presente una voce compatibile.
- **Nessuna voce per la mia lingua, o frasi pronunciate con un altro accento**: SimonSays usa le voci installate in Windows. Aggiungi una voce per la tua lingua nelle impostazioni di Windows (**Ora e lingua → Voce → Gestisci voci**), poi sceglila nelle impostazioni di SimonSays (`F2`). Alcune delle voci "naturali" più recenti di Windows potrebbero non essere disponibili per altre applicazioni.
- **Nessun suono viene riprodotto**: controlla il percorso e l'estensione del file, e che i delimitatori `♫` siano in coppia.
- **La barra delle applicazioni non si nasconde più automaticamente con SimonSays aperto**: è voluto. SimonSays sta sulla barra delle applicazioni, quindi la tiene visibile mentre è in esecuzione e riattiva "Nascondi automaticamente la barra delle applicazioni" quando lo chiudi. Se SimonSays si è chiuso in modo imprevisto (per esempio per un'interruzione di corrente), l'opzione viene riattivata la prossima volta che chiudi SimonSays.
- **Il pulsante Widget copre la barra di SimonSays** (Windows 11): disattiva i widget in **Impostazioni → Personalizzazione → Barra delle applicazioni** di Windows. SimonSays si offre di aprire quella pagina una volta, la prima volta che rileva i widget.
- **La finestra delle categorie è troppo piccola o troppo grande**: fai doppio clic su un'area vuota o sul bordo della finestra per adattarla al contenuto, oppure usa lo zoom con `F11` / `F12`.
- **Le mie categorie o frasi sono sparite**: controlla la lingua nelle impostazioni (`F2`); ogni lingua ha la sua tavola. Se un'importazione ha sostituito la tua tavola, vedi [Annullare le modifiche](#annullare-le-modifiche).
- **"SimonSays non è riuscito a salvare le tue categorie e frasi"**: la tua modifica è ancora visibile, ma potrebbe andare persa alla chiusura di SimonSays. Esporta subito la tua tavola con `F10` per conservarne una copia; se succede di nuovo, segnalalo. Il messaggio compare una sola volta, non a ogni modifica, così non ostacola mai l'uso della tavola.
- **Errori di importazione o esportazione**: controlla i permessi del file e la posizione sul disco; riprova.
- **Le modifiche di lingua o di voce non vengono applicate**: riapri le impostazioni e conferma la selezione. Se le modifiche continuano a non essere applicate, riavvia SimonSays e riprova.
- **Segnalare problemi con il tracker oculare**: se il supporto ti chiede una diagnostica, apri la finestra Sguardo / Clic a fissazione (`F3`) e fai clic su `Salva rapporto di diagnostica...`. Il rapporto (`hid_dump.txt`) viene salvato in `%LocalAppData%\SimonSays\debug\` e quella cartella si apre. Viene creato solo quando lo chiedi.

## Commenti, supporto e privacy
- La finestra `Informazioni` (menu dell'area di notifica) mostra la versione, la descrizione e il copyright.
- Condividi suggerimenti, problemi o come funziona SimonSays con il tuo tracker oculare tramite l'opzione `Invia feedback` del menu dell'area di notifica, che apre un [modulo per i commenti](https://forms.gle/KMbpEDYmwnFJUhgy8).
- Per segnalare bug e ricevere supporto tecnico, visita il [repository GitHub di SimonSays](https://github.com/juan-rey/SimonSays).
- SimonSays è libero e open source, con licenza Open Software License 3.0 (OSL-3.0). Non mostra pubblicità e non raccoglie i tuoi dati.
