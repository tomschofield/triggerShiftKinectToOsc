#include "KinectGesture.h"
#define NUM_GESTURES 6
////////////////////////---------/////////////////////////////////////
void KinectGesture::setup(int numJs){
    numJoints=numJs;
    for (int i=0; i<NUM_GESTURES; i++) {
        //set all gestures to be off as default
        gestureStates.push_back(0);
        gestureTimers.push_back(0);
    }
    
    
}

vector<bool> KinectGesture::detect(vector<ofPoint> jointCoordinates){
    
    /*
     //gestures
     arms_are_crossed; //hip hop style
     two_hands_on_left_side; //both hands are passed centre line of body to left
     two_hands_on_right_side; //both hands are passed centre line of body to right
     left_hand_is_over_right; //reach right hand high in the air and stretch left hand down
     right_hand_is_over_left;//reach left hand high in the air and stretch right hand down
     arms are boxy
     */
    
    
    //todo box gesture elbows and hands aligned
    if (!jointCoordinates.empty()) {
        
        //joints are in order 0 rhand , 1 lhand , 2 rfoot, 3 lfoot , 4 head , 5 torso, 6 relbow, 7 lelbow
        //joints are in order 0 rhand , 1 lhand , 2 right elbow, 3 left elbow, 4 neck, 5  left upper torso, 6 right upper torso, 7 left lower torso, 8 right lower torso, 9 r knee, 10 l knee, 11  rfoot ,12  lfoot, 13 body centre
        
                
        //TODO  -  CHECK CROSSED ARMS
        
        //adjust for torso position!
        int gestureThreshold = 60;
        float CrossedArmsTolerance=0.2;
        
        //TODO include torso bottom in this torso centre point
        //0 is on the left 1 is on the right so leftUpperTorso + half the difference between left and right
        ofPoint torsoCentre = ofPoint ( jointCoordinates[5].x + abs( jointCoordinates[6].x-jointCoordinates[5].x )  , jointCoordinates[5].y + abs( jointCoordinates[6].y-jointCoordinates[5].y )   ,jointCoordinates[5].z + abs( jointCoordinates[6].z-jointCoordinates[5].z )   );
        //GESTURE 0  ARMS CROSSED
        if (jointCoordinates[0].x<torsoCentre.x && jointCoordinates[1].x >torsoCentre.x ){//&& jointCoordinates[0].x -jointCoordinates[1].x > CrossedArmsTolerance) {
            gestureStates[0]=true;
            gestureTimers[0]++;
        }
        else{
            gestureStates[0]=false;
            gestureTimers[0];
        }
        if (gestureTimers[0]>gestureThreshold) {
            gestureTimers[0]=0;
        }
        
        
        
        float twoHandsTol=0.1;
        //two_hands_on_left_side
        if (jointCoordinates[0].x< torsoCentre.x-twoHandsTol && jointCoordinates[1].x < torsoCentre.x-twoHandsTol) {
            gestureStates[1]=true;
            gestureTimers[1]++;
        }
        else{
            gestureStates[1]=false;
            gestureTimers[1];
        }
        if (gestureTimers[1]>gestureThreshold) {
            gestureTimers[1]=0;
        }
        
        //two_hands_on_right_side
        if (jointCoordinates[0].x> torsoCentre.x+twoHandsTol && jointCoordinates[1].x > torsoCentre.x+twoHandsTol) {
            gestureStates[2]=true;
            gestureTimers[2]++;
        }
        else{
            gestureStates[2]=false;
            gestureTimers[2];
        }
        if (gestureTimers[2]>gestureThreshold) {
            gestureTimers[2]=0;
        }
        
        //left_hand_is_over_right
        
        float tolerance=0.5;
        //if left is over right and more than 0.2 away from it then this true TODO add must be near torso too
        if ( jointCoordinates[0].y > jointCoordinates[1].y && jointCoordinates[0].y -jointCoordinates[1].y > tolerance  ) {
            gestureStates[3]=true;
            gestureTimers[3]++;
        }
        else{
            gestureStates[3]=false;
            gestureTimers[3];
        }
        if (gestureTimers[3]>gestureThreshold) {
            gestureTimers[3]=0;
        }
        //right_hand_is_over_left
        
        //if left is over right and more than 0.2 away from it then this true TODO add must be near torso too
        if ( jointCoordinates[1].y > jointCoordinates[0].y && jointCoordinates[1].y -jointCoordinates[0].y > tolerance ) {
            gestureStates[4]=true;
            gestureTimers[4]++;
        }
        else{
            gestureStates[4]=false;
            gestureTimers[4];
        }
        if (gestureTimers[4]>gestureThreshold) {
            gestureTimers[4]=0;
        }
        
        //if arms are boxy
        //        //joints are in order 0 rhand , 1 lhand , 2 rfoot, 3 lfoot , 4 head , 5 torso, 6 relbow, 7 lelbow
        
        float boxTolerance=0.02;
        if ( abs(jointCoordinates[0].x - jointCoordinates[3].x)<boxTolerance && abs(jointCoordinates[1].x - jointCoordinates[2].x)<boxTolerance ) {
            gestureStates[5]=true;
            gestureTimers[5]++;
        }
        else{
            gestureStates[5]=false;
            gestureTimers[5];
        }
        if (gestureTimers[5]>gestureThreshold) {
            gestureTimers[5]=0;
        }
        
        
    }
    
    
    
    return gestureStates;
}