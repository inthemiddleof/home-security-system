#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);

Servo myServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte card1[4] = {0x19, 0x1E, 0xA2, 0x04};
byte card2[4] = {0x87, 0x63, 0x6C, 0x05};

const int pirPin = 3;
const int vibPin = 4;
const int buzzer = 2;
const int ldrPin = A0;
const int lampu = 6;

bool pintuTerbuka = false;

String lastBaris1 = "";
String lastBaris2 = "";

void setup()
{
    Serial.begin(9600);
    SPI.begin();
    rfid.PCD_Init();

    lcd.init();
    lcd.backlight();
    tampilkanLCD("Smart Home", "Ready");

    myServo.attach(5);
    myServo.write(0);

    pinMode(pirPin, INPUT);
    pinMode(vibPin, INPUT);
    pinMode(buzzer, OUTPUT);
    pinMode(lampu, OUTPUT);

    digitalWrite(buzzer, LOW);
    digitalWrite(lampu, LOW);
}

void loop()
{

    int ldrValue = analogRead(ldrPin);
    if (ldrValue < 500)
    {
        digitalWrite(lampu, LOW);
    }
    else
    {
        digitalWrite(lampu, HIGH);
    }
}

if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial())
{
    if (checkUID(rfid.uid.uidByte, card1))
    {
        tampilkanLCD("User Dikenal", "Hanif Dhiah");
        delay(1000);
        toggleServo();
    }
    else if (checkUID(rfid.uid.uidByte, card2))
    {
        tampilkanLCD("User Dikenal", "Rama");
        delay(1000);
        toggleServo();
    }
    else
    {
        tampilkanLCD("Kartu Tidak", "Dikenal!");
    }

    delay(2000);
    tampilkanLCD("Smart Home", "Ready");
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
}

if (!pintuTerbuka)
{
    if (digitalRead(pirPin) == HIGH)
    {
        if (digitalRead(vibPin) == HIGH)
        {
            tampilkanLCD("Getaran & Gerakan", "Terdeteksi!");
            bunyiAlarm();
        }
        else
        {
            tampilkanLCD("Gerakan", "Terdeteksi");
            noTone(buzzer);
        }
    }
    else
    {
        tampilkanLCD("Smart Home", "Ready");
        noTone(buzzer);
    }
}
}

bool checkUID(byte *a, byte *b)
{
    for (byte i = 0; i < 4; i++)
    {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

void toggleServo()
{
    if (!pintuTerbuka)
    {
        myServo.write(90);
        pintuTerbuka = true;
        tampilkanLCD("Pintu", "Terbuka");
    }
    else
    {
        myServo.write(0);
        pintuTerbuka = false;
        tampilkanLCD("Pintu", "Terkunci");
    }
    delay(2000);
    tampilkanLCD("Smart Home", "Ready");
}

void tampilkanLCD(String baris1, String baris2)
{
    if (baris1 != lastBaris1 || baris2 != lastBaris2)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(baris1);
        lcd.setCursor(0, 1);
        lcd.print(baris2);
        lastBaris1 = baris1;
        lastBaris2 = baris2;
    }
}

void bunyiAlarm()
{
    int nada[] = {1000, 500, 1000, 1000, 500, 1000};
    for (int i = 0; i < 6; i++)
    {
        tone(buzzer, nada[i]);
        delay(500);
    }
    noTone(buzzer);
}