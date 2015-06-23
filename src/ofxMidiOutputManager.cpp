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
        buffer[i] = 0;
    }
    
    for(int i = 0; i < 128; i++){
        if(willConstructBuffer[i]){
            notes[i].constructBuffer(buffer, bufferSize, true);
        }
    }
}

void ofxMidiOutputManager::willConstBuffer(int index){
    
    willConstructBuffer[index] = true;
    cout << "index" << index << "will" << endl;;

}

void ofxMidiOutputManager::wontConstBuffer(int index){
    willConstructBuffer[index] = false;
        cout << "index" << index << "wont" << endl;;

}

