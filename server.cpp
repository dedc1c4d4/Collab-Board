#include <SFML/Network.hpp>
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>

std::vector<std::unique_ptr<sf::TcpSocket>> clients;
std::mutex clientsMutex;

// Broadcast a packet to all clients except the sender
void broadcast(sf::Packet& packet, sf::TcpSocket* sender) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    for (const auto& client : clients) {
        if (client.get() != sender) {
            if (client->send(packet) != sf::Socket::Done) {
                std::cerr << "Failed to send packet to a client!" << std::endl;
            }
        }
    }
}

// Handle an individual client
void handleClient(sf::TcpSocket* client) {
    while (true) {
        sf::Packet packet;
        if (client->receive(packet) == sf::Socket::Done) {
            broadcast(packet, client);
        } else {
            // Disconnect and remove the client
            std::lock_guard<std::mutex> lock(clientsMutex);
            auto it = std::find_if(clients.begin(), clients.end(),
                [client](const std::unique_ptr<sf::TcpSocket>& c) { return c.get() == client; });
            if (it != clients.end()) {
                clients.erase(it);
            }
            std::cout << "Client disconnected." << std::endl;
            break;
        }
    }
}

int main() {
    sf::TcpListener listener;
    if (listener.listen(54000) != sf::Socket::Done) {
        std::cerr << "Failed to bind listener to port!" << std::endl;
        return -1;
    }

    std::cout << "Server is running on port 54000..." << std::endl;

    while (true) {
        auto client = std::make_unique<sf::TcpSocket>();
        if (listener.accept(*client) == sf::Socket::Done) {
            std::lock_guard<std::mutex> lock(clientsMutex);
            std::cout << "New client connected." << std::endl;
            clients.push_back(std::move(client));
            std::thread(handleClient, clients.back().get()).detach();
        }
    }

    return 0;
}
