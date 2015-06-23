//
//  Note.h
//  
//
//  Created by 櫻木善将 on 2015/06/23.
//
//



#pragma once
#include "ofMain.h"
//こいつを使ってmidi出力0~127に対し周波数を対応させ鳴らす。
class Note {
    private:
        float frequency, ampritude;
        float phase , phasePerSample;
    public:
        void init(float freq,float amp);  //周波数と振幅で初期化
        void constructBuffer(float *buffer , int bufferSize , bool overLay);
};
