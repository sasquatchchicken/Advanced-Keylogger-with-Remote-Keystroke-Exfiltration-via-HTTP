# Advanced-Keylogger-with-Remote-Keystroke-Exfiltration-via-HTTP
# Overview

This project demonstrates an advanced keylogger capable of capturing keystrokes on a Windows machine and sending the data to a remote server for monitoring. The keylogger, written in C++ as a DLL, is loaded into the target system's memory via a PowerShell script. The captured keystrokes are sent over HTTP to a Python server hosted on the attacker’s machine for real-time logging.
Additionally, a **BADUSB** payload is used to automate the execution of the PowerShell script on the target machine. The Ducky script ensures the entire process runs automatically once the flipperZero is plugged in, leveraging badusb downloading the .dll from the remote server and loading it into memory to capture keystrokes.

# Components
**C++ Keylogger (DLL):** Captures keystrokes and sends them over HTTP to the host machine.

**PowerShell Script:** Downloads and loads the DLL into memory on the target machine.

**Python HTTP Server:** Receives the keystrokes and logs them to the console.

**Ducky Script:** Automates the process of executing the PowerShell script and delivering the Keylogger on the target machine.

# Files
**dll_keylogger_hooker.cpp:** C++ file that implements the keylogger.

**mydll.dll:** The compiled DLL keylogger that logs keystrokes and sends them to the host machine.

**DLL_memory_inject.ps1:** PowerShell script that loads the DLL into memory on the target machine.

**hosting_DLL&http_keystrokeServer.py:** Python script that runs an HTTP server to receive and log the keystrokes sent from the keylogger.

**badusb_DLL_injection.txt:** The Rubber Ducky payload that automates the execution of the PowerShell script on the target.

C++ Keylogger (DLL)
The keylogger is written in C++ as a DLL that installs a keyboard hook to capture keystrokes. Instead of writing the captured keystrokes to a file, the keystrokes are sent to a remote server over HTTP, allowing real-time logging of keystrokes on the host machine.

PowerShell Script
The PowerShell script is executed on the target machine and is responsible for downloading the compiled DLL from the host machine and loading it into memory, allowing the keylogger to run without writing anything to disk.

Python HTTP Server
The Python server listens for incoming POST requests containing the keystrokes sent from the DLL on the target machine. It logs all keystrokes received from the keylogger.

Ducky Script
The Ducky script is used to automate the process of executing the PowerShell script on the target machine. It opens PowerShell on the target system and executes it to download and load the keylogger DLL.

# Deployment
# Steps:
Start the Python server on the host machine by **hosting_DLL&http_keystrokeServer.py**.

Load the **badusb_DLL_injection.txt** onto the flipperZero BADUSB device.

Plug in the **flipperZero** into the target machine. The **BADUSB** script will:
***Open PowerShell.
Execute the PowerShell script to download and run the DLL in memory.***

Monitor the Python server to receive keystrokes sent from the target machine.

***Developer command prompt was used to compile the C++ keylogger to a .dll*** 

cl.exe /LD /Fe:mydll.dll <your_filename_here>.cpp User32.lib Wininet.lib

# Key Features:
**Memory injection:** The DLL is injected into memory using a PowerShell script, avoiding writing the keylogger to disk.

**Keystroke exfiltration:** Keystrokes are sent over HTTP to the attacker's server.

**Automated delivery:** A flipperZero BADUSB device is used to automate the entire process.

# ⚠️ DISCLAIMER

This project is for educational and ethical security research purposes only. The goal is to raise awareness about potential threats and encourage the development of better security measures. Do not use this project for unauthorized or malicious activities.
