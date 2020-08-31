#include "mqtt_client.hpp"

namespace mrobot {

namespace comm {

bool MosquittoClient::initialize() {
    // TODO: implement

    return true;
}

void MosquittoClient::finalize() {
    // TODO: implement
}

bool MosquittoClient::connect() {
    // TODO: implement

    return true;
}

bool MosquittoClient::disconnect() {
    // TODO: implement

    return true;
}

bool MosquittoClient::send(std::shared_ptr<char> payload, ::size_t size) {
    (void)(payload);
    (void)(size);

    // TODO: implement

    return true;
}

} // namespace comm

} // namespace mrobot
