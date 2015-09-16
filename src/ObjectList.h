//
//  ObjectList.h
//  SimpleGraph
//
//  Created by Musashi Nakajima on 2015/04/07.
//
//

#ifndef __SimpleGraph__ObjectList__
#define __SimpleGraph__ObjectList__

#include <stdio.h>
#include "ofMain.h"
#include "MToken.h"
#include "Node.h"
#include "Information.h"

class ObjectList {
public:
    bool infoWindow, addNode;
    Node *infoNode;
    NodeIcon icon, bicon;
    Information info;
    std::map<int, Node *> mListNodes;
    std::map<int, ofImage *> mListIcons;

    void setup();
    void update(std::list<MToken*> mList);
    void draw(std::list<MToken*> mList);
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
    
    void drawInfo();
};
#endif /* defined(__SimpleGraph__ObjectList__) */
