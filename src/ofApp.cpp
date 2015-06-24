#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofBackground(255, 255, 255);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    
    midiIn.listPorts(); //コンソールにポートのリストを表示
    
    midiIn.openPort(1); //ポート番号指定して開く //リストを見て
    
    // don't ignore sysex, timing, & active sense messages,
    // these are ignored by default
    midiIn.ignoreTypes(false, false, false);
    
    // add ofApp as a listener
    midiIn.addListener(this);  //ofAppをmidiのリスナー登録
    
    // コンソールに表示するかどうか
    midiIn.setVerbose(true);
    
    
    //出力関係 Setup
    manager = ofxMidiOutputManager();
    ofSoundStreamSetup(2, 0, this, 44100, 1024, 4);
    
    /*----------------------------------------------------*/
    midiOut = ofxMidiOut();
    //outputExample
    midiOut.openPort(0);
    channel = 1;
    currentPgm = 0;
    note = 0;
    velocity = 0;
    pan = 0;
    bend = 0;
    touch = 0;
    polytouch = 0;
    /*----------------------------------------------------*/
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    // Sampleから引っ張ってきた
     ofSetColor(0);
     
     // draw the last recieved message contents to the screen
     text << "Received: " << ofxMidiMessage::getStatusString(midiMessage.status);
     ofDrawBitmapString(text.str(), 20, 20);
     text.str(""); // clear
     
     text << "channel: " << midiMessage.channel;
     ofDrawBitmapString(text.str(), 20, 34);
     text.str(""); // clear
     
     text << "pitch: " << midiMessage.pitch;
     ofDrawBitmapString(text.str(), 20, 48);
     text.str(""); // clear
     ofRect(20, 58, ofMap(midiMessage.pitch, 0, 127, 0, ofGetWidth()-40), 20);
     
     text << "velocity: " << midiMessage.velocity;
     ofDrawBitmapString(text.str(), 20, 96);
     text.str(""); // clear
     ofRect(20, 105, ofMap(midiMessage.velocity, 0, 127, 0, ofGetWidth()-40), 20);
     
     text << "control: " << midiMessage.control;
     ofDrawBitmapString(text.str(), 20, 144);
     text.str(""); // clear
     ofRect(20, 154, ofMap(midiMessage.control, 0, 127, 0, ofGetWidth()-40), 20);
     
     text << "value: " << midiMessage.value;
     ofDrawBitmapString(text.str(), 20, 192);
     text.str(""); // clear
     if(midiMessage.status == MIDI_PITCH_BEND) {
     ofRect(20, 202, ofMap(midiMessage.value, 0, MIDI_MAX_BEND, 0, ofGetWidth()-40), 20);
     }
     else {
     ofRect(20, 202, ofMap(midiMessage.value, 0, 127, 0, ofGetWidth()-40), 20);
     }
     
     text << "delta: " << midiMessage.deltatime;
     ofDrawBitmapString(text.str(), 20, 240);
     text.str(""); // clear
     
     
    
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



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch(key) {
        case 'l':
            midiIn.listPorts();
            break;
    }
    
    
    
    
    /*----------------------------------------------------*/
    //outputExample
    // send a note on if the key is a letter or a number
    if(isalnum((unsigned char) key)) {
        
        // scale the ascii values to midi velocity range 0-127
        // see an ascii table: http://www.asciitable.com/
        note = ofMap(key, 48, 122, 0, 127);
        velocity = 64;
        midiOut.sendNoteOn(channel, note,  velocity);
        //manager.willConstBuffer(note);
        cout << note << "note" << endl;
    }
    
    if(key == 'l') {
        ofxMidiOut::listPorts();
    }
    /*----------------------------------------------------*/
}

//--------------------------------------------------------------
void ofApp::exit() {
    
    midiIn.closePort();
    midiIn.removeListener(this);
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    

    /*----------------------------------------------------*/
    //outpuExample
     
     if(isalnum((unsigned char) key)) {
     
     // scale the ascii values to midi velocity range 0-127
     // see an ascii table: http://www.asciitable.com/
     note = ofMap(key, 48, 122, 0, 127);
     //manager.wontConstBuffer(note);
     cout << note << "note" << endl;
     }
    
     
     
    switch(key) {
            
            // send pgm change on arrow keys
        case OF_KEY_UP:
            currentPgm = (int) ofClamp(currentPgm+1, 0, 127);
            midiOut.sendProgramChange(channel, currentPgm);
            break;
        case OF_KEY_DOWN:
            currentPgm = (int) ofClamp(currentPgm-1, 0, 127);
            
            midiOut << ProgramChange(channel, currentPgm); // stream interface
            break;
            
            // aftertouch
        case '[':
            touch = 64;
            midiOut.sendAftertouch(channel, touch);
            break;
        case ']':
            touch = 127;
            midiOut << Aftertouch(channel, touch); // stream interface
            break;
            
            // poly aftertouch
        case '<':
            polytouch = 64;
            midiOut.sendPolyAftertouch(channel, 64, polytouch);
            break;
        case '>':
            polytouch = 127;
            midiOut << PolyAftertouch(channel, 64, polytouch); // stream interface
            break;
            
            // sysex using raw bytes (use shift + s)
        case 'S': {
            // send a pitch change to Part 1 of a MULTI on an Akai sampler
            // from http://troywoodfield.tripod.com/sysex.html
            //
            // do you have an S2000 to try?
            //
            // note: this is probably not as efficient as the next two methods
            //       since it sends only one byte at a time, instead of all
            //       at once
            //
            midiOut.sendMidiByte(MIDI_SYSEX);
            midiOut.sendMidiByte(0x47);	// akai manufacturer code
            midiOut.sendMidiByte(0x00); // channel 0
            midiOut.sendMidiByte(0x42); // MULTI
            midiOut.sendMidiByte(0x48); // using an Akai S2000
            midiOut.sendMidiByte(0x00); // Part 1
            midiOut.sendMidiByte(0x00);	// transpose
            midiOut.sendMidiByte(0x01); // Access Multi Parts
            midiOut.sendMidiByte(0x4B); // offset
            midiOut.sendMidiByte(0x00);	// offset
            midiOut.sendMidiByte(0x01); // Field size = 1
            midiOut.sendMidiByte(0x00); // Field size = 1
            midiOut.sendMidiByte(0x04); // pitch value = 4
            midiOut.sendMidiByte(0x00); // offset
            midiOut.sendMidiByte(MIDI_SYSEX_END);
            
            // send again using a vector
            //
            // sends all bytes within one message
            //
            vector<unsigned char> sysexMsg;
            sysexMsg.push_back(MIDI_SYSEX);
            sysexMsg.push_back(0x47);
            sysexMsg.push_back(0x00);
            sysexMsg.push_back(0x42);
            sysexMsg.push_back(0x48);
            sysexMsg.push_back(0x00);
            sysexMsg.push_back(0x00);
            sysexMsg.push_back(0x01);
            sysexMsg.push_back(0x4B);
            sysexMsg.push_back(0x00);
            sysexMsg.push_back(0x01);
            sysexMsg.push_back(0x00);
            sysexMsg.push_back(0x04);
            sysexMsg.push_back(0x00);
            sysexMsg.push_back(MIDI_SYSEX_END);
            midiOut.sendMidiBytes(sysexMsg);
            
            // send again with the byte stream interface
            //
            // builds the message, then sends it on FinishMidi()
            //
            midiOut << StartMidi() << MIDI_SYSEX
            << 0x47 << 0x00 << 0x42 << 0x48 << 0x00 << 0x00 << 0x01
            << 0x4B << 0x00 << 0x01 << 0x00 << 0x04 << 0x00
            << MIDI_SYSEX_END << FinishMidi();
            break;
        }
            
            // print the port list
        case '?':
            midiOut.listPorts();
            break;
            
            // note off using raw bytes
        case ' ':	
            // send with the byte stream interface, noteoff for note 60
            midiOut << StartMidi() << 0x80 << 0x3C << 0x40 << FinishMidi();
            break;
            
        default:
            
            // send a note off if the key is a letter or a number
            if(isalnum(key)) {
                note = ofMap(key, 48, 122, 0, 127);
                velocity = 0;
                midiOut << NoteOff(channel, note, velocity); // stream interface
            }
            break;
    }
    /*----------------------------------------------------*/
    
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
