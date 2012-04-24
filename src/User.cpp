#include "User.h"
#define NUM_GESTURES 6
////////////////////////---------/////////////////////////////////////
void User::setup(){
    font.loadFont("Bangla Sangam MN.ttf", 13);
    numJointsTracking=13;
    rightHandX=0;
    rightHandY=0; 
    rightHandZ=0;
    leftHandX=0; 
    leftHandY=0; 
    leftHandZ=0;
    headX=0; 
    headY=0; 
    headZ=0;
    torsoX=0; 
    torsoY=0; 
    torsoZ=0;
    leftFootX=0; 
    leftFootY=0; 
    leftFootZ=0;
    rightFootX=0;
    rightFootY=0; 
    rightFootZ=0;
    isAlive=false;
    for(int i=0;i<numJointsTracking;i++){
        allJoints.push_back(ofPoint(0,0,0));
        jointNames.push_back("test");
    }
    gesture.setup(numJointsTracking);
    bodyColour.setHsb( ofRandom(0, 255),255,255);
    
    gestureNames.push_back("arms_are_crossed");
    gestureNames.push_back("two_hands_on_left_side");
    gestureNames.push_back("two_hands_on_right_side");
    gestureNames.push_back("left_hand_is_over_right");
    gestureNames.push_back("right_hand_is_over_left");
    gestureNames.push_back("arms_are_boxy");
    
    for(int i=0;i<NUM_GESTURES;i++){
        gestureResults.push_back(false);
    }
    
}

void User::update(vector<ofPoint> joints){
    if (isAlive) {
        allJoints=joints;
        gestureResults= gesture.detect(joints);
    }
    for(int i=0;i<jointNames.size();i++){
    cout<<jointNames[i]<<endl;
    }
      
}

void User::draw(){
    if (isAlive) {
        //draw an ellipse at every tracked joint positio
        ofSetColor(bodyColour);
  //  cout<<"drawing"<<endl;
        for(int i=0;i<allJoints.size();i++){
         //   ofCircle(ofMap(allJoints[i].x,0,1,0,ofGetWidth()),ofMap(allJoints[i].y,0,1,0,ofGetHeight()),10,10);
            ofPushMatrix();
            ofTranslate( ofMap(allJoints[i].x,0,1,0,ofGetHeight()),ofMap(allJoints[i].y,0,1,0,ofGetHeight()),0);//-ofMap(allJoints[i].y,0,1,0,ofGetWidth())); 
            //font.drawString(jointNames[i], ofMap(allJoints[i].x,0,1,0,ofGetWidth()),ofMap(allJoints[i].y,0,1,0,ofGetHeight()));
            //ofCircle(ofMap(allJoints[i].x,0,1,0,ofGetWidth()),ofMap(allJoints[i].y,0,1,0,ofGetHeight()),10,10);
            font.drawString(jointNames[i],0,0);//+" "+ofToString(allJoints[i].z), 0,0);
            ofCircle(0,0,10,10);
            ofPopMatrix();
        }
        for(int i=0;i<gesture.gestureStates.size();i++){
            
            font.drawString(ofToString(gesture.gestureStates[i]),100 ,100+(i*30));
            
            
        }
        font.drawString("rightX: "+ofToString(allJoints[0].x)+"\nleftX: "+ofToString(allJoints[1].x),100 ,100+((1+gesture.gestureStates.size())*30));
        //draw head
      /*  ofSetColor(255, 255, 255);
        int bodyPartSize=20;
        ofCircle(ofMap(headX,0,1,0,ofGetWidth()), ofMap(headY,0,1,0,ofGetHeight()), bodyPartSize);
        ofLine(ofMap(headX,0,1,0,ofGetWidth()), ofMap(headY,0,1,0,ofGetHeight()), ofMap(torsoX,0,1,0,ofGetWidth()), ofMap(torsoY,0,1,0,ofGetHeight()));
        ///draw right hand
        ofCircle(ofMap(rightHandX,0,1,0,ofGetWidth()), ofMap(rightHandY,0,1,0,ofGetHeight()), bodyPartSize);
        ofLine(ofMap(rightHandX,0,1,0,ofGetWidth()), ofMap(rightHandY,0,1,0,ofGetHeight()), ofMap(torsoX,0,1,0,ofGetWidth()), ofMap(torsoY,0,1,0,ofGetHeight()));
        
        ///draw left hand
        ofCircle(ofMap(leftHandX,0,1,0,ofGetWidth()), ofMap(leftHandY,0,1,0,ofGetHeight()), bodyPartSize);
        ofLine(ofMap(leftHandX,0,1,0,ofGetWidth()), ofMap(leftHandY,0,1,0,ofGetHeight()), ofMap(torsoX,0,1,0,ofGetWidth()),ofMap( torsoY,0,1,0,ofGetHeight()));
        
        ///draw right Foot
        ofCircle(ofMap(rightFootX,0,1,0,ofGetWidth()), ofMap(rightFootY,0,1,0,ofGetHeight()), bodyPartSize);
        ofLine(ofMap(rightFootX,0,1,0,ofGetWidth()), ofMap(rightFootY,0,1,0,ofGetHeight()), ofMap(torsoX,0,1,0,ofGetWidth()), ofMap(torsoY,0,1,0,ofGetHeight()));
        
        ///draw left Foot
        ofCircle(ofMap(leftFootX,0,1,0,ofGetWidth()), ofMap(leftFootY,0,1,0,ofGetHeight()), bodyPartSize);
        ofLine(ofMap(leftFootX,0,1,0,ofGetWidth()), ofMap(leftFootY,0,1,0,ofGetHeight()), ofMap(torsoX,0,1,0,ofGetWidth()),ofMap( torsoY,0,1,0,ofGetHeight()));*/
        
        
        
    }
}