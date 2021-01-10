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

zmqpp::context context;

class myTimer_t{
private:
    size_t time_start;
    size_t timer_wait{};

public:
    myTimer_t(const size_t new_timer_wait): timer_wait(new_timer_wait), time_start(clock()){}
    myTimer_t(): time_start(0){}
    ~myTimer_t()=default;

    void Start(const size_t new_timer_wait){
        timer_wait = new_timer_wait;
        time_start = (size_t)clock();
    }

    size_t Time(){
        return time_start + timer_wait - (size_t)clock();
    }
};


zmqpp::socket req_sock(context, zmqpp::socket_type::push);
std::string req_port;


int tid;

void* awake (void* arg){
//    std::cout << "im awake for "/* << std::flush*/;
    size_t* wait = reinterpret_cast<size_t*>(arg);
//    std::cout << *wait << std::endl;
    usleep((*wait) * 1000);
    std::cout << "OK: timer out" << std::endl;
    zmqpp::message done_msg;
    done_msg << tid << static_cast<int>(action::done);
    req_sock.send(done_msg);
    std::cout << "message was sent" << std::endl;
    return NULL;
}

int main(int argc, char* argv[]){
    std::string timer_port = argv[1];
    req_port = argv[2];
    zmqpp::socket timer_sock(context, zmqpp::socket_type::pull);
    timer_sock.connect(host + timer_port);
    req_sock.connect(host + req_port);
    myTimer_t myTimer;
    pthread_t awake_timer;
    std::cout << "im timer" << std::endl;

    while(true){
        int act;
        zmqpp::message msg;

        timer_sock.receive(msg);
        msg >> tid >> act;
        std::cout << "i get message" << std::endl << "action = " << act << " true action = "
                  << static_cast<int>(action::start) << std::endl;

        switch (static_cast<action>(act)) {
            case action::start: {
                size_t wait;
                msg >> wait;
//                    std::cout << "im in start with wait = " << wait << std::endl;
                myTimer.Start(wait);
                check(pthread_create(&awake_timer, NULL, awake, (void *) &wait),
                      -1,
                      "pthread_create error");

                if(pthread_detach(awake_timer) != 0){
                    perror("pthread_detach error");
                }
                std::cout << "OK: timer " << tid << " started" << std::endl;
                break;
            }

            case action::stop: {
                req_sock.disconnect(host + req_port);
                req_sock.close();
                timer_sock.disconnect(host + timer_port);
                timer_sock.close();
                pthread_cancel(awake_timer);
                std::cout << "OK: Timer " << tid << " stopped" << std::endl;
                exit(0);
            }

            case action::time: {
                std::cout << "im in timer's time" << std::endl;
                auto to_wait = myTimer.Time();
                std::cout << "OK: " << to_wait << " left" << std::endl;
                break;
            }

            case action::exit: {
                req_sock.disconnect(host + req_port);
                req_sock.close();
                timer_sock.disconnect(host + timer_port);
                timer_sock.close();
                std::cout << "OK: timer " << tid << " closed" << std::endl;
                exit(0);
                return 0;
            }

            default: {}
        }

    }
    return 0;
}
