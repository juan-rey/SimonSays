# Guide d'aide de SimonSays

## Présentation
SimonSays est un assistant de synthèse vocale (TTS) pour Windows créé par Juan Rey Saura. Il est né comme un outil pour les personnes atteintes de SLA et d'autres personnes qui ne peuvent plus parler, afin qu'elles puissent communiquer rapidement sans quitter ce qu'elles faisaient sur l'ordinateur. Avec le temps, il est apparu que la même conception fonctionne comme un outil général de CAA (communication alternative et améliorée) pour toute personne pour qui parler est difficile ou impossible ; par exemple à cause d'une dysarthrie, d'une laryngectomie, d'une apraxie de la parole, d'un AVC ou d'une lésion cérébrale. Il est optimisé pour parler vite, utiliser des bibliothèques de phrases et lire des sons au fil du texte. Il prend en charge plusieurs langues, des voix configurables et des fonctions pensées pour une communication efficace.

SimonSays utilise l'espace libre de la barre des tâches de Windows et donne un accès rapide à une dizaine de catégories de phrases fréquentes (et personnalisables) en quelques clics seulement. Comme il ne recouvre jamais vos autres fenêtres, vous pouvez rester dans votre navigateur, vos documents ou un appel vidéo et parler chaque fois que vous en avez besoin : au clavier, à la souris ou sans les mains, avec un oculomètre.

Cas d'utilisation :
- Utilisation intensive de plusieurs applications Windows sans perdre de vue la communication.
- Tableaux créés par des orthophonistes pour des personnes ayant un trouble de la parole.
- Réunions en ligne avec [Virtual CABLE](https://vb-audio.com/Cable/) (voir [Utiliser SimonSays en réunion en ligne](#utiliser-simonsays-en-réunion-en-ligne)).

SimonSays n'a pas vocation à remplacer des logiciels de communication complets comme Mind Express, TD Communicator, Grid 3 ou OptiKey, ni l'évaluation et l'accompagnement professionnels qui les accompagnent habituellement. Il fait un travail plus ciblé : parler rapidement depuis la barre des tâches pendant que vous continuez à travailler dans d'autres applications. Il peut fonctionner à côté de ces logiciels ; par exemple, OptiKey ou un autre logiciel de contrôle oculaire peut déplacer le curseur pendant que SimonSays dit vos phrases.

SimonSays est open source et gratuit (libre au sens de la liberté, pas seulement du prix), et il le restera : chacun peut l'utiliser, l'étudier, l'adapter et le partager. Il ne contient aucune publicité et ne collecte aucune donnée.

SimonSays approche de la version 1.0. Avant cette étape, il a besoin de personnes qui l'essaient au quotidien, avec leurs propres voix, tableaux et oculomètres : si quelque chose ne fonctionne pas, est peu pratique ou manque, dites-le-nous (voir [Commentaires, assistance et confidentialité](#commentaires-assistance-et-confidentialité)). Chaque retour compte.

## Sommaire
- [Premiers pas](#premiers-pas)
- [Fenêtre des catégories](#fenêtre-des-catégories)
- [Phrases par défaut](#phrases-par-défaut)
- [Raccourcis](#raccourcis)
- [Paramètres (F2)](#paramètres-f2)
- [Regard / Clic par fixation (F3)](#regard--clic-par-fixation-f3)
- [Utiliser SimonSays en réunion en ligne](#utiliser-simonsays-en-réunion-en-ligne)
- [Menu de la zone de notification](#menu-de-la-zone-de-notification)
- Modifier les tableaux (avancé) :
  - [Modifier les catégories et les phrases](#modifier-les-catégories-et-les-phrases) (et [quel éditeur utiliser](#quel-éditeur-utiliser))
  - [Mélanger voix et sons](#mélanger-voix-et-sons)
  - [Personnaliser l'apparence (styles du tableau et des catégories)](#personnaliser-lapparence-styles-du-tableau-et-des-catégories)
  - [Importer et exporter des catégories](#importer-et-exporter-des-catégories)
  - [Annuler des modifications](#annuler-des-modifications)
  - [Où SimonSays conserve vos fichiers](#où-simonsays-conserve-vos-fichiers)
- [Dépannage](#dépannage)
- [Commentaires, assistance et confidentialité](#commentaires-assistance-et-confidentialité)

## Premiers pas
1. Lancez SimonSays. Il apparaît dans un espace libre de la barre des tâches de Windows et ajoute une icône dans la zone de notification. La barre des tâches peut se trouver en bas, en haut, à gauche ou à droite de l'écran :
   - Quand les icônes de la barre des tâches sont centrées, SimonSays se place tout au début de la barre (à l'extrémité gauche, ou en haut sur une barre latérale). Quand elles sont alignées à gauche, il se place juste avant la zone de notification.
   - Sur une petite barre des tâches (fine), la barre de SimonSays est plus basse, avec une police plus petite et les deux boutons d'accès rapide côte à côte. Sur une barre des tâches à gauche ou à droite de l'écran, les commandes sont empilées les unes sous les autres.
   - La fenêtre des catégories s'ouvre à côté de la barre des tâches, et la flèche du bouton `Catégories` indique de quel côté elle s'ouvre.
   - Si vous déplacez ou redimensionnez la barre des tâches pendant que SimonSays fonctionne, il la suit en quelques secondes.
2. Pour parler, vous pouvez :
   - Cliquer dans la zone de texte de la barre (ou y fixer le regard) : une zone de saisie plus grande s'ouvre à côté de la barre des tâches. Tapez votre texte et appuyez sur `Entrée` pour le dire ; la zone se ferme et le focus passe sur `Lire>`, si bien que `Espace` le redit (ou l'arrête). `Échap` ferme la zone sans parler et vous ramène à la fenêtre que vous utilisiez ; votre texte reste dans la barre, où `Lire>` le redit.
   - Cliquer sur une ou plusieurs phrases enregistrées dans la fenêtre des catégories.
3. Pendant la lecture, le bouton affiche `>` pour indiquer qu'elle est en cours. Vous pouvez appuyer de nouveau dessus pour l'arrêter immédiatement.

![La barre de SimonSays dans la barre des tâches](img/bar.png)

À côté du bouton `Catégories`, deux petits **boutons d'accès rapide** donnent accès en un clic à deux choses que vous atteindriez sinon par un raccourci ou par le menu de la zone de notification :

- 📂 ouvre un tableau enregistré sur votre ordinateur ; c'est la même importation que `F9`, mais elle commence dans votre dossier de tableaux (`%LocalAppData%\SimonSays\boards\`), où sont conservés les tableaux exportés et téléchargés (voir [Importer et exporter des catégories](#importer-et-exporter-des-catégories)).
- ⚙️ ouvre les paramètres, comme `F2`.

Les deux peuvent être masqués avec `Afficher les boutons et le menu d'accès rapide` dans les paramètres (`F2`), ce qui laisse plus de place à la zone de saisie. Le même réglage désactive aussi le menu contextuel (clic droit) de la fenêtre des catégories.

Si SimonSays est masqué, cliquez sur son icône dans la zone de notification pour l'afficher à nouveau.

## Fenêtre des catégories
![La fenêtre des catégories avec les phrases par défaut](img/categories.png)

- Ouvrez-la avec le bouton `Catégories`. Redimensionnez-la, ou activez `Mémoriser la taille de la fenêtre des catégories` dans les paramètres pour conserver sa taille.
- Sélectionnez une catégorie dans la liste du haut et des phrases dans la liste du bas.
- Un clic sur une phrase la dit automatiquement si `Dire immédiatement la phrase lors de la sélection` est activé ; sinon, la phrase est chargée dans la zone de saisie principale sans être dite.
- Double-cliquez sur une zone vide ou sur le bord de la fenêtre pour la redimensionner afin que toutes ses catégories et phrases y tiennent.
- Faites glisser une zone vide de la fenêtre pour la déplacer, et appuyez sur `Échap` pour la masquer.
- Zoomez et dézoomez avec `F12` et `F11` (voir [Raccourcis](#raccourcis)).
- Cliquez avec le bouton droit sur un bouton ou une zone vide (ou appuyez sur `Maj + F10` ou sur la touche Menu) pour ouvrir un menu avec `Coller`, `Zoom arrière`, `Zoom avant`, `Réinitialiser le zoom`, `Aide`, `Paramètres`, `Regard / Clic par fixation...`, `Masquer` et `Quitter SimonSays...`, chacun avec son raccourci. La première entrée affiche le titre du tableau (`Tableau sans titre` s'il n'en a pas) ; choisissez-la pour voir les crédits du tableau, sa langue et le nombre de catégories et de phrases qu'il contient. Le menu n'apparaît que lorsque `Afficher les boutons et le menu d'accès rapide` est activé dans les paramètres.
- Sur un bouton de catégorie ou de phrase, le menu propose en plus `Modifier...`, `Déplacer vers la gauche`, `Déplacer vers la droite`, `Ajouter une nouvelle catégorie...` ou `Ajouter une nouvelle phrase...` et `Supprimer la catégorie...` ou `Supprimer la phrase...` pour ce bouton, sans avoir à le sélectionner d'abord ; sur une catégorie, `Exporter la catégorie...` demande s'il faut enregistrer seulement cette catégorie ou tout le tableau. Les raccourcis `F4` à `F8` sont affichés lorsque le bouton est votre sélection actuelle, car ces touches agissent sur la sélection. Sur une zone vide, le menu propose `Ajouter une nouvelle catégorie...`, qui en ajoute une à la fin ; c'est aussi la façon de commencer un tableau vide.
- La bande entre les catégories et les phrases affiche un rappel des principaux raccourcis (ou la légende du tableau, s'il en a une).

![Le menu contextuel d'un bouton de phrase](img/menu.png)

## Phrases par défaut
SimonSays fournit un ensemble de catégories et de phrases prêt à l'emploi dans chacune de ses 18 langues, ordonné pour que les phrases les plus utiles demandent le moins de sélections :

- 👋🏻 Salutations Fréquentes
- 🗣️ Phrases de Conversation
- 🚨 Phrases d'Urgence
- 🤲 Soins & Confort
- 🏥 Santé
- 🏠 Famille & Maison
- 👥 Social
- ⏰ Temps & Date
- 💼 Phrases d'Affaires
- ✈️ Phrases de Voyage
- 🔊 Sons

Chaque langue a ses propres catégories et phrases : quand vous changez de langue dans les paramètres (`F2`), la fenêtre des catégories passe au tableau de cette langue, et les modifications faites dans une langue n'affectent pas les autres.

Les phrases sont dites avec les voix installées dans Windows. Si aucune voix n'est installée pour votre langue, les phrases s'affichent quand même, mais elles peuvent être dites avec une autre voix (voir [Dépannage](#dépannage)).

Vous pouvez modifier les phrases par défaut ou les récupérer plus tard ; voir [Modifier les catégories et les phrases](#modifier-les-catégories-et-les-phrases) et [Restaurer les phrases par défaut](#restaurer-les-phrases-par-défaut).

## Raccourcis
- `F1` : Ouvrir l'aide
- `F2` : Ouvrir les paramètres
- `F3` : Paramètres du regard / clic par fixation
- `Entrée` : Dire le texte de la zone de saisie
- `Échap` : Masquer la fenêtre des catégories
- `F11` / `Ctrl -` : Zoom arrière (fenêtre des catégories)
- `F12` / `Ctrl +` : Zoom avant (fenêtre des catégories)
- `Ctrl + 0` : Réinitialiser le zoom
- `Maj + F10` ou la touche Menu : Ouvrir le menu de la fenêtre des catégories (comme le clic droit)
- `Espace` sur `Lire>` : Redire le texte, ou l'arrêter pendant la lecture

Les raccourcis pour modifier les catégories, les phrases et les tableaux sont indiqués dans [Modifier les catégories et les phrases](#modifier-les-catégories-et-les-phrases).

## Paramètres (F2)
Ouvrez les paramètres (`F2`) pour régler les éléments suivants. Les paramètres comportent deux colonnes.

![La fenêtre des paramètres](img/settings.png)

**Colonne de gauche**
- **Texte par défaut** : activez ou désactivez `Insérer automatiquement le texte par défaut` et personnalisez la phrase par défaut.
  - `Afficher le clavier tactile` : ouvre le clavier tactile à l'écran lorsque la zone de saisie s'ouvre ; il est placé à côté de la zone de saisie, du côté opposé à la barre des tâches.
- `Afficher les boutons et le menu d'accès rapide` : affiche les boutons 📂 (ouvrir un tableau) et ⚙️ (paramètres) à côté du bouton `Catégories` de la fenêtre principale (voir [Premiers pas](#premiers-pas)) et le menu contextuel de la fenêtre des catégories (voir [Fenêtre des catégories](#fenêtre-des-catégories)). Désactivez-le pour tous les masquer et laisser plus de place à la zone de saisie.
- **Fenêtre des catégories** :
  - `Réduire automatiquement la fenêtre des catégories` après une sélection.
  - `Mémoriser la taille de la fenêtre des catégories`.
  - `Redimensionner automatiquement la fenêtre des catégories` : ajuste la fenêtre des catégories à ses catégories et phrases chaque fois qu'elles sont chargées ; par exemple au démarrage, après un changement de langue ou après une importation.
- `Regard / Clic par fixation...` : ouvre les paramètres de [Regard / Clic par fixation](#regard--clic-par-fixation-f3) par-dessus les paramètres, qui restent ouverts avec vos modifications. Les modifications que vous y faites sont enregistrées lorsque vous appuyez sur `OK` dans cette fenêtre, même si vous appuyez ensuite sur `Annuler` dans les paramètres.

**Colonne de droite**
- **Langue** : choisissez la langue de l'interface (English, Español, العربية, Euskara, Català, 简体中文, Français, Galego, Deutsch, עברית, हिन्दी, Italiano, 日本語, 한국어, Português, Português (Brasil), Русский, Valencià). Chaque langue a aussi ses propres catégories et phrases (voir [Phrases par défaut](#phrases-par-défaut)).
- **Voix** : choisissez une voix SAPI installée correspondant à votre langue.
- **Volume/Débit** : réglez `Volume (10-100)` et `Débit (-10 à 10)`.
- `Tester la voix` : écoutez la voix sélectionnée (sur le périphérique choisi dans `Sortie audio`).
- `Sortie audio` : là où SimonSays joue sa voix et ses sons : `Par défaut (Windows)` (le choix habituel) ou n'importe quel périphérique connecté. Les câbles audio virtuels sont listés en premier, ce dont vous avez besoin pour les [réunions en ligne](#utiliser-simonsays-en-réunion-en-ligne). Si le périphérique choisi n'est pas connecté, SimonSays joue sur le périphérique par défaut de Windows et vous prévient une fois ; le choix est conservé, affiché comme `(non connecté)`, et utilisé de nouveau dès que le périphérique revient.
- `Paramètres d'écoute...` : ouvre les périphériques d'enregistrement de Windows, où vous pouvez activer **Écouter ce périphérique** pour un câble virtuel (voir [Utiliser SimonSays en réunion en ligne](#utiliser-simonsays-en-réunion-en-ligne)).
- **Comportement de la lecture** :
  - `Dire immédiatement la phrase lors de la sélection` (dire automatiquement en un clic).
  - `Arrêter la lecture précédente` : arrête automatiquement toute lecture en cours avant de commencer une nouvelle phrase.
  - `Augmenter le volume de SimonSays pendant la lecture` : augmente le volume du périphérique sur lequel joue SimonSays (voir `Sortie audio`) pendant qu'il parle et rétablit le niveau précédent à la fin.
  - `Réduire temporairement les autres sons pendant la lecture` : baisse le volume des autres applications pendant qu'il parle et le rétablit à la fin.

**En bas**
- `Aide` (coin inférieur gauche, ou `F1` quand les paramètres sont ouverts) ouvre cette aide dans la langue sélectionnée dans la liste `Langue` ; les paramètres restent ouverts avec vos modifications.
- `OK` enregistre les modifications ; `Annuler` les abandonne.

Par défaut, `Dire immédiatement la phrase lors de la sélection`, `Réduire automatiquement la fenêtre des catégories`, `Redimensionner automatiquement la fenêtre des catégories`, `Augmenter le volume de SimonSays pendant la lecture` et `Réduire temporairement les autres sons pendant la lecture` sont activés, et `Arrêter la lecture précédente` est désactivé.

## Regard / Clic par fixation (F3)
Activez les phrases et les boutons sans les mains en maintenant votre regard (ou le curseur de la souris) dessus pendant une durée définie. Cette fonction est nouvelle : elle a besoin d'essais et de retours ; dites-nous comment elle fonctionne avec votre oculomètre grâce à l'option `Donnez votre avis` du menu de la zone de notification.

Configurations prises en charge :
- **Tout logiciel de contrôle oculaire qui déplace le curseur de la souris** (Irisbond EasyClick, Tobii Dynavox Computer Control, les modes curseur du Contrôle oculaire de Windows, OptiKey) : le clic par fixation suit le curseur.
- **Lecture directe du regard (sans déplacer le curseur)** : Irisbond Hiru (mode HID) et les oculomètres Tobii via leur propre logiciel ; vérifié avec le Tobii Eye Tracker 4C, le Tobii Eye Tracker 5 et le Tobii Dynavox PCEye 5.
- **Tobii Eye Tracker 5** : son logiciel actuel n'installe qu'un moteur 64 bits, SimonSays le lit donc par l'intermédiaire d'un petit programme auxiliaire, `SimonSaysGaze64.exe`, installé à côté de lui. L'auxiliaire ne démarre que lorsqu'il est nécessaire, n'a pas de fenêtre, se ferme avec SimonSays et garde le regard sur votre ordinateur : il n'est jamais enregistré ni envoyé nulle part.
- **Licence du Tobii Eye Tracker 4C et de l'Eye Tracker 5** : la licence utilisateur final de Tobii pour ces oculomètres n'autorise pas l'usage médical, y compris l'usage dans les hôpitaux et autres établissements de santé. Les appareils Tobii Dynavox, comme le PCEye 5, ont leurs propres conditions de licence.
- **Logiciel du Tobii Eye Tracker 4C** : SimonSays lit le 4C via Tobii Core. Si vous branchez un Eye Tracker 5 sur le même ordinateur, Windows installe Tobii Experience, qui désactive Tobii Core, et le 4C cesse de fonctionner. Pour utiliser à nouveau le 4C, réinstallez Tobii Core ; la version recommandée est `Tobii_Eye_Tracking_Core_v2.13.1.7294_x86.exe`, disponible sur la [page de démarrage de Tobii](https://gaming.tobii.com/getstarted/).

Où fonctionne le clic par fixation :
- La barre de SimonSays, la fenêtre des catégories et ses boutons de phrases et de catégories.
- Les boutons des fenêtres et questions de SimonSays : les paramètres, la fenêtre Regard / Clic par fixation, les boîtes de dialogue d'ajout, de modification et d'aperçu, et toutes les confirmations comme `Oui` / `Non` ou `OK` / `Annuler`. Ces boutons sont dans votre langue et commencent à réagir à la fixation environ trois quarts de seconde après l'ouverture de la fenêtre, afin qu'une question qui apparaît là où vous regardez ne puisse pas se répondre toute seule.
- Lorsque le regard est lu directement, le bouton que vous regardez s'éclaire comme sous la souris, vous voyez donc ce que vous allez sélectionner avant que la progression commence. Un clignement conserve la surbrillance ; détourner le regard l'efface.

![La fenêtre Regard / Clic par fixation](img/dwell.png)

Ouvrez les paramètres Regard / Clic par fixation (`F3`, le menu de la zone de notification ou le bouton `Regard / Clic par fixation...` des paramètres) pour régler :
- **Mode d'activation** : choisissez l'une des options suivantes :
  - `Automatique` : détecte les périphériques présents et active le clic par fixation avec le curseur de la souris ou la détection du regard, selon le cas. Le logiciel Tobii en arrière-plan (toujours actif une fois installé) ne suffit pas à activer la fixation au curseur ; un oculomètre Tobii n'est utilisé que lorsqu'il suit réellement le regard.
  - `Curseur de la souris` : active toujours le clic par fixation avec le curseur de la souris.
  - `Oculomètre HID` : active toujours le clic par fixation avec la détection du regard (Irisbond Hiru en mode HID, ou oculomètres Tobii). Seul votre regard active les boutons : tant qu'aucun regard n'arrive (suivi désactivé ou oculomètre débranché), rien n'est activé par fixation et la souris fonctionne normalement.
  - `Désactivé` : désactive toujours le clic par fixation.
  - **Délai d'accrochage** : combien de temps, en millisecondes, votre regard (ou le curseur) doit rester sur un bouton avant que la barre de progression commence (150 par défaut). Ainsi, parcourir les boutons du regard n'affiche aucune progression ; le bouton s'active après le délai d'accrochage plus le temps de fixation. Mettez-le à 0 pour que la progression commence immédiatement.
  - **Temps de fixation** : la durée (en millisecondes) nécessaire pour déclencher un clic en regardant un élément (500 par défaut). Un indicateur de progression s'affiche pendant la fixation.
  - **Rayon de tolérance** : la tolérance du regard (en pixels) qui autorise de petits mouvements de l'œil sans interrompre la fixation.
  - **Délai d'attente** : durée en millisecondes après une activation pendant laquelle le clic par fixation ne peut pas se déclencher (évite les clics répétés par accident).
  - **Couleur de progression** : la couleur de l'indicateur de progression de la fixation (par défaut, la couleur d'accentuation de Windows).
  - **S’éloigner avant qu’une fixation réactive** (activé par défaut) : après un clic par fixation, aucun bouton ne peut être réactivé par fixation tant que votre pointeur ou votre regard ne s'est pas éloigné de cet endroit (au-delà du rayon de tolérance), ou que vous n'avez pas détourné les yeux un instant. Rester sur un bouton ne l'active qu'une fois, si bien qu'une longue phrase n'est pas répétée avant d'être terminée. Les clics de souris ne sont pas concernés.
  - **Détecter comment vous utilisez le traceur** : deux boutons qui aident l'application à détecter si vous déplacez le curseur de la souris avec un oculomètre ou si vous cliquez simplement sur les boutons.
  - **Détecté** : affiche le type de périphérique détecté et d'autres informations sur les logiciels associés.
  - `Enregistrer le rapport de diagnostic...` (sous Détecté) : écrit un rapport sur votre oculomètre et son logiciel dans `hid_dump.txt` et ouvre son dossier, pour que vous puissiez l'envoyer à l'assistance.
  - Un bouton `Réinitialiser` (en bas à gauche) rétablit tous les réglages de fixation à leurs valeurs par défaut.
  - Un bouton `OK` enregistre les modifications ; `Annuler` les abandonne.

## Utiliser SimonSays en réunion en ligne
SimonSays peut parler directement dans un appel vidéo (Teams, Zoom, Google Meet…), de sorte que les autres participants entendent vos phrases comme si elles venaient de votre micro. Il vous faut un câble audio virtuel, comme le gratuit [VB-Audio Virtual Cable](https://vb-audio.com/Cable/) :

1. Installez le câble audio virtuel et redémarrez Windows si on vous le demande.
2. Envoyez le son de SimonSays vers le câble : ouvrez les paramètres (`F2`) et choisissez `CABLE Input` dans `Sortie audio` (les câbles virtuels sont listés en premier). Appuyez sur `Tester la voix` pour vérifier : vous ne devriez plus l'entendre dans vos haut-parleurs, car le son part maintenant dans le câble.
3. Dans votre application de réunion, choisissez `CABLE Output` comme micro.
4. Pour entendre aussi SimonSays vous-même, appuyez sur `Paramètres d'écoute...` dans les paramètres. Les périphériques d'enregistrement de Windows s'ouvrent : sélectionnez `CABLE Output`, ouvrez **Propriétés → Écouter**, cochez **Écouter ce périphérique**, choisissez vos haut-parleurs ou votre casque dans **Lire sur ce périphérique**, puis appuyez sur `OK`. Vous n'avez à le faire qu'une fois.

SimonSays ne peut pas activer **Écouter ce périphérique** à votre place : c'est un réglage du côté enregistrement du câble que Windows ne vous laisse modifier que vous-même, c'est pourquoi `Paramètres d'écoute...` vous amène directement à la bonne fenêtre.

Bon à savoir :
- Laissez le périphérique de lecture par défaut de Windows sur vos haut-parleurs ou votre casque, pas sur `CABLE Input`. Si **Écouter ce périphérique** joue sur « Périphérique de lecture par défaut » alors que ce périphérique par défaut est le câble, le son revient dans le câble.
- Si la réunion entend vos phrases mais pas les effets sonores (applaudissements, une sonnette), la suppression du bruit de l'application de réunion les supprime ; réduisez ou désactivez la suppression du bruit dans l'application.
- Si le câble est désinstallé ou débranché, SimonSays joue sur le périphérique par défaut de Windows et vous prévient une fois ; il revient au câble dès qu'il est de nouveau disponible.
- Pour avoir des phrases prêtes pour un appel, ajoutez la catégorie **Modèle de réunion** : appuyez sur `F9` ou sur le bouton 📂 et ouvrez le tableau nommé dans votre langue (`SimonSays_Modèle_de_réunion_(Français).ssc` en français). Il contient des salutations, « Vous m'entendez ? », une courte explication indiquant que vous parlez avec un programme, oui / non, d'accord / pas d'accord, demander de répéter ou de parler plus lentement, et des formules d'au revoir. Modifiez-le pour ajouter des phrases adaptées à votre réunion.
- En alternative à l'étape 2, vous pouvez toujours diriger SimonSays avec le réglage par application de Windows (**Paramètres → Système → Son → Mélangeur de volume**), en laissant `Sortie audio` sur `Par défaut (Windows)`. SimonSays n'apparaît dans cette liste qu'après avoir joué quelque chose.

## Menu de la zone de notification
- `Afficher` / `Masquer` l'interface principale
- `Paramètres`
- `Regard / Clic par fixation...`
- `Web` ouvre la page web de SimonSays
- `À propos` affiche la version et les crédits
- `Donnez votre avis` ouvre un [formulaire de commentaires](https://forms.gle/KMbpEDYmwnFJUhgy8) dans le navigateur par défaut
- `Quitter` ferme SimonSays, après confirmation

Vous pouvez déplacer la fenêtre contextuelle de la zone de notification et les autres boîtes de dialogue en faisant glisser leur barre de titre ; redimensionnez la fenêtre des catégories en faisant glisser ses bords, et activez `Mémoriser la taille de la fenêtre des catégories` dans les paramètres pour conserver sa taille.

## Modifier les catégories et les phrases
Cette section et les suivantes s'adressent aux aidants, aux orthophonistes et aux utilisateurs avancés qui modifient les catégories, les phrases, les sons et l'apparence d'un tableau. Chaque modification est enregistrée immédiatement (voir [Annuler des modifications](#annuler-des-modifications)).

### Quel éditeur utiliser
- **Dans SimonSays** : les modifications du quotidien ; renommer, ajouter, déplacer ou supprimer des catégories et des phrases, leur donner un emoji, ajouter des sons, coller une liste de phrases ou ajuster une couleur. La suite de cette section explique comment.
- **Dans [SimonSays Web](https://github.com/juan-rey/simonsays-web)** : le gros travail sur les tableaux, bien plus facile là-bas : tableaux construits avec des pictogrammes de CAA (il peut chercher dans la bibliothèque de pictogrammes ARASAAC), images d'icônes personnalisées et tableaux stylés, avec couleurs, tailles et dispositions. Il est particulièrement recommandé pour concevoir un tableau **Fitzgerald Key**, où chaque groupe de mots a sa propre couleur. C'est un éditeur visuel qui ouvre et enregistre les tableaux `.ssc`/`.ssz` directement dans votre navigateur, sans les envoyer en ligne ; quand vous avez fini, importez le tableau dans SimonSays avec `F9` ou le bouton 📂. C'est aussi le moyen d'utiliser les tableaux SimonSays sur d'autres plateformes.

Raccourcis de modification (fenêtre des catégories) :
- `F4` : Modifier la sélection actuelle
- `F5` / `F6` : Aller à la sélection précédente / suivante dans les listes
- `F7` : Ajouter après la sélection actuelle (catégorie ou phrase, selon le contexte)
- `F8` : Supprimer la sélection actuelle
- `F9` : Importer des catégories
- `F10` : Exporter des catégories
- `Ctrl + F4` : Modifier le style du tableau (voir [Personnaliser l'apparence](#personnaliser-lapparence-styles-du-tableau-et-des-catégories))
- `Ctrl + F8` : Supprimer toutes les catégories et leurs phrases (demande deux confirmations)
- `Ctrl + V` ou `Maj + Inser` : Ajouter le texte copié comme phrases, une par paragraphe (voir [Gérer les phrases](#gérer-les-phrases))
- Faire glisser un fichier `.txt`, `.ssc` ou `.ssz` sur la fenêtre : ajoute un `.txt` comme nouvelle catégorie, ou importe un tableau comme avec `F9` (voir [Gérer les catégories](#gérer-les-catégories))

> **Astuce :** exportez votre tableau (`F10`) avant d'utiliser `Ctrl + F8` ou d'importer un tableau complet (`F9`), afin de pouvoir le récupérer.

### Gérer les catégories
- Renommer : sélectionnez la catégorie → (raccourci : `F4`) `Changer le nom de la catégorie` → modifiez le nom → `OK`.
- Déplacer la sélection : `F5`/`F6` pour aller à l'élément précédent/suivant en parcourant les listes.
- Ajouter : sélectionnez une catégorie → (raccourci : `F7`) boîte de dialogue `Ajouter une nouvelle catégorie` → saisissez le nom → `OK`.
- Supprimer : sélectionnez la catégorie → (raccourci : `F8`) `Supprimer la catégorie` ; confirmez la suppression de la catégorie et de ses phrases.
- Ajouter depuis un fichier texte : faites glisser un fichier `.txt` sur la fenêtre des catégories → une catégorie portant le nom du fichier est créée, avec une phrase par paragraphe (les paragraphes sont séparés par une ligne vide) → vérifiez l'aperçu → `OK`. Vous pouvez déposer plusieurs fichiers à la fois ; chacun a son propre aperçu. Un fichier dont le nom correspond à une catégorie existante est refusé.
- Ouvrir un tableau en le faisant glisser : faites glisser un fichier `.ssc` ou `.ssz` sur la fenêtre des catégories → SimonSays demande s'il faut l'importer ; avec `Oui`, il est importé comme avec `F9` (voir [Importer et exporter des catégories](#importer-et-exporter-des-catégories)). Si le fichier contient un tableau complet, il remplace le tableau actuel, et une copie du tableau actuel est d'abord enregistrée sous `backup.ssz`. Vous pouvez déposer ensemble des fichiers texte et des fichiers de tableau ; chacun est traité à son tour. Ouvrir un fichier `.ssc` ou `.ssz` depuis l'Explorateur de fichiers (double-clic) pose la même question.

#### Définir ou changer l'icône d'une catégorie
Chaque catégorie peut afficher un emoji ou un fichier image personnalisé (`.ico`, `.png` ou `.jpg`) sur son bouton. Indiquez l'icône dans la boîte de dialogue d'ajout ou de modification avec le préfixe séparateur `##` :

```
<icône>##<nom de la catégorie>
```

- **Emoji** : saisissez le ou les emojis, suivis de `##` et du nom.
  - Exemple : `🙂##Salutations`
- **Fichier d'icône** (`.ico`, `.png`, `.jpg`) : saisissez le nom du fichier ou le chemin complet, suivi de `##` et du nom. Un nom de fichier sans dossier est recherché comme décrit dans [Ordre de recherche des fichiers de son et d'icône](#ordre-de-recherche-des-fichiers-de-son-et-dicône).
  - Exemple : `C:\icons\smile.png##Salutations`
- **Sans icône** : saisissez seulement le nom de la catégorie, sans préfixe `##`.
  - Exemple : `Salutations`

> Remarque : seuls les fichiers `.ico`, `.png` et `.jpg` sont pris en charge pour les icônes de fichier. Les autres extensions sont ignorées et aucune icône n'est affichée. Les images PNG conservent leur transparence ; les très grandes images sont automatiquement réduites au chargement.

### Gérer les phrases
- Modifier : sélectionnez la phrase → (raccourci : `F4`) `Modifier la phrase` → mettez à jour le texte → `OK`.
- Déplacer la sélection : `F5`/`F6` pour aller à la phrase précédente/suivante.
- Ajouter : sélectionnez une phrase → (raccourci : `F7`) `Ajouter une nouvelle phrase` → saisissez le texte → `OK`.
- Supprimer : sélectionnez la phrase → (raccourci : `F8`) `Supprimer la phrase` ; confirmez.
- En ajouter plusieurs d'un coup : copiez un texte dont les phrases sont séparées par une ligne vide → sélectionnez une phrase (ou une catégorie) → `Ctrl + V` → vérifiez l'aperçu → `OK`. Les phrases sont ajoutées après la phrase sélectionnée, ou à la fin de la catégorie. Les retours à la ligne à l'intérieur d'un paragraphe deviennent des espaces, et les marqueurs d'icône `##` et d'audio `::` fonctionnent comme dans la boîte de dialogue `Ajouter une nouvelle phrase`.
- Ajouter des sons : faites glisser un ou plusieurs fichiers `.wav` ou `.mp3` sur la fenêtre des catégories → vérifiez l'aperçu → `OK`. Chaque fichier devient une phrase sonore portant le nom du fichier, ajoutée après la phrase sélectionnée (ou à la fin de la catégorie sélectionnée), et le fichier est copié dans le dossier de ressources du tableau pour l'accompagner lorsque vous l'exportez. Un fichier existant n'est jamais écrasé : si un son différent portant le même nom existe déjà, le nouveau est enregistré sous `nom (2).wav`.

#### Définir ou changer l'icône d'une phrase
Les phrases suivent la même convention de préfixe d'icône que les catégories, avec le séparateur facultatif `##` placé avant le texte de la phrase :

```
<icône>##<texte de la phrase>
```

- **Emoji** : `👍##Ça me va !`
- **Fichier d'icône** : `C:\icons\check.png##Ça me va !`
- **Sans icône** : `Ça me va !`

Les phrases peuvent inclure un fichier audio (voir [Mélanger voix et sons](#mélanger-voix-et-sons)). Pour combiner icône, texte et audio, utilisez le format :

```
<icône>##<texte de la phrase>::<fichier audio>
```

- Exemple : `🔔##Attention::alert.mp3`

Le préfixe `##` et le suffixe `::` sont facultatifs et indépendants ; vous pouvez utiliser n'importe quelle combinaison :
- Préfixe d'icône seul : `🔔##Attention`
- Suffixe audio seul : `Attention::alert.mp3`
- Les deux : `🔔##Attention::alert.mp3`
- Aucun : `Attention`

## Mélanger voix et sons
- Utilisez le délimiteur `♫` pour insérer des noms de fichiers audio dans le texte :
  - Exemple : `Bonjour ♫doorbell.mp3♫, patientez un instant.`
  - Le texte hors des délimiteurs est dit ; le texte à l'intérieur est traité comme un fichier son et joué à cet endroit.
- Formats audio pris en charge : `.wav` et `.mp3`. Les fichiers MIDI (`.mid`, `.midi`) sont acceptés dans une phrase mais ne sont pas joués ; convertissez-les en `.wav` ou `.mp3`.
- Utilisez des chemins absolus ou relatifs accessibles au programme. Veillez à ce que les délimiteurs aillent par paires, afin que le texte final ne soit pas traité comme de l'audio.
- Pour taper `♫`, copiez-le depuis cette aide, ou sélectionnez une phrase qui contient un son : son marqueur `♫…♫` est placé dans la zone de saisie, où vous pouvez ajouter du texte autour.

### Sons fournis avec SimonSays
Ces sons sont installés avec SimonSays et peuvent être utilisés par leur nom dans n'importe quel texte ou phrase ; par exemple `♫laugh.mp3♫` dans la zone de saisie, ou `Bravo !::applause.mp3` pour un bouton de phrase intitulé « Bravo ! » qui joue des applaudissements :

`ahem.mp3`, `alert.mp3`, `applause.mp3`, `aww.mp3`, `beep.mp3`, `boo.mp3`, `call_bell.mp3`, `censor.mp3`, `doorbell.mp3`, `drumroll.mp3`, `evil_laugh.mp3`, `fail.mp3`, `hmmmm.mp3`, `kisses.mp3`, `laugh.mp3`, `mmm-hmm.mp3`, `nuh-uh.mp3`, `phone_ring.mp3`, `rimshot.mp3`, `scream.mp3`, `shots.mp3`, `sigh.mp3`

### Ordre de recherche des fichiers de son et d'icône
Quand le nom d'un fichier de son ou d'icône ne contient ni lettre de lecteur ni chemin complet, SimonSays le cherche dans l'ordre suivant et utilise la première correspondance :

1. **Le sous-dossier de ressources du tableau actuel** : si le style du tableau définit `title` ou `resource-folder`, son propre sous-dossier dans le dossier des ressources (voir [Personnaliser l'apparence](#personnaliser-lapparence-styles-du-tableau-et-des-catégories)).
2. **Dossier des ressources** : `%LocalAppData%\SimonSays\resources\` (l'emplacement par défaut des icônes et des sons des tableaux ; les paquets `.ssz` importés y installent leurs fichiers).
3. **Dossier de données de SimonSays** : `%LocalAppData%\SimonSays\` (conservé comme solution de repli pour les fichiers que des versions antérieures y plaçaient, avant l'existence du dossier des ressources).
4. **Répertoire de travail** : le répertoire de travail actuel du processus (recherché seulement s'il est différent du dossier de l'exécutable).
5. **Dossier de l'exécutable** : le dossier qui contient `SimonSays.exe`.

Si le fichier n'est trouvé dans aucun de ces emplacements, le son de secours intégré est utilisé (une icône n'est simplement pas affichée).

> **Astuce :** nous vous recommandons de placer vos fichiers de son et d'icône dans `%LocalAppData%\SimonSays\resources\` ; ils survivent aux mises à jour de l'application et sont propres à chaque utilisateur. Un tableau avec un style `title` ou `resource-folder` y obtient automatiquement son propre sous-dossier, ce qui sépare ses fichiers de ceux des autres tableaux.

## Personnaliser l'apparence (styles du tableau et des catégories)
Vous pouvez changer les couleurs, la taille et le style de la fenêtre des catégories et de ses boutons ; par exemple pour créer des tableaux de communication codés par couleurs comme Fitzgerald Key. Les styles sont facultatifs : sans eux, la fenêtre garde son apparence par défaut.

> **Astuce :** tout ce qui suit peut être saisi dans SimonSays, ce qui est pratique pour de petites retouches. Pour concevoir un tableau entier stylé ou à pictogrammes, [SimonSays Web](https://github.com/juan-rey/simonsays-web) est recommandé : vous voyez le résultat au fur et à mesure et il écrit ces styles pour vous (voir [Quel éditeur utiliser](#quel-éditeur-utiliser)).

Un style est une liste de paires `propriété:valeur;`. Les valeurs peuvent être :
- **Couleurs** : hexadécimal `#RRGGBB`, par exemple `#FFD966`.
- **Tailles** : un nombre de pixels (`96`) ou un pourcentage de la taille par défaut (`120%`).
- **Mots-clés** : par exemple `left`, `right`, `top`, `bottom`, `smart`, `normal`, `bold`.
- **Texte** : un nom de police, ou le texte de la légende, du titre ou des crédits.

Tout ce que l'application ne reconnaît pas (une propriété mal orthographiée ou une valeur incorrecte) est simplement ignoré, si bien qu'un style ne casse jamais la fenêtre.

### Styliser une catégorie (boîte de dialogue Modifier)
Sélectionnez une catégorie et appuyez sur `F4` (Modifier), puis ajoutez `::` et un style après le nom :

```
<icône>##<nom de la catégorie>::<propriété>:<valeur>;<propriété>:<valeur>;
```

- Exemple : `🧍##Personnes::background:#FFD966;` colore le bouton Personnes en jaune. La couleur de la catégorie **s'applique aussi aux phrases qu'elle contient**, si bien qu'une seule ligne colore tout un groupe de mots : la base d'un tableau Fitzgerald Key.
- Un style de catégorie peut définir `background` et `text-color` pour le bouton de la catégorie, et toute propriété de phrase (voir ci-dessous) préfixée par `phrase-`, par exemple `phrase-corner-radius:8;`.

### Styliser tout le tableau (`$$board`)
L'apparence générale (le fond de la fenêtre, les séparateurs et les valeurs par défaut de tous les boutons de catégorie et de phrase) est un **style de tableau**. Il est enregistré sous le nom de catégorie réservé `$$board` (vous ne pouvez pas utiliser de noms en `$$` pour vos propres catégories).

- **Modifiez-le** en appuyant sur `Ctrl + F4` dans la fenêtre des catégories. L'éditeur affiche un `propriété:valeur;` par ligne. Quand vous validez avec `OK`, le nouveau style est appliqué et SimonSays demande « Voulez-vous annuler les modifications ? » : choisissez `OK` pour revenir au style précédent, ou `Annuler` pour garder le nouveau.
- **Obtenez-en un** en important un fichier de catégories stylé (`.ssc` / `.ssz`). Quand vous importez un fichier qui contient un style de tableau, SimonSays demande avant de remplacer le vôtre et affiche le `title` et les `credits` du tableau (s'il en a) dans le message d'importation. Un fichier dont le style de tableau ne définit que `title`, `credits`, `language` ou `resource-folder` ne change rien de visible, si bien que SimonSays ne pose pas de question : s'il contient une seule catégorie, elle est ajoutée et votre apparence est conservée ; s'il contient un tableau complet, il remplace votre tableau, apparence comprise, comme tout tableau complet.

### Propriétés de style
- **Toute la fenêtre** : `background`, `separator-color`, `text-color`, `font-name`, `font-size`, `text-weight`, `caption` (légende affichée entre les séparateurs), `title` et `credits` (affichés à l'importation du tableau), `language` (la langue pour laquelle le tableau a été conçu, par exemple `language:French` ; SimonSays refuse d'importer le tableau tant que la langue configurée est différente, et vous indique vers quelle langue passer dans les paramètres, `F2`).
- **Tous les boutons de catégorie** (préfixe `category-`) et **tous les boutons de phrase** (préfixe `phrase-`) : `background`, `text-color`, `width`, `height`, `corner-radius`, `border-width`, `margin`, `icon-position` (`left`/`right`/`top`/`bottom`/`center`), `icon-size`, `font-name`, `font-size`, `text-weight`, `text-layout`.

Avec `icon-position` sur `center`, l'icône est placée au milieu du bouton et le texte est masqué (une touche avec icône seule) ; utilisez `top` si vous voulez l'icône en haut et le nom en dessous. Si vous voulez du texte par-dessus une icône centrée, ajoutez un `text-layout` explicite (par exemple `text-layout:bottom`) et il sera affiché à cette position.

`text-layout` accepte un mot-clé horizontal (`left`/`center`/`right`) et/ou un mot-clé vertical (`top`/`middle`/`bottom`), ou `smart`. La valeur par défaut est smart : le texte est centré dans l'espace laissé libre par l'icône (icône en haut → légende centrée en dessous ; centré dans tout le bouton quand il n'y a pas d'icône).

### Exemple : un tableau Fitzgerald Key codé par couleurs
Pour colorer six groupes de mots, modifiez chaque catégorie (`F4`) et donnez-lui une couleur de fond. Voici les chaînes exactes à saisir dans la boîte de dialogue de modification :

```
🧍##Personnes::background:#FFD966;
🏃##Actions::background:#93C47D;
📏##Descriptifs::background:#6FABDC;
🍎##Noms::background:#F6B26B;
🕒##Divers::background:#FFFFFF;
👋##Social::background:#B4A7D6;
```

Chaque couleur colore aussi les phrases de sa catégorie, si bien que tout le tableau est codé par couleurs. Les retouches de tout le tableau (comme placer l'emoji de chaque catégorie en haut avec son nom en dessous, et colorer les séparateurs) viennent d'un style de tableau (`Ctrl + F4`) comme :

```
category-icon-position:top;separator-color:#B4A7D6;
```

Pour un tableau Fitzgerald Key complet, avec des pictogrammes sur chaque touche et les phrases organisées par groupes de mots, créez-le dans [SimonSays Web](https://github.com/juan-rey/simonsays-web) et importez-le avec `F9`.

Les styles accompagnent vos catégories : l'exportation (`F10`) les inclut et l'importation (`F9`) les apporte, si bien qu'un tableau stylé peut être partagé en un seul fichier.

## Importer et exporter des catégories
Les tableaux sont partagés sous forme de fichiers `.ssc` (catégories et phrases) ou `.ssz` (la même chose, plus les fichiers de son et d'icône qu'ils utilisent, en un seul fichier). Vous pouvez aussi les créer et les modifier visuellement dans votre navigateur avec [SimonSays Web](https://github.com/juan-rey/simonsays-web).

- **Exporter** (`F10`) : à la question posée, choisissez d'exporter toutes les catégories ou seulement celle sélectionnée ; la boîte de dialogue d'enregistrement s'ouvre par défaut dans `%LocalAppData%\SimonSays\boards\`, mais vous pouvez enregistrer n'importe où. SimonSays enregistre automatiquement un `.ssz` lorsque vos catégories ou phrases utilisent des fichiers de son ou d'icône qu'il peut trouver (les sons `.mid`/`.midi` ne sont pas inclus). Exporter toutes les catégories inclut le style du tableau ; exporter une catégorie n'inclut que le style de cette catégorie. Les messages de réussite ou d'échec s'affichent dans la langue sélectionnée.
- **Importer** (`F9`) : choisissez un fichier `.ssc` ou `.ssz`.
  - Un fichier avec **plusieurs catégories ou un style de tableau** est un tableau complet : il **remplace toutes les catégories et phrases de la langue actuelle**. Votre tableau précédent est d'abord enregistré sous `backup.ssz` (voir [Annuler des modifications](#annuler-des-modifications)).
  - Un fichier avec **une seule catégorie** est ajouté à votre tableau ; si une catégorie du même nom existe, on vous demande s'il faut l'écraser. C'est aussi le cas lorsque le fichier se contente de nommer son tableau (`title`, `credits` ou `language`, sans couleurs, tailles ni polices) : ces informations ne remplacent jamais votre tableau ni son apparence.
  - Les fichiers de son et d'icône d'un `.ssz` sont installés dans le dossier des ressources, et la fenêtre des catégories s'ajuste au tableau importé. Les messages de réussite ou d'échec s'affichent dans la langue sélectionnée.

## Annuler des modifications
SimonSays enregistre chaque modification immédiatement et n'a pas de commande Annuler générale. Voici les moyens de revenir en arrière :

- **Annuler une importation** : avant qu'une importation remplace votre tableau, SimonSays enregistre le tableau précédent sous `backup.ssz` dans `%LocalAppData%\SimonSays\boards\`. Pour annuler l'importation, appuyez sur `F9` et choisissez `backup.ssz` : votre tableau précédent revient tel qu'il était, catégories et apparence, sans autre question. Seul le dernier tableau remplacé est conservé : chaque nouvelle importation d'un tableau complet écrase la copie.
- **Annuler une modification du style du tableau** : après avoir modifié le style du tableau avec `Ctrl + F4`, SimonSays demande « Voulez-vous annuler les modifications ? » ; choisissez `OK` pour revenir au style précédent.
- **Annuler une modification de catégorie ou de phrase** : les modifications faites avec `F4`, `F7` ou `F8` ne peuvent pas être annulées automatiquement. Avant de faire de grosses modifications sur un tableau auquel vous tenez, exportez-le (`F10`) ; importer ce fichier plus tard (`F9`) vous ramène à ce point.

### Restaurer les phrases par défaut
SimonSays n'écrase jamais vos propres catégories et phrases, si bien que les nouvelles phrases par défaut d'une mise à jour n'apparaissent que dans les langues que vous n'avez pas encore utilisées. Vous pouvez revenir aux catégories et phrases intégrées à tout moment ; par exemple pour repartir de zéro, ou pour récupérer les nouvelles phrases d'une mise à jour. Cela **remplace tout le tableau de la langue actuelle** :

1. Cliquez sur le bouton 📂 à côté de `Catégories` (ou appuyez sur `F9` dans la fenêtre des catégories). Le dossier des tableaux s'ouvre.
2. Choisissez le tableau des phrases par défaut de votre langue ; en français, `SimonSays_phrases_par_défaut_(Français).ssc` (dans les autres langues, il a la même forme, dans cette langue ; par exemple `SimonSays_default_phrases_(English).ssc`).
3. Confirmez l'importation. Votre tableau actuel est d'abord enregistré sous `backup.ssz`, vous pouvez donc encore le récupérer (voir [Annuler des modifications](#annuler-des-modifications)).

Ce tableau contient toujours les phrases par défaut de la version de SimonSays que vous utilisez : SimonSays le met à jour dans le dossier des tableaux au démarrage et lors d'un changement de langue. Si vous voulez garder votre propre tableau, exportez-le d'abord (`F10`) sous le nom de votre choix ; `backup.ssz` est écrasé par l'importation suivante.

Si le fichier est absent du dossier des tableaux, il reste un dernier recours : quittez SimonSays (menu de la zone de notification → `Quitter`), ouvrez l'Éditeur du Registre (`regedit`), supprimez la clé portant le nom anglais de votre langue (par exemple `French`) sous `HKEY_CURRENT_USER\SOFTWARE\SimonSays\Phrases\`, puis relancez SimonSays. Les catégories et phrases par défaut de cette langue sont alors recréées, sans sauvegarde.

Pour `Portuguese (Brazil)` : si une clé `Portuguese` existe aussi, SimonSays copie ce tableau dans `Portuguese (Brazil)` au lieu de créer les phrases par défaut brésiliennes ; supprimez donc aussi la clé `Portuguese` (exportez-la d'abord si vous l'utilisez).

## Où SimonSays conserve vos fichiers
- **Les catégories, les phrases et les paramètres** sont enregistrés dans le registre Windows sous `HKEY_CURRENT_USER\SOFTWARE\SimonSays` (le tableau de chaque langue sous `Phrases\<Language>`).
- `%LocalAppData%\SimonSays\resources\` : vos fichiers de son et d'icône, et ceux installés depuis des tableaux `.ssz` ; un tableau dont le style définit `title` ou `resource-folder` y a son propre sous-dossier.
- `%LocalAppData%\SimonSays\boards\` : le dossier par défaut des tableaux exportés, la copie automatique `backup.ssz` et les tableaux fournis avec SimonSays : les phrases par défaut de chaque langue, plus des catégories prêtes à l'emploi que vous pouvez ajouter à votre tableau avec `F9` ou le bouton 📂 : Oui / Non, Douleur et corps, Épeler, Jeux, Questions, Soins quotidiens, un Modèle de réunion et un Test d'audition, nommés dans votre langue, ainsi que `Spelling_NATO`, `Alert_tones` et `Morse_code` pour toutes les langues. Chacune ajoute une catégorie sans remplacer votre tableau. Les tableaux de votre langue y sont copiés au démarrage et lorsque vous changez de langue dans les paramètres, chaque fois que le tableau par défaut de votre langue manque ou a changé, en remplaçant les copies du dossier ; enregistrez donc sous un nouveau nom les tableaux que vous modifiez.
- `%LocalAppData%\SimonSays\debug\` : les fichiers de diagnostic (comme `hid_dump.txt`) que l'assistance peut vous demander.

Pour ouvrir l'un de ces dossiers, collez son chemin (par exemple `%LocalAppData%\SimonSays`) dans la barre d'adresse de l'Explorateur de fichiers.

## Dépannage
- **L'initialisation de la voix échoue** : vérifiez que SAPI est installé et qu'une voix compatible est présente.
- **Pas de voix pour ma langue, ou phrases dites avec un autre accent** : SimonSays utilise les voix installées dans Windows. Ajoutez une voix pour votre langue dans les paramètres de Windows (**Heure et langue → Voix → Gérer les voix**), puis choisissez-la dans les paramètres de SimonSays (`F2`). Certaines voix « naturelles » récentes de Windows peuvent ne pas être disponibles pour les autres applications.
- **Aucun son n'est joué** : vérifiez le chemin et l'extension du fichier, ainsi que l'appariement des délimiteurs `♫`.
- **La barre des tâches ne se masque plus automatiquement quand SimonSays est ouvert** : c'est voulu. SimonSays se trouve dans la barre des tâches, il la garde donc visible tant qu'il fonctionne et réactive « Masquer automatiquement la barre des tâches » quand vous le quittez. Si SimonSays a été fermé de façon inattendue (par exemple lors d'une coupure de courant), le réglage est réactivé la prochaine fois que vous quittez SimonSays.
- **Le bouton Widgets recouvre la barre de SimonSays** (Windows 11) : désactivez Widgets dans **Paramètres → Personnalisation → Barre des tâches** de Windows. SimonSays propose d'ouvrir cette page une fois, la première fois qu'il détecte Widgets.
- **La fenêtre des catégories est trop petite ou trop grande** : double-cliquez sur une zone vide ou sur le bord de la fenêtre pour l'ajuster à son contenu, ou zoomez avec `F11` / `F12`.
- **Mes catégories ou mes phrases ont disparu** : vérifiez la langue dans les paramètres (`F2`) ; chaque langue a son propre tableau. Si une importation a remplacé votre tableau, voir [Annuler des modifications](#annuler-des-modifications).
- **« SimonSays n'a pas pu enregistrer vos catégories et vos phrases »** : votre modification est toujours affichée, mais elle risque d'être perdue à la fermeture de SimonSays. Exportez immédiatement votre tableau avec `F10` pour en garder une copie ; si cela se reproduit, signalez-le. Le message n'apparaît qu'une fois, pas à chaque modification, pour ne jamais gêner l'utilisation du tableau.
- **Erreurs d'importation ou d'exportation** : vérifiez les autorisations du fichier et l'emplacement sur le disque ; réessayez.
- **Les changements de langue ou de voix ne s'appliquent pas** : rouvrez les paramètres et confirmez la sélection. Si les changements ne s'appliquent toujours pas, redémarrez SimonSays et réessayez.
- **Signaler un problème d'oculomètre** : si l'assistance vous demande un diagnostic, ouvrez la fenêtre Regard / Clic par fixation (`F3`) et cliquez sur `Enregistrer le rapport de diagnostic...`. Le rapport (`hid_dump.txt`) est enregistré dans `%LocalAppData%\SimonSays\debug\` et ce dossier s'ouvre. Il n'est créé que lorsque vous le demandez.

## Commentaires, assistance et confidentialité
- La boîte de dialogue `À propos` (menu de la zone de notification) affiche la version, la description et le copyright.
- Partagez vos suggestions, vos problèmes ou la façon dont SimonSays fonctionne avec votre oculomètre grâce à l'option `Donnez votre avis` du menu de la zone de notification, qui ouvre un [formulaire de commentaires](https://forms.gle/KMbpEDYmwnFJUhgy8).
- Pour signaler des bugs et obtenir une assistance technique, consultez le [dépôt GitHub de SimonSays](https://github.com/juan-rey/SimonSays).
- SimonSays est libre et open source, sous Open Software License 3.0 (OSL-3.0). Il n'affiche aucune publicité et ne collecte pas vos données.
