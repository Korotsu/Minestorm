Features :

-Un joueur qui peut se deplacer , tirer et se teleporter avec les touches demanders + des touches bonus pour une meilleur aisance.
-Une touche qui mets le jeu en pause et qui affiche le texte "PAUSE";
-Un ecran de titre aui permet de lancer le jeu avec f de quitter avec echap  et une image fait main;
-Un HUD aui affiche le score actuel du joueur et les controle dans le menu;
-le Vaisseaux ce deplace avec l'inertie suivant la loi de newton;
-Le tir du vaisseaux peut detruire un autre joueur, une mine, ou une fireball via la sat complete;
-Chaque ennemie detruit ajoute des points au score (score de taille dinamique);
-Le joueur meurt au contact d'un ennemie, d'un tir ou une fireball;
-Creation d'un systeme de generation de niveau procedurale de difficulter croissante;
-Si le joueur tue tous les ennemies a l'ecran le systeme de generation de niveau augemente le difficulter et relance un niveau;
-les ennemies arrive apres un delai d'apparition et leur point apparait et et generait aleatoirement, 1spawn =1 ennemie;
-si un ennmie apparait son point de spawn est detruit;
-pour commencer seule les grandes mines apparaissent;
-1 grande mine détruite -> 2 mines moyennes détruites -> 4 petites mines,
au total il y a 7 mines ennemies;
-il y as 4 types de mines :
-les normales aui se deplacent de facon rectiligne avec un vecteur creer aleatoirement;
-les fireballs mine qui quand elles meurent liberer une fireball dans la direction du joueur;
-la magnetic mine aui suot le joueur;
-et le fireball magnetic aui suit en plus de liberer une fireball a sa mort;
-ils y as trois tailles :
grandes moyennes et petites qui non pas la meme vitesse et le mem score;
-il y as les fireball qui on etet creer telle que demander;
-les bordures d'ecran permettent d'accerder de l'autre coter;
-les bullets on ete creer et gerer de la fcon demader;

Build & Run :

-1 telecharger touts les fichiers du repertoire git;
-2 les extraire dans un dossier quelconque mais respecter l'architecture des dossiers sous-dossier.
-3compiler en utilisant gcc et le makefie via les commandes make  tel que make run pour le lancer , make clean pour le nettoyer 
-4 profiter du jeu !;



Chemins d'cces :

Boites a outils : minestorm/src/my_maths.c && .h 
methodes d'applications systeme newtonien : minestorm/src/Game.c:void Player_movement(t_object* object, SDL_Event event, t_time* time)
{ ligne 547;

drift : meme fonction;

methdode de changement de referenciel : 617->655 Game.c;

SAT : 661->918 Game.c;

les methodes qui utilise le sat : Print_mines,print_player,player_fire_process;les methodes de creation des objects sont dans le game init pour suivre la creation commencer ligne 11 du Game.c;  

