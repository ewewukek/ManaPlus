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

#ifndef NET_TMWA_TRADEHANDLER_H
#define NET_TMWA_TRADEHANDLER_H

#include "net/ea/tradehandler.h"

namespace TmwAthena
{

class TradeHandler final : public Ea::TradeHandler
{
    public:
        TradeHandler();

        A_DELETE_COPY(TradeHandler)

        ~TradeHandler() override final;

        void request(const Being *const being) const override final;

        void respond(const bool accept) const override final;

        void addItem(const Item *const item,
                     const int amount) const override final;

        void setMoney(const int amount) const override final;

        void confirm() const override final;

        void finish() const override final;

        void cancel() const override final;
};

}  // namespace TmwAthena

#endif  // NET_TMWA_TRADEHANDLER_H
