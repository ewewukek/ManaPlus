/*
 *  The ManaPlus Client
 *  Copyright (C) 2007  Joshua Langley <joshlangley@optusnet.com.au>
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef GUI_SETUPACTIONDATA_H
#define GUI_SETUPACTIONDATA_H

#include "enums/input/inputaction.h"

#include <string>

#include "localconsts.h"

struct SetupActionData final
{
#ifdef ADVGCC
    SetupActionData(const std::string &name0,
                    const InputActionT actionId0,
                    const std::string &text0) :
        name(name0),
        actionId(actionId0),
        text(text0)
    { }

    A_DELETE_COPY(SetupActionData)
#endif  // ADVGCC

    std::string name;
    const InputActionT actionId;
    std::string text;
};

#endif  // GUI_SETUPACTIONDATA_H
