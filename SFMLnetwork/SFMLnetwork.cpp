#include <iostream>
#include <string>
#include <thread>
#include <SFML/Network.hpp>

short port = 6000;
sf::TcpSocket socket;
sf::IpAddress ip = sf::IpAddress::getLocalAddress();
char type;

std::string name;
std::string message;
sf::Packet sendPacket;
sf::Packet packet;

void Chat();

int main() {

    std::cout << "Local Addres " << sf::IpAddress::getLocalAddress() << " \t Public Addres " << sf::IpAddress::getPublicAddress() << std::endl;
    std::cout << "exit (q)\n";
    std::cout << "server (s) : client (c)\n";
    std::cin >> type;

    if (type == 'q' ) {
        return EXIT_SUCCESS;
    }
    else if (type == 'c') {
        std::cout << "connecting to (ip): ";
        std::cin >> ip;

        if (socket.connect(ip, port) != sf::Socket::Done) {
            std::cout << "Connection error!\n";
            return 0;
        }
    }
    else if (type == 's') {
        sf::TcpListener listener;
        listener.listen(port);
        if (listener.accept(socket) != sf::Socket::Done) {
            std::cout << "Accepting error!\n";
            return 0;
        }
    }


    std::cout << "\nDON'T USE SPACE\n";
    std::cout << "write your name: ";

    std::cin >> name;

    socket.setBlocking(false);


    std::thread th(Chat);
    while (true)
    {
        if (socket.receive(packet) == sf::Socket::Done) {
            std::string nameRec;
            std::string messegeRec;

            packet >> nameRec >> messegeRec;
            std::cout << "\t" << nameRec << ": " << messegeRec << std::endl;
        }
    }




    th.join();
    return 0;
}

void Chat() {
    while (true)
    {
        std::getline(std::cin, message);
        if (message != "") {
            sendPacket.clear();

            sendPacket << name << message;
            
            socket.send(sendPacket);
            
            message.clear();
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}
