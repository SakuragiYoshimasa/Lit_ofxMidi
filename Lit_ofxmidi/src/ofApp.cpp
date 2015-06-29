#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofBackground(255, 255, 255);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    midiIn.listPorts(); //コンソールにポートのリストを表示
    midiIn.openPort(1); //ポート番号指定して開く
    
    
    
    midiIn.addListener(this);  //ofAppをmidiのリスナー登録
    
    // コンソールに表示するかどうか
    midiIn.setVerbose(true);
    midiIn.ignoreTypes(false, false, false);
    
    //出力関係 Setup
    manager = ofxMidiOutputManager();
    ofSoundStreamSetup(2, 0, this, 44100, 1024, 4);
}


//--------------------------------------------------------------
//リスナ用の関数
//新しい入力があればmidiMessageに格納
//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {
    
    // make a copy of the latest message
    midiMessage = msg;
    
    
    //Pitchの所を出力するように
    if(midiMessage.getStatusString(midiMessage.status) == "Note On"){
        manager.willConstBuffer(midiMessage.pitch);
    }
}



//-----------
//出力用
//波形出力後、次の入力ができるようになれば呼び出される。
//-----------
void ofApp::audioRequested(float *buf, int bufSize, int nChan){
    manager.constructBuffers(buf, bufSize);
};

void ofApp::exit() {
    midiIn.closePort();
    midiIn.removeListener(this);
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(255,255,255);
    ofColor color;
    color.setHsb(midiMessage.pitch * 2, 255, 255);
    ofSetColor(color);
    ofCircle(ofGetWidth()/2, ofGetHeight()/2, midiMessage.pitch * 2);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
