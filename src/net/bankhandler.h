/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef NET_BANKHANDLER_H
#define NET_BANKHANDLER_H

#include "localconsts.h"

namespace Net
{

class BankHandler notfinal
{
    public:
        BankHandler()
        { }

        A_DELETE_COPY(BankHandler)

        virtual ~BankHandler()
        { }

        virtual void deposit(const int money) const = 0;

        virtual void withdraw(const int money) const = 0;

        virtual void check() const = 0;

        virtual void open() const = 0;

        virtual void close() const = 0;
};

}  // namespace Net

extern Net::BankHandler *bankHandler;

#endif  // NET_BANKHANDLER_H
