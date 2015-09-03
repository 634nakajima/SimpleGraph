//
//  Button.cpp
//  SimpleGraph
//
//  Created by Musashi Nakajima on 2015/04/08.
//
//

#include "Button.h"

ofTrueTypeFont verdana30;

Button::Button() {
}

void Button::setup(MODE m) {
    ofTrueTypeFont::setGlobalDpi(72);
    verdana30.loadFont("verdana.ttf", 30, true, true);
    verdana30.setLineHeight(34.0f);
    verdana30.setLetterSpacing(1.035);
    mode = m;
    np = ofPoint(10, ofGetHeight()-ofGetHeight()/20-10);
    op = ofPoint(ofGetWidth()/2-15+10+10, ofGetHeight()-ofGetHeight()/20-10);
    w = ofGetWidth()/2-15;
    h = ofGetHeight()/20;
}

void Button::draw() {
    ofRectangle netb(np, w, h);
    ofRectangle objb(op, w, h);

    ofPushStyle();
    
    switch (mode) {
        case NET:
            ofFill();
            ofSetColor(255, 255, 255, 150);
            ofRectRounded(netb, 8);
            ofSetColor(0, 0, 0,200);
            verdana30.drawString("Networking", np.x+100, np.y+38);
            
            ofNoFill();
            ofSetLineWidth(2);
            ofSetColor(255, 255, 255, 150);
            ofRectRounded(objb, 8);
            ofSetColor(255, 255, 255, 150);
            verdana30.drawString("Services", op.x+130, op.y+38);
            break;
            
        case OBJ:
            ofFill();
            ofSetColor(255, 255, 255, 150);
            ofRectRounded(objb, 8);
            ofSetColor(0, 0, 0,200);
            verdana30.drawString("Services", op.x+130, op.y+38);
            
            ofNoFill();
            ofSetLineWidth(2);
            ofSetColor(255, 255, 255, 150);
            ofRectRounded(netb, 8);
            ofSetColor(255, 255, 255, 150);
            verdana30.drawString("Networking", np.x+100, np.y+38);
            break;
            
        default:
            break;
    }
    ofPopStyle();
}

bool Button::inside(ofPoint p) {
    ofRectangle netb(np, w, h);
    ofRectangle objb(op, w, h);
    
    if(netb.inside(p)) {
        mode = NET;
        return true;
    }else if(objb.inside(p)) {
        mode = OBJ;
        return true;
    }else
        return false;
}