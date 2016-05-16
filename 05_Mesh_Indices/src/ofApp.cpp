#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetVerticalSync(true);
    ofEnableDepthTest();

    
    radius = 50;
    
    
    //let's add a bunch of random points to the mesh
    //we'll connect them later with the mouse
    int numPoints = 1000;
    
    for(int i = 0; i < numPoints; i++){
        mesh.addVertex(ofVec3f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), ofRandom(-50, 50)));
    }
    
    //get a copy of the original mesh
    originalMesh = mesh;
    
    //set the original to points since we'll mainly use it to visualize where they are
    originalMesh.setMode(OF_PRIMITIVE_POINTS);
    
    //set the main mesh to lines since we'll be connecting them later on with the mouse
    mesh.setMode(OF_PRIMITIVE_LINES);

    //clear out all the indices since we'll be using the indices to
    //connect the mesh when we drag the mouse
    mesh.clearIndices();
    
    //we also need to add in some indices (and colors to keep the index/color numbers even)
    //since the mesh will draw all the lines connected if it doesn't see any indices
    //So we'll just add a place holder and say connect the first point to the first point
    //which doesn't actually result in a line
    mesh.addIndex(0);
    mesh.addIndex(0);
    mesh.addColor(ofColor(0, 0));
    mesh.addColor(ofColor(0, 0));
    

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackgroundGradient(ofColor(100), ofColor(0));
    
    ofSetColor(255);
    ofDrawBitmapString("Framerate: " + ofToString(ofGetFrameRate()), 15, 15);
    ofDrawBitmapString("Num Vertices: " + ofToString(mesh.getNumVertices()), 15, 30);
    
    ofDrawBitmapString("Press any key to toggle drawing the underlying points", 15, 60);
    
    //draw the mesh
    ofSetColor(255);
    ofSetLineWidth(1.5);
    mesh.draw();
    
    //draw the original mesh with all the points
    if(bDrawPoints){
        glPointSize(5.0);
        ofSetColor(255, 0, 0);
        originalMesh.drawVertices();
    }
    
    

    

    
    //draw a circle to indicate the region around the mouse that will connect points
    ofNoFill();
    ofSetColor(255, 0, 0);
    ofDrawCircle(mouseX, mouseY, radius);
    
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    bDrawPoints = !bDrawPoints;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

    //The following code is to go through and find all the mesh points
    //that are inside the mouse radius. Then we'll go through and find another point
    //and connect them by adding both of their indices. Since the meshMode is set to
    //OF_PRIMITIVE_LINES, it will connect them two by two.
    //
    //We'll also keep track of how many connections we've made so we don't connect
    //everything within a single pass of the mouse.
    
    //to keep track of how many connections we've made in this event
    int connectionsMade = 0;

    //maximum number of connections in a single drag event
    int maxConnections = 10;
    
    //go through all the verts
    for(int i = 0; i < mesh.getNumVertices(); i++){
        
        //get this vertex
        ofVec3f firstVert = mesh.getVertex(i);
        
        //see if this point is inside the mouse radius
        //use the distSquared method and compare it to the mouse radius squared
        //this avoids using square roots that are used for normal distance calculations
        //that are very computationally heavy.
        float firstVertDistSq = ofDistSquared(firstVert.x, firstVert.y, x, y);
        
        if(firstVertDistSq < radius * radius){
        
            //then look for other points
            //but only start after the point we've already found
            for(int j = i + 1; j < mesh.getNumVertices(); j++){
                
                //get the second vertex
                ofVec3f secondVert = mesh.getVertex(j);
                
                //See if THIS point is in the mouse too
                float secondVertDistSq = ofDistSquared(secondVert.x, secondVert.y, x, y);
                
                if(secondVertDistSq < radius * radius){
                    
                    //if we've gotten this far, we've found two points, both inside the mouse
                    //add two indices so the mesh will connect them
                    mesh.addIndex(i);
                    mesh.addIndex(j);
                    
                    //add two colors too while we're here
                    //this will add two colors in random grayscale with random transparency
                    mesh.addColor(ofColor(ofRandom(255), ofRandom(255)));
                    mesh.addColor(ofColor(ofRandom(255), ofRandom(255)));
                    
                    //increment the counter since we've made a connection
                    connectionsMade++;
                    
                    //if we've reached the maximum number of connections
                    //we don't need to look for any more points, so use "break" to
                    //exit this for loop
                    if(connectionsMade == maxConnections) break;
                    
                }
                
                
            }
        
        }
        
        //use break again to exit out of the outer for loop
        if(connectionsMade == maxConnections) break;
    }
    
    
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

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
