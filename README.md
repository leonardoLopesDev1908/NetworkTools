<img width="1881" height="812" alt="Captura de tela 2026-07-22 094148" src="https://github.com/user-attachments/assets/27916ebf-4c8f-4a68-aba4-ef790b0435d4" />
# NetworkTools

**NetworkTools** is a project with the goal to be a practical study of network security. It has two main tools to be used
at the context of network analysis:
  Proxy Interceptor
  Traffic Analyzer

## Proxy Interceptor
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

Page 3: visualize messages
<img width="1105" height="574" alt="Captura de tela 2026-05-26 092409" src="https://github.com/user-attachments/assets/aa8bffce-4f85-4ea0-8d54-8a915611bca8" />

Page 3: edit messages
<img width="1104" height="577" alt="Captura de tela 2026-05-26 092258" src="https://github.com/user-attachments/assets/d4b2b7cc-f515-457a-a49b-4a9a5deae057" />

## Traffic Analyzer

The Traffic-Analyzer is a cross-platform tool capable of capture network packets in the selected device. For Linux it receives command line arguments visible with '--help/--h'.
For Windows, as the network device naming is different, the program shows all available devices and asks what the user want. 

This project was created as a way to study:

network programming
packet capture APIs (libpcap/npcap)
handling IPs packages
terminal user interfaces in C++

Features
pacakge capture
csv/json reports
filtering (protocols, ips, ports)

<img width="1881" height="812" alt="Captura de tela 2026-07-22 094148" src="https://github.com/user-attachments/assets/95338233-f87e-437b-8f55-79b5bba3c487" />


This part of the project is almost entirely inspired by the Traffic Analyzer at the Cybersecurity-Projects
repository developed by @deniskhud. It is available at: https://github.com/CarterPerez-dev/Cybersecurity-Projects/tree/main/PROJECTS/beginner/network-traffic-analyzer/cpp


