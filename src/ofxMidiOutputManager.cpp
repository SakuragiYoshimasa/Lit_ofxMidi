//
//  ofxMidiOutputManager.cpp
//  
//
//  Created by 櫻木善将 on 2015/06/23.
//
//

#include "ofxMidiOutputManager.h"

void ofxMidiOutputManager::constructBuffers(float *buffer, int bufferSize){
    //各音に対しBuffer構築処理
    //最初bufferの初期化と構築後、willconstructBufferをfalseに
    for(int i = 0; i < bufferSize; i++){
        buffer[2 * i] = 0;
        buffer[2 * i + 1] = 0;
   
    }
    
    for(int i = 0; i < 128; i++){
        if(notes[i].getWillConBuf()){
            notes[i].constructBuffer(buffer, bufferSize);
        }
    }
}

void ofxMidiOutputManager::willConstBuffer(int index){
    
    notes[index].willConstruntBuffer();
    cout << "index" << index << "will" << endl;;

}



