//
//  ofxPONK.h
// 
//
//  Created by Tim Redfern March 2023
//
//  PONK (Pathes Over NetworK) is a minimal protocol to transfer 2D colored pathes from a source to
//  a receiver. It has been developped to transfer laser path from a software to another over network using UDP.
//
//  https://github.com/madmappersoftware/Ponk
//                    

#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "colourPolyline.h"
#include "PonkDefs.h"
#include "DatagramSocket.h"

#include "ofxHelios.h"

string ipv4_int_to_string(uint32_t in, bool *const success=nullptr);
uint32_t ipv4_string_to_int(const string &in, bool *const success=nullptr);

class ofxPONKSender
{
public:
	GenericAddr dest;

    DatagramSocket socket;
    
    ofxPONKSender(unsigned int ip = ((127 << 24) + (0 << 16) + (0 << 8) + 1),unsigned short port = PONK_PORT): socket(INADDR_ANY,0){ 
        dest.family=AF_INET;
        dest.ip=ip;
        dest.port=port;
        enabled=true;
        ofLog()<<"PONKSender "<<ipv4_int_to_string(ip)<<":"<<ofToString(port);   
    }

    ~ofxPONKSender(){
        //delete socket;
    }
    ofParameter<bool> enabled;
    int draw(colourPolyline &line, int intensity=255);
    int draw(ofPolyline &line,ofColor colour=ofColor(255,255,255),int intensity=255);
    int draw(vector <ofPolyline> &lines,ofColor colour=ofColor(255,255,255),int intensity=255);
    int draw(vector <colourPolyline> &lines, int intensity=255);
};

class ofxPONKSenderPanel: public ofxPONKSender{
public:
    ofxPONKSenderPanel(string ip="127.0.0.1",int port=PONK_PORT){
        ofxPONKSender(ipv4_string_to_int(ip),port);
    }
    void ipChanged(string & _ip){
        bool success;
        uint32_t int_ip=ipv4_string_to_int(_ip,&success);
        if (success){
            ofLog()<<"set ip: "<<_ip;
            dest.ip=int_ip;
        }
        else {
            ip=ipv4_int_to_string(dest.ip);
        }
    }
    void portChanged(string & _port){
        int32_t int_port=ofToInt(_port);
        if (int_port>-1&&int_port<65536&&ofToString(int_port)==_port){
            ofLog()<<"set port: "<<_port;
            dest.port=int_port;
        }
        else {
            port=ofToString(dest.port);
        }
    }
    void setup(string name="PONK sender",int x=10,int y=10){
        ip.addListener(this,&ofxPONKSenderPanel::ipChanged);
        port.addListener(this,&ofxPONKSenderPanel::portChanged);
        panel.setup(name,"",x,y); //TODO save pos
        panel.add(enabled.set("enable",true));
        panel.add(ip.set("ip",ipv4_int_to_string(dest.ip)));
        panel.add(port.set("port",ofToString(dest.port)));
    }
    ofParameter<string> ip;
    ofParameter<string> port;
    ofxPanel panel;
};