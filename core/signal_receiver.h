#pragma once
#include <stdint.h>

namespace amun
{
    class SignalReceiver
    {
    public:
        virtual ~SignalReceiver() = default;
        virtual void onSignal(uint8_t signal) = 0;
        virtual void onError(uint8_t error) = 0;
    };
}
