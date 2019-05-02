#include "dbs-go-nogo.h"

DbsGoNoGo s;

void setup() {
  Serial.begin(9600);
}

void loop() {
  s.HandleFlow();
}
