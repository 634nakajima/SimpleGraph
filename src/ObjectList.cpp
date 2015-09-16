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
    infoNode = NULL;
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
void ObjectList::update(std::list<MToken*> mList){
    auto itn = mListNodes.begin();
    auto itm = mList.begin();
    bool exist = false;
    while(itm != mList.end()) {
        MToken *mt = *itm;
        //MTokenに対応するNodeがなければNode生成
        if(!mListNodes.count(mt->tID)) {
            Node *n = new Node(mt);
            n->icon = &icon;
            n->setPosition(n->nID);
            mListNodes[mt->tID] = n;
        }
        ++itm;
    }
    
    while(itn != mListNodes.end()) {
        int nID = itn->first;
        itm = mList.begin();

        while(itm != mList.end()) {
            MToken *mt = *itm;
            //Nodeに対応するMTokenがあればフラグを立てる
            if(nID == mt->tID) exist = true;
            ++itm;
        }
        //Nodeに対応するMTokenがなければNode消去
        if(!exist) {
            Node *n = itn->second;
            mListNodes.erase(itn);
            delete n;
        }else ++itn;
    }
}

//--------------------------------------------------------------
void ObjectList::draw(std::list<MToken*> mList){
    ofBackground(180,210,180);
    auto it = mListNodes.begin();
    while(it != mListNodes.end()) {
        Node *n = it->second;
        n->draw();
        ofPushStyle();
        ofSetColor(0, 0, 0);
        objectInfo.drawString(n->mtkn->osc+1, n->x-40, n->y+75);
        ofPopStyle();
        ++it;
    }
    if(infoWindow) drawInfo();
}

void ObjectList::drawInfo() {
    float w = ofGetWidth()*3/4;
    float h = ofGetHeight()/2;
    
    ofPushMatrix();
    ofPushStyle();
    
    ofTranslate(ofGetWidth()*1/8, ofGetHeight()*1/4);
    ofRectangle window(0, 0, w, h);

    ofFill();
    ofSetColor(120, 120, 120, 240);
    ofRectRounded(window, 8);
    
    ofSetColor(255, 255, 255);
    objectInfo.drawString(infoNode->mtkn->osc+1, w/2 - 6*strlen(infoNode->mtkn->osc), h/10);

    info.draw();
    ofTranslate(w/2, h/2);
    
    int inNum = infoNode->inputInfo.size();
    int outNum = infoNode->outputInfo.size();
    
    for(int i=0; i<inNum; i++) {
        int lr = inNum/2+inNum%2;//文字を右側につけるか左側につけるかの境目
        if(i<lr) objectInfo.drawString(infoNode->inputInfo[i]+1,
                                       infoNode->getInputVec(i).x*1.2 - 12*strlen(infoNode->inputInfo[i]),
                                       infoNode->getInputVec(i).y*1.2 + 5);
        else objectInfo.drawString(infoNode->inputInfo[i]+1,
                                   infoNode->getInputVec(i).x*1.2,
                                   infoNode->getInputVec(i).y*1.2 + 5);
    }
    for(int i=0; i<outNum; i++) {
        int lr = outNum/2+outNum%2;//文字を右側につけるか左側につけるかの境目
        if(i<lr) objectInfo.drawString(infoNode->outputInfo[i]+1,
                                       infoNode->getOutputVec(i).x*1.2,
                                       infoNode->getOutputVec(i).y*1.2 + 5);
        else     objectInfo.drawString(infoNode->outputInfo[i]+1,
                                       infoNode->getOutputVec(i).x*1.2 - 12*strlen(infoNode->outputInfo[i]),
                                       infoNode->getOutputVec(i).y*1.2 + 5);
    }
    
    infoNode->draw();
    ofPopMatrix();
    ofPopStyle();
}

//--------------------------------------------------------------
void ObjectList::exit(){
}

//--------------------------------------------------------------
void ObjectList::touchDown(ofTouchEventArgs & touch){
    if(!infoWindow) {
        auto it = mListNodes.begin();
        while(it != mListNodes.end()) {
            Node *n = it->second;
            if(n->insideNode(touch.x, touch.y)) {
                infoWindow = true;
                infoNode = new Node();
                infoNode->nodeBig(n->mtkn);
                infoNode->icon = &bicon;
                return;
            }
            ++it;
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
                delete infoNode;
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
