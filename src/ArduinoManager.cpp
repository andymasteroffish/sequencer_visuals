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
    
    //cout<<"test "<<couldConnect<<endl;
    
    bSetupArduino	= false;
    
    // listen for EInitialized notification.
    ofAddListener(ard.EInitialized, this, &ArduinoManager::setupArduino);
    cout<<"setup arduino"<<endl;
#endif
    
    //set the pin IDs since a few of them aren't in exact order
    pinIDs[0] = 7;		//dot polygon
    pinIDs[1] = 6;		//sweep
    pinIDs[2] = 5;		//buckshot
    pinIDs[3] = 4;		//clap

    pinIDs[4] = 14;		//crystal
    pinIDs[5] = 11;		//triangle
    pinIDs[6] = 16;		//drunk snake
    pinIDs[7] = 17;		//tunnel

    pinIDs[8] = 18;		//sizzle
	pinIDs[9] = 8;		//spokes
    pinIDs[10] = 13;	//square
    pinIDs[11] = 12;	//wave column

    pinIDs[12] = 15;	//grapes
    pinIDs[13] = 10;	//slash	
    pinIDs[14] = 9;		//trapazoid
	pinIDs[15] = 19;	//chaser
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
    //int pinID = soundID + 4;
    
    if (bSetupArduino){
        ard.sendDigital(pinIDs[soundID], isOn);
    }
#endif
}
