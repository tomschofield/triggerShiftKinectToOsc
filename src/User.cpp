#include "User.h"
#define NUM_GESTURES 6
////////////////////////---------/////////////////////////////////////
void User::setup(){
    font.loadFont("Bangla Sangam MN.ttf", 13);
    numJointsTracking=13;

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
        //draw an ellipse at every tracked joint position
        ofSetColor(bodyColour);
        for(int i=0;i<allJoints.size();i++){
            ofPushMatrix();
            ofTranslate( ofMap(allJoints[i].x,0,1,0,ofGetHeight()),ofMap(allJoints[i].y,0,1,0,ofGetHeight()),0);         
            font.drawString(jointNames[i],0,0);//+" "+ofToString(allJoints[i].z), 0,0);
            ofCircle(0,0,10,10);
            ofPopMatrix();
        }
        for(int i=0;i<gesture.gestureStates.size();i++){
            
            font.drawString(ofToString(gesture.gestureStates[i]),100 ,100+(i*30));
            
            
        }           
    }
}