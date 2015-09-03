//
//  CoffeeMill.cpp
//  RTSOSCApp
//
//  Created by Musashi Nakajima on 2015/04/11.
//  Copyright (c) 2015年 Musashi Nakajima. All rights reserved.
//

#include "CoffeeMill.h"

#pragma mark ----- CoffeeMill -----

CoffeeMill::CoffeeMill(Server *s, const char *osc) : Module(s,osc)
{
    d = NULL;
}

int CoffeeMill::something(const char   *path,
                          const char   *types,
                          lo_arg       **argv,
                          int          argc,
                          void         *data,
                          void         *user_data)
{
    CoffeeMill *cm = (CoffeeMill *)user_data;
    if (cm->d) cm->sendData(cm->d, argv, argc);
    return 0;
}



CoffeeMill::~CoffeeMill()
{
    if (d) delete d;
}

#pragma mark ----- CMManager -----

void CMManager::init(Server *s, const char *osc)
{
    rp = 0;
    wp = 0;
    ModuleManager::init(s, osc);
    //serial.setCallback(serialCallback, this);
    setMInfo("/CoffeeMill", "", "/Handle");//Module Address, Callback Function
}

Module *CMManager::initModule(Server *s, const char *osc)
{
    return new CoffeeMill(s, osc);
}

void CMManager::setData(int d, int dataID)
{
    for (std::list<Module *>::iterator iter = mList.begin(); iter != mList.end(); iter++) {
        CoffeeMill *m = (CoffeeMill *)(*iter);
        if (m->d) delete m->d;
        m->d = new Data(d,dataID);
        m->sendDataTo(dataID, 0, 0);
    }
}

void CMManager::parseData()
{
    unsigned int value;
    unsigned char a64[8], a16[2], data[4];
    
    if (available() > 16) {
        while (available() > 0) {
            if (readData() == 0x7E) {
                for (int i = 0; i < 2; i++) {
                    readData();
                }
                data[0] = readData();
                a64[0] = readData();
                a64[1] = readData();
                a64[2] = readData();
                a64[3] = readData();
                a64[4] = readData();
                a64[5] = readData();
                a64[6] = readData();
                a64[7] = readData();
                
                a16[0] = readData();
                a16[1] = readData();
                
                data[1] = readData();
                
                value = readData();
                
                data[2] = readData();
                
                long sum = 0;
                for (int i=0; i<2; i++) {
                    sum += data[i];
                }
                for (int i=0; i<8; i++) {
                    sum += a64[i];
                }
                for (int i=0; i<2; i++) {
                    sum += a16[i];
                }
                sum += value;
                
                data[3] = (0xFF - (sum & 0xFF));
                
                if(data[2] != data[3]) {
                    printf("err:xbee\n");
                    return;
                }
                value = value*10;
                if(value > 250) value = 250;
                setData(1, value);
                printf("value:%d\n", value);
            }
        }
    }
    
}

char CMManager::readData()
{
    char value;
    value = buf[rp];
    rp = (rp+1)%1024;
    return value;
}

int CMManager::available()
{
    if (rp <= wp) {
        return (wp-rp);
    }else {
        return (1024+wp-rp);
    }
}

void CMManager::serialCallback(void *data, int size, void *user_data) {
    CMManager *cmm = (CMManager *)user_data;
    char *dp = (char *)data;
    for (int i = 0; i < size; i++) {
        cmm->buf[cmm->wp] = *dp++;
        cmm->wp = (cmm->wp+1)%1024;
    }
    cmm->parseData();
}