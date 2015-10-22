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
#include "mojo/services/input_events/interfaces/input_key_codes.mojom.h"

#if SDL_VIDEO_DRIVER_MOJO

#define MOJO_KEY(x) static_cast<int>(mojo::KeyboardCode::x)

static SDL_Scancode kMojoKeymap[SDL_NUM_SCANCODES];

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

    kMojoKeymap[MOJO_KEY(BACK)] = SDL_SCANCODE_BACKSPACE;
    kMojoKeymap[MOJO_KEY(RETURN)] = SDL_SCANCODE_RETURN;
    kMojoKeymap[MOJO_KEY(ESCAPE)] = SDL_SCANCODE_ESCAPE;
    kMojoKeymap[MOJO_KEY(SPACE)] = SDL_SCANCODE_SPACE;
    kMojoKeymap[MOJO_KEY(LEFT)] = SDL_SCANCODE_LEFT;
    kMojoKeymap[MOJO_KEY(UP)] = SDL_SCANCODE_UP;
    kMojoKeymap[MOJO_KEY(RIGHT)] = SDL_SCANCODE_RIGHT;
    kMojoKeymap[MOJO_KEY(DOWN)] = SDL_SCANCODE_DOWN;
    kMojoKeymap[MOJO_KEY(A)] = SDL_SCANCODE_A;
    kMojoKeymap[MOJO_KEY(B)] = SDL_SCANCODE_B;
    kMojoKeymap[MOJO_KEY(C)] = SDL_SCANCODE_C;
    kMojoKeymap[MOJO_KEY(D)] = SDL_SCANCODE_D;
    kMojoKeymap[MOJO_KEY(E)] = SDL_SCANCODE_E;
    kMojoKeymap[MOJO_KEY(F)] = SDL_SCANCODE_F;
    kMojoKeymap[MOJO_KEY(G)] = SDL_SCANCODE_G;
    kMojoKeymap[MOJO_KEY(H)] = SDL_SCANCODE_H;
    kMojoKeymap[MOJO_KEY(I)] = SDL_SCANCODE_I;
    kMojoKeymap[MOJO_KEY(J)] = SDL_SCANCODE_J;
    kMojoKeymap[MOJO_KEY(K)] = SDL_SCANCODE_K;
    kMojoKeymap[MOJO_KEY(L)] = SDL_SCANCODE_L;
    kMojoKeymap[MOJO_KEY(M)] = SDL_SCANCODE_M;
    kMojoKeymap[MOJO_KEY(N)] = SDL_SCANCODE_N;
    kMojoKeymap[MOJO_KEY(O)] = SDL_SCANCODE_O;
    kMojoKeymap[MOJO_KEY(P)] = SDL_SCANCODE_P;
    kMojoKeymap[MOJO_KEY(Q)] = SDL_SCANCODE_Q;
    kMojoKeymap[MOJO_KEY(R)] = SDL_SCANCODE_R;
    kMojoKeymap[MOJO_KEY(S)] = SDL_SCANCODE_S;
    kMojoKeymap[MOJO_KEY(T)] = SDL_SCANCODE_T;
    kMojoKeymap[MOJO_KEY(U)] = SDL_SCANCODE_U;
    kMojoKeymap[MOJO_KEY(V)] = SDL_SCANCODE_V;
    kMojoKeymap[MOJO_KEY(W)] = SDL_SCANCODE_W;
    kMojoKeymap[MOJO_KEY(X)] = SDL_SCANCODE_X;
    kMojoKeymap[MOJO_KEY(Y)] = SDL_SCANCODE_Y;
    kMojoKeymap[MOJO_KEY(Z)] = SDL_SCANCODE_Z;

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