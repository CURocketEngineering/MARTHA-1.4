#include <Arduino.h>
#include <queue>
#include <string>


void testCommand(std::queue<std::string> arguments, std::string& response) {
    cmdLine.println("Test command executed.");
    
    // Check if there are any arguments
    if (arguments.empty()) {
        cmdLine.println("No arguments provided.");
        response = "Test command executed. Arguments: None";
    } else {
        cmdLine.println("Arguments received:");
        response = "Test command executed. Arguments: ";
        
        // Process each argument
        while (!arguments.empty()) {
            std::string argument = arguments.front();
            arguments.pop();
            
            // Print each argument to the UART
            cmdLine.println(" - " + argument);
            
            // Append the argument to the response
            response += argument + " ";
        }
    }
}


void ping(std::queue<std::string> arguments, std::string& response) {
    cmdLine.println("Pinged the microntroller ");
}

void clearPostLaunchMode(std::queue<std::string> arguments, std::string& response) {
    dataSaver.clearPostLaunchMode();
    launchDetector.reset(); // fibo
    cmdLine.println("Cleared post launch mode, reboot the device to complete the reset.");
}

std::string floatToString(float value, int precision = 2) {
    char buffer[20];
    dtostrf(value, 0, precision, buffer);
    return std::string(buffer);
}

void dumpFlash(std::queue<std::string> arguments, std::string& response) {
    // check for -a in arg
    if (arguments.empty()) {
        dataSaver.dumpData(Serial, false);
        return;
    } else if (arguments.front() == "-a") {
        arguments.pop();
        dataSaver.dumpData(Serial, true);
        return;
    } else {
      cmdLine.println("Invalid argument. Use -a to ignore empty pages.");
    }
}

void printStatus(std::queue<std::string> arguments, std::string& response) {
    cmdLine.println("--Launch Detector--");
    cmdLine.print("Launched: ");
    cmdLine.println(std::to_string(launchDetector.isLaunched())); // fibo
    cmdLine.print("Launched Time: ");
    cmdLine.println(floatToString(launchDetector.getLaunchedTime())); // fibo
    cmdLine.print("Median Acceleration Squared: ");
    cmdLine.println(floatToString(launchDetector.getMedianAccelerationSquared())); // fibo

    cmdLine.println("");
    cmdLine.println("--Apogee Detector--");
    cmdLine.print("Apogee Detected: ");
    cmdLine.println(std::to_string(apogeeDetector.isApogeeDetected()));
    cmdLine.print("Estimated Altitude: ");
    cmdLine.println(floatToString(verticalVelocityEstimator.getEstimatedAltitude()));
    cmdLine.print("Estimated Velocity: ");
    cmdLine.println(floatToString(verticalVelocityEstimator.getEstimatedVelocity()));
    cmdLine.print("Inertial Vertical Acceleration: ");
    cmdLine.println(floatToString(verticalVelocityEstimator.getInertialVerticalAcceleration()));
    cmdLine.print("Vertical Axis: ");
    cmdLine.println(std::to_string(verticalVelocityEstimator.getVerticalAxis()));
    cmdLine.print("Vertical Direction: ");
    cmdLine.println(std::to_string(verticalVelocityEstimator.getVerticalDirection()));
    cmdLine.print("Apogee Altitude: ");
    cmdLine.println(floatToString(apogeeDetector.getApogee().data));

    cmdLine.println("");
    cmdLine.println("--Data Saver--");
    cmdLine.print("Post Launch Mode: ");
    cmdLine.println(std::to_string(dataSaver.quickGetPostLaunchMode()));
    cmdLine.print("Rebooted in Post Launch Mode (won't save): ");
    cmdLine.println(std::to_string(dataSaver.getRebootedInPostLaunchMode()));
    cmdLine.print("Last Timestamp: ");
    cmdLine.println(std::to_string(dataSaver.getLastTimestamp()));
    cmdLine.print("Last Data Point Value: ");
    cmdLine.println(floatToString(dataSaver.getLastDataPoint().data));
    cmdLine.print("Super loop average hz: ");
    cmdLine.println(floatToString(loop_count / (millis() / 1000 - start_time_s)));

    cmdLine.println("");
    cmdLine.println("--Flash--");
    cmdLine.print("Stopped writing b/c wrapped around to launch address: ");
    cmdLine.println(std::to_string(dataSaver.getIsChipFullDueToPostLaunchProtection()));
    cmdLine.print("Launch Write Address: ");
    cmdLine.println(std::to_string(dataSaver.getLaunchWriteAddress()));
    cmdLine.print("Next Write Address: ");
    cmdLine.println(std::to_string(dataSaver.getNextWriteAddress()));
    cmdLine.print("Buffer Index: ");
    cmdLine.println(std::to_string(dataSaver.getBufferIndex()));
    cmdLine.print("Buffer Flushes: ");
    cmdLine.println(std::to_string(dataSaver.getBufferFlushes()));

    cmdLine.println("");
    cmdLine.println("--Sensors--");
    cmdLine.print("Accelerometer X: ");
    cmdLine.println(floatToString(xAclData.getLastDataPointSaved().data));
    cmdLine.print("Accelerometer Y: ");
    cmdLine.println(floatToString(yAclData.getLastDataPointSaved().data));
    cmdLine.print("Accelerometer Z: ");
    cmdLine.println(floatToString(zAclData.getLastDataPointSaved().data));
    cmdLine.print("Gyroscope X: ");
    cmdLine.println(floatToString(xGyroData.getLastDataPointSaved().data));
    cmdLine.print("Gyroscope Y: ");
    cmdLine.println(floatToString(yGyroData.getLastDataPointSaved().data));
    cmdLine.print("Gyroscope Z: ");
    cmdLine.println(floatToString(zGyroData.getLastDataPointSaved().data));
    cmdLine.print("Temperature: ");
    cmdLine.println(floatToString(tempData.getLastDataPointSaved().data));
    cmdLine.print("Pressure: ");
    cmdLine.println(floatToString(pressureData.getLastDataPointSaved().data));
    cmdLine.print("Altitude: ");
    cmdLine.println(floatToString(altitudeData.getLastDataPointSaved().data));
    cmdLine.print("Magnetometer X: ");
    cmdLine.println(floatToString(xMagData.getLastDataPointSaved().data));
    cmdLine.print("Magnetometer Y: ");
    cmdLine.println(floatToString(yMagData.getLastDataPointSaved().data));
    cmdLine.print("Magnetometer Z: ");
    cmdLine.println(floatToString(zMagData.getLastDataPointSaved().data));
}

