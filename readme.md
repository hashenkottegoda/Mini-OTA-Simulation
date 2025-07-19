# 🚗 Mini OTA Simulation (C++ , Python , Docker)

This project demonstrates a simple **Over-the-Air (OTA)** update system using **C++**, **Python**, and **Docker**. It simulates how a server can send firmware updates to an ECU (Electronic Control Unit) in a connected vehicle environment.

---

## 🧰 Tech Stack

- **Python** – OTA Update Server
- **C++** – Simulated ECU receiving updates
- **Docker & Docker Compose** – Containerization and network setup

---

## 🔧 How It Works

1. The **Python OTA server** sends a firmware file (`firmware_v2.txt`) over a TCP socket.
2. The **C++ ECU** receives the file and saves it as `firmware.txt`.
3. The ECU sends an acknowledgment (`OK`) back to the server.
4. Both services run in Docker containers and communicate over a virtual network.

---

## ▶️ Run the Simulation

```bash
# Clone the project
git clone https://github.com/yourusername/mini-ota.git
cd mini-ota

# Build and run everything
docker-compose up --build
```

---

## ✅ Output

- OTA Server logs:

  ```
  📡 OTA Server is waiting for ECU...
  ✅ ECU connected
  ✅ ECU says: OK
  ```

- ECU logs:

  ```
  ✅ Firmware received and saved to firmware.txt
  ```

- A new file `firmware.txt` is created in the `received` directory that contains the OTA firmware update (firmware_v2.txt).
