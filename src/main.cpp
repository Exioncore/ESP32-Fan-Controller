#include <Arduino.h>
#include <EEPROM.h>
#include <vector>

#include "Manager/FanManager.h"
#include "Manager/SensorManager.h"
#include "Manager/StorageManager.h"
#include "Network/CmdProcessor.h"
#include "Util/SerialHandler.h"
#include "Settings.h"

using namespace std;

#define ESP_LIGHT_SLEEP_TIME_MS     100
unsigned long lastUpdate = 0;

// Serial
#define SERIAL_BUFFER_SIZE  150
SerialHandler serial_handler;
char serial_buffer[SERIAL_BUFFER_SIZE];

// Managers
FanManager* fan_manager;
SensorManager* sensor_manager;
StorageManager* storage_manager;
CmdProcessor* cmd_processor;

void setup() {
  // General initialization
  setCpuFrequencyMhz(240);
  fan_manager = FanManager::getInstance();
  sensor_manager = SensorManager::getInstance();
  storage_manager = StorageManager::getInstance();
  cmd_processor = CmdProcessor::getInstance();
  // Initialize Serial
  Serial.begin(500000);
  serial_handler = SerialHandler(&Serial, serial_buffer, SERIAL_BUFFER_SIZE, '\n');
  // Initialize Fan Manager
  fan_manager->setup(fan_pwm_pin, fan_rpm_pin, 5);
  fan_manager->setEEPROMaddr(0);
  storage_manager->addSize(fan_manager->sizeOnEEPROM());
  // Initialize Sensor Manager
  sensor_manager->setup(thermistor_power, 
    thermistor_pin[0], thermistor_resistor[0],
    thermistor_pin[1], thermistor_resistor[1],
    thermistor_pin[2], thermistor_resistor[2],
    thermistor_pin[0], thermistor_pin[2]
  );
  sensor_manager->setEEPROMaddr(storage_manager->getLastAddr());
  storage_manager->addSize(sensor_manager->sizeOnEEPROM());
  // Initialize Storage
  storage_manager->initializeEEPROM();
  delay(500);
  fan_manager->readFromEEPROM();
  sensor_manager->readFromEEPROM();
}
  

void loop() {
  // Read SerialPort & Process commands
  if (serial_handler.update() != 0) {
    char* response = (char*)(cmd_processor->process(serial_handler.buffer));
    Serial.println(response);

    delete response;
    serial_handler.discardBuffer();
  }

  if (millis() - lastUpdate >= ESP_LIGHT_SLEEP_TIME_MS) {
    // Update Sensors
    sensor_manager->update();
    // Update Fan Manager
    fan_manager->update(
      vector<double>{
      sensor_manager->get_sensor(SensorManager::Ambient)->get_temperature(),
      sensor_manager->get_sensor(SensorManager::T1)->get_temperature(),
      sensor_manager->get_sensor(SensorManager::T2)->get_temperature(),
      sensor_manager->get_sensor(SensorManager::CPU)->get_temperature(),
      sensor_manager->get_sensor(SensorManager::GPU)->get_temperature()}
    );

    // Update Timing
    lastUpdate = millis();
  }
}