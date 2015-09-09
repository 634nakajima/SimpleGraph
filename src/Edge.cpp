//
//  Edge.cpp
//  SimpleGraph
//
//  Created by Musashi Nakajima on 2015/04/07.
//
//

#include "Edge.h"

Edge::Edge() {
    
}
//inputNodeID,outputNodeID, inputID, outputID, inputdatatype, outputdatatype
Edge::Edge(int inNode, int outNode, int input, int output, DATATYPE inDT, DATATYPE outDT) {
    setup(inNode, outNode, input, output);
    inType = inDT;
    outType = outDT;
}

Edge::Edge(int inNode, int outNode, int input, int output) {
    setup(inNode, outNode, input, output);
}

Edge::~Edge() {
    
}

void Edge::setup(int inNode, int outNode, int input, int output) {
    inNodeID = inNode;
    outNodeID = outNode;
    inputID = input;
    outputID = output;
}

void Edge::update(ofVec2f input, ofVec2f output) {
    inVec = input;
    outVec = output;
}

void Edge::draw() {
    ofPushStyle();  // push the current style for use later
    ofSetLineWidth(3);
    ofSetColor(200, 255, 255, 200);
    ofLine(inVec, outVec);
    ofPopStyle();
}

bool Edge::checkAudioIN(Edge edge) {
    if((inNodeID == edge.inNodeID) &&
       (inputID == edge.inputID) &&
       (inType == AUDIO) &&
       (edge.inType == AUDIO))
        return false;
    else
        return true;
}

bool Edge::connectable(Edge edge) {
    if(checkAudioIN(edge))
        return true;
    else
        return false;
}

bool Edge::operator==(Edge edge) {
    if (edge.inNodeID == inNodeID &&
        edge.outNodeID == outNodeID &&
        edge.inputID == inputID &&
        edge.outputID == outputID)
        return true;
    else return false;
}