#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		

    //the main mesh
    ofMesh mesh;
    
    //another object that will hold the
    //original mesh values when we change them
    ofMesh originalMesh;

    //convenience variables
    int numVerts;
    int meshWidth, meshHeight;
    
    bool bWireframe;
    
    //easy cam to let us view from different angles
    ofEasyCam cam;
    

    //we'll use the texture from the image/movie
    //to draw on the mesh
    ofVideoPlayer movie;
    ofImage img;
};
