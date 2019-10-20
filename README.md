# Subway
Programme de calculs d'itinéraires pour des stations de métro dans Paris. Réalisé en C et avec GTK, en deuxième année à l’Efrei Paris, en 3 mois en groupe de 6.

# C et GTK
Pour développer le programme, nous avons utilisé Code::Blocks avec l'implémentation de GTK pour l'interface graphique.

# Initialisation du programme
Le programme a été conçu pour représenter une partie du réseau de métro de Paris, les lignes 1, 4, 6, 7 et 9. Elles sont visibles sur la page d’accueil en cliquant sur ‘Lines’ pour connaître les noms de stations. 

Les calculs de temps sont basés sur des moyennes initialisées par le programme, les trains seront plus abondant en heure de pointe (matin, midi et soir) mais le trajet prendra plus longtemps entre chaque arrêt. 

Des stations sont indiquées à l’ouverture du programme comme étant 'en travaux'. Elles sont aléatoirement choisies et ne peuvent alors pas être desservies. 

![page d accueil]( https://github.com/CelineCornet/Subway/blob/master/images/01.png)

# Exécution du programme
Le programme permet de calculer l’itinéraire et le temps pour aller d’une station à une autre. On peut renseigner à la page d’accueil les deux stations, choisir le moyen le plus rapide ou avec le moins de correspondance ainsi que l’heure du départ.

Une fois configuré, on arrive à une fenêtre qui nous offre 1 à 3 itinéraires selon les possibilités. 

![]( https://github.com/CelineCornet/Subway/blob/master/images/02.png)

Il suffit d’appuyer sur ‘détail’ pour avoir l’explication de l’itinéraire calculé.

![]( https://github.com/CelineCornet/Subway/blob/master/images/03.png)

Cependant, il se peut qu’une station ne puisse pas être desservie dû travaux. Dans ce cas, il n’y a pas de possibilité et une fenêtre le signale.

![travaux]( https://github.com/CelineCornet/Subway/blob/master/images/04.png)

Dans le cas où il n’y a pas de solutions d'itinéraires, le programme affiche l’image suivante.

![pas de cas possible]( https://github.com/CelineCornet/Subway/blob/master/loney2.jpg)

