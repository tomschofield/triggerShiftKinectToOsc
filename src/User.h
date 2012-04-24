

#ifndef _User_
#define _User_

#include "ofMain.h"
#include "KinectGesture.h"

using namespace std;

class User{
    
public:
    void setup();
    void update(vector<ofPoint> joints);
    void draw();
    
    int     uId;
    bool    isAlive;
    int     numJointsTracking;
    float   rightHandX, rightHandY, rightHandZ;
    float   leftHandX, leftHandY, leftHandZ;
    float   headX, headY, headZ;
    float   torsoX, torsoY, torsoZ;
    float   leftFootX, leftFootY, leftFootZ;
    float   rightFootX, rightFootY, rightFootZ;
    
    vector<ofPoint> allJoints;
    vector<string> jointNames;
    KinectGesture gesture;
    vector<bool> gestureResults;
    vector<string>gestureNames;
    ofTrueTypeFont font;
    ofColor bodyColour;
    ofPixels pixels;
};
#endif

