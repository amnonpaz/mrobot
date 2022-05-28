#include "bluetooth.hpp"
#include <iostream>

int main()
{
    mrobot::comm::Bluetooth bt;

    if (!bt.scan()) {
        std::cerr << "Failed scanning for peers" << '\n';
        return 1;
    }

    std::cout << "Peers list:" << '\n';
    for (const auto &peer : bt.getPeersList()) {
        std::cout << peer.name << " [" << peer.id << "]" << '\n';
    }

    return 0;
}
