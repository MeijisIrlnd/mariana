#pragma once 
#include <array>
namespace Mariana 
{ 
    template<int N>
    class Diffuser 
    { 
    public:       
        template<int N_CHANNELS>
        struct Args { 
            std::array<int, N_CHANNELS> delayTimesMS;
            std::array<bool, N_CHANNELS> polarities;
        };

        explicit Diffuser(const Args<N>& args)
        { 

        }

        ~Diffuser() = default;

        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) 
        { 

        }

        void diffuse(std::array<float, N>& samples) 
        { 

        }

        void releaseResources() 
        { 
            
        }
    };
}