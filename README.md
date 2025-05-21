# uneviedefourmi
exploration des structures nodales (pathing?) via un exemple de fourmilière.

## how to use:

1. open a terminal
2. use 'cd' to go to the project folder "uneviedefourmi" (or open a terminal directly from that folder)
3. in the terminal launch the .exe: write 'ant_colony.exe' (or './ant_colony' with Powershell) followed by the .txt file of the ant colony you wish to use

Exemple:

```bash
.\ant_colony fourmiliere_cinq.txt
```

## analyse:

1. **Structure de graphe**: La fourmilière est essentiellement un graphe où:
   - Les nœuds sont les salles (vestibule Sv, dortoir Sd, et salles intermédiaires)
   - Les arêtes sont les tunnels entre les salles
   
2. **Contraintes**:
   - Les fourmis démarrent toutes du vestibule
   - Chaque salle intermédiaire ne peut contenir qu'une seule fourmi à la fois (sauf indication contraire)
   - Une fourmi ne peut entrer dans une salle que si celle-ci est vide ou si sa seule occupante est en train de partir
   - Les tunnels sont traversés instantanément

3. **Objectif**:
   - Déplacer toutes les fourmis vers le dortoir en un minimum d'étapes

## Approche algorithmique

Pour résoudre ce problème:

1. Représenter la fourmilière comme un graphe
2. Utiliser un algorithme de plus court chemin (comme BFS) pour déterminer les chemins optimaux
3. Simuler le déplacement des fourmis en respectant les contraintes
4. Optimiser l'ordre de déplacement des fourmis