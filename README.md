# Proximity Sonar & 7-segment Displayer Project

### Auteurs

- Guillaume Garcia - 3A LE
- Alexandre Jamet - 3A LE

### Le projet de la semaine embarquée

Sonar de proximité affichant le distance calculée en **mm** sur un afficheur 7-segments à 3 digits.

Ce projet est constitué des éléments suivants :
- le driver du sonar de proximité sous `ProximityRadarDriver`
- le driver du 7-segment à 3 digits sous `DisplayDriver`
- un petit module utilisateur faisant l'interface entre les 2 composants précédents, calculant la distance de l'objet en face du sonar toutes les secondes. Sous : `SonarDisplayUserInterface`.
- une petite vidéo de présentation du projet présentée par G.Garcia filmée par A.Jamet

Une documentation complète de chaque driver/circuit électronique utilisé est détaillée dans [ProximityRadarDriver/README.md](ProximityRadarDriver/README.md) et [ProximityRadarDriver/README.md](DisplayDriver/README.md)
