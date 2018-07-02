#include "MicroBit.h"
SPI            spi(MOSI, MISO, SCK);
MicroBitSerial serial(USBTX, USBRX);

// Use P16 as !CS pin.
MicroBitPin CS(MICROBIT_ID_IO_P16,
               MICROBIT_PIN_P16,
               PIN_CAPABILITY_DIGITAL);

class MCP3008{
    public:
        MCP3008(void);

        void     init(void);
        uint16_t read(char CH);
    //private:
};

MCP3008::MCP3008(void){
}
void MCP3008::init(void){
    CS.setDigitalValue(1);
    //spi.format(8, 0); // How many bits in one byte, and SPI mode0(0,0).
    //spi.frequency(1000000);
}
uint16_t MCP3008::read(char CH){
    char mosi[] = {1, 0b10000000 | CH<<4, 0};
    char miso[3];

    CS.setDigitalValue(0);
    for(int i=0; i<3; i++){
        miso[i] = spi.write(mosi[i]);
    }    
    CS.setDigitalValue(1);
    return  (miso[1]<<8 | miso[2]) & 1023;
}

////////////////
// How to use //
////////////////
int main(void){
    MCP3008 mcp; // Instantiate the MCP3008 class.
    mcp.init();  // Initialize the instance.

    while(1){
        uint16_t ADdata = mcp.read(0); // From which channel to read AD data.
        float    volt   = (float)ADdata * 3.23/1024.0; // 3.23 is the voltage measured at VREF pin.

        serial.printf("AD %d, %.2f(v)\n", ADdata, volt);
        wait(1.0);
    }

    release_fiber();
    return 0;
}
