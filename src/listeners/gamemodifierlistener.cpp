/*
 *  The ManaPlus Client
 *  Copyright (C) 2014-2019  The ManaPlus Developers
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

#include "listeners/gamemodifierlistener.h"

#include "utils/foreach.h"

#include "debug.h"

defineListener(GameModifierListener)

void GameModifierListener::distributeEvent()
{
    FOR_EACH (STD_VECTOR<GameModifierListener*>::iterator,
              it, mListeners)
    {
        GameModifierListener *const listener = *it;
        if (listener != nullptr)
            listener->gameModifiersChanged();
    }
}
