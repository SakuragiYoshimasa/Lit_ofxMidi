//
//  Note.cpp
//  
//
//  Created by 櫻木善将 on 2015/06/23.
//
//

#include "Note.h"

void Note::init(float freq, float amp){
    
    phase = 0;
    frequency = freq;
    ampritude = amp;
    phasePerSample = TWO_PI * freq / 44100;
    willConstructBuffer = false;
    attenuation = 0.0;


}

void Note::constructBuffer(float *buffer, int bufferSize){
    
    for (int i = 0; i < bufferSize; i++) {
        phase += phasePerSample;
        while (phase > TWO_PI) phase -= TWO_PI;
        float value = ampritude * sin(phase) * exp(attenuation);
    
        buffer[i * 2    ] += value;
        buffer[i * 2 + 1] += value;
    }
    attenuation -= 0.1;
    
    if(attenuation < -10.0){
        willConstructBuffer = false;
        attenuation = 0.0;
    }
    
}

void Note::willConstruntBuffer(){
    willConstructBuffer = true;
    attenuation = 0.0;
}

bool Note::getWillConBuf(){
    return willConstructBuffer;
}