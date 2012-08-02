/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "gui/widgets/slider.h"

#include "client.h"
#include "configuration.h"
#include "graphics.h"
#include "keyevent.h"
#include "keydata.h"

#include "gui/theme.h"

#include "resources/image.h"

#include "debug.h"

ImageRect Slider::buttons[2];
float Slider::mAlpha = 1.0;
int Slider::mInstances = 0;

static std::string const data[2] =
{
    "slider.xml",
    "slider_highlighted.xml"
};

Slider::Slider(double scaleEnd):
    gcn::Slider(scaleEnd),
    mHasMouse(false)
{
    init();
}

Slider::Slider(double scaleStart, double scaleEnd):
    gcn::Slider(scaleStart, scaleEnd),
    mHasMouse(false)
{
    init();
}

Slider::~Slider()
{
    mInstances--;
    if (mInstances == 0 && Theme::instance())
    {
        Theme *theme = Theme::instance();
        for (int mode = 0; mode < 2; mode ++)
            theme->unloadRect(buttons[mode]);
    }
}

void Slider::init()
{
    setFrameSize(0);

    // Load resources
    if (mInstances == 0)
    {
        if (Theme::instance())
        {
            for (int mode = 0; mode < 2; mode ++)
            {
                Theme::instance()->loadRect(buttons[mode],
                    data[mode], "slider.xml", 0, 8);
            }
        }
        updateAlpha();
    }

    mInstances++;

    if (buttons[0].grid[HGRIP])
        setMarkerLength(buttons[0].grid[HGRIP]->getWidth());
}

void Slider::updateAlpha()
{
    float alpha = std::max(Client::getGuiAlpha(),
                           Theme::instance()->getMinimumOpacity());

    if (alpha != mAlpha)
    {
        mAlpha = alpha;
        for (int f = 0; f < 2; f ++)
        {
            for (int d = 0; d < SLIDER_MAX; d ++)
            {
                if (buttons[f].grid[d])
                    buttons[f].grid[d]->setAlpha(mAlpha);
            }
        }
    }
}

void Slider::draw(gcn::Graphics *graphics)
{
    if (!buttons[0].grid[HSTART] || !buttons[1].grid[HSTART]
        || !buttons[0].grid[HEND])
    {
        return;
    }

    int w = getWidth();
    int h = getHeight();
    int x = 0;
    int y = mHasMouse ? (h - buttons[1].grid[HSTART]->getHeight()) / 2 :
        (h - buttons[0].grid[HSTART]->getHeight()) / 2;

    updateAlpha();

    if (!mHasMouse)
    {
        static_cast<Graphics*>(graphics)->drawImage(
            buttons[0].grid[HSTART], x, y);

        const int width = buttons[0].grid[HSTART]->getWidth();
        w -= width + buttons[0].grid[HEND]->getWidth();
        x += width;

        if (buttons[0].grid[HMID])
        {
            const Image *hMid = buttons[0].grid[HMID];
            static_cast<Graphics*>(graphics)->
                drawImagePattern(hMid, x, y, w, hMid->getHeight());
        }

        x += w;
        static_cast<Graphics*>(graphics)->drawImage(
            buttons[0].grid[HEND], x, y);
    }
    else
    {
        static_cast<Graphics*>(graphics)->drawImage(
            buttons[1].grid[HSTART], x, y);

        const int width = buttons[1].grid[HSTART]->getWidth();
        w -= width;
        if (buttons[1].grid[HEND])
            w -= buttons[1].grid[HEND]->getWidth();
        x += width;

        if (buttons[1].grid[HMID])
        {
            const Image *hMid = buttons[1].grid[HMID];
            static_cast<Graphics*>(graphics)->drawImagePattern(
                hMid, x, y, w,
                hMid->getHeight());
        }

        x += w;
        if (buttons[1].grid[HEND])
        {
            static_cast<Graphics*>(graphics)->drawImage(
                buttons[1].grid[HEND], x, y);
        }
    }

    drawMarker(graphics);
}

void Slider::drawMarker(gcn::Graphics *graphics)
{
    const Image *img = (mHasMouse ? buttons[1].grid[HGRIP]
        : buttons[0].grid[HGRIP]);
    if (!img)
        return;

    static_cast<Graphics*>(graphics)->drawImage(img, getMarkerPosition(),
       (getHeight() - img->getHeight()) / 2);
}

void Slider::mouseEntered(gcn::MouseEvent& event A_UNUSED)
{
    mHasMouse = true;
}

void Slider::mouseExited(gcn::MouseEvent& event A_UNUSED)
{
    mHasMouse = false;
}

void Slider::keyPressed(gcn::KeyEvent& keyEvent)
{
    int action = static_cast<KeyEvent*>(&keyEvent)->getActionId();

    if (getOrientation() == HORIZONTAL)
    {
        if (action == Input::KEY_GUI_RIGHT)
        {
            setValue(getValue() + getStepLength());
            distributeActionEvent();
            keyEvent.consume();
        }
        else if (action == Input::KEY_GUI_LEFT)
        {
            setValue(getValue() - getStepLength());
            distributeActionEvent();
            keyEvent.consume();
        }
    }
    else
    {
        if (action == Input::KEY_GUI_UP)
        {
            setValue(getValue() + getStepLength());
            distributeActionEvent();
            keyEvent.consume();
        }
        else if (action == Input::KEY_GUI_DOWN)
        {
            setValue(getValue() - getStepLength());
            distributeActionEvent();
            keyEvent.consume();
        }
    }
}
