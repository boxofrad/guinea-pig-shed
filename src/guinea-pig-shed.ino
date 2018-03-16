/*

D3 - Ground (Black)
D4 - Signal (White)
D5 - Power (Red)

4.7K Ohm resistor between D4 and D5.

*/

#include <OneWire.h>
#include <spark-dallas-temperature.h>

#define SENSOR_RESOLUTION 10
#define READING_INTERVAL 60000

OneWire wire(D4);
DallasTemperature sensors(&wire);

DeviceAddress mansion = { 0x28, 0x70, 0xB4, 0x61, 0x9, 0x0, 0x0, 0x47 };
DeviceAddress annex = { 0x28, 0x17, 0xFD, 0x62, 0x9, 0x0, 0x0, 0x93 };

double mansionTemp, annexTemp;

void setup() {
  Serial.begin(9600);

  Particle.variable("mansionTemp", &mansionTemp, DOUBLE);
  Particle.variable("annexTemp", &annexTemp, DOUBLE);

  pinMode(D3, OUTPUT);
  pinMode(D5, OUTPUT);

  digitalWrite(D3, LOW);
  digitalWrite(D5, HIGH);

  sensors.begin();
  sensors.setResolution(mansion, SENSOR_RESOLUTION);
  sensors.setResolution(annex, SENSOR_RESOLUTION);
}

void loop() {
    sensors.requestTemperatures();

    mansionTemp = sensors.getTempC(mansion);
    if (readError(mansionTemp)) {
        Serial.println("Error reading mansion temperature");
    }

    annexTemp = sensors.getTempC(annex);
    if (readError(annexTemp)) {
        Serial.println("Error reading annex temperature");
    }

    Serial.printf("Mansion: %f C, Annex: %f C\n", mansionTemp, annexTemp);

    Particle.publish("temperature_reading", "{\"mansion\":" + String(mansionTemp) + ",\"annex\":" + String(annexTemp) + "}");

    delay(READING_INTERVAL);
}

bool readError(float temp) {
    return temp == -127.00 || temp == 85.0;
}
