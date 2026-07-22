#include <Arduino.h>

// **** MODULE TESTS: ****
// #define LED_TEST
#define SERVO_TEST
#define ADC_TEST
// #define SPI_TRANSMIT
// #define SPI_RECEIVE
// **** RECEIVER/TRANSMITTER ****
#define RECEIVER
#define TRANSMITTER
#define LED_PIN PC13

#if defined(RECEIVER) // RC PLane
  #if defined(SERVO_TEST)
    #include <Servo.h>
    Servo myServo;
    int servoPin = PA0; // PA3 => IN3
  #endif

  #if defined(SPI_RECEIVE)
  #endif

#endif

#if defined(TRANSMITTER) // Remote Controller
  #if defined(ADC_TEST)
    int ledPin = PC13;
    int joyPin1 = PA3;
    int joyPin2 = PA4;
    int val1 = 0, val2 = 0;
    int treatValue(int data);
  #endif

  #if defined(SPI_TRANSMIT)

  #endif

#endif


void setup() {
  pinMode(LED_PIN, OUTPUT);  

  #if defined(SERVO_TEST)
    myServo.attach(servoPin);
  #endif

  #if defined(ADC_TEST)
    pinMode(ledPin, OUTPUT);
    Serial.begin(9600);
  #endif

  #if defined(SPI_RECEIVE)
  #endif

  #if defined(SPI_TRANSMIT)
  #endif
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  delay(1000);

  #if defined(SERVO_TEST)
    myServo.write(0); // Turn to 0 degrees
    delay(1000);
    myServo.write(180); // Turn to 180 degrees
    delay(1000);
  #endif

  #if defined(ADC_TEST)
    val1 = analogRead(joyPin1);
    delay(100);
    val2 = analogRead(joyPin2j);
    digitalWrite(ledPin, HIGH);
    delay(val1);
    digitalWrite(ledPin, LOW);
    delay(val2);
    Serial.print('J')
    Serial.print(treatValue(val1));
    Serial.print(treatValue(val2));

  #endif

  #if defined(SPI_RECEIVE)
  #endif

  #if defined(SPI_TRANSMIT)
  #endif
}

#if defined(ADC_TEST)
  int treatValue(int data) { 
    return (data * 9 / 1024) + 48;
  }
#endif