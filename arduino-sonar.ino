/* A simple sonar for Arduino
 *  Jeff Wilhite
 *  1/16/2021
 *  
 *  This program is meant to be used on a Arduino Uno with a sonar on pins 8 & 9 and a beeper on pin 10.
 *  It emits a series of beeps that are spaced closer together as the sonar approches an object.
 *  This is based upon the lesson at: https://www.youtube.com/watch?v=K5Aw86ROFA8
 *  The original program shown there has been re-written for clarity.
 */

#define TRIG_PIN  8     // OUTPUT pin: Setting this pin to HIGH triggers a sonar pulse
#define ECHO_PIN  9     // INPUT pin: This pin is HIGH when a pulse starts and LOW when the sonar hears an echo
#define BEEP_PIN 10     // OUTPUT pin: The speaker beeps as long as this pin is HIGH

// Uncomment this line to send debug messages back to the computer
#define DEBUG 

const int BEEP_DELAY_PER_INCH = 5;            // milliseconds of beep delay per inch of distance
const int MAX_DURATION = 30000;               // longest useful echo duration in microseconds (about 200 inches)
const int SONAR_MICROSECONDS_PER_INCH = 148;  // half the speed of sound (there and back)
const int BEEP_LENGTH = 20;                   // length of the beep tone (in milliseconds)

// The setup function is automatically run once when the program is started
void setup() 
{
    pinMode(TRIG_PIN, OUTPUT);  // Tells Arduino that we will write data to the trigger pin
    pinMode(ECHO_PIN, INPUT);   // Tells Arduino that we will read data from the echo pin
    pinMode(BEEP_PIN, OUTPUT);  // Tells Arduino that we will write data to the beep pin

// This only gets called if we are debugging
#ifdef DEBUG
    Serial.begin(9600);  // Communicate with the computer over a 9600 baud serial port
    Serial.print("Sonar started");
#endif
}

// This function is called over and over again by Arduino while the program is running
void loop() 
{
    triggerSonar();   // Send the sonar ping
  
    // The sonar sets the echoPin to HIGH when the ping is sent. 
    // Then it sets it to LOW when the echo is heard.
    // 'pulseIn' will wait until it detects a HIGH on the echoPin and then
    // returns the number of microseconds the echoPin was HIGH.
    // If it waits longer than MAX_DURATION microseconds, it gives up and returns 0.
    int echoDuration = pulseIn(ECHO_PIN, HIGH, MAX_DURATION);  

    // Only beep if the sonar detects something in its useful range
    // echoDuration is 0 if the sonar echo takes longer than MAX_DURATION microseconds.
    if (echoDuration > 0) 
    {
        // Uses the speed of sound to convert the echoDuration to inches
        int inches = echoDuration / SONAR_MICROSECONDS_PER_INCH;
        // Decides how long to delay the beeps based on how far away the sonar is detecting an object
        int beepDelay = BEEP_DELAY_PER_INCH * inches;
        printDebug(echoDuration, inches, beepDelay);

        playBeep();
        // Wait a total of beepDelay milliseconds between beeps       
        delay(beepDelay);
    }
}

void triggerSonar()
{
    digitalWrite(TRIG_PIN, HIGH); // Trigger the sonar to start sending a ping
    delayMicroseconds(10);        // Wait 10 microseconds
    digitalWrite(TRIG_PIN, LOW);  // Stop sending the sonar ping  
}

void playBeep()
{
    digitalWrite(BEEP_PIN, HIGH);
    delay(BEEP_LENGTH);
    digitalWrite(BEEP_PIN, LOW);
}

void printDebug(int echoDuration, int inches, int beepDelay)
{
#ifdef DEBUG
    // Send our measurements back to the computer for debug purposes
    Serial.print(" Duration (μs): ");
    Serial.print(echoDuration);
    Serial.print("   Distance (in): ");
    Serial.print(inches);
    Serial.print("   Beep delay (ms): ");
    Serial.print(beepDelay);
    Serial.print("\r\n");   // skips to the next line
#endif
}
