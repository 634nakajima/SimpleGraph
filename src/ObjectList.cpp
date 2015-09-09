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
    objectInfo.loadFont("verdana.ttf", 17, true, true);
    objectInfo.setLineHeight(34.0f);
    objectInfo.setLetterSpacing(1.035);
    infoWindow = false;
    addNode = false;
    mtkn = NULL;
    info.setup(ofGetWidth()*3/4, ofGetHeight()/2);
    
    icon.ring.loadImage("Ring.png");
    icon.ring.resize(96,96);
    icon.audioIn.loadImage("AudioIn30.png");
    icon.audioIn.resize(14,14);
    icon.audioOut.loadImage("AudioOut30.png");
    icon.audioOut.resize(14,14);
    icon.dataIn.loadImage("DataIn30.png");
    icon.dataIn.resize(14,14);
    icon.dataOut.loadImage("DataOut30.png");
    icon.dataOut.resize(14,14);
    
    bicon.ring.loadImage("Ring.png");
    bicon.ring.resize(96*3,96*3);
    bicon.audioIn.loadImage("AudioIn30.png");
    bicon.audioIn.resize(14*3,14*3);
    bicon.audioOut.loadImage("AudioOut30.png");
    bicon.audioOut.resize(14*3,14*3);
    bicon.dataIn.loadImage("DataIn30.png");
    bicon.dataIn.resize(14*3,14*3);
    bicon.dataOut.loadImage("DataOut30.png");
    bicon.dataOut.resize(14*3,14*3);
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
        n->icon = &icon;
        n->draw();
        objectInfo.drawString(mt->osc+1, n->x-40, n->y+75);
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
    n.icon = &bicon;
    n.radius *= 3;
    n.x = 0;
    n.y = 0;
    
    ofPushMatrix();
    ofPushStyle();
    
    ofTranslate(ofGetWidth()*1/8, ofGetHeight()*1/4);
    ofRectangle window(0, 0, w, h);

    ofFill();
    ofSetColor(100, 100, 100, 240);
    ofRectRounded(window, 8);
    
    ofSetColor(255, 255, 255);
    objectInfo.drawString(mtkn->osc+1, w/2 - 6*strlen(mtkn->osc), h/10);

    info.draw();
    ofTranslate(w/2, h/2);
    
    int inNum = mtkn->inputInfo.size();
    int outNum = mtkn->outputInfo.size();
    
    for(int i=0; i<inNum; i++) {
        int lr = inNum/2+inNum%2;//文字を右側につけるか左側につけるかの境目
        if(i<lr) objectInfo.drawString(mtkn->inputInfo[i]+1,
                                       n.getInputVec(i).x*1.2 - 12*strlen(mtkn->inputInfo[i]),
                                       n.getInputVec(i).y*1.2 + 5);
        else objectInfo.drawString(mtkn->inputInfo[i]+1,
                                   n.getInputVec(i).x*1.2,
                                   n.getInputVec(i).y*1.2 + 5);
    }
    for(int i=0; i<outNum; i++) {
        int lr = outNum/2+outNum%2;//文字を右側につけるか左側につけるかの境目
        if(i<lr) objectInfo.drawString(mtkn->outputInfo[i]+1,
                                       n.getOutputVec(i).x*1.2,
                                       n.getOutputVec(i).y*1.2 + 5);
        else     objectInfo.drawString(mtkn->outputInfo[i]+1,
                                       n.getOutputVec(i).x*1.2 - 12*strlen(mtkn->outputInfo[i]),
                                       n.getOutputVec(i).y*1.2 + 5);
    }
    
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
