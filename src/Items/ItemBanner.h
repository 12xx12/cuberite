

#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Blocks/BlockBanner.h"





class cItemBannerHandler: public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemBannerHandler(int a_ItemType): Super(a_ItemType)
	{
	}





	virtual bool IsPlaceable(void) override
	{
		return true;
	}





	virtual bool GetBlocksToPlace(
		cWorld & a_World,
		cPlayer & a_Player,
		const cItem & a_EquippedItem,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		sSetBlockVector & a_BlocksToPlace
	) override
	{
		/*
			may have 16 colors
			can be placed in item frame - just shows the item model
			If renamed retains its name on adding and removing pattern
			keeps its name on building and breaking

			Banners can be copied using blank banner
			may be crated together with shield to display the banner

			can be used for 1.5 items as fuel
		*/

		if (a_ClickedBlockFace == BLOCK_FACE_BOTTOM)
		{
			return false;
		}
		// Placing on the floor
		else if (a_ClickedBlockFace == BLOCK_FACE_TOP)
		{

			NIBBLETYPE BlockMeta = 0x00;
			double Rotation = a_Player.GetYaw();
			if ((Rotation >= -135) && (Rotation < -45))
			{
				BlockMeta |= 0x00;
			}
			else if ((Rotation >= -45) && (Rotation < 45))
			{
				BlockMeta |= 0x04;
			}
			else if ((Rotation >= 45) && (Rotation < 135))
			{
				BlockMeta |= 0x08;
			}
			else  // degrees jumping from 180 to -180
			{
				BlockMeta |= 0x0ab;
			}
			a_BlocksToPlace.emplace_back(a_PlacedBlockPos, E_BLOCK_STANDING_BANNER, BlockMeta);
		}
		// placing on the sides
		else if (a_ClickedBlockFace != BLOCK_FACE_NONE)
		{
			NIBBLETYPE BlockMeta = 0x00;
			double Rotation = a_Player.GetYaw();
			if ((Rotation >= -135) && (Rotation < -45))
			{
				BlockMeta |= 0x00;
			}
			else if ((Rotation >= -45) && (Rotation < 45))
			{
				BlockMeta |= 0x04;
			}
			else if ((Rotation >= 45) && (Rotation < 135))
			{
				BlockMeta |= 0x08;
			}
			else  // degrees jumping from 180 to -180
			{
				BlockMeta |= 0x0ab;
			}
			a_BlocksToPlace.emplace_back(a_PlacedBlockPos, E_BLOCK_WALL_BANNER, BlockMeta);
		}
		else
		{
			return false;
		}
		return true;
	}
};