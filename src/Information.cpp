//
//  Information.cpp
//  SimpleGraph
//
//  Created by Musashi Nakajima on 2015/04/12.
//
//

#include "Information.h"

ofTrueTypeFont action;

void Information::setup(float w, float h) {
    ofTrueTypeFont::setGlobalDpi(72);
    action.loadFont("verdana.ttf", 30, true, true);
    action.setLineHeight(34.0f);
    action.setLetterSpacing(1.035);

    ap = ofPoint(w/2, h*9/10);
    cp = ofPoint(w*3/4, h*9/10);
    width = w/4-5;
    height = h/10-5;
}

void Information::draw() {
    ofRectangle add(ap, width, height);
    ofRectangle can(cp, width, height);
    
    ofPushStyle();
    ofSetLineWidth(2);
    ofNoFill();
    ofSetColor(200, 255, 255, 150);
    ofRectRounded(add, 8);
    action.drawString("Add", ap.x+40, ap.y+38);
    
    ofNoFill();
    ofSetLineWidth(2);
    ofRectRounded(can, 8);
    action.drawString("Cancel", cp.x+20, cp.y+38);
    
    ofPopStyle();
}

void Information::drawNet() {
    ofRectangle close(ap, width, height);
    ofRectangle del(cp, width, height);

    ofPushStyle();
    ofSetLineWidth(2);
    ofNoFill();
    ofSetColor(200, 255, 255, 150);
    ofRectRounded(close, 8);
    action.drawString("Close", ap.x+40, ap.y+38);
    
    ofNoFill();
    ofSetLineWidth(2);
    ofRectRounded(del, 8);
    action.drawString("Delete", cp.x+20, ap.y+38);

    ofPopStyle();
}
ACTION Information::checkAction(ofPoint p) {
    ofRectangle add(ap, width, height);
    ofRectangle can(cp, width, height);
    ofPoint tp;
    tp.x = p.x - ofGetWidth()/8;
    tp.y = p.y - ofGetHeight()/4;
    
    if(add.inside(tp)) {
        return ADD;
    }else if(can.inside(tp)) {
        return CANCEL;
    }else
        return NOACTION;
}

ACTION Information::checkActionNet(ofPoint p) {
    ofRectangle add(ap, width, height);
    ofRectangle can(cp, width, height);
    ofPoint tp;
    tp.x = p.x - ofGetWidth()/8;
    tp.y = p.y - ofGetHeight()/4;
    
    if(add.inside(tp)) {
        return CLOSE;
    }else if(can.inside(tp)) {
        return DELETE;
    }else
        return NOACTION;
}