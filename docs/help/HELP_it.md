# Guida di SimonSays

## Panoramica
SimonSays è un assistente Windows di sintesi vocale (TTS), creato da Juan Rey Saura, ottimizzato per comunicazione rapida, librerie di frasi e riproduzione di suoni in linea. Supporta più lingue, voci personalizzabili e funzionalità intuitive per una comunicazione efficiente.

SimonSays utilizza lo spazio libero a sinistra della barra delle applicazioni di Windows, offrendo accesso rapido a una dozzina di categorie di frasi di uso frequente (e personalizzabili) con pochi clic.

## Scorciatoie
- `F1`: Apri Aiuto.
- `F2`: Apri Impostazioni.
- `F3`: Aggiungi dopo la selezione corrente (contesto categoria/frase).
- `F4`: Modifica la selezione corrente.
- `F5` / `F6`: Sposta alla selezione precedente / successiva nelle liste.
- `F8`: Elimina la selezione corrente.
- `F9`: Importa categorie.
- `F10`: Esporta categorie.
- Pulsante `Riproduci>`: Avvia riproduzione.

## Avvio e uso base
1. Avvia SimonSays; appare nell'area di notifica di Windows.
2. Per parlare, puoi:
   - Digitare testo personalizzato nella casella di input e premere il pulsante `Riproduci>` (o `Enter`).
   - Fare clic su una o più frasi salvate nella finestra Categorie.
3. Durante la riproduzione, il pulsante mostra `>` per indicare lo stato attivo. Puoi premerlo di nuovo per fermare istantaneamente.

## Finestra Categorie (frasi)
- Aprila con il pulsante `Categorie`. Ridimensiona o ricorda la dimensione tramite Impostazioni.
- Seleziona una categoria in alto e le frasi in basso.
- Un clic su una frase la riproduce automaticamente se `Pronuncia subito la frase quando la selezioni` è attivato; altrimenti, la frase viene caricata nella casella di input principale senza essere pronunciata.

### Gestione categorie
- Aggiungi: seleziona categoria → (scorciatoia: `F3`) dialogo `Aggiungi nuova categoria` → inserisci nome → `OK`.
- Rinomina: seleziona categoria → (scorciatoia: `F4`) `Cambia nome categoria` → cambia nome → `OK`.
- Sposta selezione: `F5`/`F6` per elemento precedente/successivo.
- Elimina: seleziona categoria → (scorciatoia: `F8`) `Elimina categoria`; conferma eliminazione della categoria e delle sue frasi.

### Gestione frasi
- Aggiungi: seleziona frase → (scorciatoia: `F3`) `Aggiungi nuova frase` → inserisci testo → `OK`.
- Modifica: seleziona frase → (scorciatoia: `F4`) `Modifica frase` → aggiorna testo → `OK`.
- Sposta selezione: `F5`/`F6` per frase precedente/successiva.
- Elimina: seleziona frase → (scorciatoia: `F8`) `Elimina frase`; conferma.

## Impostazioni (F2)
Apri Impostazioni (`F2`) per controllare:
- **Testo predefinito**: attiva/disattiva `Inserisci automaticamente il testo predefinito` e personalizza la frase predefinita.
  - `Mostra la tastiera touch` — apre la tastiera touch su schermo quando la casella di input riceve il focus, posizionata vicino alla finestra principale.
- **Lingua dell'interfaccia**: scegli la lingua UI (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **Voce**: seleziona una voce SAPI installata in base alla tua preferenza.
- **Volume/Velocità**: regola `Volume (10-100)` e `Velocità (-10 a 10)`.
- `Prova voce`: ascolta la voce selezionata.
- **Finestra categorie**:
  - `Riduci automaticamente la finestra delle categorie` dopo la selezione.
  - `Ricorda la dimensione della finestra delle categorie`.
- **Comportamento riproduzione**:
  - `Pronuncia subito la frase quando la selezioni` (riproduzione automatica al clic).
  - `Ferma la riproduzione precedente` — ferma automaticamente qualsiasi riproduzione in corso prima di avviare una nuova frase.
  - `Aumenta il volume di SimonSays durante la riproduzione` — alza il volume master del sistema durante la riproduzione, ripristinandolo al termine.
  - `Riduci temporaneamente gli altri audio durante la riproduzione` — riduce il volume di tutte le altre applicazioni durante la riproduzione, ripristinandolo al termine.
- `OK` salva le modifiche; `Annulla` le scarta.

## Menu dell'icona nell'area di notifica
- `Mostra` / `Nascondi` interfaccia principale.
- `Impostazioni`.
- `Informazioni` mostra versione e crediti.
- `Web` apre la pagina del progetto (se configurata).
- `Feedback` apre un [modulo di feedback](https://forms.gle/KMbpEDYmwnFJUhgy8) nel browser predefinito.
- `Esci` chiude SimonSays.

Puoi spostare la finestra popup dell'area di notifica e altri dialoghi trascinando la barra del titolo; ridimensiona la finestra Categorie trascinando i bordi, e attiva `Ricorda la dimensione della finestra delle categorie` nelle Impostazioni per mantenere la dimensione.

## Mescolare voce e suoni
- Usa il delimitatore `♫` (`SOUND_NOTE_DELIMITER`) per inserire nomi di file audio nel testo:
  - Esempio: `Ciao ♫notification.wav♫, attendere prego.`
  - Il testo fuori dai delimitatori viene pronunciato; il testo tra i delimitatori viene trattato come file audio e riprodotto in linea.
- Formati audio supportati: `.wav`, `.mid`, `.midi`, `.mp3`.
- Usa percorsi assoluti o relativi accessibili al processo. Accoppia i delimitatori per evitare che il testo finale venga trattato come audio.

## Importare ed esportare categorie
- Esportare: puoi esportare tutte le categorie o solo quella selezionata quando richiesto. I messaggi di successo/errore appaiono nella lingua selezionata (scorciatoia: `F10`).
- Importare: scegli un file; se la categoria esiste, ti verrà chiesto di sovrascrivere. I messaggi di successo/errore appaiono nella lingua selezionata (scorciatoia: `F9`).

## Risoluzione problemi
- **Inizializzazione voce fallita**: verifica che SAPI sia installato e che sia presente una voce compatibile.
- **Nessuna riproduzione audio**: conferma percorso/estensione e delimitatori `♫` accoppiati.
- **Errori di importazione/esportazione**: controlla permessi e posizione del disco; riprova.
- **Lingua UI non applicata**: riapri Impostazioni e conferma la selezione della lingua; assicurati che esistano stringhe di localizzazione per la lingua scelta.

## Informazioni
- Il dialogo `Informazioni` mostra versione, descrizione e copyright.
