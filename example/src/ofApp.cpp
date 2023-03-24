#include "ofApp.h"

#define NUM_POINTS 1000
#define CIRCLE_RADIUS 300

//--------------------------------------------------------------
void testApp::setup(){

}

//--------------------------------------------------------------
void testApp::update(){
    

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
	ofSetColor(255,255,255);
/*
	if (line.size()){
		current++;
	}
	ofPolyline todraw;
    for (int i=max(0,min(current,(int)line.size()))	;i<line.size();i++){
    	todraw.addVertex(line[i]);
    }

    what should the default coordinate system be?
    centred?

    the coordinate system goes from 0-4096 in X and Y
    It's preferable that drawing is centred around 2000

    Let's offset the centre of the screen to 0 and draw natively from there
    Let's apply a scaling factor which can increase coords
    And a calibration/ warp

    The aim is to be able to preview in any size window


    todraw.draw();
    */

    float pfactor=(PI*2)/NUM_POINTS;

    vector <colourPolyline> lines;

    for (int k=0;k<2;k++){
        for (int j=0;j<2;j++){

            colourPolyline line;

            for (int i=0;i<NUM_POINTS+1;i++){
                line.addVertex(
                    ((cos(float(i)*pfactor)+1)*CIRCLE_RADIUS)+(ofGetWidth()/2)+(-(CIRCLE_RADIUS+200)+(k*400)),
                    ((sin(float(i)*pfactor)+1)*CIRCLE_RADIUS)+(ofGetHeight()/2)+(-(CIRCLE_RADIUS+200)+(j*400)),
                    (sin(float(i+(ofGetElapsedTimef()*30*j))*pfactor)+1)*128,
                    (sin(float(i+(ofGetElapsedTimef()*57*k))*pfactor)+1)*128,
                    (sin(float(i+(ofGetElapsedTimef()*101*(j+k)))*pfactor)+1)*128
                    );
            }
            lines.push_back(line);
            line.draw();
        }
    }

    int num = 0; //laser.draw(lines,255);

    if (num>0){
        ofSetWindowTitle(ofToString(ofGetFrameRate(), 2)+" fps laser points: "+ofToString(num));
    }
    else {
        ofSetWindowTitle("laser error ");
    }

    

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){



	/// int i = 1;
	/// while ( i<bounds.size()) {
	/// 	float angle = atan2(line[i-1].y - line[i].y, line[i-1].x - line[i].x);  
	/// 	bounds[i].set(bounds[i-1].x - cos(angle) * 20, bounds[i-1].y - sin(angle) * 20);
	/// 	i++;
	/// }

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	//line.addVertex(x, y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

	//line.clear();
	//current=-100;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
