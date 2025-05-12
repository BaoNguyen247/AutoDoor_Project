Hi!, my name is Nguyen Chi Bao
I've create some cool project, let dive in!

## 1. Hardware Design: Diagrams and Components

The **hardware design** includes various components orchestrated by a central microcontroller [1, 2].

A **Hardware Block Diagram**:
![ảnh](https://github.com/user-attachments/assets/6e131476-bfea-4b7a-8411-be4b831ac8a1)

In this project i use 2 Relay to replace Servo 

Key components used in the system include:
*   **STM32H743IIT6 Microcontroller:** 
*   **Ultrasonic Sensor US-100:** 
*   **PIR Sensor SR504/SR505:**
*   **4x4 Matrix Keypad:** 
*   **LCD 16x2:** 
*   **Relay:** 

I show you how to connect component
![ảnh](https://github.com/user-attachments/assets/a98c131a-028a-414d-8815-d7c2f9fb8350)

the usb-c capble is the pin UART A9,A10(note to putty via ucb-c capble) connect stm32 to laptop

![ảnh](https://github.com/user-attachments/assets/94719bfe-a041-46f4-b1e1-061f43414203)
![ảnh](https://github.com/user-attachments/assets/3f72357d-630e-4512-bef9-13a4d478a59b)
![ảnh](https://github.com/user-attachments/assets/d499decc-c43d-4d7c-85c2-629c2ca6a5fe)  
![ảnh](https://github.com/user-attachments/assets/cbbfc102-cfdf-4e98-adb7-f8568b095be9)
![ảnh](https://github.com/user-attachments/assets/7b51fe8c-21ec-47b5-af10-289c3d60eef7)
![ảnh](https://github.com/user-attachments/assets/7efdf336-7b5c-4af8-9bb0-6bb48b905ceb)




## 2. Software Design: Diagrams and Modules

The software is programmed in C on the STM32H743IIT6, using Visual Studio Code and CMake for development [41-45]. The programming approach utilizes **Hybrid Scheduling Systems**, combining a main loop for sequential tasks (Cooperative Scheduling) and interrupts for real-time events (Preemptive Scheduling) [46-50].

A **Software Design Diagram** is referenced [50], but its content is not included in the source text. However, **flowcharts** detailing the **Door Control (ActionDoor)** [51, 52], **Open Door (OpenDoor)** [52, 53], and **Close Door (CloseDoor)** [53] logic are provided later in the document.

The software is structured into several modules [54, 55]:
*   **GPIO Module:** Initializes and configures GPIO pins for peripherals [30, 54, 56-69].
*   **Keypad Module:** Manages the 4x4 keypad, detecting key presses via interrupts and scanning [54, 59-64].
*   **LCD Module:** Controls the LCD1602 display via I2C [30-38, 54].
*   **Sensor Module:** Manages the PIR sensor (status reading) and Ultrasonic sensor (distance measurement) using timers and interrupts [10, 38, 54, 70-81].
*   **UART Module:** Handles UART communication for receiving commands, displaying data, and managing passwords [10, 55, 82-91].
*   **Update Module:** Coordinates system status updates, fetching sensor data, handling physical keypad input, and triggering actions [55, 91-102].
*   **Program Manager Module:** The main module, responsible for system initialization and running the main update loop [55, 102-109].
*   **Action Module:** Implements the control logic for the light and the door based on sensor data and manual input [51-53, 81, 109-117].

## 3. Project Objectives: Features and Use Cases

The project aimed to design and implement a system with specific features and use cases [14].

### System Features

The implemented embedded system is designed to have the following features [8, 14, 53, 118-125]:
*   **Automatic Operation:** The door automatically opens upon detecting movement via a motion sensor and an infrared sensor (for presence detection) [14, 121]. The light in the room also turns on automatically [14, 121].
*   **One-Way Automatic Movement:** Automatic opening and closing occurs in one direction of travel, along with automatic light switching [8, 121].
*   **Manual Operation:** The door can be opened and closed manually, independent of sensors, in case sensors are inactive [8, 121].
*   **Automatic Operation Scenarios:**
    *   Door remains closed if no motion is detected [8, 122].
    *   Door automatically opens if motion is detected while closed [8, 122].
    *   Door remains open if motion continues to be detected [8, 122].
    *   Door automatically closes after a configurable time X seconds if no motion is detected while open [118, 122].
*   **Configuration via Keypad:** System configuration is performed using the keypad buttons [118].
    *   A "Turn Off" button disables automatic mode [118, 122].
    *   A "Turn On" button enables automatic mode [118, 123].
    *   Keys allow setting the automatic closing time (X seconds) [118]. **Note: The source indicates this feature was not completed in testing [123, 126].**
*   **Password Security:**
    *   Allows setting and changing a password to secure system configuration [118, 123].
    *   Requires the correct password to change configuration [119, 123].
    *   Locks the system after 5 incorrect password attempts [119, 123]. A RESET button is used to reset after lockout [119, 123].
*   **LCD Display:** The system has a 2-line LCD screen [119, 123].
    *   Line 1 displays "AUTO DOOR" when automatic mode is active and working, and "WELCOME" otherwise (manual mode) [119, 124].
    *   Line 2 displays "OPENING" when the door detects motion and opens automatically, and "CLOSING" when the door is automatically closing after X seconds without detected movement [119, 124].
*   **Sensor Range:** The motion sensor (likely PIR) can detect movement within 2m [120, 124].
*   **Relay Functionality:** Relays acting as motor switches operate correctly [120, 124].
*   **Project Management:** The project was completed within 1 week [120, 125] and under the 1.2 million VND budget [120, 125].
*   **Documentation:** A report covering hardware design, software design, system flowchart, etc., was produced [120, 125].

### Use Cases

The source details three main **use cases** for the system [127-130]:

1.  **Case 1: Door operates in automatic mode**
    *   **Trigger:** User wants the system to operate automatically, opening/closing on motion detection [127, 131].
    *   **Precondition:** System is powered correctly [127, 131].
    *   **Basic Flow:** If the sensor detects motion, the door automatically opens. After X seconds without detecting motion, the door automatically closes [127, 131].
    *   **Alternative Flows:**
        *   User can turn off automatic mode (leads to Use Case 2) [128, 131].
        *   User can adjust the waiting time X (leads to Use Case 3) [128, 131].
    *   **Status:** This use case is marked as **Completed** [131].

2.  **Case 2: User can turn automatic mode on/off**
    *   **Trigger:** User wants to disable or re-enable automatic door operation [128, 132].
    *   **Precondition:** System is powered correctly [128]. **Note: The source marks this precondition as Not Completed in the evaluation table [132].**
    *   **Basic Flow:** User presses the "Turn Off" button to disable automatic mode or "Turn On" to enable it. For security, the user needs to enter the password before toggling the mode [129]. **Note: The source explicitly states in the evaluation table that the requirement to enter a password to toggle automatic mode was Not Completed [132, 133].**
    *   **Alternative Flows:**
        *   System warns if password is incorrect [129, 133].
        *   System locks after 5 incorrect attempts [129, 133].
        *   Pressing the RESET button allows control after a 5-attempt lockout [129, 133].
    *   **Status:** This use case is marked as **Partially Completed** (specifically, the password requirement for toggling mode is incomplete) [132, 133].

3.  **Case 3: User can adjust the automatic closing time (X seconds)**
    *   **Trigger:** User wants to change the duration the door stays open when no motion is detected (X seconds) [130, 134].
    *   **Precondition:** System is powered correctly [130]. **Note: The source marks this precondition as Not Completed in the evaluation table [134].**
    *   **Basic Flow:** User enters the password to gain configuration rights, then performs specific operations (using buttons) to adjust time X [130]. **Note: The source marks this entire basic flow as Not Completed in the evaluation table [134].**
    *   **Alternative Flows:**
        *   System warns if password is incorrect [130, 134].
        *   System locks after 5 incorrect attempts [130, 134].
        *   Pressing the RESET button allows control after a 5-attempt lockout [130, 134].
    *   **Status:** This use case is marked as **Not Completed** [134].

## 4. Conclusion

The project successfully implemented a smart automatic door system meeting approximately **87% of the planned features** and completing the primary **automatic operation use case (Case 1)** [135-137]. Key achievements include basic automatic control based on motion, light control, LCD status display, and a password-based security system [136].

However, limitations include not implementing a sliding door mechanism [121, 138, 139], the inability to set the automatic closing time (X) via the keypad [123, 126, 139], and not integrating the password check for toggling automatic/manual mode [132, 133, 139]. These omissions affect the system's flexibility, intelligence, and security in certain operational modes [140, 141].

Future development could focus on implementing the missing configuration features, clarifying hardware details, and potentially simulating or testing the incomplete flows [142, 143].

If you want to se my demo video, go to this drive link: https://drive.google.com/drive/folders/1O-6jDjv1y6W5biuknN-qtYZUr4EQHqs4?usp=drive_link

Have a good day ^^ !


