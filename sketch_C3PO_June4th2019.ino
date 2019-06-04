/**
 * C^3PO - Corner Crash Corrector for People Obtuse
 * 
 * Github: https://github.com/ntoenis/C-3PO/
 * Version: 0.1.0
 *
 * This sketch uses the NewPing library which you can get at: http://playground.arduino.cc/Code/NewPing
 */
 
#include <NewPing.h>

#define MAX_DISTANCE 180 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm. 180~=5.9ft
#define TRIGGER_PIN1  0  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define TRIGGER_PIN2  0  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN1     1  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define ECHO_PIN2     1  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define PIN_OUT      2  // Go low to activate MP3 module
#define PLAY_TIME    15 // Number of seconds it will allow an mp3 file to play for without possible interruption

unsigned int distance1;
unsigned int distance2;
unsigned int previous_distance1;
unsigned int previous_distance2;
unsigned int threshold;
unsigned int low_threshold;
unsigned int high_threshold;

NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

//If distance of one side is High start checking the other side. 

void play(){
  //Serial.println("play"); //Debug print, can be removed.
  digitalWrite(PIN_OUT, LOW);
  delay(100);
  digitalWrite(PIN_OUT, HIGH);
  delay(PLAY_TIME * 1000);
}

boolean check_distance(int x, int y){ //x=distance, y=previous_distance
  //Serial.println("check_distance"); //Debug print, can be removed.
  if(x == 5 || x == 0){ return false; } // Seem to be getting a lot of invalid values from the sensor/library ignore
  
  threshold = (y / 5) + 1; // Calculate what 25% of the previous distance is
  low_threshold = y - threshold; // 25% lower than previous_distance
  high_threshold = y + threshold; // 25% higher than previous_distance

  if(low_threshold < 0) { low_threshold = 0; } // Ensure the low_threshold does not go negative
  if(high_threshold > MAX_DISTANCE) { high_threshold = MAX_DISTANCE; } // Ensure we are activating only up to the MAX_DISTANCE value

  if(x <= low_threshold || x > high_threshold){ 
    // If the new distance is more than 25% lower or higher than the previous distance, activate
    return true;
  }
  else{
    return false;
  }
}

void setup() {
  //Serial.begin(9600);
  pinMode(PIN_OUT, OUTPUT);
  digitalWrite(PIN_OUT, HIGH);
  delay(3000); // allow things to stabilize, incase the mp3 module needs time
  play(); //trigger the mp3 once on startup just to test everything
  previous_distance1 = sonar1.convert_cm(sonar1.ping_median(40)); // Set the previous distance
}

void loop() {
  distance1 = sonar1.convert_cm(sonar1.ping_median(15)); // Take 15 readings and average them
  delay(50);
  distance2 = sonar2.convert_cm(sonar2.ping_median(15)); // Take 15 readings and average them

  if(check_distance(distance1, previous_distance1) && check_distance(distance2, previous_distance2)){
    previous_distance1 = distance1;
    previous_distance2 = distance2;
    play();
  }
  else{delay(100);}
  
  //Serial.println(distance1 && distance2); //Debug print, can be removed.
}
