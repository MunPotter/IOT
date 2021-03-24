#include <ArduinoJson.h>

class JsonData{
  public:
  String jsonDataString(String d){
      String value = "{\"SensorData\":\"" + d + "\"}";
    return value;
  }
};
JsonData jsonData;
