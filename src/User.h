/*
 cc tom schofield 2012
 This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 
 The above copyleft notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYLEFT HOLDERS BE LIABLE FOR ANY CLAIM, 
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 DEALINGS IN THE SOFTWARE.
 */

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

