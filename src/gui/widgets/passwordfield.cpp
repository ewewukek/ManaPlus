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

#include "gui/widgets/passwordfield.h"

#include "gui/skin.h"

#include "debug.h"

PasswordField::PasswordField(const Widget2 *const widget,
                             const std::string &text) :
    TextField(widget, text,
        LoseFocusOnTab_true, nullptr, std::string(), false),
    mPasswordChar(mSkin != nullptr ? CAST_8(
        mSkin->getOption("passwordChar", 42)) : CAST_8(42))
{
}

void PasswordField::draw(Graphics *const graphics)
{
    BLOCK_START("PasswordField::draw")
    // std::string uses cow, thus cheap copy
    const std::string original = mText;
    if (mPasswordChar != 0)
        mText.assign(mText.length(), mPasswordChar);
    else
        mText.clear();
    TextField::draw(graphics);
    mText = original;
    BLOCK_END("PasswordField::draw")
}

void PasswordField::safeDraw(Graphics *const graphics)
{
    PasswordField::draw(graphics);
}
