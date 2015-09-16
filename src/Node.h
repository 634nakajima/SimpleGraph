//
//  Node.h
//  SimpleGraph
//
//  Created by Musashi Nakajima on 2015/03/18.
//
//

#ifndef __SimpleGraph__Node__
#define __SimpleGraph__Node__

#include <stdio.h>
#include "ofMain.h"
#include "MToken.h"

enum IOTYPE {
    IN = 0,
    OUT = 1
};

typedef struct NodeIcon{
    ofImage ring;
    ofImage audioIn;
    ofImage audioOut;
    ofImage dataIn;
    ofImage dataOut;
    ofImage module;
}NodeIcon;

class Node {
public:
    Node();
    Node(MToken *m);
    ~Node();
    void nodeBig(MToken *m);
    void setup(std::vector<char *> inInfo, std::vector<char *> outInfo, char *icon, int size);
    void update();
    void draw();
    void drawInOut(IOTYPE type, int n);
    void drawNode();
    bool insideNode(int tx, int ty);
    bool nearNode(int tx, int ty);
    void addInput();
    void addOutput();
    ofVec2f getInputVec(int inID);
    ofVec2f getOutputVec(int outID);
    void setPosition(int nID);
    void setIcon(ofImage *sIcon, NodeIcon *nIcon);
    void resizeIcon(int size);
    
    int x, y, numIN, numOUT, nID, radius;
    float angle;
    MToken *mtkn;
    NodeIcon *icon;
    ofImage module;
    std::vector<char *> inputInfo;
    std::vector<char *> outputInfo;
};
#endif /* defined(__SimpleGraph__Node__) */
