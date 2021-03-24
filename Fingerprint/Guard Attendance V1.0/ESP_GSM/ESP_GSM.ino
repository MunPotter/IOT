#include "WifiConfig.h"
#include "GsmConfig.h"
void setup() {

  wifi.wifiSetup();

}

void loop() {

  gsmc.gsmSetup();
  gsmc.gsmSend("data");
}
