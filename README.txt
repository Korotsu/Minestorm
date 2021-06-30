Features :

-Un joueur qui peut se déplacer, tirer et se téléporter avec les touches du clavier ;
-Une touche qui mets le jeu en pause et qui affiche le texte « PAUSE » ;
-Un écran de titre qui permet de lancer le jeu avec f de quitter avec « Echap » et une image fait main ;
-Un HUD qui affiche le score actuel du joueur et les contrôle dans le menu ;
-Le Vaisseaux se déplace avec l'inertie suivant la loi de Newton ;
-Le tir du vaisseau peut détruire un autre joueur, une mine, ou une fireball via la SAT (Separating Axis Theorem) complète ;
-Chaque ennemie détruit ajoute des points au score (score de taille dynamique) ;
-Le joueur meurt au contact d'un ennemie, d'un tir ou une fireball ;
-Création d'un système de génération de niveau procédurale de difficulté croissante ;
-Si le joueur tue toute l’ennemie à l'écran le système de génération de niveau augmente la difficulté et relance un niveau ;
-Les ennemies arrivent après un délai d'apparition et leur point apparait et est générerez aléatoirement, 1 spawn = 1 ennemie ;
-Si un ennemi apparait son point de spawn est détruit ;
-Pour commencer seule les grandes mines apparaissent ;
-1 grande mine détruite -> 2 mines moyennes détruites -> 4 petites mines,
Au total il y a 7 mines ennemies ;
-Il y as 4 types de mines :
-Les normales qui se déplacent de façon rectiligne avec un vecteur créer aléatoirement ;
-Les fireballs mine qui quand elles meurent libérées une fireball dans la direction du joueur ;
-La Magnetic mine qui suit le joueur ;
-Et le fireball Magnetic qui suit en plus de libérer une fireball à sa mort ;
-Ils y as trois tailles :
Grandes moyennes et petites qui non pas la même vitesse et le même score ;
-Les bordures d'écran permettent d'accéder de l'autre coter ;

Build & Run :

-1 Télécharger tous les fichiers du répertoire git ;
-2 Les extraire dans un dossier quelconque mais respecter l'architecture des dossiers sous-dossier.
-3 Lancer l'exectutable situer dans bin ou lancer la solution visual studio. 
-4 Profiter du jeu !
