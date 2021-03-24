#include <FS.h>
#include <WiFiManager.h>
#include "Display.h"
class wificonfig {
  public:
    WiFiManager wifiManager;
    void wifiSetup() {

      wifiManager.setConfigPortalTimeout(180);
      String wifimac = WiFi.macAddress();
      //      wifiManager.resetSettings();
      if (WiFi.status() != WL_CONNECTED) {
        dis.DisplayConfig();
        dis.TextDisplay("Please,");
        dis.TextDisplay("connect");
        dis.TextDisplay("wifi");
        dis.FinishDisplay();
      }
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
