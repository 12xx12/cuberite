
#pragma once

#include "BlockHandler.h"
#include "Mixins.h"
#include "../EffectID.h"




class cBlockTrapdoorHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	static inline bool IsBlockTrapdoor(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaTrapdoor:
			case BlockType::BirchTrapdoor:
			case BlockType::CrimsonTrapdoor:
			case BlockType::DarkOakTrapdoor:
			case BlockType::IronTrapdoor:
			case BlockType::JungleTrapdoor:
			case BlockType::OakTrapdoor:
			case BlockType::SpruceTrapdoor:
			case BlockType::WarpedTrapdoor:
				return true;
			default: return false;
		}
	}




	static inline BlockState ToggleTrapdoor(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaTrapdoor:  return AcaciaTrapdoor::AcaciaTrapdoor  (AcaciaTrapdoor::Facing(a_Block),  AcaciaTrapdoor::Half(a_Block),  !AcaciaTrapdoor::Open(a_Block),  AcaciaTrapdoor::Powered(a_Block));
			case BlockType::BirchTrapdoor:   return BirchTrapdoor::BirchTrapdoor    (BirchTrapdoor::Facing(a_Block),   BirchTrapdoor::Half(a_Block),   !BirchTrapdoor::Open(a_Block),   BirchTrapdoor::Powered(a_Block));
			case BlockType::CrimsonTrapdoor: return CrimsonTrapdoor::CrimsonTrapdoor(CrimsonTrapdoor::Facing(a_Block), CrimsonTrapdoor::Half(a_Block), !CrimsonTrapdoor::Open(a_Block), CrimsonTrapdoor::Powered(a_Block));
			case BlockType::DarkOakTrapdoor: return DarkOakTrapdoor::DarkOakTrapdoor(DarkOakTrapdoor::Facing(a_Block), DarkOakTrapdoor::Half(a_Block), !DarkOakTrapdoor::Open(a_Block), DarkOakTrapdoor::Powered(a_Block));
			case BlockType::IronTrapdoor:    return IronTrapdoor::IronTrapdoor      (IronTrapdoor::Facing(a_Block),    IronTrapdoor::Half(a_Block),    !IronTrapdoor::Open(a_Block),    IronTrapdoor::Powered(a_Block));
			case BlockType::JungleTrapdoor:  return JungleTrapdoor::JungleTrapdoor  (JungleTrapdoor::Facing(a_Block),  JungleTrapdoor::Half(a_Block),  !JungleTrapdoor::Open(a_Block),  JungleTrapdoor::Powered(a_Block));
			case BlockType::OakTrapdoor:     return OakTrapdoor::OakTrapdoor        (OakTrapdoor::Facing(a_Block),     OakTrapdoor::Half(a_Block),     !OakTrapdoor::Open(a_Block),     OakTrapdoor::Powered(a_Block));
			case BlockType::SpruceTrapdoor:  return SpruceTrapdoor::SpruceTrapdoor  (SpruceTrapdoor::Facing(a_Block),  SpruceTrapdoor::Half(a_Block),  !SpruceTrapdoor::Open(a_Block),  SpruceTrapdoor::Powered(a_Block));
			case BlockType::WarpedTrapdoor:  return WarpedTrapdoor::WarpedTrapdoor  (WarpedTrapdoor::Facing(a_Block),  WarpedTrapdoor::Half(a_Block),  !WarpedTrapdoor::Open(a_Block),  WarpedTrapdoor::Powered(a_Block));
			default: return a_Block;
		}
	}




	static inline BlockState ToggleTrapdoorPower(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaTrapdoor:  return AcaciaTrapdoor::AcaciaTrapdoor  (AcaciaTrapdoor::Facing(a_Block),  AcaciaTrapdoor::Half(a_Block),  AcaciaTrapdoor::Open(a_Block),  !AcaciaTrapdoor::Powered(a_Block));
			case BlockType::BirchTrapdoor:   return BirchTrapdoor::BirchTrapdoor    (BirchTrapdoor::Facing(a_Block),   BirchTrapdoor::Half(a_Block),   BirchTrapdoor::Open(a_Block),   !BirchTrapdoor::Powered(a_Block));
			case BlockType::CrimsonTrapdoor: return CrimsonTrapdoor::CrimsonTrapdoor(CrimsonTrapdoor::Facing(a_Block), CrimsonTrapdoor::Half(a_Block), CrimsonTrapdoor::Open(a_Block), !CrimsonTrapdoor::Powered(a_Block));
			case BlockType::DarkOakTrapdoor: return DarkOakTrapdoor::DarkOakTrapdoor(DarkOakTrapdoor::Facing(a_Block), DarkOakTrapdoor::Half(a_Block), DarkOakTrapdoor::Open(a_Block), !DarkOakTrapdoor::Powered(a_Block));
			case BlockType::IronTrapdoor:    return IronTrapdoor::IronTrapdoor      (IronTrapdoor::Facing(a_Block),    IronTrapdoor::Half(a_Block),    IronTrapdoor::Open(a_Block),    !IronTrapdoor::Powered(a_Block));
			case BlockType::JungleTrapdoor:  return JungleTrapdoor::JungleTrapdoor  (JungleTrapdoor::Facing(a_Block),  JungleTrapdoor::Half(a_Block),  JungleTrapdoor::Open(a_Block),  !JungleTrapdoor::Powered(a_Block));
			case BlockType::OakTrapdoor:     return OakTrapdoor::OakTrapdoor        (OakTrapdoor::Facing(a_Block),     OakTrapdoor::Half(a_Block),     OakTrapdoor::Open(a_Block),     !OakTrapdoor::Powered(a_Block));
			case BlockType::SpruceTrapdoor:  return SpruceTrapdoor::SpruceTrapdoor  (SpruceTrapdoor::Facing(a_Block),  SpruceTrapdoor::Half(a_Block),  SpruceTrapdoor::Open(a_Block),  !SpruceTrapdoor::Powered(a_Block));
			case BlockType::WarpedTrapdoor:  return WarpedTrapdoor::WarpedTrapdoor  (WarpedTrapdoor::Facing(a_Block),  WarpedTrapdoor::Half(a_Block),  WarpedTrapdoor::Open(a_Block),  !WarpedTrapdoor::Powered(a_Block));
			default: return a_Block;
		}
	}

private:

	virtual bool IsUseable(void) const override
	{
		return true;
	}





	virtual bool OnUse(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace,
		const Vector3i a_CursorPos
	) const override
	{
		if (m_BlockType == BlockType::IronTrapdoor)
		{
			// Iron doors can only be toggled by redstone, not by right-clicking
			return false;
		}

		// Flip the ON bit on / off using the XOR bitwise operation
		auto Self = a_ChunkInterface.GetBlock(a_BlockPos);
		a_ChunkInterface.SetBlock(a_BlockPos, ToggleTrapdoor(Self));
		a_WorldInterface.GetBroadcastManager().BroadcastSoundParticleEffect(EffectID::SFX_RANDOM_FENCE_GATE_OPEN, a_BlockPos, 0, a_Player.GetClientHandle());

		return true;
	}





	virtual void OnCancelRightClick(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace
	) const override
	{
		UNUSED(a_ChunkInterface);
		a_WorldInterface.SendBlockTo(a_BlockPos, a_Player);
	}





	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BlockState & a_Block
	) const override
	{
		bool IsBottom = false;
		eBlockFace DestFacing;
		switch (a_ClickedBlockFace)
		{
			case BLOCK_FACE_NONE: return false;

			// Clicked one of the sides
			case BLOCK_FACE_XM:
			case BLOCK_FACE_XP:
			case BLOCK_FACE_ZM:
			case BLOCK_FACE_ZP:
			{
				if (a_CursorPos.y < 8)
				{
					// Trapdoor is placed on a higher half of a vertical block.
					IsBottom = true;
				}
				DestFacing = MirrorBlockFaceY(a_ClickedBlockFace);
				break;
			}
			case BLOCK_FACE_YM:
			{
				IsBottom = true;
				DestFacing = RotationToBlockFace(a_Player.GetYaw());
				break;
			}
			case BLOCK_FACE_YP: break;
		}

		using namespace Block;

		switch (m_BlockType)
		{
			case BlockType::AcaciaTrapdoor:
			{
				a_Block = AcaciaTrapdoor::AcaciaTrapdoor(
					DestFacing,
					IsBottom ? AcaciaTrapdoor::Half::Bottom : AcaciaTrapdoor::Half::Top,
					false, false);
				return true;
			}
			case BlockType::BirchTrapdoor:
			{
				a_Block = BirchTrapdoor::BirchTrapdoor(
					DestFacing,
					IsBottom ? BirchTrapdoor::Half::Bottom : BirchTrapdoor::Half::Top,
					false, false);
				return true;
			}
			case BlockType::CrimsonTrapdoor:
			{
				a_Block = CrimsonTrapdoor::CrimsonTrapdoor(
					DestFacing,
					IsBottom ? CrimsonTrapdoor::Half::Bottom : CrimsonTrapdoor::Half::Top,
					false, false);
				return true;
			}
			case BlockType::DarkOakTrapdoor:
			{
				a_Block = DarkOakTrapdoor::DarkOakTrapdoor(
					DestFacing,
					IsBottom ? DarkOakTrapdoor::Half::Bottom : DarkOakTrapdoor::Half::Top,
					false, false);
				return true;
			}
			case BlockType::IronTrapdoor:
			{
				a_Block = IronTrapdoor::IronTrapdoor(
					DestFacing,
					IsBottom ? IronTrapdoor::Half::Bottom : IronTrapdoor::Half::Top,
					false, false);
				return true;
			}
			case BlockType::JungleTrapdoor:
			{
				a_Block = JungleTrapdoor::JungleTrapdoor(
					DestFacing,
					IsBottom ? JungleTrapdoor::Half::Bottom : JungleTrapdoor::Half::Top,
					false, false);
				return true;
			}
			case BlockType::OakTrapdoor:
			{
				a_Block = OakTrapdoor::OakTrapdoor(
					DestFacing,
					IsBottom ? OakTrapdoor::Half::Bottom : OakTrapdoor::Half::Top,
					false, false);
				return true;
			}
			case BlockType::SpruceTrapdoor:
			{
				a_Block = SpruceTrapdoor::SpruceTrapdoor(
					DestFacing,
					IsBottom ? SpruceTrapdoor::Half::Bottom : SpruceTrapdoor::Half::Top,
					false, false);
				return true;
			}
			case BlockType::WarpedTrapdoor:
			{
				a_Block = WarpedTrapdoor::WarpedTrapdoor(
					DestFacing,
					IsBottom ? WarpedTrapdoor::Half::Bottom : WarpedTrapdoor::Half::Top,
					false, false);
				return true;
			}
			default: return false;
		}
	}




	virtual ColourID GetMapBaseColourID() const override
	{
		switch (m_BlockType)
		{
			case BlockType::AcaciaTrapdoor:  return 15;
			case BlockType::BirchTrapdoor:   return 2;
			case BlockType::CrimsonTrapdoor: return 35;
			case BlockType::DarkOakTrapdoor: return 26;
			case BlockType::IronTrapdoor:    return 6;
			case BlockType::JungleTrapdoor:  return 10;
			case BlockType::OakTrapdoor:     return 13;
			case BlockType::SpruceTrapdoor:  return 26;
			case BlockType::WarpedTrapdoor:  return 56;
			default:
			{
				ASSERT(!"Unhandled blocktype in trapdoor handler!");
				return 0;
			}
		}
	}
};




