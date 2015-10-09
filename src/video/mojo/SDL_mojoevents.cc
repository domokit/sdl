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

#include "../../events/SDL_events_c.h"
#include "../../events/SDL_sysevents.h"
#include "SDL.h"

#include "SDL_mojoevents_c.h"
#include "SDL_mojovideo.h"

#include "mojo/public/cpp/utility/run_loop.h"

#include <queue>

static std::queue<SDL_Event*> event_queue;
static std::queue<SDL_Event*> unused_events;

namespace sdl {

void
Mojo_PumpEvents(_THIS)
{
    /* Allows the RunLoop to process any outstanding waits, then adds 
     * SDL_Events to the event queue in their respective callbacks
     */
    mojo::RunLoop* run_loop = mojo::RunLoop::current();
    assert(run_loop);
    run_loop->RunUntilIdle();

    while (event_queue.size() > 0) {
        SDL_Event* event = event_queue.front();
        event_queue.pop();

        switch (event->type) {
            case SDL_WINDOWEVENT:
                switch (event->window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                        SDL_SendWindowEvent(
                            SDL_GetWindowFromID(event->window.windowID),
                            SDL_WINDOWEVENT_RESIZED, event->window.data1,
                            event->window.data2);
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                SDL_SendMouseButton(
                        SDL_GetWindowFromID(event->button.windowID),
                        event->button.which, event->button.state,
                        event->button.button);
                break;
            case SDL_MOUSEMOTION:
                bool relative_coords = false;
                SDL_SendMouseMotion(
                        SDL_GetWindowFromID(event->motion.windowID),
                        event->motion.which, relative_coords, event->motion.x,
                        event->motion.y);
                break;
        }
        Mojo_RelinquishEvent(event);
    }
}

void
Mojo_EnqueueEvent(SDL_Event* event)
{
    event_queue.push(event);
}

/*
 * Reuses an existing SDL event, allocates one if none available 
 */
SDL_Event*
Mojo_RetrieveEvent()
{
    SDL_Event* event = NULL;
    if (unused_events.size() > 0) {
        event = unused_events.front();
        unused_events.pop();
    } else {
        event = new SDL_Event;
    }
    return event;
}

/*
 * Returns an SDL_Event to the queue of unused events for reuse
 */
void
Mojo_RelinquishEvent(SDL_Event* event)
{
    unused_events.push(event);
}

void
Mojo_DeallocateEvents()
{
    while (event_queue.size() > 0) {
        SDL_Event* event = event_queue.front();
        event_queue.pop();
        delete event;
    }

    while (unused_events.size() > 0) {
        SDL_Event* event = unused_events.front();
        unused_events.pop();
        delete event;
    }
}

} /* namespace sdl */

#endif /* SDL_VIDEO_DRIVER_MOJO */

/* vi: set ts=4 sw=4 expandtab: */
