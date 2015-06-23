/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#ifndef GUI_WINDOWS_DEBUGWINDOW_H
#define GUI_WINDOWS_DEBUGWINDOW_H

#include "gui/widgets/window.h"

class MapDebugTab;
class NetDebugTab;
class TabbedArea;
class TargetDebugTab;

/**
 * The debug window.
 *
 * \ingroup Interface
 */
class DebugWindow final : public Window
{
    public:
        /**
         * Constructor.
         */
        DebugWindow();

        A_DELETE_COPY(DebugWindow)

        ~DebugWindow();

        void postInit() override final;

        /**
         * Logic (updates components' size and infos)
         */
        void slowLogic();

        void draw(Graphics *g) override final A_NONNULL(2);

        void safeDraw(Graphics *g) override final A_NONNULL(2);

        void setPing(int pingTime);

        void widgetResized(const Event &event) override final;

#ifdef USE_PROFILER
        void logicChildren();
#endif

    private:
        TabbedArea *mTabs;
        MapDebugTab *mMapWidget;
        TargetDebugTab *mTargetWidget;
        NetDebugTab *mNetWidget;
};

extern DebugWindow *debugWindow;

#endif  // GUI_WINDOWS_DEBUGWINDOW_H
