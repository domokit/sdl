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

#ifndef _SDL_mojowindow_h
#define _SDL_mojowindow_h

#include "../SDL_sysvideo.h"

#include "mojo/common/binding_set.h"
#include "mojo/services/native_viewport/interfaces/native_viewport.mojom.h"

namespace sdl {

/* SDL function implementations */
extern int Mojo_CreateWindow(_THIS, SDL_Window * window);
extern void Mojo_SetWindowSize(_THIS, SDL_Window * window);
extern void Mojo_DestroyWindow(_THIS, SDL_Window * window);

/* Internal helper functions */
extern void Mojo_SetApplicationImpl(
    mojo::ApplicationImpl* passed_application_impl);
extern void Mojo_SetEventDispatcher(
    mojo::Binding<mojo::NativeViewportEventDispatcher>* dispatcher_binding);
extern void Mojo_GetContextProvider(mojo::ContextProviderPtr* context_provider);
extern void Mojo_OnMetricsChanged(mojo::ViewportMetricsPtr metrics);
extern int Mojo_GetWindowId();

} /* namespace sdl */

#endif /* _SDL_mojowindow_h */

/* vi: set ts=4 sw=4 expandtab: */
