/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2019  The ManaPlus Developers
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

#include "resources/map/walklayer.h"

#include "debug.h"

WalkLayer::WalkLayer(const int width, const int height) :
    Resource(),
    mWidth(width),
    mHeight(height),
    mTiles(new int[width * height])
{
    std::fill_n(mTiles, width * height, 0);
}

WalkLayer::~WalkLayer()
{
    delete [] mTiles;
}

int WalkLayer::getDataAt(const int x, const int y) const
{
    if (x < 0 || x >= mWidth || y < 0 || y >= mHeight)
        return 0;
    return mTiles[x + y * mWidth];
}

int WalkLayer::calcMemoryLocal() const
{
    return Resource::calcMemoryLocal() +
        static_cast<int>(sizeof(WalkLayer) +
        sizeof(int) * mWidth * mHeight);
}
