# GSM_lib_based_on_STM32

## Authors:
- Denysova Iryna
- Korets Khrystyna
- Pavlosiuk Roman
- Hrynda Yuliana

## Project Structure
Project devided into two parts (branches): Lib and Screen:
* Denysova Iryna with Korets Khrystyna work on screen states and keyboard interuction
* Pavlosiuk Roman with Hrynda Yuliana work on building library

Main interface with screen states is in Test project, and in future it'll become a phone project. \
The GSM library is in GSMLib project, where you can get ```lib.a``` file and use this library in your projects.

## Prerequirements
In order to compile the existing part of our project, you need to install **STM32CUBEIDE** (it is better to do it for Windows). Here is the link to the site where you can find it [here](https://www.st.com/en/development-tools/stm32cubeide.html).
You will also need a STM32f3discovery board, bread board, screen, 8 Arduino wires and mini USB.

## Compilation
In order to compile our project 
1) Clone repository<br>
```bash
git clone https://github.com/UCUgllekk/GSM_lib_based_on_STM32.git
```
3) Then go to the required directory:<br>
```bash
cd TelephoneLib/Core/Src
```
4) Then run this code to compile project:<br>
```bash
g++ main.c -o main
```

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
