//
//  Networking.h
//  SimpleGraph
//
//  Created by Musashi Nakajima on 2015/04/07.
//
//

#ifndef __SimpleGraph__Networking__
#define __SimpleGraph__Networking__

#include <stdio.h>
#include <map>
#include "ofMain.h"
#include "Node.h"
#include "Edge.h"
#include "MToken.h"
#include "Coordinator.h"
#include "Information.h"

class Networking {
public:
    void setup(Coordinator *c);
    void update();
    void draw();
    void exit();
    
    void touchDown(ofTouchEventArgs & touch);
    void touchMoved(ofTouchEventArgs & touch);
    void touchUp(ofTouchEventArgs & touch);
    void touchDoubleTap(ofTouchEventArgs & touch);
    void touchCancelled(ofTouchEventArgs & touch);
    
    void lostFocus();
    void gotFocus();
    void gotMemoryWarning();
    void deviceOrientationChanged(int newOrientation);
    void addNode(int tID, MToken *m);
    void drawInfo();
    Node *touchingNode, *rotatingNode, *infoNode;
    int touchingID, rotatingID, movingID, infoID;
    float initialDeg;
    void *clt;
    Coordinator *co;
    Information info;
    std::map<int, Node*> nodes, touchingNodes, movingNodes;

    NodeIcon icon, bicon;
};
#endif /* defined(__SimpleGraph__Networking__) */
