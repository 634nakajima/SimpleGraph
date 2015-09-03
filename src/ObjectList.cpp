//
//  ObjectList.cpp
//  SimpleGraph
//
//  Created by Musashi Nakajima on 2015/04/07.
//
//

#include "ObjectList.h"

ofTrueTypeFont objectInfo;

//--------------------------------------------------------------
void ObjectList::setup(){
    objectInfo.loadFont("verdana.ttf", 12, true, true);
    objectInfo.setLineHeight(34.0f);
    objectInfo.setLetterSpacing(1.035);
    infoWindow = false;
    addNode = false;
    mtkn = NULL;
    info.setup(ofGetWidth()*3/4, ofGetHeight()/2);
}

//--------------------------------------------------------------
void ObjectList::update(){

}

//--------------------------------------------------------------
void ObjectList::draw(std::list<MToken*> mList){
    ofBackground(20,40,10);
    auto it = mList.begin();
    while(it != mList.end()) {
        MToken *mt = *it;
        Node *n = new Node(mt->inputInfo, mt->outputInfo);
        n->setPosition(mt->tID);
        n->draw();
        objectInfo.drawString(mt->osc, n->x-60, n->y+75);
        ++it;
        delete n;
    }
    if(infoWindow) drawInfo();
}

void ObjectList::drawInfo() {
    float w = ofGetWidth()*3/4;
    float h = ofGetHeight()/2;
    
    Node n;
    n.setup(mtkn->inputInfo, mtkn->outputInfo);
    n.x = 0;
    n.y = 0;
    
    ofPushMatrix();
    ofPushStyle();
    
    ofTranslate(ofGetWidth()*1/8, ofGetHeight()*1/4);
    ofRectangle window(0, 0, w, h);

    ofFill();
    ofSetColor(100, 100, 100, 200);
    ofRectRounded(window, 8);
    
    ofSetColor(255, 255, 255);
    objectInfo.drawString(mtkn->osc, w/2 - 7*strlen(mtkn->osc), h/10);

    info.draw();
    ofTranslate(w/2, h/2);
    
    int inNum = mtkn->inputInfo.size();
    int outNum = mtkn->outputInfo.size();
    
    for(int i=0; i<inNum; i++)
        objectInfo.drawString(mtkn->inputInfo[i], n.getInputVec(i).x*3.33 - 14*strlen(mtkn->inputInfo[i]), n.getInputVec(i).y*3.33+5);
    for(int i=0; i<outNum; i++)
        objectInfo.drawString(mtkn->outputInfo[i], n.getOutputVec(i).x*3.33 + 25, n.getOutputVec(i).y*3.33+5);
    
    ofScale(3, 3);
    n.draw();
    

    ofPopMatrix();
    ofPopStyle();
}

//--------------------------------------------------------------
void ObjectList::exit(){
}

//--------------------------------------------------------------
void ObjectList::touchDown(ofTouchEventArgs & touch, std::list<MToken*> mList){
    if(!infoWindow) {
        auto it = mList.begin();
        while(it != mList.end()) {
            MToken *mt = *it;
            Node *n = new Node(mt->inputInfo, mt->outputInfo);
            n->setPosition(mt->tID);
            if(n->insideNode(touch.x, touch.y)) {
                infoWindow = true;
                mtkn = mt;
                delete n;
                return;
            }
            ++it;
            delete n;
        }
    }else {
        //ボタン押下チェック
        switch(info.checkAction(ofPoint(touch.x,touch.y))) {
            case ADD:
                addNode = true;
                infoWindow = false;
            break;
                
            case CANCEL:
                infoWindow = false;
                mtkn = NULL;
                break;
                
            case NOACTION:
                break;
                
            default:
                break;
        }
    }
}

//--------------------------------------------------------------
void ObjectList::touchMoved(ofTouchEventArgs & touch){
}

//--------------------------------------------------------------
void ObjectList::touchUp(ofTouchEventArgs & touch){
}

//--------------------------------------------------------------
void ObjectList::touchDoubleTap(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ObjectList::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ObjectList::lostFocus(){
    
}

//--------------------------------------------------------------
void ObjectList::gotFocus(){
    
}

//--------------------------------------------------------------
void ObjectList::gotMemoryWarning(){
    
}

//--------------------------------------------------------------
void ObjectList::deviceOrientationChanged(int newOrientation){
    
}
