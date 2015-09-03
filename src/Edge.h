//
//  Edge.h
//  SimpleGraph
//
//  Created by Musashi Nakajima on 2015/04/07.
//
//

#ifndef __SimpleGraph__Edge__
#define __SimpleGraph__Edge__

#include <stdio.h>
#include "ofMain.h"

enum DATATYPE {
    DATA = 0,
    AUDIO = 1
};

class Edge {
public:
    Edge();
    Edge(int inNode, int outNode, int input, int output);
    Edge(int inNode, int outNode, int input, int output, DATATYPE inDT, DATATYPE outDT);
    ~Edge();
    void setup(int inNode, int outNode, int input, int output);
    void update(ofVec2f input, ofVec2f output);
    void draw();
    
    bool operator==(Edge edge);
    bool checkAudioIN(Edge edge);
    bool connectable(Edge edge);
    int inputID, outputID;
    int inNodeID, outNodeID;
    DATATYPE inType, outType;
    ofVec2f inVec, outVec;
};
#endif /* defined(__SimpleGraph__Edge__) */
