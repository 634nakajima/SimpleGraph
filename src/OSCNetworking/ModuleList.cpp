/*
 *  ModuleList.cpp
 *  
 *
 *  Created by kannolab on 12/10/05.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "ModuleList.h"
#include <iostream>

ModuleList::ModuleList(Server *s, const char *osc) : Module(s,osc)
{
    addMethodToServer("/setMList", "ssssi", setMList, this);
    addMethodToTCPServer("/setMList", "ssssi", setMListTCP, this);
    addMethodToServer("/deleteMList", "ssi", deleteMList, this);
    addMethodToTCPServer("/deleteMList", "ssi", deleteMList, this);
    requestML();
}

int ModuleList::setMList(const char   *path, 
                         const char   *types, 
                         lo_arg       **argv, 
                         int          argc,
                         void         *data,
                         void         *user_data)
{
    ModuleList *mlc = (ModuleList *)user_data;
    int i = 0;
    char ip[16];
    strcpy(ip, mlc->getSenderIP());
    //エラー処理、既存のモジュールリスト確認
    for (std::list<MToken*>::iterator iter = mlc->mList.begin(); iter != mlc->mList.end(); iter++) {
        MToken* ml = (*iter);
        if (strcmp(ml->ip, ip)==0) {
            if (strcmp(ml->osc, (char *)argv[1])==0) {
                return 0;
            }
        }
		i++;
    }
    
    //モジュールリストの生成
    MToken *m = new MToken();
    if (argv[4]->i != -1) {//mColorが-1でなければ
        m->mColor = argv[4]->i;
		m->tID = i;//tIDをモジュールリストのインデックスに使う
        strcpy(m->ip, ip);
        strcpy(m->osc, (char *)argv[1]);
        m->setInputInfo((char *)argv[2]);
        m->setOutputInfo((char *)argv[3]);
        mlc->mList.push_back(m);
        
        //モジュールインデックスをモジュールマネージャに送信
        void *data;
        unsigned long d_len;
        
        lo_message lm = lo_message_new();
        lo_message_add_int32(lm, m->tID);
        
        data = lo_message_serialise(lm, m->osc, NULL, NULL);
        d_len = lo_message_length(lm, m->osc);
        
        lo_address lo_ip = lo_address_new_with_proto(LO_TCP, m->ip, "6341");
        if (strcmp(mlc->IPAddr,lo_address_get_hostname(lo_ip))==0) {
            lo_server_dispatch_data(lo_server_thread_get_server(mlc->st->st_tcp), data, d_len);
        }else {
            lo_send_message(lo_ip, m->osc, lm);
        }
        
        lo_message_free(lm);
        lo_address_free(lo_ip);
        free(data);
        
        printf("ModuleList_Set:%s %s Module Index:%d\n",m->ip, m->osc, m->tID);
    }

    return 0;
	
}

int ModuleList::setMListTCP(const char   *path,
                            const char   *types,
                            lo_arg       **argv,
                            int          argc,
                            void         *data,
                            void         *user_data)
{
    ModuleList *mlc = (ModuleList *)user_data;
    int i = 0;
    char ip[16];
    strcpy(ip, mlc->getSenderTCPIP());
    //エラー処理、既存のモジュールリスト確認
    for (std::list<MToken*>::iterator iter = mlc->mList.begin(); iter != mlc->mList.end(); iter++) {
        MToken* ml = (*iter);
        if (strcmp(ml->ip, ip)==0) {
            if (strcmp(ml->osc, (char *)argv[1])==0) {
                return 0;
            }
        }
        i++;
    }
    
    //モジュールリストの生成
    MToken *m = new MToken();
    if (argv[4]->i != -1) {//mColorが-1でなければ
        m->mColor = argv[4]->i;
        m->tID = i;//tIDをモジュールリストのインデックスに使う
        strcpy(m->ip, ip);
        strcpy(m->osc, (char *)argv[1]);
        m->setInputInfo((char *)argv[2]);
        m->setOutputInfo((char *)argv[3]);
        mlc->mList.push_back(m);
        
        //モジュールインデックスをモジュールマネージャに送信
        void *data;
        unsigned long d_len;
        
        lo_message lm = lo_message_new();
        lo_message_add_int32(lm, m->tID);
        
        data = lo_message_serialise(lm, m->osc, NULL, NULL);
        d_len = lo_message_length(lm, m->osc);
        
        lo_address lo_ip = lo_address_new_with_proto(LO_TCP, m->ip, "6341");
        if (strcmp(mlc->IPAddr,lo_address_get_hostname(lo_ip))==0) {
            lo_server_dispatch_data(lo_server_thread_get_server(mlc->st->st_tcp), data, d_len);
        }else {
            lo_send_message(lo_ip, m->osc, lm);
        }
        
        lo_message_free(lm);
        lo_address_free(lo_ip);
        free(data);
        
        printf("ModuleList_Set:%s %s Module Index:%d\n",m->ip, m->osc, m->tID);
    }
    
    return 0;
    
}

int ModuleList::deleteMList(const char   *path, 
                            const char   *types, 
                            lo_arg       **argv, 
                            int          argc,
                            void         *data,
                            void         *user_data)
{
	ModuleList *mlc = (ModuleList *)user_data;
    char ip[16];
    strcpy(ip, mlc->getSenderIP());
    
    //エラー処理、既存のモジュールリスト確認
    for (std::list<MToken*>::iterator iter = mlc->mList.begin(); iter != mlc->mList.end(); iter++) {
        MToken* ml = (*iter);
        if (strcmp(ml->ip, ip)==0) {
            if (strcmp(ml->osc, (char *)argv[1])==0) {
                printf("delete:%s,%s ModuleIndex:%d\n",ml->ip, ml->osc, ml->tID);
                mlc->mList.remove(ml);
                delete ml;
                return 0;
            }
        }
    }
    
    return 0;
	
}

int ModuleList::stream(const char   *path, 
					   const char   *types, 
					   lo_arg       **argv, 
					   int          argc,
					   void         *data, 
					   void         *user_data)
{
	ModuleList *mlc = (ModuleList *)user_data;
	lo_blob b = (lo_blob)argv[0];
    char *dp = (char *)lo_blob_dataptr(b);
    //int size = lo_blob_datasize(b);
	
	mlc->t = *dp;
	return 0;

}

void ModuleList::createModule(char *tID, MToken *ml)
{
    void *data;
    unsigned long d_len;
    printf("13:%s,%s\n",ml->ip,ml->osc);

    lo_message m = lo_message_new();
    lo_message_add_int32(m, 1);
    lo_message_add_string(m, tID);
    
    data = lo_message_serialise(m, ml->osc, NULL, NULL);
    d_len = lo_message_length(m, ml->osc);
    
    lo_address lo_ip = lo_address_new_with_proto(LO_TCP, ml->ip, "6341");
    if (strcmp(this->IPAddr,lo_address_get_hostname(lo_ip))==0) {
        lo_server_dispatch_data(lo_server_thread_get_server(st->st_tcp), data, d_len);
        printf("dame:%s,%s\n",ml->ip,ml->osc);

    }else {
        lo_send_message(lo_ip, ml->osc, m);
        printf("yoi:%s,%s\n",ml->ip,ml->osc);
    }
    
    lo_message_free(m);
    lo_address_free(lo_ip);
    free(data);
}

void ModuleList::deleteModule(char *tID, MToken *ml)
{
    void *data;
    unsigned long d_len;
    
    lo_message m = lo_message_new();
    lo_message_add_int32(m, 0);
    lo_message_add_string(m, tID);
    
    data = lo_message_serialise(m, ml->osc, NULL, NULL);
    d_len = lo_message_length(m, ml->osc);
    
    lo_address lo_ip = lo_address_new_with_proto(LO_TCP, ml->ip, "6341");
    if (strcmp(this->IPAddr,lo_address_get_hostname(lo_ip))==0) {
        lo_server_dispatch_data(lo_server_thread_get_server(st->st_tcp), data, d_len);
    }else {
        lo_send_message(lo_ip, ml->osc, m);
    }
    
    lo_message_free(m);
    lo_address_free(lo_ip);
    free(data);
}

void ModuleList::createModule(int tID, MToken *ml)
{
	char t[4];
    if (tID < 10) {
        t[0] = (tID + 0x30);
        t[1] = '\0';
    }else if (tID < 100) {
        t[0] = (tID/10 + 0x30);
        t[1] = (tID%10 + 0x30);
        t[2] = '\0';
    }else {
        t[0] = '1';
        t[1] = ((tID%100)/10 + 0x30);
        t[2] = ((tID%100)%10 + 0x30);
        t[3] = '\0';
    }
	createModule(t, ml);
}

void ModuleList::deleteModule(int tID, MToken *ml)
{
	char t[4];
    if (tID < 10) {
        t[0] = (tID + 0x30);
        t[1] = '\0';
    }else if (tID < 100) {
        t[0] = (tID/10 + 0x30);
        t[1] = (tID%10 + 0x30);
        t[2] = '\0';
    }else {
        t[0] = '1';
        t[1] = ((tID%100)/10 + 0x30);
        t[2] = ((tID%100)%10 + 0x30);
        t[3] = '\0';
    }
	deleteModule(t, ml);
}

void ModuleList::createModule(char *tID, int mc)
{
	for (std::list<MToken*>::iterator iter = mList.begin(); iter != mList.end(); iter++) {
        MToken* ml = (*iter);
        if (mc == ml->tID) {
			createModule(tID, ml);
			break;
		}
    }
}

void ModuleList::deleteModule(char *tID, int mc)
{
	for (std::list<MToken*>::iterator iter = mList.begin(); iter != mList.end(); iter++) {
        MToken* ml = (*iter);
        if (mc == ml->tID) {
			deleteModule(tID, ml);
			break;
		}
    }
}

void ModuleList::createModule(int tID, int mc)
{
	char t[4];
    if (tID < 10) {
        t[0] = (tID + 0x30);
        t[1] = '\0';
    }else if (tID < 100) {
        t[0] = (tID/10 + 0x30);
        t[1] = (tID%10 + 0x30);
        t[2] = '\0';
    }else {
        t[0] = '1';
        t[1] = ((tID%100)/10 + 0x30);
        t[2] = ((tID%100)%10 + 0x30);
        t[3] = '\0';
    }

    for (std::list<MToken*>::iterator iter = mList.begin(); iter != mList.end(); iter++) {
        MToken* ml = (*iter);
        if (mc == ml->tID) {
			createModule(t, ml);
			break;
		}
    }
}

void ModuleList::deleteModule(int tID, int mc)
{	
	char t[4];
    if (tID < 10) {
        t[0] = (tID + 0x30);
        t[1] = '\0';
    }else if (tID < 100) {
        t[0] = (tID/10 + 0x30);
        t[1] = (tID%10 + 0x30);
        t[2] = '\0';
    }else {
        t[0] = '1';
        t[1] = ((tID%100)/10 + 0x30);
        t[2] = ((tID%100)%10 + 0x30);
        t[3] = '\0';
    }
	
	for (std::list<MToken*>::iterator iter = mList.begin(); iter != mList.end(); iter++) {
        MToken* ml = (*iter);
        if (mc == ml->tID) {
			deleteModule(t, ml);
			break;
		}
    }	
}

void ModuleList::requestML()
{
	int sock;
    unsigned long n, d_len;
    struct sockaddr_in addr;
    void *data; 
    char path[] = "/ModuleManager/RequestML";
    char p[64];
    //int  mColor;
    strcpy(p, OSCAddr);
	
	//create lo_message
	lo_message m = lo_message_new();
	lo_message_add_int32(m, 0);
	
	
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
    free(data);
    close(sock);	
}

void ModuleList::displayModules()
{
    for (std::list<MToken*>::iterator iter = mList.begin(); iter != mList.end(); iter++) {
        MToken* ml = (*iter);
        printf("ip:%s osc:%s\n",ml->ip,ml->osc);
    }
}

ModuleList::~ModuleList()
{
    for (std::list<MToken*>::iterator iter = mList.begin(); iter!=mList.end(); iter++) {
		mList.remove(*iter);
		delete (*iter);
	}
	deleteMethodFromServer("/setMList", "ssi");
    deleteMethodFromTCPServer("/setMList", "ssi");
    deleteMethodFromServer("/deleteMList", "ssi");
    deleteMethodFromTCPServer("/deleteMList", "ssi");
}





