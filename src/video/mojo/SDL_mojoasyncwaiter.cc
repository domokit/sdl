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
#include "SDL_mojoasyncwaiter.h"

#include <assert.h>

#include "base/logging.h"
#include "mojo/public/c/environment/async_waiter.h"
#include "mojo/public/cpp/utility/run_loop.h"
#include "mojo/public/cpp/utility/run_loop_handler.h"

/*
 * Based on mojo/public/cpp/environment/lib/default_async_waiter.cc
 * Performs a synchronous wait on a handle by exiting RunLoop::Run()
 * only once the handle has been signaled.
 */

namespace sdl {

static MojoResult g_block_result;

namespace {

/*
 * RunLoopHandler implementation used for a request to AsyncWait(). There are
 * two ways RunLoopHandlerImpl is deleted:
 * . when the handle is ready (or errored).
 * . when CancelWait() is invoked.
 */

class RunLoopHandlerImpl : public mojo::RunLoopHandler
{
  public:
    RunLoopHandlerImpl(const mojo::Handle& handle,
                       MojoAsyncWaitCallback callback,
                       void* closure)
        : handle_(handle), callback_(callback), closure_(closure) {}

    ~RunLoopHandlerImpl() override
    {
        mojo::RunLoop::current()->RemoveHandler(handle_);
    }

    void
    OnHandleReady(const mojo::Handle& handle) override
    {
        NotifyCallback(MOJO_RESULT_OK);
    }

    void
    OnHandleError(const mojo::Handle& handle, MojoResult result) override
    {
        NotifyCallback(result);
    }

 private:
    void
    NotifyCallback(MojoResult result)
    {
        g_block_result = result;

        /* Delete this to unregister the handle. That way if the callback
         * reregisters everything is ok.
         */
        MojoAsyncWaitCallback callback = callback_;
        void* closure = closure_;
        delete this;

        /* Ends the synchonous Mojo interface call */
        mojo::RunLoop* run_loop = mojo::RunLoop::current();
        assert(run_loop);
        run_loop->Quit();

        callback(closure, result);
    }

    const mojo::Handle handle_;
    MojoAsyncWaitCallback callback_;
    void* closure_;

    MOJO_DISALLOW_COPY_AND_ASSIGN(RunLoopHandlerImpl);
};

MojoAsyncWaitID
AsyncWait(MojoHandle handle,
          MojoHandleSignals signals,
          MojoDeadline deadline,
          MojoAsyncWaitCallback callback,
          void* closure)
{
    RunLoopHandlerImpl* run_loop_handler =
        new RunLoopHandlerImpl(mojo::Handle(handle), callback, closure);

    /* |run_loop_handler| is destroyed either when the handle is ready or if
     * CancelWait is invoked.
     */
    mojo::RunLoop* run_loop = mojo::RunLoop::current();
    assert(run_loop);
    MojoDeadline fixed_deadline = 1000000;  /* 1 second */
    run_loop->AddHandler(run_loop_handler, mojo::Handle(handle), signals,
        fixed_deadline);

    return reinterpret_cast<MojoAsyncWaitID>(run_loop_handler);
}

void
CancelWait(MojoAsyncWaitID wait_id)
{
    delete reinterpret_cast<RunLoopHandlerImpl*>(wait_id);
}

} /* namespace */

MojoResult
Mojo_BlockOnHandle()
{
    /* Executes Run() until the handle has been signaled, it then exits the
     * RunLoop. We call RunUnilIdle() to serivce any remaining handles being
     * waited on in the RunLoop
     */
    mojo::RunLoop* run_loop = mojo::RunLoop::current();
    assert(run_loop);
    run_loop->Run();
    run_loop->RunUntilIdle();

    return g_block_result;
}

namespace internal {

const MojoAsyncWaiter kSdlAsyncWaiter = {AsyncWait, CancelWait};

} /* namespace internal */

} /* namespace sdl */

/* vi: set ts=4 sw=4 expandtab: */
