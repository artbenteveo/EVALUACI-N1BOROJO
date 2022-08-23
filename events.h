#ifndef _EVENTS_H_
#define _EVENTS_H_

enum class BUTTONS{NONE, UP_BTN,DOWN_BTN};

typedef struct{
    bool trigger;
    BUTTONS whichButton;
} buttonEvt_t;

extern buttonEvt_t buttonEvt;

#endif