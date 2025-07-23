#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <string.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial gps(10, 11);

char *gpsRefString = "$GPGGA";
String gpsRespString = "";
String latitude = "No Range        ";
String longitude = "No Range        ";
int gpsRespCharCnt = 0;
Boolean gps_status = 0;
int isGsmCmdReceived = 0;

const int buttonPin = 7;
bool btnState = HIGH;
#define BUZZER_PIN 13

void setup() {
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
    lcd.init();
    lcd.begin(16, 2);
    lcd.backlight();
    Serial.begin(9600);
    gps.begin(9600);
    lcd.print("Alert System for");
    lcd.setCursor(0, 1);
    lcd.print("Women in Automob:");
    delay(1000);
    gsm_init();
    lcd.clear();
    lcd.print("GPS Initializing");
    lcd.setCursor(0, 1);
    get_gps();
    delay(1000);
    lcd.clear();
    lcd.print("GPS Range Found");
    lcd.setCursor(0, 1);
    lcd.print("GPS is Ready");
    delay(1000);
    lcd.clear();
    lcd.print("System Ready");
    isGsmCmdReceived = 0;
}

void loop() {
    get_gps();
    btnState = !digitalRead(buttonPin);
    if (btnState) {
        btnState = 0;
        tracking(1);
    }
    serialEvent();
    if (isGsmCmdReceived == 0) {
        digitalWrite(BUZZER_PIN, LOW);
    }
}

void serialEvent() {
    while (Serial.available()) {
        if (Serial.find("STOP")) {
            isGsmCmdReceived = 0;
        }
    }
}

void gpsEvent() {
    gpsRespString = "";
    while (1) {
        while (gps.available() > 0) {
            char inChar = (char)gps.read();
            gpsRespString += inChar;
            gpsRespCharCnt++;
            if (gpsRespCharCnt < 7) {
                if (gpsRespString[gpsRespCharCnt - 1] != gpsRefString[gpsRespCharCnt - 1]) {
                    gpsRespCharCnt = 0;
                    gpsRespString = "";
                }
            }
            if (inChar == '') {
                if (gpsRespCharCnt > 65) {
                    gps_status = 1;
                    break;
                } else {
                    gpsRespCharCnt = 0;
                }
            }
        }
        if (gps_status)
            break;
    }
}

void gsm_init() {
    lcd.clear();
    lcd.print("Finding Module...");
    Boolean at_flag = 1;
    while (at_flag) {
        Serial.println("AT");
        while (Serial.available() > 0) {
            if (Serial.find("OK"))
                at_flag = 0;
        }
        delay(1000);
    }
    lcd.clear();
    lcd.print("Module Connected...");
    delay(1000);

    Boolean echo_flag = 1;
    while (echo_flag) {
        Serial.println("ATE0");
        while (Serial.available() > 0) {
            if (Serial.find("OK"))
                echo_flag = 0;
        }
        delay(1000);
    }

    lcd.clear();
    lcd.print("Finding Network...");
    Boolean at_cmgf_flag = 1;
    while (at_cmgf_flag) {
        Serial.println("AT+CMGF=1");
        while (Serial.available() > 0) {
            if (Serial.find("OK"))
                at_cmgf_flag = 0;
        }
        delay(1000);
    }

    Boolean at_cnmi_flag = 1;
    while (at_cnmi_flag) {
        Serial.println("AT+CNMI=2,2,0,0,0");
        while (Serial.available() > 0) {
            if (Serial.find("OK"))
                at_cnmi_flag = 0;
        }
        delay(1000);
    }

    Boolean at_csmp_flag = 1;
    while (at_csmp_flag) {
        Serial.println("AT+CSMP=17,167,0,0");
        while (Serial.available() > 0) {
            if (Serial.find("OK"))
                at_csmp_flag = 0;
        }
        delay(1000);
    }

    Boolean net_flag = 1;
    while (net_flag) {
        Serial.println("AT+CPIN?");
        while (Serial.available() > 0) {
            if (Serial.find("+CPIN: READY"))
                net_flag = 0;
        }
        delay(1000);
    }

    lcd.clear();
    lcd.print("Network Found...");
    delay(1000);
    lcd.clear();
}

void get_gps() {
    gps_status = 0;
    int x = 0;
    while (gps_status == 0) {
        gpsEvent();
        int str_length = gpsRespCharCnt;
        latitude = "";
        longitude = "";
        int comma = 0;
        while (x < str_length) {
            if (gpsRespString[x] == ',')
                comma++;
            if (comma == 2)
                latitude += gpsRespString[x + 1];
            else if (comma == 4)
                longitude += gpsRespString[x + 1];
            x++;
        }
        int l1 = latitude.length();
        latitude[l1 - 1] = ' ';
        l1 = longitude.length();
        longitude[l1 - 1] = ',';
        lcd.clear();
        lcd.print("Lat:");
        lcd.print(latitude);
        lcd.setCursor(0, 1);
        lcd.print("Long:");
        lcd.print(longitude);
        gpsRespCharCnt = 0;
        x = 0;
        str_length = 0;
        delay(1000);
    }
}

void init_sms(int mob_cnt) {
    Serial.println("AT+CMGF=1");
    delay(1000);
    if (mob_cnt == 1) {
        Serial.println("AT+CMGS="+918329417147"");
        delay(1000);
    }
}

void send_data(String message) {
    Serial.println(message);
    delay(200);
}

void send_sms() {
    Serial.write((char)26);
    Serial.write(26);
}

void lcd_status() {
    lcd.clear();
    lcd.print("Message Sent");
    delay(1000);
    lcd.clear();
    lcd.print("System Ready");
}

void tracking(int mob_cnt) {
    digitalWrite(BUZZER_PIN, HIGH);
    init_sms(mob_cnt);
    Serial.println("Emergency! lat: long:");
    Serial.print(latitude);
    Serial.println(longitude);
    send_sms();
    delay(200);
    lcd_status();
}