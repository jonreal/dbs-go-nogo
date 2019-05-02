#include "DbsGoNoGo.h"
#include "Arduino.h"
#include <Wire.h>

DbsGoNoGo::DbsGoNoGo(){

  // PinMux
  nogo = 2;
  target1 = 3;
  target2 = 4;
  target3 = 5;
  start = 6;
  pinMode(target1,OUTPUT);
  digitalWrite(target1, LOW);
  pinMode(target2,OUTPUT);
  digitalWrite(target2, LOW);
  pinMode(target3,OUTPUT);
  digitalWrite(target3, LOW);
  pinMode(nogo,OUTPUT);
  digitalWrite(nogo, LOW);
  pinMode(start,INPUT);

  // Testing parameters
  numOfReaches = 96;
  maxTaskTime = 3500;
  minOnTime = 1500;

  // Task transition vars
  transitions = new transition_t[numOfReaches];
  colors = new unsigned int[numOfReaches];
  gonogo = new unsigned int[numOfReaches];

  //
  state = FREE;
  tnow = 0;
  tprev = 0;
  tp = 0;
  ts = 0;
  t = 0;
  buttonpress = 0;
  threshold1 = 3000;
  threshold2 = 500;
  buttoncnt = 0;
  taskcnt = 0;

  randomSeed(analogRead(0));
}

void DbsGoNoGo::ToggleNoGo(void) {
  digitalWrite(nogo, !digitalRead(nogo));
}

void DbsGoNoGo::SetTaskAssignment(unsigned int c, unsigned int gn) {

  switch (c) {
    case 1 :
      digitalWrite(target1, HIGH);
      digitalWrite(target2, LOW);
      digitalWrite(target3, LOW);
      break;
    case 2 :
      digitalWrite(target1, LOW);
      digitalWrite(target2, HIGH);
      digitalWrite(target3, LOW);
      break;
    case 3 :
      digitalWrite(target1, LOW);
      digitalWrite(target2, LOW);
      digitalWrite(target3, HIGH);
      break;
  }

  switch (gn) {
    case 1 :
      digitalWrite(nogo, HIGH);
      break;
    case 0 :
      digitalWrite(nogo, LOW);
      break;
  }
}

void DbsGoNoGo::LEDsOff(void) {
  digitalWrite(target1, LOW);
  digitalWrite(target2, LOW);
  digitalWrite(target3, LOW);
  digitalWrite(nogo, LOW);
}


void DbsGoNoGo::CycleLEDs(unsigned int t) {

  digitalWrite(target1, HIGH);
  digitalWrite(target2, LOW);
  digitalWrite(target3, LOW);
  digitalWrite(nogo, LOW);
  delay(t);

  digitalWrite(target1, LOW);
  digitalWrite(target2, HIGH);
  digitalWrite(target3, LOW);
  digitalWrite(nogo, LOW);
  delay(t);

  digitalWrite(target1, LOW);
  digitalWrite(target2, LOW);
  digitalWrite(target3, HIGH);
  digitalWrite(nogo, LOW);
  delay(t);

  digitalWrite(target1, LOW);
  digitalWrite(target2, LOW);
  digitalWrite(target3, LOW);
  digitalWrite(nogo, HIGH);
  delay(t);

  digitalWrite(nogo, LOW);

}


void DbsGoNoGo::MakeNewExp() {
  GenerateNewTransitions();
  PrintTransitions();
  GenerateNewTaskAssignment();
  PrintTaskAssignment();
}


unsigned char DbsGoNoGo::StartButtonPushed() {
  return digitalRead(start);
}


void DbsGoNoGo::GenerateNewTransitions(void) {
  long t1_;
  long t2_;
  long temp_;

  for (int i=0; i<numOfReaches; i++) {
    t1_ = random(0,maxTaskTime);
    while(1) {
      t2_ = random(0,maxTaskTime);
      if (abs(t1_ - t2_) < minOnTime) {
        continue;
      } else {
        break;
      }
    }
    if (t1_ > t2_) {
      temp_ = t1_;
      t1_ = t2_;
      t2_ = temp_;
    }
    transitions[i].t1 = (unsigned int)t1_;
    transitions[i].t2 = (unsigned int)t2_;
  }
}

void DbsGoNoGo::PrintTransitions(void) {
  Serial.println();
  Serial.print("t1\t");
  Serial.print("t2\t");
  Serial.println("t2-t1\t");
  for (int i=0; i<numOfReaches; i++) {
    Serial.print(transitions[i].t1);
    Serial.print("\t");
    Serial.print(transitions[i].t2);
    Serial.print("\t");
    Serial.print(transitions[i].t2 - transitions[i].t1);
    Serial.println();
  }
}

void DbsGoNoGo::GenerateNewTaskAssignment(void) {
  unsigned int colors_bag[numOfReaches];
  unsigned int gonogo_bag[numOfReaches];
  long randi;
  long maxRandi;

  // Fill "bags"
  for (int i=0; i<(numOfReaches/3); i++) {
    colors_bag[i] = 1;
    colors_bag[i + (numOfReaches/3)] = 2;
    colors_bag[i + 2*(numOfReaches/3)] = 3;
  }

//  // 1:2
//  for (int i=0; i<numOfReaches; i+=2) {
//    gonogo_bag[i] = 0;
//    gonogo_bag[i+1] = 1;
//  }

  // 1:4
  for (int i=0; i<numOfReaches; i+=4) {
    gonogo_bag[i] = 0;
    gonogo_bag[i+1] = 0;
    gonogo_bag[i+2] = 0;
    gonogo_bag[i+3] = 1;
  }


  // Sample from bags
  maxRandi = numOfReaches;
  for (int i=0; i<numOfReaches; i++) {
    randi = random(0,maxRandi);
    colors[i] = colors_bag[randi];
    gonogo[i] = gonogo_bag[randi];
    for (int j=(randi+1); j<maxRandi; j++) {
      colors_bag[j-1] = colors_bag[j];
      gonogo_bag[j-1] = gonogo_bag[j];
    }
    maxRandi -= 1;
  }
}

void DbsGoNoGo::PrintTaskAssignment(void) {
  Serial.println();
  Serial.print("color\t");
  Serial.print("gonogo\t");
  Serial.println();
  for (int i=0; i<numOfReaches; i++) {
    Serial.print(colors[i]);
    Serial.print("\t");
    Serial.print(gonogo[i]);
    Serial.println();
  }
}


void DbsGoNoGo::HandleFlow(void) {

  if ( (tnow = millis()) == tprev)
    return;
  buttonpress = StartButtonPushed();

  switch (state) {
    case FREE :
      if ((tnow % 500) == 0){
        ToggleNoGo();
      }
      if (buttonpress) {
        buttoncnt++;
      } else {
        buttoncnt = 0;
      }
      if (buttoncnt >= threshold1) {
        LEDsOff();
        CycleLEDs(250);
        MakeNewExp();
        CycleLEDs(250);
        buttoncnt = 0;
        state = WAITING;
      }
      break;

    case WAITING :
      if (buttonpress) {
        buttoncnt++;
      } else {
        buttoncnt = 0;
      }
      if (buttoncnt >= threshold2) {
        buttoncnt = 0;
        ts = tnow;
        state = TASK_OFF;
        if (taskcnt >= numOfReaches) {
          LEDsOff();
          taskcnt = 0;
          state = FREE;
        }
      }
      break;

    case TASK_OFF :
      t = tnow - ts;
      if (t  >= transitions[taskcnt].t1) {
        state = TASK_ON;
      }
      break;

    case TASK_ON :
      t = tnow - ts;
      if (t >= transitions[taskcnt].t2) {
        LEDsOff();
        state = WAITING;
        taskcnt++;
      } else {
        SetTaskAssignment(colors[taskcnt],gonogo[taskcnt]);
      }
      break;
  }
  tprev = tnow;
}
