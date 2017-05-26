#pragma once

#include "ofMain.h"
#include "ofxVoronoi.h"
#include "ofxDatGui.h"

class ofApp : public ofBaseApp{
    private:
        vector <ofPoint> generateRandomPoints(int count, int seed, ofRectangle bounds, int _p);
        ofxVoronoi voronoi;
        vector<ofPoint> points;
        vector<ofColor> cols;
        ofVec3f centre;
        ofxDatGui gui;
    int bpm;
    

    float disp;
    int border_cell_no;
    
        bool isBorder(ofPoint _pt);
    
    
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void createFullScreenQuad();
    
    
        ofxVoronoi chillax(ofxVoronoi _v);
        ofFbo vx_fbo;
        ofFbo sh_fbo;
    
    ofShader shady;
    ofShader thick;
    
    ofVboMesh   m_fsQuadVbo;
};
