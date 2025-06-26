#define STYLE_CUSTOM ImGuiWindowFlags_NoDecoration|ImGuiWindowFlags_NoBackground

#pragma once
#include <string>
class BaseWindow
{
    using visble_pointer= bool*;
    public:
        BaseWindow() {}

        virtual void draw() = 0;

        float getWidth();
        float getHeight();

        void setWidth(float width);
        void setHeight(float height);
        void setVisble(bool isVisble);
        void setInit(bool init);

        bool isVisble();
        bool isInit();
        
    protected:
        float width;
        float height;

        bool visble;
        bool init;
        visble_pointer visblePointer=&visble;
        std::string name;
        
    
};

