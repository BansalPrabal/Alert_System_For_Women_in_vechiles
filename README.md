# 🚨 Alert System for Women in Vehicles

An embedded safety system using Arduino to provide real-time GPS location and emergency SMS alerts.

## 📦 Features
- Emergency button triggers location tracking and SMS alert
- Sends GPS coordinates via GSM module (SIM800L)
- Displays status on 16x2 LCD
- Sounds buzzer to alert nearby people
- Remote deactivation via SMS "STOP"

## 🧰 Hardware Used
- Arduino Uno
- GPS Module (NEO-6M)
- GSM Module (SIM800L)
- Buzzer
- 16x2 LCD Display
- Emergency Push Button
- Breadboard, jumper wires

## 💻 Software
- Developed in Arduino IDE using Embedded C
- Uses SoftwareSerial for GPS communication
- Uses AT commands to control GSM for SMS

## 🧪 Testing
- Validated GPS fix, SMS delivery, LCD output, and remote control
- Successfully received emergency SMS on phone with live coordinates

## 🔋 Future Enhancements
- Battery-powered version
- Android app integration
- Live tracking via IoT
