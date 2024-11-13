#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>


#define SS_PIN 10
#define RST_PIN 9
String UID = "D3 66 5F FA";
String UID2 = "B3 39 82 34";
byte lock = 0;

Servo servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  servo.write(70);
  lcd.init();
  lcd.backlight();
  servo.attach(3);
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("Bluetooth Iniciado");
}

void loop() {
  lcd.setCursor(3, 0);
  lcd.print("Bem vindo!");
  lcd.setCursor(0, 1);
  lcd.print("Aproxime sua tag");

  if (Serial.available() > 0) {
    char btCommand = Serial.read();
    if (btCommand == 'o') { // Comando para abrir a fechadura
      abrirFechadura();
    } else if (btCommand == 'c') { // Comando para fechar a fechadura
      fecharFechadura();
    }
  }

  if (!rfid.PICC_IsNewCardPresent())
    return;
  if (!rfid.PICC_ReadCardSerial())
    return;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Lendo.....");
  Serial.print("NUID tag is :");
  String ID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    lcd.print(".");
    ID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    ID.concat(String(rfid.uid.uidByte[i], HEX));
    delay(300);
  }
  ID.toUpperCase();

  if ((ID.substring(1) == UID || ID.substring(1) == UID2) && lock == 0) {
    fecharFechadura();
  } else if ((ID.substring(1) == UID || ID.substring(1) == UID2) && lock == 1) {
    abrirFechadura();
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tag Errada!");
    delay(1500);
    lcd.clear();
  }
}

void abrirFechadura() {
  servo.write(160);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Porta Aberta");
  Serial.println("Porta Aberta");
  delay(1500);
  lcd.clear();
  lock = 0;
}

void fecharFechadura() {
  servo.write(70);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Porta Fechada");
  Serial.println("Porta Fechada");
  delay(1500);
  lcd.clear();
  lock = 1;
}
