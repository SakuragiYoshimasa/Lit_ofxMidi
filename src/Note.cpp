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


}

void Note::constructBuffer(float *buffer, int bufferSize, bool overLay){
    
    for (int i = 0; i < bufferSize; i++) {
        phase += phasePerSample;
        while (phase > TWO_PI) phase -= TWO_PI;
        float value = ampritude * sin(phase);
        
        if (overLay) {
            buffer[i * 2    ] += value;
            buffer[i * 2 + 1] += value;
        }
        else {
            buffer[i * 2    ] = value;
            buffer[i * 2 + 1] = value;
        }
    }
}