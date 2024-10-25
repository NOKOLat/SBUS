#ifndef INC_SBUS_HPP_
#define INC_SBUS_HPP_

#include <array>
#include <iterator>

#include "ringBuffer/ringBuffer.h"

namespace nokolat {

struct SBUS_DATA{
    std::array<uint16_t,18> data;
    bool failsafe = false;
    bool framelost = false;

    uint16_t &operator[](std::size_t __n) noexcept{
        return data[__n];
    }
    uint16_t &at(std::size_t __n){
        return data.at(__n);
    }

    SBUS_DATA(std::array<uint16_t,18> data = {})
    :data(data){}
};

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
        if(needDecode){
            decode(receiveBuffer,data);
        }

        return data[channel-1];
    }

    SBUS_DATA &getData(){
        if(needDecode){
            decode(receiveBuffer,data);
        }
        return data;
    }

    constexpr uint8_t getDataLen(){
        return length;
    }

    void requireDecode(bool arg=true){
        needDecode = arg;
    }

    inline std::array<uint8_t, 25> getRawBuffer(){
    	std::array<uint8_t, 25> res;
    	for(uint8_t n=0; n<25; n++){
    		res[n] = receiveBuffer[n];
    	}
    	return res;
    }

    SBUS_DATA decode(std::array<uint8_t,25> &arg);
    SBUS_DATA decode(RingBuffer<uint8_t,25> &arg);
    void decode(std::array<uint8_t,25> &arg, SBUS_DATA &res);
    void decode(RingBuffer<uint8_t,25> &arg, SBUS_DATA &res);

    std::array<uint8_t,25> encode(SBUS_DATA &arg);

protected:
    SBUS_DATA data;
    RingBuffer<uint8_t,25> receiveBuffer;
private:
    bool needDecode;

    const uint8_t length = 25;

    const uint8_t HEADER = 0x0f;
    const uint8_t FOOTER = 0x00;
    const uint8_t FOOTER2 = 0x04;
};

} /* namespace nokolat */

#endif /* INC_SBUS_HPP_ */
