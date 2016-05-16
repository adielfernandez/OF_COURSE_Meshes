#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetVerticalSync(true);
    ofEnableDepthTest();
    
    //Load an image so we can use it as a texture for our mesh
    img.load("water.jpg");
    
    
    //Create a primitive shape so we can grab the mesh from it
    ofIcoSpherePrimitive icoSphere;
    
    radius = 100;
    icoSphere.setResolution(4);
    icoSphere.setRadius(radius);
    icoSphere.setPosition(0, 0, 0);
    
    //The primitive sphere has a method that will conveniently
    //give us the right texture coordinates per vertex to map an image to it
    icoSphere.mapTexCoordsFromTexture(img.getTexture());
    
    mesh = icoSphere.getMesh();
    
    //set up the different properties of the lighting
    light.setPosition(400, 0, 400);
    light.setup();

    light.setDiffuseColor(ofFloatColor::white);
    light.setAmbientColor(ofFloatColor::darkGray);

    //we also need to set a material property for our objects
    //so we know how the light will reflect off of them
    material.setDiffuseColor(ofFloatColor::white);
    material.setShininess(10.0);
    
    cout << "Mode: " << mesh.getMode() << endl;

    originalMesh = mesh;
}

//--------------------------------------------------------------
void ofApp::update(){

    
    //let's manipulate all the vertices algorithmically
    for(int i = 0; i < mesh.getNumVertices(); i++){
        
        
        //push vertices out then back in using sine
        //Since the sphere is positioned around the origin, if we scale
        //the vertex up or down, it will have the effect of pushing the
        //points in and out of the sphere.
        
        //start with the original mesh. If we keep scaling the current mesh
        //it will spiral out of control
        ofVec3f vert = originalMesh.getVertex(i);
        
        
        float amplitude = 0.05;  //this amplitude is as a percentage of the radius
        
        //multiplying time makes the oscillations faster
        float time = ofGetElapsedTimef() * 3.5;
        
        //changing phase makes the wave offset at different parts.
        //if phase was zero, the whole sphere would pulse in and out at the same time
        //making it non-zero and mapped according to the Z axis (from -radius to radius)
        //makes it look like the wave is travelling in the Z direction
        float phase = ofMap(vert.z, -radius, radius, 0, PI * 12);
        
        //scale the vertex (start at 1 (original radius) then add a little bit from there
        vert *= 1 + amplitude * sin(time + phase);
        
        mesh.setVertex(i, vert);
        
        
    }
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackgroundGradient(ofColor(100), ofColor(0));
    

    
    //enable/disable, bind/unbind all the cameras, textures, lighting, etc.
    
    cam.begin();
    
    light.enable();
    material.begin();
    

  
    img.getTexture().bind();
    
    if(bWire){
        mesh.drawWireframe();
    } else {
        mesh.draw();
    }
    
    img.getTexture().unbind();

    material.end();
    light.disable();

    cam.end();
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    bWire = !bWire;
    
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
