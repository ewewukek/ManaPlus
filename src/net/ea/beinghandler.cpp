/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "net/ea/beinghandler.h"

#include "actormanager.h"
#include "configuration.h"
#include "effectmanager.h"
#include "game.h"
#include "guildmanager.h"
#include "party.h"

#include "being/localplayer.h"
#include "being/playerrelations.h"

#include "particle/particle.h"

#include "gui/windows/botcheckerwindow.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/killstats.h"

#include "resources/map/map.h"

#include "debug.h"

namespace Ea
{
BeingHandler::BeingHandler(const bool enableSync) :
    mSync(enableSync),
    mSpawnId(0),
    mHideShield(config.getBoolValue("hideShield"))
{
}

Being *BeingHandler::createBeing(const int id, const int16_t job) const
{
    if (!actorManager)
        return nullptr;

    ActorType::Type type = ActorType::Unknown;
    if (job <= 25 || (job >= 4001 && job <= 4049))
        type = ActorType::Player;
    else if (job >= 46 && job <= 1000)
        type = ActorType::Npc;
    else if (job > 1000 && job <= 2000)
        type = ActorType::Monster;
    else if (job == 45)
        type = ActorType::Portal;

    Being *const being = actorManager->createBeing(id, type, job);
    return being;
}

void BeingHandler::setSprite(Being *const being, const unsigned int slot,
                             const int id, const std::string &color,
                             const unsigned char colorId,
                             const bool isWeapon,
                             const bool isTempSprite) const
{
    if (!being)
        return;
    being->updateSprite(slot, id, color, colorId, isWeapon, isTempSprite);
}

void BeingHandler::processBeingRemove(Net::MessageIn &msg) const
{
    BLOCK_START("BeingHandler::processBeingRemove")
    if (!actorManager || !localPlayer)
    {
        BLOCK_END("BeingHandler::processBeingRemove")
        return;
    }

    // A being should be removed or has died

    const int id = msg.readInt32("being id");
    Being *const dstBeing = actorManager->findBeing(id);
    if (!dstBeing)
    {
        msg.readUInt8("dead flag?");
        BLOCK_END("BeingHandler::processBeingRemove")
        return;
    }

    localPlayer->followMoveTo(dstBeing, localPlayer->getNextDestX(),
        localPlayer->getNextDestY());

    // If this is player's current target, clear it.
    if (dstBeing == localPlayer->getTarget())
        localPlayer->stopAttack(true);

    if (msg.readUInt8("dead flag?") == 1U)
    {
        if (dstBeing->getCurrentAction() != BeingAction::DEAD)
        {
            dstBeing->setAction(BeingAction::DEAD, 0);
            dstBeing->recalcSpritesOrder();
        }
        if (dstBeing->getName() == "Jack O" && killStats)
            killStats->jackoDead(id);
    }
    else
    {
        if (dstBeing->getType() == ActorType::Player)
        {
            if (botCheckerWindow)
                botCheckerWindow->updateList();
            if (socialWindow)
                socialWindow->updateActiveList();
        }
        actorManager->destroy(dstBeing);
    }
    BLOCK_END("BeingHandler::processBeingRemove")
}

void BeingHandler::processSkillDamage(Net::MessageIn &msg) const
{
    BLOCK_START("BeingHandler::processSkillDamage")
    if (!actorManager)
    {
        BLOCK_END("BeingHandler::processSkillDamage")
        return;
    }

    const int id = msg.readInt16("skill id");
    Being *const srcBeing = actorManager->findBeing(
        msg.readInt32("src being id"));
    Being *const dstBeing = actorManager->findBeing(
        msg.readInt32("dst being id"));
    msg.readInt32("tick");
    msg.readInt32("src speed");
    msg.readInt32("dst speed");
    const int param1 = msg.readInt32("damage");
    const int level = msg.readInt16("skill level");
    msg.readInt16("div");
    msg.readUInt8("skill hit/type?");
    if (srcBeing)
        srcBeing->handleSkill(dstBeing, param1, id, level);
    if (dstBeing)
        dstBeing->takeDamage(srcBeing, param1, Being::SKILL, id);
    BLOCK_END("BeingHandler::processSkillDamage")
}

void BeingHandler::processBeingAction(Net::MessageIn &msg) const
{
    BLOCK_START("BeingHandler::processBeingAction")
    if (!actorManager)
    {
        BLOCK_END("BeingHandler::processBeingAction")
        return;
    }

    Being *const srcBeing = actorManager->findBeing(
        msg.readInt32("src being id"));
    Being *const dstBeing = actorManager->findBeing(
        msg.readInt32("dst being id"));

    msg.readInt32("tick");
    const int srcSpeed = msg.readInt32("src speed");
    msg.readInt32("dst speed");
    const int param1 = msg.readInt16("param1");
    msg.readInt16("param 2");
    const uint8_t type = msg.readUInt8("type");
    msg.readInt16("param 3");

    switch (type)
    {
        case Being::HIT:  // Damage
        case Being::CRITICAL:  // Critical Damage
        case Being::MULTI:  // Critical Damage
        case Being::REFLECT:  // Reflected Damage
        case Being::FLEE:  // Lucky Dodge
            if (srcBeing)
            {
                if (srcSpeed && srcBeing->getType() == ActorType::Player)
                    srcBeing->setAttackDelay(srcSpeed);
                // attackid=1, type
                srcBeing->handleAttack(dstBeing, param1, 1);
                if (srcBeing->getType() == ActorType::Player)
                    srcBeing->setAttackTime();
            }
            if (dstBeing)
            {
                dstBeing->takeDamage(srcBeing, param1,
                    static_cast<Being::AttackType>(type));
            }
            break;

        case 0x01:  // dead or pickup?
            break;
            // tmw server can send here garbage?
//            if (srcBeing)
//                srcBeing->setAction(BeingAction::DEAD, 0);

        case 0x02:  // Sit
            if (srcBeing)
            {
                srcBeing->setAction(BeingAction::SIT, 0);
                if (srcBeing->getType() == ActorType::Player)
                {
                    srcBeing->setMoveTime();
                    if (localPlayer)
                        localPlayer->imitateAction(srcBeing, BeingAction::SIT);
                }
            }
            break;

        case 0x03:  // Stand up
            if (srcBeing)
            {
                srcBeing->setAction(BeingAction::STAND, 0);
                if (srcBeing->getType() == ActorType::Player)
                {
                    srcBeing->setMoveTime();
                    if (localPlayer)
                    {
                        localPlayer->imitateAction(srcBeing,
                            BeingAction::STAND);
                    }
                }
            }
            break;
        default:
            logger->log("QQQ1 SMSG_BEING_ACTION:");
            if (srcBeing)
                logger->log("srcBeing:" + toString(srcBeing->getId()));
            if (dstBeing)
                logger->log("dstBeing:" + toString(dstBeing->getId()));
            logger->log("type: " + toString(type));
            break;
    }
    BLOCK_END("BeingHandler::processBeingAction")
}

void BeingHandler::processBeingSelfEffect(Net::MessageIn &msg) const
{
    BLOCK_START("BeingHandler::processBeingSelfEffect")
    if (!effectManager || !actorManager)
    {
        BLOCK_END("BeingHandler::processBeingSelfEffect")
        return;
    }

    const int id = static_cast<uint32_t>(msg.readInt32("being id"));
    Being *const being = actorManager->findBeing(id);
    if (!being)
    {
        BLOCK_END("BeingHandler::processBeingSelfEffect")
        return;
    }

    const int effectType = msg.readInt32("effect type");

    if (Particle::enabled)
        effectManager->trigger(effectType, being);

    // +++ need dehard code effectType == 3
    if (effectType == 3 && being->getType() == ActorType::Player
        && socialWindow)
    {   // reset received damage
        socialWindow->resetDamage(being->getName());
    }
    BLOCK_END("BeingHandler::processBeingSelfEffect")
}

void BeingHandler::processBeingEmotion(Net::MessageIn &msg) const
{
    BLOCK_START("BeingHandler::processBeingEmotion")
    if (!localPlayer || !actorManager)
    {
        BLOCK_END("BeingHandler::processBeingEmotion")
        return;
    }

    Being *const dstBeing = actorManager->findBeing(msg.readInt32("being id"));
    if (!dstBeing)
    {
        BLOCK_END("BeingHandler::processBeingEmotion")
        return;
    }

    if (player_relations.hasPermission(dstBeing, PlayerRelation::EMOTE))
    {
        const uint8_t emote = msg.readUInt8("emote");
        if (emote)
        {
            dstBeing->setEmote(emote, 0);
            localPlayer->imitateEmote(dstBeing, emote);
        }
    }
    if (dstBeing->getType() == ActorType::Player)
        dstBeing->setOtherTime();
    BLOCK_END("BeingHandler::processBeingEmotion")
}

void BeingHandler::processNameResponse(Net::MessageIn &msg) const
{
    BLOCK_START("BeingHandler::processNameResponse")
    if (!localPlayer || !actorManager)
    {
        BLOCK_END("BeingHandler::processNameResponse")
        return;
    }

    const int beingId = msg.readInt32("being id");
    Being *const dstBeing = actorManager->findBeing(beingId);

    if (dstBeing)
    {
        if (beingId == localPlayer->getId())
        {
            localPlayer->pingResponse();
        }
        else
        {
            dstBeing->setName(msg.readString(24, "name"));
            dstBeing->updateGuild();
            dstBeing->addToCache();

            if (dstBeing->getType() == ActorType::Player)
                dstBeing->updateColors();

            if (localPlayer)
            {
                const Party *const party = localPlayer->getParty();
                if (party && party->isMember(dstBeing->getId()))
                {
                    PartyMember *const member = party->getMember(
                        dstBeing->getId());

                    if (member)
                        member->setName(dstBeing->getName());
                }
                localPlayer->checkNewName(dstBeing);
            }
            BLOCK_END("BeingHandler::processNameResponse")
            return;
        }
    }
    msg.readString(24, "name");
    BLOCK_END("BeingHandler::processNameResponse")
}

void BeingHandler::processIpResponse(Net::MessageIn &msg) const
{
    BLOCK_START("BeingHandler::processIpResponse")
    if (!actorManager)
    {
        BLOCK_END("BeingHandler::processIpResponse")
        return;
    }

    Being *const dstBeing = actorManager->findBeing(msg.readInt32("being id"));
    if (dstBeing)
        dstBeing->setIp(ipToString(msg.readInt32("ip address")));
    BLOCK_END("BeingHandler::processIpResponse")
}

void BeingHandler::processPlayerStop(Net::MessageIn &msg) const
{
    BLOCK_START("BeingHandler::processPlayerStop")
    if (!actorManager || !localPlayer)
    {
        BLOCK_END("BeingHandler::processPlayerStop")
        return;
    }

    const int id = msg.readInt32("account id");

    if (mSync || id != localPlayer->getId())
    {
        Being *const dstBeing = actorManager->findBeing(id);
        if (dstBeing)
        {
            const uint16_t x = msg.readInt16("x");
            const uint16_t y = msg.readInt16("y");
            dstBeing->setTileCoords(x, y);
            if (dstBeing->getCurrentAction() == BeingAction::MOVE)
                dstBeing->setAction(BeingAction::STAND, 0);
            BLOCK_END("BeingHandler::processPlayerStop")
            return;
        }
    }
    msg.readInt16("x");
    msg.readInt16("y");
    BLOCK_END("BeingHandler::processPlayerStop")
}

void BeingHandler::processPlayerMoveToAttack(Net::MessageIn &msg A_UNUSED)
                                             const
{
    BLOCK_START("BeingHandler::processPlayerStop")
    /*
      * This is an *advisory* message, telling the client that
      * it needs to move the character before attacking
      * a target (out of range, obstruction in line of fire).
      * We can safely ignore this...
      */
    if (localPlayer)
        localPlayer->fixAttackTarget();
    BLOCK_END("BeingHandler::processPlayerStop")
}

void BeingHandler::processSkillNoDamage(Net::MessageIn &msg) const
{
    msg.readInt16("skill id");
    msg.readInt16("heal");
    msg.readInt32("dst id");
    msg.readInt32("src id");
    msg.readUInt8("fail");
}

void BeingHandler::processPvpMapMode(Net::MessageIn &msg) const
{
    BLOCK_START("BeingHandler::processPvpMapMode")
    const Game *const game = Game::instance();
    if (!game)
    {
        BLOCK_END("BeingHandler::processPvpMapMode")
        return;
    }

    Map *const map = game->getCurrentMap();
    if (map)
        map->setPvpMode(msg.readInt16("pvp mode"));
    BLOCK_END("BeingHandler::processPvpMapMode")
}

void BeingHandler::processPvpSet(Net::MessageIn &msg) const
{
    BLOCK_START("BeingHandler::processPvpSet")
    const int id = msg.readInt32("being id");
    const int rank = msg.readInt32("rank");
    msg.readInt32("num");
    if (actorManager)
    {
        Being *const dstBeing = actorManager->findBeing(id);
        if (dstBeing)
            dstBeing->setPvpRank(rank);
    }
    BLOCK_END("BeingHandler::processPvpSet")
}

}  // namespace Ea
