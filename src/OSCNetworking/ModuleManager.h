//
//  ModuleManager.h
//  
//
//  Created by NAKAJIMA Musashi on 12/12/18.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef _ModuleManager_h
#define _ModuleManager_h

#include <stdio.h>
#include <stdlib.h>
#include "Module.h"
#include "Tile.h"
#include <list>

enum {module_new, module_delete};

class ModuleManager : public Module
{
public:
    char MAddr[32];
    int mID;
    bool local;
    std::list<Module *>         mList;
    const char *inInfo, *outInfo;
    
    ModuleManager(Server *s, const char *osc);
    ModuleManager();
    ~ModuleManager();
    
    virtual void init(Server *s, const char *osc);
    virtual void init(Server *s, const char *osc, const char *coAddr);
    void setMInfo(const char *mAddr, const char* input, const char* output);
    void setMInfo(const char *mAddr);
    void setInputInfo(const char *inAddr);
    void setOutputInfo(const char *outAddr);
    void sendModuleList(int status);
    virtual Module *initModule(Server *s, const char *osc);
    Module *getModule(Tile *t);
    
private:
    static int module(const char   *path,
                      const char   *types,
                      lo_arg       **argv,
                      int          argc,
                      void         *data,
                      void         *user_data);
    
	static int requestML(const char   *path, 
						 const char   *types, 
						 lo_arg       **argv, 
						 int          argc,
						 void         *data, 
						 void         *user_data);

    static int mIndex(const char   *path,
                      const char   *types,
                      lo_arg       **argv,
                      int          argc,
                      void         *data,
                      void         *user_data);
    
};

#endif
