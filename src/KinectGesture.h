

#ifndef _KinectGesture_
#define _KinectGesture_

#include "ofMain.h"

using namespace std;

class KinectGesture{
    
public:
    void setup(int numJs);
    int numJoints;
    vector<bool>gestureStates; //there is a list of possible gestures each one with an on/off
    vector<bool> detect(vector<ofPoint> jointCoordinates);
    vector<int> gestureTimers;
    
};

#endif

