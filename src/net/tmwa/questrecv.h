/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
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

#ifndef NET_TMWA_QUESTRECV_H
#define NET_TMWA_QUESTRECV_H

namespace Net
{
    class MessageIn;
}  // namespace Net

namespace TmwAthena
{
    namespace QuestRecv
    {
        void processSetQuestVar(Net::MessageIn &msg);
        void processPlayerQuests(Net::MessageIn &msg);
    }  // namespace QuestRecv
}  // namespace TmwAthena

#endif  // NET_TMWA_QUESTRECV_H
