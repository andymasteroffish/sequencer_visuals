//
//  ArduinoManager.cpp
//  sequencer
//
//  Created by Andrew Wallace on 9/9/17.
//
//

#include "ArduinoManager.hpp"


void ArduinoManager::setup(){
#ifdef USING_ARCADE

    couldConnect = false;
    
	#ifdef USING_OSX
    couldConnect = ard.connect("/dev/tty.usbserial-A700fitl", 57600);
	#endif

	#ifdef USING_WIN
	couldConnect = ard.connect("COM3", 57600);
	#endif
    
    cout<<"test "<<couldConnect<<endl;
    
    bSetupArduino	= false;
    
    // listen for EInitialized notification.
    ofAddListener(ard.EInitialized, this, &ArduinoManager::setupArduino);
    cout<<"setup arduino"<<endl;
#endif
}

void ArduinoManager::update(){
#ifdef USING_ARCADE
    if (couldConnect){
        ard.update();
    }
#endif
}


void ArduinoManager::setupArduino(const int & version) {
#ifdef USING_ARCADE
    // remove listener because we don't need it anymore
    ofRemoveListener(ard.EInitialized, this, &ArduinoManager::setupArduino);
    
    // it is now safe to send commands to the Arduino
    bSetupArduino = true;
    
    // print firmware name and version to the console
    cout << ard.getFirmwareName() << endl;
    cout << "firmata v" << ard.getMajorFirmwareVersion() << "." << ard.getMinorFirmwareVersion() <<endl;
    
    // Note: pins A0 - A5 can be used as digital input and output.
    // Refer to them as pins 14 - 19 if using StandardFirmata from Arduino 1.0.
    // If using Arduino 0022 or older, then use 16 - 21.
    // Firmata pin numbering changed in version 2.3 (which is included in Arduino 1.0)
    
    // set all pins as digital output
    for (int i=0; i<20; i++){
        ard.sendDigitalPinMode(i, ARD_OUTPUT);
    }
#endif
}

void ArduinoManager::clear(){
    for (int i=0; i<16; i++){
        setSound(i, false);
    }
}
void ArduinoManager::setSound(int soundID, bool isOn){
#ifdef USING_ARCADE
    int pinID = soundID + 4;
    
    if (bSetupArduino){
        ard.sendDigital(pinID, isOn);
    }
#endif
}
