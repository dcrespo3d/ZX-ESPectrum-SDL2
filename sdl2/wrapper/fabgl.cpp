#include "fabgl.h"
#include <SDL.h>

const fabgl::KeyboardLayout fabgl::USLayout;
const fabgl::KeyboardLayout fabgl::UKLayout;
const fabgl::KeyboardLayout fabgl::GermanLayout;
const fabgl::KeyboardLayout fabgl::ItalianLayout;
const fabgl::KeyboardLayout fabgl::SpanishLayout;
const fabgl::KeyboardLayout fabgl::FrenchLayout;
const fabgl::KeyboardLayout fabgl::BelgianLayout;
const fabgl::KeyboardLayout fabgl::NorwegianLayout;
const fabgl::KeyboardLayout fabgl::JapaneseLayout;

#include <deque>
#include <utility>
#include <map>

typedef std::pair<uint32_t, SDL_Scancode> SDLKeyEventPair;

static std::deque<SDLKeyEventPair> keyEventQueue;

typedef std::map<fabgl::VirtualKey, SDL_Scancode> SDL4FAB;
typedef std::map<SDL_Scancode, fabgl::VirtualKey> FAB4SDL;

static SDL4FAB sdl4fab;
static FAB4SDL fab4sdl;

#define ASSOC_PAIR_FAB_SDL(FAB, SDL) {\
    sdl4fab[FAB] = SDL; \
    fab4sdl[SDL] = FAB; \
}

void initKeyEventQueue()
{
    ASSOC_PAIR_FAB_SDL(fabgl::VK_0, SDL_SCANCODE_0);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_1, SDL_SCANCODE_1);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_2, SDL_SCANCODE_2);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_3, SDL_SCANCODE_3);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_4, SDL_SCANCODE_4);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_5, SDL_SCANCODE_5);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_6, SDL_SCANCODE_6);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_7, SDL_SCANCODE_7);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_8, SDL_SCANCODE_8);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_9, SDL_SCANCODE_9);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_Q, SDL_SCANCODE_Q);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_W, SDL_SCANCODE_W);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_E, SDL_SCANCODE_E);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_R, SDL_SCANCODE_R);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_T, SDL_SCANCODE_T);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_Y, SDL_SCANCODE_Y);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_U, SDL_SCANCODE_U);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_I, SDL_SCANCODE_I);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_O, SDL_SCANCODE_O);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_P, SDL_SCANCODE_P);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_A, SDL_SCANCODE_A);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_S, SDL_SCANCODE_S);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_D, SDL_SCANCODE_D);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_F, SDL_SCANCODE_F);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_G, SDL_SCANCODE_G);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_H, SDL_SCANCODE_H);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_J, SDL_SCANCODE_J);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_K, SDL_SCANCODE_K);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_L, SDL_SCANCODE_L);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_Z, SDL_SCANCODE_Z);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_X, SDL_SCANCODE_X);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_C, SDL_SCANCODE_C);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_V, SDL_SCANCODE_V);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_B, SDL_SCANCODE_B);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_N, SDL_SCANCODE_N);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_M, SDL_SCANCODE_M);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_ESCAPE, SDL_SCANCODE_ESCAPE);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_RETURN, SDL_SCANCODE_RETURN);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_SPACE, SDL_SCANCODE_SPACE);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_F1, SDL_SCANCODE_F1);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_F2, SDL_SCANCODE_F2);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_F3, SDL_SCANCODE_F3);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_F4, SDL_SCANCODE_F4);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_F5, SDL_SCANCODE_F5);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_F6, SDL_SCANCODE_F6);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_F7, SDL_SCANCODE_F7);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_F8, SDL_SCANCODE_F8);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_F9, SDL_SCANCODE_F9);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_F10, SDL_SCANCODE_F10);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_F11, SDL_SCANCODE_F11);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_F12, SDL_SCANCODE_F12);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_LEFT, SDL_SCANCODE_LEFT);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_RIGHT, SDL_SCANCODE_RIGHT);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_UP, SDL_SCANCODE_UP);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_DOWN, SDL_SCANCODE_DOWN);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_KP_LEFT, SDL_SCANCODE_KP_4);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_KP_RIGHT, SDL_SCANCODE_KP_6);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_KP_UP, SDL_SCANCODE_KP_8);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_KP_DOWN, SDL_SCANCODE_KP_2);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_KP_CENTER, SDL_SCANCODE_KP_5);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_LSHIFT, SDL_SCANCODE_LSHIFT);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_RSHIFT, SDL_SCANCODE_RSHIFT);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_LCTRL, SDL_SCANCODE_LCTRL);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_RCTRL, SDL_SCANCODE_RCTRL);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_LALT, SDL_SCANCODE_LALT);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_RALT, SDL_SCANCODE_RALT);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_HOME, SDL_SCANCODE_HOME);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_END, SDL_SCANCODE_END);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_PAGEUP, SDL_SCANCODE_PAGEUP);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_PAGEDOWN, SDL_SCANCODE_PAGEDOWN);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_BACKSPACE, SDL_SCANCODE_BACKSPACE);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_COMMA, SDL_SCANCODE_COMMA);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_PERIOD, SDL_SCANCODE_PERIOD);
    ASSOC_PAIR_FAB_SDL(fabgl::VK_TAB, SDL_SCANCODE_TAB);
    //ASSOC_PAIR_FAB_SDL(fabgl::VK_, SDL_SCANCODE_);
    //ASSOC_PAIR_FAB_SDL(fabgl::VK_, SDL_SCANCODE_);
}

void pushToKeyEventQueue(uint32_t type, SDL_Scancode ekey)
{
    keyEventQueue.push_back(SDLKeyEventPair(type, ekey));
}

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

    bool Keyboard::isVKDown(VirtualKey vk)
    {
        SDL4FAB::iterator it = sdl4fab.find(vk);
        if (it == sdl4fab.end()) return false;

        SDL_Scancode sdlscan = (*it).second;

        const uint8_t* sdlkeys = SDL_GetKeyboardState(NULL);
        return bool(sdlkeys[sdlscan]);
    }

    int Keyboard::virtualKeyAvailable()
    {
        return keyEventQueue.empty() ? 0 : 1;
    }

    bool Keyboard::getNextVirtualKey(VirtualKeyItem* item, int timeOutMS)
    {
        item->down = false;

        if (keyEventQueue.empty()) return false;
        //printf("queue size: %d\n", keyEventQueue.size());

        auto keyEvent = keyEventQueue.front();
        keyEventQueue.pop_front();

        FAB4SDL::iterator it = fab4sdl.find(keyEvent.second);
        if (it == fab4sdl.end()) return false;

        item->down = keyEvent.first == SDL_KEYDOWN;
        item->vk = (*it).second;

        return true;
    }

    SquareWaveformGenerator::SquareWaveformGenerator()
        : m_phaseInc(0),
        m_phaseAcc(0),
        m_frequency(0),
        m_lastSample(0),
        m_dutyCycle(127)
    {
    }


    void SquareWaveformGenerator::setFrequency(int value) {
        if (m_frequency != value) {
            m_frequency = value;
            m_phaseInc = (((uint32_t)m_frequency * 256) << 11) / sampleRate();
        }
    }


    // dutyCycle: 0..255 (255=100%)
    void SquareWaveformGenerator::setDutyCycle(int dutyCycle)
    {
        m_dutyCycle = dutyCycle;
    }


    int SquareWaveformGenerator::getSample() {
        if (m_frequency == 0 || duration() == 0) {
            if (m_lastSample > 0)
                --m_lastSample;
            else if (m_lastSample < 0)
                ++m_lastSample;
            else
                m_phaseAcc = 0;
            return m_lastSample;
        }

        uint32_t index = m_phaseAcc >> 11;
        // int sample = (index <= m_dutyCycle ? 127 : -127);
        int sample = (index <= m_dutyCycle ? 255 : 0);

        // process volume
        sample = sample * volume() / 127;

        m_lastSample = sample;

        m_phaseAcc = (m_phaseAcc + m_phaseInc) & 0x7ffff;

        decDuration();

        return sample;
    }



}