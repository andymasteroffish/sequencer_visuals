//
//  ArduinoManager.hpp
//  sequencer
//
//  Created by Andrew Wallace on 9/9/17.
//
//

#ifndef ArduinoManager_hpp
#define ArduinoManager_hpp

#include "ofMain.h"

class ArduinoManager{
public:
    
    void setup();
    //void setupArduino();
    void update();
    
    void clear();
    void setSound(int soundID, bool isOn);
    
    bool bSetupArduino;
    ofArduino	ard;
    
private:
    
    void setupArduino(const int & version);
};

#endif /* ArduinoManager_hpp */
