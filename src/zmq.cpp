#include <zmq.h>
#include <string>
#include <iostream>

int main() {
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp:://*:5555");
    while (true) {
        zmq::message_t request;
        socket.recv(&request);
        std::cout << "Recived Hello" << std::endl;
        sleep(1);
        zmq::message_t reply(5);
        memcpy(reply.data(), "World", 5);
        socket.send(reply);
    }
    return 0;
}
