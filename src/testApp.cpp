// (c) colin wright 2012, MIT licence. 

#include "testApp.h"

void testApp::setup() {

    
    //kinect field of view
    fov = ofPoint(58.,45.);    
    //change this to effect seam cutting for the mesh
    depthThreshhold = 128;    
    //set simplification level for mesh saving. 
    simpl= 5;    
   
    filecounter = 0;
    filecount = 0;    
    
}

void testApp::update() {
    
    if (!fileq.empty()){                
        filecounter--;
        
        
        //statusText += ("processed " + fileq.back() +"\n");
        doneFiles.push_back("processed " + fileq.back() +"\n");
        if (doneFiles.size()>=5) doneFiles.pop_front();
        
        kinect.readCompressedPng(fileq.back(), spix);           
        
        
        
        int height = 480;
        int width = 640;
        
        //this section is modified from the ofxRGBDepth renderer so as to not require opencv camera intrinsics. 
        ofFile file("/tmp/rgbd_"+ofToString(filecounter,3)+".obj", ofFile::WriteOnly);
        float fx = tanf(ofDegToRad(fov.x) / 2) * 2;
        float fy = tanf(ofDegToRad(fov.y) / 2) * 2;
        
        file << "#vertices" << endl;
        
        for (int y = 0; y < height; y+=1){
            for (int x = 0; x<width; x+=1){
                unsigned short z = spix[x+y*width];
                float xReal=(((float)x-320) / 640.) * z * fx;
                float yReal=(((float)y-240) / 480.) * z * fy;
                //blender coordinates are weird so we invert everything here. 
                //this will orient for a blender camera at 0,0,0 with x rotation at 90. 
                file << "v " << ofToString((float)xReal/32.) << " " << ofToString(-(float)yReal/32.) << " " << ofToString((float)(-spix[x+y*width]/32.)) << endl;
                
            }
        }
        
        file << "#faces" << endl;
        int coordx, coordy=0;
        
        for (int y = 0; y<height-simpl; y+=simpl){
            for (int x=0; x<width-simpl; x+=simpl){
                if (
                    
                    
                    (spix[x+y*width] > 0)
                    &&(abs(spix[x+y*width] - spix[(x+simpl)+y*width]) <= depthThreshhold)
                    &&(abs(spix[x+y*width] - spix[x+(y+simpl)*width]) <= depthThreshhold)
                    &&(abs(spix[(x+simpl)+y*width] - spix[x+(y+simpl)*width]) <= depthThreshhold)
                    
                    &&(abs(spix[(x+simpl)+y*width] - spix[(x+simpl)+(y+simpl)*width]) <= depthThreshhold)
                    &&(abs(spix[(x+simpl)+y*width] - spix[x+(y+simpl)*width]) <= depthThreshhold)
                    &&(abs(spix[(x+simpl)+(y+simpl)*width] - spix[x+(y+simpl)*width]) <= depthThreshhold)
                    ){
                    file << "f " << ofToString(1+x+y*width) << " " << ofToString(1+(x+simpl)+y*width)<< " " << ofToString(1+x+(y+simpl)*width) << endl;
                    file << "f " << ofToString(1+(x+simpl)+y*(width))<< " " << ofToString(1+(x+simpl)+(y+simpl)*width)<< " " << ofToString(1+x+(y+simpl)*(width)) << endl;
                    
                }
                
            }
            
        }
        file.close();
        if (fileq.size()==1) doneFiles.push_back("done!");
        fileq.pop_back();
    } 
    
}

void testApp::draw() {
    ofBackground(59, 0, 0);
    ofSetColor(255, 255, 255);
    statusText = ""; 
    list<string>::iterator it;
    for(it = doneFiles.begin(); it != doneFiles.end(); it++){
        statusText += *it;
    }
    ofDrawBitmapString("drag-n-drop rgbdtoolkit-pngs here to convert\n\nobj files appear in /tmp/rgbd_[filenumber].obj\n\n" + statusText , 20, 20);
}


void testApp::mouseReleased(int x, int y, int button){
}

void testApp::dragEvent(ofDragInfo dragInfo){
    fileq = dragInfo.files;
    filecounter = dragInfo.files.size(); 
    filecount = dragInfo.files.size(); 
    doneFiles.push_back("files to process: " + ofToString(filecount, 3) + "\n");
}


