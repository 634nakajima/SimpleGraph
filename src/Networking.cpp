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
    nodeInfo.loadFont("verdana.ttf", 12, true, true);
    nodeInfo.setLineHeight(34.0f);
    nodeInfo.setLetterSpacing(1.035);

    touchingNode = NULL;
    infoNode = NULL;
    rotatingNode = NULL;
    touchingID = -1;
    rotatingID = -1;
    movingID = -1;
    initialDeg = 0;
    co = c;
    info.setup(ofGetWidth()*3/4, ofGetHeight()/2);
}

void Networking::addNode(int tID, MToken *m) {
    Node *n = new Node(m->inputInfo, m->outputInfo);
    
    auto it = nodes.begin();
    bool inside = false;
    n->x = ofGetWidth()/2;
    n->y = ofGetHeight()/2;
    
    do {
        inside = false;
        while(it != nodes.end()) {
            Node *tn = it->second;
            if(tn->insideNode(n->x, n->y)) {
                inside = true;
                if(n->y > 170) n->y -= 120;
                else {
                    n->y = ofGetHeight()/2;
                    n->x -= 150;
                }
            }
            ++it;
        }
    }while(inside);
    
    n->nID = tID;
    n->mtkn = m;
    nodes[tID] = n;
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
                                co->addConnection(it->first, lt->first, i, n2->inputInfo[j]);
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
        e->outVec = nodes[e->outNodeID]->getOutputVec(e->outputID);
        e->inVec = nodes[e->inNodeID]->getInputVec(e->inputID);
        
        if (e->outVec.distance(e->inVec) > 160) {
            ofLog() << "disconnect " << e->outNodeID << " from " << e->inNodeID;
            et = edges.erase(et);
            co->disconnect(e->outNodeID, e->inNodeID, e->outputID, nodes[e->inNodeID]->inputInfo[e->inputID]);

            delete e;
            continue;
        }
        ++et;
    }
}

//--------------------------------------------------------------
void Networking::draw(){
    ofBackground(0,30,50);
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
    
    Node n;
    n.setup(infoNode->inputInfo, infoNode->outputInfo);
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
    nodeInfo.drawString(infoNode->mtkn->osc, w/2 - 7*strlen(infoNode->mtkn->osc), h/10);
    
    info.drawNet();
    ofTranslate(w/2, h/2);
    
    int inNum = infoNode->inputInfo.size();
    int outNum = infoNode->outputInfo.size();
    
    for(int i=0; i<inNum; i++)
        nodeInfo.drawString(infoNode->inputInfo[i],
                            n.getInputVec(i).x*3.33 - 14*strlen(infoNode->inputInfo[i]),
                            n.getInputVec(i).y*3.33+5);
    for(int i=0; i<outNum; i++)
        nodeInfo.drawString(infoNode->outputInfo[i],
                            n.getOutputVec(i).x*3.33 + 25,
                            n.getOutputVec(i).y*3.33+5);
    
    ofScale(3, 3);
    n.draw();
    
    ofPopMatrix();
    ofPopStyle();
}

//--------------------------------------------------------------
void Networking::exit(){
    std::map<int, Node *>::iterator it = nodes.begin();
    while(it != nodes.end()) {
        Node *n = it->second;
        delete n;
        nodes.erase(it);
    }
}

//--------------------------------------------------------------
void Networking::touchDown(ofTouchEventArgs & touch){
    std::map<int, Node *>::iterator it = nodes.begin();
    if(!infoNode) {
        while(it != nodes.end()) {
            Node *n = it->second;
            if (n->insideNode(touch.x, touch.y)) {
                touchingNodes[touch.id] = n;
            }
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
                infoNode = NULL;
                break;
                
            case DELETE: {
                auto et = edges.begin();
                while (et != edges.end()) {
                    Edge *e = (Edge *)*et;
                    if (e->inNodeID == infoNode->nID ||
                        e->outNodeID == infoNode->nID) {
                        e->outVec = nodes[e->outNodeID]->getOutputVec(e->outputID);
                        e->inVec = nodes[e->inNodeID]->getInputVec(e->inputID);
                        
                        ofLog() << "disconnect " << e->outNodeID << " from " << e->inNodeID;
                        et = edges.erase(et);
                        co->disconnect(e->outNodeID, e->inNodeID, e->outputID, nodes[e->inNodeID]->inputInfo[e->inputID]);
                        
                        delete e;
                    }
                }
                co->deleteModule(infoNode->nID, infoNode->mtkn->tID);//ノード番号、モジュールマネージャ番号
                delete nodes[infoNode->nID];
                nodes.erase(infoNode->nID);
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
        if (touchingNodes.size() == 1)//他にタッチしてなければインフォ表示
            infoNode = touchingNodes[touch.id];
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
