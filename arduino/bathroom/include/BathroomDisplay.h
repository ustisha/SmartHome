#ifndef BATHROOM_BATHROOMDISPLAY_H
#define BATHROOM_BATHROOMDISPLAY_H

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <DebugLog.h>
#include <TInterface.h>
#include <WashingMachine.h>
#include <ValueInt.h>
#include <Net.h>
#include <DisplayST7735.h>
#include <Adafruit_ST7735.h>

class BathroomDisplay : public DisplayST7735
{
    using DisplayST7735::DisplayST7735;
public:

    void render() override;

    void addFloorModule(TInterface *tInterface)
    {
        tInterface->addDisplayHandler(this);
        floorTemp[floorTempIdx] = tInterface;
        floorTempIdx++;
    }

    void addValueModule(ValueInt *vInterface)
    {
        vInterface->addDisplayHandler(this);
        valueInt[valueIdx] = vInterface;
        valueIdx++;
    }

    void addWashingModule(WashingMachine *wm)
    {
        wm->addDisplayHandler(this);
        washing = wm;
    }

protected:

    typedef struct WashingUpdate : public ValueUpdate<float>
    {
        bool opened = false;
        uint8_t remainingSec = 0;
    };

    TInterface *floorTemp[2]{};
    ThUpdate floorUpdate[2];
    WashingMachine *washing;
    WashingUpdate washingUpdate[1];
    ValueInt *valueInt[2]{};
    ValueUpdate<long> valueUpdate[2];
    uint8_t floorTempIdx = 0;
    uint8_t valueIdx = 0;
};


#endif //BATHROOM_BATHROOMDISPLAY_H
