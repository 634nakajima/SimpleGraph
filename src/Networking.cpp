//
//  Networking.cpp
//  SimpleGraph
//
//  Created by Musashi Nakajima on 2015/04/07.
//
//

#include "Networking.h"

//謎エラーが出るためヘッダでなくここで宣言
std::list<Edge *> edges;
ofTrueTypeFont nodeInfo;

//--------------------------------------------------------------
void Networking::setup(Coordinator *c){
    nodeInfo.loadFont("verdana.ttf", 15, true, true);
    nodeInfo.setLineHeight(34.0f);
    nodeInfo.setLetterSpacing(1.035);

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
    
    touchingNode = NULL;
    infoNode = NULL;
    rotatingNode = NULL;
    touchingID = -1;
    rotatingID = -1;
    movingID = -1;
    initialDeg = 0;
    numNodes = 0;
    co = c;
    info.setup(ofGetWidth()*3/4, ofGetHeight()/2);
}

void Networking::addNode(MToken *m) {
    Node *n = new Node(m);
    n->nID = numNodes;
    n->icon = &icon;

    auto it = nodes.begin();
    bool near = false;
    n->x = ofGetWidth()/2;
    n->y = ofGetHeight()/2;
    
    do {
        near = false;
        while(it != nodes.end()) {
            Node *tn = it->second;
            if(tn->nearNode(n->x, n->y)) {
                near = true;
                if(n->y > 48*4) n->y -= 48*3;
                else {
                    n->y = ofGetHeight()/2;
                    if(n->x > 48*4) n->x -= 48*3;
                }
            }
            ++it;
        }
    }while(near);
    
    nodes[numNodes++] = n;
}

//--------------------------------------------------------------
void Networking::update(){
    auto it = nodes.begin();
    while(it != nodes.end()) {
        std::map<int, Node*>::iterator lt = nodes.begin();
        while(lt != nodes.end()) {
            if (it != lt) {
                Node *n1 = it->second;
                Node *n2 = lt->second;
                
                for (int i=0; i<n1->numOUT;i++) {
                    ofVec2f outp = n1->getOutputVec(i);
                    
                    for (int j=0; j<n2->numIN;j++) {
                        ofVec2f inp = n2->getInputVec(j);
                        
                        if (outp.distance(inp) < 40) {
                            bool found = false;

                            DATATYPE n1A, n2A;
                            if(strstr(n1->outputInfo[i], "Audio")) n1A = AUDIO;
                            else n1A = DATA;
                            if(strstr(n2->inputInfo[j], "Audio")) n2A = AUDIO;
                            else n2A = DATA;
                            if (n1A != n2A) found = true;
                            
                            Edge *te = new Edge(lt->first, it->first, j, i, n2A, n1A);
                            std::list<Edge *>::iterator et = edges.begin();

                            while (et != edges.end()) {
                                Edge *e = *et;
                                if (*e == *te) found = true;//既存エッジか確認
                                else if(!te->connectable(*e)) found = true;//AudioInに2個以上つながってたらNG
                                ++et;
                            }
                            if (found) delete te;//既存エッジならデリート
                            else {
                                edges.push_back(te);//新エッジならリスト追加
                                ofLog() << "connect " << "Node" << it->first << n1->outputInfo[i] << " to "
                                << "Node" << lt->first << n2->inputInfo[j];//コネクト送信の予定
                                co->addConnection(it->first, lt->first, i, j);
                            }
                        }
                    }
                }
            }
            ++lt;
        }
        ++it;
    }
    
    auto et = edges.begin();
    while (et != edges.end()) {
        Edge *e = (Edge *)*et;
        if(!nodes[e->inNodeID] || !nodes[e->outNodeID]) continue;

        e->outVec = nodes[e->outNodeID]->getOutputVec(e->outputID);
        e->inVec = nodes[e->inNodeID]->getInputVec(e->inputID);
        
        if (e->outVec.distance(e->inVec) > 160) {
            ofLog() << "disconnect " << e->outNodeID << " from " << e->inNodeID;
            et = edges.erase(et);
            co->disconnect(e->outNodeID, e->inNodeID, e->outputID, e->inputID);

            delete e;
            continue;
        }
        ++et;
    }
}

//--------------------------------------------------------------
void Networking::draw(){
    ofBackground(170,190,200);
    std::map<int, Node *>::iterator it = nodes.begin();
    while(it != nodes.end()) {
        Node *n = it->second;
        n->draw();
        ++it;
    }
    
    std::list<Edge *>::iterator et = edges.begin();
    while (et != edges.end()) {
        Edge *e = *et;
        e->draw();
        ++et;
    }
    if (infoNode) drawInfo();
}

//--------------------------------------------------------------
void Networking::drawInfo() {
    float w = ofGetWidth()*3/4;
    float h = ofGetHeight()/2;
    
    ofPushMatrix();
    ofPushStyle();
    
    ofTranslate(ofGetWidth()*1/8, ofGetHeight()*1/4);
    ofRectangle window(0, 0, w, h);
    
    ofFill();
    ofSetColor(150, 150, 160, 240);
    ofRectRounded(window, 8);
    
    ofSetColor(255, 255, 255);
    nodeInfo.drawString(infoNode->mtkn->osc+1, w/2 - 6*strlen(infoNode->mtkn->osc+1), h/10);
    
    info.drawNet();
    ofTranslate(w/2, h/2);
    
    int inNum = infoNode->inputInfo.size();
    int outNum = infoNode->outputInfo.size();
    
    for(int i=0; i<inNum; i++) {
        int lr = inNum/2+inNum%2;//文字を右側につけるか左側につけるかの境目
        if(i<lr) nodeInfo.drawString(infoNode->inputInfo[i]+1,
                                       infoNode->getInputVec(i).x*1.2 - 10*strlen(infoNode->inputInfo[i]),
                                       infoNode->getInputVec(i).y*1.2 + 5);
        else nodeInfo.drawString(infoNode->inputInfo[i]+1,
                                   infoNode->getInputVec(i).x*1.2,
                                   infoNode->getInputVec(i).y*1.2 + 5);
    }
    for(int i=0; i<outNum; i++) {
        int lr = outNum/2+outNum%2;//文字を右側につけるか左側につけるかの境目
        if(i<lr) nodeInfo.drawString(infoNode->outputInfo[i]+1,
                                     infoNode->getOutputVec(i).x*1.2,
                                     infoNode->getOutputVec(i).y*1.2 + 5);
        else     nodeInfo.drawString(infoNode->outputInfo[i]+1,
                                     infoNode->getOutputVec(i).x*1.2 - 10*strlen(infoNode->outputInfo[i]),
                                     infoNode->getOutputVec(i).y*1.2 + 5);
    }
    infoNode->draw();
    
    ofPopMatrix();
    ofPopStyle();
}

//--------------------------------------------------------------
void Networking::exit(){
    std::map<int, Node *>::iterator it = nodes.begin();
    while(it != nodes.end()) {
        Node *n = it->second;
        co->deleteModule(it->first);//ノード番号、モジュールマネージャ番号
        usleep(1000);
        delete n;
        nodes.erase(it);
        it++;
    }
}

//--------------------------------------------------------------
void Networking::touchDown(ofTouchEventArgs & touch){
    std::map<int, Node *>::iterator it = nodes.begin();
    if(!infoNode) {
        while(it != nodes.end()) {
            Node *n = it->second;
            if (n->insideNode(touch.x, touch.y))
                touchingNodes[touch.id] = n;
            ++it;
        }
        if (rotatingID == -1 && touchingNodes.size()+movingNodes.size() == 1 && touchingNodes.find(touch.id) == touchingNodes.end()){
            rotatingID = touch.id;
            if (touchingNodes.size()) rotatingNode = touchingNodes.begin()->second;
            else rotatingNode = movingNodes.begin()->second;
            initialDeg = 180.0*atan((double)(touch.y-rotatingNode->y)/(touch.x-rotatingNode->x))/M_PI-rotatingNode->angle;
            if(touch.x < rotatingNode->x) initialDeg += 180;
        }
    }else {
        switch(info.checkActionNet(ofPoint(touch.x,touch.y))) {
            case CLOSE:
                delete infoNode;
                infoNode = NULL;
                break;
                
            case DELETE: {
                auto et = edges.begin();
                while (et != edges.end()) {
                    Edge *e = (Edge *)*et;
                    if (e->inNodeID == infoNode->nID || e->outNodeID == infoNode->nID) {
                        e->outVec = nodes[e->outNodeID]->getOutputVec(e->outputID);
                        e->inVec = nodes[e->inNodeID]->getInputVec(e->inputID);
                        
                        ofLog() << "disconnect " << e->outNodeID << " from " << e->inNodeID;
                        et = edges.erase(et);
                        co->disconnect(e->outNodeID, e->inNodeID, e->outputID, e->inputID);
                        
                        delete e;
                    }else ++et;
                }
                co->deleteModule(infoNode->nID);//ノード番号、モジュールマネージャ番号
                printf("delete Node nID:%d\n", infoNode->nID);
                delete nodes[infoNode->nID];
                nodes.erase(infoNode->nID);
                delete infoNode;
                infoNode = NULL;
                break;
            }
            case NOACTION:
                break;
                
            default:
                break;
        }
    }
}

//--------------------------------------------------------------
void Networking::touchMoved(ofTouchEventArgs & touch){
    if(touchingNodes.find(touch.id) != touchingNodes.end()) {//ノードタッチしたての指ならノード座標更新
        movingNodes[touch.id] = touchingNodes[touch.id];
        movingNodes[touch.id]->x = touch.x;
        movingNodes[touch.id]->y = touch.y;
        touchingNodes.erase(touchingNodes.find(touch.id));
    }else if(movingNodes.find(touch.id) != movingNodes.end()) {//移動中ノードの指ならノード座標更新
        movingNodes[touch.id]->x = touch.x;
        movingNodes[touch.id]->y = touch.y;
    }else if (rotatingNode != NULL && touch.id == rotatingID) {//ノードタッチ中の2本目の指なら角度更新
        float deg = 180.0*atan((double)(touch.y-rotatingNode->y)/(touch.x-rotatingNode->x))/M_PI;
        if(touch.x < rotatingNode->x) deg += 180;
        rotatingNode->angle = deg-initialDeg;
        while(rotatingNode->angle > 360) rotatingNode->angle -= 360;
        while(rotatingNode->angle < 0) rotatingNode->angle += 360;
    }
}

//--------------------------------------------------------------
void Networking::touchUp(ofTouchEventArgs & touch){
    if (movingNodes.find(touch.id) != movingNodes.end()) {//移動中ノードの指なら移動中ノード要素削除
        if (movingNodes[touch.id] == rotatingNode) rotatingID = -1;
        movingNodes.erase(movingNodes.find(touch.id));
    }else if (touchingNodes.find(touch.id) != touchingNodes.end()) {//ノードタッチしたての指ならタッチ仕立てノード削除
        if (touchingNodes.size() == 1) {//他にタッチしてなければインフォ表示
            infoNode = new Node();
            infoNode->nodeBig(touchingNodes[touch.id]->mtkn);
            infoNode->nID = touchingNodes[touch.id]->nID;
            infoNode->icon = &bicon;
        }
        if (touchingNodes[touch.id] == rotatingNode) rotatingID = -1;
        touchingNodes.erase(touchingNodes.find(touch.id));
    }else if (touch.id == rotatingID) {//回転の指なら回転の指情報初期化
        rotatingID = -1;
        initialDeg = 0;
    }
}

//--------------------------------------------------------------
void Networking::touchDoubleTap(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void Networking::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void Networking::lostFocus(){
    
}

//--------------------------------------------------------------
void Networking::gotFocus(){
    
}

//--------------------------------------------------------------
void Networking::gotMemoryWarning(){
    
}

//--------------------------------------------------------------
void Networking::deviceOrientationChanged(int newOrientation){
    
}
