
#pragma once

#include "BlockPlant.h"
#include "../FastRandom.h"





/** Common class that takes care of beetroots, carrots, potatoes and wheat */
class cBlockCropsHandler final :
	public cBlockPlant<true>
{
	using Super = cBlockPlant<true>;

public:

	using Super::Super;

	static const unsigned char BeetrootsMaxAge = 3;
	static const unsigned char CarrotsMaxAge = 7;
	static const unsigned char PotatoesMaxAge = 7;
	static const unsigned char WheatMaxAge = 7;

	static inline bool IsBlockCrop(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::Beetroots:
			case BlockType::Carrots:
			case BlockType::Potatoes:
			case BlockType::Wheat:
				return true;
			default: return false;
		}
	}

	static inline bool IsFullyGrown(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::Beetroots: return (Beetroots::Age(a_Block) == BeetrootsMaxAge);
			case BlockType::Carrots: return (Carrots::Age(a_Block) == CarrotsMaxAge);
			case BlockType::Potatoes: return (Potatoes::Age(a_Block) == PotatoesMaxAge);
			case BlockType::Wheat: return (Wheat::Age(a_Block) == WheatMaxAge);
			default: return false;
		}
	}

private:

	/** Calculate the number of seeds to drop when the crop is broken. */
	static char CalculateSeedCount(char a_Min, char a_BaseRolls, unsigned char a_FortuneLevel)
	{
		std::binomial_distribution<> Binomial(a_BaseRolls + a_FortuneLevel, 0.57);
		return static_cast<char>(a_Min + Binomial(GetRandomProvider().Engine()));
	}





	virtual cItems ConvertToPickups(BlockState a_Block, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		using namespace Block;
		auto & Rand = GetRandomProvider();
		switch (a_Block.Type())
		{
			case BlockType::Beetroots:
			{
				if (Beetroots::Age(a_Block) < BeetrootsMaxAge)
				{
					return cItem(E_ITEM_BEETROOT_SEEDS);
				}
				const auto SeedCount = CalculateSeedCount(0, 3, ToolFortuneLevel(a_Tool));
				cItems Res;
				Res.Add(E_ITEM_BEETROOT_SEEDS, SeedCount);
				Res.Add(E_ITEM_BEETROOT);
				return Res;
			}
			case BlockType::Carrots:
			{
				if (Carrots::Age(a_Block) < CarrotsMaxAge)
				{
					return cItem(E_ITEM_CARROT);
				}
				cItems Res;
				// https://minecraft.gamepedia.com/Carrot#Breaking
				const auto CarrotCount = CalculateSeedCount(1, 4, ToolFortuneLevel(a_Tool));
				Res.Add(E_ITEM_CARROT, CarrotCount);
				return Res;
			}
			case BlockType::Potatoes:
			{
				if (Potatoes::Age(a_Block) < PotatoesMaxAge)
				{
					return cItem(E_ITEM_POTATO);
				}
				cItems Res;
				// https://minecraft.gamepedia.com/Potato#Breaking
				const auto PotatoCount = CalculateSeedCount(2, 3, ToolFortuneLevel(a_Tool));
				Res.Add(E_ITEM_POTATO, PotatoCount);
				if (Rand.RandBool(0.02))
				{
					// https://minecraft.gamepedia.com/Poisonous_Potato#Obtaining
					// With a 2% chance, drop a poisonous potato as well:
					Res.Add(E_ITEM_POISONOUS_POTATO);
				}
				return Res;
			}
			case BlockType::Wheat:
			{
				if (Wheat::Age(a_Block) < WheatMaxAge)
				{
					return cItem(E_ITEM_SEEDS);
				}
				cItems Res;
				// https://minecraft.fandom.com/wiki/Seeds_(Wheat)
				Res.Add(E_ITEM_WHEAT);
				const auto SeedCount = CalculateSeedCount(1, 3, ToolFortuneLevel(a_Tool));
				Res.Add(E_ITEM_SEEDS, SeedCount);
				return Res;
			}
			default:
			{
				ASSERT(!"Unhandled block type");
				return {};
			}
		}
	}





	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, unsigned char a_NumStages = 1) const override
	{
		using namespace Block;
		const auto OldSelf = a_Chunk.GetBlock(a_RelPos);

		switch (OldSelf.Type())
		{
			case BlockType::Beetroots:
			{
				auto OldAge = Beetroots::Age(OldSelf);
				auto NewAge = std::clamp<unsigned char>(OldAge + a_NumStages, 0, BeetrootsMaxAge);
				a_Chunk.SetBlock(a_RelPos, Beetroots::Beetroots(NewAge));
				return NewAge - OldAge;
			}
			case BlockType::Carrots:
			{
				auto OldAge = Carrots::Age(OldSelf);
				auto NewAge = std::clamp<unsigned char>(OldAge + a_NumStages, 0, CarrotsMaxAge);
				a_Chunk.SetBlock(a_RelPos, Carrots::Carrots(NewAge));
				return NewAge - OldAge;
			}
			case BlockType::Potatoes:
			{
				auto OldAge = Potatoes::Age(OldSelf);
				auto NewAge = std::clamp<unsigned char>(OldAge + a_NumStages, 0, PotatoesMaxAge);
				a_Chunk.SetBlock(a_RelPos, Potatoes::Potatoes(NewAge));
				return NewAge - OldAge;
			}
			case BlockType::Wheat:
			{
				auto OldAge = Wheat::Age(OldSelf);
				auto NewAge = std::clamp<unsigned char>(OldAge + a_NumStages, 0, WheatMaxAge);
				a_Chunk.SetBlock(a_RelPos, Wheat::Wheat(NewAge));
				return NewAge - OldAge;
			}
			default: return 0;
		}
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		return ((a_RelPos.y > 0) && (a_Chunk.GetBlock(a_RelPos.addedY(-1)).Type() == BlockType::Farmland));
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 7;
	}
};
