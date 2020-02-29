/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP32 chip.

  Note: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "xUmi0vWUMEBPC4cb3jqC1Gq94eT5wNI6";

// Your WiFi credentials.
// Set password to "" for open networks.
// The EE IOT network is hidden. You might not be able to see it.
// But you should be able to connect with these credentials. 
char ssid[32] = "EE-IOT-Platform-02";
char pass[32] = "g!TyA>hR2JTy";

#define LED 0
#define HIGH 1024
#define LOW 0

const int freq = 5000;     // 5KHz frequency is more than enough. Remember we used 100 before.
const int ledChannel = 0;  // You can have up to 16 pwm channels (0 to 15)
const int resolution = 10; // 10 bit resolution for 1023. Freq should be greater than resolution.
int current_brightness = HIGH;
bool led_state = false;

BlynkTimer timer;
int time_count = 0; // timer counter global variable
String content = "";  // null string constant ( an empty string )

// Note the different pin setup function!
void setup() {
    // Serial Monitor
    Serial.begin(115200);
    Blynk.begin(auth, ssid, pass);
  
    // configure LED PWM functionality
    ledcSetup(ledChannel, freq, resolution);

    // attach the channel to the GPIO to be controlled
    ledcAttachPin(LED, ledChannel);

    // Setup a timer with function to be called every 10ms
    timer.setInterval(10L, myTimerEvent); // 10 ms interval
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}

void myTimerEvent() // Every 10 ms
{
    if (time_count % 25 == 0) { // every 250 ms
        // Do thing that needs to happen every 0.25 seconds
    }
    if (time_count == 100){
        // Do thing that needs to happen every 1 second
        time_count = 0; // reset time counter
    }
    else {
        // Send serial data to Blynk terminal
        char character;
        while(Serial.available()) { // Check if serial is available every 10 ms
            character = Serial.read();
            content.concat(character);
        }
        if (content != "") {
            Blynk.virtualWrite(3, content);//write to V3
            content = ""; // Clear String
        }  
    }
    time_count += 1; // Increment on every tick
}

// If virtual pin 2 controls fade value, 0 to 1023.
BLYNK_WRITE(V1)
{
    // param is a member variable of the Blynk ADT. It is exposed so you can read it.
    current_brightness = param.asInt(); // assigning incoming value from pin V1 to a variable
    if(led_state){
      ledcWrite(ledChannel, current_brightness);
    }
}

// This function executes whenever V1 is written to.
// You don't need to call it. Blynk handles the trigger for you.
BLYNK_WRITE(V0)
{

    // param is a member variable of the Blynk ADT. It is exposed so you can read it.
    int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable

    // Because V1 is a button, pinValue will be a 0 or a 1.
    if (pinValue == 0) {
        ledcWrite(ledChannel, LOW);
        led_state = false;
    }
    else {
        ledcWrite(ledChannel, current_brightness);
        led_state = true;
    }
}

BLYNK_READ(V2) // Widget in the app READS Virtal Pin V2 with the certain frequency
{
    // This command writes Arduino's uptime in seconds to Virtual Pin V2
    Blynk.virtualWrite(2, millis() / 1000);
}
