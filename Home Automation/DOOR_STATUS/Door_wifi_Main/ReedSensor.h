
class reedSensor {

  public:
    int sensor_status;

    String reedData(int Pin ) {
      String door_data;
      sensor_status = analogRead(Pin);
      if (sensor_status == 0) {
        door_data = "close";
      }
      else {
        door_data = "open";
      }
      return door_data;
    }
};

reedSensor reed;
