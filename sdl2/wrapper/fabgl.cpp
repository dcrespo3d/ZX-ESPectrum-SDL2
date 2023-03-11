#include "fabgl.h"

const fabgl::KeyboardLayout fabgl::USLayout;
const fabgl::KeyboardLayout fabgl::UKLayout;
const fabgl::KeyboardLayout fabgl::GermanLayout;
const fabgl::KeyboardLayout fabgl::ItalianLayout;
const fabgl::KeyboardLayout fabgl::SpanishLayout;
const fabgl::KeyboardLayout fabgl::FrenchLayout;
const fabgl::KeyboardLayout fabgl::BelgianLayout;
const fabgl::KeyboardLayout fabgl::NorwegianLayout;
const fabgl::KeyboardLayout fabgl::JapaneseLayout;

namespace fabgl
{
    PS2Controller *    PS2Controller::s_instance = nullptr;
    Keyboard *         PS2Controller::s_keyboard = nullptr;
    // Mouse *            PS2Controller::s_mouse    = nullptr;


    PS2Controller::PS2Controller()
    {
        if (!s_instance)
            s_instance = this;
    }


    PS2Controller::~PS2Controller()
    {
        // end();
        if (this == s_instance)
            s_instance = nullptr;
    }

    void PS2Controller::begin(PS2Preset preset, KbdMode keyboardMode)
    {
        s_keyboard = new Keyboard();
    }
}