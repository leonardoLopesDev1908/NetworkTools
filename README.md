
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
<img width="1100" height="579" alt="Captura de tela 2026-05-26 092337" src="https://github.com/user-attachments/assets/ab153c21-5419-46e7-84fa-d8d33ce9ab54" />

Page 3: visualize messages
<img width="1105" height="574" alt="Captura de tela 2026-05-26 092409" src="https://github.com/user-attachments/assets/aa8bffce-4f85-4ea0-8d54-8a915611bca8" />

Page 4: Options
Here you can enable the Keep flag to edit messages
<img width="1104" height="577" alt="Captura de tela 2026-05-26 092258" src="https://github.com/user-attachments/assets/1b9d6e47-7efe-4178-8083-957c7cf12c14" />

Page 3: edit messages
<img width="1104" height="577" alt="Captura de tela 2026-05-26 092258" src="https://github.com/user-attachments/assets/d4b2b7cc-f515-457a-a49b-4a9a5deae057" />

