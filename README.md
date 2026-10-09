# WMATABoard

ESP32 Metro arrival display using C++ and the Arduino framework. Setup instructions below are for Windows.

## 1. Clone and open the project

In a terminal, run:

```bash
git clone git@github.com:sabakuro/WMATABoard.git
cd WMATABoard
```

If you haven't configured GitHub SSH access, use `git clone https://github.com/sabakuro/WMATABoard.git` instead. Keep the project on Windows for this USB setup; WSL requires separate USB forwarding.

## 2. Install VS Code

Install [VS Code](https://code.visualstudio.com/).

## 3. Install the PlatformIO extension

In VS Code, open **Extensions** (`Ctrl+Shift+X`), search for **PlatformIO IDE**, and install it. Reload VS Code if prompted.

## 4. Open the project via PlatformIO

Click the **PlatformIO** icon in the sidebar → **PIO Home → Open**. In PlatformIO Home, choose **Open Project** and select the cloned `WMATABoard` folder containing `platformio.ini`. Wait for the toolchain installation to finish.

## 5. Connect USB and install the driver

Connect the ESP32 using a **USB data cable**, then open Windows **Device Manager**.

If **CP2102 USB to UART Bridge Controller** appears under **Other devices** with a warning icon:

1. Download **CP210x Universal Windows Driver** from [Silicon Labs](https://www.silabs.com/software-and-tools/usb-to-uart-bridge-vcp-drivers) and extract the ZIP.
2. Right-click the CP2102 device → **Update driver → Browse my computer for drivers**.
3. Select the extracted folder, check **Include subfolders**, and click **Next**.

Confirm the device appears under **Ports (COM & LPT)** with a COM number. If nothing appears when connecting the board, try another data cable or USB port.

## 6. Upload the program

Click PlatformIO's **Upload** arrow in the bottom toolbar. Wait for **[SUCCESS]**.

If no port is found, add `upload_port = COM5` under the existing environment in `platformio.ini`, replacing `COM5` with your port. If upload stalls at **Connecting…**, hold **BOOT** and release it when flashing starts.

## 7. Monitor

Set `monitor_speed = 115200` under the existing environment in `platformio.ini`, matching `Serial.begin(115200)` in the code.

Press `Ctrl+Shift+P` → **PlatformIO: Monitor**, then press the board's **EN / RESET** button to see startup messages. The initial test prints:

```text
Hello World!
ESP32 is running!
ESP32 is running!
```
