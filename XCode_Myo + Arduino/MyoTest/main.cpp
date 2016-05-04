// Copyright (C) 2013-2014 Thalmic Labs Inc.
// Distributed under the Myo SDK license agreement. See LICENSE.txt for details.
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <myo/myo.hpp>

std::string angle;
std::string command;
FILE * arduino;
FILE * test;

class DataCollector : public myo::DeviceListener {
public:
    DataCollector()
    : onArm(false), isUnlocked(false), roll_w(0), pitch_w(0), yaw_w(0), currentPose()
    {
    }
    // onUnpair() is called whenever the Myo is disconnected from Myo Connect by the user.
    void onUnpair(myo::Myo* myo, uint64_t timestamp) {
        // We've lost a Myo.
        // Let's clean up some leftover state.
        roll_w = 0;
        pitch_w = 0;
        yaw_w = 0;
        onArm = false;
        isUnlocked = false;
    }
    // onOrientationData() is called whenever the Myo device provides its current orientation, which is represented
    // as a unit quaternion.
    void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat) {
        using std::atan2;
        using std::asin;
        using std::sqrt;
        using std::max;
        using std::min;
        // Calculate Euler angles (roll, pitch, and yaw) from the unit quaternion.
        float roll = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),
                           1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
        float pitch = asin(max(-1.0f, min(1.0f, 2.0f * (quat.w() * quat.y() - quat.z() * quat.x()))));
        float yaw = atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),
                          1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));
        // Convert the floating point angles in radians to a scale from 0 to 18.
        roll_w = static_cast<int>((roll + (float)M_PI)/(M_PI * 2.0f) * 18);
        pitch_w = static_cast<int>((pitch + (float)M_PI/2.0f)/M_PI * 18);
        yaw_w = static_cast<int>((yaw + (float)M_PI)/(M_PI * 2.0f) * 18);
    }
    // onPose() is called whenever the Myo detects that the person wearing it has changed their pose, for example,
    // making a fist, or not making a fist anymore.
    void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
    {
        currentPose = pose;
        if (pose != myo::Pose::unknown) {// && pose != myo::Pose::rest) {
            // Tell the Myo to stay unlocked until told otherwise. We do that here so you can hold the poses without the
            // Myo becoming locked.
            myo->unlock(myo::Myo::unlockHold);
            // Notify the Myo that the pose has resulted in an action, in this case changing
            // the text on the screen. The Myo will vibrate.
            myo->notifyUserAction();
            
            /*
             Detect pose then send command to Arduino to turn servos
             Each command has 5 characters, from the left [0] = thumb, [1] = index, [2] = middle, etc
             r = rest, return finger to "fingers spread" position
             c = close, pull finger back to the palm
            */
            if(pose == myo::Pose::fingersSpread) {
                std::string command = "rrrrr"; //Open hand
                fprintf(arduino,"%s", command.c_str());
                fflush(arduino);
            } else if(pose == myo::Pose::fist) {
                std::string command = "ccccc"; //Close hand to form fist
                fprintf(arduino,"%s", command.c_str());
                fflush(arduino);
            } else if(pose == myo::Pose::waveOut) { //A basic fingers movement sequence
                std::string command = "rrcrr"; //Only middle finger down
                fprintf(arduino,"%s", command.c_str());
                fflush(arduino);
                sleep(1);
                command = "rcrrr"; //Only index finger down
                fprintf(arduino,"%s", command.c_str());
                fflush(arduino);
                sleep(1);
                command = "crrrr"; //Only thum down
                fprintf(arduino,"%s", command.c_str());
                fflush(arduino);
                sleep(1);
                command = "rrrrr"; //Back to "finger spread" rest position
                fprintf(arduino,"%s", command.c_str());
                fflush(arduino);
            } else if(pose == myo::Pose::waveIn) { //Form "W", "F", "U" in American Sign Language (ASL)
                std::string command = "crrrc"; //Thumb, pinky down
                fprintf(arduino,"%s", command.c_str());
                fflush(arduino);
                sleep(2);
                command = "ccrrr"; //Thumb, index down
                fprintf(arduino,"%s", command.c_str());
                fflush(arduino);
                sleep(2);
                command = "crrcc"; //Thum, ring, pinky down
                fprintf(arduino,"%s", command.c_str());
                fflush(arduino);
            } else if(pose == myo::Pose::doubleTap) { //Perform 1-9 in ASL continuously 
               std::string command = "crccc";
               fprintf(arduino,"%s", command.c_str());
               fflush(arduino);
               sleep(2);
               command = "crrcc";
               fprintf(arduino,"%s", command.c_str());
               fflush(arduino);
               sleep(2);
               command = "rrrcc";
               fprintf(arduino,"%s", command.c_str());
               fflush(arduino);
               sleep(2);
               command = "crrrr";
               fprintf(arduino,"%s", command.c_str());
               fflush(arduino);
               sleep(2);
               command = "rrrrr";
               fprintf(arduino,"%s", command.c_str());
               fflush(arduino);
               sleep(2);
               command = "crrrc";
               fprintf(arduino,"%s", command.c_str());
               fflush(arduino);
               sleep(2);
               command = "crrcr";
               fprintf(arduino,"%s", command.c_str());
               fflush(arduino);
               sleep(2);
               command = "crcrr";
               fprintf(arduino,"%s", command.c_str());
               fflush(arduino);
               sleep(2);
               command = "ccrrr";
               fprintf(arduino,"%s", command.c_str());
               fflush(arduino);
            }
        }
    }
    // onArmSync() is called whenever Myo has recognized a Sync Gesture after someone has put it on their
    // arm. This lets Myo know which arm it's on and which way it's facing.
    void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation,
                   myo::WarmupState warmupState) {
        onArm = true;
        whichArm = arm;
    }
    // onArmUnsync() is called whenever Myo has detected that it was moved from a stable position on a person's arm after
    // it recognized the arm. Typically this happens when someone takes Myo off of their arm, but it can also happen
    // when Myo is moved around on the arm.
    void onArmUnsync(myo::Myo* myo, uint64_t timestamp) {
        onArm = false;
    }
    // onUnlock() is called whenever Myo has become unlocked, and will start delivering pose events.
    void onUnlock(myo::Myo* myo, uint64_t timestamp) {
        isUnlocked = true;
    }
    // onLock() is called whenever Myo has become locked. No pose events will be sent until the Myo is unlocked again.
    void onLock(myo::Myo* myo, uint64_t timestamp) {
        isUnlocked = false;
    }
    // There are other virtual functions in DeviceListener that we could override here, like onAccelerometerData().
    // For this example, the functions overridden above are sufficient.
    // We define this function to print the current values that were updated by the on...() functions above.
    void print() {
        // Clear the current line
        std::cout << '\r';
        // Print out the orientation. Orientation data is always available, even if no arm is currently recognized.
        std::cout << '[' << std::string(roll_w, '*') << std::string(18 - roll_w, ' ') << ']'
        << '[' << std::string(pitch_w, '*') << std::string(18 - pitch_w, ' ') << ']'
        << '[' << std::string(yaw_w, '*') << std::string(18 - yaw_w, ' ') << ']';
        if (onArm) {
            // Print out the lock state, the currently recognized pose, and which arm Myo is being worn on.
            // Pose::toString() provides the human-readable name of a pose. We can also output a Pose directly to an
            // output stream (e.g. std::cout << currentPose;). In this case we want to get the pose name's length so
            // that we can fill the rest of the field with spaces below, so we obtain it as a string using toString().
            std::string poseString = currentPose.toString();
            std::cout << '[' << (isUnlocked ? "unlocked" : "locked  ") << ']'
            << '[' << (whichArm == myo::armLeft ? "L" : "R") << ']'
            << '[' << poseString << std::string(14 - poseString.size(), ' ') << ']';
        } else {
            // Print out a placeholder for the arm and pose when Myo doesn't currently know which arm it's on.
            std::cout << '[' << std::string(8, ' ') << ']' << "[?]" << '[' << std::string(14, ' ') << ']';
        }
        std::cout << std::flush;
    }
    // These values are set by onArmSync() and onArmUnsync() above.
    bool onArm;
    myo::Arm whichArm;
    // This is set by onUnlocked() and onLocked() above.
    bool isUnlocked;
    // These values are set by onOrientationData() and onPose() above.
    int roll_w, pitch_w, yaw_w;
    myo::Pose currentPose;
};

int main(int argc, char** argv) {  
    // We catch any exceptions that might occur below -- see the catch statement for more details.
    try {
        // First, we create a Hub with our application identifier. Be sure not to use the com.example namespace when
        // publishing your application. The Hub provides access to one or more Myos.
        myo::Hub hub("com.example.hello-myo");
        std::cout << "Attempting to find a Myo..." << std::endl;
        // Next, we attempt to find a Myo to use. If a Myo is already paired in Myo Connect, this will return that Myo
        // immediately.
        // waitForMyo() takes a timeout value in milliseconds. In this case we will try to find a Myo for 10 seconds, and
        // if that fails, the function will return a null pointer.
        myo::Myo* myo = hub.waitForMyo(10000);
        // If waitForMyo() returned a null pointer, we failed to find a Myo, so exit with an error message.
        if (!myo) {
            throw std::runtime_error("Unable to find a Myo!");
        }
        // We've found a Myo.
        std::cout << "Connected to a Myo armband!" << std::endl << std::endl;
        
        /*
         Establish serial communication connection to Arduino board
        */
        arduino = fopen("/dev/cu.usbmodem1411","w");
        if (arduino == NULL) {
            printf("not open\n");
            return -1 ;
        }
        else {
            printf("arduino opened\n");
        }
        
        // Next we construct an instance of our DeviceListener, so that we can register it with the Hub.
        DataCollector collector;
        // Hub::addListener() takes the address of any object whose class inherits from DeviceListener, and will cause
        // Hub::run() to send events to all registered device listeners.
        hub.addListener(&collector);
        // Finally we enter our main loop.
        while (1) {
            // In each iteration of our main loop, we run the Myo event loop for a set number of milliseconds.
            // In this case, we wish to update our display 20 times a second, so we run for 1000/20 milliseconds.
            hub.run(1000/20);
            // After processing events, we call the print() member function we defined above to print out the values we've
            // obtained from any events that have occurred.
            collector.print();
        }
        // If a standard exception occurred, we print out its message and exit.
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Press enter to continue.";
        std::cin.ignore();
        return 1;
    }
}
