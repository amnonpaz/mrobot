#include "message.hpp"

#include <iostream>
#include <cassert>

namespace mrobot {
    
namespace messaging {

void Router::registerHandler(uint32_t messageId, Handler *handler)
{
    assert(messageId < m_handlers.size());

    m_handlers[messageId].push_back(handler);
}

bool Router::route(uint32_t messageId, const unsigned char *payload, ::size_t size)
{
    if (payload == nullptr) {
        std::cout << "Routing error: Received null payload" << '\n';
        return false;
    }

    if (messageId >= m_handlers.size()) {
        std::cout << "Routing error: Invalid message ID " << std::to_string(messageId) << '\n';
        return false;
    }

    auto messagePtr = m_factory->create(messageId, payload, size);
    if (!messagePtr) {
        std::cout << "Routing error: Invalid message" << '\n';
        return false;
    }

    for (auto *handler : m_handlers[messageId]) {
        handler->handleMessage(messagePtr);
    }

    return true;
}

} // namespace messaging

} // namespace mrobot
