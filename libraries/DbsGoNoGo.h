#ifndef _DBSGONOGO_H_
#define _DBSGONOGO_H_

#include "Arduino.h"


typedef struct {
  unsigned int t1;
  unsigned int t2;
}transition_t;

typedef enum {
  FREE = 0,
  WAITING,
  TASK_OFF,
  TASK_ON,
} states_t;


class DbsGoNoGo
{
  public:
    DbsGoNoGo(void);
    void HandleFlow(void);

  private:
    void CycleLEDs(unsigned int t);
    void LEDsOff(void);
    void ToggleNoGo(void);
    void SetTaskAssignment(unsigned int c, unsigned int gn);
    void MakeNewExp(void);
    unsigned char StartButtonPushed(void);
    void GenerateNewTransitions(void);
    void PrintTransitions(void);
    void GenerateNewTaskAssignment(void);
    void PrintTaskAssignment(void);

    // Pins
    unsigned char target1;
    unsigned char target2;
    unsigned char target3;
    unsigned char nogo;
    unsigned char start;

    // Task parameters
    unsigned int numOfReaches;
    unsigned int maxTaskTime;
    unsigned int minOnTime;

    // Task Assignment
    transition_t* transitions;
    unsigned int* colors;
    unsigned int* gonogo;

    // Program Flow
    states_t state;
    unsigned long tnow;
    unsigned long tprev;
    unsigned long tp;
    unsigned long ts;
    unsigned long t;
    unsigned char buttonpress;
    unsigned int threshold1;
    unsigned int threshold2;
    unsigned int buttoncnt;
    unsigned int taskcnt;


};

#endif
