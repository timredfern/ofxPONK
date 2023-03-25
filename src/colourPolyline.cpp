#include "colourPolyline.h"


void colourPolyline::addVertex( float x, float y){
	addVertex(ofPoint(x,y));
}

void colourPolyline::addVertex( const ofPoint& p ){
	addVertex(p, ofColor(255,255,255));
}

void colourPolyline::addVertex( float x, float y, int r, int g, int b){
	addVertex(ofPoint(x,y),ofColor(r,g,b));
}

void colourPolyline::addVertex( const ofPoint& p , int r, int g, int b){
	addVertex(p,ofColor(r,g,b));
}

void colourPolyline::addVertex( const ofPoint& p , const ofColor& c){
	ofPolyline::addVertex(p);
	pointColours.push_back(c);
}



const ofColor colourPolyline::getColourAt(const int i){
	return pointColours[i];
}

void colourPolyline::draw(){
	for (int i=0;i<ofPolyline::size()-1;i++){
		ofSetColor(pointColours[i]);
		ofPolyline l;
		l.addVertex(ofPolyline::operator[](i)); 
		l.addVertex(ofPolyline::operator[](i+1));
		l.draw();
		//todo - cache
	}
}
