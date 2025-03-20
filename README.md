![image](https://github.com/user-attachments/assets/8c0c86e5-34e3-45ea-83dc-a9a94a2f9300)


# Network Attached Storage (NAS) 📡💾

Welcome to **Network Attached Storage (NAS)**! This project transforms your ESP32 into a smart file storage device by syncing files from a cloud-hosted Node.js server to an SD card over WiFi. Upload files via a web interface, and let your ESP32 automatically download them—ideal for IoT projects, remote backups, or a lightweight NAS solution!


---

## 🚀 Project Overview

**Network Attached Storage (NAS)** integrates a Node.js server and ESP32 firmware to create a simple, cloud-connected storage system:
- **Node.js Server**: Hosted on Render, it manages file uploads and serves them to the ESP32.
- **ESP32 Firmware**: Polls the server every 30 seconds to download new files to an SD card.

---

## 📋 Features
- 🌐 **Cloud-Hosted**: Deployed on Render for global access.
- 📤 **Web Upload**: Upload files through a browser interface.
- 📥 **Automatic Sync**: ESP32 fetches new files periodically.
- 💾 **SD Card Storage**: Stores files locally on the ESP32.
- 🔧 **Customizable**: Adjust WiFi, server URL, and polling interval.

---

## 🛠️ How It Works

1. **Upload Files**  
   Visit the Render-hosted server (e.g., `https://nas-server.onrender.com`) and upload files via the web form.

2. **ESP32 Syncs**  
   The ESP32 checks the server’s `/files` endpoint every 30 seconds, compares the list with its SD card contents, and downloads any new files.

3. **Local Storage**  
   Files are saved to the ESP32’s SD card for local access.

![Workflow](https://via.placeholder.com/400x200.png?text=Upload+to+Sync+Workflow)  
*Workflow: Upload → Server → ESP32 → SD Card (replace with your own image).*

---

## 📦 Prerequisites

### Hardware
- ESP32 Dev Module
- MicroSD Card + Adapter (FAT32 formatted)
- USB Cable for programming

### Software
- [Arduino IDE](https://www.arduino.cc/en/software)
- [Node.js](https://nodejs.org) (for local testing)
- Git
- Render account ([render.com](https://render.com))
- GitHub account

---

## ⚙️ Setup Instructions

### 1. Deploy the Node.js Server to Render
1. **Clone or Create Repository**
   - Clone this repo or set up a new one:



git clone https://github.com/yourusername/network-attached-storage.git
cd network-attached-storage




2. **Server Files**
- Add `server.js` and `package.json` from this repository to your project folder.

3. **Push to GitHub**
- Commit and push:



git add .
git commit -m "Add NAS server"
git push origin main





4. **Deploy on Render**
- Sign in to [render.com](https://render.com) and click **New > Web Service**.
- Connect your GitHub repo (`network-attached-storage`).
- Configure:
- Name: `nas-server`
- Environment: Node.js
- Branch: `main`
- Build Command: `npm install`
- Start Command: `npm start`
- Instance Type: Free
- Deploy and note the URL (e.g., `https://nas-server.onrender.com`).

5. **Test the Server**
- Visit your Render URL in a browser and upload a test file.

---

### 2. Set Up the ESP32
1. **Hardware Connections**
- Connect the SD card to your ESP32:
- CS: Pin 5
- MOSI: Pin 23
- MISO: Pin 19
- SCK: Pin 18
- VCC: 3.3V or 5V (check your module)
- GND: Ground

2. **ESP32 Firmware**
- Open `server.ino` from this repository in Arduino IDE.
- Update the following in `server.ino`:
- `ssid`: Your WiFi network name
- `password`: Your WiFi password
- `serverBaseUrl`: Your Render URL (e.g., `https://nas-server.onrender.com`)
- Upload to your ESP32:
- Select your ESP32 board and COM port.
- Upload (Ctrl+U).

3. **Monitor Output**
- Open Serial Monitor (115200 baud) to verify:





---

## 🧪 Testing
1. **Upload a File**
- Go to your Render URL (e.g., `https://nas-server.onrender.com`).
- Upload a file (e.g., `test.txt`).

2. **Check ESP32**
- Within 30 seconds, the ESP32 should download the file to its SD card.
- Verify via Serial Monitor or by checking the SD card.

---

## 🔍 Troubleshooting
- **WiFi Issues**
- "Failed to connect": Ensure `ssid` and `password` are correct and use a 2.4GHz network.
- **Server Errors**
- Check Render logs for issues (e.g., port binding).
- Test the `/files` endpoint in a browser (e.g., `https://nas-server.onrender.com/files`).
- **HTTPS Problems**
- If downloads fail with HTTP code -1, consider adding SSL support or using an insecure client (see code comments).

---

## 🌟 Contributing
Feel free to fork this repo, submit pull requests, or open issues with suggestions! Contributions are welcome to enhance features like persistent storage or authentication.

---

## 📸 Screenshots
*Add your own screenshots here!*  
- Web interface: `https://nas-server.onrender.com`  
- Serial Monitor output showing a file download.

---

## 📜 License
This project is licensed under the MIT License—see the [LICENSE](LICENSE) file for details.

---

*Built with ❤️ by [yourusername] | Powered by ESP32 & Render*
