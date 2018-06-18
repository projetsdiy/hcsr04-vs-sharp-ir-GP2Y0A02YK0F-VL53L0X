/* Distance measurement comparison with HC-SR04, Sharp GP2Y0A02YK0F and VL53L0X SMTe sensors
 * Recording of 10 measurement points. Time between each changeable point with the key DELAY_REFRESH
 * Reset between each distance to restart the program

 * Manual calibration procedure of the Sharp GP2Y0A02YK0F proximity sensor
 * https://projetsdiy.fr/test-and-calibration-of-the-proximity-captor-a02yk0-clone-asiatic-of-sharp-gp2y0a02yk0f/
 *
 * Test support to print in 3D
 * https://www.thingiverse.com/thing:2961289
 * 
 * Comparaison de mesure de distance avec les capteurs HC-SR04, Sharp GP2Y0A02YK0F et VL53L0X SMTe
 * Enregistrement de 10 points de mesure. Temps entre chaque point modifiable avec la clé DELAY_REFRESH
 * Faire un reset entre chaque distance pour relancer le programme
 * 
 * Procédure d'étalonnage manuel du Sharp GP2Y0A02YK0F
 * https://projetsdiy.fr/test-et-etalonnage-du-capteur-de-proximite-a02yk0-clone-asiatique-du-sharp-gp2y0a02yk0f/
 * 
 * Support de test à imprimer en 3D 
 * https://www.thingiverse.com/thing:2961289
 */

#include <Wire.h>
#include <hcsr04.h>
#include <VL53L0X.h>
#include <MD_MAX72xx.h>
// ZSharpIR changes to apply - modification à apporter au fichier: Arduino/Librarie/ZSharpIR.cpp
// Sort it 
//    sort(ir_val,NB_SAMPLE);
// add - ajouter
//    if (_model==999){
//        distanceMM=(int)(138773.464825 * pow(ir_val[0],-1.0233470));
//    }
// change WMath -> Math.h
// Comment analogReadResolution( res);
#include <ZSharpIR.h>

#define ir A0
#define model 20150
#define OFFSET_HCSR04     -10
#define PIN_HCSR04_ECHO   4
#define PIN_HCSR04_TRIG   3
#define PIN_GP2Y0A02YK0F  A0
#define DELAY_REFRESH     1000
#define POINT_FILTER      10
#define LONG_RANGE       true

int posFilter = 0;
int filterVl53[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int filterSr04[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int filterIr[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

ZSharpIR SharpIR(ir, model);

HCSR04 hcsr04(PIN_HCSR04_TRIG, PIN_HCSR04_ECHO, 20, 4000);
VL53L0X vl53;

void setup() {
  Wire.begin();
  Serial.begin(115200);

  vl53.init();
  vl53.setTimeout(DELAY_REFRESH - 100);

  #if defined LONG_RANGE
    // lower the return signal rate limit (default is 0.25 MCPS)
    vl53.setSignalRateLimit(0.1);
    // increase laser pulse periods (defaults are 14 and 10 PCLKs)
    vl53.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
    vl53.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
  #endif
  
  vl53.startContinuous(DELAY_REFRESH - 50);

}

void print(String key, String val, boolean returnline ) {
  Serial.print(key); Serial.print(": ");
  if ( returnline ) {
    Serial.println(val);
  } else {
    Serial.print(val);
  }
}

bool measure = true;

void loop() {
  if ( measure ) {
    float distSR04 = hcsr04.distanceInMillimeters() + OFFSET_HCSR04;
    int distSharp = SharpIR.distance();
    float distVl53 = vl53.readRangeContinuousMillimeters();
    if (vl53.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
    print("HR-SR04 (mm)", String(distSR04), false);
    print(" | IR (mm)", String(distSharp), false);
    print(" | VL53L0X (mm)", String(distVl53), true);
    if ( distVl53 != 8190 ) {
       filterVl53[posFilter] = distVl53;
    }
    
    filterSr04[posFilter] = distSR04;
    filterIr[posFilter] = distSharp;
    posFilter++;

    if (posFilter >= 10 ) {
      Serial.println("# résultats des mesures");
      measure = false;
      for ( int k = 0 ; k <= 9 ; k++ ) {
        //Serial.print(k);
        
        Serial.print(filterSr04[k]); Serial.print(";");
        Serial.print(filterIr[k]); Serial.print(";");
        Serial.println(filterVl53[k]); 
      }

    }
  }  
  delay(DELAY_REFRESH);
} 
  



