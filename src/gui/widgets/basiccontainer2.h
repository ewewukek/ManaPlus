/*
 *  The ManaPlus Client
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

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef GUI_WIDGETS_BASICCONTAINER2_H
#define GUI_WIDGETS_BASICCONTAINER2_H

#include "gui/widgets/basiccontainer.h"

#include "enums/simpletypes/opaque.h"

/**
  * An implementation of a container able to contain other widgets. A widget's
  * position in the container is relative to the container itself and not the screen.
  * A container is the most common widget to use as the Gui's top widget as makes the Gui
  * able to contain more than one widget.
  *
  * @see Gui::setTop
  */
class BasicContainer2 notfinal : public BasicContainer
{
    public:
        /**
          * Constructor. A container is opauqe as default, if you want a
          * none opaque container call setQpaque(false).
          *
          * @see setOpaque, isOpaque
          */
        explicit BasicContainer2(const Widget2 *const widget);

        /**
          * Destructor.
          */
        ~BasicContainer2() override;

        A_DEFAULT_COPY(BasicContainer2)

        /**
          * Sets the container to be opaque or not. If the container
          * is opaque its background will be drawn, if it's not opaque
          * its background will not be drawn, and thus making the container
          * completely transparent.
          *
          * NOTE: This is not the same as to set visibility. A non visible
          *       container will not itself nor will it draw its content.
          *
          * @param opaque True if the container should be opaque, false otherwise.
          * @see isOpaque
          */
        void setOpaque(Opaque opaque)
        { mOpaque = opaque; }

        /**
          * Checks if the container is opaque or not.
          *
          * @return True if the container is opaque, false otherwise.
          * @see setOpaque
          */
        bool isOpaque() const
        { return mOpaque == Opaque_true; }

        /**
          * Adds a widget to the container.
          *
          * @param widget The widget to add.
          * @see remove, clear
          */
        virtual void add(Widget *const widget);

        /**
          * Adds a widget to the container and also specifies the widget's
          * position in the container. The position is relative to the container
          * and not relative to the screen.
          *
          * @param widget The widget to add.
          * @param x The x coordinate for the widget.
          * @param y The y coordinate for the widget.
          * @see remove, clear
          */
        void addXY(Widget *const widget,
                   const int x, const int y);

        // Inherited from Widget

        void draw(Graphics *const graphics) override A_NONNULL(2);

        void safeDraw(Graphics *const graphics) override A_NONNULL(2);

    protected:
        /**
          * True if the container is opaque, false otherwise.
          */
        Opaque mOpaque;
};

#endif  // GUI_WIDGETS_BASICCONTAINER2_H
