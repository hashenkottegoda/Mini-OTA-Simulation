// ecu_device/main.cpp

#include <iostream>
#include <chrono>
#include <thread>
#include <filesystem>
#include <fstream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>  // For getaddrinfo

#define PORT "8080"
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct addrinfo hints{}, *res;

    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP

    std::cout << "⏳ Waiting for server DNS to resolve...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));

    int status = getaddrinfo("ota_server", PORT, &hints, &res);
    if (status != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << "\n";
        return 1;
    }

    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock < 0) {
        perror("socket");
        freeaddrinfo(res);
        return 1;
    }

    // Retry loop for connecting to the server
    const int MAX_RETRIES = 5;
    const int RETRY_DELAY_SECONDS = 2;

    bool connected = false;
    for (int attempt = 1; attempt <= MAX_RETRIES; ++attempt) {
        if (connect(sock, res->ai_addr, res->ai_addrlen) == 0) {
            connected = true;
            break;
        }

        std::cerr << "❌ Connection attempt " << attempt 
                  << " failed. Retrying in " << RETRY_DELAY_SECONDS << "s...\n";
        std::this_thread::sleep_for(std::chrono::seconds(RETRY_DELAY_SECONDS));
    }

    if (!connected) {
        std::cerr << "❌ Could not connect to server after " << MAX_RETRIES << " attempts\n";
        freeaddrinfo(res);
        close(sock);
        return 1;
    }

    freeaddrinfo(res);

    // Create received directory if not exists
    std::filesystem::create_directories("./received");

    // 2. Receive firmware
    std::ofstream firmware("./received/firmware.txt", std::ios::binary);
    if (!firmware.is_open()) {
        std::cerr << "❌ Failed to open file for writing\n";
        close(sock);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    int bytesRead;

    while ((bytesRead = read(sock, buffer, BUFFER_SIZE)) > 0) {
        firmware.write(buffer, bytesRead);
    }
    firmware.close();

    std::cout << "✅ Firmware received and saved to firmware.txt\n";

    // 3. Send confirmation
    std::string ack = "OK";
    send(sock, ack.c_str(), ack.length(), 0);

    close(sock);
    return 0;
}
