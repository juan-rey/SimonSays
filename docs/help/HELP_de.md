# SimonSays-Hilfe

## Überblick
SimonSays ist ein Text-zu-Sprache-Assistent (TTS) für Windows, entwickelt von Juan Rey Saura. Er entstand als Werkzeug für Menschen mit ALS und andere, die nicht mehr sprechen können, damit sie schnell kommunizieren können, ohne zu verlassen, was sie gerade am Computer tun. Mit der Zeit wurde klar, dass dasselbe Konzept als allgemeines Werkzeug der UK (Unterstützten Kommunikation) für alle taugt, denen das Sprechen schwerfällt oder unmöglich ist; zum Beispiel wegen einer Dysarthrie, einer Laryngektomie, einer Sprechapraxie, eines Schlaganfalls oder einer Hirnverletzung. Er ist auf schnelles Sprechen, Satzbibliotheken und das Abspielen von Klängen mitten im Text ausgelegt. Er unterstützt mehrere Sprachen, einstellbare Stimmen und Funktionen für eine effiziente Kommunikation.

SimonSays nutzt den freien Platz in der Windows-Taskleiste und bietet mit nur ein paar Klicks schnellen Zugriff auf rund ein Dutzend Kategorien häufig verwendeter (und anpassbarer) Sätze. Da er Ihre anderen Fenster nie verdeckt, können Sie in Ihrem Browser, Ihren Dokumenten oder einem Videoanruf bleiben und jederzeit sprechen: mit der Tastatur, mit der Maus oder freihändig mit einem Eyetracker.

Anwendungsfälle:
- Intensive Nutzung mehrerer Windows-Anwendungen, ohne die Kommunikation aus dem Blick zu verlieren.
- Boards, die Sprachtherapeutinnen und Sprachtherapeuten für Menschen mit Sprechbehinderungen erstellen.
- Online-Besprechungen mit [Virtual CABLE](https://vb-audio.com/Cable/) (siehe [SimonSays in Online-Besprechungen verwenden](#simonsays-in-online-besprechungen-verwenden)).

SimonSays soll keine vollständigen Kommunikationsprogramme wie Mind Express, TD Communicator, Grid 3 oder OptiKey ersetzen, ebenso wenig die fachliche Beratung und Begleitung, die meist dazugehört. Er erfüllt eine engere Aufgabe: schnelles Sprechen aus der Taskleiste, während Sie in anderen Anwendungen weiterarbeiten. Er kann neben diesen Programmen laufen; zum Beispiel kann OptiKey oder eine andere Augensteuerung den Mauszeiger bewegen, während SimonSays Ihre Sätze spricht.

SimonSays ist Open Source und kostenlos (frei im Sinne von Freiheit, nicht nur im Sinne des Preises), und das bleibt so: Alle dürfen es nutzen, untersuchen, anpassen und weitergeben. Es enthält keine Werbung und sammelt keine Daten.

SimonSays nähert sich der Version 1.0. Vor diesem Meilenstein braucht es Menschen, die es in ihrem Alltag ausprobieren, mit ihren eigenen Stimmen, Boards und Eyetrackern: Wenn etwas nicht funktioniert, umständlich ist oder fehlt, sagen Sie es uns bitte (siehe [Feedback, Support und Datenschutz](#feedback-support-und-datenschutz)). Jede Rückmeldung hilft.

## Inhalt
- [Erste Schritte](#erste-schritte)
- [Kategorienfenster](#kategorienfenster)
- [Standardsätze](#standardsätze)
- [Tastenkombinationen](#tastenkombinationen)
- [Einstellungen (F2)](#einstellungen-f2)
- [Blick / Verweilklick (F3)](#blick--verweilklick-f3)
- [SimonSays in Online-Besprechungen verwenden](#simonsays-in-online-besprechungen-verwenden)
- [Menü des Infobereichssymbols](#menü-des-infobereichssymbols)
- Boards bearbeiten (für Fortgeschrittene):
  - [Kategorien und Sätze bearbeiten](#kategorien-und-sätze-bearbeiten) (und [welchen Editor verwenden](#welchen-editor-verwenden))
  - [Sprache und Klänge mischen](#sprache-und-klänge-mischen)
  - [Aussehen anpassen (Stile für Board und Kategorien)](#aussehen-anpassen-stile-für-board-und-kategorien)
  - [Kategorien importieren und exportieren](#kategorien-importieren-und-exportieren)
  - [Änderungen rückgängig machen](#änderungen-rückgängig-machen)
  - [Wo SimonSays Ihre Dateien speichert](#wo-simonsays-ihre-dateien-speichert)
- [Fehlerbehebung](#fehlerbehebung)
- [Feedback, Support und Datenschutz](#feedback-support-und-datenschutz)

## Erste Schritte
1. Starten Sie SimonSays. Es erscheint in einem freien Bereich der Windows-Taskleiste und fügt dem Infobereich ein Symbol hinzu. Die Taskleiste kann sich unten, oben, links oder rechts am Bildschirm befinden:
   - Wenn die Taskleistensymbole zentriert sind, sitzt SimonSays ganz am Anfang der Taskleiste (am linken Ende, oder oben bei einer seitlichen Taskleiste). Wenn sie linksbündig sind, sitzt es direkt vor dem Infobereich.
   - Bei einer kleinen (schmalen) Taskleiste wird die SimonSays-Leiste niedriger, mit kleinerer Schrift und den zwei Schnellzugriffsschaltflächen nebeneinander. Bei einer Taskleiste am linken oder rechten Bildschirmrand werden die Bedienelemente untereinander gestapelt.
   - Das Kategorienfenster öffnet sich neben der Taskleiste, und der Pfeil auf der Schaltfläche `Kategorien` zeigt, in welche Richtung es sich öffnet.
   - Wenn Sie die Taskleiste verschieben oder ihre Größe ändern, während SimonSays läuft, folgt es ihr innerhalb weniger Sekunden.
2. Zum Sprechen können Sie:
   - Auf das Textfeld in der Leiste klicken (oder darauf verweilen): Neben der Taskleiste öffnet sich ein größeres Eingabefeld. Geben Sie Ihren Text ein und drücken Sie `Enter`, um ihn zu sprechen; das Feld schließt sich und der Fokus wechselt auf `Abspielen>`, sodass `Leertaste` den Text erneut spricht (oder stoppt). `Esc` schließt das Feld, ohne zu sprechen, und bringt Sie zu dem Fenster zurück, das Sie gerade benutzt haben; Ihr Text bleibt in der Leiste, wo `Abspielen>` ihn erneut spricht.
   - Auf einen oder mehrere gespeicherte Sätze im Kategorienfenster klicken.
3. Während der Wiedergabe zeigt die Schaltfläche `>` an. Sie können sie erneut drücken, um sofort zu stoppen.

![Die SimonSays-Leiste in der Taskleiste](img/bar.png)

Neben der Schaltfläche `Kategorien` bieten zwei kleine **Schnellzugriffsschaltflächen** mit einem Klick Zugang zu zwei Dingen, die Sie sonst über eine Tastenkombination oder das Infobereichsmenü erreichen würden:

- 📂 öffnet ein auf Ihrem Computer gespeichertes Board; das ist derselbe Import wie mit `F9`, beginnt aber in Ihrem Board-Ordner (`%LocalAppData%\SimonSays\boards\`), in dem exportierte und heruntergeladene Boards liegen (siehe [Kategorien importieren und exportieren](#kategorien-importieren-und-exportieren)).
- ⚙️ öffnet die Einstellungen, wie `F2`.

Beide lassen sich mit `Schnellzugriffsschaltflächen und -menü anzeigen` in den Einstellungen (`F2`) ausblenden, was dem Eingabefeld mehr Platz lässt. Dieselbe Einstellung schaltet auch das Kontextmenü (Rechtsklick) des Kategorienfensters aus.

Wenn SimonSays ausgeblendet ist, klicken Sie auf sein Symbol im Infobereich, um es wieder anzuzeigen.

## Kategorienfenster
![Das Kategorienfenster mit den Standardsätzen](img/categories.png)

- Öffnen Sie es mit der Schaltfläche `Kategorien`. Ändern Sie seine Größe, oder aktivieren Sie `Größe des Kategorienfensters merken` in den Einstellungen, um die Größe beizubehalten.
- Wählen Sie oben eine Kategorie und unten Sätze aus.
- Ein Klick auf einen Satz spricht ihn automatisch, wenn `Satz bei Auswahl sofort aussprechen` aktiviert ist; andernfalls wird der Satz in das Haupteingabefeld geladen, ohne gesprochen zu werden.
- Doppelklicken Sie auf eine leere Fläche oder auf den Fensterrand, um die Größe so anzupassen, dass alle Kategorien und Sätze hineinpassen.
- Ziehen Sie eine leere Fläche des Fensters, um es zu verschieben, und drücken Sie `Esc`, um es auszublenden.
- Vergrößern und verkleinern Sie mit `F12` und `F11` (siehe [Tastenkombinationen](#tastenkombinationen)).
- Klicken Sie mit der rechten Maustaste auf eine Schaltfläche oder eine leere Fläche (oder drücken Sie `Umschalt + F10` oder die Menütaste), um ein Menü mit `Einfügen`, `Verkleinern`, `Vergrößern`, `Zoom zurücksetzen`, `Hilfe`, `Einstellungen`, `Blick / Verweilklick...`, `Ausblenden` und `SimonSays beenden...` zu öffnen, jeweils mit ihrer Tastenkombination. Der erste Eintrag zeigt den Titel des Boards (`Unbenanntes Board`, wenn es keinen hat); wählen Sie ihn, um die Credits, die Sprache und die Anzahl der Kategorien und Sätze des Boards zu sehen. Das Menü erscheint nur, solange `Schnellzugriffsschaltflächen und -menü anzeigen` in den Einstellungen aktiviert ist.
- Auf einer Kategorie- oder Satzschaltfläche bietet das Menü außerdem `Bearbeiten...`, `Nach links verschieben`, `Nach rechts verschieben`, `Neue Kategorie hinzufügen...` oder `Neuen Satz hinzufügen...` sowie `Kategorie löschen...` oder `Satz löschen...` für diese Schaltfläche, ohne sie vorher auszuwählen; auf einer Kategorie fragt `Kategorie exportieren...`, ob nur diese Kategorie oder das ganze Board gespeichert werden soll. Die Tastenkombinationen `F4` bis `F8` werden angezeigt, wenn die Schaltfläche Ihre aktuelle Auswahl ist, weil diese Tasten auf die Auswahl wirken. Auf einer leeren Fläche bietet das Menü `Neue Kategorie hinzufügen...`, das eine am Ende hinzufügt; so beginnen Sie auch ein leeres Board.
- Der Streifen zwischen den Kategorien und den Sätzen zeigt eine Erinnerung an die wichtigsten Tastenkombinationen (oder die Beschriftung des Boards, falls es eine hat).

![Das Kontextmenü einer Satzschaltfläche](img/menu.png)

## Standardsätze
SimonSays bringt in jeder seiner 18 Sprachen einen fertigen Satz von Kategorien und Sätzen mit, so geordnet, dass die am häufigsten benötigten Sätze die wenigsten Auswahlschritte erfordern:

- 👋🏻 Häufige Grüße
- 🗣️ Konversationssätze
- 🚨 Notfallsätze
- 🤲 Pflege & Komfort
- 🏥 Gesundheit
- 🏠 Familie & Zuhause
- 👥 Soziales
- ⏰ Zeit & Datum
- 💼 Geschäftliche Sätze
- ✈️ Reisesätze
- 🔊 Sounds

Jede Sprache hat ihre eigenen Kategorien und Sätze: Wenn Sie die Sprache in den Einstellungen (`F2`) ändern, wechselt das Kategorienfenster zum Board dieser Sprache, und Änderungen in einer Sprache wirken sich nicht auf die anderen aus.

Die Sätze werden mit den in Windows installierten Stimmen gesprochen. Ist für Ihre Sprache keine Stimme installiert, erscheinen die Sätze trotzdem, werden aber möglicherweise mit einer anderen Stimme gesprochen (siehe [Fehlerbehebung](#fehlerbehebung)).

Sie können die Standardsätze ändern oder später zurückholen; siehe [Kategorien und Sätze bearbeiten](#kategorien-und-sätze-bearbeiten) und [Standardsätze wiederherstellen](#standardsätze-wiederherstellen).

## Tastenkombinationen
- `F1`: Hilfe öffnen
- `F2`: Einstellungen öffnen
- `F3`: Einstellungen für Blick / Verweilklick
- `Enter`: Den Text im Eingabefeld sprechen
- `Esc`: Das Kategorienfenster ausblenden
- `F11` / `Strg -`: Verkleinern (Kategorienfenster)
- `F12` / `Strg +`: Vergrößern (Kategorienfenster)
- `Strg + 0`: Zoom zurücksetzen
- `Umschalt + F10` oder die Menütaste: Das Menü des Kategorienfensters öffnen (wie ein Rechtsklick)
- `Leertaste` auf `Abspielen>`: Den Text erneut sprechen, oder ihn während der Wiedergabe stoppen

Die Tastenkombinationen zum Bearbeiten von Kategorien, Sätzen und Boards finden Sie unter [Kategorien und Sätze bearbeiten](#kategorien-und-sätze-bearbeiten).

## Einstellungen (F2)
Öffnen Sie die Einstellungen (`F2`), um Folgendes festzulegen. Die Einstellungen haben zwei Spalten.

![Das Einstellungsfenster](img/settings.png)

**Linke Spalte**
- **Standardtext**: Schalten Sie `Standardtext automatisch einfügen` ein oder aus und passen Sie den Standardsatz an.
  - `Bildschirmtastatur anzeigen`: öffnet die Bildschirmtastatur, wenn sich das Eingabefeld öffnet; sie wird neben dem Eingabefeld platziert, auf der von der Taskleiste abgewandten Seite.
- `Schnellzugriffsschaltflächen und -menü anzeigen`: zeigt die Schaltflächen 📂 (Board öffnen) und ⚙️ (Einstellungen) neben der Schaltfläche `Kategorien` im Hauptfenster (siehe [Erste Schritte](#erste-schritte)) und das Kontextmenü des Kategorienfensters (siehe [Kategorienfenster](#kategorienfenster)). Schalten Sie die Option aus, um alles auszublenden und dem Eingabefeld mehr Platz zu geben.
- **Kategorienfenster**:
  - `Kategorienfenster automatisch minimieren` nach einer Auswahl.
  - `Größe des Kategorienfensters merken`.
  - `Kategoriefenster automatisch an den Inhalt anpassen`: passt das Kategorienfenster jedes Mal an seine Kategorien und Sätze an, wenn sie geladen werden; zum Beispiel beim Start, nach einem Sprachwechsel oder nach einem Import.
- `Blick / Verweilklick...`: öffnet die Einstellungen für [Blick / Verweilklick](#blick--verweilklick-f3) über den Einstellungen, die mit Ihren Änderungen geöffnet bleiben. Was Sie dort ändern, wird gespeichert, wenn Sie in diesem Fenster `OK` drücken, auch wenn Sie danach in den Einstellungen `Abbrechen` drücken.

**Rechte Spalte**
- **Sprache**: Wählen Sie die Sprache der Oberfläche (English, Español, العربية, Euskara, Català, 简体中文, Français, Galego, Deutsch, עברית, हिन्दी, Italiano, 日本語, 한국어, Português, Português (Brasil), Русский, Valencià). Jede Sprache hat auch ihre eigenen Kategorien und Sätze (siehe [Standardsätze](#standardsätze)).
- **Stimme**: Wählen Sie eine installierte SAPI-Stimme, die zu Ihrer Sprache passt.
- **Lautstärke/Geschwindigkeit**: Stellen Sie `Lautstärke (10-100)` und `Geschwindigkeit (-10 bis 10)` ein.
- `Stimme testen`: Hören Sie die ausgewählte Stimme an (über das unter `Audioausgabe` gewählte Gerät).
- `Audioausgabe`: wo SimonSays seine Stimme und Klänge abspielt: `Windows-Standard` (die übliche Wahl) oder ein beliebiges angeschlossenes Gerät. Virtuelle Audiokabel stehen an erster Stelle; die brauchen Sie für [Online-Besprechungen](#simonsays-in-online-besprechungen-verwenden). Ist das gewählte Gerät nicht angeschlossen, spielt SimonSays über das Windows-Standardgerät ab und weist Sie einmal darauf hin; die Auswahl bleibt erhalten, wird als `(nicht verbunden)` angezeigt und wieder verwendet, sobald das Gerät zurück ist.
- `Abhör-Einstellungen...`: öffnet die Windows-Aufnahmegeräte, wo Sie für ein virtuelles Kabel **Dieses Gerät als Wiedergabequelle verwenden** einschalten können (siehe [SimonSays in Online-Besprechungen verwenden](#simonsays-in-online-besprechungen-verwenden)).
- **Wiedergabeverhalten**:
  - `Satz bei Auswahl sofort aussprechen` (automatisches Sprechen mit einem Klick).
  - `Vorherige Wiedergabe stoppen`: stoppt automatisch jede laufende Wiedergabe, bevor ein neuer Satz beginnt.
  - `Lautstärke von SimonSays beim Abspielen erhöhen`: erhöht die Lautstärke des Geräts, über das SimonSays abspielt (siehe `Audioausgabe`), während es spricht, und stellt danach den vorherigen Pegel wieder her.
  - `Andere Audios beim Abspielen vorübergehend reduzieren`: senkt die Lautstärke anderer Anwendungen, während es spricht, und stellt sie danach wieder her.

**Unten**
- `Hilfe` (linke untere Ecke, oder `F1` bei geöffneten Einstellungen) öffnet diese Hilfe in der in der Liste `Sprache` ausgewählten Sprache; die Einstellungen bleiben mit Ihren Änderungen geöffnet.
- `OK` speichert die Änderungen; `Abbrechen` verwirft sie.

Standardmäßig sind `Satz bei Auswahl sofort aussprechen`, `Kategorienfenster automatisch minimieren`, `Kategoriefenster automatisch an den Inhalt anpassen`, `Lautstärke von SimonSays beim Abspielen erhöhen` und `Andere Audios beim Abspielen vorübergehend reduzieren` eingeschaltet, und `Vorherige Wiedergabe stoppen` ist ausgeschaltet.

## Blick / Verweilklick (F3)
Aktivieren Sie Sätze und Schaltflächen freihändig, indem Sie Ihren Blick (oder den Mauszeiger) eine festgelegte Zeit lang darauf ruhen lassen. Diese Funktion ist neu: Sie braucht Tests und Rückmeldungen; teilen Sie uns über die Option `Feedback geben` im Infobereichsmenü mit, wie sie mit Ihrem Tracker funktioniert.

Unterstützte Konfigurationen:
- **Jede Augensteuerungssoftware, die den Mauszeiger bewegt** (Irisbond EasyClick, Tobii Dynavox Computer Control, die Cursor-Modi der Windows-Augensteuerung, OptiKey): Der Verweilklick folgt dem Mauszeiger.
- **Direktes Lesen des Blicks (ohne den Mauszeiger zu bewegen)**: Irisbond Hiru (HID-Modus) und Tobii-Tracker über ihre eigene Software; geprüft mit dem Tobii Eye Tracker 4C, dem Tobii Eye Tracker 5 und dem Tobii Dynavox PCEye 5.
- **Tobii Eye Tracker 5**: Seine aktuelle Software installiert nur eine 64-Bit-Engine, daher liest SimonSays ihn über ein kleines Hilfsprogramm, `SimonSaysGaze64.exe`, das daneben installiert wird. Das Hilfsprogramm startet nur bei Bedarf, hat kein Fenster, wird mit SimonSays beendet und behält den Blick auf Ihrem Computer: Er wird nie gespeichert oder irgendwohin gesendet.
- **Lizenz für Tobii Eye Tracker 4C und Eye Tracker 5**: Die Endbenutzerlizenz von Tobii für diese Tracker erlaubt keine medizinische Nutzung, einschließlich der Nutzung in Krankenhäusern und anderen Gesundheitseinrichtungen. Geräte von Tobii Dynavox, etwa das PCEye 5, haben eigene Lizenzbedingungen.
- **Software für den Tobii Eye Tracker 4C**: SimonSays liest den 4C über Tobii Core. Wenn Sie einen Eye Tracker 5 an denselben Computer anschließen, installiert Windows Tobii Experience, das Tobii Core abschaltet, und der 4C funktioniert nicht mehr. Um den 4C wieder zu verwenden, installieren Sie Tobii Core neu; die empfohlene Version ist `Tobii_Eye_Tracking_Core_v2.13.1.7294_x86.exe` von der [Tobii-Einstiegsseite](https://gaming.tobii.com/getstarted/).

Wo der Verweilklick funktioniert:
- In der SimonSays-Leiste, im Kategorienfenster und auf seinen Satz- und Kategorieschaltflächen.
- Auf den Schaltflächen der SimonSays-eigenen Fenster und Fragen: den Einstellungen, dem Fenster Blick / Verweilklick, den Dialogen zum Hinzufügen, Bearbeiten und für die Vorschau sowie allen Bestätigungen wie `Ja` / `Nein` oder `OK` / `Abbrechen`. Diese Schaltflächen sind in Ihrer Sprache beschriftet und reagieren erst etwa eine Dreiviertelsekunde nach dem Öffnen des Fensters auf Verweilen, damit eine Frage, die genau dort erscheint, wo Sie hinschauen, sich nicht selbst beantwortet.
- Wenn der Blick direkt gelesen wird, leuchtet die Schaltfläche, die Sie ansehen, genauso auf wie unter der Maus, sodass Sie sehen, was Sie gleich auswählen, bevor der Fortschritt beginnt. Ein Blinzeln behält die Hervorhebung bei; wegschauen hebt sie auf.

![Das Fenster Blick / Verweilklick](img/dwell.png)

Öffnen Sie die Einstellungen für Blick / Verweilklick (`F3`, das Infobereichsmenü oder die Schaltfläche `Blick / Verweilklick...` in den Einstellungen), um Folgendes einzustellen:
- **Aktivierungsmodus**: Wählen Sie eine der folgenden Optionen:
  - `Automatisch`: erkennt vorhandene Geräte und aktiviert den Verweilklick je nach Lage mit dem Mauszeiger oder mit der Blickerkennung. Die im Hintergrund laufende Tobii-Software (nach der Installation immer aktiv) allein schaltet den Verweilklick mit dem Mauszeiger nicht ein; ein Tobii-Tracker wird nur verwendet, solange er tatsächlich den Blick verfolgt.
  - `Mauszeiger`: aktiviert den Verweilklick immer mit dem Mauszeiger.
  - `HID-Eyetracker`: aktiviert den Verweilklick immer mit der Blickerkennung (Irisbond Hiru im HID-Modus oder Tobii-Tracker). Nur Ihr Blick aktiviert Schaltflächen: Solange kein Blick ankommt (Verfolgung ausgeschaltet oder Tracker getrennt), wird nichts durch Verweilen ausgelöst, und die Maus funktioniert normal.
  - `Aus`: deaktiviert den Verweilklick immer.
  - **Erfassungsverzögerung**: wie lange, in Millisekunden, Ihr Blick (oder der Mauszeiger) auf einer Schaltfläche ruhen muss, bevor der Fortschrittsbalken beginnt (Standard 250). So zeigt ein Blick über die Schaltflächen hinweg überhaupt keinen Fortschritt; die Schaltfläche wird nach der Erfassungsverzögerung plus der Verweilzeit aktiviert. Stellen Sie 0 ein, damit der Fortschritt sofort beginnt.
  - **Verweilzeit**: die Dauer (in Millisekunden), die nötig ist, um beim Anschauen eines Elements einen Klick auszulösen (Standard 500). Während des Verweilens wird eine Fortschrittsanzeige eingeblendet.
  - **Toleranzradius**: die Blicktoleranz (in Pixeln), die kleine Augenbewegungen zulässt, ohne das Verweilen abzubrechen.
  - **Abklingzeit**: Millisekunden nach einer Aktivierung, in denen der Verweilklick nicht erneut auslösen kann (verhindert versehentliche Mehrfachklicks).
  - **Fortschrittsfarbe**: die Farbe der Fortschrittsanzeige (standardmäßig die Windows-Akzentfarbe).
  - **Wegbewegen, bevor Verweilen erneut auslöst** (standardmäßig eingeschaltet): Nach einem Verweilklick kann keine Schaltfläche erneut durch Verweilen aktiviert werden, bis sich Ihr Zeiger oder Blick von dieser Stelle entfernt (weiter als der Toleranzradius) oder Sie kurz wegschauen. Auf einer Schaltfläche zu verweilen aktiviert sie nur einmal, sodass ein langer Satz nicht wiederholt wird, bevor er zu Ende ist. Mausklicks sind davon nicht betroffen.
  - **Erkennen, wie Sie den Tracker verwenden**: zwei Schaltflächen, mit denen die Anwendung erkennt, ob Sie den Mauszeiger mit einem Eyetracker bewegen oder einfach auf Schaltflächen klicken.
  - **Erkannt**: zeigt den erkannten Gerätetyp und weitere Informationen zur zugehörigen Software.
  - `Diagnosebericht speichern...` (unter Erkannt): schreibt einen Bericht über Ihren Eyetracker und seine Software in `hid_dump.txt` und öffnet den Ordner, damit Sie ihn an den Support senden können.
  - Eine Schaltfläche `Zurücksetzen` (unten links) setzt alle Verweileinstellungen auf ihre Standardwerte zurück.
  - Eine Schaltfläche `OK` speichert die Änderungen; `Abbrechen` verwirft sie.

## SimonSays in Online-Besprechungen verwenden
SimonSays kann direkt in einen Videoanruf sprechen (Teams, Zoom, Google Meet…), sodass die anderen Teilnehmenden Ihre Sätze hören, als kämen sie aus Ihrem Mikrofon. Dafür brauchen Sie ein virtuelles Audiokabel, etwa das kostenlose [VB-Audio Virtual Cable](https://vb-audio.com/Cable/):

1. Installieren Sie das virtuelle Audiokabel und starten Sie Windows neu, falls Sie dazu aufgefordert werden.
2. Leiten Sie den Ton von SimonSays in das Kabel: Öffnen Sie die Einstellungen (`F2`) und wählen Sie `CABLE Input` unter `Audioausgabe` (virtuelle Kabel stehen an erster Stelle). Drücken Sie `Stimme testen`, um es zu prüfen: Sie sollten es nicht mehr über Ihre Lautsprecher hören, denn der Ton geht jetzt in das Kabel.
3. Wählen Sie in Ihrer Besprechungs-App `CABLE Output` als Mikrofon.
4. Um SimonSays auch selbst zu hören, drücken Sie in den Einstellungen `Abhör-Einstellungen...`. Die Windows-Aufnahmegeräte öffnen sich: Wählen Sie `CABLE Output`, öffnen Sie **Eigenschaften → Abhören**, aktivieren Sie **Dieses Gerät als Wiedergabequelle verwenden**, wählen Sie unter **Wiedergabe über dieses Gerät** Ihre Lautsprecher oder Ihr Headset und drücken Sie `OK`. Das müssen Sie nur einmal tun.

SimonSays kann **Dieses Gerät als Wiedergabequelle verwenden** nicht für Sie einschalten: Es ist eine Einstellung der Aufnahmeseite des Kabels, die Windows nur Ihnen selbst erlaubt, deshalb bringt Sie `Abhör-Einstellungen...` direkt zum richtigen Fenster.

Gut zu wissen:
- Lassen Sie das Windows-Standardwiedergabegerät auf Ihren Lautsprechern oder Ihrem Headset, nicht auf `CABLE Input`. Wenn **Dieses Gerät als Wiedergabequelle verwenden** auf „Standardwiedergabegerät“ abspielt, während dieses Standardgerät das Kabel ist, läuft der Ton zurück in das Kabel.
- Wenn die Besprechung Ihre Sätze hört, aber keine Soundeffekte (Applaus, eine Türklingel), entfernt die Geräuschunterdrückung der Besprechungs-App sie; verringern oder deaktivieren Sie die Geräuschunterdrückung in der App.
- Wird das Kabel deinstalliert oder getrennt, spielt SimonSays über das Windows-Standardgerät ab und weist Sie einmal darauf hin; sobald das Kabel wieder verfügbar ist, kehrt es dorthin zurück.
- Für Sätze, die für einen Anruf bereitstehen, fügen Sie die Kategorie **Besprechungsvorlage** hinzu: Drücken Sie `F9` oder die Schaltfläche 📂 und öffnen Sie das Board, das in Ihrer Sprache benannt ist (`SimonSays_Besprechungsvorlage_(Deutsch).ssc` auf Deutsch). Es enthält Begrüßungen, „Können Sie mich hören?“, eine kurze Erklärung, dass Sie mit einem Programm sprechen, ja / nein, einverstanden / nicht einverstanden, die Bitte um Wiederholung oder langsameres Sprechen sowie Verabschiedungen. Bearbeiten Sie es, um Sätze für Ihre eigene Besprechung hinzuzufügen.
- Alternativ zu Schritt 2 können Sie SimonSays weiterhin mit der App-spezifischen Windows-Einstellung umleiten (**Einstellungen → System → Sound → Lautstärkemixer**) und `Audioausgabe` auf `Windows-Standard` lassen. SimonSays erscheint erst in dieser Liste, nachdem es etwas abgespielt hat.

## Menü des Infobereichssymbols
- `Anzeigen` / `Ausblenden` der Hauptoberfläche
- `Einstellungen`
- `Blick / Verweilklick...`
- `Web` öffnet die SimonSays-Webseite
- `Über` zeigt Version und Credits
- `Feedback geben` öffnet ein [Feedback-Formular](https://forms.gle/KMbpEDYmwnFJUhgy8) im Standardbrowser
- `Beenden` schließt SimonSays, nach einer Rückfrage

Sie können das Infobereich-Popup und andere Dialoge verschieben, indem Sie ihre Titelleiste ziehen; ändern Sie die Größe des Kategorienfensters durch Ziehen an seinen Rändern, und aktivieren Sie `Größe des Kategorienfensters merken` in den Einstellungen, um die Größe beizubehalten.

## Kategorien und Sätze bearbeiten
Dieser Abschnitt und die folgenden richten sich an Betreuende, Sprachtherapeutinnen und Sprachtherapeuten sowie fortgeschrittene Nutzer, die Kategorien, Sätze, Klänge und das Aussehen eines Boards ändern. Jede Änderung wird sofort gespeichert (siehe [Änderungen rückgängig machen](#änderungen-rückgängig-machen)).

### Welchen Editor verwenden
- **In SimonSays**: alltägliche Änderungen; Kategorien und Sätze umbenennen, hinzufügen, verschieben oder löschen, ihnen ein Emoji geben, Klänge hinzufügen, eine Liste von Sätzen einfügen oder eine Farbe anpassen. Der Rest dieses Abschnitts erklärt, wie.
- **In [SimonSays Web](https://github.com/juan-rey/simonsays-web)**: größere Arbeiten an Boards, die dort viel einfacher sind: Boards aus UK-Piktogrammen (es kann in der ARASAAC-Piktogrammbibliothek suchen), eigene Symbolbilder und gestaltete Boards mit Farben, Größen und Layouts. Es wird besonders empfohlen, um ein **Fitzgerald-Key**-Board zu gestalten, in dem jede Wortgruppe ihre eigene Farbe hat. Es ist ein visueller Editor, der `.ssc`/`.ssz`-Boards direkt in Ihrem Browser öffnet und speichert, ohne sie hochzuladen; wenn Sie fertig sind, importieren Sie das Board in SimonSays mit `F9` oder der Schaltfläche 📂. Damit lassen sich SimonSays-Boards auch auf anderen Plattformen nutzen.

Tastenkombinationen zum Bearbeiten (Kategorienfenster):
- `F4`: Aktuelle Auswahl bearbeiten
- `F5` / `F6`: Zur vorherigen / nächsten Auswahl in den Listen wechseln
- `F7`: Nach der aktuellen Auswahl hinzufügen (Kategorie oder Satz, je nach Kontext)
- `F8`: Aktuelle Auswahl löschen
- `F9`: Kategorien importieren
- `F10`: Kategorien exportieren
- `Strg + F4`: Den Board-Stil bearbeiten (siehe [Aussehen anpassen](#aussehen-anpassen-stile-für-board-und-kategorien))
- `Strg + F8`: Alle Kategorien und ihre Sätze löschen (fragt zweimal nach)
- `Strg + V` oder `Umschalt + Einfg`: Den kopierten Text als Sätze hinzufügen, einen pro Absatz (siehe [Sätze verwalten](#sätze-verwalten))
- Eine `.txt`-, `.ssc`- oder `.ssz`-Datei auf das Fenster ziehen: fügt eine `.txt` als neue Kategorie hinzu oder importiert ein Board wie mit `F9` (siehe [Kategorien verwalten](#kategorien-verwalten))

> **Tipp:** Exportieren Sie Ihr Board (`F10`), bevor Sie `Strg + F8` verwenden oder ein ganzes Board importieren (`F9`), damit Sie es zurückholen können.

### Kategorien verwalten
- Umbenennen: Kategorie auswählen → (Tastenkombination: `F4`) `Kategoriename ändern` → Namen ändern → `OK`.
- Auswahl verschieben: `F5`/`F6`, um beim Durchgehen der Listen zum vorherigen/nächsten Eintrag zu wechseln.
- Hinzufügen: Kategorie auswählen → (Tastenkombination: `F7`) Dialog `Neue Kategorie hinzufügen` → Namen eingeben → `OK`.
- Löschen: Kategorie auswählen → (Tastenkombination: `F8`) `Kategorie löschen`; bestätigen Sie das Löschen der Kategorie und ihrer Sätze.
- Aus einer Textdatei hinzufügen: Ziehen Sie eine `.txt`-Datei auf das Kategorienfenster → eine Kategorie mit dem Dateinamen wird erstellt, mit einem Satz pro Absatz (Absätze sind durch eine Leerzeile getrennt) → Vorschau prüfen → `OK`. Sie können mehrere Dateien auf einmal ablegen; jede erhält ihre eigene Vorschau. Eine Datei, deren Name einer vorhandenen Kategorie entspricht, wird abgelehnt.
- Ein Board durch Ziehen öffnen: Ziehen Sie eine `.ssc`- oder `.ssz`-Datei auf das Kategorienfenster → SimonSays fragt, ob sie importiert werden soll; mit `Ja` wird sie wie mit `F9` importiert (siehe [Kategorien importieren und exportieren](#kategorien-importieren-und-exportieren)). Enthält die Datei ein ganzes Board, ersetzt sie Ihr aktuelles, und vorher wird eine Kopie des aktuellen Boards als `backup.ssz` gespeichert. Sie können Text- und Board-Dateien zusammen ablegen; jede wird der Reihe nach verarbeitet. Das Öffnen einer `.ssc`- oder `.ssz`-Datei aus dem Datei-Explorer (Doppelklick) stellt dieselbe Frage.

#### Symbol einer Kategorie festlegen oder ändern
Jede Kategorie kann auf ihrer Schaltfläche ein Emoji oder eine eigene Bilddatei (`.ico`, `.png` oder `.jpg`) anzeigen. Geben Sie das Symbol im Dialog zum Hinzufügen oder Bearbeiten mit dem Trennpräfix `##` an:

```
<Symbol>##<Kategoriename>
```

- **Emoji**: Geben Sie das Emoji (oder die Emojis) ein, gefolgt von `##` und dem Namen.
  - Beispiel: `🙂##Begrüßungen`
- **Symboldatei** (`.ico`, `.png`, `.jpg`): Geben Sie den Dateinamen oder den vollständigen Pfad ein, gefolgt von `##` und dem Namen. Ein Dateiname ohne Ordner wird gesucht wie unter [Suchreihenfolge für Klangdateien und Symboldateien](#suchreihenfolge-für-klangdateien-und-symboldateien) beschrieben.
  - Beispiel: `C:\icons\smile.png##Begrüßungen`
- **Kein Symbol**: Geben Sie nur den Kategorienamen ohne `##`-Präfix ein.
  - Beispiel: `Begrüßungen`

> Hinweis: Für dateibasierte Symbole werden nur `.ico`-, `.png`- und `.jpg`-Dateien unterstützt. Andere Dateiendungen werden ignoriert, und es wird kein Symbol angezeigt. PNG-Bilder behalten ihre Transparenz; sehr große Bilder werden beim Laden automatisch verkleinert.

### Sätze verwalten
- Bearbeiten: Satz auswählen → (Tastenkombination: `F4`) `Satz bearbeiten` → Text ändern → `OK`.
- Auswahl verschieben: `F5`/`F6`, um zum vorherigen/nächsten Satz zu wechseln.
- Hinzufügen: Satz auswählen → (Tastenkombination: `F7`) `Neuen Satz hinzufügen` → Text eingeben → `OK`.
- Löschen: Satz auswählen → (Tastenkombination: `F8`) `Satz löschen`; bestätigen.
- Viele auf einmal hinzufügen: Kopieren Sie Text, dessen Sätze durch eine Leerzeile getrennt sind → wählen Sie einen Satz (oder eine Kategorie) → `Strg + V` → Vorschau prüfen → `OK`. Die Sätze werden nach dem ausgewählten Satz oder am Ende der Kategorie hinzugefügt. Zeilenumbrüche innerhalb eines Absatzes werden zu Leerzeichen, und die Symbolmarkierung `##` und die Audiomarkierung `::` funktionieren wie im Dialog `Neuen Satz hinzufügen`.
- Klänge hinzufügen: Ziehen Sie eine oder mehrere `.wav`- oder `.mp3`-Dateien auf das Kategorienfenster → Vorschau prüfen → `OK`. Jede Datei wird zu einem Klangsatz mit dem Dateinamen, der nach dem ausgewählten Satz (oder am Ende der ausgewählten Kategorie) hinzugefügt wird, und die Datei wird in den Ressourcenordner des Boards kopiert, damit sie beim Export mit dem Board mitreist. Eine vorhandene Datei wird nie überschrieben: Gibt es bereits einen anderen Klang mit demselben Namen, wird der neue als `name (2).wav` gespeichert.

#### Symbol eines Satzes festlegen oder ändern
Sätze folgen derselben Symbolpräfix-Konvention wie Kategorien, mit dem optionalen Trennzeichen `##` vor dem Satztext:

```
<Symbol>##<Satztext>
```

- **Emoji**: `👍##Klingt gut!`
- **Symboldatei**: `C:\icons\check.png##Klingt gut!`
- **Kein Symbol**: `Klingt gut!`

Sätze können eine Audiodatei enthalten (siehe [Sprache und Klänge mischen](#sprache-und-klänge-mischen)). Um Symbol, Text und Audio zu kombinieren, verwenden Sie das Format:

```
<Symbol>##<Satztext>::<Audiodatei>
```

- Beispiel: `🔔##Achtung::alert.mp3`

Das Präfix `##` und das Suffix `::` sind optional und voneinander unabhängig; Sie können jede Kombination verwenden:
- Nur Symbolpräfix: `🔔##Achtung`
- Nur Audiosuffix: `Achtung::alert.mp3`
- Beides: `🔔##Achtung::alert.mp3`
- Keines: `Achtung`

## Sprache und Klänge mischen
- Verwenden Sie das Trennzeichen `♫`, um Namen von Audiodateien in den Text einzufügen:
  - Beispiel: `Hallo ♫doorbell.mp3♫, bitte warten Sie.`
  - Text außerhalb der Trennzeichen wird gesprochen; Text dazwischen wird als Klangdatei behandelt und an dieser Stelle abgespielt.
- Unterstützte Audioformate: `.wav` und `.mp3`. MIDI-Dateien (`.mid`, `.midi`) werden in einem Satz akzeptiert, aber nicht abgespielt; wandeln Sie sie in `.wav` oder `.mp3` um.
- Verwenden Sie absolute oder relative Pfade, auf die das Programm zugreifen kann. Achten Sie darauf, dass die Trennzeichen paarweise stehen, damit nachfolgender Text nicht als Audio behandelt wird.
- Um `♫` einzugeben, kopieren Sie es aus dieser Hilfe, oder wählen Sie einen Satz mit einem Klang: Seine `♫…♫`-Markierung wird in das Eingabefeld gesetzt, wo Sie Text darum herum ergänzen können.

### Mit SimonSays gelieferte Klänge
Diese Klänge werden mit SimonSays installiert und können in jedem Text oder Satz über ihren Namen verwendet werden; zum Beispiel `♫laugh.mp3♫` im Eingabefeld, oder `Gut gemacht!::applause.mp3` für eine Satzschaltfläche mit der Beschriftung „Gut gemacht!“, die Applaus abspielt:

`ahem.mp3`, `alert.mp3`, `applause.mp3`, `aww.mp3`, `beep.mp3`, `boo.mp3`, `call_bell.mp3`, `censor.mp3`, `doorbell.mp3`, `drumroll.mp3`, `evil_laugh.mp3`, `fail.mp3`, `hmmmm.mp3`, `kisses.mp3`, `laugh.mp3`, `mmm-hmm.mp3`, `nuh-uh.mp3`, `phone_ring.mp3`, `rimshot.mp3`, `scream.mp3`, `shots.mp3`, `sigh.mp3`

### Suchreihenfolge für Klangdateien und Symboldateien
Wenn ein Klang- oder Symboldateiname keinen Laufwerksbuchstaben oder vollständigen Pfad enthält, sucht SimonSays in dieser Reihenfolge danach und verwendet den ersten Treffer:

1. **Der Ressourcen-Unterordner des aktuellen Boards**: Wenn der Stil des Boards `title` oder `resource-folder` festlegt, dessen eigener Unterordner im Ressourcenordner (siehe [Aussehen anpassen](#aussehen-anpassen-stile-für-board-und-kategorien)).
2. **Ressourcenordner**: `%LocalAppData%\SimonSays\resources\` (der Standardort für Symbole und Klänge von Boards; importierte `.ssz`-Pakete installieren ihre Dateien hier).
3. **SimonSays-Datenordner**: `%LocalAppData%\SimonSays\` (als Rückfalloption für Dateien beibehalten, die frühere Versionen hier abgelegt haben, bevor es den Ressourcenordner gab).
4. **Arbeitsverzeichnis**: das aktuelle Arbeitsverzeichnis des Prozesses (wird nur durchsucht, wenn es sich vom Programmordner unterscheidet).
5. **Programmordner**: der Ordner, der `SimonSays.exe` enthält.

Wird die Datei an keinem dieser Orte gefunden, wird der eingebaute Ersatzklang verwendet (ein Symbol wird einfach nicht angezeigt).

> **Tipp:** Wir empfehlen, eigene Klang- und Symboldateien in `%LocalAppData%\SimonSays\resources\` abzulegen; sie überstehen Programmaktualisierungen und gehören dem jeweiligen Benutzer. Ein Board mit einem Stil `title` oder `resource-folder` erhält dort automatisch einen eigenen Unterordner, der seine Dateien von denen anderer Boards trennt.

## Aussehen anpassen (Stile für Board und Kategorien)
Sie können Farben, Größe und Stil des Kategorienfensters und seiner Schaltflächen ändern; zum Beispiel, um farbcodierte Kommunikations-Boards wie Fitzgerald Key zu erstellen. Stile sind optional: Ohne sie behält das Fenster sein Standardaussehen.

> **Tipp:** Alles Folgende kann in SimonSays eingegeben werden, was für kleine Anpassungen praktisch ist. Um ein ganzes gestaltetes oder Piktogramm-Board zu entwerfen, wird [SimonSays Web](https://github.com/juan-rey/simonsays-web) empfohlen: Sie sehen das Ergebnis während der Arbeit, und es schreibt diese Stile für Sie (siehe [Welchen Editor verwenden](#welchen-editor-verwenden)).

Ein Stil ist eine Liste von `Eigenschaft:Wert;`-Paaren. Werte können sein:
- **Farben**: hexadezimal `#RRGGBB`, z. B. `#FFD966`.
- **Größen**: eine Anzahl Pixel (`96`) oder ein Prozentsatz der Standardgröße (`120%`).
- **Schlüsselwörter**: z. B. `left`, `right`, `top`, `bottom`, `smart`, `normal`, `bold`.
- **Text**: ein Schriftartname oder der Text von Beschriftung, Titel oder Credits.

Alles, was die Anwendung nicht erkennt (eine falsch geschriebene Eigenschaft oder ein ungültiger Wert), wird einfach ignoriert, sodass ein Stil das Fenster nie beschädigt.

### Eine Kategorie gestalten (Bearbeiten-Dialog)
Wählen Sie eine Kategorie aus und drücken Sie `F4` (Bearbeiten), dann fügen Sie nach dem Namen `::` und einen Stil hinzu:

```
<Symbol>##<Kategoriename>::<Eigenschaft>:<Wert>;<Eigenschaft>:<Wert>;
```

- Beispiel: `🧍##Personen::background:#FFD966;` färbt die Schaltfläche Personen gelb. Die Farbe der Kategorie **gilt auch für die Sätze darin**, sodass eine einzige Zeile eine ganze Wortgruppe einfärbt: die Grundlage eines Fitzgerald-Key-Boards.
- Ein Kategoriestil kann `background` und `text-color` für die Kategorieschaltfläche festlegen, und jede Satzeigenschaft (siehe unten) mit dem Präfix `phrase-`, z. B. `phrase-corner-radius:8;`.

### Das ganze Board gestalten (`$$board`)
Das Gesamtaussehen (Fensterhintergrund, Trennlinien und Standardwerte für alle Kategorie- und Satzschaltflächen) ist ein **Board-Stil**. Er wird unter dem reservierten Kategorienamen `$$board` gespeichert (Sie können keine `$$`-Namen für eigene Kategorien verwenden).

- **Bearbeiten** Sie ihn mit `Strg + F4` im Kategorienfenster. Der Editor zeigt ein `Eigenschaft:Wert;` pro Zeile. Wenn Sie mit `OK` bestätigen, wird der neue Stil angewendet und SimonSays fragt „Möchten Sie die Änderungen rückgängig machen?“: Wählen Sie `OK`, um zum vorherigen Stil zurückzukehren, oder `Abbrechen`, um den neuen zu behalten.
- **Holen Sie sich einen**, indem Sie eine gestaltete Kategoriedatei (`.ssc` / `.ssz`) importieren. Wenn Sie eine Datei mit einem Board-Stil importieren, fragt SimonSays, bevor Ihr aktueller ersetzt wird, und zeigt `title` und `credits` des Boards (falls vorhanden) in der Importmeldung. Eine Datei, deren Board-Stil nur `title`, `credits`, `language` oder `resource-folder` festlegt, ändert nichts Sichtbares, daher fragt SimonSays nicht: Enthält sie eine einzige Kategorie, wird diese hinzugefügt und Ihr Aussehen bleibt; enthält sie ein ganzes Board, ersetzt es Ihr Board einschließlich Aussehen, wie jedes ganze Board.

### Stileigenschaften
- **Ganzes Fenster**: `background`, `separator-color`, `text-color`, `font-name`, `font-size`, `text-weight`, `caption` (Beschriftung zwischen den Trennlinien), `title` und `credits` (beim Import des Boards angezeigt), `language` (die Sprache, für die das Board erstellt wurde, z. B. `language:German`; SimonSays verweigert den Import des Boards, solange eine andere Sprache eingestellt ist, und sagt Ihnen, zu welcher Sprache Sie in den Einstellungen, `F2`, wechseln müssen).
- **Alle Kategorieschaltflächen** (Präfix `category-`) und **alle Satzschaltflächen** (Präfix `phrase-`): `background`, `text-color`, `width`, `height`, `corner-radius`, `border-width`, `margin`, `icon-position` (`left`/`right`/`top`/`bottom`/`center`), `icon-size`, `font-name`, `font-size`, `text-weight`, `text-layout`.

Mit `icon-position` auf `center` steht das Symbol in der Mitte der Schaltfläche und der Text wird ausgeblendet (eine reine Symboltaste); verwenden Sie `top`, wenn Sie das Symbol oben und den Namen darunter möchten. Wenn Sie Text über einem zentrierten Symbol möchten, fügen Sie ein ausdrückliches `text-layout` hinzu (z. B. `text-layout:bottom`), dann wird er an dieser Position angezeigt.

`text-layout` akzeptiert ein horizontales (`left`/`center`/`right`) und/oder ein vertikales Schlüsselwort (`top`/`middle`/`bottom`), oder `smart`. Standard ist smart: Der Text wird in dem Platz zentriert, den das Symbol frei lässt (Symbol oben → Beschriftung darunter zentriert; ohne Symbol in der ganzen Schaltfläche zentriert).

### Beispiel: ein farbcodiertes Fitzgerald-Key-Board
Um sechs Wortgruppen einzufärben, bearbeiten Sie jede Kategorie (`F4`) und geben ihr eine Hintergrundfarbe. Das sind die genauen Zeichenfolgen, die Sie im Bearbeiten-Dialog eingeben:

```
🧍##Personen::background:#FFD966;
🏃##Handlungen::background:#93C47D;
📏##Beschreibungen::background:#6FABDC;
🍎##Nomen::background:#F6B26B;
🕒##Verschiedenes::background:#FFFFFF;
👋##Soziales::background:#B4A7D6;
```

Jede Farbe färbt auch die Sätze ihrer Kategorie, sodass das ganze Board farbcodiert ist. Board-weite Anpassungen (etwa das Emoji jeder Kategorie oben mit dem Namen darunter und farbige Trennlinien) kommen aus einem Board-Stil (`Strg + F4`) wie:

```
category-icon-position:top;separator-color:#B4A7D6;
```

Ein vollständiges Fitzgerald-Key-Board, mit Piktogrammen auf jeder Taste und den Sätzen nach Wortgruppen angeordnet, erstellen Sie in [SimonSays Web](https://github.com/juan-rey/simonsays-web) und importieren es mit `F9`.

Stile reisen mit Ihren Kategorien: Der Export (`F10`) schließt sie ein, und der Import (`F9`) bringt sie mit, sodass ein gestaltetes Board als eine einzige Datei weitergegeben werden kann.

## Kategorien importieren und exportieren
Boards werden als `.ssc`-Dateien (Kategorien und Sätze) oder `.ssz`-Dateien (dasselbe, plus die verwendeten Klang- und Symboldateien, in einer einzigen Datei) weitergegeben. Sie können sie auch visuell im Browser mit [SimonSays Web](https://github.com/juan-rey/simonsays-web) erstellen und bearbeiten.

- **Exportieren** (`F10`): Wählen Sie auf Nachfrage, ob alle Kategorien oder nur die ausgewählte exportiert werden sollen; der Speichern-Dialog öffnet sich standardmäßig in `%LocalAppData%\SimonSays\boards\`, Sie können aber überall speichern. SimonSays speichert automatisch eine `.ssz`, wenn Ihre Kategorien oder Sätze Klang- oder Symboldateien verwenden, die es finden kann (`.mid`/`.midi`-Klänge werden nicht eingeschlossen). Der Export aller Kategorien schließt den Board-Stil ein; der Export einer Kategorie nur den Stil dieser Kategorie. Erfolgs- und Fehlermeldungen erscheinen in der ausgewählten Sprache.
- **Importieren** (`F9`): Wählen Sie eine `.ssc`- oder `.ssz`-Datei.
  - Eine Datei mit **mehreren Kategorien oder einem Board-Stil** ist ein ganzes Board: Sie **ersetzt alle Kategorien und Sätze der aktuellen Sprache**. Ihr bisheriges Board wird vorher als `backup.ssz` gespeichert (siehe [Änderungen rückgängig machen](#änderungen-rückgängig-machen)).
  - Eine Datei mit **einer einzigen Kategorie** wird Ihrem Board hinzugefügt; gibt es bereits eine Kategorie mit demselben Namen, werden Sie gefragt, ob sie überschrieben werden soll. Das gilt auch, wenn die Datei ihr Board nur benennt (`title`, `credits` oder `language`, ohne Farben, Größen oder Schriften): Solche Angaben ersetzen nie Ihr Board oder sein Aussehen.
  - Klang- und Symboldateien in einer `.ssz` werden im Ressourcenordner installiert, und das Kategorienfenster passt seine Größe an das importierte Board an. Erfolgs- und Fehlermeldungen erscheinen in der ausgewählten Sprache.

## Änderungen rückgängig machen
SimonSays speichert jede Änderung sofort und hat keinen allgemeinen Rückgängig-Befehl. So kommen Sie zurück:

- **Einen Import rückgängig machen**: Bevor ein Import Ihr Board ersetzt, speichert SimonSays das bisherige Board als `backup.ssz` in `%LocalAppData%\SimonSays\boards\`. Um den Import rückgängig zu machen, drücken Sie `F9` und wählen `backup.ssz`: Ihr bisheriges Board kommt so zurück, wie es war, mit Kategorien und Aussehen, ohne weitere Rückfragen. Nur das zuletzt ersetzte Board wird aufbewahrt: Jeder neue Import eines ganzen Boards überschreibt die Sicherung.
- **Eine Änderung am Board-Stil rückgängig machen**: Nachdem Sie den Board-Stil mit `Strg + F4` bearbeitet haben, fragt SimonSays „Möchten Sie die Änderungen rückgängig machen?“; wählen Sie `OK`, um zum vorherigen Stil zurückzukehren.
- **Eine Änderung an Kategorie oder Satz rückgängig machen**: Mit `F4`, `F7` oder `F8` vorgenommene Änderungen lassen sich nicht automatisch rückgängig machen. Exportieren Sie ein Board, das Ihnen wichtig ist, vor größeren Änderungen (`F10`); ein späterer Import dieser Datei (`F9`) bringt Sie zu diesem Stand zurück.

### Standardsätze wiederherstellen
SimonSays überschreibt nie Ihre eigenen Kategorien und Sätze, daher erscheinen neue Standardsätze eines Updates nur in Sprachen, die Sie noch nicht verwendet haben. Sie können jederzeit zu den eingebauten Kategorien und Sätzen zurückkehren; zum Beispiel, um neu anzufangen oder die neuen Sätze eines Updates zu übernehmen. Das **ersetzt das ganze Board der aktuellen Sprache**:

1. Klicken Sie auf die Schaltfläche 📂 neben `Kategorien` (oder drücken Sie `F9` im Kategorienfenster). Der Board-Ordner öffnet sich.
2. Wählen Sie das Standardsätze-Board Ihrer Sprache; auf Deutsch `SimonSays_Standardsätze_(Deutsch).ssc` (in anderen Sprachen hat es dieselbe Form, in der jeweiligen Sprache; zum Beispiel `SimonSays_default_phrases_(English).ssc`).
3. Bestätigen Sie den Import. Ihr aktuelles Board wird vorher als `backup.ssz` gespeichert, Sie können es also noch zurückholen (siehe [Änderungen rückgängig machen](#änderungen-rückgängig-machen)).

Dieses Board enthält immer die Standardsätze der SimonSays-Version, die Sie verwenden: SimonSays aktualisiert es im Board-Ordner beim Start und beim Sprachwechsel. Wenn Sie Ihr eigenes Board behalten möchten, exportieren Sie es vorher (`F10`) unter einem Namen Ihrer Wahl; `backup.ssz` wird beim nächsten Import überschrieben.

Fehlt die Datei im Board-Ordner, gibt es einen letzten Ausweg: Beenden Sie SimonSays (Infobereichsmenü → `Beenden`), öffnen Sie den Registrierungs-Editor (`regedit`), löschen Sie unter `HKEY_CURRENT_USER\SOFTWARE\SimonSays\Phrases\` den Schlüssel mit dem englischen Namen Ihrer Sprache (zum Beispiel `German`) und starten Sie SimonSays erneut. Die Standardkategorien und -sätze dieser Sprache werden dann neu erstellt, ohne Sicherung.

Für `Portuguese (Brazil)`: Gibt es auch einen Schlüssel `Portuguese`, kopiert SimonSays dieses Board nach `Portuguese (Brazil)`, statt die brasilianischen Standardsätze zu erstellen; löschen Sie daher auch den Schlüssel `Portuguese` (exportieren Sie ihn vorher, falls Sie ihn nutzen).

## Wo SimonSays Ihre Dateien speichert
- **Kategorien, Sätze und Einstellungen** werden in der Windows-Registrierung unter `HKEY_CURRENT_USER\SOFTWARE\SimonSays` gespeichert (das Board jeder Sprache unter `Phrases\<Language>`).
- `%LocalAppData%\SimonSays\resources\`: Ihre Klang- und Symboldateien sowie die aus `.ssz`-Boards installierten; ein Board, dessen Stil `title` oder `resource-folder` festlegt, erhält einen eigenen Unterordner.
- `%LocalAppData%\SimonSays\boards\`: der Standardordner für exportierte Boards, die automatische `backup.ssz` und die mit SimonSays gelieferten Boards: die Standardsätze jeder Sprache sowie fertige Kategorien, die Sie Ihrem Board mit `F9` oder der Schaltfläche 📂 hinzufügen können: Ja / Nein, Schmerz und Körper, Buchstabieren, Spiele, Fragen, Tägliche Pflege, eine Besprechungsvorlage und ein Hörtest, in Ihrer Sprache benannt, sowie `Spelling_NATO`, `Alert_tones` und `Morse_code` für alle Sprachen. Jede fügt eine Kategorie hinzu, ohne Ihr Board zu ersetzen. Die Boards Ihrer Sprache werden beim Start und beim Sprachwechsel in den Einstellungen dorthin kopiert, wann immer das Standard-Board Ihrer Sprache fehlt oder sich geändert hat, und ersetzen die Kopien im Ordner; speichern Sie ein Board, das Sie bearbeiten, daher unter einem neuen Namen.
- `%LocalAppData%\SimonSays\debug\`: Diagnosedateien (wie `hid_dump.txt`), die der Support anfordern kann.

Um einen dieser Ordner zu öffnen, fügen Sie seinen Pfad (zum Beispiel `%LocalAppData%\SimonSays`) in die Adressleiste des Datei-Explorers ein.

## Fehlerbehebung
- **Die Initialisierung der Stimme schlägt fehl**: Prüfen Sie, ob SAPI installiert ist und eine kompatible Stimme vorhanden ist.
- **Keine Stimme für meine Sprache, oder Sätze werden mit falschem Akzent gesprochen**: SimonSays verwendet die in Windows installierten Stimmen. Fügen Sie in den Windows-Einstellungen eine Stimme für Ihre Sprache hinzu (**Zeit und Sprache → Spracherkennung → Stimmen verwalten**) und wählen Sie sie dann in den SimonSays-Einstellungen (`F2`). Einige neuere „natürliche“ Windows-Stimmen stehen anderen Anwendungen möglicherweise nicht zur Verfügung.
- **Keine Klangwiedergabe**: Prüfen Sie Dateipfad und -endung sowie paarweise gesetzte `♫`-Trennzeichen.
- **Die Taskleiste blendet sich bei geöffnetem SimonSays nicht mehr automatisch aus**: Das ist beabsichtigt. SimonSays sitzt in der Taskleiste, daher hält es sie sichtbar, solange es läuft, und schaltet „Taskleiste automatisch ausblenden“ wieder ein, wenn Sie es beenden. Wurde SimonSays unerwartet geschlossen (zum Beispiel durch einen Stromausfall), wird die Option beim nächsten Beenden von SimonSays wieder eingeschaltet.
- **Die Widgets-Schaltfläche verdeckt die SimonSays-Leiste** (Windows 11): Schalten Sie Widgets in den Windows-**Einstellungen → Personalisierung → Taskleiste** aus. SimonSays bietet einmal an, diese Seite zu öffnen, wenn es Widgets zum ersten Mal bemerkt.
- **Das Kategorienfenster ist zu klein oder zu groß**: Doppelklicken Sie auf eine leere Fläche oder den Fensterrand, um es an seinen Inhalt anzupassen, oder zoomen Sie mit `F11` / `F12`.
- **Meine Kategorien oder Sätze sind verschwunden**: Prüfen Sie die Sprache in den Einstellungen (`F2`); jede Sprache hat ihr eigenes Board. Hat ein Import Ihr Board ersetzt, siehe [Änderungen rückgängig machen](#änderungen-rückgängig-machen).
- **„SimonSays konnte Ihre Kategorien und Sätze nicht speichern“**: Ihre Änderung wird noch angezeigt, kann aber beim Schließen von SimonSays verloren gehen. Exportieren Sie Ihr Board sofort mit `F10`, um eine Kopie zu behalten; wenn es wieder vorkommt, melden Sie es bitte. Die Meldung erscheint nur einmal, nicht bei jeder Änderung, damit sie die Nutzung des Boards nie behindert.
- **Import- oder Exportfehler**: Prüfen Sie Dateiberechtigungen und Speicherort; versuchen Sie es erneut.
- **Änderungen an Sprache oder Stimme werden nicht übernommen**: Öffnen Sie die Einstellungen erneut und bestätigen Sie die Auswahl. Wenn die Änderungen weiterhin nicht greifen, starten Sie SimonSays neu und versuchen Sie es erneut.
- **Probleme mit dem Eyetracker melden**: Wenn der Support Sie um Diagnosedaten bittet, öffnen Sie das Fenster Blick / Verweilklick (`F3`) und klicken Sie auf `Diagnosebericht speichern...`. Der Bericht (`hid_dump.txt`) wird in `%LocalAppData%\SimonSays\debug\` gespeichert, und dieser Ordner öffnet sich. Er wird nur erstellt, wenn Sie ihn anfordern.

## Feedback, Support und Datenschutz
- Der Dialog `Über` (Infobereichsmenü) zeigt Version, Beschreibung und Copyright.
- Teilen Sie Vorschläge, Probleme oder Ihre Erfahrungen mit SimonSays und Ihrem Eyetracker über die Option `Feedback geben` im Infobereichsmenü mit, die ein [Feedback-Formular](https://forms.gle/KMbpEDYmwnFJUhgy8) öffnet.
- Für Fehlerberichte und technischen Support besuchen Sie das [SimonSays-Repository auf GitHub](https://github.com/juan-rey/SimonSays).
- SimonSays ist frei und quelloffen unter der Open Software License 3.0 (OSL-3.0). Es zeigt keine Werbung und sammelt keine Daten.
