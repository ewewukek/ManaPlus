/*
 * This code provides a glue layer between PhysicsFS and Simple Directmedia
 *  Layer's (SDL) RWops i/o abstraction.
 *
 * License: this code is public domain. I make no warranty that it is useful,
 *  correct, harmless, or environmentally safe.
 *
 * This particular file may be used however you like, including copying it
 *  verbatim into a closed-source project, exploiting it commercially, and
 *  removing any trace of my name from the source (although I hope you won't
 *  do that). I welcome enhancements and corrections to this file, but I do
 *  not require you to send me patches if you make changes. This code has
 *  NO WARRANTY.
 *
 * Unless otherwise stated, the rest of PhysicsFS falls under the zlib license.
 *  Please see LICENSE.txt in the root of the source tree.
 *
 * SDL falls under the LGPL license. You can get SDL at http://www.libsdl.org/
 *
 *  This file was written by Ryan C. Gordon. (icculus@icculus.org).
 *
 *  Copyright (C) 2012-2017  The ManaPlus Developers
 */

#include "utils/physfsrwops.h"

#include "logger.h"

#include "utils/fuzzer.h"
#include "utils/physfscheckutils.h"

#include "debug.h"

#ifdef USE_SDL2
#define PHYSFSINT int64_t
#define PHYSFSSIZE size_t
#else  // USE_SDL2
#define PHYSFSINT int32_t
#define PHYSFSSIZE int
#endif  // USE_SDL2

#ifdef DUMP_LEAKED_RESOURCES
static int openedRWops = 0;
#endif  // DUMP_LEAKED_RESOURCES

static PHYSFSINT physfsrwops_seek(SDL_RWops *const rw, const PHYSFSINT offset,
                                  const int whence)
{
    if (!rw)
        return -1;
    PHYSFS_file *const handle = static_cast<PHYSFS_file *const>(
        rw->hidden.unknown.data1);
    PHYSFSINT pos = 0;

    if (whence == SEEK_SET)
    {
        pos = offset;
    } /* if */
    else if (whence == SEEK_CUR)
    {
        const PHYSFS_sint64 current = PHYSFS_tell(handle);
        if (current == -1)
        {
            logger->assertLog(
                "physfsrwops_seek: Can't find position in file: %s",
                PHYSFS_getLastError());
            return -1;
        } /* if */

        pos = CAST_S32(current);
        if (static_cast<PHYSFS_sint64>(pos) != current)
        {
            logger->assertLog("physfsrwops_seek: "
                "Can't fit current file position in an int!");
            return -1;
        } /* if */

        if (offset == 0)  /* this is a "tell" call. We're done. */
            return pos;

        pos += offset;
    } /* else if */
    else if (whence == SEEK_END)
    {
        const PHYSFS_sint64 len = PHYSFS_fileLength(handle);
        if (len == -1)
        {
            logger->assertLog("physfsrwops_seek:Can't find end of file: %s",
                PHYSFS_getLastError());
            return -1;
        } /* if */

        pos = static_cast<PHYSFSINT>(len);
        if (static_cast<PHYSFS_sint64>(pos) != len)
        {
            logger->assertLog("physfsrwops_seek: "
                "Can't fit end-of-file position in an int!");
            return -1;
        } /* if */

        pos += offset;
    } /* else if */
    else
    {
        logger->assertLog("physfsrwops_seek: Invalid 'whence' parameter.");
        return -1;
    } /* else */

    if (pos < 0)
    {
        logger->assertLog("physfsrwops_seek: "
            "Attempt to seek past start of file.");
        return -1;
    } /* if */

    if (!PHYSFS_seek(handle, static_cast<PHYSFS_uint64>(pos)))
    {
        logger->assertLog("physfsrwops_seek: seek error: %s",
            PHYSFS_getLastError());
        return -1;
    } /* if */

    return pos;
} /* physfsrwops_seek */

static PHYSFSSIZE physfsrwops_read(SDL_RWops *const rw,
                                   void *ptr,
                                   const PHYSFSSIZE size,
                                   const PHYSFSSIZE maxnum)
{
    if (!rw)
        return 0;
    PHYSFS_file *const handle = static_cast<PHYSFS_file *const>(
        rw->hidden.unknown.data1);
    const PHYSFS_sint64 rc = PHYSFS_read(handle, ptr,
        CAST_U32(size),
        CAST_U32(maxnum));
    if (rc != static_cast<PHYSFS_sint64>(maxnum))
    {
        if (!PHYSFS_eof(handle)) /* not EOF? Must be an error. */
        {
            logger->assertLog("physfsrwops_seek: read error: %s",
                PHYSFS_getLastError());
        }
    } /* if */

    return CAST_S32(rc);
} /* physfsrwops_read */

static PHYSFSSIZE physfsrwops_write(SDL_RWops *const rw, const void *ptr,
                                    const PHYSFSSIZE size,
                                    const PHYSFSSIZE num)
{
    if (!rw)
        return 0;
    PHYSFS_file *const handle = static_cast<PHYSFS_file *const>(
        rw->hidden.unknown.data1);
    const PHYSFS_sint64 rc = PHYSFS_write(handle, ptr,
        CAST_U32(size),
        CAST_U32(num));
    if (rc != static_cast<PHYSFS_sint64>(num))
    {
        logger->assertLog("physfsrwops_seek: write error: %s",
            PHYSFS_getLastError());
    }

    return CAST_S32(rc);
} /* physfsrwops_write */

static int physfsrwops_close(SDL_RWops *const rw)
{
    if (!rw)
        return 0;
    PHYSFS_file *const handle = static_cast<PHYSFS_file*>(
        rw->hidden.unknown.data1);
    if (!PHYSFS_close(handle))
    {
        logger->assertLog("physfsrwops_seek: close error: %s",
            PHYSFS_getLastError());
        return -1;
    } /* if */

    SDL_FreeRW(rw);
#ifdef DUMP_LEAKED_RESOURCES
    if (openedRWops <= 0)
        logger->assertLog("physfsrwops_seek: closing already closed RWops");
    openedRWops --;
#endif  // DUMP_LEAKED_RESOURCES
#ifdef DEBUG_PHYSFS
    FakePhysFSClose(rw);
#endif  // DEBUG_PHYSFS

    return 0;
} /* physfsrwops_close */

#ifdef USE_SDL2
static PHYSFSINT physfsrwops_size(SDL_RWops *const rw)
{
    PHYSFS_file *const handle = static_cast<PHYSFS_file*>(
        rw->hidden.unknown.data1);
    return PHYSFS_fileLength(handle);
} /* physfsrwops_size */
#endif  // USE_SDL2

static SDL_RWops *create_rwops(PHYSFS_file *const handle)
{
    SDL_RWops *retval = nullptr;

    if (!handle)
    {
        logger->assertLog("physfsrwops_seek: create rwops error: %s",
            PHYSFS_getLastError());
    }
    else
    {
        retval = SDL_AllocRW();
        if (retval)
        {
#ifdef USE_SDL2
            retval->size  = &physfsrwops_size;
#endif  // USE_SDL2

            retval->seek  = &physfsrwops_seek;
            retval->read  = &physfsrwops_read;
            retval->write = &physfsrwops_write;
            retval->close = &physfsrwops_close;
            retval->hidden.unknown.data1 = handle;
        } /* if */
#ifdef DUMP_LEAKED_RESOURCES
        openedRWops ++;
#endif  // DUMP_LEAKED_RESOURCES
    } /* else */

    return retval;
} /* create_rwops */

SDL_RWops *PHYSFSRWOPS_makeRWops(PHYSFS_file *const handle)
{
    SDL_RWops *retval = nullptr;
    if (!handle)
    {
        logger->assertLog("physfsrwops_seek: NULL pointer passed to "
            "PHYSFSRWOPS_makeRWops().");
    }
    else
    {
        retval = create_rwops(handle);
    }

    return retval;
} /* PHYSFSRWOPS_makeRWops */

#ifdef __APPLE__
static bool checkFilePath(const char *const fname)
{
    if (!fname || !*fname)
        return false;
    if (!VirtFs::exists(fname) || VirtFs::isDirectory(fname))
        return false;
    return true;
}
#endif  // __APPLE__

SDL_RWops *PHYSFSRWOPS_openRead(const char *const fname)
{
    BLOCK_START("PHYSFSRWOPS_openRead")
#ifdef __APPLE__
    if (!checkFilePath(fname))
        return nullptr;
#endif  // __APPLE__
#ifdef USE_FUZZER
    if (Fuzzer::conditionTerminate(fname))
        return nullptr;
#endif  // USE_FUZZER
#ifdef USE_PROFILER
    SDL_RWops *const ret = create_rwops(VirtFs::openRead(fname));
    BLOCK_END("PHYSFSRWOPS_openRead")
    return ret;
#else  // USE_PROFILER

    return create_rwops(VirtFs::openRead(fname));
#endif  // USE_PROFILER
} /* PHYSFSRWOPS_openRead */

SDL_RWops *PHYSFSRWOPS_openWrite(const char *const fname)
{
#ifdef __APPLE__
    if (!checkFilePath(fname))
        return nullptr;
#endif  // __APPLE__

    return create_rwops(VirtFs::openWrite(fname));
} /* PHYSFSRWOPS_openWrite */

SDL_RWops *PHYSFSRWOPS_openAppend(const char *const fname)
{
#ifdef __APPLE__
    if (!checkFilePath(fname))
        return nullptr;
#endif  // __APPLE__

    return create_rwops(VirtFs::openAppend(fname));
} /* PHYSFSRWOPS_openAppend */

#ifdef DUMP_LEAKED_RESOURCES
void reportRWops()
{
    if (openedRWops)
    {
        logger->assertLog("physfsrwops_seek: leaking RWops: %d",
            openedRWops);
    }
}
#endif  // DUMP_LEAKED_RESOURCES

/* end of physfsrwops.c ... */
