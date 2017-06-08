#pragma once

#include "ofMain.h"
#include "ofxVoronoi.h"
#include "ofxDatGui.h"
#include "ofxAudioAnalyzer.h"
#include "ofSoundPlayerExtended.h"

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
    
        ofxAudioAnalyzer audioAnalyzer;
        ofSoundPlayerExtended player;
        bool isOnset_c1;
        bool isOnset_c2;
    
        int sampleRate;
        int bufferSize;
        ofSoundBuffer soundBuffer;
        float pitchFreq_c1;
        float pitchFreq_c2;
        float pitchConf_c1;
        float pitchConf_c2;
        float smoothing;
    
    
        ofxVoronoi chillax(ofxVoronoi _v);
        ofFbo vx_fbo;
    ofFbo al_fbo;
  
    ofShader shady;
    ofShader alpha;
    ofShader thick;

    
    ofVboMesh   m_fsQuadVbo;
};
