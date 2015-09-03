//
//  Node.cpp
//  SimpleGraph
//
//  Created by Musashi Nakajima on 2015/03/18.
//
//

#include "Node.h"

Node::Node() {
}

Node::Node(std::vector<char *> inInfo, std::vector<char *> outInfo) {
    setup(inInfo, outInfo);
}

void Node::setup(std::vector<char *> inInfo, std::vector<char *> outInfo) {
    x = ofGetWidth()/2;
    y = ofGetHeight()/2;
    angle = 0;
    numIN = inInfo.size();
    numOUT = outInfo.size();
    inputInfo = inInfo;
    outputInfo = outInfo;
}

void Node::update() {
    
}

void Node::draw() {
    ofPushMatrix();
    ofTranslate(x, y);
    ofRotateZ(angle);
    drawNode();
    ofPopMatrix();
}

void Node::drawNode() {
    ofPushStyle();  // push the current style for use later
    ofPushMatrix();
    ofSetLineWidth(5);
    ofFill();
    ofSetColor(175,195,215,180);
    ofCircle(0, 0, RADIUS);
    ofNoFill();
    ofSetColor(225,255,225,180);
    ofCircle(0, 0, RADIUS);

    for (int i=0;i<numOUT;i++){
        drawInOut(OUT, i);
        ofRotateZ(180.0/(float)numOUT);
    }
    ofPopMatrix();
    ofPushMatrix();
    ofRotateZ(180);
    for (int i=numOUT;i<numIN+numOUT;i++){
        drawInOut(IN, i-numOUT);
        ofRotateZ(180.0/(float)numIN);
    }
    ofPopMatrix();
    ofPopStyle();   // recall the pushed style
}

ofVec2f Node::getOutputVec(int outID) {
    float outX = x+(RADIUS+3)*cos((angle+outID*180.0/(float)numOUT)*M_PI/180.0);
    float outY = y+(RADIUS+3)*sin((angle+outID*180.0/(float)numOUT)*M_PI/180.0);
    ofVec2f p;
    p.x = outX;
    p.y = outY;
    return p;
}

ofVec2f Node::getInputVec(int inID) {
    float inX = x+(RADIUS+3)*cos((180.0+angle+inID*180.0/(float)numIN)*M_PI/180.0);
    float inY = y+(RADIUS+3)*sin((180.0+angle+inID*180.0/(float)numIN)*M_PI/180.0);
    ofVec2f p;
    p.x = inX;
    p.y = inY;
    return p;
}

bool Node::insideNode(int tx, int ty) {
    float d = sqrt((x-tx)*(x-tx) + (y-ty)*(y-ty));
    if (d < RADIUS+4) return true;
    else return false;
}

void Node::drawInOut(IOTYPE type, int n) {
    ofPushMatrix();
    ofPushStyle();
    ofSetLineWidth(2);
    if (type == OUT) {
        ofFill();
        if(strstr(outputInfo[n], "Audio"))
            ofSetColor(255,10,0,180);
        else
            ofSetColor(0,10,255,180);
        ofTranslate(RADIUS+7,0);
        ofCircle(0, 0, 6);
    }
    else {
        if(strstr(inputInfo[n], "Audio")) {
            ofFill();
            ofTranslate(RADIUS+7,0);
            ofSetColor(255,10,0,80);
            ofCircle(0, 0, 6);
            ofNoFill();
            ofSetColor(255,10,0,180);
            ofCircle(0, 0, 6);
        } else {
            ofFill();
            ofTranslate(RADIUS+7,0);
            ofSetColor(0,10,255,80);
            ofCircle(0, 0, 6);
            ofNoFill();
            ofSetColor(0,10,255,180);
            ofCircle(0, 0, 6);
        }
    }

    ofPopMatrix();
    ofPopStyle();
}

void Node::setPosition(int nID) {
    ofVec2f p;
    x = (nID%5)*ofGetWidth()/5 + ofGetWidth()/10;
    y = (nID/5)*ofGetHeight()/8 + ofGetHeight()/16;
}
Node::~Node() {
    
}