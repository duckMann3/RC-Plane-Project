#include <Arduino.h>

// **** MODULE TESTS: ****
// #define LED_TEST
// #define SERVO_TEST
// #define ADC_TEST
#define SPI_RECEIVE
// #define SPI_TRANSMIT
// #define UART_TEST
// **** RECEIVER/TRANSMITTER ****
// #define TRANSMITTER
#define RECEIVER
#if defined(LED_TEST) 
  #define LED_PIN PC13
#endif

#if defined(RECEIVER) // RC PLane
  #if defined(SERVO_TEST)
    #include <Servo.h>
    Servo myServo;
    int servoPin = PA0; // PA3 => IN3
  #endif

  #if defined(SPI_RECEIVE)
    /* SPI/NRF Libraries: */
    // NRF.h or nRF24L01.h (???)
    // SPI.h
    #include <SPI.h>
    #include <nRF24L01.h>
    #include <RF24.h>

    #define CSN  PB12 // Chip Select Not (CSN)
    #define CE   PB13 // Chip Enable
    #define SCLK  PB10 // Serial Clock
    #define MISO PB14 // Master In-Slave Out
    #define MOSI PB15 // Master Out-Slave In   

    // SPIClass etx_spi(MOSI, MISO, SCLK);
    RF24 radio(CE, CSN);
    // RF24 radio(PB13, PB12);
    // RF24 radio(PA3, PB12);
    const byte address[6] = "00001";
  #endif

#endif

#if defined(TRANSMITTER) // Remote Controller
  #if defined(ADC_TEST)
    int led_in  = PA5;
    int led_out = PC13; 
    int joyPin1 = PA3;
    int joyPin2 = PA4;
    int val1 = 0, val2 = 0;
    int treatValue(int data);
  #endif

  #if defined(SPI_TRANSMIT)
    /* SPI/NRF Libraries: */
    // NRF.h or nRF24L01.h (???)
    // SPI.h
    #include <SPI.h>
    #include <nRF24L01.h>
    #include <RF24.h>

    #define CSN  PB12 // Chip Select Not (CSN)
    #define CE   PB13 // Chip Enable
    #define SCK  PB10 // Serial Clock
    #define MISO PB14 // Master In-Slave Out
    #define MOSI PB15 // Master Out-Slave In

    RF24 radio(CE, CSN);

    const byte address[6] = "00001";

  #endif

#endif

void setup() {
  #if defined(LED_TEST) 
    pinMode(LED_PIN, OUTPUT);  
  #endif

  #if defined(SERVO_TEST)
    myServo.attach(servoPin);
  #endif

  #if defined(ADC_TEST)
    Serial.begin(115200);
    while(!Serial) {
      delay(10);
    }
    Serial.println("USB Serial initialized successfully on Black Pill");

    pinMode(led_in, INPUT_PULLUP);      // Detect Joystick SW press
    pinMode(led_out, OUTPUT); // Output to on board LED
  #endif

  #if defined(UART_TEST) 
    Serial.begin(115200);
  #endif

  #if defined(SPI_TRANSMIT)
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening();
  #endif 

  #if defined(SPI_RECEIVE)
    Serial.begin(115200);
    while(!Serial);
    Serial.println("Serial Established.");
    SPI.begin();
    Serial.println("SPI Established.");
    radio.begin();
    Serial.println("Radio Established.");
    // etx_spi.setSCLK(SCLK);
    // etx_spi.setMOSI(MOSI);
    // etx_spi.setMISO(MISO);
    // etx_spi.begin();
    // pinMode(CSN, OUTPUT);
    // digitalWrite(CSN, HIGH);
    // Serial.println("SPI Established.");
    // if(!radio.begin()) {
    //   Serial.println("nRF24L01 hardware is not responding");
    //   while(1);
    // }
    // radio.openReadingPipe(0, address);
    // radio.setPALevel(RF24_PA_MIN);
    // radio.startListening();
  #endif
}

void loop() {
  #if defined(LED_TEST) 
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    delay(1000);
  #endif

  #if defined(SERVO_TEST)
    for(int pos = 0; pos <= 180; pos += 1) {
      myServo.write(pos); // Turn to 0 degrees
      delay(15);
    }
    for(int pos = 180; pos >= 0; pos -= 1) {
      myServo.write(pos); // Turn to 0 degrees
      delay(15);
    }
  #endif

  #if defined(ADC_TEST)
    bool SW = digitalRead(led_in);
    if(SW == LOW) {
        digitalWrite(led_out, HIGH);
        delay(25);
    } else {
        digitalWrite(led_out, LOW);
        delay(25);
    }
    val1 = analogRead(joyPin1);
    delay(100);
    val2 = analogRead(joyPin2);
    Serial.print('J');
    Serial.print(treatValue(val1));
    Serial.print(treatValue(val2));
  #endif

  #if defined(UART_TEST)
    Serial.println("MOW");
  #endif

  #if defined(SPI_TRANSMIT)
    const char text[] = "Hello World";
    radio.write(&text, sizeof(text));
    delay(1000);
  #endif

  #if defined(SPI_RECEIVE)
    Serial.println("Works");
    delay(1000);
    // if(radio.available()) {
    //   char text[32] = "";
    //   radio.read(&text, sizeof(text));
    //   Serial.println(text);
    // }
  #endif
}

#if defined(ADC_TEST)
  int treatValue(int data) { 
    return (data * 9 / 1024) + 48;
  }
#endif