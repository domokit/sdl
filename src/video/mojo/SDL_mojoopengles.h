/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2015 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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

#ifndef _SDL_mojoopengles_h
#define _SDL_mojoopengles_h

#include "../../main/mojo/SDL_mojo_main.h"

#include "mojo/common/binding_set.h"
#include "mojo/public/c/gpu/MGL/mgl.h"
#include "mojo/public/c/gpu/MGL/mgl_onscreen.h"
#include "mojo/services/native_viewport/public/interfaces/native_viewport.mojom.h"

namespace sdl {

/* SDL function implementations */
extern int Mojo_GLES2_LoadLibrary(_THIS, const char *path);
extern SDL_GLContext Mojo_GLES2_CreateContext(_THIS, SDL_Window * window);
extern int Mojo_GLES2_MakeCurrent(_THIS, SDL_Window * window, SDL_GLContext context);
extern void Mojo_GLES2_SwapWindow(_THIS, SDL_Window * window);
extern void Mojo_GLES2_DeleteContext(_THIS, SDL_GLContext context);

/* Internal helper functions */
extern bool Mojo_HasOpenGLContext();
extern void Mojo_SetAppDelegate(sdl::SdlAppDelegate* passed_app_delegate);
static void Mojo_ContextCreated(mojo::CommandBufferPtr command_buffer);
static void Mojo_ContextLostThunk(void* closure);

} /* namespace sdl */

#endif /* _SDL_mojoopengles_h */

/* vi: set ts=4 sw=4 expandtab: */
