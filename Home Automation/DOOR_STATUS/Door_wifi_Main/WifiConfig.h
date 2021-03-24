
#include <FS.h>
#include <WiFiManager.h>

class wificonfig {
  public:
    WiFiManager wifiManager;
    void wifiSetup() {

      wifiManager.setConfigPortalTimeout(180);

      String wifimac = WiFi.macAddress();

      char hotspot [30];
      wifimac.toCharArray(hotspot, 30);

      if (!wifiManager.autoConnect(hotspot, hotspot)) {
        delay(3000);
        ESP.deepSleep(0);
      }
    }

    void resetSetting() {
      wifiManager.resetSettings();
    }
};
wificonfig wifi;
