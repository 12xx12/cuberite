
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"





class cItemHoeHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemHoeHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}





	virtual bool OnItemUse(
		cWorld * a_World,
		cPlayer * a_Player,
		cBlockPluginInterface & a_PluginInterface,
		const cItem & a_HeldItem,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace
	) override
	{
		if ((a_ClickedBlockFace == BLOCK_FACE_NONE) || (a_ClickedBlockPos.y >= cChunkDef::Height))
		{
			return false;
		}

		// Need air above the hoe-d block to transform it:
		auto UpperBlockType = a_World->GetBlock(a_ClickedBlockPos.addedY(1));
		if (UpperBlockType != Block::Air::Air())
		{
			return false;
		}

		// Can only transform dirt or grass blocks:
		auto BlockBelow = a_World->GetBlock(a_ClickedBlockPos);
		switch (BlockBelow.Type())
		{
			case BlockType::GrassBlock:
			case BlockType::Dirt:
			case BlockType::CoarseDirt:
				break;
			default: return false;
		}

		// Transform:
		switch (BlockBelow.Type())
		{
			case BlockType::GrassBlock:
			case BlockType::Dirt:
			{
				a_World->SetBlock(a_ClickedBlockPos, Block::Farmland::Farmland());
				break;
			}
			case BlockType::CoarseDirt:
			{
				a_World->SetBlock(a_ClickedBlockPos, Block::Dirt::Dirt());
				break;
			}
			default: return false;
		}
		a_World->BroadcastSoundEffect("item.hoe.till", a_ClickedBlockPos + Vector3d(0.5, 0.5, 0.5), 1.0f, 0.8f);
		a_Player->UseEquippedItem();
		return true;
	}





	virtual short GetDurabilityLossByAction(eDurabilityLostAction a_Action) override
	{
		switch (a_Action)
		{
			case dlaAttackEntity:       return 1;
			case dlaBreakBlock:         return 0;
			case dlaBreakBlockInstant:  return 0;
		}
		UNREACHABLE("Unsupported durability loss action");
	}
} ;
