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
    string command;
    FILE * arduino;
    FILE * test;
    arduino = fopen("/dev/cu.usbmodem1411","w");
    test = fopen("test.txt","w");
    if (test == NULL) {
        printf("not open\n");
        printf("%s", strerror(errno));
        return -1 ;
    }
    else
        printf("arduino opened\n");
    
    
    while(command != "s")
    {
        cout << "Give me command\n";
        cin >> command;
        
        if(command == "g") {
            angle = "t30i0m0r0p0";
        } else if(command == "r"){
            angle = "t100i90m130r90p90";
        } else if(command == "m"){
            angle = "t30i0m130r0p0";
        }

        
        fprintf(test,"%s", angle.c_str());
        fclose(test);
        
        
        sleep(1);
    }
    
}
