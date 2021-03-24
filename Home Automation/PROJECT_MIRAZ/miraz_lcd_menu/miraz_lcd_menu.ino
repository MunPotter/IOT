#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

boolean backlight = true;
int contrast = 50;

int menuitem = 1;
int page = 1;

volatile boolean up = false;
volatile boolean down = false;
volatile boolean middle = false;
volatile boolean middle2 = false;

int downButtonState = 0;
int upButtonState = 0;
int selectButtonState = 0;
int cancelButtonState = 0;
int lastDownButtonState = 0;
int lastSelectButtonState = 0;
int lastUpButtonState = 0;
int lastCancelButtonState = 0;

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

void setup() {

  pinMode(2, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(0, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(8, OUTPUT);

  digitalWrite(8, LOW); //Turn Backlight ON

  //  Serial.begin(9600);

  display.begin();
  display.setContrast(contrast); //Set contrast to 50
  display.clearDisplay();
  display.display();
}

void loop() {

  drawMenu();

  downButtonState = digitalRead(2);
  selectButtonState = digitalRead(1);
  upButtonState =   digitalRead(0);
  cancelButtonState =   digitalRead(9);

  checkIfDownButtonIsPressed();
  checkIfUpButtonIsPressed();
  checkIfSelectButtonIsPressed();
  checkIfCancelButtonIsPressed();

  if (up && page == 1 ) {
    up = false;
    menuitem--;
    if (menuitem == 0)
    {
      menuitem = 3;
    }
  } else if (up && page == 2 ) {
    up = false;
  }


  if (down && page == 1) {
    down = false;
    menuitem++;
    if (menuitem == 4)
    {
      menuitem = 1;
    }
  }
  else if (down && page == 2 ) {
    down = false;
  }


  if (middle) {
    middle = false;
    if (page == 1 && menuitem == 000)
    {
      page = 2;
    }
    else if (page == 1 && menuitem == 1)
    {
      page = 2;
    }

    else if (page == 1 && menuitem == 2)
    {
      page = 2;
    }
    else if (page == 1 && menuitem == 3)
    {
      page = 2;
    }
    else if (page == 2) {
      page = 1;
    }
  }

    else if (middle2) {
    middle2 = false;
    if (page == 1 && menuitem == 000)
    {
      page = 3;
    }
    else if (page == 1 && menuitem == 1)
    {
      page = 3;
    }

    else if (page == 1 && menuitem == 2)
    {
      page = 3;
    }
    else if (page == 1 && menuitem == 3)
    {
      page = 3;
    }
    else if (page == 3) {
      page = 1;
    }
  }

}

void checkIfDownButtonIsPressed()
{
  if (downButtonState != lastDownButtonState)
  {
    if (downButtonState == 0)
    {
      down = true;
    }
    delay(50);
  }
  lastDownButtonState = downButtonState;
}

void checkIfUpButtonIsPressed()
{
  if (upButtonState != lastUpButtonState)
  {
    if (upButtonState == 0) {
      up = true;
    }
    delay(50);
  }
  lastUpButtonState = upButtonState;
}

void checkIfSelectButtonIsPressed()
{
  if (selectButtonState != lastSelectButtonState)
  {
    if (selectButtonState == 0) {
      middle = true;
    }
    delay(50);
  }
  lastSelectButtonState = selectButtonState;
}

void checkIfCancelButtonIsPressed()
{
  if (cancelButtonState != lastCancelButtonState)
  {
    if (cancelButtonState == 0) {
      middle2 = true;
    }
    delay(50);
  }
  lastCancelButtonState = cancelButtonState;
}


void drawMenu()
{

  if (page == 1)
  {
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print("REQUEST");
    display.drawFastHLine(0, 10, 83, BLACK);
    display.setCursor(0, 15);

    if (menuitem == 1)
    {
      display.setTextColor(WHITE, BLACK);
    }
    else
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">Shanto:Coffee");
    display.setCursor(0, 25);
    if (menuitem == 2)
    {
      display.setTextColor(WHITE, BLACK);
    }
    else
    {
      display.setTextColor(BLACK, WHITE);
    }

    display.print(">Mehedi:Tea ");


    if (menuitem == 3)
    {
      display.setTextColor(WHITE, BLACK);
    }
    else
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.setCursor(0, 35);
    display.print(">Rejoan:Tea");
    display.display();
  }


  else if (page == 2)
  {
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print("REQUEST");
    display.drawFastHLine(0, 10, 83, BLACK);
    display.setCursor(5, 15);
    display.print("CONFIRM");
    display.setTextSize(2);
    display.setCursor(5, 25);
    display.display();
  }

    else if (page == 3)
  {
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print("REQUEST");
    display.drawFastHLine(0, 10, 83, BLACK);
    display.setCursor(5, 15);
    display.print("CANCEL");
    display.setTextSize(2);
    display.setCursor(5, 25);
    display.display();
  }
  display.display();
}
