// --- Hardware Pin Definitions ---
const int buzzerPin = 8;        // Replace with your buzzer's digital pin
const int piezoPin = A0;        // Replace with the analog pin connected to piezo
const int threshold = 500;      // Impact detection threshold (tune as needed)

// --- SoftwareSerial for GSM & GPS ---
#include <SoftwareSerial.h>
SoftwareSerial gsmSerial(7, 8);   // RX, TX for SIM900A (adjust pins)
SoftwareSerial gpsSerial(4, 3);   // RX, TX for NEO-6M (adjust pins)

// Variables
String latitude = "";
String longitude = "";

void setup() {
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  Serial.begin(9600);       // Debug
  gsmSerial.begin(9600);    // SIM900A
  gpsSerial.begin(9600);    // NEO-6M

  delay(1000);
  initGSM();
}

void loop() {
  int impact = analogRead(piezoPin);
  Serial.print("Impact: ");
  Serial.println(impact);

  if (impact > threshold) {
    digitalWrite(buzzerPin, HIGH);

    Serial.println("🚨 Accident happened!");

    getGPSLocation();  // Get latest GPS data

    // Print location before sending SMS
    Serial.print("Location: http://maps.google.com/maps?q=");
    Serial.print(latitude);
    Serial.print(",");
    Serial.println(longitude);

    sendSMS(latitude, longitude);

    delay(10000);  // Wait before next detection
    digitalWrite(buzzerPin, LOW);
  }

  delay(200);
}

void initGSM() {
  gsmSerial.println("AT");
  delay(1000);
  gsmSerial.println("AT+CMGF=1");  // SMS text mode
  delay(1000);
}

void sendSMS(String lat, String lon) {
  gsmSerial.println("AT+CMGS=\"+91XXXXXXXXXX\"");  // Emergency number
  delay(1000);
  gsmSerial.print("🚨 Accident detected!\nLocation: ");
  gsmSerial.print("http://maps.google.com/maps?q=");
  gsmSerial.print(lat);
  gsmSerial.print(",");
  gsmSerial.print(lon);
  gsmSerial.write(26);  // CTRL+Z to send
  delay(5000);
}

// Converts NMEA format (ddmm.mmmm) to decimal degrees
float convertToDecimal(String nmeaCoord) {
  float raw = nmeaCoord.toFloat();
  int degrees = int(raw / 100);
  float minutes = raw - (degrees * 100);
  return degrees + (minutes / 60.0);
}

void getGPSLocation() {
  while (gpsSerial.available()) {
    String gpsData = gpsSerial.readStringUntil('\n');
    if (gpsData.startsWith("$GPGGA")) {
      int firstComma = gpsData.indexOf(',') + 1;
      int secondComma = gpsData.indexOf(',', firstComma);
      String rawLat = gpsData.substring(firstComma, secondComma);

      firstComma = gpsData.indexOf(',', secondComma + 1) + 1;
      secondComma = gpsData.indexOf(',', firstComma);
      String latDir = gpsData.substring(firstComma, secondComma);  // N or S

      firstComma = gpsData.indexOf(',', secondComma + 1) + 1;
      secondComma = gpsData.indexOf(',', firstComma);
      String rawLon = gpsData.substring(firstComma, secondComma);

      firstComma = gpsData.indexOf(',', secondComma + 1) + 1;
      secondComma = gpsData.indexOf(',', firstComma);
      String lonDir = gpsData.substring(firstComma, secondComma);  // E or W

      float latDecimal = convertToDecimal(rawLat);
      if (latDir == "S") latDecimal = -latDecimal;

      float lonDecimal = convertToDecimal(rawLon);
      if (lonDir == "W") lonDecimal = -lonDecimal;

      latitude = String(latDecimal, 6);
      longitude = String(lonDecimal, 6);

      Serial.print("Latitude: ");
      Serial.println(latitude);
      Serial.print("Longitude: ");
      Serial.println(longitude);
      break;
    }
  }
}