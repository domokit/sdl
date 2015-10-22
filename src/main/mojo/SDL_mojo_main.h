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
#ifndef _SDL_mojomain_h
#define _SDL_mojomain_h

#include "base/bind.h"
#include "mojo/common/binding_set.h"
#include "mojo/public/c/system/main.h"
#include "mojo/public/cpp/application/application_delegate.h"
#include "mojo/public/cpp/application/application_impl.h"
#include "mojo/public/cpp/application/application_runner.h"
#include "mojo/public/cpp/application/interface_factory.h"
#include "mojo/services/native_viewport/interfaces/native_viewport.mojom.h"

namespace sdl {

class SdlAppDelegate : public mojo::ApplicationDelegate,
                       public mojo::NativeViewportEventDispatcher
{
  public:
    SdlAppDelegate();
    virtual void Initialize(mojo::ApplicationImpl* app) override;
    virtual void OnEvent(mojo::EventPtr event,
                   const mojo::Callback<void()>& callback) override;

  private:
    mojo::Binding<mojo::NativeViewportEventDispatcher> dispatcher_binding_;

    DISALLOW_COPY_AND_ASSIGN(SdlAppDelegate);
};

} /* namespace sdl */

#endif /* _SDL_mojomain_h */

/* vi: set ts=4 sw=4 expandtab: */
