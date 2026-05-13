#include "MainApp.h"

#include <format>

using namespace ftxui;

MainApp::~MainApp()
{
    if (m_proxy)
        m_proxy->stop();

    if (proxyThread.joinable())
        proxyThread.join();
}

ftxui::Element MainApp::manual_page()
{
    switch (optionsState.selectedLanguage)
    {
        case 1:
        {
            return
                vbox({
                    filler(),
                    paragraph(R"(
            Este programa pretende ser uma forma simples de verificar e editar mensagens HTTP, 
            seja uma requisicao ou uma resposta. A ideia de construir este projeto surgiu quando
            comecei a estudar programacao de rede em C++ e vi como alguns recursos do BurpSuite 
            eram legais. Entao, decidi fazer um por conta propria (talvez esta parte nao tenha 
            sido tao legal quanto eu pensava).
                
            Para ser mais direto, o fluxo funciona da seguinte forma:
                - Apos pressionar "1", uma janela de entrada sera aberta esperando que voce insira
                um endpoint (host + porta) onde este programa ficara ouvindo as mensagens. 
                Exemplo de entrada:

                    host: 127.0.0.1

                    port: 3000
            
                - Em seguida, voce vera uma janela onde poderA verificar uma fila de todas as 
                mensagens que chegarem. Voce decide qual mensagem mais lhe atrai (ou, se ja 
                estiver procurando por uma especifica, basta esperar por ela) e o que fazer 
                com ela: alterar o metodo, alterar o conteudo e assim por diante.

                - Depois de escolher uma mensagem e realizar todas as operacoes desejadas, 
                voce pode simplesmente encaminhar a mensagem para o servidor e, se pretender,
                aguardar pela respectiva resposta.
                - Se voce estiver apenas observando as mensagens e nao quiser editar nada 
                (lembre-se que o processo de edicao atrasa o envio das mensagens ate que voce 
                faca o encaminhamento), voce pode desativar a flag "Keep", o que significa que 
                o programa apenas recebera as mensagens, fara um registro delas e as 
                encaminhara automaticamente.)") | center | flex,
                    filler(),
                    }) | center | flex;
        }
        case 2: 
        {
            return
                vbox({
                    filler(),
                    paragraph(R"(
            Este programa pretende ser una forma sencilla de verificar y editar mensajes HTTP, 
            ya sea una peticion o una respuesta. La idea de construir este proyecto se me ocurrio 
            cuando empece a estudiar programacion de redes en C++ y vi lo geniales que eran algunas 
            funciones de BurpSuite. Asi que decidi hacer uno por mi cuenta (tal vez esta parte no 
            fue tan genial como pensaba).
                
            Para ser mas directo, el flujo es el siguiente:
                - Despues de presionar "1", se abrira una ventana de entrada esperando que ingreses um endpoint 
                (host + puerto) donde este programa estara escuchando mensajes. Ejemplo de entrada:

                    host: 127.0.0.1

                    port: 3000
            
                - Luego, veras una ventana donde podras consultar una cola de todos los mensajes que 
                iran llegando. Tu decides que mensaje te atrae mas (o si ya estas buscando uno especifico, 
                solo esperalo) y que hacer con el: cambiar el metodo, cambiar el contenido, etc.

                - Despues de elegir un mensaje y realizar todas las operaciones que desees, puedes 
                simplemente reenviar el mensaje al servidor y, si lo deseas, esperar la respuesta respectiva.
                - Si solo estas observando los mensajes y no quieres editar nada (ten en cuenta que 
                el proceso de edicion retrasa el envio de los mensajes hasta que realices el reenvio), puedes 
                desactivar la bandera "Keep", lo que significa que el programa simplemente tomara los mensajes, 
                hara un registro de ellos y los reenviara automaticamente.)") | center | flex,
                    filler(),
                    }) | center | flex;
        }
        default:
        {
            return
                vbox({
                    filler(),
                    paragraph(R"(
        This program intends to be a simple way to check and edit HTTP messages, whether 
        its a request or a response. The idea to build this project came to me when i 
        started to study network programming in C++ and i saw how cool were some of BurpSuit 
        features.So i decided to make one on my own (maybe this part wasnt as cool as i thought).

        To be more straightforward, the flow is as follows :
            - After press "1" a input window will open waiting for you to enter a
                endpoint(host + port) where this program will be listen for messages.
                Example of input :
                    - host: 127.0.0.1
                    - port : 3000
            - Then, you will see a window where you can check a queue of all the messages that will
                be arriving.You decide which message more attracts you(or if you are already looking
                for some specific one, just wait for it) and what to do with it : change the method,
                change content, and so on.
            - After chose a message, do all the operations you want to, you can just forward
                the message to the server and, if you intend to, wait for the respective response.
            - If you are just watching for the messages and dont want to edit nothing(keep in
                mind that the process of editing delays the messages sending until you make the
                forwarding) you can disable the "Keep" flag, which means that the program will
                just take the messages, make a register of it and automatically forward.)") | center | flex,
                    filler(),
                    }) | center | flex;
        }
    }
}

ftxui::Element MainApp::input_endpoint_page()
{
    auto errorElement = m_submitError.empty()
        ? text("")
        : text(m_submitError) | color(Color::Red) | bold;

    return vbox({
        text("Endpoint Configuration") | bold,
        separator(),
        hbox({
            text("Host: "),
            inputHost->Render()
        }),

        hbox({
            text("Port: "),
            inputPort->Render()
        }),
        separator(),
        errorElement,
        btnSubmit->Render(),
        separator(),
        text("Esc to comeback to menu") | color(Color::GrayLight) | italic,
    }) | border;
}

ftxui::Element MainApp::messages_menu_page()
{
    if (endpointState.host == "" &&
        endpointState.port == "")
    {
        return text("Enter a endpoint at page 2");
    }

    auto messages = m_proxy->getQueue().snapshot();

    if (messages.empty())
        return text("Queue is empty.");

    messagesState.m_messageEntries.clear();
    for (auto it = messages.begin(); it != messages.end(); it++)
    {
        std::string entry;
        if (it->isRequest()) {
            entry = std::format("{:<6} {:<8} {}",
                messagesState.m_messageEntries.size() + 1,
                it->request().method,
                it->request().path);
        }
        else {
            entry = std::format("{:<6} {:<8} {}",
                messagesState.m_messageEntries.size() + 1,
                it->response().statusCode,
                it->response().statusText);
        }
        messagesState.m_messageEntries.push_back(entry);
    }

    Element detail;
    switch (optionsState.selectedLanguage)
    {
    case 1:
        detail = text("Selecione uma mensagem") | center;
        break;
    case 2:
        detail = text("Selecione una mensage") | center;
        break;
    default:
        detail = text("Select a message") | center;
        break;
    }

    if (messagesState.selectedMessage >= 0 &&
        messagesState.selectedMessage < messages.size())
    {
        const auto& msg = messages[messagesState.selectedMessage];
        std::vector<Element> headerElements;
        for (const auto& [key, value] : msg.headers)
        {
            headerElements.push_back(text(key + ": " + value));
        }

        detail = vbox({
            text("Headers: ") | bold,
            vbox(std::move(headerElements)),
            separator(),
            text("Body: ") | bold,
            paragraph(msg.body)
            });
    }
    return vbox({
           hbox({
               text(" #      ") | bold,
               text(" Method ") | bold,
               text(" Path/Status   ") | bold,
           }) | color(Color::Blue),
           separator(),
           messages_container->Render() | flex,
           separator(),
           detail | flex,
        }) | border;
}

ftxui::Element MainApp::options_page()
{
    std::string selected_name = optionsState.languages[optionsState.selectedLanguage];

    return vbox({
        separator(),
        options_container->Render(),
        separator(),
    }) | border;
}

void MainApp::start() {

    auto screen = ftxui::ScreenInteractive::Fullscreen();

    auto header =
        hbox({
            text(" ProxyInterceptor ") | color(Color::Blue),
            filler(),
            text(" @github: leonardoLopesDev1908 ")
            }) | border;

    //Menu 
    std::vector<std::string> pages = {
        "[1] Manual;", "[2] Enter/Change endpoint;", "[3] Messages menu;",
        "[4] Options;","[5] Exit;"
    };
    menu = Menu({
         &pages,
         &selected,
    });

    //Endpoint input components
    inputHost = Input(&endpointState.host, "Host");
    inputPort = Input(&endpointState.port, "Port");

    inputPort |= CatchEvent([&](Event event) {
        return event.is_character() && !std::isdigit(event.character()[0]);
    });

    auto submitEndpoint = [&] {
        m_submitError = "";

        if(endpointState.host.empty() 
            || endpointState.port.empty())
        {
            m_submitError = "Host or port empties.";
            return;
        }

        #ifdef _WIN32
            m_proxy = std::make_shared<ProxyWindows>(&endpointState.host, &endpointState.port);
        #elif __linux__
            m_proxy = std::make_shared<ProxyLinux>(&endpointState.host, &endpointState.port);
        #endif
        proxyThread = std::thread([p = m_proxy, &error = m_submitError, &screen] {
            try {
                p->start();
            }
            catch (std::exception& e)
            {
                error = e.what();
                screen.PostEvent(ftxui::Event::Custom); 
            }
        });
        m_proxy->getQueue().setScreen(&screen);
    };
    
    btnSubmit = Button("Launch proxy", submitEndpoint, ButtonOption::Animated());

    input_container = Container::Vertical({
        inputHost,
        inputPort,
        btnSubmit
    });

    //Message selection
    messages_container = Menu({
        &messagesState.m_messageEntries,
        &messagesState.selectedMessage
    });

    //Options
    auto radio = Radiobox(&optionsState.languages, &optionsState.selectedLanguage);
    auto checkKeep = Checkbox("Enable keep messages", &optionsState.keepMessagesFlag);

    options_container = Container::Vertical({ 
        radio,
        checkKeep
    });

    main_container = Container::Tab({
        menu,
        input_container,
        messages_container,
        options_container
    }, &activeTab);

    auto renderer = ftxui::Renderer(main_container, [&] {
        Element body;
        switch (activeTab)
        {
        case 0:
            body = manual_page();
            break;
        case 1:
            body = input_endpoint_page();
            break;
        case 2:
            body = messages_menu_page();
            break;
        case 3:
            body = options_page();
            break;  
        case 4:
            if (m_proxy->proxyRun)
                m_proxy->proxyRun = false;
            m_proxy->stop();
            screen.ExitLoopClosure()();
            break;
        }

        return vbox({
           header,
           separator(),
           body | flex,
           separator(),
           menu->Render()
        });
    });

    auto app = CatchEvent(renderer, [&](Event event) {
        if(event == Event::Escape && activeTab != 0)
        {
            activeTab = 0;
            main_container->SetActiveChild(menu);
            return true;
        }

        if (event == Event::Return && activeTab == 0)
        {
            switch(selected)
            {
                case 1: 
                    activeTab = 1;
                    main_container->SetActiveChild(input_container);
                    return true;
                case 3:
                    activeTab = 3;
                    main_container->SetActiveChild(options_container);
                    options_container->SetActiveChild(radio);
                    return true;
                case 4:
                    if (m_proxy->proxyRun)
                        m_proxy->proxyRun = false;
                    m_proxy->stop();
                    screen.ExitLoopClosure()();
                    return true;
                default:
                    activeTab = selected;
                    return true; 
            }
        }
        return false;
    });
    screen.Loop(app);
}
