# Guide d'aide SimonSays

## Vue d'ensemble
SimonSays est un assistant Windows de synthèse vocale (TTS), créé par Juan Rey Saura, optimisé pour la parole rapide, les bibliothèques de phrases et la lecture de sons intégrés. Il prend en charge plusieurs langues, des voix personnalisables et des fonctionnalités pratiques pour une communication efficace.

SimonSays utilise l'espace libre à gauche de la barre des tâches Windows, offrant un accès rapide à une douzaine de catégories de phrases fréquemment utilisées (et personnalisables) en quelques clics.

## Raccourcis
- `F1` : Ouvrir l'aide.
- `F2` : Ouvrir les paramètres.
- `F3` : Ajouter après la sélection actuelle (contexte catégorie/phrase).
- `F4` : Modifier la sélection actuelle.
- `F5` / `F6` : Déplacer vers la sélection précédente / suivante dans les listes.
- `F8` : Supprimer la sélection actuelle.
- `F9` : Importer des catégories.
- `F10` : Exporter des catégories.
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
- Ajouter : sélectionner catégorie → (raccourci : `F3`) dialogue `Ajouter une nouvelle catégorie` → saisir le nom → `OK`.
- Renommer : sélectionner catégorie → (raccourci : `F4`) `Changer le nom de la catégorie` → modifier le nom → `OK`.
- Déplacer la sélection : `F5`/`F6` pour précédent/suivant.
- Supprimer : sélectionner catégorie → (raccourci : `F8`) `Supprimer la catégorie` ; confirmer la suppression de la catégorie et de ses phrases.

### Gérer les phrases
- Ajouter : sélectionner phrase → (raccourci : `F3`) `Ajouter une nouvelle phrase` → saisir le texte → `OK`.
- Modifier : sélectionner phrase → (raccourci : `F4`) `Modifier la phrase` → mettre à jour le texte → `OK`.
- Déplacer la sélection : `F5`/`F6` pour précédent/suivant.
- Supprimer : sélectionner phrase → (raccourci : `F8`) `Supprimer la phrase` ; confirmer.

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

## Menu de l'icône de notification
- `Afficher` / `Masquer` l'interface principale.
- `Paramètres`.
- `À propos` affiche la version et les crédits.
- `Web` ouvre la page du projet (si configurée).
- `Feedback` ouvre un formulaire de commentaires dans le navigateur par défaut.
- `Quitter` ferme SimonSays.

Vous pouvez déplacer la fenêtre contextuelle de notification et les autres dialogues en faisant glisser la barre de titre ; redimensionnez la fenêtre Catégories en faisant glisser ses bords, et activez `Mémoriser la taille de la fenêtre des catégories` dans les Paramètres pour conserver la taille.

## Mélange parole et sons
- Utilisez le délimiteur `♫` (`SOUND_NOTE_DELIMITER`) pour insérer des noms de fichiers audio dans le texte :
  - Exemple : `Bonjour ♫notification.wav♫, veuillez patienter.`
  - Le texte en dehors des délimiteurs est prononcé ; le texte entre les délimiteurs est traité comme un fichier audio et joué en ligne.
- Formats audio pris en charge : `.wav`, `.mid`, `.midi`, `.mp3`.
- Utilisez des chemins absolus ou relatifs accessibles au processus. Appariez les délimiteurs pour éviter que le texte final ne soit traité comme audio.

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
