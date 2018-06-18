# Comparative study between 3 proximity sensor technologies with an Arduino
Download the Arduino project [HC-SR04_vs_VL53L0X_vs_SharpIR](https://github.com/projetsdiy/hcsr04-vs-sharp-ir-GP2Y0A02YK0F-VL53L0X/tree/master/HC-SR04_vs_VL53L0X_vs_SharpIR)

Install the following libraries from the library manager of the Arduino IDE
- [HC-SR04 (Bifrost library)](https://github.com/jeremylindsayni/Bifrost.Arduino.Sensors.HCSR04)
- [Pololu VL53L0X](https://github.com/pololu/vl53l0x-arduino)
- [ZSharpIR by Pierre Valleau](https://github.com/zoubworldArduino/ZSharpIR)
- [MD_MAX72XX from majicDesigns](https://github.com/MajicDesigns/MD_MAX72XX)

Change the settings in the Arduino program
- model, 20150 for the Sharp GP2Y0A02YK0F
- PIN_HCSR04_ECHO
- PIN_HCSR04_TRIG
- PIN_GP2Y0A02YK0F

Changes to the file: Arduino/Librarie/ZSharpIR.cpp
- change WMath -> Math.h
- comment out analogReadResolution (res);
- After
```js
// Sort it
 sort (ir_val, NB_SAMPLE);
```
- add
```js
if (_model == 999) {
 distanceMM = (int) (138773.464825 * pow (ir_val [0], - 1.0233470));
}
```

Between each measurement, make a ** Reset ** to restart the program. The program makes 10 measurements then displays the table of measured distances.

Use the LibreOffice spreadsheet to compare the performance of each sensor based on distance and measurement environment.

Here's what I got after manually calibrating the Sharp IR
! [alt text](https://github.com/projetsdiy/hcsr04-vs-sharp-ir-GP2Y0A02YK0F-VL53L0X/blob/master/Img/hc-sr04%20vs%20sharp%20GP2YAO2%20vs%20VL53L0X.png)

# Project, measure the fill level of a water tank with HC-SR04 or V053L0X
It is very easy to measure the fill level of a water tank using an Arduino, ESP8266 (ESP8266) or ESP32 (WiFi + Bluetooth) microcontroller.
This project compares the HC-SR04 sensor that uses ultrasound and the VL53L0X that uses a laser beam.

## Equipment used
- [Arduino nano v3](http://bit.ly/2MvO8Dj) or [Arduino Uno r3](http://bit.ly/2yicddF)
- [Breadboard + jumpers](http://bit.ly/2Mgxl76)
- [HC-SR04](http://bit.ly/2JJiXH0)
- [VL53L0X](http://bit.ly/2lgDpAw)
- [Led Matrix 4 * 64 points](http://bit.ly/2th2COD)

## Arduino source code for the project
Download the Arduino project [HC-SR04_Sharp_GY_VL053L0X](https://github.com/projetsdiy/hcsr04-vs-sharp-ir-GP2Y0A02YK0F-VL53L0X/blob/master/HC-SR04_Sharp_GY_VL053L0X/HC-SR04_Sharp_GY_VL053L0X.ino)
Change the following settings:
- Activate the demo by passing the key **THANK_DEMO** to true
- Specify the SPI pins of the [Dot matrix](http://bit.ly/2th2COD) CLK_PIN, DATA_PIN, CS_PIN
- For a [Pololu compatible 64x4 points led block](http://bit.ly/2th2COD), you must choose the type **HARDWARE_TYPE MD_MAX72XX :: FC16_HW**

[All the details on the blog] (https://diyprojects.io/hc-sr04-ultrasound-vs-sharp-gp2y0a02yk0f-ir-vl53l0x-laser-solutions-choose-distance-measurement-arduino-raspberrypi/)

If you are having trouble making distance measurements with the Sharp IR, read [this tutorial](https://diyprojects.io/proximity-sensor-a02yk0-test-calibration-sharp-gp2y0a02yk0f-asian-clone/) that explains how to do a manual calibration

[Demonstration video](https://youtu.be/EF3lKs7dgTE)

# Etude comparative entre 3 technologies de capteurs de proximité avec un Arduino
Téléchargez le projet Arduino [HC-SR04_vs_VL53L0X_vs_SharpIR](https://github.com/projetsdiy/hcsr04-vs-sharp-ir-GP2Y0A02YK0F-VL53L0X/tree/master/HC-SR04_vs_VL53L0X_vs_SharpIR)

Installez les librairies suivantes depuis le gestionnaire de bibliotèque de l'IDE Arduino
- [HC-SR04 (Bifrost library)](https://github.com/jeremylindsayni/Bifrost.Arduino.Sensors.HCSR04)
- [VL53L0X de Pololu](https://github.com/pololu/vl53l0x-arduino)
- [ZSharpIR de Pierre Valleau](https://github.com/zoubworldArduino/ZSharpIR)
- [MD_MAX72XX de majicDesigns](https://github.com/MajicDesigns/MD_MAX72XX)

Modifiez les paramètres dans le programme Arduino
- model, 20150 pour le Sharp GP2Y0A02YK0F
- PIN_HCSR04_ECHO   
- PIN_HCSR04_TRIG   
- PIN_GP2Y0A02YK0F  

Modifications à apporter au fichier: Arduino/Librarie/ZSharpIR.cpp
- changer WMath -> Math.h
- Mettre en commentaire analogReadResolution( res);
- Après
```js
// Sort it 
   sort(ir_val,NB_SAMPLE);
```
- ajouter
```js
   if (_model==999){
        distanceMM=(int)(138773.464825 * pow(ir_val[0],-1.0233470));
   }
```
Entre chaque mesure, faites un **Reset** pour relancer le programme. Le programme fait 10 mesures puis affiche le tableau des distances mesurées. 

Utilisez la feuille de calcul LibreOffice pour comparer la performance du chaque capteur en fonction de la distance et des l'environnement de mesure. 

Si vous rencontrez des difficultés pour réaliser des mesures de distance avec le Sharp IR, lisez [ce tutoriel](https://projetsdiy.fr/test-et-etalonnage-du-capteur-de-proximite-a02yk0-clone-asiatique-du-sharp-gp2y0a02yk0f/) qui explique comment faire un étalonnage manuel

Voici ce que j'ai obtenu après avoir étalonné manuellement le Sharp IR
![alt text](https://github.com/projetsdiy/hcsr04-vs-sharp-ir-GP2Y0A02YK0F-VL53L0X/blob/master/Img/hc-sr04%20vs%20sharp%20GP2YAO2%20vs%20VL53L0X.png)

# Projet, mesurer le niveau de remplissage d'un réservoir d'eau avec un HC-SR04 ou un V053L0X
Il est très facile de mesurer le niveau de remplissage d'un réservoir d'eau à l'aide d'un micro-contrôleur Arduino, ESP8266 (ESP8266) ou ESP32 (WiFi + Bluetooth).
Ce projet permet de comparer le capteur HC-SR04 qui utilise des ultrasons et le VL53L0X qui utilise un faisceau laser.

## Matériel utilisé
- [Arduino nano v3](http://bit.ly/2MvO8Dj) ou [Arduino Uno r3](http://bit.ly/2yicddF)
- [Breadboard + jumpers](http://bit.ly/2Mgxl76)
- [HC-SR04](http://bit.ly/2JJiXH0)
- [VL53L0X](http://bit.ly/2lgDpAw)
- [Matrice de Led 4*64 points](http://bit.ly/2th2COD)

## Code source Arduino du projet 
Téléchargez le projet Arduino [HC-SR04_Sharp_GY_VL053L0X](https://github.com/projetsdiy/hcsr04-vs-sharp-ir-GP2Y0A02YK0F-VL53L0X/blob/master/HC-SR04_Sharp_GY_VL053L0X/HC-SR04_Sharp_GY_VL053L0X.ino)
Modifiez les paramètres suivants :
- Activer la démo en passant la clé **THANK_DEMO** à true
- Indiquez les broches SPI de la [matrice de LED](http://bit.ly/2th2COD) CLK_PIN, DATA_PIN, CS_PIN
- Pour un [bloc de led 64x4 points compatible Pololu](http://bit.ly/2th2COD), il faut choisir le type **HARDWARE_TYPE MD_MAX72XX::FC16_HW**

[Tous les détails sur le blog](https://projetsdiy.fr/comparaison-hc-sr04-ultrasons-sharp-gp2y0a02yk0f-ir-vl53l0x-laser-quelle-solutions-choisir-mesure-distance-arduino-raspberry-pi/)

[Vidéo de démonstration](https://youtu.be/EF3lKs7dgTE)

## Forums
Si vous rencontrez des difficultés dans la réalisation de vos projets d'objets connectés ou domotique, rendez-vous sur le forum :
- [Tous les sujets](https://forum.projetsdiy.fr/)
- [Arduino, ESP8266, ESP32](https://forum.projetsdiy.fr/c/arduino-esp8266-esp32)
- [Mini PC](https://forum.projetsdiy.fr/c/mini-pc-raspberry-pi-orange-pi)
- [Objets connectés](https://forum.projetsdiy.fr/c/objets-connectes)
- [Domotique](https://forum.projetsdiy.fr/c/domotique)
- [Retro gaming](https://forum.projetsdiy.fr/c/retrogaming)

Il est possible que certains liens dans la description soient des liens d'affiliation, ça ne change absolument rien pour vous. Vous aidez juste à soutenir mon blog et je vous en remercie :)
