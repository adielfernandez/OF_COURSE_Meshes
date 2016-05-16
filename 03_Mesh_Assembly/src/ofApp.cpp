#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetVerticalSync(true);
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    
    movie.load("trapped.mov");
    movie.setLoopState(OF_LOOP_NORMAL);
    
    img.load("stars.png");
    
    

    
    
    /*
     * Now we'll construct a mesh from scratch instead of getting it from a primitive
     * Since we'll add vertices in the order that makes sense for a OF_PRIMITIVE_TRIANGLES
     * style mesh (first 3 vertices are one triangle, the next 3 vertices are
     * the second triangle, etc.) we won't need to add any indices.
     *
     * Arrangement:
     * Each time we go through an X and a Y from the nested for loops, we'll create 1 unit cell.
     * Since there are no quads (i.e. rectangles) in OpenGl anymore, we need to do this with
     * two triangles:
     *
     *       0_______1
     *       |      /|
     *       |     / |
     *       |    /  |
     *       |   /   |
     *       |  /    |
     *       | /     |
     *       |/______|
     *       2       3
     *
     * The first triangle will be 0-1-2 while the second will be 1-3-2
     * (keep winding clockwise so the normals are all in the same direction)
     *
     * Since our for loops go through X and Y together, the XY version of the 
     * triangle above will be:
     *
     *  0: (x    , y    )
     *  1: (x + 1, y    )
     *  2: (x    , y + 1)
     *  3: (x + 1, y + 1)
     *
     */
    
    //set the mode. "OF_PRIMITIVE_TRIANGLES" expects triplets of
    //vertices, one triplet per triangle
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    //Number of grid points in our mesh in each direction
    //These can be higher, but bigger triangles make it easier to see the movie texture
    int numX = 4;
    int numY = 4;
    
    //this will set the overall mesh dimensions to the same as
    //that of the movie file you give it
    int meshWidth = movie.getWidth();
    int meshHeight = movie.getHeight();
    

    //space between X and Y grid points
    int gridSpacingX = meshWidth/(numX-1);
    int gridSpacingY = meshHeight/(numY-1);
    
    //package them together for easy multiplication later
    ofVec2f gridSpacing(gridSpacingX, gridSpacingY);
    
    
    for(int y = 0; y < numY - 1; y++){
        for(int x = 0; x < numX - 1; x++){
            
            
            //Create points for the first triangle
            ofVec3f v1(x    , y    , 0);
            ofVec3f v2(x + 1, y    , 0);
            ofVec3f v3(x    , y + 1, 0);
            
            //scale them up to the actual spacing between grid points we want
            //since X and Y are only incrementing by 1 pixel each for loop
            v1 *= gridSpacing;
            v2 *= gridSpacing;
            v3 *= gridSpacing;
            
            //add the first triangle's vertices to the mesh
            mesh.addVertex(v1);
            mesh.addVertex(v2);
            mesh.addVertex(v3);
            
            //add the same vertices as texture coordinates. this will map the texture
            //linearly (no warping/stretching) but beware of situations where the size of the texture doesn't match the size of the mesh.
            mesh.addTexCoord(v1);
            mesh.addTexCoord(v2);
            mesh.addTexCoord(v3);
            
            
            //Create points for the second triangle
            ofVec3f v4(x + 1, y    , 0);
            ofVec3f v5(x + 1, y + 1, 0);
            ofVec3f v6(x    , y + 1, 0);
            
            //scale it up
            v4 *= gridSpacing;
            v5 *= gridSpacing;
            v6 *= gridSpacing;
        
            //add second triangle
            mesh.addVertex(v4);
            mesh.addVertex(v5);
            mesh.addVertex(v6);
            
            //add next texCoords
            mesh.addTexCoord(v4);
            mesh.addTexCoord(v5);
            mesh.addTexCoord(v6);
            
            
        }
    }


    
    //Since our mesh starts at (0,0) at the upper left corner, go through
    //all the mesh points and shift them over by half the width and height
    //so it is centered around the origin and draws nicer
    for(int i = 0; i < mesh.getNumVertices(); i++){
        mesh.setVertex(i, mesh.getVertex(i) - ofVec2f(meshWidth/2, meshHeight/2));
    }
    
    
    //get a copy of the mesh and store it so we can know the original
    //data after we've manipulated it
    originalMesh = mesh;
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //update the movie so our texture has the next frame
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
    ofDrawBitmapString("Num Vertices: " + ofToString(mesh.getNumVertices()), 15, 30);
    
    
    ofDrawBitmapString("Press SPACEBAR to randomize the triangles in the mesh", 15, 60);
    ofDrawBitmapString("Press 'r' to restore the vertex positions from the original mesh", 15, 75);
    ofDrawBitmapString("Press 'w' to toggle wireframe drawing", 15, 90);
    
    //start the camera so we can move our mesh around
    cam.begin();
    
    
    //bind the texture, but only if the movie has actually loaded
    if(movie.isLoaded()) movie.getTexture().bind();
    
    if(bWire){
        mesh.drawWireframe();
    } else {
        mesh.draw();
    }
    
    //unbind (if the movie has loaded)
    if(movie.isLoaded()) movie.getTexture().unbind();
    
    //finish camera manipulation
    cam.end();
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == 'w'){
        bWire = !bWire;
    }
    
    //scatter all the triangles
    if(key == ' '){
        
        
        //go through all the points by 3's (3 verts in each triangle)
        //and give them a random distance
        for(int i = 0; i < mesh.getNumVertices(); i += 3){
            
            //create a random direction then add that same ofVec3f to all three
            //vertices. Doing it this way moves the three vertices in each triangle
            //the same amount keeping the triangle together. Try giving each
            //vertex a different random amount and see what happens
            
            ofVec3f scatter;
            float dist = 100;
            scatter.set(ofRandom(-dist, dist), ofRandom(-dist, dist), ofRandom(-dist, dist));

            mesh.setVertex(i + 0, mesh.getVertex(i + 0) + scatter);
            mesh.setVertex(i + 1, mesh.getVertex(i + 1) + scatter);
            mesh.setVertex(i + 2, mesh.getVertex(i + 2) + scatter);
            
        }

        
        
        
    }
    

    //Since we kept a copy of the mesh when it was in its original state we can
    //restore it very easily
    if(key == 'r'){
        mesh = originalMesh;
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
