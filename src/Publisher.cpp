//
// Created by Bogdan on 19.03.2023.
//

#include "Publisher.h"


MPublisher::MPublisher(const char *host, int port) {
    auto neg = mosquitto_lib_init();
    if(neg == MOSQ_ERR_UNKNOWN){
        throw std::runtime_error("Publisher could not connect to broker!: Sockets couldn’t be initialized.");
    }
    mosq = mosquitto_new("publisher", true, NULL);

    int return_code;
    return_code = mosquitto_connect(mosq, host, port, 60);
    if(return_code){
        mosquitto_destroy(mosq);

        std::string error_message =  "Publisher could not connect to broker! ";
        switch(return_code) {
            case MOSQ_ERR_INVAL:
                error_message += "Input parameters were invalid\n";
                throw std::runtime_error(error_message);
            case MOSQ_ERR_ERRNO:
                error_message += "system call returned an error\n";
                throw std::runtime_error(error_message);
        }
    }
}

MPublisher::~MPublisher() {
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}

void MPublisher::publish(std::string message){

    int return_code = mosquitto_publish(mosq, NULL, "test/t1", message.size()+1, message.data(), 0, false);
    //mosquitto_loop(mosq, 1000, 1) ;
    _sleep(1);
    std::cout << "Publish in "<< message ;
    if(return_code != MOSQ_ERR_SUCCESS) {
        std::string error_message = "Publisher could not send a message: ";
        switch (return_code) {
            case MOSQ_ERR_INVAL:
                error_message += "Input parameters were invalid\n";
                throw std::runtime_error(error_message);
            case MOSQ_ERR_NOMEM:
                error_message += "out of memory condition occurred\n";
                throw std::runtime_error(error_message);
            case MOSQ_ERR_NO_CONN:
                error_message += "client isn’t connected to a broker\n";
                throw std::runtime_error(error_message);
            case MOSQ_ERR_PROTOCOL:
                error_message += "protocol error communicating with the broker\n";
                throw std::runtime_error(error_message);
            case MOSQ_ERR_PAYLOAD_SIZE:
                error_message += "payloadlen is too large\n";
                throw std::runtime_error(error_message);
            case MOSQ_ERR_MALFORMED_UTF8:
                error_message += "the topic is not valid UTF-8\n";
                throw std::runtime_error(error_message);
            case MOSQ_ERR_QOS_NOT_SUPPORTED:
                error_message += "the QoS is greater than that supported by the broker\n";
                throw std::runtime_error(error_message);;
            case MOSQ_ERR_OVERSIZE_PACKET:
                error_message += " the resulting packet would be larger than supported by the broker\n";
                throw std::runtime_error(error_message);
        }
    }
    
}