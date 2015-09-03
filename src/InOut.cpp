//
//  InOut.cpp
//  SimpleGraph
//
//  Created by Musashi Nakajima on 2015/03/18.
//
//

#include "InOut.h"

void InOut::update() {
    
}

void InOut::draw() {
    ofPushMatrix();
    ofPushStyle();
    ofSetLineWidth(2);
    if (type == OUT) {
        ofFill();
        ofSetColor(255,10,0,180);
    }
    else {
        ofFill();
        ofSetColor(0,10,255,180);
    }
    ofTranslate(34,0);
    ofCircle(0, 0, 5);
    ofPopMatrix();
    ofPopStyle();
}