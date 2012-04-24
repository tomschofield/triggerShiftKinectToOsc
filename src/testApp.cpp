#include "testApp.h"
#define NUM_JOINTS 8
#define NUM_USERS 7
//--------------------------------------------------------------
void testApp::setup() {

	
	isTracking		= true;
	nearThreshold = 500;
	farThreshold  = 1000;
    tilt=0;
	
    setupOpenNI();

	   
    sender.setup( HOST, PORT );
    user.setUseCloudPoints(true);
    font.loadFont("Bangla Sangam MN.ttf", 24);
   //user objects have their own set of joint info and gesture detection - this is kind of a redundancy but I wanted a light user object with just the stuff im interested in in it
    for(int i=0;i<NUM_USERS;i++){
        User tuser;
        tuser.setup();
        users.push_back(tuser);
    }
   

}

void testApp::setupOpenNI() {

#if defined (TARGET_OSX) //|| defined(TARGET_LINUX) // only working on Mac/Linux at the moment (but on Linux you need to run as sudo...)
	hardware.setup();				// libusb direct control of motor, LED and accelerometers
	hardware.setLedOption(LED_OFF); // turn off the led just for yacks (or for live installation/performances ;-)
#endif

	context.setup();	// all nodes created by code -> NOT using the xml config file at all
	//context.setupUsingXMLFile();
	depth.setup(&context);
	image.setup(&context);

	user.setup(&context);
	user.setSmoothing(filterFactor);				// built in openni skeleton smoothing...
	user.setUseMaskPixels(true);
	user.setUseCloudPoints(true);
	user.setMaxNumberOfUsers(7);					// use this to set dynamic max number of users (NB: that a hard upper limit is defined by MAX_NUMBER_USERS in ofxUserGenerator)
	context.toggleRegisterViewport();
	context.toggleMirror();
   
}

//--------------------------------------------------------------
void testApp::update(){

#ifdef TARGET_OSX // only working on Mac at the moment
	hardware.update();
#endif
        //record context is the context generator - this is basically the "manager" class for the user and hand tracking
		context.update();
		depth.update();
		image.update();

		// update tracking/recording nodes
		if (isTracking){
            user.update();
            
            //first assume all users are dead:
            for(int i=0;i<NUM_USERS;i++){
                users[i].isAlive=false;
            }
            
            //getNumberOfTrackedUsers is calculated by the user generator and only gives actually tracked users - but those users don't necessarily have a skeleton
             for(int i=0;i<user.getNumberOfTrackedUsers();i++){
               //  cout<<"user.getNumberOfTrackedUsers() "<<user.getNumberOfTrackedUsers()<<endl;
                 ofxTrackedUser * someUser=user.updateAndReturn(i);
                 //I had to hack around here to make sure we really really are tracking a skeleton - all the obvious flags inside the class seemed to be more like suggestions as to whether we are tracking or not :( TODO work this out
                // cout<<" someUser->id "<<someUser->id<<endl;
                 if (someUser->skeletonTracking) {
                    users[someUser->id].isAlive=true;
                    users[someUser->id].update(passJointInfo(someUser,users[someUser->id].jointNames,  i));
                    
                 }
               
             }
        }

    
            // demo getting depth pixels directly from depth gen
            depthRangeMask.setFromPixels(depth.getDepthPixels(nearThreshold, farThreshold), depth.getWidth(), depth.getHeight(), OF_IMAGE_GRAYSCALE);
			
            allUserMasks.setFromPixels(user.getUserPixels(), user.getWidth(), user.getHeight(), OF_IMAGE_GRAYSCALE);
			user1Mask.setFromPixels(user.getUserPixels(1), user.getWidth(), user.getHeight(), OF_IMAGE_GRAYSCALE);
			user2Mask.setFromPixels(user.getUserPixels(2), user.getWidth(), user.getHeight(), OF_IMAGE_GRAYSCALE);

    int userId=1;  
    
    //send all user joint and gesture info over osc
    for(int i=0;i<NUM_USERS;i++){
        if(users[i].isAlive){
            ofxOscMessage m;
            m.setAddress("/choose_user");
            m.addIntArg(userId);
            sender.sendMessage( m );
            m.clear();
            
            for (int j=0;j<users[i].allJoints.size(); j++) {
                userJointOSCMessage(users[i].allJoints[j], users[i].jointNames[j], userId);
                for(int j=0;j<users[i].gestureResults.size();j++){
                    gestureOSCmessage(users[i].gestureNames[j], users[i].gestureResults[j], i);
                }
            }
            userId++;
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0, 0, 0);
    for(int i=0;i<NUM_USERS;i++){
        users[i].draw();  
    }
  
}

void testApp:: drawMasks() {
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
	allUserMasks.draw(640, 0, 640, 480);
	glDisable(GL_BLEND);
    glPopMatrix();
	user1Mask.draw(320, 480, 320, 240);
	user2Mask.draw(640, 480, 320, 240);
   
        
	
}

void testApp::drawPointCloud(ofxUserGenerator * user_generator, int userID) {

	glPushMatrix();

	int w = user_generator->getWidth();
	int h = user_generator->getHeight();

	

	int step = 1;
   
    ofTranslate(0, 0   ,-10*mouseX);
	glBegin(GL_POINTS);
    step=1;

	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			ofPoint pos = user_generator->getWorldCoordinateAt(x, y, userID);
			if (pos.z == 0 && true) continue;	// gets rid of background -> still a bit weird if userID > 0...
			ofColor color = user_generator->getWorldColorAt(x,y, userID);
			glColor4ub((unsigned char)color.r, (unsigned char)color.g, (unsigned char)color.b, (unsigned char)color.a);
			glVertex3f(pos.x, pos.y, pos.z);
		}
	}

	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);

	glPopMatrix();
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
            
            float smooth;
            
            switch (key) {
#ifdef TARGET_OSX // only working on Mac at the moment
                case 357: // up key
                    tilt++;
                    hardware.setTiltAngle(tilt);
                    break;
                case 359: // down key
                    tilt--;
                    hardware.setTiltAngle(tilt);

                    break;
#endif
    }
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    pointCloudRotationY = x;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

void testApp::gestureOSCmessage(string gestureName, bool gestureIsOn, int nUserNum){
    ofxOscMessage m;
  
    m.setAddress( "/gesture"  );
    m.addStringArg(gestureName);
    m.addIntArg(gestureIsOn);
    m.addIntArg(nUserNum);
   
    
    sender.sendMessage( m );
    //rip off basic hand positions this way - the first 2 points in the positions array are the start and end, the 3rd i guess is the centre.
    m.clear();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
}

//send an osc message from the ofxOpenNi ofxLimb data
//----------------------------------------
void testApp::jointOSCMessage(ofxLimb * jointName, string jointString, int nUserNum){
    ofxOscMessage m;
    m.setAddress( "/joint"  );
    m.addStringArg(jointString);
    m.addIntArg(nUserNum);
    m.addFloatArg(ofMap(jointName->position->X,0,640,0.,1.));
    m.addFloatArg(ofMap(jointName->position->Y,0,480,0.,1.) );
    
    //there is something extremely funky about this mapping what the hell is the z range? 
    m.addFloatArg(ofMap(jointName->position->Z,0,1000,0.,7.) );
    
    
    
    sender.sendMessage( m );
    //rip off basic hand positions this way - the first 2 points in the positions array are the start and end, the 3rd i guess is the centre.
    m.clear();
    if(jointString=="right_lower_arm"){

        m.setAddress( "/joint"  );
        m.addStringArg("right_hand");
        m.addIntArg(nUserNum);
        m.addFloatArg(ofMap(jointName->position[1].X,0,640,0.,1.));
        m.addFloatArg(ofMap(jointName->position[1].Y,0,480,0.,1.) );
        m.addFloatArg(ofMap(jointName->position[1].Z,0,1000,0.,7.) );
        
        
        
        sender.sendMessage( m );

    }
    m.clear();
    if(jointString=="left_lower_arm"){
        
        m.setAddress( "/joint"  );
        m.addStringArg("left_hand");
        m.addIntArg(nUserNum);
        m.addFloatArg(ofMap(jointName->position[1].X,0,640,0.,1.));
        m.addFloatArg(ofMap(jointName->position[1].Y,0,480,0.,1.) );
        m.addFloatArg(ofMap(jointName->position[1].Z,0,1000,0.,7.) );
        
        
        
        sender.sendMessage( m );
        
    }    
}
//send an osc message from the my thin user class

//----------------------------------------
void testApp::userJointOSCMessage(ofPoint jointName, string jointString, int nUserNum){
    ofxOscMessage m;
    m.setAddress( "/joint"  );
    m.addStringArg(jointString);
    m.addIntArg(nUserNum);
   // m.addFloatArg(ofMap(jointName.x,0,640,0.,1.));
   // m.addFloatArg(ofMap(jointName.y,0,480,0.,1.) );
    
    m.addFloatArg(jointName.x);
    m.addFloatArg(jointName.y);
    
    //there is something extremely funky about this mapping what the hell is the z range? 
   // m.addFloatArg(ofMap(jointName.z,0,1000,0.,7.) );
    m.addFloatArg(jointName.z);
    sender.sendMessage( m );
    //rip off basic hand positions this way - the first 2 points in the positions array are the start and end, the 3rd i guess is the centre.
    m.clear();
   
}

void testApp::handOSCMessage(ofxTrackedHand * aHand, string handString, int nHandNum){
    ofxOscMessage m;
    /*
     Address pattern: "/joint"
     Type tag: "sifff"
     s: Joint name, check out the full list of joints below.
     i: The ID of the user.
     f: X coordinate of joint in interval [0.0, 1.0]
     f: Y coordinate of joint in interval [0.0, 1.0]
     f: Z coordinate of joint in interval [0.0, 7.0]
     */
    
    m.setAddress( "/joint"  );
    m.addStringArg(handString);
    m.addIntArg(nHandNum);
    m.addFloatArg(aHand->progPos.x );
    m.addFloatArg(aHand->progPos.y );
    m.addFloatArg(aHand->progPos.z );
    
    sender.sendMessage( m );
}
//send hand positions over OSC
void testApp::sendHandInfo(ofxTrackedHand * aHand, int nHandNum) {
   
    //hmm can't work out how to tell left from right unless we do this witht he calibration pose and then just stick with
    //of compare to lower hand pos in skeleton....
    handOSCMessage( aHand,"hand", nHandNum);

    
    
}
//send user info(start counting at 0) over OSC 
//----------------------------------------
void testApp::sendUserInfo(ofxTrackedUser * aUser, int nUserNum) {
    

    jointOSCMessage(& aUser->left_shoulder,"l_shoulder", nUserNum);
    jointOSCMessage(& aUser->left_upper_arm,"l_upper_arm", nUserNum);
    jointOSCMessage(& aUser->left_lower_arm,"l_lower_arm", nUserNum);
    
    jointOSCMessage(& aUser->right_shoulder,"r_shoulder", nUserNum);
    jointOSCMessage(& aUser->right_upper_arm,"r_upper_arm", nUserNum);
    jointOSCMessage(& aUser->right_lower_arm,"r_lower_arm", nUserNum);
    
    jointOSCMessage(& aUser->left_upper_torso,"l_upper_torso", nUserNum);
    jointOSCMessage(& aUser->right_upper_torso,"r_upper_torso", nUserNum);
    
    jointOSCMessage(& aUser->left_lower_torso,"l_lower_torso", nUserNum);
    jointOSCMessage(& aUser->left_upper_leg,"l_upper_leg", nUserNum);
    jointOSCMessage(& aUser->left_lower_leg,"l_lower_leg", nUserNum);
    
    jointOSCMessage(& aUser->right_lower_torso,"r_lower_torso", nUserNum);
    jointOSCMessage(& aUser->right_upper_leg,"r_upper_leg", nUserNum);
    jointOSCMessage(& aUser->right_lower_leg,"r_lower_leg", nUserNum);
    
    jointOSCMessage(& aUser->hip,"hip", nUserNum);
    
}
//puts all the user joint info into a single vector of ofPoints  - each joint position has, confusingly 3 seperate points for the 2 ends and the middle
vector<ofPoint> testApp::passJointInfo(ofxTrackedUser * aUser, vector<string> &jointNames, int nUserNum){
        //joints are in order 0 rhand , 1 lhand , 2 rfoot, 3 lfoot , 4 head , 5 torso, 6 relbow, 7 lelbow
   vector<ofPoint>jointPositions;
    int wMap = 640;
    int hMap = 480;
    int dMap  = 5000;
    jointNames.clear();
    jointNames.push_back("right_hand");
   
    jointPositions.push_back(ofPoint( ofMap(aUser->right_lower_arm.position[1].X ,0,wMap,0,1),ofMap (aUser->right_lower_arm.position[1].Y,0,hMap,0,1),ofMap (aUser->right_lower_arm.position[1].Z,0,dMap,0,1) )); 
    
    //left hand
    jointNames.push_back("left_hand");
    jointPositions.push_back(ofPoint(ofMap(aUser->left_lower_arm.position[1].X,0,wMap,0,1),ofMap(aUser->left_lower_arm.position[1].Y,0,hMap,0,1),ofMap (aUser->left_lower_arm.position[1].Z,0,dMap,0,1) ));
    
    //right elbow
    jointNames.push_back("r_elbow");
    jointPositions.push_back(ofPoint(ofMap(aUser->right_lower_arm.position[0].X,0,wMap,0,1),ofMap(aUser->right_lower_arm.position[0].Y,0,hMap,0,1),ofMap (aUser->right_lower_arm.position[0].Z,0,dMap,0,1) ));
    //left elbow
    jointNames.push_back("l_elbow");
    jointPositions.push_back(ofPoint(ofMap(aUser->left_lower_arm.position[0].X,0,wMap,0,1),ofMap(aUser->left_lower_arm.position[0].Y,0,hMap,0,1),ofMap (aUser->left_lower_arm.position[0].Z,0,dMap,0,1) ));

    jointNames.push_back("neck");
    jointPositions.push_back(ofPoint(ofMap(aUser->neck.position->X,0,wMap,0,1),ofMap(aUser->neck.position->Y,0,hMap,0,1),ofMap (aUser->neck.position->Z,0,dMap,0,1) ));
    //left upper torso
    jointNames.push_back("l_upper_torso");
    jointPositions.push_back(ofPoint(ofMap(aUser->left_upper_torso.position->X,0,wMap,0,1),ofMap(aUser->left_upper_torso.position->Y,0,hMap,0,1),ofMap (aUser->left_upper_torso.position->Z,0,dMap,0,1)));    //right upper torso
    jointNames.push_back("r_upper_torso");
    jointPositions.push_back(ofPoint(ofMap(aUser->right_upper_torso.position->X,0,wMap,0,1),ofMap(aUser->right_upper_torso.position->Y,0,hMap,0,1), ofMap (aUser->right_upper_torso.position->Z,0,dMap,0,1))); 
    
    //left upper torso
    jointNames.push_back("l_lower_torso");
    jointPositions.push_back(ofPoint(ofMap(aUser->left_lower_torso.position->X,0,wMap,0,1),ofMap(aUser->left_lower_torso.position->Y,0,hMap,0,1),ofMap (aUser->left_lower_torso.position->Z,0,dMap,0,1)));    //right upper torso
    jointNames.push_back("r_lower_torso");
    jointPositions.push_back(ofPoint(ofMap(aUser->right_lower_torso.position->X,0,wMap,0,1),ofMap(aUser->right_lower_torso.position->Y,0,hMap,0,1), ofMap (aUser->right_lower_torso.position->Z,0,dMap,0,1))); 
    
    //right knee
    jointNames.push_back("r_knee");
    jointPositions.push_back(ofPoint(ofMap(aUser->right_upper_leg.position[1].X,0,wMap,0,1),ofMap(aUser->right_upper_leg.position[1].Y,0,hMap,0,1),ofMap (aUser->right_upper_leg.position[1].Z,0,dMap,0,1) ));
    
    //left knee
    jointNames.push_back("l_knee");
    jointPositions.push_back(ofPoint(ofMap(aUser->left_upper_leg.position[1].X,0,wMap,0,1),ofMap(aUser->left_upper_leg.position[1].Y,0,hMap,0,1),ofMap (aUser->left_upper_leg.position[1].Z,0,dMap,0,1) ));
    
    //right foot
    jointNames.push_back("r_foot");
    jointPositions.push_back(ofPoint(ofMap(aUser->right_lower_leg.position[1].X,0,wMap,0,1),ofMap(aUser->right_lower_leg.position[1].Y,0,hMap,0,1),ofMap (aUser->right_lower_leg.position[1].Z,0,dMap,0,1) ));
    //left foot
    jointNames.push_back("l_foot");
    jointPositions.push_back(ofPoint(ofMap(aUser->left_lower_leg.position[1].X,0,wMap,0,1),ofMap(aUser->left_lower_leg.position[1].Y,0,hMap,0,1),ofMap (aUser->left_lower_leg.position[1].Z,0,dMap,0,1) ));
    //body centre - there is something deeply weird about this value  - seems to be in a -1 to 1 range rather than 0 to 1. 
    jointNames.push_back("body_centre");
    jointPositions.push_back(ofPoint(ofMap(aUser->center.X  ,0,wMap,0,1),ofMap(aUser->center.Y ,0,hMap,0,1),ofMap (aUser->center.Z,0,dMap,0,1) ));

    
       
//    jointPositions->push_back(ofPoint( ofMap(aUser->right_lower_arm.position[1].X ,0,wMap,0,1),ofMap (aUser->right_lower_arm.position[1].Y,0,hMap,0,1),aUser->right_lower_arm.position[1].Z,0,dMap,0,1) ));
    return jointPositions;
    
}