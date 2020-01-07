#ifndef _MODEL_H
#define _MODEL_H

#include <stdint.h>

typedef struct {
  uint16_t pm25 = 0;
  uint16_t pm10 = 0;
  float temp = 0;
  uint16_t humidity = 0;
  uint16_t pressure = 0;
} JsonModel;

typedef struct {
  uint16_t pm25;
  uint16_t pm25rel;
  uint16_t pm10;
  uint16_t pm10rel;
  float temp;
  uint16_t humidity;
  uint16_t pressure;
  float level;
  float local_temp;
  float local_humidity;
  float local_pressure;
} DisplayModel;

typedef enum {
  AQI_ECO, LOCAL_DEVICE
} SensorType;

typedef struct {
  String sensorUrl;
  SensorType sensorType;
  int timeZoneOffset;
  int backlightTime;
  int sensorAltitude;
} Config;

#endif