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
#include "SDL_video.h"

#include "SDL_mojoasyncwaiter.h"
#include "SDL_mojoopengles.h"
#include "SDL_mojowindow.h"

#include "base/logging.h"
#include "mojo/public/c/system/main.h"
#include "mojo/services/native_viewport/public/interfaces/native_viewport.mojom.h"

namespace sdl {

static sdl::SdlAppDelegate* app_delegate;
static mojo::ContextProviderPtr context_provider;
static MGLContext mgl_context = MGL_NO_CONTEXT;

int 
Mojo_GLES2_LoadLibrary(_THIS, const char* path)
{
    /* OpenGL is ready */
    return 0;
}

SDL_GLContext
Mojo_GLES2_CreateContext(_THIS, SDL_Window * window)
{
    Mojo_GetContextProvider(&context_provider);
    context_provider.Bind(context_provider.PassInterface(), &sdl::internal::kSdlAsyncWaiter);

    context_provider->Create(nullptr, 
                          [](mojo::CommandBufferPtr command_buffer) {
                                Mojo_ContextCreated(command_buffer.Pass());
                          });
    sdl::Mojo_BlockOnHandle();

    return reinterpret_cast<SDL_GLContext>(&mgl_context);
}

int 
Mojo_GLES2_MakeCurrent(_THIS, SDL_Window * window, SDL_GLContext context)
{
    MGLMakeCurrent(*reinterpret_cast<MGLContext*>(context));
    return 0; 
}

void 
Mojo_GLES2_SwapWindow(_THIS, SDL_Window * window)
{
    MGLSwapBuffers();
}

void 
Mojo_GLES2_DeleteContext(_THIS, SDL_GLContext context)
{
    MGLDestroyContext(*reinterpret_cast<MGLContext*>(context));
    mgl_context = MGL_NO_CONTEXT;
}

bool
Mojo_HasOpenGLContext()
{
    return (mgl_context != MGL_NO_CONTEXT);
}

/* AppDelegate needed for use with OpenGL Context creation */
void
Mojo_SetAppDelegate(sdl::SdlAppDelegate* passed_app_delegate)
{
    app_delegate = passed_app_delegate;
}

void
Mojo_ContextCreated(mojo::CommandBufferPtr command_buffer)
{
    mgl_context = MGLCreateContext(
        MGL_API_VERSION_GLES2,
        command_buffer.PassInterface().PassHandle().release().value(),
        MGL_NO_CONTEXT, &Mojo_ContextLostThunk, app_delegate, 
        &sdl::internal::kSdlAsyncWaiter);
    MGLMakeCurrent(mgl_context);
}

void 
Mojo_ContextLostThunk(void* closure)
{
    /* TODO(jaween): Replace with SDL_LogMessage() calls */
    LOG(ERROR) << "Mojo OpenGL Context lost";
}

} /* namespace sdl */

#endif /* SDL_VIDEO_DRIVER_MOJO */

/* vi: set ts=4 sw=4 expandtab: */
