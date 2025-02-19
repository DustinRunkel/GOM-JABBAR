# USB Monitor GUI for Raspberry Pi 2040

This application provides a graphical interface to monitor the connection status of a Raspberry Pi 2040, send test commands to it, and receive responses.

## **Installation Instructions**

### **Windows Installation**
1. **Install Python** (if not installed):  
   Download and install Python from [python.org](https://www.python.org/downloads/).

2. **Install Dependencies**:

   We have to install these globally as running in a virtual environment will not work for windows

   ```sh
   pip install pyusb pyqt6 --break-system-packages
   ```
3. **Run application**:
   ```sh
   cd GUI
   python3 gui.py
   ```
   
### **Linux Installation**
1. **Install Python** (if not installed):  
   Download and install Python from [python.org](https://www.python.org/downloads/).
   ```sh
   sudo apt update && sudo apt install -y python3 python3-pip usbutils libusb-1.0-0-dev qt6-base-dev
   ```

3. **Create Virtual Environment**:
   ```sh
   python3 -m /GUI/venv venv
   ```

3. **Activate Venv**:
   ```sh
   cd GUI
   source venv/bin/activate
   ```

4. **Install Dependencies**:
   ```sh
   pip install pyusb pyqt6
   ```

5. **Run program**:
   ```sh
   python gui.py
   ```


## **Running the Application on WSL (Windows Subsystem for Linux)**
Since WSL does not have direct USB support, we need to attach the Raspberry Pi 2040 using `usbipd`.

### **1. Install and Enable USB Support in Windows**
Run the following in **PowerShell (Admin)**:
   ```sh
   wsl --shutdown
   winget install dorssel.usbipd-win
   ```
Restart WSL after installation.

### **2. Find the Raspberry Pi Pico on Windows**
   ```sh
   usbipd wsl list
   ```
   Identify the **bus ID** of the Raspberry Pi Pico (2040).

### **3. Bind the Device**
   ```sh
   usbipd wsl bind --busid <BUS-ID>
   ```
   Replace `<BUS-ID>` with the actual ID from the previous step.

### **4. Attach the Device in WSL**
   ```sh
   usbipd wsl attach --busid <BUS-ID>
   ```

### **5. Verify USB Connection in WSL**
   ```sh
   lsusb
   ```
   You should see an entry for the **Raspberry Pi Pico (2040)**.

### **6. Run the Application**
   ```sh
   python3 usb_monitor.py
   ```

## **Troubleshooting**
- If the device is not detected, ensure it's in **USB mode** (e.g., holding down the BOOTSEL button while plugging in the USB cable).
- Run `usbipd wsl list` again to check if it's bound.
- If using WSL, make sure to attach the device each time WSL restarts.

---
Now your GUI should run successfully on **Windows and WSL Linux**! 🎉





