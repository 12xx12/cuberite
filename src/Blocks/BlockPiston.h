
#pragma once

#include "BlockHandler.h"
#include "Mixins.h"
#include "../Item.h"




// fwd:
class cWorld;





class cBlockPistonHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	/** Converts piston block's metadata into a unit vector representing the direction in which the piston will extend. */
	static Vector3i MetadataToOffset(BlockState a_Block);

	static void ExtendPiston(Vector3i aBlockPos, cWorld & aWorld);
	static void RetractPiston(Vector3i aBlockPos, cWorld & aWorld);

	/** Returns true if the piston (with the specified meta) is extended */
	static inline bool IsExtended(BlockState a_Block) { return Block::Piston::Extended(a_Block); }

private:

	typedef std::unordered_set<Vector3i, VectorHasher<int>> Vector3iSet;

	/** Piston extension block action */
	static const Byte PistonExtendAction = 0U;

	/** Piston retraction block action */
	static const Byte PistonRetractAction = 1U;

	/** Returns true if the piston (specified by blocktype) is a sticky piston */
	static inline bool IsSticky(BlockState a_Block) { return (a_Block.Type() == BlockType::StickyPiston); }

	/** Returns true if the specified block can be pushed by a piston (and left intact) */
	static inline bool CanPush(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::Anvil:
			case BlockType::ChippedAnvil:
			case BlockType::DamagedAnvil:

			case BlockType::Barrier:
			case BlockType::Beacon:
			case BlockType::Bedrock:
			case BlockType::BrewingStand:
			case BlockType::ChainCommandBlock:
			case BlockType::Chest:
			case BlockType::CommandBlock:
			case BlockType::DaylightDetector:
			case BlockType::Dispenser:
			case BlockType::Dropper:
			case BlockType::EnchantingTable:
			case BlockType::EndGateway:
			case BlockType::EndPortal:
			case BlockType::EndPortalFrame:
			case BlockType::EnderChest:
			case BlockType::Furnace:
			case BlockType::Hopper:
			case BlockType::Jukebox:
			case BlockType::Spawner:
			case BlockType::NetherPortal:
			case BlockType::NoteBlock:
			case BlockType::Observer:
			case BlockType::PoweredRail:
			case BlockType::RepeatingCommandBlock:

			case BlockType::BlackBanner:
			case BlockType::BlueBanner:
			case BlockType::BrownBanner:
			case BlockType::CyanBanner:
			case BlockType::GrayBanner:
			case BlockType::GreenBanner:
			case BlockType::LightBlueBanner:
			case BlockType::LightGrayBanner:
			case BlockType::LimeBanner:
			case BlockType::MagentaBanner:
			case BlockType::OrangeBanner:
			case BlockType::PinkBanner:
			case BlockType::PurpleBanner:
			case BlockType::RedBanner:
			case BlockType::WhiteBanner:
			case BlockType::YellowBanner:

			case BlockType::BlackWallBanner:
			case BlockType::BlueWallBanner:
			case BlockType::BrownWallBanner:
			case BlockType::CyanWallBanner:
			case BlockType::GrayWallBanner:
			case BlockType::GreenWallBanner:
			case BlockType::LightBlueWallBanner:
			case BlockType::LightGrayWallBanner:
			case BlockType::LimeWallBanner:
			case BlockType::MagentaWallBanner:
			case BlockType::OrangeWallBanner:
			case BlockType::PinkWallBanner:
			case BlockType::PurpleWallBanner:
			case BlockType::RedWallBanner:
			case BlockType::WhiteWallBanner:
			case BlockType::YellowWallBanner:

			case BlockType::StructureBlock:
			case BlockType::TrappedChest:
			{
				return false;
			}
			case BlockType::Piston:
			case BlockType::StickyPiston:
			{
				// A piston can only be pushed if retracted:
				return !IsExtended(a_Block);
			}
			default: return true;
		}
	}

	/** Tries to push a block and increases the pushed blocks variable. Returns true if the block is pushable */
	static bool CanPushBlock(
		const Vector3i & aBlockPos, cWorld & aWorld, bool aRequirePushable,
		Vector3iSet & aBlocksPushed, const Vector3i & aPushDir
	);

	virtual ColourID GetMapBaseColourID(BlockState aBlock) const override
	{
		UNUSED(aBlock);
		return 11;
	}

	virtual void OnBroken(
		cChunkInterface & aChunkInterface, cWorldInterface & aWorldInterface,
		Vector3i aBlockPos,
		BlockState aBlock,
		const cEntity * aDigger
	) const override;

	/** Moves a list of blocks in a specific direction */
	static void PushBlocks(const Vector3iSet & aBlocksToPush,
		cWorld & aWorld, const Vector3i & aPushDir
	);
} ;





class cBlockPistonHeadHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	constexpr cBlockPistonHeadHandler(void) :
		Super(BlockType::MovingPiston)
	{
	}

	virtual void OnBroken(
		cChunkInterface & aChunkInterface, cWorldInterface & aWorldInterface,
		Vector3i aBlockPos,
		BlockState a_Block,
		const cEntity * aDigger
	) const override;

	virtual cItems ConvertToPickups(BlockState a_Block, const cEntity * aDigger, const cItem * aTool) const override;
} ;
