//
//  InOut.h
//  SimpleGraph
//
//  Created by Musashi Nakajima on 2015/03/18.
//
//

#ifndef __SimpleGraph__InOut__
#define __SimpleGraph__InOut__

#include <stdio.h>
#include "ofMain.h"

enum IOTYPE {
    IN = 0,
    OUT = 1
};

class InOut {
public:
    InOut(){}
    ~InOut(){}
    IOTYPE type;
    float angle;
    void draw();
    void update();
    ofVec2f p;
};
#endif /* defined(__SimpleGraph__InOut__) */
