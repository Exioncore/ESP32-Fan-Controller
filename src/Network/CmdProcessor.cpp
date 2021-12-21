#include "Network/CmdProcessor.h"
#include "Manager/SensorManager.h"
#include "Manager/FanManager.h"
#include "Manager/StorageManager.h"
#include "Fan/Algorithms/Curve.h"

#define BUFFER_MAX_LEN 50
#define RESPONSE_MAX_LEN 200

#define FAN_PREFIX 'F'
#define SENSOR_PREFIX 'S'

// Allocating the pointer to the static instance
CmdProcessor *CmdProcessor::instance = 0;

/**
 * Empty Constructor due to Singleton Object
 */
CmdProcessor::CmdProcessor()
{
}

/**
 * This method either retrieves the existing instance or creates a new one if it
 * does not exist.
 */
CmdProcessor *CmdProcessor::getInstance()
{
    if (!instance)
    {
        instance = new CmdProcessor();
    }
    return instance;
}

/**
 * Process a command and return a response
 * @result response
 */
char *CmdProcessor::process(char *cmd)
{
    char *response = new char[RESPONSE_MAX_LEN];

    if (strlen(cmd) > 0)
    {
        char *buff = new char[BUFFER_MAX_LEN];
        StringProcessor stringProcessor = StringProcessor();
        stringProcessor.setSeparators(',', '\n');
        stringProcessor.setInput(cmd + 2, strlen(cmd + 2));
        switch (cmd[0])
        {
        case FAN_PREFIX:
            processFan(cmd + 1, response, buff, stringProcessor);
            break;
        case SENSOR_PREFIX:
            processSensor(cmd + 1, response, buff, stringProcessor);
            break;
        default:
            sprintf(response, "Invalid Command");
            break;
        }
        delete buff;
        return response;
    }
    sprintf(response, "Empty command");
    return response;
}

/**
 * Process Fan Commands
 * No mallformed input checking for performance sake
 */
void CmdProcessor::processFan(char *cmd, char *response, char *buff, StringProcessor stringProcessor)
{
    FanManager *fanManager = FanManager::getInstance();

    // Process command
    switch (cmd[0])
    {
    case 'P': // Set Profile
    {
        stringProcessor.next(buff, BUFFER_MAX_LEN);
        uint8_t i = atoi(buff);
        fanManager->set_profile(i);
        sprintf(response, "OK");
        break;
    }
    case 'p': // Get Profile
    {
        sprintf(response, "%d", fanManager->get_profile());
        break;
    }
    case 'D': // Set Duty
    {
        stringProcessor.next(buff, BUFFER_MAX_LEN);
        uint8_t i = atoi(buff);
        stringProcessor.next(buff, BUFFER_MAX_LEN);
        int8_t duty = atoi(buff);
        fanManager->get_fan(i)->set_duty(duty);
        fanManager->get_fan(i)->writeToEEPROM();
        EEPROM.commit();
        sprintf(response, "OK");
        break;
    }
    case 'C': // Set Curve
    {
        stringProcessor.next(buff, BUFFER_MAX_LEN);
        uint8_t fan = atoi(buff);
        stringProcessor.next(buff, BUFFER_MAX_LEN);
        uint8_t sensor = atoi(buff);
        Curve *algorithm = (Curve *)fanManager->get_fan(fan)->get_algorithm(sensor);
        for (uint8_t i = 0; i < algorithm->get_number_of_points(); i++)
        {
            stringProcessor.next(buff, BUFFER_MAX_LEN);
            uint8_t t = atoi(buff);
            stringProcessor.next(buff, BUFFER_MAX_LEN);
            uint8_t d = atoi(buff);
            algorithm->set_point(Point_i8(t, d), i);
        }
        stringProcessor.next(buff, BUFFER_MAX_LEN);
        uint8_t ambient_offset = atoi(buff);
        algorithm->set_ambient_reference(ambient_offset);
        fanManager->get_fan(fan)->writeToEEPROM();
        EEPROM.commit();
        sprintf(response, "OK");
        break;
    }
    case 'c': // Get Curve
    {
        uint8_t len = 0;
        stringProcessor.next(buff, BUFFER_MAX_LEN);
        uint8_t fan = atoi(buff);
        stringProcessor.next(buff, BUFFER_MAX_LEN);
        uint8_t sensor = atoi(buff);
        Curve *algorithm = (Curve *)fanManager->get_fan(fan)->get_algorithm(sensor);
        for (uint8_t i = 0; i < algorithm->get_number_of_points(); i++)
        {
            if (i > 0)
            {
                response[len] = ',';
                len++;
            }
            len += sprintf(response + len, "%d,%u", algorithm->get_points()[i].x, algorithm->get_points()[i].y);
        }
        sprintf(response + len, ",%d", algorithm->is_using_ambient_reference());
        break;
    }
    case 'S': // Set Enabled Sensors
    {
        stringProcessor.next(buff, BUFFER_MAX_LEN);
        uint8_t fan = atoi(buff);
        stringProcessor.next(buff, BUFFER_MAX_LEN);
        uint8_t sensors = atoi(buff);
        fanManager->get_fan(fan)->set_enabled_sensors(sensors);
        fanManager->get_fan(fan)->writeToEEPROM();
        EEPROM.commit();
        sprintf(response, "OK");
        break;
    }
    case 's': // Get Enabled Sensors
    {
        stringProcessor.next(buff, BUFFER_MAX_LEN);
        uint8_t fan = atoi(buff);
        sprintf(response, "%d", fanManager->get_fan(fan)->get_enabled_sensors());
        break;
    }
    default:
    {
        sprintf(response, "Invalid Fan Command");
        break;
    }
    }
}
/**
 * Process Sensor Commands
 * No mallformed input checking for performance for sake
 */
void CmdProcessor::processSensor(char *cmd, char *response, char *buff, StringProcessor stringProcessor)
{
    SensorManager *sensorManager = SensorManager::getInstance();

    // Process command
    switch (cmd[0])
    {
    case 'C': // Calibrate ADC
    {
        sensorManager->calibrate_adc(true);
        sprintf(response, "OK");
        break;
    }
    case 'T': // Get sensor reading
    {
        stringProcessor.next(buff, BUFFER_MAX_LEN);
        int n = atoi(buff);
        float temperature = sensorManager->get_sensor((SensorManager::SensorSource)n)->get_temperature();
        sprintf(response, "%.2f", temperature);
        break;
    }
    case 'r': // Ryder Display
    {
        sprintf(response, "%.2f,%.2f",
                sensorManager->get_sensor((SensorManager::SensorSource)0)->get_temperature(),
                sensorManager->get_sensor((SensorManager::SensorSource)1)->get_temperature());
        break;
    }
    default:
    {
        sprintf(response, "Invalid Sensor Command");
        break;
    }
    }
}