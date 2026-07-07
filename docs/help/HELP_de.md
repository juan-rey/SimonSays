# SimonSays Hilfe

## Übersicht
SimonSays ist ein Windows-Text-zu-Sprache-Assistent (TTS), erstellt von Juan Rey Saura, optimiert für schnelle Sprache, Phrasenbibliotheken und eingebettete Soundwiedergabe. Er unterstützt mehrere Sprachen, anpassbare Stimmen und benutzerfreundliche Funktionen für effiziente Kommunikation.

SimonSays nutzt den freien Platz links auf der Windows-Taskleiste und bietet schnellen Zugriff auf ein Dutzend Kategorien häufig verwendeter (und anpassbarer) Phrasen mit nur wenigen Klicks.

## Tastenkürzel
- `F1`: Hilfe öffnen.
- `F2`: Einstellungen öffnen.
- `F3`: Einstellungen für Blick- / Verweilklick.
- `F4`: Aktuelle Auswahl bearbeiten.
- `F5` / `F6`: Zur vorherigen / nächsten Auswahl in Listen.
- `F7`: Nach aktueller Auswahl hinzufügen (Kategorie-/Phrasenkontext).
- `F8`: Aktuelle Auswahl löschen.
- `F9`: Kategorien importieren.
- `F10`: Kategorien exportieren.
- `F11` / `Ctrl -`: Herauszoomen (Kategorienfenster).
- `F12` / `Ctrl +`: Hineinzoomen (Kategorienfenster).
- `Ctrl 0`: Zoom zurücksetzen.
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
- Umbenennen: Kategorie auswählen → (Kürzel: `F4`) `Kategoriename ändern` → Namen ändern → `OK`.
- Auswahl verschieben: `F5`/`F6` für vorheriges/nächstes Element.
- Hinzufügen: Kategorie auswählen → (Kürzel: `F7`) Dialog `Neue Kategorie hinzufügen` → Namen eingeben → `OK`.
- Löschen: Kategorie auswählen → (Kürzel: `F8`) `Kategorie löschen`; Löschung von Kategorie und Phrasen bestätigen.



#### Kategorie-Symbol festlegen oder ändern
Jede Kategorie kann auf ihrer Schaltfläche ein Emoji oder eine benutzerdefinierte `.ico`-Datei anzeigen. Das Symbol wird im Dialog „Hinzufügen/Bearbeiten“ mit dem Trennpräfix `##` angegeben:

```
<symbol>##<kategoriename>
```

- **Emoji**: Emoji-Zeichen gefolgt von `##` und dem Namen eingeben.
  - Beispiel: `🙂##Grüße`
- **Symbol-Datei** (`.ico`): Dateipfad (absolut oder relativ zum SimonSays-Ordner) gefolgt von `##` und dem Namen eingeben.
  - Beispiel: `C:\icons\smile.ico##Grüße`
- **Ohne Symbol**: Nur den Kategorienamen ohne `##` eingeben.
  - Beispiel: `Grüße`

> Hinweis: Für dateibasierte Symbole werden nur `.ico`-Dateien unterstützt. Andere Dateiendungen werden ignoriert und es wird kein Symbol angezeigt.

### Phrasen verwalten
- Bearbeiten: Phrase auswählen → (Kürzel: `F4`) `Satz bearbeiten` → Text aktualisieren → `OK`.
- Auswahl verschieben: `F5`/`F6` für vorherige/nächste Phrase.
- Hinzufügen: Phrase auswählen → (Kürzel: `F7`) `Neuen Satz hinzufügen` → Text eingeben → `OK`.
- Löschen: Phrase auswählen → (Kürzel: `F8`) `Satz löschen`; bestätigen.



#### Phrasen-Symbol festlegen oder ändern
Phrasen verwenden dieselbe Symbolpräfix-Konvention wie Kategorien, mit dem optionalen Trennzeichen `##` vor dem Phrasentext:

```
<symbol>##<phrasentext>
```

- **Emoji**: `👍##Klingt gut!`
- **Symbol-Datei**: `C:\icons\check.ico##Klingt gut!`
- **Ohne Symbol**: `Klingt gut!`

Phrasen können auch eine eingebettete Audiodatei enthalten. Wenn Symbol, Text und Audiodatei kombiniert werden, lautet das vollständige Format:

```
<symbol>##<phrasentext>::<audiodatei>
```

- Beispiel: `🔔##Achtung::notification.wav`

Das Präfix `##` und das Suffix `::` sind optional und unabhängig:
- Nur Symbolpräfix: `🔔##Achtung`
- Nur Audiosuffix: `Achtung::notification.wav`
- Beides: `🔔##Achtung::notification.wav`
- Keines: `Achtung`

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

## Blick- / Verweilklick (F3)
Aktivieren Sie Phrasen und Schaltflächen freihändig, indem Sie Ihren Blick (oder den Mauszeiger) für eine festgelegte Dauer darauf halten. Diese Funktion ist neu — sie braucht Tests und Feedback: Teilen Sie uns über die Option `Feedback` im Infobereich-Menü mit, wie sie mit Ihrem Eyetracker funktioniert.

Unterstützte Konfigurationen:
- **Jede Augensteuerungssoftware, die den Mauszeiger bewegt** (Irisbond EasyClick, Tobii Dynavox Computer Control, Cursor-Modi der Windows-Augensteuerung, OptiKey, ...): Der Verweilklick folgt dem Mauszeiger.
- **Direkte Blickerfassung, ohne Mausbewegung**: Irisbond Hiru (HID-Modus) und Tobii-Geräte über deren eigene installierte Software — verifiziert mit dem Tobii Eye Tracker 4C und dem Tobii Dynavox PCEye 5; der Tobii Eye Tracker 5 sollte funktionieren, wurde aber noch nicht getestet.

Öffnen Sie die Blick- / Verweilklick-Einstellungen (`F3`) zum Konfigurieren (die Steuerelemente dieses Dialogs werden auf Englisch angezeigt):
- **Aktivierungsmodus**: Wählen Sie zwischen `Automatic` — erkennt Vorhandensein und Verhalten des Geräts und entscheidet, ob der Verweilklick über den Mauszeiger oder über die Blickerfassung aktiviert wird (wird ein Eyetracker oder zugehörige Software erkannt, wird der Verweilklick aktiviert; wird nichts erkannt, bleibt er deaktiviert) —, `Mouse cursor`, um den Verweilklick immer über den Mauszeiger zu aktivieren, `HID eye tracker`, um ihn immer über die Blickerfassung zu aktivieren, oder `Off`, um ihn unabhängig von vorhandenen Geräten immer zu deaktivieren.
- **Verweilzeit** (`Dwell time`): Dauer (in Millisekunden), die nötig ist, um durch Ansehen eines Elements einen Klick auszulösen. Während des Verweilens wird ein Fortschrittsindikator angezeigt.
- **Toleranzradius** (`Tolerance radius`): Blicktoleranz (in Pixeln), die leichte Augenbewegungen erlaubt, ohne den Verweilklick abzubrechen.
- **Abklingzeit** (`Cooldown`): Millisekunden nach einer Aktivierung, in denen kein weiterer Verweilklick ausgelöst wird (verhindert versehentliche Wiederholungsklicks).
- **Fortschrittsfarbe** (`Progress color`): Farbe des Verweil-Fortschrittsindikators (standardmäßig die Windows-Akzentfarbe).
- **Erkennen, wie Sie den Tracker verwenden** (`Detect how you use the tracker`): Zwei Schaltflächen, die der App helfen zu erkennen, ob Sie den Mauszeiger mit einem Eyetracker bewegen oder einfach auf Schaltflächen klicken.
- **Erkannt** (`Detected`): Zeigt den erkannten Gerätetyp und weitere Informationen zur zugehörigen Software.
- Eine `Reset`-Schaltfläche (unten links) setzt alle Verweilklick-Einstellungen auf die Standardwerte zurück.
- Die Schaltfläche `OK` speichert Änderungen; `Abbrechen` verwirft sie.

## Infobereich-Menü
- `Anzeigen` / `Ausblenden` der Hauptoberfläche.
- `Einstellungen`.
- `Gaze / Dwell-click` (Blick- / Verweilklick).
- `Über` zeigt Version und Credits.
- `Web` öffnet die Projektseite (falls konfiguriert).
- `Feedback` öffnet ein [Feedback-Formular](https://forms.gle/KMbpEDYmwnFJUhgy8) im Standardbrowser.
- `Beenden` beendet SimonSays.

Sie können das Popup-Fenster des Infobereichs und andere Dialoge durch Ziehen der Titelleiste verschieben; ändern Sie die Größe des Kategorienfensters durch Ziehen der Ränder, und aktivieren Sie `Größe des Kategorienfensters merken` in den Einstellungen, um die Größe beizubehalten.

## Sprache und Sounds mischen
- Verwenden Sie den Begrenzer `♫` (`SOUND_NOTE_DELIMITER`), um Audiodateinamen in den Text einzubetten:
  - Beispiel: `Hallo ♫notification.wav♫, bitte warten.`
  - Text außerhalb der Begrenzer wird gesprochen; Text dazwischen wird als Sounddatei behandelt und inline abgespielt.
- Unterstützte Audioformate: `.wav`, `.mid`, `.midi`, `.mp3`.
- Verwenden Sie absolute oder relative Pfade, die für den Prozess zugänglich sind. Paaren Sie Begrenzer, um zu vermeiden, dass nachfolgender Text als Audio behandelt wird.



### Suchreihenfolge für Sound- und Symboldateien
Wenn ein Sound- oder Symbol-Dateiname keinen Laufwerksbuchstaben oder vollständigen Pfad enthält, sucht SimonSays in dieser Reihenfolge und verwendet den ersten Treffer:

1. **AppData-Ordner** — `%APPDATA%\SimonSays\` (benutzerspezifisch; Dateien hier ablegen, um sie von der Installation zu trennen).
2. **Arbeitsverzeichnis** — aktuelles Arbeitsverzeichnis des Prozesses (nur wenn es sich vom Ordner der EXE unterscheidet).
3. **Ausführungsordner** — der Ordner, der `SimonSays.exe` enthält.

Wird die Datei dort nicht gefunden, wird der integrierte Fallback-Sound verwendet.

> **Tipp:** Eigene Sound- und Symboldateien am besten in `%APPDATA%\SimonSays\` speichern; sie bleiben bei Updates erhalten und sind benutzerspezifisch.

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
