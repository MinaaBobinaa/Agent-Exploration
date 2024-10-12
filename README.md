# Agent d'exploration d'un univers à portails

## Description
Ce projet implémente un agent capable d'explorer un univers composé de salles interconnectées par des portails. Chaque salle est définie par une position dans une grille et les portails permettent de lier des cellules entre différentes salles. L'agent est conçu pour répondre à des requêtes, cherchant tous les chemins possibles entre deux points sans passer deux fois par la même cellule.

Le programme lit deux fichiers d'entrée :
1. **Fichier d'univers** : décrit la configuration des salles et des portails.
2. **Fichier de requêtes** : contient des requêtes de chemins entre deux cellules de l'univers.

## Fonctionnalités
- Lecture et parsing d'un fichier d'univers définissant les salles et les portails.
- Lecture des requêtes pour trouver tous les chemins possibles entre deux points.
- Utilisation d'un algorithme de parcours avec des marqueurs pour éviter de repasser par la même cellule.
- Retourne le nombre total de chemins pour chaque requête ou -1 si aucun chemin n'est possible.

## Format des fichiers

### Fichier d'univers
Le fichier d'univers contient une liste de salles, suivie par une liste de portails qui les relient. Les portails sont décrits par les deux cellules qu'ils connectent, séparées par une flèche bilatérale.

Exemple d'univers :


```
carre 4
diamant 5
triangle 7
-----
0 (1,0) <--> 1 (0,2)
0 (0,3) <--> 2 (0,6)
1 (3,3) <--> 2 (3,3)
```

### Fichier de requêtes
Chaque requête est définie par une cellule de départ et une cellule d'arrivée. L'algorithme calcule tous les chemins possibles entre ces deux points.

Exemple de fichier de requêtes :

```
0 (3,0) --> 2 (2,3)
1 (2,1) --> 2 (0,0)
```

### Format de sortie
Le programme retourne pour chaque requête le nombre total de chemins possibles entre les deux points, sans passer deux fois par la même cellule.

Exemple de sortie :

```
6050
45370
```


