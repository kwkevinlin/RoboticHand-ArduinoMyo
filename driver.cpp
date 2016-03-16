#include <iostream>
#include <unistd.h>
#include <fstream>

/*
    Next:
        Use bluetooth to establish serial
*/

using namespace std;

int main()
{
    unsigned int angle;
    fstream arduino;

    cout << "check-1";

    //Plug in, can check
    //arduino.open("/dev/tty.usbmodem3a21");

    int fd;                             // File descriptor
    // Open port
    
    fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        printf("Device cannot be opened.\n");
        exit(-1);                       // If the device is not open, return -1
    }

    cout<<"check-2";

    if(arduino)
    {
        do
        {
            cout<<"\n\ninsert a number between 0 and 179";

            cin>>angle;
            arduino<<angle;

        }while(angle <= 179);

        arduino.close();
    }
    else
    {
        cout<<"\n\nERROR!!\n\n";
    }


}