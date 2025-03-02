/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RENDER_SAFEOPENGLGRAPHICS_H
#define RENDER_SAFEOPENGLGRAPHICS_H

#if defined USE_OPENGL && !defined ANDROID && !defined(__SWITCH__)

#include "render/graphics.h"

#include "resources/fboinfo.h"

#ifdef ANDROID
#include <GLES/gl.h>
#include <GLES/glext.h>
#else  // ANDROID
#ifndef USE_SDL2
#define GL_GLEXT_PROTOTYPES 1
#endif  // USE_SDL2
PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
// ignore -Wredundant-decls for SDL 1.2
PRAGMA45(GCC diagnostic push)
PRAGMA45(GCC diagnostic ignored "-Wredundant-decls")
#include <SDL_opengl.h>
PRAGMA45(GCC diagnostic pop)
PRAGMA48(GCC diagnostic pop)
#if defined(__APPLE__)
#include <OpenGL/glext.h>
#elif !defined(__native_client__)
PRAGMA45(GCC diagnostic push)
PRAGMA45(GCC diagnostic ignored "-Wredundant-decls")
#include <GL/glext.h>
PRAGMA45(GCC diagnostic pop)
#endif  // defined(__APPLE__)
#endif  // ANDROID

class SafeOpenGLGraphics final : public Graphics
{
    public:
        SafeOpenGLGraphics();

        A_DELETE_COPY(SafeOpenGLGraphics)

        ~SafeOpenGLGraphics() override final;

        void testDraw() restrict2 override final;

        #include "render/graphicsdef.hpp"
        RENDER_GRAPHICSDEF_HPP

        #include "render/openglgraphicsdef.hpp"
        RENDER_OPENGLGRAPHICSDEF_HPP

        #include "render/openglgraphicsdef1.hpp"
        RENDER_OPENGLGRAPHICSDEF1_HPP

    private:
        bool mTexture;
        bool mIsByteColor;
        Color mByteColor;
        float mFloatColor;
        bool mColorAlpha;
        FBOInfo mFbo;
};

#endif  // defined USE_OPENGL && !defined ANDROID
#endif  // RENDER_SAFEOPENGLGRAPHICS_H
