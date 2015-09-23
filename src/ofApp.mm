#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableAntiAliasing();
    ofSetLineWidth(1);
    net.setup(&co);
    objl.setup();
    button.setup(OBJ);
    
    co.init(&s);
    tID = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    switch (button.mode) {
        case NET:
            net.update();
            break;
        case OBJ:
            objl.update(co.ml->mList);
            break;
        default:
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
    net.exit();
    objl.exit();
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
    
    if(button.inside(ofPoint(touch.x, touch.y)) == OBJ) {
        //co.ml->requestML();
    }
    switch (button.mode) {
        case NET:
            net.touchDown(touch);
            break;
        case OBJ:
            objl.touchDown(touch);
            if(objl.addNode) {
                co.createModule(objl.infoNode->nID);
                net.addNode(objl.infoNode->mtkn);
                button.mode = NET;
                objl.addNode = false;
                objl.infoNode = NULL;
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
