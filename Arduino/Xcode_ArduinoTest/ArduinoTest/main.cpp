//
//  main.cpp
//  ArduinoTest
//
//  Created by Cyrus Liu on 3/18/16.
//  Copyright © 2016 Cyrus Liu. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <string>

using namespace std;

int main()
{
    string angle;
    FILE * arduino;
    arduino = fopen("/dev/cu.usbmodem1411","w");
    if (arduino == NULL) {
        printf("not open\n");
        return -1 ;
    }
    else
        printf("arduino opened\n");
    
    while(angle != "stop")
    {
        
        
        cout<<"give me the angle\n";
        cin>>angle;
        
        fprintf(arduino,"%s", angle.c_str());
        sleep(1);
        
        //fclose(arduino);
    }
    
}
