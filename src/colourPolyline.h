#pragma once
#include "ofPolyline.h"
#include "ofMain.h"

class colourPolyline: public ofPolyline {
	public:

		colourPolyline(){}

		colourPolyline(const ofPolyline line,const ofColor colour=ofColor(255,255,255)){
			for (auto& point:line){
				addVertex(point, colour);
			}
		}

		void clear(){
			ofPolyline::clear();
			pointColours.clear();
		}

		void addVertex( float x, float y);

		void addVertex( const ofPoint& p );

		void addVertex( float x, float y, int r, int g, int b);

		void addVertex( const ofPoint& p , int r, int g, int b);

		void addVertex( const ofPoint& p , const ofColor& c);

		void draw();

		const ofColor getColourAt(const int i);

	private:

		vector <ofColor> pointColours;

};