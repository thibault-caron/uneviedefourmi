# uneviedefourmi
exploration des structures nodales via un exemple de fourmilière

# Analyse du problème de déplacement de fourmis

pathing optimisation:

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

1. Représente la fourmilière comme un graphe
2. Utilise un algorithme de plus court chemin (comme BFS) pour déterminer les chemins optimaux
3. Simule le déplacement des fourmis en respectant les contraintes
4. Optimise l'ordre de déplacement des fourmis