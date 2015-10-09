/*
        Simple DirectMedia Layer
        Copyright (C) 1997-2015 Sam Lantinga <slouken@libsdl.org>

        This software is provided 'as-is', without any express or implied
        warranty.        In no event will the authors be held liable for any damages
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

#include "../SDL_sysvideo.h"

#include "SDL_mojoasyncwaiter.h"
#include "SDL_mojoevents_c.h"
#include "SDL_mojoopengles.h"
#include "SDL_mojovideo.h"
#include "SDL_mojowindow.h"

#include "mojo/public/c/gpu/MGL/mgl.h"
#include "mojo/public/c/gpu/MGL/mgl_onscreen.h"

namespace sdl {

/* TODO(jaween): Support multiple viewports */
static mojo::ApplicationImpl* application_impl;
static mojo::NativeViewportPtr viewport;
static int window_id;

int
Mojo_CreateWindow(_THIS, SDL_Window * window)
{
    application_impl->ConnectToService("mojo:native_viewport_service",
        &viewport);

    /* Sets up synchronous Mojo interface calls */
    viewport.Bind(viewport.PassInterface(), &internal::kSdlAsyncWaiter);

    mojo::SizePtr size(mojo::Size::New());

    size->width = window->w;
    size->height = window->h;

    auto requested_configuration = mojo::SurfaceConfiguration::New();
    requested_configuration->depth_bits = 32;

    viewport->Create(
        size.Clone(),
        requested_configuration.Pass(),
        base::Bind(&Mojo_OnMetricsChanged));
    MojoResult result = sdl::Mojo_BlockOnHandle();

    if (result != MOJO_RESULT_OK) {
        return -1;
    }

    /* All windows must use OpenGL on Mojo */
    window->flags |= SDL_WINDOW_OPENGL;

    window_id = window->id;
    return 0;
}

void
Mojo_SetWindowSize(_THIS, SDL_Window * window)
{
    MGLResizeSurface(window->w, window->h);
}

void
Mojo_DestroyWindow(_THIS, SDL_Window * window)
{
    viewport->Close();
    sdl::Mojo_BlockOnHandle();
}

void
Mojo_SetApplicationImpl(mojo::ApplicationImpl* passed_application_impl)
{
    application_impl = passed_application_impl;
}

void
Mojo_GetContextProvider(mojo::ContextProviderPtr* context_provider)
{
    viewport->GetContextProvider(GetProxy(context_provider));
}

void
Mojo_OnMetricsChanged(mojo::ViewportMetricsPtr metrics)
{
    assert(metrics);
    viewport->RequestMetrics(base::Bind(&Mojo_OnMetricsChanged));
    
    if (Mojo_HasOpenGLContext()) {
        MGLResizeSurface(metrics->size->width, metrics->size->height);

        SDL_Event* event = Mojo_RetrieveEvent();
        event->type = SDL_WINDOWEVENT;
        event->window.windowID = window_id;
        event->window.event = SDL_WINDOWEVENT_RESIZED;
        event->window.data1 = metrics->size->width;
        event->window.data2 = metrics->size->height;
        Mojo_EnqueueEvent(event);
    }
}

int
Mojo_GetWindowId()
{
    return window_id;
}

} /* namespace sdl */

#endif /* SDL_VIDEO_DRIVER_MOJO */

/* vi: set ts=4 sw=4 expandtab: */
