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
    void setup();
    void update();
    void draw(std::list<MToken*> mList);
    void exit();
    
    void touchDown(ofTouchEventArgs & touch, std::list<MToken*> mList);
    void touchMoved(ofTouchEventArgs & touch);
    void touchUp(ofTouchEventArgs & touch);
    void touchDoubleTap(ofTouchEventArgs & touch);
    void touchCancelled(ofTouchEventArgs & touch);
    
    void lostFocus();
    void gotFocus();
    void gotMemoryWarning();
    void deviceOrientationChanged(int newOrientation);
    
    void drawInfo();
    
    bool infoWindow, addNode;
    MToken *mtkn;
    Information info;

};
#endif /* defined(__SimpleGraph__ObjectList__) */
