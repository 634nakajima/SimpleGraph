//
//  Button.h
//  SimpleGraph
//
//  Created by Musashi Nakajima on 2015/04/08.
//
//

#ifndef __SimpleGraph__Button__
#define __SimpleGraph__Button__

#include <stdio.h>
#include "ofMain.h"

enum MODE{
    NET,
    OBJ
};

class Button {
public:
    Button();
    ~Button(){}
    void setup(MODE m);
    void draw();
    bool inside(ofPoint p);
    
    MODE mode;
    ofPoint np,op;
    float w, h;
};
#endif /* defined(__SimpleGraph__Button__) */
