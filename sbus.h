#ifndef INC_SBUS_HPP_
#define INC_SBUS_HPP_

#include <array>
#include <iterator>

namespace nokolat {

struct SBUS_DATA{
    std::array<uint16_t,18> data = {};
    bool failsafe = false;
    bool flamelost = false;

    uint16_t operator[](size_type __n) noexcept{
        return data[__n];
    }
    uint16_t at(size_type __n){
        return data.at(__n);
    }
}

class SBUS {
public:
    SBUS(){}
    virtual ~SBUS(){};

    auto getBufferIterator(){
        return receiveBuffer.cbegin();
    }

    uint8_t *getReceiveBufferPtr() {
        return (uint8_t*)receiveBuffer.data();
    }

    int16_t getData(const uint8_t channel){
        if(channel > 18 or channel < 1) return -1;
        if(needParse){
            decode(receiveBuffer,data);
        }

        return data[channel-1];
    }

    const std::array<uint16_t,18> &getData(){
        return data;
    }
    std::array<uint16_t,18> getData(){
        return data;
    }

    constexpr uint8_t getDataLen(){
        return length;
    }

    void requireDecode(bool arg=true){
        needDecode = arg;
    }

    SBUS_DATA decode(std::array<uint8_t,25> &arg);
    void decode(std::array<uint8_t,25> &arg, SBUS_DATA &res);

    std::array<uint8_t,25> encode(SBUS_DATA &arg);

private:
    std::array<uint8_t,25> receiveBuffer;
    SBUS_DATA data;
    bool needDecode;

    const uint8_t length = 25;

    const uint8_t HEADER = 0x0f;
    const uint8_t FOOTER = 0x00;
    const uint8_t FOOTER = 0x04
};

} /* namespace nokolat */

#endif /* INC_SBUS_HPP_ */