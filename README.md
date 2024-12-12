# GSM_lib_based_on_STM32

## Authors:
- Denysova Iryna
- Pavlosiuk Roman
- Hrynda Yuliana
- Korets Khrystyna

## Project Structure
Project is devided into 2 CubeIDE projects: GSMLib, Test(phone) <br>
Team work devided into two branches: Lib and Screen
* In Lib branch work Roman Pavlosiuk with Yuliana Hrynda building main library to work with GSM module.
* In Screen branch work Iryna Denysova with Khrystyna Korets developing screen states and connecting keyboard to STM32.

P.S GSMLib is currently outdated due to dependance project to other files which is created only in executable project in CubeIDE, but not in static library

## Prerequirements
In order to compile the existing part of our project, you need to install **STM32CUBEIDE** (work better with Ubuntu 22 or Windows). Here is the link to the site where you can find it [here](https://www.st.com/en/development-tools/stm32cubeide.html).
You will also need a STM32f3discovery board, bread board, screen, speakers, GSM A9G module, Arduino wires, micro-USB, mini-USB.

## Linking the Library with Your Project

Follow these steps to integrate the GSMLibrary with your project:

1. Build the Library
   - Open the GSMLibrary project in STM32CubeIDE and build it.
2. Set Up the Project Directory
   - Navigate to your project folder → Core.
   - Create a directory (e.g., lib or GSMLib).
3. Add Library Files
   - Copy libGSMLibrary.a from GSMLibrary/Core/Debug into the newly created directory.
   - Copy GSMLibrary.hpp from GSMLibrary/Core/Inc into the same directory.
4. Update Project Properties
   - Right-click on your project folder → Properties.
   - Navigate to C/C++ General → Paths and Symbols → Includes → GNU C++.
       - Click Add, choose Workspace, select the created directory, and click OK.
   - Repeat the process in the Library Paths tab.
5. Configure Linker Settings

   - Open C/C++ Build → Settings → Tool Settings.
   - Scroll to MCU/MPU G++ Linker → Miscellaneous.
   - In the Additional Object Files field, click Add and include libGSMLibrary.a.
6. Include the Library Header

   - At the top of your source files, include the library header:
   ```{c++}
   #include "GSMModule.hpp"
   ```
## Connection
  * In order to connect STM32F3Discovery with GSM module, LCD5110 screen and keyboard use following scheme:
![image](https://github.com/user-attachments/assets/f2b1c2f9-22e0-436e-ade6-2ddabe87ad29)

  * After successful connection and building project the next image should appear on your screen:
![image](https://github.com/user-attachments/assets/8f011a59-f5bc-4fee-951f-3d0ecf5a72b7)

## Navigation
  * To navigate through GUI use following scheme:
![image](https://github.com/user-attachments/assets/661d9116-1368-4cb0-8240-6c169e90e16f)

## Usage
To run our project you need to follow these steps:

1. Choose the stm32f3discovery board in the IDE as the working one.
2. Copy all our existing project files and paste them into the IDE. 
3. Connect STM with bread board, using the wires and the blueprint below.
![Screenshot from 2024-10-09 14-08-09](https://github.com/user-attachments/assets/731d29bb-2b3a-425a-b890-6cb4620704bd)
4. Connect the screen to the bread board and STM using mini USB to connect to the computer.
![photo_2024-10-09_14-34-56](https://github.com/user-attachments/assets/51a1e1ae-9507-4fc8-bc4c-5570f82f73dd)
5. To run
```bash
./main
```
