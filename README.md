# Colab-Board 🎨

**Colab-Board** is a real-time collaborative whiteboard application built using **C++** and **SFML (Simple and Fast Multimedia Library)**. It enables multiple users to connect and draw together on a shared canvas, with seamless synchronization across all connected clients using a client-server architecture.

## Features

- **Real-Time Collaboration**: Multiple users can draw simultaneously, and all updates are reflected in real time.
- **Undo/Redo Functionality**: Allows users to undo or redo their drawing actions locally.
- **Clear Canvas**: Users can clear the canvas, and the change is synchronized across all clients.
- **Scalable Networking**: Supports multiple clients with efficient packet broadcasting.
- **Cross-Platform Compatibility**: Works on Linux, Windows, and macOS systems.

## Architecture

1. **Server**:
    - Manages client connections.
    - Broadcasts drawing updates to all connected clients.
    - Ensures real-time synchronization across sessions.
2. **Client**:
    - Captures user interactions such as drawing, undo/redo, and clearing the canvas.
    - Communicates with the server for updates.
    - Renders the collaborative canvas in real time.

## Prerequisites

- **C++ Compiler** (with C++11 or later support)
- **SFML Library** (2.5 or later)
    - Install via a package manager or download from [SFML's official website](https://www.sfml-dev.org/).

## Installation and Usage

1. Clone the repository:
    
    ```bash
    git clone https://github.com/your-username/colab-board.git
    cd colab-board
    
    ```
    
2. Install SFML (Linux example):
    
    ```bash
    sudo apt update && sudo apt install libsfml-dev
    
    ```
    
3. Build the **server**:
    
    ```bash
    g++ server.cpp -o server -lsfml-network -lsfml-system -pthread
    
    ```
    
4. Build the **client**:
    
    ```bash
    g++ client.cpp -o client -lsfml-graphics -lsfml-window -lsfml-network -lsfml-system
    
    ```
    
5. Start the server:
    
    ```bash
    ./server
    
    ```
    
6. Launch multiple clients:
    
    ```bash
    ./client
    
    ```
    
7. In the client interface, enter the server's IP address and port to connect.

## How to Use

1. Start the server on a central machine.
2. Connect clients to the server by entering the server's IP and port.
3. Use the left mouse button to draw, and watch the canvas update in real time across all connected clients.
4. Keyboard shortcuts:
    - **Ctrl+Z**: Undo
    - **Ctrl+Y**: Redo
    - **C**: Clear canvas

## Future Enhancements

- **Toolbox**: Add support for drawing shapes, colors, and text.
- **Persistence**: Save and load canvas data for later use.
- **Authentication**: Enable user login for secure collaboration.
- **Scalability**: Switch to WebSocket-based communication for larger user bases.

---

Contributions are welcome! Fork the repository, make improvements, and submit a pull request. Let's make **Colab-Board** the ultimate collaborative drawing tool. 😊
