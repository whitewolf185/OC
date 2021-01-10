#include <unistd.h>
#include <pthread.h>
#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <algorithm>
#include <signal.h>
#include <map>

#include "network.h"
//#define DEBUG

int id = -1, next_id = -1, prev_id = -1;
std::unique_ptr<zmqpp::socket> front_in(nullptr), front_out(nullptr), front_ping(nullptr),
        back_in(nullptr), back_out(nullptr), back_ping(nullptr);
std::string back_in_port, back_out_port, back_ping_port, front_ping_port;
zmqpp::context context;


void* ping(void*) {
    int packet[2];
    size_t length = 2 * sizeof(int);
    while (*front_ping) {
        front_ping->receive_raw(reinterpret_cast<char *>(packet), length);
        // std::cout << "received ping " << packet[0] << " " << packet[1] << std::endl;
        if (packet[0] != id) {
            // std::cout << "sending forward" << std::endl;
            if (back_ping.get() != nullptr) {
                if (!back_ping->send_raw(reinterpret_cast<char *>(packet), length, zmqpp::socket::dont_wait)) {
                    packet[1] = 0;
                } else if (!back_ping->receive_raw(reinterpret_cast<char *>(packet), length)) {
                    packet[1] = 0;
                    back_ping->close();
                    back_ping.reset(new zmqpp::socket(context, zmqpp::socket_type::req));
                    back_ping->set(zmqpp::socket_option::receive_timeout, 1000);
                    back_ping->connect(host + back_ping_port);
                }
            } else {
                front_ping->close();
                front_ping.reset(new zmqpp::socket(context, zmqpp::socket_type::rep));
                front_ping->bind(host + front_ping_port);
                continue;
            }
        }
        // std::cout << "sending back" << std::endl;
        front_ping->send_raw(reinterpret_cast<char *>(packet), length);
    }
    return NULL;
}


#ifdef DEBUG
[[noreturn]] void* awake(void* arg){
    bool* flag = static_cast<bool*>(arg);
    while(true) {
        sleep(5);
        std::cout << "im making flag true" << std::endl;
        *flag = true;
    }
}
#endif


void* back_to_front(void*) {
    zmqpp::message msg;
    //std::cout << "im in back_to_front" << std::endl; //debug
#ifdef DEBUG
    bool flag = true;
    pthread_t flag_awake;//debug
    check(pthread_create(&flag_awake, NULL, awake, (void*) &flag),
          -1,
          "pthread_create error");
    if(pthread_detach(flag_awake) != 0){
        perror("detach error");
    }
#endif

    while (back_in.get() == nullptr || *back_in) {
#ifdef DEBUG
        if(flag){
            std::cout << "im in back_to_front" << std::endl; //debug
            std::cout << back_in_port << std::endl;
        }
#endif

        if (back_in.get() == nullptr) {
#ifdef DEBUG
            flag = false;
#endif
            continue;
        }

        back_in->receive(msg);
        // std::cout << "sending back" << std::endl;
        int rid;
        msg >> rid;
        if (rid != id) {
            front_out->send(msg);
            continue;
        }

        int act;
        msg >> act;
        switch (static_cast<action>(act)) {
            case action::rebind_back: {
                msg >> next_id;
                back_out->disconnect(host + back_out_port);
                msg >> back_out_port;
                back_out->connect(host + back_out_port);

                back_in->unbind(host + back_in_port);
                msg >> back_in_port;
                back_in->bind(host + back_in_port);

                back_ping->disconnect(host + back_ping_port);
                msg >> back_ping_port;
                back_ping->connect(host + back_ping_port);
                break;
            }

            case action::unbind_back: {
                back_in.reset(nullptr);
                back_out.reset(nullptr);
                back_ping.reset(nullptr);
                next_id = -1;
                break;
            }

            default: {}
        }
    }
    return NULL;
}


int main(int argc, char* argv[]){
    assert(argc >= 4);
    id = strtoul(argv[1], nullptr, 10);
    prev_id = strtoul(argv[2], nullptr, 10);
    std::string bridge_port(argv[3]);
    // std::cout << getpid() << " " << id << std::endl;

    zmqpp::message msg;

    zmqpp::socket bridge(context, zmqpp::socket_type::req);
    bridge.connect(host + bridge_port);

    front_in.reset(new zmqpp::socket(context, zmqpp::socket_type::pull));
    std::string front_in_port = std::to_string(try_bind(*front_in));

    front_ping.reset(new zmqpp::socket(context, zmqpp::socket_type::rep));
    front_ping_port = std::to_string(try_bind(*front_ping));

    msg << front_in_port << front_ping_port;
    bridge.send(msg);

    bridge.receive(msg);

    std::string front_out_port;
    msg >> front_out_port;
    front_out.reset(new zmqpp::socket(context, zmqpp::socket_type::push));
    front_out->connect(host + front_out_port);

    bridge.disconnect(host + bridge_port);
    bridge.close();
    bridge = zmqpp::socket(context, zmqpp::socket_type::rep);
    bridge_port = std::to_string(try_bind(bridge));

    {
        zmqpp::message ans;
        ans << id << static_cast<int>(action::fork) << getpid();
        front_out->send(ans);
#ifdef DEBUG
        std::cout << "sending the msg"<< std::endl;
#endif
    }

    pthread_t ping_id;
    check(pthread_create(&ping_id, NULL, ping, NULL), -1, "pthread_create error");
    if (pthread_detach(ping_id) != 0) {
        perror("detach error");
    }

    pthread_t back_to_front_id;
    check(pthread_create(&back_to_front_id, NULL, back_to_front, NULL), -1, "pthread_create error");
    if (pthread_detach(back_to_front_id) != 0) {
        perror("detach error");
    }

    int act;
    int tid;

    zmqpp::socket timer_sock(context, zmqpp::socket_type::push);
    std::string timer_port = std::to_string(try_bind(timer_sock));
    bool timer_started = false;
    while (true) {
        if (!front_in->receive(msg)) {
            perror("");
        }

        msg >> tid;

        if (tid != id) {
            // std::cout << "sending forward" << std::endl;
            back_out->send(msg);
            continue;
        }

        msg >> act;

        switch (static_cast<action>(act)) {
            case action::fork: {
                int cid;
                msg >> cid;


                if (next_id == -1) {
                    next_id = cid;

                    int pid = fork();
                    check(pid, -1, "fork error");
                    if (pid == 0) {
                        check(execl("client", "client", std::to_string(cid).c_str(), std::to_string(id).c_str(),
                                    bridge_port.c_str(), NULL), -1, "execl error");
                    }


                    back_in.reset(new zmqpp::socket(context, zmqpp::socket_type::pull));
                    back_in_port = std::to_string(try_bind(*back_in));

                    zmqpp::message ports;
                    bridge.receive(ports);

                    ports >> back_out_port >> back_ping_port;
                    ports.pop_back();
                    ports.pop_back();

                    back_out.reset(new zmqpp::socket(context, zmqpp::socket_type::push));
                    back_ping.reset(new zmqpp::socket(context, zmqpp::socket_type::req));
                    back_ping->set(zmqpp::socket_option::receive_timeout, 1000);

                    back_out->connect(host + back_out_port);
                    back_ping->connect(host + back_ping_port);

                    ports << back_in_port;
                    bridge.send(ports);
                }
                std::cout << "im in fork" << std::endl;
                break;
            }

            case action::done: {
                std::cout << "im in client's done" << std::endl;
                zmqpp::message exit_msg;
                exit_msg << tid << static_cast<int>(action::exit);
                timer_sock.send(exit_msg);
                std::cout << "client's done msg was sent" << std::endl;
                break;
            }

            case action::start: {
                if(timer_started){
                    std::cout << "timer already started" << std::endl;
                    break;
                }

                int pid;
                pid = fork();
                check(pid,-1,"fork error");
                if(pid == 0){
                    check(execl("timer", "timer",timer_port.c_str(), front_in_port.c_str(), NULL),
                          -1,
                          "execl error");
                }
                size_t wait;
                msg >> wait;

                zmqpp::message timer_msg;
                timer_msg << tid << static_cast<int>(action::start) << wait;
                timer_sock.send(timer_msg);

                timer_started = true;
                std::cout << "message sent. flag = " << timer_started << std::endl;

                break;
            }

            case action::time: {
                std::cout << "im in client's time" << std::endl;
                if(!timer_started){
                    std::cout << "You had not started timer" << std::endl;
                    break;
                }
                std::cout << "trying to send msg" << std::endl;

                zmqpp::message timer_msg;
                timer_msg << tid << static_cast<int>(action::time) ;
                std::cout << "msg = " << timer_sock.send(timer_msg) << std::endl;
                break;
            }

            case action::stop: {
                if(!timer_started){
                    std::cout << "Please start timer" << std::endl;
                    break;
                }

                zmqpp::message timer_msg;
                timer_msg << tid << static_cast<int>(action::stop) ;
                timer_sock.send(timer_msg);
                timer_started = false;
                break;
            }

            default:{}
        }
    }


    return 0;
}
