/*
    Simple DirectMedia Layer
    Copyright (C) 1997-2015 Sam Lantinga <slouken@libsdl.org>

    This software is provided 'as-is', without any express or implied
    warranty.    In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
         claim that you wrote the original software. If you use this software
         in a product, an acknowledgment in the product documentation would be
         appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
         misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/
#include "../../SDL_internal.h"

extern "C" {
#include "../../events/SDL_keyboard_c.h"
}
#include "SDL_scancode.h"
#include "mojo/services/input_events/public/interfaces/input_key_codes.mojom.h"

#if SDL_VIDEO_DRIVER_MOJO

#define MOJO_KEY(x) static_cast<int>(mojo::KeyboardCode::x)

static SDL_Scancode kMojoKeymap[] =
{
  [MOJO_KEY(BACK)] = SDL_SCANCODE_BACKSPACE,
  // TAB = 0x09,
  // CLEAR = 0x0C,
  [MOJO_KEY(RETURN)] = SDL_SCANCODE_RETURN,
  // SHIFT = 0x10,
  // CONTROL = 0x11,
  // MENU = 0x12,
  // PAUSE = 0x13,
  // CAPITAL = 0x14,
  // KANA = 0x15,
  // HANGUL = 0x15,
  // JUNJA = 0x17,
  // FINAL = 0x18,
  // HANJA = 0x19,
  // KANJI = 0x19,
  [MOJO_KEY(ESCAPE)] = SDL_SCANCODE_ESCAPE,
  // CONVERT = 0x1C,
  // NONCONVERT = 0x1D,
  // ACCEPT = 0x1E,
  // MODECHANGE = 0x1F,
  [MOJO_KEY(SPACE)] = SDL_SCANCODE_SPACE,
  // PRIOR = 0x21,
  // NEXT = 0x22,
  // END = 0x23,
  // HOME = 0x24,
  [MOJO_KEY(LEFT)] = SDL_SCANCODE_LEFT,
  [MOJO_KEY(UP)] = SDL_SCANCODE_UP,
  [MOJO_KEY(RIGHT)] = SDL_SCANCODE_RIGHT,
  [MOJO_KEY(DOWN)] = SDL_SCANCODE_DOWN,
  // SELECT = 0x29,
  // PRINT = 0x2A,
  // EXECUTE = 0x2B,
  // SNAPSHOT = 0x2C,
  // INSERT = 0x2D,
  // DELETE = 0x2E,
  // HELP = 0x2F,
  // NUM_0 = 0x30,
  // NUM_1 = 0x31,
  // NUM_2 = 0x32,
  // NUM_3 = 0x33,
  // NUM_4 = 0x34,
  // NUM_5 = 0x35,
  // NUM_6 = 0x36,
  // NUM_7 = 0x37,
  // NUM_8 = 0x38,
  // NUM_9 = 0x39,
  [MOJO_KEY(A)] = SDL_SCANCODE_A,
  [MOJO_KEY(B)] = SDL_SCANCODE_B,
  [MOJO_KEY(C)] = SDL_SCANCODE_C,
  [MOJO_KEY(D)] = SDL_SCANCODE_D,
  [MOJO_KEY(E)] = SDL_SCANCODE_E,
  [MOJO_KEY(F)] = SDL_SCANCODE_F,
  [MOJO_KEY(G)] = SDL_SCANCODE_G,
  [MOJO_KEY(H)] = SDL_SCANCODE_H,
  [MOJO_KEY(I)] = SDL_SCANCODE_I,
  [MOJO_KEY(J)] = SDL_SCANCODE_J,
  [MOJO_KEY(K)] = SDL_SCANCODE_K,
  [MOJO_KEY(L)] = SDL_SCANCODE_L,
  [MOJO_KEY(M)] = SDL_SCANCODE_M,
  [MOJO_KEY(N)] = SDL_SCANCODE_N,
  [MOJO_KEY(O)] = SDL_SCANCODE_O,
  [MOJO_KEY(P)] = SDL_SCANCODE_P,
  [MOJO_KEY(Q)] = SDL_SCANCODE_Q,
  [MOJO_KEY(R)] = SDL_SCANCODE_R,
  [MOJO_KEY(S)] = SDL_SCANCODE_S,
  [MOJO_KEY(T)] = SDL_SCANCODE_T,
  [MOJO_KEY(U)] = SDL_SCANCODE_U,
  [MOJO_KEY(V)] = SDL_SCANCODE_V,
  [MOJO_KEY(W)] = SDL_SCANCODE_W,
  [MOJO_KEY(X)] = SDL_SCANCODE_X,
  [MOJO_KEY(Y)] = SDL_SCANCODE_Y,
  [MOJO_KEY(Z)] = SDL_SCANCODE_Z,
  // LWIN = 0x5B,
  // COMMAND = 0x5B,
  // RWIN = 0x5C,
  // APPS = 0x5D,
  // SLEEP = 0x5F,
  // NUMPAD0 = 0x60,
  // NUMPAD1 = 0x61,
  // NUMPAD2 = 0x62,
  // NUMPAD3 = 0x63,
  // NUMPAD4 = 0x64,
  // NUMPAD5 = 0x65,
  // NUMPAD6 = 0x66,
  // NUMPAD7 = 0x67,
  // NUMPAD8 = 0x68,
  // NUMPAD9 = 0x69,
  // MULTIPLY = 0x6A,
  // ADD = 0x6B,
  // SEPARATOR = 0x6C,
  // SUBTRACT = 0x6D,
  // DECIMAL = 0x6E,
  // DIVIDE = 0x6F,
  // F1 = 0x70,
  // F2 = 0x71,
  // F3 = 0x72,
  // F4 = 0x73,
  // F5 = 0x74,
  // F6 = 0x75,
  // F7 = 0x76,
  // F8 = 0x77,
  // F9 = 0x78,
  // F10 = 0x79,
  // F11 = 0x7A,
  // F12 = 0x7B,
  // F13 = 0x7C,
  // F14 = 0x7D,
  // F15 = 0x7E,
  // F16 = 0x7F,
  // F17 = 0x80,
  // F18 = 0x81,
  // F19 = 0x82,
  // F20 = 0x83,
  // F21 = 0x84,
  // F22 = 0x85,
  // F23 = 0x86,
  // F24 = 0x87,
  // NUMLOCK = 0x90,
  // SCROLL = 0x91,
  // LSHIFT = 0xA0,
  // RSHIFT = 0xA1,
  // LCONTROL = 0xA2,
  // RCONTROL = 0xA3,
  // LMENU = 0xA4,
  // RMENU = 0xA5,
  // BROWSER_BACK = 0xA6,
  // BROWSER_FORWARD = 0xA7,
  // BROWSER_REFRESH = 0xA8,
  // BROWSER_STOP = 0xA9,
  // BROWSER_SEARCH = 0xAA,
  // BROWSER_FAVORITES = 0xAB,
  // BROWSER_HOME = 0xAC,
  // VOLUME_MUTE = 0xAD,
  // VOLUME_DOWN = 0xAE,
  // VOLUME_UP = 0xAF,
  // MEDIA_NEXT_TRACK = 0xB0,
  // MEDIA_PREV_TRACK = 0xB1,
  // MEDIA_STOP = 0xB2,
  // MEDIA_PLAY_PAUSE = 0xB3,
  // MEDIA_LAUNCH_MAIL = 0xB4,
  // MEDIA_LAUNCH_MEDIA_SELECT = 0xB5,
  // MEDIA_LAUNCH_APP1 = 0xB6,
  // MEDIA_LAUNCH_APP2 = 0xB7,
  // OEM_1 = 0xBA,
  // OEM_PLUS = 0xBB,
  // OEM_COMMA = 0xBC,
  // OEM_MINUS = 0xBD,
  // OEM_PERIOD = 0xBE,
  // OEM_2 = 0xBF,
  // OEM_3 = 0xC0,
  // OEM_4 = 0xDB,
  // OEM_5 = 0xDC,
  // OEM_6 = 0xDD,
  // OEM_7 = 0xDE,
  // OEM_8 = 0xDF,
  // OEM_102 = 0xE2,
  // PROCESSKEY = 0xE5,
  // PACKET = 0xE7,
  // DBE_SBCSCHAR = 0xF3,
  // DBE_DBCSCHAR = 0xF4,
  // ATTN = 0xF6,
  // CRSEL = 0xF7,
  // EXSEL = 0xF8,
  // EREOF = 0xF9,
  // PLAY = 0xFA,
  // ZOOM = 0xFB,
  // NONAME = 0xFC,
  // PA1 = 0xFD,
  // OEM_CLEAR = 0xFE,
  // UNKNOWN = 0,
  [MOJO_KEY(ALTGR)] = SDL_NUM_SCANCODES,
};

/* Mojo SDL video driver implementation
*/
#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../../events/SDL_events_c.h"

#include "SDL_mojoasyncwaiter.h"
#include "SDL_mojoevents_c.h"
#include "SDL_mojoopengles.h"
#include "SDL_mojovideo.h"
#include "SDL_mojowindow.h"

#include "base/bind.h"
#include "base/logging.h"

#define MOJO_VID_DRIVER_NAME "Mojo"

namespace sdl {

/* Initialization/Query functions */
static int Mojo_VideoInit(_THIS);
static void Mojo_VideoQuit(_THIS);

/* Mojo driver bootstrap functions */

static int
Mojo_Available(void)
{
    return 1;
}

static void
Mojo_DeleteDevice(SDL_VideoDevice * device)
{
    SDL_free(device);
}

static SDL_VideoDevice *
Mojo_CreateDevice(int devindex)
{
    SDL_VideoDevice *device;

    /* Initialize all variables that we clean on shutdown */
    device = (SDL_VideoDevice *) SDL_calloc(1, sizeof(SDL_VideoDevice));
    if (!device) {
        SDL_OutOfMemory();
        return NULL;
    }

    /* Set the function pointers */
    device->VideoInit = Mojo_VideoInit;
    device->VideoQuit = Mojo_VideoQuit;
    device->PumpEvents = sdl::Mojo_PumpEvents;


    device->CreateWindow = sdl::Mojo_CreateWindow;
    device->SetWindowSize = sdl::Mojo_SetWindowSize;
    device->DestroyWindow = sdl::Mojo_DestroyWindow;

    device->free = Mojo_DeleteDevice;

    /* GL pointers */
    device->GL_LoadLibrary = sdl::Mojo_GLES2_LoadLibrary;
    device->GL_CreateContext = sdl::Mojo_GLES2_CreateContext;
    device->GL_MakeCurrent = sdl::Mojo_GLES2_MakeCurrent;
    device->GL_SwapWindow = sdl::Mojo_GLES2_SwapWindow;
    device->GL_DeleteContext = sdl::Mojo_GLES2_DeleteContext;

    return device;
}

int
Mojo_VideoInit(_THIS)
{
    SDL_DisplayMode mode;

    /* TODO(jaween): Query physical display properties and use them here */
    mode.format = SDL_PIXELFORMAT_RGB888;
    mode.w = 0;
    mode.h = 0;
    mode.refresh_rate = 0;
    mode.driverdata = NULL;
    if (SDL_AddBasicVideoDisplay(&mode) < 0) {
        return -1;
    }

    SDL_AddDisplayMode(&_this->displays[0], &mode);

    /* We're done! */
    return 0;
}

void
Mojo_VideoQuit(_THIS)
{
    Mojo_DeallocateEvents();
}

static SDL_Scancode mojo_to_sdl(int mojo_key_code) {
    return kMojoKeymap[mojo_key_code];
}

void
Mojo_HandleInputEvent(const mojo::Event& mojo_event)
{
    int button;
    if (mojo_event.flags == mojo::EventFlags::LEFT_MOUSE_BUTTON) {
        button = SDL_BUTTON_LEFT;
    } else if (mojo_event.flags == mojo::EventFlags::RIGHT_MOUSE_BUTTON) {
        button = SDL_BUTTON_RIGHT;
    } else if (mojo_event.flags == mojo::EventFlags::MIDDLE_MOUSE_BUTTON) {
        button = SDL_BUTTON_MIDDLE;
    }

    /* TODO(jaween): Use mojo_event.pointer_data->kind to determine when to
     * send SDL_TouchFingerEvents 
     */
    SDL_Event* event = Mojo_RetrieveEvent();
    switch (mojo_event.action) {
        case mojo::EventType::KEY_PRESSED:
        case mojo::EventType::KEY_RELEASED:
            if (mojo_event.key_data->is_char) {
              // drop char events, keep keypress events
              return;
            }
            event->type = (mojo_event.action == mojo::EventType::KEY_PRESSED) ?
                SDL_KEYDOWN : SDL_KEYUP;
            event->key.windowID = Mojo_GetWindowId();
            event->key.keysym.scancode = mojo_to_sdl(mojo_event.key_data->key_code);
            event->key.state = (mojo_event.action == mojo::EventType::KEY_PRESSED) ?
                SDL_PRESSED : SDL_RELEASED;
            break;

        case mojo::EventType::POINTER_DOWN:
            event->type = SDL_MOUSEBUTTONDOWN;
            event->button.windowID = Mojo_GetWindowId();
            event->button.which = 0;
            event->button.button = button;
            event->button.state = SDL_PRESSED;
            event->button.x = mojo_event.pointer_data->x;
            event->button.y = mojo_event.pointer_data->y;
            break;
        case mojo::EventType::POINTER_MOVE:
            event->type = SDL_MOUSEMOTION;
            event->motion.windowID = Mojo_GetWindowId();
            event->motion.which = 0;
            event->motion.x = mojo_event.pointer_data->x;
            event->motion.y = mojo_event.pointer_data->y;
            break;
        case mojo::EventType::POINTER_UP:
            event->type = SDL_MOUSEBUTTONUP;
            event->button.windowID = Mojo_GetWindowId();
            event->button.which = 0;
            event->button.button = button;
            event->button.state = SDL_RELEASED;
            event->button.x = mojo_event.pointer_data->x;
            event->button.y = mojo_event.pointer_data->y;
            break;
        case mojo::EventType::UNKNOWN:
        case mojo::EventType::POINTER_CANCEL:
            /* SDL doesn't support cancel */
        default:
            Mojo_RelinquishEvent(event);
            return;
    }
    Mojo_EnqueueEvent(event);
}

} /* namespace sdl */

VideoBootStrap Mojo_bootstrap = {
    MOJO_VID_DRIVER_NAME, "SDL Mojo video driver",
    sdl::Mojo_Available, sdl::Mojo_CreateDevice
};

#endif /* SDL_VIDEO_DRIVER_MOJO */

/* vi: set ts=4 sw=4 expandtab: */