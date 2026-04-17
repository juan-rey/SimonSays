# SimonSays Hilfe

## Übersicht
SimonSays ist ein Windows-Text-zu-Sprache-Assistent (TTS), erstellt von Juan Rey Saura, optimiert für schnelle Sprache, Phrasenbibliotheken und eingebettete Soundwiedergabe. Er unterstützt mehrere Sprachen, anpassbare Stimmen und benutzerfreundliche Funktionen für effiziente Kommunikation.

SimonSays nutzt den freien Platz links auf der Windows-Taskleiste und bietet schnellen Zugriff auf ein Dutzend Kategorien häufig verwendeter (und anpassbarer) Phrasen mit nur wenigen Klicks.

## Tastenkürzel
- `F1`: Hilfe öffnen.
- `F2`: Einstellungen öffnen.
- `F3`: Nach aktueller Auswahl hinzufügen (Kategorie-/Phrasenkontext).
- `F4`: Aktuelle Auswahl bearbeiten.
- `F5` / `F6`: Zur vorherigen / nächsten Auswahl in Listen.
- `F8`: Aktuelle Auswahl löschen.
- `F9`: Kategorien importieren.
- `F10`: Kategorien exportieren.
- Schaltfläche `Abspielen>`: Wiedergabe starten.

## Start und Grundbedienung
1. Starten Sie SimonSays; es erscheint im Windows-Infobereich.
2. Zum Sprechen können Sie:
   - Benutzerdefinierten Text in das Eingabefeld eingeben und die Schaltfläche `Abspielen>` drücken (oder `Enter`).
   - Eine oder mehrere gespeicherte Phrasen im Kategorienfenster anklicken.
3. Während der Wiedergabe zeigt die Schaltfläche `>` an, dass die Wiedergabe aktiv ist. Sie können erneut drücken, um sofort zu stoppen.

## Kategorienfenster (Phrasen)
- Öffnen Sie es mit der Schaltfläche `Kategorien`. Größe ändern oder merken über Einstellungen.
- Wählen Sie eine Kategorie oben und Phrasen unten.
- Ein Klick auf eine Phrase spricht sie sofort, wenn `Satz bei Auswahl sofort aussprechen` aktiviert ist; andernfalls wird die Phrase nur ins Eingabefeld geladen.

### Kategorien verwalten
- Hinzufügen: Kategorie auswählen → (Kürzel: `F3`) Dialog `Neue Kategorie hinzufügen` → Namen eingeben → `OK`.
- Umbenennen: Kategorie auswählen → (Kürzel: `F4`) `Kategoriename ändern` → Namen ändern → `OK`.
- Auswahl verschieben: `F5`/`F6` für vorheriges/nächstes Element.
- Löschen: Kategorie auswählen → (Kürzel: `F8`) `Kategorie löschen`; Löschung von Kategorie und Phrasen bestätigen.

### Phrasen verwalten
- Hinzufügen: Phrase auswählen → (Kürzel: `F3`) `Neuen Satz hinzufügen` → Text eingeben → `OK`.
- Bearbeiten: Phrase auswählen → (Kürzel: `F4`) `Satz bearbeiten` → Text aktualisieren → `OK`.
- Auswahl verschieben: `F5`/`F6` für vorherige/nächste Phrase.
- Löschen: Phrase auswählen → (Kürzel: `F8`) `Satz löschen`; bestätigen.

## Einstellungen (F2)
Öffnen Sie die Einstellungen (`F2`) um zu kontrollieren:
- **Standardtext**: `Standardtext automatisch einfügen` aktivieren/deaktivieren und Standardphrase anpassen.
  - `Bildschirmtastatur anzeigen` — öffnet die Bildschirmtastatur, wenn das Eingabefeld den Fokus erhält, in der Nähe des Hauptfensters positioniert.
- **Oberflächensprache**: UI-Sprache wählen (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **Stimme**: Installierte SAPI-Stimme entsprechend Ihrer Präferenz auswählen.
- **Lautstärke/Geschwindigkeit**: `Lautstärke (10-100)` und `Geschwindigkeit (-10 bis 10)` anpassen.
- `Stimme testen`: Die ausgewählte Stimme anhören.
- **Kategorienfenster**:
  - `Kategorienfenster automatisch minimieren` nach Auswahl.
  - `Größe des Kategorienfensters merken`.
- **Wiedergabeverhalten**:
  - `Satz bei Auswahl sofort aussprechen` (Auto-Wiedergabe bei Klick).
  - `Vorherige Wiedergabe stoppen` — stoppt automatisch jede laufende Wiedergabe, bevor eine neue Phrase gestartet wird.
  - `Lautstärke von SimonSays beim Abspielen erhöhen` — erhöht die Systemlautstärke während des Sprechens und stellt sie nach Ende der Wiedergabe wieder her.
  - `Andere Audios beim Abspielen vorübergehend reduzieren` — reduziert die Lautstärke aller anderen Anwendungen während des Sprechens und stellt sie nach Ende der Wiedergabe wieder her.
- `OK` speichert Änderungen; `Abbrechen` verwirft sie.

## Infobereich-Menü
- `Anzeigen` / `Ausblenden` der Hauptoberfläche.
- `Einstellungen`.
- `Über` zeigt Version und Credits.
- `Web` öffnet die Projektseite (falls konfiguriert).
- `Beenden` beendet SimonSays.

Sie können das Popup-Fenster des Infobereichs und andere Dialoge durch Ziehen der Titelleiste verschieben; ändern Sie die Größe des Kategorienfensters durch Ziehen der Ränder, und aktivieren Sie `Größe des Kategorienfensters merken` in den Einstellungen, um die Größe beizubehalten.

## Sprache und Sounds mischen
- Verwenden Sie den Begrenzer `♫` (`SOUND_NOTE_DELIMITER`), um Audiodateinamen in den Text einzubetten:
  - Beispiel: `Hallo ♫notification.wav♫, bitte warten.`
  - Text außerhalb der Begrenzer wird gesprochen; Text dazwischen wird als Sounddatei behandelt und inline abgespielt.
- Unterstützte Audioformate: `.wav`, `.mid`, `.midi`, `.mp3`.
- Verwenden Sie absolute oder relative Pfade, die für den Prozess zugänglich sind. Paaren Sie Begrenzer, um zu vermeiden, dass nachfolgender Text als Audio behandelt wird.

## Kategorien importieren und exportieren
- Exportieren: Sie können alle Kategorien oder nur die ausgewählte exportieren. Erfolgs-/Fehlermeldungen erscheinen in der gewählten Sprache (Kürzel: `F10`).
- Importieren: Datei auswählen; falls die Kategorie existiert, werden Sie gefragt, ob Sie überschreiben möchten. Erfolgs-/Fehlermeldungen erscheinen in der gewählten Sprache (Kürzel: `F9`).

## Fehlerbehebung
- **Stimminitialisierung fehlgeschlagen**: Überprüfen Sie, ob SAPI installiert ist und eine kompatible Stimme vorhanden ist.
- **Keine Soundwiedergabe**: Bestätigen Sie Dateipfad/Erweiterung und gepaarte `♫`-Begrenzer.
- **Import-/Exportfehler**: Überprüfen Sie Berechtigungen und Speicherort; versuchen Sie es erneut.
- **UI-Sprache nicht angewandt**: Öffnen Sie die Einstellungen erneut und bestätigen Sie die Sprachauswahl; stellen Sie sicher, dass Lokalisierungszeichenfolgen für die gewählte Sprache vorhanden sind.

## Über
- Der Dialog `Über` zeigt Version, Beschreibung und Copyright an.
