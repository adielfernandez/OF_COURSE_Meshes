#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetVerticalSync(true);
    
    meshMode = 0;
    bShowWire = true;
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackgroundGradient(ofColor(80), ofColor(0));
    

    //store the mode in a string so we can draw the name on screen
    string whichMode;
    
    //set the mode depending on which one we're using
    switch (meshMode) {
     
        case 0:
            mesh.setMode(OF_PRIMITIVE_POINTS);
            whichMode = "OF_PRIMITIVE_POINTS";
            break;
        case 1:
            mesh.setMode(OF_PRIMITIVE_LINES);
            whichMode = "OF_PRIMITIVE_LINES";
            break;
        case 2:
            mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
            whichMode = "OF_PRIMITIVE_LINE_STRIP";
            break;
        case 3:
            mesh.setMode(OF_PRIMITIVE_LINE_LOOP);
            whichMode = "OF_PRIMITIVE_LINE_LOOP";
            break;
        case 4:
            mesh.setMode(OF_PRIMITIVE_TRIANGLES);
            whichMode = "OF_PRIMITIVE_TRIANGLES";
            break;
        case 5:
            mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
            whichMode = "OF_PRIMITIVE_TRIANGLE_STRIP";
            break;
        case 6:
            mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
            whichMode = "OF_PRIMITIVE_TRIANGLE_FAN";
            break;

    }
    


    //make points bigger and more visible
    glPointSize(3.0);
    
    
    //draw the mesh itself
    ofSetColor(0, 128, 255);
    
    //enable the colors on the mesh here (since we're disabling them later)
    mesh.enableColors();
    mesh.draw();

    
    //draw a wireframe on top of it
    if(bShowWire){
        
        //disable the colors of the mesh so we can draw the wireframe
        //over the mesh itself in white
        mesh.disableColors();
        
        ofSetColor(255);
        mesh.drawWireframe();
    }
    
    
    
    //also draw the point number next to the point
    for(int i = 0; i < mesh.getNumVertices(); i++){
        
        //get the position of each mesh vertex
        ofVec3f thisVertex = mesh.getVertex(i);
        
        //draw the string (i.e. the vertex number) next to each vertex point
        ofSetColor(255);
        ofDrawBitmapString(ofToString(i), thisVertex.x + 5, thisVertex.y - 5);
    }
    
    
    
    //draw relevant text
    ofSetColor(255);
    ofDrawBitmapString("CURRENT MODE:" + whichMode, 15, 20);
    ofDrawBitmapString("Press SPACE to clear points\nPress 'W' to toggle drawing the wireframe on top", 15, ofGetHeight() - 30);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    //Loop through the mesh modes
    if(key == OF_KEY_RIGHT){
        meshMode++;
        
        if(meshMode > 6){
            meshMode = 0;
        }
    } else if(key == OF_KEY_LEFT){
        meshMode--;
        
        if(meshMode < 0){
            meshMode = 6;
        }
    }
    
    //clear out all the points
    if(key == ' '){
        mesh.clear();
    }
    
    if(key == 'w' || key == 'W'){
        bShowWire = !bShowWire;
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

    //add a point to our mesh
    mesh.addVertex(ofVec3f(x, y, 0));

    //select a new color everytime we add a point and add it to the mesh.
    //The mesh will draw the first vertex with the first color it has in its lists
    //so it's a good idea to add them at the same time
    ofColor newColor;
    newColor.setHsb(ofRandom(255), 200, 200); //use HSB mode so colors come out purty
    mesh.addColor(newColor);
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
