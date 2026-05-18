
A lightweight cross-platform HTTP interception proxy written in modern C++.

This project was created as a way to study:

network programming
socket APIs
HTTP internals
concurrent systems
proxy architecture
terminal user interfaces in C++

The application allows intercepting, visualizing and forwarding HTTP messages in real time through a terminal interface built with FTXUI.

Features
HTTP request interception
Real-time message queue visualization
Request/response inspection
Cross-platform architecture (Windows/Linux)
Concurrent connection handling
Interactive terminal UI
Manual forwarding workflow
Message persistence ("Keep" mode)
Modular proxy architecture
Architecture Overview

The project follows a layered architecture designed to separate:

UI
networking
parsing
message management
platform-specific socket handling

Page 2: entering an endpoint
<img width="1103" height="582" alt="Captura de tela 2026-05-18 105547" src="https://github.com/user-attachments/assets/d5218b2d-099a-41ba-881f-9180d117b1bd" />

Page 3: visualize messages
<img width="917" height="758" alt="image" src="https://github.com/user-attachments/assets/65a6b810-4f4d-454c-8e68-4b8d06ebd06f" />
