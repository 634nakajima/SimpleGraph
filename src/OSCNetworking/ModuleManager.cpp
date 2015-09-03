//
//  ModuleManager.cpp
//  
//
//  Created by NAKAJIMA Musashi on 12/12/18.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "ModuleManager.h"

ModuleManager::ModuleManager()
{}

ModuleManager::ModuleManager(Server *s, const char *osc) : Module(s,osc)
{
    init(s, osc);
}

void ModuleManager::init(Server *s, const char *osc) {
    init(s, osc, "");
    local = false;
}

void ModuleManager::init(Server *s, const char *osc, const char *coAddr)
{
    Module::init(s, osc);
    lo_server_thread_add_method(st->st, "/ModuleManager/RequestML", "i", requestML, this);
    inInfo = NULL;
    outInfo = NULL;
    strcpy(CoIP,coAddr);
    MAddr[0] = '\0';
    mID = 0;
    local = true;
}

void ModuleManager::sendModuleList(int status)
{
    int sock;
    unsigned long n, d_len;
    struct sockaddr_in addr;
    void *data; 
    char path[30];
    char p[64];
    int  mColor;
    
    if (status == module_new)
        strcpy(path, "/ModuleList/setMList");
    else if (status == module_delete)
        strcpy(path, "/ModuleList/deleteMList");

    strcpy(p, OSCAddr);
    strcat(p, MAddr);
    mColor = 0;
    
    if (local) {
        lo_address lo_ip = lo_address_new_with_proto(LO_TCP, CoIP, "6341");
        lo_send(lo_ip,
                path,
                "ssssi",
                IPAddr,
                p,
                inInfo,
                outInfo,
                mID);
        lo_address_free(lo_ip);
    }else {
        //create lo_message
        lo_message m = lo_message_new();
        lo_message_add_string(m, IPAddr);
        lo_message_add_string(m, p);
        lo_message_add_string(m, inInfo);
        lo_message_add_string(m, outInfo);
        lo_message_add_int32(m, mID);
        
        data = lo_message_serialise(m, path, NULL, NULL);
        d_len = lo_message_length(m, path);
        
        //create socket
        int opt = 1;
        sock = socket(AF_INET, SOCK_DGRAM, 0);
        setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(int));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(6340);
        inet_pton(AF_INET, "255.255.255.255", &addr.sin_addr.s_addr);
        
        //send(念のため3回)
        for (int j=0; j<3; j++) {
            n = sendto(sock, data, d_len, 0, (struct sockaddr *)&addr, sizeof(addr));
            if (n < 1) {
                perror("sendto");
            }
            usleep(1000);
        }
        lo_message_free(m);
        close(sock);
    }
}

void ModuleManager::setMInfo(const char *mAddr) {
    strcpy(MAddr, mAddr);
    addMethodToTCPServer(MAddr, "i", mIndex, this);//set Module Index
    addMethodToTCPServer(MAddr, "is", module, this);//(1:create 0:delete, tID) (2:set Module Index, mID)

    sendModuleList(module_new);
}

void ModuleManager::setMInfo(const char *mAddr, const char* input, const char* output) {
    strcpy(MAddr, mAddr);
    addMethodToTCPServer(MAddr, "i", mIndex, this);//set Module Index
    addMethodToTCPServer(MAddr, "is", module, this);//(1:create 0:delete, tID) (2:set Module Index, mID)
    inInfo = input;
    outInfo = output;
    sendModuleList(module_new);
}

void ModuleManager::setInputInfo(const char *inAddr) {
    inInfo = inAddr;
}

void ModuleManager::setOutputInfo(const char *outAddr) {
    outInfo = outAddr;
}

Module *ModuleManager::initModule(Server *s, const char *osc)
{
    return new Module(s, osc);
}

int ModuleManager::requestML(const char   *path,
                             const char   *types,
                             lo_arg       **argv,
                             int          argc,
                             void         *data, 
                             void         *user_data)
{
    ModuleManager *mm = (ModuleManager *)user_data;
	mm->sendModuleList(module_new);
	return 1;
}

int ModuleManager::mIndex(const char   *path,
                          const char   *types,
                          lo_arg       **argv,
                          int          argc,
                          void         *data,
                          void         *user_data)
{
    ModuleManager *mm = (ModuleManager *)user_data;
    mm->mID = argv[0]->i;
    return 0;
}

int ModuleManager::module(const char   *path,
                          const char   *types,
                          lo_arg       **argv,
                          int          argc,
                          void         *data,
                          void         *user_data)
{
    ModuleManager *mm = (ModuleManager *)user_data;
    char p[64];
    strcpy(p, mm->OSCAddr);         //ModuleManagerアドレス
    strcat(p, mm->MAddr);          //ModuleManagerが管理するModuleアドレス
    strcat(p, "/Tile");
    strcat(p, &argv[1]->s);         //タイルID
    if (argv[0]->i) {//argv[0] = 1:モジュール生成 0:モジュール解放
        for (std::list<Module *>::iterator iter = mm->mList.begin(); iter != mm->mList.end(); iter++) {
            Module *m = (*iter);
            if (strcmp(p,m->OSCAddr)==0) {
                if (m->tID == atoi(&argv[1]->s)) {
                    printf("err: Creating %s\n",&mm->MAddr[1]);
                    return 0;
                }
            }
        }
        
        Module *m = mm->initModule(mm->st, p);//モジュール生成
        m->setCoIP();
        m->setTID(atoi(&argv[1]->s));//タイルIDの設定
        m->mColor = mm->mID;//モジュールカラーの設定
        m->sendSetMdtkn();//コーディネータにモジュールトークン生成要求
        mm->mList.push_back(m);
        printf("create %s\n",&mm->MAddr[1]);
        
    }else {
        for (std::list<Module *>::iterator iter = mm->mList.begin(); iter != mm->mList.end();) {
            Module *m = (*iter);
            if (strcmp(p,m->OSCAddr)==0) {
                iter = mm->mList.erase(iter);
                delete m;
                printf("delete %s\n",&mm->MAddr[1]);
                break;
            }
            iter++;
        }
    }
    return 0;
}

Module *ModuleManager::getModule(Tile *t) {
    Module *m = NULL;
    auto it = mList.begin();
    while(it != mList.end()) {
        Module *tm = *it;
        if(tm->tID == t->tID)
            return tm;
        ++it;
    }
    return m;
}

ModuleManager::~ModuleManager()
{
    for (std::list<Module *>::iterator iter = mList.begin(); iter != mList.end();) {
        Module *m = (*iter);
        iter = mList.erase(iter);
        delete m;
    }
    sendModuleList(module_delete);
    lo_server_thread_del_method(st->st, "ModuleManager/RequestML", "i");
    deleteMethodFromTCPServer(MAddr, "is");
    deleteMethodFromTCPServer(MAddr, "i");
}


