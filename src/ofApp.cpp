#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableAlphaBlending();
    ofDisableArbTex();
    //ofEnableNormalizedTexCoords();
    
    sampleRate = 44100;
    bufferSize = 512;
    int channels = 2;
    
    audioAnalyzer.setup(sampleRate, bufferSize, channels);
    
    player.load("test.mp3");
    player.play();
    player.setLoop(true);
    
    gui = ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
    gui.addTextInput("CONTROL_GUI");
    
    gui.addFRM();
    gui.addBreak();
    gui.addSlider("BPM", 50.0, 200, 120.0);
    gui.addSlider("thickness",0.0,50.0,8.0);
    gui.addToggle("shader");
    gui.addSlider("alpha", 0.0, 1.0,0.2);
    gui.addSlider("silence_threshold", 0.0, 1.0, 0.06);
    gui.addToggle("use_time_threshold");
    gui.addSlider("time_threshold", 0.0, 1000.0, 200.0);
    gui.addSlider("smooth", 0.0, 1.0,0.1);
    gui.addFooter();
    gui.getToggle("use_time_threshold")->setChecked(true);

    ofSetWindowTitle("RupertM_Assignment3");
    ofBackground(255);
    disp = (ofGetWidth()/2)-((ofGetHeight()-20)/2);
    ofRectangle bounds = ofRectangle(10, 10, ofGetWidth()-20, ofGetHeight()-20);
    
    vx_fbo.allocate(ofGetWidth(), ofGetHeight(),GL_RGBA);
    vx_fbo.begin();
    ofClear(255, 255, 255,0);
    vx_fbo.end();
    al_fbo.allocate(ofGetWidth(), ofGetHeight(),GL_RGBA);
    al_fbo.begin();
    ofClear(255, 255, 255,0);
    al_fbo.end();
    border_cell_no = floor(30/10+10);
    centre.x = (ofGetWidth()/2);
    centre.y = (ofGetHeight()/2);
    centre.z = (0.0);
    points = generateRandomPoints(30, 1, bounds, border_cell_no);
    shady.load("shader");
    alpha.load("alpha");
    thick.load("thickness.vert", "thickness.frag","thickness.geom");
    voronoi.setBounds(bounds);
    voronoi.setPoints(points);
    
    voronoi.generate();
    

    createFullScreenQuad();
}

//--------------------------------------------------------------
void ofApp::update(){
    float alpha = gui.getSlider("alpha")->getValue();
    bool t_thresh = gui.getToggle("use_time_threshold")->getChecked();
    float s_thresh = gui.getSlider("silence_threshold")->getValue();
    float t_time = gui.getSlider("time_threshold")->getValue();
    float smooth = gui.getSlider("smooth")->getValue();
    soundBuffer = player.getCurrentSoundBuffer(bufferSize);
    audioAnalyzer.setOnsetsParameters(0, alpha, s_thresh, t_time,t_thresh);
    audioAnalyzer.analyze(soundBuffer);
    isOnset_c1 = audioAnalyzer.getOnsetValue(0);
    pitchFreq_c1 = audioAnalyzer.getValue(PITCH_FREQ, 0,smooth);
    pitchConf_c1 = audioAnalyzer.getValue(PITCH_CONFIDENCE, 0,smooth);
    audioAnalyzer.setOnsetsParameters(1, alpha, s_thresh, t_time,t_thresh);
    audioAnalyzer.analyze(soundBuffer);
    isOnset_c2 = audioAnalyzer.getOnsetValue(1);
    pitchFreq_c2 = audioAnalyzer.getValue(PITCH_FREQ, 1,smooth);
    pitchConf_c2 = audioAnalyzer.getValue(PITCH_CONFIDENCE, 1,smooth);

}

//--------------------------------------------------------------
void ofApp::draw(){
   // shady.load("shader");
    ofSetColor(255);
    ofBackground(255,255);
    vx_fbo.begin();
    ofEnableAlphaBlending();
    ofClear(255, 255, 255,0);
    ofSetColor(255);
    ofBackground(255,0);
    ofVec3f centre = ofVec3f(ofGetWidth()/2,ofGetHeight()/2,0.0);
    float seconds = ofGetElapsedTimef();
    float b_period = (1/gui.getSlider("BPM")->getValue())*60;
    float bar_period = 4* b_period;
    float pos = fmod(seconds, b_period);
    float pos4_c1 = fmod(seconds, bar_period);
    float pos4_c2 = fmod(seconds+bar_period/2,bar_period);
    ofVec2f cursor_c1 = ofVec2f(centre.x+sin(pos4_c1*(TWO_PI/bar_period))*50,
                             centre.y+cos(pos4_c1*(TWO_PI/bar_period))*50
                             );
    ofVec2f cursor_c2 = ofVec2f(centre.x+sin(pos4_c2*(TWO_PI/bar_period))*50,centre.y+cos(pos4_c2*(TWO_PI/bar_period))*50);
    
    if (isOnset_c1){
        //cout << ofGetFrameNum();
        ofColor color;
        float hue;
        if (pitchFreq_c1 < 550){
            hue = ofMap(pitchFreq_c1, 200.0, 500.0, 0, 255);
        } else if (pitchFreq_c1 < 2000) {
            hue = ofMap(pitchFreq_c1, 500.0, 2000.0, 0, 255);
        } else {
            hue = ofMap(pitchFreq_c1, 1900.0, 4500.0, 0, 255);
        }
        float satuation = ofMap(pitchConf_c1, 0.0, 1.0, 100, 255);
        color.setHsb(hue, satuation, 255);
        voronoi.addPoint(ofPoint(
                         cursor_c1.x,
                         cursor_c1.y));
        cols.push_back(color);
        voronoi.generate();
    }
    if (isOnset_c2){
        //cout << ofGetFrameNum();
        ofColor color;
        float hue;
        if (pitchFreq_c2 < 550){
            hue = ofMap(pitchFreq_c2, 200.0, 500.0, 0, 255);
        } else if (pitchFreq_c2 < 2000) {
            hue = ofMap(pitchFreq_c2, 500.0, 2000.0, 0, 255);
        } else {
            hue = ofMap(pitchFreq_c2, 1900.0, 4500.0, 0, 255);
        }
        float satuation = ofMap(pitchConf_c2, 0.0, 1.0, 100, 255);
        color.setHsb(hue, satuation, 255);
        voronoi.addPoint(ofPoint(
                                 cursor_c2.x,
                                 cursor_c2.y));
        cols.push_back(color);
        voronoi.generate();
    }

    
        float dis = ((ofGetHeight()-50)/2);
    
    vector <ofxVoronoiCell> cells = voronoi.getCells();


    
    

        for (int i = border_cell_no; i < cells.size();i++){
            ofMesh mesh;
            float d = ofMap(centre.distance(cells[i].pt),0.0, dis-10, 255, 0.0);
            //cout << centre.distanceSquared(cells[i].pt);
           // cout << " ";
            ofSetColor(cols[i-border_cell_no],d);
            ofFill();
            mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
            mesh.addVertices(cells[i].pts);
            mesh.draw();
            mesh.clear();
        }
    thick.begin();
    thick.setUniform1f("thickness", gui.getSlider("thickness")->getValue());

    
    for (int i = border_cell_no; i < cells.size();i++){
        ofMesh mesh;
        mesh.setMode(OF_PRIMITIVE_LINE_LOOP);
        mesh.addVertices(cells[i].pts);
        mesh.draw();
        mesh.clear();
    }
    thick.end();
    vx_fbo.end();
   // al_fbo.begin();
   // alpha.begin();
   // alpha.setUniformTexture("inTex", vx_fbo.getTexture(), 0);
   // alpha.setUniform2f("iResoluton", ofGetWidth(), ofGetHeight());
   // alpha.setUniform1i("iDist", 5);
   // m_fsQuadVbo.draw();
   // alpha.end();
   // al_fbo.end();
    //shady.begin();
    //shady.setUniformTexture("inTex", vx_fbo.getTexture(), 0);
   // shady.setUniform2f("iResoluton", float(ofGetWidth()), float(ofGetHeight()));
    //shady.setUniform1i("iDist", 5);
    //shady.setUniformTexture("alpha", al_fbo.getTexture(), 1);
    //m_fsQuadVbo.draw();
    //shady.end();
    //al_fbo.draw(0,0);
   vx_fbo.draw(0,0);

    voronoi.chillax(centre, border_cell_no, cols);

}

//--------------------------------------------------------------
vector <ofPoint> ofApp::generateRandomPoints(int count, int seed, ofRectangle bounds, int _p) {
    vector <ofPoint> points;
    ofSeedRandom(seed);
    ofVec3f centre = ofVec3f(ofGetWidth()/2,ofGetHeight()/2,0.0);
    float dis = (ofGetHeight()-50)/2;
    float ang_step = TWO_PI/_p;
    for (int i = 0; i < _p;i++){
        ofPoint newPoint = ofPoint(sin(ang_step*i)*dis+centre.x,cos(ang_step*i)*dis+centre.y);
        points.push_back(newPoint);
    }
    
    
    for(int i=_p; i<count; i++) {
        float ang = ofMap(i, _p-1, count, 0.0, TWO_PI);

        float p_pos = ofRandom(5.0,dis);
        
        ofPoint newPoint = ofPoint(
            sin(ang)*p_pos+centre.x,
            cos(ang)*p_pos+centre.y
        );
        //points.push_back(newPoint);
    }
    
    return points;
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'l'){
        alpha.load("alpha");
        shady.load("shader");
    }
   }

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


void ofApp::createFullScreenQuad()
{
    // -1.0 to +1.0 is the full viewport (screen) if you use these as vertices in your vertex shader
    // (without multiplying by model, view, and projection matrices)
    
    ofVec3f vertices[4] =
    {
        ofVec3f(  1.0f,  1.0f, 0.0f ),
        ofVec3f( -1.0f,  1.0f, 0.0f ),
        ofVec3f(  1.0f, -1.0f, 0.0f ),
        ofVec3f( -1.0f, -1.0f, 0.0f )
    };
    
    ofIndexType indices[6] =
    {
        0, 1, 2,
        2, 1, 3
    };
    
    // Texture coordinates vary from 0.0 to 1.0 when they are in normalized format
    // ofDisableArbTex() was called earlier set that we're using normalized texture coordinates
    ofVec2f texCoords[4] =
    {
        ofVec2f( 1.0f, 0.0f ),
        ofVec2f( 0.0f, 0.0f ),
        ofVec2f( 1.0f, 1.0f ),
        ofVec2f( 0.0f, 1.0f )
    };
    
    m_fsQuadVbo.addVertices( vertices, 4 );
    m_fsQuadVbo.addTexCoords( texCoords, 4 );
    m_fsQuadVbo.addIndices( indices, 6 );
}



