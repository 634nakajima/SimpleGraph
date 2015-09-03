#pragma once

#include "ofMain.h"
#include "ofxiOS.h"
#include "ofxiOSExtras.h"
#include "Networking.h"
#include "ObjectList.h"
#include "Button.h"
#include "Coordinator.h"
#include "Server.h"

class ofApp : public ofxiOSApp {
public:
    void setup();
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
    
    Networking net;
    ObjectList objl;
    Button button;
    Server s;
    Coordinator co;
    
    volatile int active, up;
    volatile int done;
    pthread_t   thread;
    void threadStop();
    void threadStart();
    int tID;
    
private:
    static void *threadFunction(void *data);
};


