#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxMidiOutputManager.h"

class ofApp : public ofBaseApp ,public ofxMidiListener{ //Listenerdelegateなので新しいメッッセージを受けれる

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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void exit();
    
        void newMidiMessage(ofxMidiMessage& msg);
    
        void audioRequested (float *buf, int bufSize, int nChan);
        stringstream text;
    
        ofxMidiIn midiIn; //注意！！！　coreMidiFramework を追加する！
        ofxMidiMessage midiMessage;
        ofxMidiOutputManager manager;
    

        /*----------------------------------------------------*/
        //outputExample
        ofxMidiOut midiOut;
        int channel;
        unsigned int currentPgm;
        int note, velocity;
        int pan, bend, touch, polytouch;
        /*----------------------------------------------------*/
		
};
