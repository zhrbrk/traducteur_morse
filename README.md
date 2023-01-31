# traducteur_morse
Traducteur de code Morse sur Arduino.

Retrouvez ici la vidéo de démonstration : https://youtu.be/art60jmW170

<h2>Matériel</h2>

* 1 Arduino Nano ATmega 328 ;
* 1 écran OLED I2C avec contrôleur SSD1306 ;
* 3 boutons poussoir et des résistances adaptés ;
* 1 LED et une résistance adaptée ;
* 1 buzzer et une résistance adaptée.

Note : le schéma de branchement est disponible dans les fichiers du projet.

<h2>Caractéristiques du projet</h2>

* La LED s'allume dès que le traducteur est intialisé et prêt à être utilisé.


* Le bouton poussoir relié à la broche D7 permet de taper le code Morse souhaité :
  - Une pression entre 0 et 0.2s correspond à un point '.' ;
  - Une pression entre au-delà de 0.2s et ne dépassant pas les 0.7s (0.2+0.5) correspond à un tiret '_'.

  Si la pression dépasse 0.7s, il ne se passe rien.
  /!\ Ces valeurs peuvent être modifiées aux lignes 244 pour le point '.' et 255 pour le tiret '_'.

  Si au bout de 2s, aucun caractère supplémentaire n'est ajouté, le traducteur considère que le code Morse de la lettre est terminé et l'interprète.
  /!\ Cette valeur peut être modifiée à la ligne 213, dans la condition de la boucle if-else.

* Le bouton poussoir relié à la broche D2 permet de supprimer le mot entièrement ou la lettre en cours :
  - Une pression entre 0 et 0.2s permet de supprimer la lettre en cours de frappe (et non pas la dernière lettre interprétée par le traducteur) ;
  - Une pression au-delà de 0.2s et ne dépassant pas les 0.7s permet de supprimer le mot entièrement.

  /!\ Ces valeurs peuvent être modifiées à la ligne 123.


* Le buzzer émet un son d'une durée de 0.15s si un point est tapé, et un son d'une durée de 0.3s si un tiret est tapé.


* Le bouton poussoir relié à la broche D3 permet d'accéder à l'alphabet Morse.
