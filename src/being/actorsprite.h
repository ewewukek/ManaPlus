/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#ifndef BEING_ACTORSPRITE_H
#define BEING_ACTORSPRITE_H

#include "localconsts.h"
#include "map.h"

#include "being/actor.h"
#include "being/compoundsprite.h"

#include "particle/particlecontainer.h"

#include <set>
#include <list>

#include "localconsts.h"

class AnimatedSprite;
class StatusEffect;
class ActorSpriteListener;

class ActorSprite : public CompoundSprite, public Actor
{
public:
    enum Type
    {
        UNKNOWN = 0,
        PLAYER,
        NPC,
        MONSTER,
        FLOOR_ITEM,
        PORTAL,
        PET,
        AVATAR
    };

    enum TargetCursorSize
    {
        TC_SMALL = 0,
        TC_MEDIUM,
        TC_LARGE,
        NUM_TC
    };

    enum TargetCursorType
    {
        TCT_NONE = -1,
        TCT_NORMAL = 0,
        TCT_IN_RANGE,
        NUM_TCT
    };

    explicit ActorSprite(const int id);

    A_DELETE_COPY(ActorSprite)

    ~ActorSprite();

    int getId() const A_WARN_UNUSED
    { return mId; }

    void setId(const int id)
    { mId = id; }

    /**
     * Returns the type of the ActorSprite.
     */
    virtual Type getType() const A_WARN_UNUSED
    { return UNKNOWN; }

    virtual bool draw(Graphics *const graphics,
                      const int offsetX, const int offsetY) const override;

    virtual bool drawSpriteAt(Graphics *const graphics,
                              const int x, const int y) const;

    virtual void logic();

    static void actorLogic();

    void setMap(Map *const map) override;

    /**
     * Gets the way the object blocks pathfinding for other objects
     */
    virtual Map::BlockType getBlockType() const A_WARN_UNUSED
    { return Map::BLOCKTYPE_NONE; }

    /**
     * Take control of a particle.
     */
    void controlParticle(Particle *const particle);

    /**
     * Returns the required size of a target cursor for this being.
     */
    virtual TargetCursorSize getTargetCursorSize() const A_WARN_UNUSED
    { return TC_MEDIUM; }

    virtual int getTargetOffsetX() const A_WARN_UNUSED
    { return 0; }

    virtual int getTargetOffsetY() const A_WARN_UNUSED
    { return 0; }

    /**
     * Sets the target animation for this actor.
     */
    void setTargetType(const TargetCursorType type);

    /**
     * Untargets the actor.
     */
    void untarget()
    { mUsedTargetCursor = nullptr; }

    /**
     * Sets the actor's stun mode. If zero, the being is `normal', otherwise it
     * is `stunned' in some fashion.
     */
    void setStunMode(const uint16_t stunMode)
    {
        if (mStunMode != stunMode)
            updateStunMode(mStunMode, stunMode);
        mStunMode = stunMode;
    }

    void setStatusEffect(const int index, const bool active);

    /**
     * A status effect block is a 16 bit mask of status effects. We assign each
     * such flag a block ID of offset + bitnr.
     *
     * These are NOT the same as the status effect indices.
     */
    void setStatusEffectBlock(const int offset, const uint16_t flags);

    virtual void setAlpha(const float alpha) override
    { CompoundSprite::setAlpha(alpha); }

    virtual float getAlpha() const override A_WARN_UNUSED
    { return CompoundSprite::getAlpha(); }

    virtual int getWidth() const override A_WARN_UNUSED
    { return CompoundSprite::getWidth(); }

    virtual int getHeight() const override A_WARN_UNUSED
    { return CompoundSprite::getHeight(); }

    static void load();

    static void unload();

    /**
     * Add an ActorSprite listener.
     */
    void addActorSpriteListener(ActorSpriteListener *const listener);

    /**
     * Remove an ActorSprite listener.
     */
    void removeActorSpriteListener(ActorSpriteListener *const listener);

protected:
    /**
     * Notify self that the stun mode has been updated. Invoked by
     * setStunMode if something changed.
     */
    virtual void updateStunMode(const int oldMode, const int newMode);

    /**
     * Notify self that a status effect has flipped.
     * The new flag is passed.
     */
    virtual void updateStatusEffect(const int index, const bool newStatus);

    /**
     * Handle an update to a status or stun effect
     *
     * \param The StatusEffect to effect
     * \param effectId -1 for stun, otherwise the effect index
     */
    virtual void handleStatusEffect(StatusEffect *const effect,
                                    const int effectId);

    void setupSpriteDisplay(const SpriteDisplay &display,
                            const bool forceDisplay = true,
                            const int imageType = 0,
                            const std::string &color = "");

    std::set<int> mStatusEffects;   /**< set of active status effects */

    ParticleList mStunParticleEffects;
    ParticleVector mStatusParticleEffects;
    ParticleList mChildParticleEffects;
    int mId;
    uint16_t mStunMode;               /**< Stun mode; zero if not stunned */

private:
    /** Load the target cursors into memory */
    static void initTargetCursor();

    /** Remove the target cursors from memory */
    static void cleanupTargetCursors();

    /** Animated target cursors. */
    static AnimatedSprite *targetCursor[NUM_TCT][NUM_TC];

    static bool loaded;

    /** Target cursor being used */
    AnimatedSprite *mUsedTargetCursor;

    typedef std::list<ActorSpriteListener*> ActorSpriteListeners;
    typedef ActorSpriteListeners::iterator ActorSpriteListenerIterator;
    ActorSpriteListeners mActorSpriteListeners;

    int mCursorPaddingX;
    int mCursorPaddingY;

    /** Reset particle status effects on next redraw? */
    bool mMustResetParticles;
};

#endif  // BEING_ACTORSPRITE_H
