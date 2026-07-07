# Guide d'aide SimonSays

## Vue d'ensemble
SimonSays est un assistant Windows de synthèse vocale (TTS), créé par Juan Rey Saura, optimisé pour la parole rapide, les bibliothèques de phrases et la lecture de sons intégrés. Il prend en charge plusieurs langues, des voix personnalisables et des fonctionnalités pratiques pour une communication efficace.

SimonSays utilise l'espace libre à gauche de la barre des tâches Windows, offrant un accès rapide à une douzaine de catégories de phrases fréquemment utilisées (et personnalisables) en quelques clics.

## Raccourcis
- `F1` : Ouvrir l'aide.
- `F2` : Ouvrir les paramètres.
- `F3` : Paramètres Regard / Clic par fixation.
- `F4` : Modifier la sélection actuelle.
- `F5` / `F6` : Déplacer vers la sélection précédente / suivante dans les listes.
- `F7` : Ajouter après la sélection actuelle (contexte catégorie/phrase).
- `F8` : Supprimer la sélection actuelle.
- `F9` : Importer des catégories.
- `F10` : Exporter des catégories.
- `F11` / `Ctrl -` : Zoom arrière (fenêtre des catégories).
- `F12` / `Ctrl +` : Zoom avant (fenêtre des catégories).
- `Ctrl 0` : Réinitialiser le zoom.
- Bouton `Lire>` : Démarrer la lecture.

## Démarrage et utilisation de base
1. Lancez SimonSays ; il apparaît dans la zone de notification Windows.
2. Pour parler, vous pouvez :
   - Saisir un texte personnalisé dans la zone de saisie et appuyer sur le bouton `Lire>` (ou `Enter`).
   - Cliquer sur une ou plusieurs phrases enregistrées dans la fenêtre Catégories.
3. Pendant la lecture, le bouton affiche `>` pour indiquer la lecture active. Vous pouvez appuyer à nouveau pour arrêter instantanément.

## Fenêtre Catégories (phrases)
- Ouvrez via le bouton `Catégories`. Redimensionnement/mémorisation de la taille via Paramètres.
- Sélectionnez une catégorie en haut et les phrases en bas.
- Un clic sur une phrase lance la lecture immédiate si `Dire immédiatement la phrase lors de la sélection` est activé ; sinon la phrase est chargée dans la zone de saisie principale sans lecture.

### Gérer les catégories
- Renommer : sélectionner catégorie → (raccourci : `F4`) `Changer le nom de la catégorie` → modifier le nom → `OK`.
- Déplacer la sélection : `F5`/`F6` pour précédent/suivant.
- Ajouter : sélectionner catégorie → (raccourci : `F7`) dialogue `Ajouter une nouvelle catégorie` → saisir le nom → `OK`.
- Supprimer : sélectionner catégorie → (raccourci : `F8`) `Supprimer la catégorie` ; confirmer la suppression de la catégorie et de ses phrases.



#### Définir ou modifier l’icône d’une catégorie
Chaque catégorie peut afficher un emoji ou un fichier `.ico` personnalisé sur son bouton. L’icône se spécifie dans la boîte de dialogue Ajouter/Modifier à l’aide du préfixe séparateur `##` :

```
<icône>##<nom de catégorie>
```

- **Emoji** : saisissez le ou les emoji suivis de `##` puis du nom.
  - Exemple : `🙂##Salutations`
- **Fichier d’icône** (`.ico`) : saisissez le chemin du fichier (absolu ou relatif au dossier SimonSays) suivi de `##` puis du nom.
  - Exemple : `C:\icons\smile.ico##Salutations`
- **Sans icône** : saisissez simplement le nom de la catégorie sans préfixe `##`.
  - Exemple : `Salutations`

> Remarque : seuls les fichiers `.ico` sont pris en charge pour les icônes basées sur un fichier. Toute autre extension est ignorée et aucune icône n’est affichée.

### Gérer les phrases
- Modifier : sélectionner phrase → (raccourci : `F4`) `Modifier la phrase` → mettre à jour le texte → `OK`.
- Déplacer la sélection : `F5`/`F6` pour précédent/suivant.
- Ajouter : sélectionner phrase → (raccourci : `F7`) `Ajouter une nouvelle phrase` → saisir le texte → `OK`.
- Supprimer : sélectionner phrase → (raccourci : `F8`) `Supprimer la phrase` ; confirmer.



#### Définir ou modifier l’icône d’une phrase
Les phrases suivent la même convention de préfixe d’icône que les catégories, avec le séparateur optionnel `##` placé avant le texte de la phrase :

```
<icône>##<texte de la phrase>
```

- **Emoji** : `👍##Ça marche bien !`
- **Fichier d’icône** : `C:\icons\check.ico##Ça marche bien !`
- **Sans icône** : `Ça marche bien !`

Les phrases peuvent également inclure un fichier audio intégré. Lorsque vous combinez une icône, du texte et un fichier audio, le format complet est :

```
<icône>##<texte de la phrase>::<fichier audio>
```

- Exemple : `🔔##Attention::notification.wav`

Le préfixe `##` et le suffixe `::` sont optionnels et indépendants ; vous pouvez utiliser n’importe quelle combinaison :
- Préfixe d’icône uniquement : `🔔##Attention`
- Suffixe audio uniquement : `Attention::notification.wav`
- Les deux : `🔔##Attention::notification.wav`
- Aucun : `Attention`

## Paramètres (F2)
Ouvrez les paramètres (`F2`) pour contrôler :
- **Texte par défaut** : activer/désactiver `Insérer automatiquement le texte par défaut` et personnaliser la phrase par défaut.
  - `Afficher le clavier tactile` — ouvre le clavier tactile à l'écran lorsque la zone de saisie reçoit le focus, positionné près de la fenêtre principale.
- **Langue de l'interface** : choisir la langue de l'UI (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **Voix** : sélectionner une voix SAPI installée correspondant à votre préférence.
- **Volume/Débit** : ajuster `Volume (10-100)` et `Débit (-10 à 10)`.
- `Tester la voix` : écouter la voix sélectionnée.
- **Fenêtre des catégories** :
  - `Réduire automatiquement la fenêtre des catégories` après sélection.
  - `Mémoriser la taille de la fenêtre des catégories`.
- **Comportement de lecture** :
  - `Dire immédiatement la phrase lors de la sélection` (lecture automatique au clic).
  - `Arrêter la lecture précédente` — arrête automatiquement toute lecture en cours avant de démarrer une nouvelle phrase.
  - `Augmenter le volume de SimonSays pendant la lecture` — augmente le volume maître du système pendant la parole, le restaurant à la fin de la lecture.
  - `Réduire temporairement les autres sons pendant la lecture` — réduit le volume de toutes les autres applications pendant la parole, le restaurant à la fin de la lecture.
- `OK` enregistre les modifications ; `Annuler` les annule.

## Regard / Clic par fixation (F3)
Activez des phrases et des boutons sans les mains en maintenant votre regard (ou le curseur de la souris) dessus pendant une durée définie. Cette fonctionnalité est nouvelle — elle a besoin de tests et de retours : dites-nous comment elle fonctionne avec votre dispositif de suivi oculaire via l'option `Feedback` du menu de notification.

Configurations prises en charge :
- **Tout logiciel de contrôle oculaire qui déplace le curseur de la souris** (Irisbond EasyClick, Tobii Dynavox Computer Control, modes curseur du Contrôle visuel de Windows, OptiKey, ...) : le clic par fixation suit le curseur.
- **Lecture directe du regard, sans déplacement du curseur** : Irisbond Hiru (mode HID) et dispositifs Tobii via leur propre logiciel installé — vérifié avec le Tobii Eye Tracker 4C et le Tobii Dynavox PCEye 5 ; le Tobii Eye Tracker 5 devrait fonctionner, mais n'a pas encore été testé.

Ouvrez les paramètres Regard / Clic par fixation (`F3`) pour configurer (les contrôles de cette boîte de dialogue s'affichent en anglais) :
- **Mode d'activation** : choisissez entre `Automatic`, qui détecte la présence et le comportement du dispositif et décide d'activer le clic par fixation via le curseur de la souris ou via la détection du regard (si un dispositif de suivi oculaire ou un logiciel associé est détecté, le clic par fixation est activé ; sinon, il reste désactivé), `Mouse cursor` pour toujours activer le clic par fixation via le curseur, `HID eye tracker` pour toujours l'activer via la détection du regard, ou `Off` pour toujours le désactiver, quels que soient les dispositifs présents.
- **Temps de fixation** (`Dwell time`) : durée (en millisecondes) nécessaire pour déclencher un clic en regardant un élément. Un indicateur de progression s'affiche pendant la fixation.
- **Rayon de tolérance** (`Tolerance radius`) : tolérance du regard (en pixels) pour permettre de légers mouvements des yeux sans annuler le clic par fixation.
- **Délai de repos** (`Cooldown`) : millisecondes après une activation pendant lesquelles aucun autre clic par fixation ne sera déclenché (évite les clics répétés accidentels).
- **Couleur de progression** (`Progress color`) : couleur de l'indicateur de progression de la fixation (par défaut, la couleur d'accentuation de Windows).
- **Détecter comment vous utilisez le dispositif** (`Detect how you use the tracker`) : deux boutons qui aident l'application à détecter si vous déplacez le curseur avec un dispositif de suivi oculaire ou si vous cliquez simplement sur les boutons.
- **Détecté** (`Detected`) : affiche le type de dispositif détecté et des informations supplémentaires sur les logiciels associés.
- Un bouton `Reset` (en bas à gauche) restaure tous les paramètres de fixation à leurs valeurs par défaut.
- Le bouton `OK` enregistre les modifications ; `Annuler` les rejette.

## Menu de l'icône de notification
- `Afficher` / `Masquer` l'interface principale.
- `Paramètres`.
- `Gaze / Dwell-click` (Regard / Clic par fixation).
- `À propos` affiche la version et les crédits.
- `Web` ouvre la page du projet (si configurée).
- `Feedback` ouvre un [formulaire de commentaires](https://forms.gle/KMbpEDYmwnFJUhgy8) dans le navigateur par défaut.
- `Quitter` ferme SimonSays.

Vous pouvez déplacer la fenêtre contextuelle de notification et les autres dialogues en faisant glisser la barre de titre ; redimensionnez la fenêtre Catégories en faisant glisser ses bords, et activez `Mémoriser la taille de la fenêtre des catégories` dans les Paramètres pour conserver la taille.

## Mélange parole et sons
- Utilisez le délimiteur `♫` (`SOUND_NOTE_DELIMITER`) pour insérer des noms de fichiers audio dans le texte :
  - Exemple : `Bonjour ♫notification.wav♫, veuillez patienter.`
  - Le texte en dehors des délimiteurs est prononcé ; le texte entre les délimiteurs est traité comme un fichier audio et joué en ligne.
- Formats audio pris en charge : `.wav`, `.mid`, `.midi`, `.mp3`.
- Utilisez des chemins absolus ou relatifs accessibles au processus. Appariez les délimiteurs pour éviter que le texte final ne soit traité comme audio.



### Ordre de recherche des fichiers audio et d’icône
Lorsqu’un nom de fichier audio ou d’icône ne contient ni lettre de lecteur ni chemin complet, SimonSays le recherche dans l’ordre suivant et utilise la première correspondance trouvée :

1. **Dossier AppData** — `%APPDATA%\SimonSays\` (spécifique à l’utilisateur ; placez vos fichiers ici pour les séparer de l’installation de l’application).
2. **Répertoire de travail** — le répertoire de travail actuel du processus (uniquement s’il diffère du dossier de l’exécutable).
3. **Dossier de l’exécutable** — le dossier contenant `SimonSays.exe`.

Si le fichier n’est trouvé dans aucun de ces emplacements, le son de secours intégré est utilisé.

> **Conseil :** il est recommandé de placer vos fichiers son et icônes personnalisés dans `%APPDATA%\SimonSays\` ; ils survivront aux mises à jour et resteront propres à l’utilisateur.

## Importer et exporter des catégories
- Exporter : vous pouvez exporter toutes les catégories ou seulement celle sélectionnée lorsque cela est demandé. Les messages de succès/échec apparaissent dans la langue sélectionnée (raccourci : `F10`).
- Importer : choisir un fichier ; si la catégorie existe, on vous demandera de l'écraser. Les messages de succès/échec apparaissent dans la langue sélectionnée (raccourci : `F9`).

## Dépannage
- **Échec d'initialisation de la voix** : vérifiez que SAPI est installé et qu'une voix compatible est présente.
- **Pas de lecture de sons** : confirmez le chemin/l'extension et les délimiteurs `♫` appariés.
- **Erreurs d'importation/exportation** : vérifiez les permissions et l'emplacement du disque ; réessayez.
- **Langue de l'UI non appliquée** : rouvrez les Paramètres et confirmez la sélection de langue ; assurez-vous que les chaînes de localisation existent pour la langue choisie.

## À propos
- Le dialogue `À propos` affiche la version, la description et le copyright.
