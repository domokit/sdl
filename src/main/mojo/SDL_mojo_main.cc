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

#ifdef SDL_VIDEO_DRIVER_MOJO

/* Include the SDL main definition header */
#include "SDL_main.h"

#include "../../video/mojo/SDL_mojoasyncwaiter.h"
#include "../../video/mojo/SDL_mojovideo.h"
#include "SDL_mojo_main.h"

namespace sdl {

SdlAppDelegate::SdlAppDelegate()
{
    /* Needed for MOJO_DISALLOW_COPY_AND_ASSIGN */
}

void 
SdlAppDelegate::Initialize(mojo::ApplicationImpl* app)
{
    Mojo_SetApplicationImpl(app);

    /* TODO(jaween): Forward the appropriate argc and argv */
    SDL_SetMainReady();
    SDL_main(0, NULL);
}

} /* namespace sdl */

MojoResult
MojoMain(MojoHandle application_request)
{
    mojo::ApplicationRunner runner(new sdl::SdlAppDelegate);
    return runner.Run(application_request);
}

#endif /* __MOJO__ */

/* vi: set ts=4 sw=4 expandtab: */
