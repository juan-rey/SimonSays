# Guide d'aide SimonSays

## Vue d'ensemble
SimonSays est un assistant Windows de synthèse vocale (TTS), créé par Juan Rey Saura, optimisé pour la parole rapide, les bibliothèques de phrases et la lecture de sons intégrés. Il prend en charge plusieurs langues, des voix personnalisables et des fonctionnalités pratiques pour une communication efficace.

## Raccourcis
- `F1` : ouvrir l'aide.
- `F2` : ouvrir les paramètres.
- `F3` : ajouter après la sélection actuelle (catégorie/phrase).
- `F4` : modifier la sélection actuelle.
- `F5` / `F6` : déplacer vers la sélection précédente / suivante.
- `F8` : supprimer la sélection actuelle.
- `F9` : importer des catégories.
- `F10` : exporter des catégories.
- Bouton `Play>` : démarrer la lecture.

## Démarrage et utilisation de base
1. Lancez SimonSays ; il apparaît dans la zone de notification Windows.
2. Cliquez gauche sur l’icône de la zone de notification ou choisissez `Show` pour ouvrir la fenêtre principale.
3. Saisissez un texte personnalisé ou cliquez sur une phrase enregistrée dans la fenêtre Catégories.
4. Appuyez sur `Play>` pour parler.
5. Pendant la lecture, le bouton affiche `>`.

## Fenêtre Catégories (phrases)
- Ouvrez via le bouton `Categories`. Redimensionnement/mémorisation via Paramètres.
- Sélectionnez une catégorie en haut et les phrases en bas.
- Un clic sur une phrase lance la lecture immédiate si `Speak phrase immediately when selecting` est activé ; sinon la phrase est chargée sans lecture.

### Gérer les catégories
- Ajouter : sélectionner catégorie → (`F3`) `Add category` → nom → `OK`.
- Renommer : sélectionner catégorie → (`F4`) `Edit` → modifier → `OK`.
- Déplacer : `F5`/`F6` pour précédent/suivant.
- Supprimer : sélectionner catégorie → (`F8`) `Delete` ; confirmer.

### Gérer les phrases
- Ajouter : sélectionner phrase → (`F3`) `Add phrase` → texte → `OK`.
- Modifier : sélectionner phrase → (`F4`) `Edit` → mise à jour → `OK`.
- Déplacer : `F5`/`F6`.
- Supprimer : sélectionner phrase → (`F8`) `Delete` ; confirmer.

## Paramètres (F2)
Ouvrez les paramètres (`F2`) pour contrôler : texte par défaut, langue de l’interface, voix SAPI, volume/vitesse et comportement de lecture.
`Test Voice` permet d’écouter la voix. `OK` enregistre, `Cancel` annule.

## Menu de l’icône de notification
- `Show` / `Hide`
- `Settings`
- `About`
- `Web`
- `Exit`

Vous pouvez déplacer les fenêtres en glissant la barre de titre. Redimensionnez la fenêtre Catégories et activez `Remember category window size` pour conserver la taille.

## Mélange parole + sons
- Utilisez le délimiteur `♫` (`SOUND_NOTE_DELIMITER`) :
  - Exemple : `Hello ♫notification.wav♫, please wait.`
- En dehors des délimiteurs : texte parlé.
- Entre délimiteurs : fichier audio joué via `PlaySound`.
- Formats : `.wav`, `.mid`, `.midi`, `.mp3`.

## Importer et exporter des catégories
- Export : toutes les catégories ou seulement la sélection (`F10`).
- Import : choisir un fichier ; confirmation d’écrasement si nécessaire (`F9`).

## Stockage des valeurs par défaut
- Valeurs créées sous `HKCU\SOFTWARE\SimonSays\Phrases\<Language>` au premier lancement.
- Pour réinstaller : fermer SimonSays → supprimer la clé langue → relancer.

## Dépannage
- **Voix indisponible** : vérifier SAPI et voix installées.
- **Son non lu** : vérifier chemin/extension et délimiteurs `♫`.
- **Erreur import/export** : vérifier permissions et emplacement.

## À propos
- `About` affiche version, description et copyright.
