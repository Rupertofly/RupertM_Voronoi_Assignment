#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofDisableArbTex();
    //ofEnableNormalizedTexCoords();
    
    gui = ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
    gui.addTextInput("message", "# open frameworks #");
    
    gui.addFRM();
    gui.addBreak();
    gui.addSlider("BPM", 50.0, 200, 90.0);
    gui.addSlider("thickness",0.0,50.0,20.0);
    gui.addToggle("shader");
    gui.addFooter();

    ofSetWindowTitle("ofxVoronoi / example_basic");
    ofBackground(255);
    disp = (ofGetWidth()/2)-((ofGetHeight()-20)/2);
    ofRectangle bounds = ofRectangle(10, 10, ofGetWidth()-20, ofGetHeight()-20);
    
    int pointCount = 100;
    int seed = 33;
    vx_fbo.allocate(ofGetWidth(), ofGetHeight(),GL_RGBA);
    vx_fbo.begin();
    sh_fbo.allocate(ofGetWidth(), ofGetHeight(),GL_RGBA);
    sh_fbo.begin();
    ofClear(255,255,255);
    sh_fbo.end();
    vx_fbo.begin();
    ofClear(255, 255, 255,0);
    vx_fbo.end();
    border_cell_no = floor(pointCount/10+10);
    centre.x = (ofGetWidth()/2);
    centre.y = (ofGetHeight()/2);
    centre.z = (0.0);
    points = generateRandomPoints(pointCount, seed, bounds, border_cell_no);
    shady.load("shader");
    thick.load("thickness.vert", "thickness.frag","thickness.geom");
    
    voronoi.setBounds(bounds);
    voronoi.setPoints(points);
    
    voronoi.generate();
    
    points.clear();
    for(auto cell : voronoi.getCells()) {
        points.push_back(cell.pt);
    }
    
    createFullScreenQuad();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    ofBackground(255);
    vx_fbo.begin();
    ofClear(255, 255, 255,0);
    ofSetColor(255);
    ofBackground(255);
    ofVec3f centre = ofVec3f(ofGetWidth()/2,ofGetHeight()/2,0.0);
    
//    voronoi.draw();
    
    // Or feel free to draw the voronoi diagram yourself:
    float seconds = ofGetElapsedTimef();
    float b_period = (1/gui.getSlider("BPM")->getValue())*60;
    float bar_period = 4* b_period;
    float pos = fmod(seconds, b_period);
    float pos4 = fmod(seconds, bar_period);
    ofVec2f cursor = ofVec2f(centre.x+sin(pos4*(TWO_PI/bar_period))*40,
                             centre.y+cos(pos4*(TWO_PI/bar_period))*40
                             );
    
    if (pos < 1.0/30.0){
        //cout << ofGetFrameNum();
        voronoi.addPoint(ofPoint(
                         cursor.x,
                         cursor.y));
        cols.push_back(ofColor(ofRandom(255),ofRandom(255),ofRandom(255)));
        voronoi.generate();
    }
    
    ofRectangle bounds = voronoi.getBounds();
    ofSetLineWidth(0);
    ofNoFill();
    ofSetColor(220);
    //ofDrawRectangle(bounds);
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
    
    shady.load("shader");
    //sh_fbo.begin();
    //ofClear(255,255,255);
    ofVec2f inverseResolution( 1.0f / (float)ofGetWidth(), 1.0f / (float)ofGetHeight() );
    shady.begin();
    shady.setUniformTexture("vor_tex", vx_fbo.getTexture(), 0);
    shady.setUniform2f("resolution",inverseResolution);
    ofSetColor(255);
    m_fsQuadVbo.draw();
    //ofRectangle(0, 0, ofGetWidth(), ofGetHeight());
    shady.end();
    if (gui.getToggle("shader")->getChecked())  vx_fbo.draw(0,0);
    //sh_fbo.end();
    
        // Draw cell poin
    voronoi.chillax(centre, border_cell_no, cols);
    sh_fbo.draw(0,0);
    ofSetColor(0);
    ofFill();
    ofDrawEllipse(cursor.x, cursor.y, 5, 5);
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
bool ofApp::isBorder(ofPoint _pt){
    ofRectangle bounds = voronoi.getBounds();
    
    return (_pt.x == bounds.x || _pt.x == bounds.x+bounds.width
            || _pt.y == bounds.y || _pt.y == bounds.y+bounds.height);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    int n = 0;
    switch(key) {
        case '1': n = 1; break;
        case '2': n = 10; break;
        case '3': n = 100; break;
        case '4': n = 1000; break;
    }
    for(int i = 0; i < n; i++) {
        voronoi.chillax(centre, border_cell_no, cols);
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



