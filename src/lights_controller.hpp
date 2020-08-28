#ifndef _LIGHTS_CONTROL_H
#define _LIGHTS_CONTROL_H

#include "led.hpp"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>
#include <limits>
#include <unistd.h>

namespace mrobot {

namespace control {

class Lights final {
    public:
        enum SchemeType {
            SchemeTypeBlink
        };

        Lights() : 
            m_scheme(SchemeTypeBlink),
            m_period(InfPeriod),
            m_between(0) {}
        ~Lights();

        void add(const hw::Led *led);
        inline void setSchemeType(SchemeType scheme) { m_scheme = scheme; }
        inline void setParameters(uint32_t period_ms, uint32_t betweenLeds_ms) {
            m_period = (period_ms > 0) ? period_ms : InfPeriod;
            m_between = betweenLeds_ms;
        }

        void start();
        void stop();

    private:
        void work();
        void blink();

        std::list<const hw::Led *> m_leds;
        SchemeType m_scheme;
        uint32_t m_period;
        uint32_t m_between;

        std::thread m_worker;
        std::mutex m_lock;
        std::condition_variable m_cv;
        bool m_isWorking;

        static constexpr uint32_t InfPeriod = std::numeric_limits<uint32_t>::max();
};

} // namespace control

} // namespace mrobot

#endif // _LIGHTS_CONTROL_H

