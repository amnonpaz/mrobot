#include "bluetooth.hpp"

#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include <memory>

namespace mrobot {

namespace comm {

bool Bluetooth::scan() {
    int fd = ::hci_get_route(nullptr);
    if (fd < 0) {
        return false;
    }

    int socket = ::hci_open_dev(fd);
    if (socket < 0) {
        return false;
    }

    auto *inquiriesPtr = new ::inquiry_info[s_scanMaxResponse];
    std::unique_ptr<::inquiry_info[]> inquiries(inquiriesPtr);

    // hci_inquiry doesn't change inquiriesPtr, therefore
    // this code is safe, although not best practice
    int nResponses = ::hci_inquiry(fd,
                                   s_scanTimeLength,
                                   s_scanMaxResponse,
                                   nullptr,
                                   &inquiriesPtr,
                                   IREQ_CACHE_FLUSH);
    if (nResponses <= 0) {
        ::close(socket);
        return false;
    }

    char addr[s_maxAddressLength];
    char name[s_maxNameLength];
    m_peers.clear();
    for (int i = 0; i < nResponses; ++i) {
        ::memset(addr, 0, sizeof(addr));
        ::ba2str(&inquiries[i].bdaddr, addr);

        ::memset(name, 0, sizeof(name));
        int res = ::hci_read_remote_name(socket,
                                         &inquiries[i].bdaddr,
                                         sizeof(name), 
                                         name, 0);
        if (res < 0) {
            ::strcpy(name, s_defaultName);
        }

        m_peers.push_back({std::string(name), std::string(addr)});
    }

    ::close(socket);

    return true;
}

} // namespace comm

} // namespace mrobot
