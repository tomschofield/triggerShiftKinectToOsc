/*
 
 cc tom schofield 2012
 This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 
 I acknowledge the following code libraries
 oxOpenNI, 
 Copyright 2011 (c) Matthew Gingold http://gingold.com.au
 * Originally forked from a project by roxlu http://www.roxlu.com/ 
 
 ofxOsc, 
 Copyright (c) 2007-2009, Damian Stewart
 All rights reserved
 
 and osc message format to fit
 osceleton https://github.com/Sensebloom/OSCeleton 
 
 The above copyleft notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED ÒAS ISÓ, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYLEFT HOLDERS BE LIABLE FOR ANY CLAIM, 
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 DEALINGS IN THE SOFTWARE.
 */



#ifndef _TEST_APP
#define _TEST_APP

//#define USE_IR // Uncomment this to use infra red instead of RGB cam...

#include "ofxOpenNI.h"
#include "ofMain.h"
#include "ofxOsc.h"
#include "KinectGesture.h"
#include "User.h"

#define HOST "localhost"
#define PORT 7110

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
    
    void jointOSCMessage(ofxLimb * jointName, string jointString, int nUserNum);
    void userJointOSCMessage(ofPoint jointName, string jointString, int nUserNum);
    void handOSCMessage(ofxTrackedHand * aHand, string handString, int nHandNum);
	void gestureOSCmessage(string gestureName, bool gestureIsOn, int nUserNum);
    
    void sendUserInfo(ofxTrackedUser * aUser, int nUserNum);
    void sendHandInfo(ofxTrackedHand * aHand, int nUserNum);
    void sendNewUserInfo(int nUserNum);
    
    
    void	setupOpenNI();
	void	setupPlayback(string _filename);
	string	generateFileName();
    
    //returns vector of joint points and has a pointer to given vector of strings TODO - fix this ridiculous pointer approach
    vector<ofPoint> passJointInfo(ofxTrackedUser * aUser, vector<string> & jointNames,   int nUserNum);
    vector<ofPoint>     user1Joints;
    vector<string>      user1JointNames;

    ofxOscSender        sender;
	bool				isTracking;
    
    float tilt;
	
    ofxOpenNIContext	context;
	ofxDepthGenerator	depth;
    ofTrueTypeFont      font;
    
    vector<User> users;
    
#ifdef USE_IR
	ofxIRGenerator		image;
#else
	ofxImageGenerator	image;
#endif
    //not currently used
	ofxHandGenerator	handTracker;

	ofxUserGenerator	user;

#if defined (TARGET_OSX) //|| defined(TARGET_LINUX) // only working on Mac/Linux at the moment (but on Linux you need to run as sudo...)
	ofxHardwareDriver	hardware;
#endif

	void				drawMasks();
	void				drawPointCloud(ofxUserGenerator * user_generator, int userID);

	int					nearThreshold, farThreshold;
	int					pointCloudRotationY;

	ofImage				allUserMasks, user1Mask, user2Mask, depthRangeMask;

	float				filterFactor;
    //actually position rather than gesture
    KinectGesture       kGesture;

};

#endif
