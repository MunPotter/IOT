
class led {
  public:
    void blinkMe(int l) {
      digitalWrite(l, HIGH);
      delay(200);
      digitalWrite(l, LOW);
      delay(200);
    }
};

led led;
