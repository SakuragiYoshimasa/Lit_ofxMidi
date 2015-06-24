//
//  ofxMidiOutputManager.h
//  
//
//  Created by 櫻木善将 on 2015/06/23.
//
//

#ifndef ____ofxMidiOutputManager__
#define ____ofxMidiOutputManager__

#include <stdio.h>
#include "Note.h"


class ofxMidiOutputManager {

private:
    Note notes[128];
    
public:
    ofxMidiOutputManager(){
        for(int i = 0; i < 128; i++){
            notes[i] = Note();
            notes[i].init(10.0 * i, 0.5); //imprement
        }
    }
    ~ofxMidiOutputManager(){}
    
    void willConstBuffer(int index);
    
    void constructBuffers(float *buffer , int bufferSize);


};

#endif /* defined(____ofxMidiOutputManager__) */
