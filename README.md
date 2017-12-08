# embedded-proximity-sonar

**Projet de sonar embarqué de proximité avec afficheur 7-segment à 3 digits**

*Projet du cadre de la semaine embarquée 3A LE à TELECOM Nancy*

### Auteurs

- Guillaume Garcia - 3A LE
- Alexandre Jamet - 3A LE

### Architecture cible

Le projet a été conçu et testé pour et sur une carte embarquée **Olimex-olinuxino-maxi**, tournant sur un Linux *embarqué* (**buildroot**)

Pour tester ce projet il vous faudra donc :
- Une carte *Olimex-olinuxino-maxi* tournant sur *buildroot*
- Une PC hôte de type Debian pour compiler et exporter les drivers et module sur la carte embarquée
- Un module de détection ultrasonique *HC-SR04*
- Un afficheur 7-segments à 3 digits *HDSP-433G*
- 12 diodes IN4004
- 1 LED (verte) simple à 2 broches
- 4 transistors 2N2222
- 8 résistances 1.5 Ohms
- 3 résistances 33 Ohms
- 3 résistances 12k Ohms
- De nombreaux fils électriques ainsi qu'une platine d'essai

Les circuits électriques sont spécifiés dans les README des drivers.

### Le projet de la semaine embarquée

Sonar de proximité affichant le distance calculée toutes les secondes en **mm** sur un afficheur 7-segments à 3 digits.

Ce projet est constitué des éléments suivants :
- le driver du sonar de proximité sous `ProximityRadarDriver`
- le driver du 7-segment à 3 digits sous `DisplayDriver`
- un petit module utilisateur faisant l'interface entre les 2 composants précédents, calculant la distance de l'objet en face du sonar toutes les secondes. Sous : `SonarDisplayUserInterface`.
- une petite vidéo de présentation du projet présentée par G.Garcia filmée par A.Jamet

Une documentation complète de chaque driver/circuit électronique utilisé est détaillée dans [proximity-sonar-driver/README.md](proximity-sonar-driver/README.md) et [display-driver/README.md](display-river/README.md)

### Vidéo de démonstration du projet

[![IMAGE ALT TEXT HERE](http://img.youtube.com/vi/S7bQagEMhYM/0.jpg)](http://www.youtube.com/watch?v=S7bQagEMhYM)
