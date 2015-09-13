//
//  Node.cpp
//  SimpleGraph
//
//  Created by Musashi Nakajima on 2015/03/18.
//
//

#include "Node.h"

Node::Node() {
    x = ofGetWidth()/2;
    y = ofGetHeight()/2;
    angle = 0;
    numIN = 0;
    numOUT = 0;
    radius = 48;
}

Node::Node(MToken *m) {
    setup(m->inputInfo, m->outputInfo, m->iconData, m->iconSize);
    nID = m->tID;
    mtkn = m;
}

void Node::nodeBig(MToken *m) {
    setup(m->inputInfo, m->outputInfo, m->iconData, m->iconSize);
    nID = m->tID;
    mtkn = m;
    x = 0;
    y = 0;
    radius *= 3;
}

void Node::setup(std::vector<char *> inInfo, std::vector<char *> outInfo, char *icon, int size) {
    x = ofGetWidth()/2;
    y = ofGetHeight()/2;
    angle = 0;
    numIN = inInfo.size();
    numOUT = outInfo.size();
    inputInfo = inInfo;
    outputInfo = outInfo;
    radius = 48;
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
    
    icon->ring.draw(-icon->ring.width/2,-icon->ring.height/2);
    
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
    float outX = x+(radius+3)*cos((angle+outID*180.0/(float)numOUT)*M_PI/180.0);
    float outY = y+(radius+3)*sin((angle+outID*180.0/(float)numOUT)*M_PI/180.0);
    ofVec2f p;
    p.x = outX;
    p.y = outY;
    return p;
}

ofVec2f Node::getInputVec(int inID) {
    float inX = x+(radius+3)*cos((180.0+angle+inID*180.0/(float)numIN)*M_PI/180.0);
    float inY = y+(radius+3)*sin((180.0+angle+inID*180.0/(float)numIN)*M_PI/180.0);
    ofVec2f p;
    p.x = inX;
    p.y = inY;
    return p;
}

bool Node::insideNode(int tx, int ty) {
    float d = sqrt((x-tx)*(x-tx) + (y-ty)*(y-ty));
    if (d < radius+4) return true;
    else return false;
}

bool Node::nearNode(int tx, int ty) {
    float d = sqrt((x-tx)*(x-tx) + (y-ty)*(y-ty));
    if (d < radius*3) return true;
    else return false;
}

void Node::drawInOut(IOTYPE type, int n) {
    ofPushMatrix();
    ofPushStyle();
    ofSetLineWidth(2);
    ofTranslate(radius+2,0);
    if (type == OUT) {
        ofFill();
        if(strstr(outputInfo[n], "Audio"))
            icon->audioOut.draw(-icon->audioOut.width/2,-icon->audioOut.height/2);
        else
            icon->dataOut.draw(-icon->dataOut.width/2,-icon->dataOut.height/2);
    }
    else {
        if(strstr(inputInfo[n], "Audio"))
            icon->audioIn.draw(-icon->audioIn.width/2,-icon->audioIn.height/2);
        else
            icon->dataIn.draw(-icon->dataIn.width/2,-icon->dataIn.height/2);
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