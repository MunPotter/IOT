
#include <FS.h>
#include <WiFiManager.h>

class wificonfig {
  public:
    WiFiManager wifiManager;
    
    void wifiSetup() {
      wifiManager.setBreakAfterConfig(true);

      String wifimac = WiFi.macAddress();

      char hotspot [30];
      wifimac.toCharArray(hotspot, 30);

      if (!wifiManager.autoConnect(hotspot, hotspot)) {
        delay(3000);
        ESP.restart();
        delay(1000);
      }
    }

    void resetSetting() {
      wifiManager.resetSettings();
    }
};
wificonfig wifi;
