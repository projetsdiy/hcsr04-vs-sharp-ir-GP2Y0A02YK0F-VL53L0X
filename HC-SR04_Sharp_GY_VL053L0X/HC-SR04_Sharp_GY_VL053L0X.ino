/* Distance measurement comparison with HC-SR04, Sharp GP2Y0A02YK0F and VL53L0X SMTe sensors
 * A0 raw signal
 * Thank level monitoring HC-SR04 vs VL53L0X
 *
 * Manual calibration procedure of the Sharp GP2Y0A02YK0F proximity sensor
 * https://projetsdiy.fr/test-and-calibration-of-the-proximity-captor-a02yk0-clone-asiatic-of-sharp-gp2y0a02yk0f/
 *
 * Test support to print in 3D
 * https://www.thingiverse.com/thing:2961289
 * 
 * Comparaison de mesure de distance avec les capteurs HC-SR04, Sharp GP2Y0A02YK0F et VL53L0X SMTe
 * Affiche le signal A0 pour étalonnage manuel SharpIR
 * Démo mesure taux de remplissage HC-SR04 vs VL53L0X
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
#include <ZSharpIR.h>

#define ir A0
#define model 999
#define OFFSET_HCSR04     -10
#define PIN_HCSR04_ECHO   4
#define PIN_HCSR04_TRIG   3
#define PIN_GP2Y0A02YK0F  A0
#define DELAY_REFRESH     1000
#define POINT_FILTER      10
#define LONG_RANGE        true
#define RAW_SIGNAL        false   // A0 -> Serial monitor
#define COMPARE           true    // Measure 10 points for HC-SR04, SharpIR, VL53L0X
#define THANK_DEMO        false   // Thank demo with 4x64 dot matrix display
// Define the number of devices we have in the chain and the hardware interface
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES  4
   
/*
  pin 12 is connected to the DataIn
  pin 11 is connected to the CLK
  pin 10 is connected to LOAD
*/
const uint8_t CLK_PIN = 12;   // or SCK
const uint8_t DATA_PIN = 11;  // or MOSI
const uint8_t CS_PIN = 10;    // or SS

int posFilter = 0;
int countFilter = 0;
int filterVl53[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int filterSr04[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int filterIr[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

ZSharpIR SharpIR(ir, model);
HCSR04 hcsr04(PIN_HCSR04_TRIG, PIN_HCSR04_ECHO, 20, 4000);
VL53L0X vl53;


void setup() {
  Wire.begin();
  Serial.begin(115200);

  // Initialise et efface la matrice de Led - Init dot matrix display and clear it
  Serial.println("Init and clear MX Panel");
  mx.begin();
  mx.clear();

  // Initialise le capteur VL53L0X - Init VL53L0X laser sensor
  // Scan i2c bus
  Serial.println("Scan i2c");
  Serial.println(I2CScanner());
  //delay(500);

  Serial.println("Init ToF VL53L0X sensor");
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

void loop() {
  if ( COMPARE ) {

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
      COMPARE = false;
      for ( int k = 0 ; k <= 10 ; k++ ) {
        //Serial.print(k);
        
        Serial.print(filterSr04[k]); Serial.print(";");
        Serial.print(filterIr[k]); Serial.print(";");
        Serial.println(filterVl53[k]); 
      }

    }

  } 
  
  if (RAW_SIGNAL ) {
    Serial.println(analogRead(A0));
  }

  // Run tank demo if true
  IF ( THANK_DEMO ) {
    float distSR04 = hcsr04.distanceInMillimeters() + OFFSET_HCSR04;
    float distSharp = getDistanceSharp();
    float distVl53 = vl53.readRangeContinuousMillimeters();
    float levelSR04 = tankLevel(distSR04);
    float levelSharp = tankLevel(distSharp);
    float levelVl53 = tankLevel(distVl53);

    print("HR-SR04 (mm)", String(distSR04), false);
    print(" | IR (mm)", String(distSharp), false);
    print(" | VL53L0X (mm)", String(distVl53), true);

    updateDotMatrix(levelVl53, levelSR04, levelSharp);
  }
  delay(DELAY_REFRESH);
  
}

void print(String key, String val, boolean returnline ) {
  Serial.print(key); Serial.print(": ");
  if ( returnline ) {
    Serial.println(val);
  } else {
    Serial.print(val);
  }
}

// short distance prof of concept - essai d'étalonnage du Sharp IR < 200mm
// Tutoriel en français https://projetsdiy.fr/test-et-etalonnage-du-capteur-de-proximite-a02yk0-clone-asiatique-du-sharp-gp2y0a02yk0f/
float getDistanceSharp() {
  int a0 = analogRead(PIN_GP2Y0A02YK0F);
  //Serial.println(a0);
  float dist = 0.002421276045 * a0 * a0 - 3.024416502 * a0 + 114.7941861;
  if ( dist < 20 ) {
    dist = 20;
  }

  return dist;
}

void updateDotMatrix(float vl53, float sr04, float ir) {
  filterVl53[posFilter] = vl53;
  filterSr04[posFilter] = sr04;
  posFilter++;
  if ( posFilter > 10 ) {
    posFilter = 0;
  }
  countFilter++;

  if (countFilter > 10 ) {
    vl53 = average(filterVl53, POINT_FILTER);
    sr04 = average(filterSr04, POINT_FILTER);

    print("SR04(%)", String(sr04), false);
    print(" | VL53L0X(%)", String(vl53), true);

    int dotVl53 = round(vl53 / 3.125);
    int dotSr04 = round(sr04 / 3.125);

    mx.clear();
    mx.update(MD_MAX72XX::OFF);
    for ( int i = 32 ; i >= 32 - dotVl53 ; i--) {
      mx.setPoint(0, i, true);
      mx.setPoint(1, i, true);
      mx.setPoint(2, i, true);
    }
    for ( int i = 32 ; i >= 32 - dotSr04 ; i--) {
      mx.setPoint(5, i, true);
      mx.setPoint(6, i, true);
      mx.setPoint(7, i, true);
    }
    mx.update(MD_MAX72XX::ON);
  }
}

float average (int * array, int len)  // assuming array is int.
{
  long sum = 0L ;  // sum will be larger than an item, long for safety.
  for (int i = 0 ; i < len ; i++)
    sum += array [i] ;
  return  ((float) sum) / len ;  // average will be fractional, so float may be appropriate.
}

float tankLevel(float dist) {
  float tankLev = map(dist, 150, 20, 0, 100);
  //Serial.print("Tank level (%)");
  //Serial.println(tankLev);
  return tankLev;
}

String I2CScanner()
{
  byte error, address;
  int nDevices;
  String s;

  s = "Scanning:\n";

  nDevices = 0;
  for (address = 1; address < 127; address++ )
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      //s+="\n";
      s += "I2C device found at address 0x";
      if (address < 16)
        s += "0";
      s += String(address, HEX);
      s += "\n";

      nDevices++;
    }
    else if (error == 4)
    {
      s += "Unknow error at address 0x";
      if (address < 16)
        s += "0";
      s += String(address, HEX);
      s += "\n";
    }
  }
  if (nDevices == 0)
    s += "No I2C devices found\n";
  else
    s += "done\n";
  return s;
}
