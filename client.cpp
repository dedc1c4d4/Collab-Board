#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Connect to the server
sf::TcpSocket socket;

// Handle receiving updates from the server
void receiveUpdates(sf::Image& canvas, sf::Texture& texture) {
    while (true) {
        sf::Packet packet;
        if (socket.receive(packet) == sf::Socket::Done) {
            // Extract drawing 
            int x1, y1, x2, y2;
            if (packet >> x1 >> y1 >> x2 >> y2) {
                sf::RenderTexture renderTexture;
                renderTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
                renderTexture.clear(sf::Color::White);
                renderTexture.draw(sf::Sprite(texture));

                sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(x1, y1), sf::Color::Black),
                    sf::Vertex(sf::Vector2f(x2, y2), sf::Color::Black)
                };
                renderTexture.draw(line, 2, sf::Lines);

                renderTexture.display();
                canvas = renderTexture.getTexture().copyToImage();
                texture.loadFromImage(canvas);
            }
        }
    }
}

int main() {
    // Connect to the server
    if (socket.connect("127.0.0.1", 54000) != sf::Socket::Done) {
        std::cerr << "Failed to connect to server!" << std::endl;
        return -1;
    }
    std::cout << "Connected to server!" << std::endl;

    sf::Image canvas;
    canvas.create(WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color::White);

    sf::Texture texture;
    texture.loadFromImage(canvas);

    sf::Sprite sprite(texture);

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Collaborative Whiteboard");
    window.setFramerateLimit(60);

    bool isDrawing = false;
    sf::Vector2i lastMousePosition;

    // Start a thread to receive updates from the server
    std::thread(receiveUpdates, std::ref(canvas), std::ref(texture)).detach();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                isDrawing = true;
                lastMousePosition = sf::Mouse::getPosition(window);
            }

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                isDrawing = false;
            }
        }

        if (isDrawing) {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

            // Draw locally
            sf::RenderTexture renderTexture;
            renderTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
            renderTexture.clear(sf::Color::White);
            renderTexture.draw(sprite);

            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(lastMousePosition.x, lastMousePosition.y), sf::Color::Black),
                sf::Vertex(sf::Vector2f(mousePosition.x, mousePosition.y), sf::Color::Black)
            };
            renderTexture.draw(line, 2, sf::Lines);

            renderTexture.display();
            canvas = renderTexture.getTexture().copyToImage();
            texture.loadFromImage(canvas);

            // Send drawing data to the server
            sf::Packet packet;
            packet << lastMousePosition.x << lastMousePosition.y << mousePosition.x << mousePosition.y;
            if (socket.send(packet) != sf::Socket::Done) {
                std::cerr << "Failed to send drawing data!" << std::endl;
            }

            lastMousePosition = mousePosition;
        }

        // Render the canvas
        window.clear(sf::Color::White);
        window.draw(sprite);
        window.display();
    }

    return 0;
}
