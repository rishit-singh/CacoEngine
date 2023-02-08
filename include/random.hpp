#ifndef RANDOM_H_
#define RANDOM_H_

#include <random>

namespace CacoEngine::Util
{
    template<typename T = int>
    class Random
    {
    protected:
        std::random_device Device;

        std::mt19937 MTInstance;

        std::uniform_int_distribution<T> Distribution;
    public:

        T Next(T min, T max)
        {
            this->Distribution = std::uniform_int_distribution<T>(min, max);

            return this->Distribution(this->MTInstance);
        }
        
        Random() : Device(std::random_device()), MTInstance(std::mt19937(Device()))
        {
        }

        ~Random()
        {
        }
    };
}

#endif // RANDOM_H_
