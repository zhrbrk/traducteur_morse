# traducteur_morse
Traducteur de code Morse sur Arduino Nano ATmega 328.

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
  - Une pression entre 0 et 0.2s correspond à un point ;
  - Une pression entre au-delà de 0.2s et ne dépassant pas les 0.7s correspond à un tiret.

  Si la pression dépasse 0.7s, il ne se passe rien.
  /!\ Ces valeurs peuvent être modifiées aux lignes 253 ou 269, ou en cherchant (Ctrl+F) : "Modifier temps pression pour le point" et "Modifier temps pression pour le tiret".


* Le bouton poussoir relié à la broche D2 permet de supprimer le mot entièrement ou la lettre en cours :
  - Une pression entre 0 et 0.2s permet de supprimer la lettre en cours de frappe (et non pas la dernière lettre interprétée par le traducteur) ;
  - Une pression au-delà de 0.2s et ne dépassant pas les 0.7s permet de supprimer le mot entièrement.

  Si la pression dépasse 0.7s, il ne se passe rien.
  /!\ Ces valeurs peuvent être modifiées aux lignes 253 ou 269, ou en cherchant (Ctrl+F) : "Modifier temps pour point" et "Modifier temps pour tiret".


* Le buzzer émet un son d'une durée de 0.15s si un point est tapé, et un son d'une durée de 0.3s si un tiret est tapé.


* Le bouton poussoir relié à la broche D3 permet d'accéder à l'alphabet Morse.
