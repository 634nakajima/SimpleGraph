#include "ofApp.h"

void ofApp::threadStart()
{
    int result;
    
    if (!active) {
        active = 1;
        done = 0;
        
        // Create the server thread
        result = pthread_create(&thread, NULL, threadFunction, (void *)this);
        if (result) {
            fprintf(stderr,
                    "Failed to create thread: pthread_create(), %s",
                    strerror(result));
        }
    } 
}

void ofApp::threadStop()
{
    int result;
    
    if (active) {
        // Signal thread to stop
        active = 0;
        
        // pthread_join waits for thread to terminate
        // and then releases the thread's resources
        result = pthread_join( thread, NULL );
        if (result) {
            fprintf(stderr, "Failed to stop thread: pthread_join(), %s",
                    strerror(result));
        }
    }
}

void *ofApp::threadFunction(void *data)
{
    ofApp *app = (ofApp *)data;
    
    while (app->active) {
        switch (app->up) {
            case 1:
                app->objl.update();
                app->up = 0;
                break;
            case 2:
                app->net.update();
                app->up = 0;
                break;
            default:
                break;
        }
    }
    pthread_exit(NULL);
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableAntiAliasing();
    ofSetLineWidth(1);
    net.setup(&co);
    objl.setup();
    button.setup(OBJ);
    
    co.init(&s);
    tID = 0;
    active = 0;
    done = 0;
    threadStart();
}

//--------------------------------------------------------------
void ofApp::update(){
    switch (button.mode) {
        case NET:
            //net.update();
            up = 2;
            break;
        case OBJ:
            //objl.update();
            up = 1;
            break;
        default:
            up = 0;
            break;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    switch (button.mode) {
        case NET:
            net.draw();
            break;
        case OBJ:
            objl.draw(co.ml->mList);
        default:
            break;
    }
    button.draw();
}

//--------------------------------------------------------------
void ofApp::exit(){
    threadStop();
    net.exit();
    objl.exit();
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
    
    button.inside(ofPoint(touch.x, touch.y));
    switch (button.mode) {
        case NET:
            net.touchDown(touch);
            break;
        case OBJ:
            objl.touchDown(touch, co.ml->mList);
            if(objl.addNode) {
                co.createModule(tID, objl.mtkn->tID);
                net.addNode(tID++, objl.mtkn);
                button.mode = NET;
                objl.addNode = false;
                objl.mtkn = NULL;
            }
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){

    switch (button.mode) {
        case NET:
            net.touchMoved(touch);
            break;
        case OBJ:
            objl.touchMoved(touch);
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){

    switch (button.mode) {
        case NET:
            net.touchUp(touch);
            break;
        case OBJ:
            objl.touchUp(touch);
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::lostFocus(){

}

//--------------------------------------------------------------
void ofApp::gotFocus(){

}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){

}
