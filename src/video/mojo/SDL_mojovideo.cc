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

#if SDL_VIDEO_DRIVER_MOJO

/* Mojo SDL video driver implementation
*/
#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../../events/SDL_events_c.h"

#include "SDL_mojoasyncwaiter.h"
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

    device->CreateWindow = sdl::Mojo_CreateWindow;
    device->SetWindowSize = sdl::Mojo_SetWindowSize;
    device->DestroyWindow = sdl::Mojo_DestroyWindow;

    device->free = Mojo_DeleteDevice;

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
    /* Nothing to do */
}

} /* namespace sdl */

VideoBootStrap Mojo_bootstrap = {
    MOJO_VID_DRIVER_NAME, "SDL Mojo video driver",
    sdl::Mojo_Available, sdl::Mojo_CreateDevice
};

#endif /* SDL_VIDEO_DRIVER_MOJO */

/* vi: set ts=4 sw=4 expandtab: */