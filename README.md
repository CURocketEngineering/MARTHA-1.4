# MARTHA 1.4

## What is the MARTHA repo for? 

MARTHA (Miniaturized Avionics for Rapid Testing Handling and Assessment) is our
all-in-one flight computer for data collection and flight stage prediction. 

This repo hosts all the software used exclusively by MARTHA. Any code that is also used by other hardware systems are found within the submodules of this repo such as [Avionics](https://github.com/CURocketEngineering/Avionics). We use the submodules to reduce code duplication as many of our systems rely on the same basic ideas (state machines, data handling, etc.).

## Operations
- Initialize each sensor driver
- Initialize data handlers
- Initialize flight status 
- Request data from each sensor driver 
- Pass sensor data into the data handlers
- Pass the data handlers into the flight status updater

## Workspace Setup
1. Download VS code
2. Clone the repo
```bash
git clone https://github.com/CURocketEngineering/MARTHA-1.4.git
```
3. Setup the submodules 
```bash
git submodule init
git submodule update 
```
4. Install the PlatformIO VScode extension: `platformio.platformio-ide`
5. Install the [Cube Programmer](https://www.st.com/en/development-tools/stm32cubeprog.html#get-software) and run it to get all the stm32 drivers you'll need. 

### Unity Testing Setup
1. Run the test within the pio terminal with `pio test -e martha_stm --without-testing` this will flash the test script onto the board.
2. Open the serial monitor and see the results of the test. (Make sure your test waits for serial to start before running)

## Firmware Update
1. Connect ST-Link: [helpful guide](https://stm32-base.org/guides/connecting-your-debugger.html)
2. Connect MARTHA's USB to your laptop for serial messages
3. Uncomment the lines of code at the beginning of the `setup` method which are needed to setup the debug serial messages
4. Press the PlatformIO "Upload" button at the bottom (it looks like: "-->")
5. Press the PlatformIO "Serial Monitor" to initiate serial communication (it looks like a plug)  
    - Leaving the port selection on "auto" tends to work unless MARTHA [isn't booting](#not-booting-issues)
    - If you skipped step 3 where you uncomment the waiting for serial lines, then you don't have to start the serial monitor for execution to begin, but you will miss the first few messages. 
6. Once everything looks good, undo step 3, so that the software can start without you. 
7. Try some of these [tests](#tests-that-must-be-ran-before-using-this-software-for-spaceport) to make sure it's good to go. 

## Not booting issues
Below are all the know reasons why MARTHA may not boot and show up as a serial device after uploading the code   
1. Hardware switches are in the wrong states   
MARTHA has a boot mode switch onboard, trying playing with it. When it's on the wrong state, you won't be able to run the debugger and serial will not start. 

2. Software error during compile time   
Look at all the code that runs prior to `setup` especially the stuff you may have just added. Any errors there will prevent serial and everything else from starting. Use the debugger to get an idea of where the it fails and comment stuff out to isolate it.

3. Code uploaded from Windows (Intermittent)  
When the software freezes after starting to setup the first sensor, try uploading from a different computer or different OS on the same computer. This issue has only been seen on one laptop and the root cause has not yet been identified.  

## How to contribute

We follow the basic [GitHub Flow](https://docs.github.com/en/get-started/using-github/github-flow#following-github-flow).

- The main branch is protected, so you must create a pull request and have it reviewed by at least one other contributor 
  
## Tests that must be ran before using this software for Spaceport
*If the software is changed in any meaningful way, then the tests below must be redone*
- [ ] **Collection and Boot up Test**  
This test ensures that MARTHA actually starts when you plug it in and will collect data. Sometimes MARTHA is waiting for serial to start with a laptop, this ensures that has been properly disabled. By doing this multiple times, you ensure that it boots reliable. 
  1. Clear all the logs
  2. Plug in MARTHA
  3. Wait 1 minutes
  4. Remove the SD card
  5. Ensure that there is 1 minute of log data on the SD card
  6. Repeat steps 1-5, 3+ times 

- [ ] **Battery Drain Test**  
This test ensures that the battery doesn't drain too fast. At Spaceport, the flight computer has to sit while on for multiple hours, and we don't want it to die before launch. 
    1. Plug MARTHA in to the same kind of battery which will be used at Spaceport
    2. Note the time when you plugged it in
    3. Let it run until it dies (or 5 hours passed)
    4. Check the log's oldest timestamp to see if it makes sense and that logging last long enough
