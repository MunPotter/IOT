#include <Arduino_FreeRTOS.h>

#include <SoftwareSerial.h>

SoftwareSerial espSerial(6, 7); //Software serial for gsm module  //RX TX
SoftwareSerial gsmSerial(2, 3); //A4,A5  RX, TX ports

String finalSoftValue;
String finalSoftValues;
String finalSoftValue2;
String softdata1;
String sdata1;
String sdata2;
int count = 0;

TaskHandle_t Task1_Handler;
TaskHandle_t Task2_Handler;
TaskHandle_t Task3_Handler;

void Task1(void *pvParameters);
void Task2(void* pvParameters);
void Task3(void* pvParameters);

void setup() {
  Serial.begin(9600);
//  espSerial.begin(9600);
//  gsmSerial.begin(115200);
  xTaskCreate(Task1, "Serial1", 128, NULL, 2, &Task1_Handler);
//  xTaskCreate(Task2, "stop_task", 128, NULL, 0, &Task2_Handler);
  xTaskCreate(Task3, "Serial2", 128, NULL, 2, &Task3_Handler);
}


void loop()
{
}

//string split function
String split(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}



void Task1(void *pvParameters)
{
  (void) pvParameters;
  for (;;)
  {
//    if (count > 100)count = 0;

    while (Serial.available() > 0) {
      delay(2);
      char SoftValue = (char)Serial.read();
      finalSoftValue += SoftValue;
    }

    if (finalSoftValue == " stm") {
      delay(100);
      //      Serial.println(finalSoftValue);
      String softValue1 = "";
      softValue1 += "@";
      softValue1 += "uno bhai ";
      //      softValue1 += count;
      softValue1 += " ";
      int softValueLength1 = softValue1.length();
      char finalSoftValue1[softValueLength1 + 1];
      strcpy(finalSoftValue1, softValue1.c_str());
      //      gsmSerial.write(finalSoftValue1);
//      count++;
      Serial.println(finalSoftValue1);
      delay(120);

    }

    finalSoftValue = " ";

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void Task3(void *pvParameters)
{
  (void) pvParameters;
  for (;;)
  {

//
//    while (Serial.available() > 0) {
//      delay(2);
//      char SoftValue = (char)Serial.read();
//      finalSoftValue += SoftValue;
//    }
//
//    if (finalSoftValue == " esp") {
//      delay(100);
//      //      Serial.println(finalSoftValue);
//      String softValue2 = "";
//      softValue2 += "@";
//      softValue2 += "esp bhai ";
//      //      softValue1 += count;
//      softValue2 += " ";
//      int softValueLength2 = softValue2.length();
//      char finalSoftValue2[softValueLength2 + 1];
//      strcpy(finalSoftValue2, softValue2.c_str());
//      //      gsmSerial.write(finalSoftValue1);
//      Serial.println(finalSoftValue2);
//      delay(100);
//
//    }
//
//    finalSoftValue = " ";
//
//    vTaskDelay(120 / portTICK_PERIOD_MS);
  }
}

void Task2(void* pvParameters) {
  (void) pvParameters;
  for (;;)
  {
    while (Serial.available() > 0) {
      switch (Serial.read()) {
        case 's':
          vTaskSuspend(Task1_Handler);
          Serial.println("Task 1 Suspend!");
          break;
        case 'r':
          vTaskResume(Task1_Handler);
          Serial.println("Task 1 Resume!");
          break;
          //        case 'm':
          //          vTaskSuspend(Task3_Handler);
          //          Serial.println("Task 2 Suspend!");
          //          break;
          //        case 'n':
          //          vTaskResume(Task3_Handler);
          //          Serial.println("Task 2 Resume!");
          //          break;
      }
      vTaskDelay(1);
    }
  }
}
