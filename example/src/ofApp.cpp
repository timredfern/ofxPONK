#include "ofApp.h"

#define NUM_POINTS 100
#define CIRCLE_RADIUS 800
#define CIRCLE_SPACING 300

//--------------------------------------------------------------
void testApp::setup(){

    laser.setup();

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

    1. how hard is it to make/ storr mapping
    2. what do we want to do with it
    3. hw to make M divide drawing amongst lasers


    todraw.draw();
    */

    float pfactor=(PI*2)/NUM_POINTS;

    vector <colourPolyline> lines;

    for (int j=0;j<4;j++){

        float circle_radius=sin((ofGetElapsedTimef()+(j*PI))*.1)*CIRCLE_RADIUS;

        ofPoint centre(
            (ofGetWidth()/2)+(cos(ofGetElapsedTimef()+(j*PI*0.5))*CIRCLE_SPACING),
            (ofGetHeight()/2)+(sin(ofGetElapsedTimef()+(j*PI*0.5))*CIRCLE_SPACING)
            );

        colourPolyline line;

        for (int i=0;i<=NUM_POINTS;i++){
                line.addVertex(
                    (cos(float(i)*pfactor)*circle_radius)+centre.x,
                    (sin(float(i)*pfactor)*circle_radius)+centre.y,
                    (sin(float(i+(ofGetElapsedTimef()*30*j))*pfactor)+1)*128,
                    (sin(float(i+(ofGetElapsedTimef()*57*j))*pfactor)+1)*128,
                    (sin(float(i+(ofGetElapsedTimef()*101*(j)))*pfactor)+1)*128
                    );
        }
        lines.push_back(line);
        line.draw();
    }

    int num = laser.draw(lines,255);

    if (num>0){
        ofSetWindowTitle(ofToString(ofGetFrameRate(), 2)+" fps laser points: "+ofToString(num));
    }
    else {
        ofSetWindowTitle("laser error ");
    }

    laser.panel.draw();

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
