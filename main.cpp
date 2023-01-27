#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const byte pinBouton = 7; //liee au bouton qui permet de taper des caracteres Morse ('.', '_')
const byte pinLed = 4; //liee a la LED
const byte pinBoutonAlphab = 3; //liee au bouton qui permet d'afficher l'alphabet Morse
const byte pinControle = 2; //liee au bouton qui permet de supprimer la lettre en cours ou le mot 
const byte pinBuzz = 5; //liee au buzzer (emet des sons)

const byte absMorse = 28; //pour contrôler les coordonnées des '.' et '_' affichés
const byte ordMorse = 44;
byte caracMorse = 0; //pour compter le nbr de caractères ('.' ou '_') écrits, remis 0 à la fin de chaque lettre
char lettreMorse[5]; //pour stocker les caractères tapés, remis à 0 lorsque la lettre est finie


const byte absMot = 10; //pour contrôler les coordonnées des lettres (du mot que l'on veut écrire) affichées
const byte ordMot = 20;
byte numLettre = 0; //pour compter le nombre de lettres écrites pour l'instant

unsigned long tempsDebut = 0; //pour compter le temps qui s'écoule après avoir taper un caractère ('.' ou '_')

char mot[9]; //on limite la taille du mot (car l'écran n'est pas infini)
byte maxLettres = 9-1;

//à chaque fin de lettre, il faut effacer tout ce qu'il y a sur l'écran pour supprimer les . et _
//donc il faut stocker tous les autres éléments de l'écran pour les réafficher
//à stocker : les précédentes lettres
//comment ? : sous forme de string 
//la prochaine lettre ne sera pas affichée à côté individuellement, mais directement concaténée au string principal

bool alphabet = 0; //permet de savoir si on est dans l'alphabet ou pas

char conversion(char* morse) { //convertit une série de caractères Morse ('.' et '_') en lettre de l'alphabet latin

  if ( strcmp(morse, "._") == 0) { return 'A'; }
  if ( strcmp(morse, "_...") == 0) { return 'B'; }
  if ( strcmp(morse, "_._.") == 0) { return 'C'; }
  if ( strcmp(morse, "_..") == 0) { return 'D'; }
  if ( strcmp(morse, ".") == 0) { return 'E'; }
  if ( strcmp(morse, ".._.") == 0) { return 'F'; }
  if ( strcmp(morse, "__.") == 0) { return 'G'; }
  if ( strcmp(morse, "....") == 0) { return 'H'; }
  if ( strcmp(morse, "..") == 0) { return 'I'; }
  if ( strcmp(morse, ".___") == 0) { return 'J'; }
  if ( strcmp(morse, "_._") == 0) { return 'K'; }
  if ( strcmp(morse, "._..") == 0) { return 'L'; }
  if ( strcmp(morse, "__") == 0) { return 'M'; }
  if ( strcmp(morse, "_.") == 0) { return 'N'; }
  if ( strcmp(morse, "___") == 0) { return 'O'; }
  if ( strcmp(morse, ".__.") == 0) { return 'P'; }
  if ( strcmp(morse, "__._") == 0) { return 'Q'; }
  if ( strcmp(morse, "._.") == 0) { return 'R'; }
  if ( strcmp(morse, "...") == 0) { return 'S'; }
  if ( strcmp(morse, "_") == 0) { return 'T'; }
  if ( strcmp(morse, ".._") == 0) { return 'U'; }
  if ( strcmp(morse, "..._") == 0) { return 'V'; }
  if ( strcmp(morse, ".__") == 0) { return 'W'; }
  if ( strcmp(morse, "_.._") == 0) { return 'X'; }
  if ( strcmp(morse, "_.__") == 0) { return 'Y'; }
  if ( strcmp(morse, "__..") == 0) { return 'Z'; }
  return '?'; //dans le cas où l'utilisateur entre une série de '.' et '_' qui ne correspond à rien, on renvoie '?'
}


void setup() {

  Serial.begin(9600);

  pinMode(pinBouton, INPUT);
  pinMode(pinLed, OUTPUT);
  pinMode(pinBoutonAlphab, INPUT);
  pinMode(pinBuzz, OUTPUT);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(28, 22);

  //Affichage du nom du projet
  display.println(F("Traducteur de"));
  display.setCursor(38, 30);
  display.println(F("code Morse"));
  display.drawRect(20, 16, 93, 27, WHITE);

  //Animation du mot "Zahara"
  display.setCursor(41, 52); display.println(F("Z")); display.display(); delay(200);
  display.setCursor(49, 52); display.println(F("A")); display.display(); delay(200);
  display.setCursor(57, 52); display.println(F("H")); display.display(); delay(200);
  display.setCursor(65, 52); display.println(F("A")); display.display(); delay(200);
  display.setCursor(73, 52); display.println(F("R")); display.display(); delay(200);
  display.setCursor(81, 52); display.println(F("A")); display.display();

  delay(4000);
  display.clearDisplay();
  display.display();
}

void loop() {

  digitalWrite(pinLed, HIGH); //allume la LED (ne s'éteint jamais)
  
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("Tapez votre mot :"));
  display.display();

  display.drawRect(absMot-3, ordMot-3, absMot+6*maxLettres, ordMot-3, WHITE);
  //la taille du rectangle est calculee de facon a correspondre exactement au nombre de lettres maximum autorise

  display.setCursor(absMot, ordMot);
  display.println(mot); //on affiche le nouveau mot à chaque itération de la loop() (ou le même, si pas de changement !)

  if ( digitalRead(pinControle) == HIGH ) { 

    delay(200); 
    if (digitalRead(pinControle) == LOW) { //si la pression dure entre 0 et 0.2s : annuler la lettre en cours

        //Suppression de la lettre en cours au niveau data
        caracMorse = 0;
        for (int i = 0; i < 4; i++) {
          lettreMorse[i] = NULL;
        }
        display.fillRect(absMorse, ordMorse-3, absMorse+100, ordMorse+3, BLACK); //on efface les caracteres '.' et '_' tapés/affichés
        display.display();

        display.setCursor(0, ordMorse);
        display.println(F("Lettre annulee."));
        display.display();
        delay(1500);
        display.fillRect(0, ordMorse-3, absMorse+100, ordMorse+3, BLACK); //on efface "Lettre annulee"
        display.display();

    } else { //si la pression dure au delà de 0.2s : supprimer le mot
        
        //Suppression du mot, ainsi que de la lettre en cours si elle existe
        numLettre = 0;
        for (int i = 0; i <= maxLettres; i++) {
          mot[i] = NULL;
        }
        caracMorse = 0;
        for (int i = 0; i < 4; i++) {
          lettreMorse[i] = NULL;
        }

        display.fillRect(absMot-3, ordMot-3, absMot+6*maxLettres+3, ordMot-3, BLACK); //on efface le mot précédemment affiché
        display.fillRect(absMorse, ordMorse-3, absMorse+100, ordMorse+3, BLACK); //on efface les caractères Morse
        display.display();

        display.setCursor(0, ordMorse);
        display.println(F("Mot supprime."));
        display.display();
        delay(1500);
        display.fillRect(0, ordMorse-3, absMorse+100, ordMorse+3, BLACK); //on efface "Mot supprime"
        display.display();

    }
    
  }

  if ( digitalRead(pinBoutonAlphab) == HIGH ) {
    display.clearDisplay();

    display.setCursor(0, 0); display.println(F("A : ._ | B : _..."));
    display.setCursor(0, 10); display.println(F("C : _._. | D : _.."));
    display.setCursor(0, 20); display.println(F("E : . | F : .._."));
    display.setCursor(0, 30); display.println(F("G : _ _. | H : ...."));
    display.setCursor(0, 40); display.println(F("I : .. | J : . _ _ _"));
    display.setCursor(0, 50); display.println(F("K : _._ | L : ._.."));
    display.display();

    delay(6000); //la "premiere page" de l'alphabet Morse reste affichee 6s
    display.clearDisplay();

    display.setCursor(0, 0); display.println(F("M : _ _ | N : _."));
    display.setCursor(0, 10); display.println(F("O : _ _ _ | P : ._ _."));
    display.setCursor(0, 20); display.println(F("Q : _ _._ | R : ._."));
    display.setCursor(0, 30); display.println(F("S : ... | T : _"));
    display.setCursor(0, 40); display.println(F("U : .._ | V : ..._"));
    display.setCursor(0, 50);
    display.println(F("W : ._ _ | X : _.._"));
    display.display();

    delay(6000);
    display.clearDisplay();

    display.setCursor(0, 0); display.println(F("Y : _._ _ | Z : _ _.."));
    display.display();

    delay(6000);
    
    display.clearDisplay(); //nettoyage de l'écran pour ensuite réafficher l'interface "Tapez votre mot :", etc
    display.display();

    //Apres chaque appel de l'alphabet Morse, on reinitialise la lettre en cours (on devra la recommencer)
    caracMorse = 0;
    for (int i = 0; i < 4; i++) {
      lettreMorse[i] = NULL;
    }

    //L'interface initiale (avec le texte "Tapez votre mot :", etc) se réaffichera lors de la prochaine iteration de loop()
  }

  if ( digitalRead(pinBouton) == LOW ) {

    if (millis() - tempsDebut >= 2000 && caracMorse != 0) {
      //on verifie que caracMorse != 0 pour etre sur qu'on est actuellement en train d'écrire une lettre

      display.fillRect(absMot, ordMot-3, absMot+100, ordMot+3, BLACK); //on efface le mot précédemment affiché, on affichera le nouveau ensuite
      display.fillRect(absMorse, ordMorse-3, absMorse+100, ordMorse+3, BLACK); //pour effacer les caractères morse
      display.display();

      char lettreConverti = conversion(lettreMorse); //conversion Morse->alphabet latin
      mot[numLettre] = lettreConverti; //mise à jour du mot
      numLettre++; //on passe à la lettre suivante, donc a la case suivante dans le char* mot

      caracMorse = 0; //remis à 0, car on passe à une nouvelle lettre (les . et _ précédents seront effacés pour être remplacés)
      for (int i = 0; i < 4; i++) { //remise à 0 de lettreMorse
        lettreMorse[i] = NULL;
      }

      //Le nouveau mot sera affiche au debut de la prochaine iteration de loop()
    }

  } else if ( digitalRead(pinBouton) == HIGH ) {

    if (numLettre == maxLettres) {
      display.setCursor(0, ordMorse);
      display.println(F("Plus de place !"));
      display.display();
      delay(1500);
      display.fillRect(0, ordMorse-3, absMorse+100, ordMorse+3, BLACK); //on efface "Plus de place"
      display.display();

    } else if (caracMorse < 4) { //tant que l'on a pas depasse le nombre maximum de caractères Morse ('.', '_') qui est de 4, on peut ecrire des caracteres Morse

      delay(200); 
      if (digitalRead(pinBouton) == LOW) { //si la pression dure entre 0 et 0.2s : un point '.'

        //Buzzer emet un son d'une duree de 0.15s
        tone(pinBuzz, 400);
        delay(150);
        noTone(pinBuzz);

        display.setCursor(absMorse + caracMorse*5, ordMorse);
        //l'emplacement du nouveau caractere est logiquement calcule
        //par rapport au nombre de caracteres precedemment entres :
        //cela permet que les caracteres se suivent joliment

        display.println(F("."));
        lettreMorse[caracMorse] = '.'; //on ajoute le caractere a lettreMorse qui sera ensuite utiliser pour trouver la lettre latine correspondante
        caracMorse++;

        display.display();
        tempsDebut = millis();
        //on commence à mesurer le temps qui va s'écouler entre cette lettre et la potentielle suivante :
        //si ce temps excède 2s (si on reste à l'état LOW durant un temps >= à 2),
        //alors le code comprend que la lettre est terminée et l'interprète

      } else {

        delay(500);
        if (digitalRead(pinBouton) == LOW) { //si la pression dure entre 0.2s et 0.2+0.5s=0.7s : un trait '_'
  
          tone(pinBuzz, 400);
          delay(300);
          noTone(pinBuzz);

          display.setCursor(absMorse + caracMorse*5, ordMorse);
          display.println(F("_"));
          lettreMorse[caracMorse] = '_';
          display.display();
          caracMorse++;
          tempsDebut = millis();

        } else {
          //dans le cas où on appuie au delà de ce temps, on ne fait rien
          //on l'écrit ici pour ne pas avoir de réaction aléatoire

          //dans les faits, si on appuie trop longtemps on ne lance pas : tempsDebut = millis();
          //donc si on avait commencé à écrire une lettre, au bout d'un moment elle sera comme terminée
        }
      }

    }
  }

}
