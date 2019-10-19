# Subway
Programme de calculs d'itinéraires pour des stations de métro dans Paris. Réalisé en C, en deuxième année à l’Efrei Paris, en 3 mois en groupe de 6.

# Initialisation du programme
Le programme a été conçu pour représenter des stations de métro existantes à Paris. Les lignes 1, 4, 6, 7 et 9. Elles sont visibles sur la page d’accueil en cliquant sur ‘Lines’ pour connaître les noms de stations. 

Les calculs de temps sont basés sur des moyennes initialisées par le programme. Les trains seront plus abondant en heure de pointe (matin, midi et soir) mais le trajet prendra plus longtemps entre chaque arrêt. 

Des stations sont indiquées à l’ouverture du programme comme étant en travaux. Elles sont aléatoirement choisies et ne peuvent alors pas être desservies. 

![page d accueil]( https://github.com/CelineCornet/Subway/blob/master/images/01.png)

# Exécution du programme
Le programme permet de calculer l’itinéraire et le temps pour aller d’une station à une autre. On peut renseigner à la page d’accueil les deux stations, choisir le moyen le plus rapide ou avec le moins de correspondance ainsi que l’heure du départ.

Une fois configuré, on arrive à une fenêtre qui nous offre 1 à 3 choix selon les possibilités. 

![]( https://github.com/CelineCornet/Subway/blob/master/images/02.png)

Il suffit d’appuyer sur ‘détail’ pour avoir l’explication de l’itinéraire calculé

![]( https://github.com/CelineCornet/Subway/blob/master/images/03.png)

Cependant, il se peut qu’une station ne puisse pas être desservie dû aux travaux. Dans ce cas, il n’y a pas de possibilité et une fenêtre informe de ce cas.

![travaux]( https://github.com/CelineCornet/Subway/blob/master/images/04.png)

Une dernière possibilité est qu’il n’y a pas de solutions du tout. Et dans ce cas, le programme affiche l’image suivante

![pas de cas possible]( https://github.com/CelineCornet/Subway/blob/master/loney2.jpg)

