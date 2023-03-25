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
#include "colourPolyline.h"
//#include "PonkDefs.h"
#include "DatagramSocket.h"

class ofxPONKsender
{
public:
	GenericAddr dest;

    DatagramSocket socket;
    
    ofxPONKsender(unsigned int ip = ((127 << 24) + (0 << 16) + (0 << 8) + 1),unsigned short port = 0) : socket(INADDR_ANY,0){
    	dest.family=0;
        dest.ip=ip;
    	dest.port=port;    
    }

    ~ofxPONKsender(){
        //delete socket;
    }

    int draw(colourPolyline &line, int intensity=255);
    int draw(ofPolyline &line,ofColor colour=ofColor(255,255,255),int intensity=255);
    int draw(vector <ofPolyline> &lines,ofColor colour=ofColor(255,255,255),int intensity=255);
    int draw(vector <colourPolyline> &lines, int intensity=255);
};