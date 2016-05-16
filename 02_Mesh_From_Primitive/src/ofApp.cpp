#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetVerticalSync(true);
    
    ofEnableDepthTest();
    
    //load image and or video
    img.load("stars.png");
    movie.load("trapped.mov");
    movie.setLoopState(OF_LOOP_NORMAL);

    
    //create a primitive shape
    ofPlanePrimitive plane;
    
    //set the dimensions of the plane
    meshWidth = 640;
    meshHeight = 360;
    
    //Comment these out for using a texture from the movie or image
//    meshWidth = movie.getWidth();
//    meshHeight = movie.getHeight();
    
    //for using mesh with image texture
//    meshWidth = img.getWidth();
//    meshHeight = img.getHeight();
    
    plane.set(meshWidth, meshHeight, 50, 50);
    //the 50-50 set the number of rows/columns in the mesh
    
    
    //copy the mesh from the plane object
    mesh = plane.getMesh();
    
    
    //Now we'll go through all the vertices and add some colors

    //first get the number of vertices
    numVerts = mesh.getNumVertices();
    
    //since we got all our mesh data from the plane primitive
    //clear out the colors and other stuff (texcoords) so we
    //can replace it with our own data
    mesh.clearColors();
    mesh.clearTexCoords();
    
    //for loop to go through all the vertices
    for(int i = 0; i < numVerts; i++){

        //this will set the color at each mesh point to be increasingly more green
        //So it will start at red and end at yellow
        mesh.addColor(ofColor(255, 255 * (i/(float)numVerts), 0));
        
        
        //Texcoords - The way to tell the mesh how to wrap and image/movie texture
        //to each vertex point
        //In this case, the tex coord is the same as vertex: 1 to 1 mapping
        //i.e. no stretching/warping, etc.
        //since we've made the mesh the same dimensions as the texture
        mesh.addTexCoord(mesh.getVertex(i) + ofVec3f(meshWidth/2, meshHeight/2, 0));
        
        
    }
    
    //Copy the mesh into the "originalMesh" so that we can
    //manipulate the mesh later and still know where the original data was
    originalMesh = mesh;
    
    
}

//--------------------------------------------------------------
void ofApp::update(){

    //----------------------------------------------------------------------------
    //Comment out the individual sections to manipulate the mesh in different ways
    //----------------------------------------------------------------------------
    
    //Mouse
    //lift all the vertices that are "close" to the mouse
    //if mouse screen coords = mesh's XY coords
//    float radius = 50;
//    float amplitude = 200;
//    
//    for(int i = 0; i < numVerts; i++){
//        
//        //convert the mouse XY to the XY of the mesh
//        //these map the values from the width/height of the screen to the mesh width/height
//        //we're going from -meshWidth/2 to +meshWidth/2 because the mesh we got from
//        //the plane primitive is centered at zero
//        float mapMouseX = ofMap(mouseX, 0, ofGetWidth(), -meshWidth/2, meshWidth/2);
//        float mapMouseY = ofMap(mouseY, 0, ofGetWidth(), -meshHeight/2, meshHeight/2);
//        
//        //get the mesh vertex
//        ofVec3f p = mesh.getVertex(i);
//        
//        //calculate the distance from the mapped mouse point to the vertex
//        float dist = ofDist(mapMouseX, mapMouseY, p.x, p.y);
//        
//        //if the distance is within the radius we set above...
//        if(dist < radius){
//            
//            //...set the Z value of the point to a height proportional to the distance
//            //closer to the mouse is higher, away fromt the mouse is lower
//            p.z = ofMap(dist, 0, radius, amplitude, 0);
//            
//            //while we're here, let's change the color too!
//            mesh.setColor(i, ofColor(0, 255, 0));
//
//        } else {
//
//            //if we're not close to the mouse, set the Z value to zero
//            p.z = 0;
//            
//            //and change the color back to the original value too
//            mesh.setColor(i, originalMesh.getColor(i));
//        }
//        
//        //put the newly changed point back in the mesh
//        mesh.setVertex(i, p);
//        
//    }
    
    
    
    
    
    //Perlin Noise
    //Pseudo randomness - allows random-ish oscillations but with a certain
    //degree of smoothness. "noiseScale" will change the spatial scale (wide flowing peaks
    //versus many smaller peaks) while "speed" will alter how fast we go through the randomness.
    
    //maximum height of the oscillation
    float amp = 100;
    
    //change the scale of the noise based on the X value of the mouse
    float noiseScale = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0.001, 0.012);
    
    //change how fast we go through the noise based on the Y value of the mouse
    float speed = ofMap(ofGetMouseY(), 0, ofGetHeight(), 0.0, 3.0);

    
    //Go through each vertex and set them to the noise value
    for(int i = 0; i < numVerts; i++){
        
        
        ofVec3f vertex = mesh.getVertex(i);

        ofVec3f noiseyVertex;
        
        //get the 3 dimensional noise value (3 Dims = X, Y and time as the 3rd dimension so the noise value changes over time)
        float noise = amp * ofNoise(vertex.x * noiseScale, vertex.y * noiseScale, speed * ofGetElapsedTimef());
        
        //set the point with noise as the Z value
        noiseyVertex.set(vertex.x, vertex.y, noise);
        
        //add the noise vertex back into the mesh
        mesh.setVertex(i, noiseyVertex);
        
    }
    
    //update the movie (if we're using the movie texture)
    movie.update();

    
}

//--------------------------------------------------------------
void ofApp::draw(){

    //play the movie
    if(movie.isLoaded()){
        movie.play();
    }
    
    
    ofBackgroundGradient(ofColor(80), ofColor(0));
    
    ofSetColor(255);
    ofDrawBitmapString("Framerate: " + ofToString(ofGetFrameRate()), 15, 15);
    ofDrawBitmapString("Num Vertices: " + ofToString(numVerts), 15, 30);

    
    //Everythign drawn between cam.begin() and cam.end() will be subject to
    //camera rotations using the mouse
    cam.begin();
    
    //we need to bind the texture we want to use. This sets the active texture
    //on the GPU when the mesh is drawn.

    //Comment out the one you don't want.
    //(comment out both to use the mesh colors instead of the texture)

    //    movie.getTexture().bind();
    img.bind();
    
    //mesh colors will tint the texture so disable them if you want to use a texture
    mesh.disableColors();
    
    
    //draw the mesh or the wireframe
    if(bWireframe){
        ofSetLineWidth(1);
        mesh.drawWireframe();
    } else {
        mesh.draw();
    }
    
    //unbind th texture once we're done drawing the mesh
    movie.getTexture().unbind();
    img.unbind();
    
    
    //finish wrapping the camera so it knows what is going to be manipulated and what isnt
    cam.end();

    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    //toggle the mesh/wireframe view
    if(key == 'w'){
        bWireframe = !bWireframe;
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
