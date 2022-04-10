#pragma once

#include <list>
#include <string>

namespace mrobot {

namespace comm {

class Bluetooth final {
    public:
        struct Peer {
            std::string name;
            std::string id;
        };

        Bluetooth() :
            m_fd(0), m_socket(0) {}
        ~Bluetooth() = default;

        bool initialize();
        void finalize();

        bool scan();
       
        const std::list<Peer> &getPeersList() const { return m_peers; }

    private:
        int m_fd;
        int m_socket;

        std::list<Peer> m_peers;

        static constexpr uint32_t s_scanTimeLength = 8;
        static constexpr uint32_t s_scanMaxResponse = 255;
        static constexpr uint32_t s_maxAddressLength = 19;
        static constexpr uint32_t s_maxNameLength = 248;
        static constexpr char s_defaultName[] = "unknown";
};

} // namespace comm

} // namespace mrobot
