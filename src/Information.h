//
//  Information.h
//  SimpleGraph
//
//  Created by Musashi Nakajima on 2015/04/12.
//
//

#ifndef __SimpleGraph__Information__
#define __SimpleGraph__Information__

#include <stdio.h>
#include "ofMain.h"

enum ACTION {
    ADD,
    CANCEL,
    CLOSE,
    DELETE,
    NOACTION
};
class Information {
public:
    Information(){}
    ~Information(){}
    void draw();
    void drawNet();
    void setup(float w, float h);
    ACTION checkAction(ofPoint);
    ACTION checkActionNet(ofPoint);

    ofPoint ap,cp;
    float width, height;
};
#endif /* defined(__SimpleGraph__Information__) */
