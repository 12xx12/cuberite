
// IntGen.h

// Declares the cIntGen class and descendants for generating and filtering various 2D arrays of ints

/*
The integers generated may be interpreted in several ways:
- land / sea designators
	- 0 = ocean
	- >0 = land
- biome group
	- 0 = ocean
	- 1 = desert biomes
	- 2 = temperate biomes
	- 3 = mountains (hills and forests)
	- 4 = ice biomes
- biome group with "bgfRare" flag (for generating rare biomes for the group)
- biome IDs
The interpretation depends on the generator used and on the position in the chain.

The generators can be chained together - one produces data that another one consumes.
Some of such chain connections require changing the data dimensions between the two, which is handled automatically
by using templates.
*/





#pragma once

#include <tuple>
#include "../Noise/Noise.h"
#include "BiomeDef.h"





/** Constants representing the biome group designators. */
const int bgOcean        = 0;
const int bgDesert       = 1;
const int bgTemperate    = 2;
const int bgMountains    = 3;
const int bgIce          = 4;
const int bgLandOceanMax = 4;  // Maximum biome group value generated in the landOcean generator
const int bgfRare        = 1024;  // Flag added to values to generate rare biomes for the group





/** Interface that all the generator classes provide. */
template <int SizeX, int SizeZ = SizeX>
class cIntGen
{
public:

	using IntGenType = cIntGen<SizeX, SizeZ>;

	/** Force a virtual destructor in all descendants.
	Descendants contain virtual functions and are referred to via pointer-to-base, so they need a virtual destructor. */
	virtual ~cIntGen() {}

	/** Holds the array of values generated by this class (descendant). */
	using Values = int[SizeX * SizeZ];

	/** Generates the array of templated size into a_Values, based on given min coords. */
	virtual void GetInts(int a_MinX, int a_MinZ, Values & a_Values) = 0;

};

// Code adapted from https://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer

template<int... >
struct sSeq
{
};

template<int N, int... S>
struct sGens : sGens<N - 1, N - 1, S...>
{
};

template<int... S>
struct sGens<0, S...>
{
	using type = sSeq<S...>;
};


template<class Gen, class... Args>
class cIntGenFactory
{

public:

	using Generator = Gen;

	cIntGenFactory(Args&&... a_args) :
		m_args(std::make_tuple<Args...>(std::forward<Args>(a_args)...))
	{
	}

	template <class LhsGen>
	std::shared_ptr<Gen> construct(LhsGen&& a_Lhs)
	{
		return construct_impl<LhsGen>(std::forward<LhsGen>(a_Lhs), typename sGens<sizeof...(Args)>::type());
	}


private:
	std::tuple<Args...> m_args;

	template <class LhsGen, int... S>
	std::shared_ptr<Gen> construct_impl(LhsGen&& a_Lhs, sSeq<S...>)
	{
		return std::make_shared<Gen>(std::get<S>(m_args)..., std::forward<LhsGen>(a_Lhs));
	}

};

template<class T, class RhsGen, class... Args>
std::shared_ptr<RhsGen> operator| (std::shared_ptr<T> a_Lhs, cIntGenFactory<RhsGen, Args...> a_Rhs)
{
	return a_Rhs.construct(static_cast<std::shared_ptr<typename T::IntGenType>>(a_Lhs));
}

template<class Gen, class... Args>
cIntGenFactory<Gen, Args...> MakeIntGen(Args&&... a_Args)
{
	return cIntGenFactory<Gen, Args...>(std::forward<Args>(a_Args)...);
}



/** Provides additional cNoise member and its helper functions. */
template <int SizeX, int SizeZ = SizeX>
class cIntGenWithNoise:
	public cIntGen<SizeX, SizeZ>
{
	using Super = cIntGen<SizeX, SizeZ>;

public:

	cIntGenWithNoise(int a_Seed):
		m_Noise(a_Seed)
	{
	}

protected:
	cNoise m_Noise;

	/** Chooses one of a_Val1 or a_Val2, based on m_Noise and the coordinates for querying the noise. */
	int ChooseRandomOne(int a_RndX, int a_RndZ, int a_Val1, int a_Val2)
	{
		int rnd = m_Noise.IntNoise2DInt(a_RndX, a_RndZ) / 7;
		return ((rnd & 1) == 0) ? a_Val1 : a_Val2;
	}

	/** Chooses one of a_ValN, based on m_Noise and the coordinates for querying the noise. */
	int ChooseRandomOne(int a_RndX, int a_RndZ, int a_Val1, int a_Val2, int a_Val3, int a_Val4)
	{
		int rnd = m_Noise.IntNoise2DInt(a_RndX, a_RndZ) / 7;
		switch (rnd % 4)
		{
			case 0:  return a_Val1;
			case 1:  return a_Val2;
			case 2:  return a_Val3;
			default: return a_Val4;
		}
	}
};





/** Generates a 2D array of random integers in the specified range [0 .. Range). */
template <int Range, int SizeX, int SizeZ = SizeX>
class cIntGenChoice:
	public cIntGenWithNoise<SizeX, SizeZ>
{
	using Super = cIntGenWithNoise<SizeX, SizeZ>;

public:

	cIntGenChoice(int a_Seed):
		Super(a_Seed)
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, typename Super::Values & a_Values) override
	{
		for (int z = 0; z < SizeZ; z++)
		{
			int BaseZ = a_MinZ + z;
			for (int x = 0; x < SizeX; x++)
			{
				a_Values[x + SizeX * z] = (Super::m_Noise.IntNoise2DInt(a_MinX + x, BaseZ) / 7) % Range;
			}
		}  // for z
	}
};





/** Decides between the ocean and landmass biomes.
Has a threshold (in percent) of how much land, the larger the threshold, the more land.
Generates 0 for ocean, biome group ID for landmass. */
template <int SizeX, int SizeZ = SizeX>
class cIntGenLandOcean:
	public cIntGenWithNoise<SizeX, SizeZ>
{
	using Super = cIntGenWithNoise<SizeX, SizeZ>;

public:

	cIntGenLandOcean(int a_Seed, int a_Threshold):
		Super(a_Seed),
		m_Threshold(a_Threshold)
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, typename Super::Values & a_Values) override
	{
		for (int z = 0; z < SizeZ; z++)
		{
			int BaseZ = a_MinZ + z;
			for (int x = 0; x < SizeX; x++)
			{
				int rnd = (Super::m_Noise.IntNoise2DInt(a_MinX + x, BaseZ) / 7);
				a_Values[x + SizeX * z] = ((rnd % 100) < m_Threshold) ? ((rnd / 101) % bgLandOceanMax + 1) : 0;
			}
		}

		// If the centerpoint of the world is within the area, set it to bgTemperate, always:
		if ((a_MinX <= 0) && (a_MinZ <= 0) && (a_MinX + SizeX > 0) && (a_MinZ + SizeZ > 0))
		{
			a_Values[-a_MinX - a_MinZ * SizeX] = bgTemperate;
		}
	}

protected:
	int m_Threshold;
};





/** Zooms the underlying value array to twice the size. Uses random-neighbor for the pixels in-between.
This means that the zoome out image is randomly distorted. Applying zoom several times provides all
the distortion that the generators need. */
template <int SizeX, int SizeZ = SizeX>
class cIntGenZoom:
	public cIntGenWithNoise<SizeX, SizeZ>
{
	using Super = cIntGenWithNoise<SizeX, SizeZ>;

protected:

	static const int m_LowerSizeX = (SizeX / 2) + 2;
	static const int m_LowerSizeZ = (SizeZ / 2) + 2;

public:

	using Underlying = std::shared_ptr<cIntGen<m_LowerSizeX, m_LowerSizeZ>>;


	cIntGenZoom(int a_Seed, Underlying a_UnderlyingGen):
		Super(a_Seed),
		m_UnderlyingGen(a_UnderlyingGen)
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, typename Super::Values & a_Values) override
	{
		// Generate the underlying data with half the resolution:
		int lowerMinX = a_MinX >> 1;
		int lowerMinZ = a_MinZ >> 1;
		int lowerData[m_LowerSizeX * m_LowerSizeZ];
		m_UnderlyingGen->GetInts(lowerMinX, lowerMinZ, lowerData);
		const int lowStepX = (m_LowerSizeX - 1) * 2;
		const int lowStepZ = (m_LowerSizeZ - 1) * 2;
		int cache[lowStepX * lowStepZ];

		// Discreet-interpolate the values into twice the size:
		for (int z = 0; z < m_LowerSizeZ - 1; ++z)
		{
			int idx = (z * 2) * lowStepX;
			int PrevZ0 = lowerData[z * m_LowerSizeX];
			int PrevZ1 = lowerData[(z + 1) * m_LowerSizeX];

			for (int x = 0; x < m_LowerSizeX - 1; ++x)
			{
				int ValX1Z0 = lowerData[x + 1 + z * m_LowerSizeX];
				int ValX1Z1 = lowerData[x + 1 + (z + 1) * m_LowerSizeX];
				int RndX = (x + lowerMinX) * 2;
				int RndZ = (z + lowerMinZ) * 2;
				cache[idx] = PrevZ0;
				cache[idx + lowStepX] = Super::ChooseRandomOne(RndX, RndZ + 1, PrevZ0, PrevZ1);
				cache[idx + 1]        = Super::ChooseRandomOne(RndX, RndZ - 1, PrevZ0, ValX1Z0);
				cache[idx + 1 + lowStepX] = Super::ChooseRandomOne(RndX, RndZ, PrevZ0, ValX1Z0, PrevZ1, ValX1Z1);
				idx += 2;
				PrevZ0 = ValX1Z0;
				PrevZ1 = ValX1Z1;
			}
		}

		// Copy from Cache into a_Values; take into account the even / odd offsets in a_Min:
		for (int z = 0; z < SizeZ; ++z)
		{
			memcpy(a_Values + z * SizeX, cache + (z + (a_MinZ & 1)) * lowStepX + (a_MinX & 1), SizeX * sizeof(int));
		}
	}

protected:
	Underlying m_UnderlyingGen;
};





/** Smoothes out some artifacts generated by the zooming - mostly single-pixel values.
Compares each pixel to its neighbors and if the neighbors are equal, changes the pixel to their value. */
template <int SizeX, int SizeZ = SizeX>
class cIntGenSmooth:
	public cIntGenWithNoise<SizeX, SizeZ>
{
	using Super = cIntGenWithNoise<SizeX, SizeZ>;

	static const int m_LowerSizeX = SizeX + 2;
	static const int m_LowerSizeZ = SizeZ + 2;

public:

	using Underlying = std::shared_ptr<cIntGen<m_LowerSizeX, m_LowerSizeZ>>;


	cIntGenSmooth(int a_Seed, Underlying a_Underlying):
		Super(a_Seed),
		m_Underlying(a_Underlying)
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, typename Super::Values & a_Values) override
	{
		// Generate the underlying values:
		int lowerData[m_LowerSizeX * m_LowerSizeZ];
		m_Underlying->GetInts(a_MinX - 1, a_MinZ - 1, lowerData);

		// Smooth - for each square check if the surroundings are the same, if so, expand them diagonally.
		// Also get rid of single-pixel irregularities (A-B-A):
		for (int z = 0; z < SizeZ; z++)
		{
			int NoiseZ = a_MinZ + z;
			for (int x = 0; x < SizeX; x++)
			{
				int val   = lowerData[x + 1 + (z + 1) * m_LowerSizeX];
				int above = lowerData[x + 1 +  z      * m_LowerSizeX];
				int below = lowerData[x + 1 + (z + 2) * m_LowerSizeX];
				int left  = lowerData[x     + (z + 1) * m_LowerSizeX];
				int right = lowerData[x + 2 + (z + 1) * m_LowerSizeX];

				if ((left == right) && (above == below))
				{
					if (((Super::m_Noise.IntNoise2DInt(a_MinX + x, NoiseZ) / 7) % 2) == 0)
					{
						val = left;
					}
					else
					{
						val = above;
					}
				}
				else
				{
					if (left == right)
					{
						val = left;
					}

					if (above == below)
					{
						val = above;
					}
				}

				a_Values[x + z * SizeX] = val;
			}
		}
	}

protected:
	Underlying m_Underlying;
};





/** Converts land biomes at the edge of an ocean into the respective beach biome. */
template <int SizeX, int SizeZ = SizeX>
class cIntGenBeaches:
	public cIntGen<SizeX, SizeZ>
{
	using Super = cIntGen<SizeX, SizeZ>;

	static const int m_UnderlyingSizeX = SizeX + 2;
	static const int m_UnderlyingSizeZ = SizeZ + 2;

public:

	using Underlying = std::shared_ptr<cIntGen<m_UnderlyingSizeX, m_UnderlyingSizeZ>>;


	cIntGenBeaches(Underlying a_Underlying):
		m_Underlying(a_Underlying)
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, typename Super::Values & a_Values) override
	{
		// Map for biome -> its beach:
		static const int ToBeach[] =
		{
			/* biOcean            */ biOcean,
			/* biPlains           */ biBeach,
			/* biDesert           */ biBeach,
			/* biExtremeHills     */ biStoneBeach,
			/* biForest           */ biBeach,
			/* biTaiga            */ biColdBeach,
			/* biSwampland        */ biSwampland,
			/* biRiver            */ biRiver,
			/* biNether           */ biNether,
			/* biEnd              */ biEnd,
			/* biFrozenOcean      */ biColdBeach,
			/* biFrozenRiver      */ biColdBeach,
			/* biIcePlains        */ biColdBeach,
			/* biIceMountains     */ biColdBeach,
			/* biMushroomIsland   */ biMushroomShore,
			/* biMushroomShore    */ biMushroomShore,
			/* biBeach            */ biBeach,
			/* biDesertHills      */ biBeach,
			/* biForestHills      */ biBeach,
			/* biTaigaHills       */ biColdBeach,
			/* biExtremeHillsEdge */ biStoneBeach,
			/* biJungle           */ biBeach,
			/* biJungleHills      */ biBeach,
			/* biJungleEdge       */ biBeach,
			/* biDeepOcean        */ biOcean,
			/* biStoneBeach       */ biStoneBeach,
			/* biColdBeach        */ biColdBeach,
			/* biBirchForest      */ biBeach,
			/* biBirchForestHills */ biBeach,
			/* biRoofedForest     */ biBeach,
			/* biColdTaiga        */ biColdBeach,
			/* biColdTaigaHills   */ biColdBeach,
			/* biMegaTaiga        */ biStoneBeach,
			/* biMegaTaigaHills   */ biStoneBeach,
			/* biExtremeHillsPlus */ biStoneBeach,
			/* biSavanna          */ biBeach,
			/* biSavannaPlateau   */ biBeach,
			/* biMesa             */ biMesa,
			/* biMesaPlateauF     */ biMesa,
			/* biMesaPlateau      */ biMesa,
		};

		// Generate the underlying values:
		int lowerValues[m_UnderlyingSizeX * m_UnderlyingSizeZ];
		m_Underlying->GetInts(a_MinX - 1, a_MinZ - 1, lowerValues);

		// Add beaches between ocean and biomes:
		for (int z = 0; z < SizeZ; z++)
		{
			for (int x = 0; x < SizeX; x++)
			{
				int val   = lowerValues[x + 1 + (z + 1) * m_UnderlyingSizeX];
				int above = lowerValues[x + 1 + z       * m_UnderlyingSizeX];
				int below = lowerValues[x + 1 + (z + 2) * m_UnderlyingSizeX];
				int left  = lowerValues[x     + (z + 1) * m_UnderlyingSizeX];
				int right = lowerValues[x + 2 + (z + 1) * m_UnderlyingSizeX];
				if (!IsBiomeOcean(val))
				{
					if (IsBiomeOcean(above) || IsBiomeOcean(below) || IsBiomeOcean(left) || IsBiomeOcean(right))
					{
						// First convert the value to a regular biome (drop the M flag), then modulo by our biome count:
						val = ToBeach[(val % 128) % ARRAYCOUNT(ToBeach)];
					}
				}
				a_Values[x + z * SizeX] = val;
			}
		}
	}

protected:
	Underlying m_Underlying;
};





/** Generates the underlying numbers and then randomly changes some ocean group pixels into random land
biome group pixels, based on the predefined chance. */
template <int SizeX, int SizeZ = SizeX>
class cIntGenAddIslands:
	public cIntGenWithNoise<SizeX, SizeZ>
{
	using Super = cIntGenWithNoise<SizeX, SizeZ>;

public:

	using Underlying = std::shared_ptr<cIntGen<SizeX, SizeZ>>;


	cIntGenAddIslands(int a_Seed, int a_Chance, Underlying a_Underlying):
		Super(a_Seed),
		m_Chance(a_Chance),
		m_Underlying(a_Underlying)
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, typename Super::Values & a_Values) override
	{
		m_Underlying->GetInts(a_MinX, a_MinZ, a_Values);
		for (int z = 0; z < SizeZ; z++)
		{
			for (int x = 0; x < SizeX; x++)
			{
				if (a_Values[x + z * SizeX] == bgOcean)
				{
					int rnd = Super::m_Noise.IntNoise2DInt(a_MinX + x, a_MinZ + z) / 7;
					if (rnd % 1000 < m_Chance)
					{
						a_Values[x + z * SizeX] = (rnd / 1003) % bgLandOceanMax;
					}
				}
			}  // for x
		}  // for z
	}

protected:
	/** Chance, in permille, of an island being generated in ocean. */
	int m_Chance;

	Underlying m_Underlying;
};





/** A filter that adds an edge biome group between two biome groups that need an edge between them. */
template <int SizeX, int SizeZ = SizeX>
class cIntGenBiomeGroupEdges:
	public cIntGen<SizeX, SizeZ>
{
	using Super = cIntGen<SizeX, SizeZ>;

	static const int m_UnderlyingSizeX = SizeX + 2;
	static const int m_UnderlyingSizeZ = SizeZ + 2;

public:

	using Underlying = std::shared_ptr<cIntGen<m_UnderlyingSizeX, m_UnderlyingSizeZ>>;


	cIntGenBiomeGroupEdges(Underlying a_Underlying):
		m_Underlying(a_Underlying)
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, typename Super::Values & a_Values)
	{
		// Generate the underlying biome groups:
		int lowerValues[m_UnderlyingSizeX * m_UnderlyingSizeZ];
		m_Underlying->GetInts(a_MinX, a_MinZ, lowerValues);

		// Change the biomes on incompatible edges into an edge biome:
		for (int z = 0; z < SizeZ; z++)
		{
			for (int x = 0; x < SizeX; x++)
			{
				int val   = lowerValues[x + 1 + (z + 1) * m_UnderlyingSizeX];
				int above = lowerValues[x + 1 + z       * m_UnderlyingSizeX];
				int below = lowerValues[x + 1 + (z + 2) * m_UnderlyingSizeX];
				int left  = lowerValues[x     + (z + 1) * m_UnderlyingSizeX];
				int right = lowerValues[x + 2 + (z + 1) * m_UnderlyingSizeX];
				switch (val)
				{
					// Desert should neighbor only oceans, desert and temperates; change to temperate when another:
					case bgDesert:
					{
						if (
							!isDesertCompatible(above) ||
							!isDesertCompatible(below) ||
							!isDesertCompatible(left) ||
							!isDesertCompatible(right)
						)
						{
							val = bgTemperate;
						}
						break;
					}  // case bgDesert

					// Ice should not neighbor deserts; change to temperate:
					case bgIce:
					{
						if (
							(above == bgDesert) ||
							(below == bgDesert) ||
							(left  == bgDesert) ||
							(right == bgDesert)
						)
						{
							val = bgTemperate;
						}
						break;
					}  // case bgIce
				}
				a_Values[x + z * SizeX] = val;
			}  // for x
		}  // for z
	}

protected:
	Underlying m_Underlying;


	inline bool isDesertCompatible(int a_BiomeGroup)
	{
		switch (a_BiomeGroup)
		{
			case bgOcean:
			case bgDesert:
			case bgTemperate:
			{
				return true;
			}
			default:
			{
				return false;
			}
		}
	}
};





/** Turns biome group indices into real biomes.
For each pixel, takes its biome group and chooses a random biome from that group; replaces the value with
that biome. */
template <int SizeX, int SizeZ = SizeX>
class cIntGenBiomes:
	public cIntGenWithNoise<SizeX, SizeZ>
{
	using Super = cIntGenWithNoise<SizeX, SizeZ>;

public:

	using Underlying = std::shared_ptr<cIntGen<SizeX, SizeZ>>;


	cIntGenBiomes(int a_Seed, Underlying a_Underlying):
		Super(a_Seed),
		m_Underlying(a_Underlying)
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, typename Super::Values & a_Values) override
	{
		// Define the per-biome-group biomes:
		static const int oceanBiomes[] =
		{
			biOcean,  // biDeepOcean,
		};

		// Same as oceanBiomes, there are no rare oceanic biomes (mushroom islands are handled separately)
		static const int rareOceanBiomes[] =
		{
			biOcean,
		};

		static const int desertBiomes[] =
		{
			biDesert, biDesert, biDesert, biDesert, biDesert, biDesert, biSavanna, biSavanna, biPlains,
		};

		static const int rareDesertBiomes[] =
		{
			biMesaPlateau, biMesaPlateauF,
		};

		static const int temperateBiomes[] =
		{
			biForest, biForest, biRoofedForest, biExtremeHills, biPlains, biBirchForest, biSwampland,
		};

		static const int rareTemperateBiomes[] =
		{
			biJungle,  // Jungle is not strictly temperate, but let's piggyback it here
		};

		static const int mountainBiomes[] =
		{
			biExtremeHills, biForest, biTaiga, biPlains,
		};

		static const int rareMountainBiomes[] =
		{
			biMegaTaiga,
		};

		static const int iceBiomes[] =
		{
			biIcePlains, biIcePlains, biIcePlains, biIcePlains, biColdTaiga,
		};

		// Same as iceBiomes, there's no rare ice biome
		static const int rareIceBiomes[] =
		{
			biIcePlains, biIcePlains, biIcePlains, biIcePlains, biColdTaiga,
		};

		static const cBiomesInGroups biomesInGroups[] =
		{
			/* bgOcean */     { static_cast<int>(ARRAYCOUNT(oceanBiomes)),     oceanBiomes},
			/* bgDesert */    { static_cast<int>(ARRAYCOUNT(desertBiomes)),    desertBiomes},
			/* bgTemperate */ { static_cast<int>(ARRAYCOUNT(temperateBiomes)), temperateBiomes},
			/* bgMountains */ { static_cast<int>(ARRAYCOUNT(mountainBiomes)),  mountainBiomes},
			/* bgIce */       { static_cast<int>(ARRAYCOUNT(iceBiomes)),       iceBiomes},
		};

		static const cBiomesInGroups rareBiomesInGroups[] =
		{
			/* bgOcean */     { static_cast<int>(ARRAYCOUNT(rareOceanBiomes)),     rareOceanBiomes},
			/* bgDesert */    { static_cast<int>(ARRAYCOUNT(rareDesertBiomes)),    rareDesertBiomes},
			/* bgTemperate */ { static_cast<int>(ARRAYCOUNT(rareTemperateBiomes)), rareTemperateBiomes},
			/* bgMountains */ { static_cast<int>(ARRAYCOUNT(rareMountainBiomes)),  rareMountainBiomes},
			/* bgIce */       { static_cast<int>(ARRAYCOUNT(rareIceBiomes)),       rareIceBiomes},
		};

		// Generate the underlying values, representing biome groups:
		m_Underlying->GetInts(a_MinX, a_MinZ, a_Values);

		// Overwrite each biome group with a random biome from that group:
		for (int z = 0; z < SizeZ; z++)
		{
			int IdxZ = z * SizeX;
			for (int x = 0; x < SizeX; x++)
			{
				size_t val = static_cast<size_t>(a_Values[x + IdxZ]);
				const cBiomesInGroups & Biomes = (val > bgfRare) ?
					rareBiomesInGroups[(val & (bgfRare - 1)) % ARRAYCOUNT(rareBiomesInGroups)] :
					biomesInGroups[val % ARRAYCOUNT(biomesInGroups)];
				int rnd = (Super::m_Noise.IntNoise2DInt(x + a_MinX, z + a_MinZ) / 7);
				a_Values[x + IdxZ] = Biomes.Biomes[rnd % Biomes.Count];
			}
		}
	}

protected:

	struct cBiomesInGroups
	{
		const int Count;
		const int * Biomes;
	};


	/** The underlying int generator */
	Underlying m_Underlying;
};





/** Randomly replaces pixels of one value to another value, using the given chance. */
template <int SizeX, int SizeZ = SizeX>
class cIntGenReplaceRandomly:
	public cIntGenWithNoise<SizeX, SizeZ>
{
	using Super = cIntGenWithNoise<SizeX, SizeZ>;

public:

	using Underlying = std::shared_ptr<cIntGen<SizeX, SizeZ>>;


	cIntGenReplaceRandomly(int a_From, int a_To, int a_Chance, int a_Seed, Underlying a_Underlying):
		Super(a_Seed),
		m_From(a_From),
		m_To(a_To),
		m_Chance(a_Chance),
		m_Underlying(a_Underlying)
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, typename Super::Values & a_Values) override
	{
		// Generate the underlying values:
		m_Underlying->GetInts(a_MinX, a_MinZ, a_Values);

		// Replace some of the values:
		for (int z = 0; z < SizeZ; z++)
		{
			int idxZ = z * SizeX;
			for (int x = 0; x < SizeX; x++)
			{
				int idx = x + idxZ;
				if (a_Values[idx] == m_From)
				{
					int rnd = Super::m_Noise.IntNoise2DInt(x + a_MinX, z + a_MinZ) / 7;
					if (rnd % 1000 < m_Chance)
					{
						a_Values[idx] = m_To;
					}
				}
			}
		}  // for z
	}


protected:
	/** The original value to be replaced. */
	int m_From;

	/** The destination value to which to replace. */
	int m_To;

	/** Chance, in permille, of replacing the value. */
	int m_Chance;

	Underlying m_Underlying;
};





/** Mixer that joins together finalized biomes and rivers.
It first checks for oceans, if there is an ocean in the Biomes, it keeps the ocean.
If there's no ocean, it checks Rivers for a river, if there is a river, it uses the Biomes to select either
regular river or frozen river, based on the biome. */
template <int SizeX, int SizeZ = SizeX>
class cIntGenMixRivers:
	public cIntGen<SizeX, SizeZ>
{
	using Super = cIntGen<SizeX, SizeZ>;

public:

	using Underlying = std::shared_ptr<cIntGen<SizeX, SizeZ>>;


	cIntGenMixRivers(Underlying a_Biomes, Underlying a_Rivers):
		m_Biomes(a_Biomes),
		m_Rivers(a_Rivers)
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, typename Super::Values & a_Values) override
	{
		// Generate the underlying data:
		m_Biomes->GetInts(a_MinX, a_MinZ, a_Values);
		typename Super::Values riverData;
		m_Rivers->GetInts(a_MinX, a_MinZ, riverData);

		// Mix the values:
		for (int z = 0; z < SizeZ; z++)
		{
			int idxZ = z * SizeX;
			for (int x = 0; x < SizeX; x++)
			{
				int idx = x + idxZ;
				if (IsBiomeOcean(a_Values[idx]))
				{
					// Oceans are kept without any changes
					continue;
				}
				if (riverData[idx] != biRiver)
				{
					// There's no river, keep the current value
					continue;
				}

				// There's a river, change the output to a river or a frozen river, based on the original biome:
				if (IsBiomeVeryCold(static_cast<EMCSBiome>(a_Values[idx])))
				{
					a_Values[idx] = biFrozenRiver;
				}
				else
				{
					a_Values[idx] = biRiver;
				}
			}  // for x
		}  // for z
	}

protected:
	Underlying m_Biomes;
	Underlying m_Rivers;
};





/** Generates a river based on the underlying data.
This is basically an edge detector over the underlying data. The rivers are the edges where the underlying data
changes from one pixel to its neighbor. */
template <int SizeX, int SizeZ = SizeX>
class cIntGenRiver:
	public cIntGenWithNoise<SizeX, SizeZ>
{
	using Super = cIntGenWithNoise<SizeX, SizeZ>;

	static const int UnderlyingSizeX = SizeX + 2;
	static const int UnderlyingSizeZ = SizeZ + 2;

public:

	using Underlying = std::shared_ptr<cIntGen<UnderlyingSizeX, UnderlyingSizeZ>>;


	cIntGenRiver(int a_Seed, Underlying a_Underlying):
		Super(a_Seed),
		m_Underlying(a_Underlying)
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, typename Super::Values & a_Values) override
	{
		// Generate the underlying data:
		int Cache[UnderlyingSizeX * UnderlyingSizeZ];
		m_Underlying->GetInts(a_MinX - 1, a_MinZ - 1, Cache);

		// Detect the edges:
		for (int z = 0; z < SizeZ; z++)
		{
			for (int x = 0; x < SizeX; x++)
			{
				int Above = Cache[x + 1 + z * UnderlyingSizeX];
				int Below = Cache[x + 1 + (z + 2) * UnderlyingSizeX];
				int Left  = Cache[x +     (z + 1) * UnderlyingSizeX];
				int Right = Cache[x + 2 + (z + 1) * UnderlyingSizeX];
				int val   = Cache[x + 1 + (z + 1) * UnderlyingSizeX];

				if ((val == Above) && (val == Below) && (val == Left) && (val == Right))
				{
					val = 0;
				}
				else
				{
					val = biRiver;
				}
				a_Values[x + z * SizeX] = val;
			}  // for x
		}  // for z
	}

protected:
	Underlying m_Underlying;
};





/** Turns some of the oceans into the specified biome. Used for mushroom and deep ocean.
The biome is only placed if at least 3 of its neighbors are ocean and only with the specified chance. */
template <int SizeX, int SizeZ = SizeX>
class cIntGenAddToOcean:
	public cIntGenWithNoise<SizeX, SizeZ>
{
	using Super = cIntGenWithNoise<SizeX, SizeZ>;

	static const int UnderlyingSizeX = SizeX + 2;
	static const int UnderlyingSizeZ = SizeZ + 2;

public:

	using Underlying = std::shared_ptr<cIntGen<UnderlyingSizeX, UnderlyingSizeZ>>;


	cIntGenAddToOcean(int a_Seed, int a_Chance, int a_ToValue, Underlying a_Underlying):
		Super(a_Seed),
		m_Chance(a_Chance),
		m_ToValue(a_ToValue),
		m_Underlying(a_Underlying)
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, typename Super::Values & a_Values) override
	{
		// Generate the underlying data:
		int Cache[UnderlyingSizeX * UnderlyingSizeZ];
		m_Underlying->GetInts(a_MinX - 1, a_MinZ - 1, Cache);

		// Add the mushroom islands:
		for (int z = 0; z < SizeZ; z++)
		{
			for (int x = 0; x < SizeX; x++)
			{
				int val = Cache[x + 1 + (z + 1) * UnderlyingSizeX];
				if (!IsBiomeOcean(val))
				{
					a_Values[x + z * SizeX] = val;
					continue;
				}

				// Count the ocean neighbors:
				int Above = Cache[x + 1 + z * UnderlyingSizeX];
				int Below = Cache[x + 1 + (z + 2) * UnderlyingSizeX];
				int Left  = Cache[x +     (z + 1) * UnderlyingSizeX];
				int Right = Cache[x + 2 + (z + 1) * UnderlyingSizeX];
				int NumOceanNeighbors = 0;
				if (IsBiomeOcean(Above))
				{
					NumOceanNeighbors += 1;
				}
				if (IsBiomeOcean(Below))
				{
					NumOceanNeighbors += 1;
				}
				if (IsBiomeOcean(Left))
				{
					NumOceanNeighbors += 1;
				}
				if (IsBiomeOcean(Right))
				{
					NumOceanNeighbors += 1;
				}

				// If at least 3 ocean neighbors and the chance is right, change:
				if (
					(NumOceanNeighbors >= 3) &&
					((Super::m_Noise.IntNoise2DInt(x + a_MinX, z + a_MinZ) / 7) % 1000 < m_Chance)
				)
				{
					a_Values[x + z * SizeX] = m_ToValue;
				}
				else
				{
					a_Values[x + z * SizeX] = val;
				}
			}  // for x
		}  // for z
	}

protected:
	/** Chance, in permille, of changing the biome. */
	int m_Chance;

	/** The value to change the ocean into. */
	int m_ToValue;

	Underlying m_Underlying;
};





/** Changes random pixels of the underlying data to the specified value. */
template <int SizeX, int SizeZ = SizeX>
class cIntGenSetRandomly:
	public cIntGenWithNoise<SizeX, SizeZ>
{
	using Super = cIntGenWithNoise<SizeX, SizeZ>;

public:

	using Underlying = std::shared_ptr<cIntGen<SizeX, SizeZ>>;


	cIntGenSetRandomly(int a_Seed, int a_Chance, int a_ToValue, Underlying a_Underlying):
		Super(a_Seed),
		m_Chance(a_Chance),
		m_ToValue(a_ToValue),
		m_Underlying(a_Underlying)
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, typename Super::Values & a_Values) override
	{
		// Generate the underlying data:
		m_Underlying->GetInts(a_MinX, a_MinZ, a_Values);

		// Change random pixels to bgOcean:
		for (int z = 0; z < SizeZ; z++)
		{
			for (int x = 0; x < SizeX; x++)
			{
				int rnd = Super::m_Noise.IntNoise2DInt(x + a_MinX, z + a_MinZ) / 7;
				if (rnd % 1000 < m_Chance)
				{
					a_Values[x + z * SizeX] = m_ToValue;
				}
			}
		}
	}

protected:
	/** Chance, in permille, of changing each pixel. */
	int m_Chance;

	/** The value to which to set the pixel. */
	int m_ToValue;

	Underlying m_Underlying;
};





/** Adds a "rare" flag to random biome groups, based on the given chance. */
template <int SizeX, int SizeZ = SizeX>
class cIntGenRareBiomeGroups:
	public cIntGenWithNoise<SizeX, SizeZ>
{
	using Super = cIntGenWithNoise<SizeX, SizeZ>;

public:

	using Underlying = std::shared_ptr<cIntGen<SizeX, SizeZ>>;


	cIntGenRareBiomeGroups(int a_Seed, int a_Chance, Underlying a_Underlying):
		Super(a_Seed),
		m_Chance(a_Chance),
		m_Underlying(a_Underlying)
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, typename Super::Values & a_Values) override
	{
		// Generate the underlying data:
		m_Underlying->GetInts(a_MinX, a_MinZ, a_Values);

		// Change some of the biome groups into rare biome groups:
		for (int z = 0; z < SizeZ; z++)
		{
			for (int x = 0; x < SizeX; x++)
			{
				int rnd = Super::m_Noise.IntNoise2DInt(x + a_MinX, z + a_MinZ) / 7;
				if (rnd % 1000 < m_Chance)
				{
					int idx = x + SizeX * z;
					a_Values[idx] = a_Values[idx] | bgfRare;
				}
			}
		}
	}

protected:
	/** Chance, in permille, of changing each pixel into the rare biome group. */
	int m_Chance;

	/** The underlying generator. */
	Underlying m_Underlying;
};





/** Changes biomes in the parent data into an alternate versions (usually "hill" variants), in such places
that have their alterations set. */
template <int SizeX, int SizeZ = SizeX>
class cIntGenAlternateBiomes:
	public cIntGenWithNoise<SizeX, SizeZ>
{
	using Super = cIntGenWithNoise<SizeX, SizeZ>;

public:

	using Underlying = std::shared_ptr<cIntGen<SizeX, SizeZ>>;


	cIntGenAlternateBiomes(int a_Seed, Underlying a_Alterations, Underlying a_BaseBiomes):
		Super(a_Seed),
		m_Alterations(a_Alterations),
		m_BaseBiomes(a_BaseBiomes)
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, typename Super::Values & a_Values) override
	{
		// Generate the base biomes and the alterations:
		m_BaseBiomes->GetInts(a_MinX, a_MinZ, a_Values);
		typename Super::Values alterations;
		m_Alterations->GetInts(a_MinX, a_MinZ, alterations);

		// Change the biomes into their alternate versions:
		for (int idx = 0; idx < SizeX * SizeZ; ++idx)
		{
			if (alterations[idx] == 0)
			{
				// No change
				continue;
			}

			// Change to alternate biomes:
			int val = a_Values[idx];
			switch (val)
			{
				case biBirchForest:   val = biBirchForestHills; break;
				case biDesert:        val = biDesertHills;      break;
				case biExtremeHills:  val = biExtremeHillsPlus; break;
				case biForest:        val = biForestHills;      break;
				case biIcePlains:     val = biIceMountains;     break;
				case biJungle:        val = biJungleHills;      break;
				case biMegaTaiga:     val = biMegaTaigaHills;   break;
				case biMesaPlateau:   val = biMesa;             break;
				case biMesaPlateauF:  val = biMesa;             break;
				case biMesaPlateauM:  val = biMesa;             break;
				case biMesaPlateauFM: val = biMesa;             break;
				case biPlains:        val = biForest;           break;
				case biRoofedForest:  val = biPlains;           break;
				case biSavanna:       val = biSavannaPlateau;   break;
				case biTaiga:         val = biTaigaHills;       break;
			}
			a_Values[idx] = val;
		}  // for idx - a_Values[]
	}

protected:
	Underlying m_Alterations;
	Underlying m_BaseBiomes;
};





/** Adds an edge between two specifically incompatible biomes, such as mesa and forest. */
template <int SizeX, int SizeZ = SizeX>
class cIntGenBiomeEdges:
	public cIntGenWithNoise<SizeX, SizeZ>
{
	using Super = cIntGenWithNoise<SizeX, SizeZ>;

	static const int m_LowerSizeX = SizeX + 2;
	static const int m_LowerSizeZ = SizeZ + 2;

public:

	using Underlying = std::shared_ptr<cIntGen<m_LowerSizeX, m_LowerSizeZ>>;


	cIntGenBiomeEdges(int a_Seed, Underlying a_Underlying):
		Super(a_Seed),
		m_Underlying(a_Underlying)
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, typename Super::Values & a_Values) override
	{
		// Generate the underlying biomes:
		typename Underlying::element_type::Values lowerValues;
		m_Underlying->GetInts(a_MinX - 1, a_MinZ - 1, lowerValues);

		// Convert incompatible edges into neutral biomes:
		for (int z = 0; z < SizeZ; z++)
		{
			for (int x = 0; x < SizeX; x++)
			{
				int biome = lowerValues[x + 1 + (z + 1) * m_LowerSizeX];
				int above = lowerValues[x + 1 + z *       m_LowerSizeX];
				int below = lowerValues[x + 1 + (z + 2) * m_LowerSizeX];
				int left  = lowerValues[x +     (z + 1) * m_LowerSizeX];
				int right = lowerValues[x + 2 + (z + 1) * m_LowerSizeX];

				switch (biome)
				{
					case biDesert:
					case biDesertM:
					case biDesertHills:
					{
						if (
							IsBiomeVeryCold(static_cast<EMCSBiome>(above)) ||
							IsBiomeVeryCold(static_cast<EMCSBiome>(below)) ||
							IsBiomeVeryCold(static_cast<EMCSBiome>(left))  ||
							IsBiomeVeryCold(static_cast<EMCSBiome>(right))
						)
						{
							biome = biPlains;
						}
						break;
					}  // case biDesert

					case biMesaPlateau:
					case biMesaPlateauF:
					case biMesaPlateauFM:
					case biMesaPlateauM:
					{
						if (
							!isMesaCompatible(above) ||
							!isMesaCompatible(below) ||
							!isMesaCompatible(left)  ||
							!isMesaCompatible(right)
						)
						{
							biome = biDesert;
						}
						break;
					}  // Mesa biomes

					case biJungle:
					case biJungleM:
					{
						if (
							!isJungleCompatible(above) ||
							!isJungleCompatible(below) ||
							!isJungleCompatible(left)  ||
							!isJungleCompatible(right)
						)
						{
							biome = (biome == biJungle) ? biJungleEdge : biJungleEdgeM;
						}
						break;
					}  // Jungle biomes

					case biSwampland:
					case biSwamplandM:
					{
						if (
							IsBiomeNoDownfall(static_cast<EMCSBiome>(above)) ||
							IsBiomeNoDownfall(static_cast<EMCSBiome>(below)) ||
							IsBiomeNoDownfall(static_cast<EMCSBiome>(left))  ||
							IsBiomeNoDownfall(static_cast<EMCSBiome>(right))
						)
						{
							biome = biPlains;
						}
						break;
					}  // Swampland biomes
				}  // switch (biome)

				a_Values[x + z * SizeX] = biome;
			}  // for x
		}  // for z
	}


protected:
	Underlying m_Underlying;


	bool isMesaCompatible(int a_Biome)
	{
		switch (a_Biome)
		{
			case biDesert:
			case biMesa:
			case biMesaBryce:
			case biMesaPlateau:
			case biMesaPlateauF:
			case biMesaPlateauFM:
			case biMesaPlateauM:
			case biOcean:
			case biDeepOcean:
			{
				return true;
			}
			default:
			{
				return false;
			}
		}
	}


	bool isJungleCompatible(int a_Biome)
	{
		switch (a_Biome)
		{
			case biJungle:
			case biJungleM:
			case biJungleEdge:
			case biJungleEdgeM:
			case biJungleHills:
			{
				return true;
			}
			default:
			{
				return false;
			}
		}
	}
};





/** Changes biomes in the parent data into their alternate versions ("M" variants), in such places that
have their alterations set. */
template <int SizeX, int SizeZ = SizeX>
class cIntGenMBiomes:
	public cIntGenWithNoise<SizeX, SizeZ>
{
	using Super = cIntGenWithNoise<SizeX, SizeZ>;

public:

	using Underlying = std::shared_ptr<cIntGen<SizeX, SizeZ>>;


	cIntGenMBiomes(int a_Seed, Underlying a_Alteration, Underlying a_Underlying):
		Super(a_Seed),
		m_Underlying(a_Underlying),
		m_Alteration(a_Alteration)
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, typename Super::Values & a_Values) override
	{
		// Generate the underlying biomes and the alterations:
		m_Underlying->GetInts(a_MinX, a_MinZ, a_Values);
		typename Super::Values alterations;
		m_Alteration->GetInts(a_MinX, a_MinZ, alterations);

		// Wherever alterations are nonzero, change into alternate biome, if available:
		for (int idx = 0; idx < SizeX * SizeZ; ++idx)
		{
			if (alterations[idx] == 0)
			{
				continue;
			}

			// Ice spikes biome was removed from here, because it was generated way too often
			switch (a_Values[idx])
			{
				case biPlains:               a_Values[idx] = biSunflowerPlains;      break;
				case biDesert:               a_Values[idx] = biDesertM;              break;
				case biExtremeHills:         a_Values[idx] = biExtremeHillsM;        break;
				case biForest:               a_Values[idx] = biFlowerForest;         break;
				case biTaiga:                a_Values[idx] = biTaigaM;               break;
				case biSwampland:            a_Values[idx] = biSwamplandM;           break;
				case biJungle:               a_Values[idx] = biJungleM;              break;
				case biJungleEdge:           a_Values[idx] = biJungleEdgeM;          break;
				case biBirchForest:          a_Values[idx] = biBirchForestM;         break;
				case biBirchForestHills:     a_Values[idx] = biBirchForestHillsM;    break;
				case biRoofedForest:         a_Values[idx] = biRoofedForestM;        break;
				case biColdTaiga:            a_Values[idx] = biColdTaigaM;           break;
				case biMegaSpruceTaiga:      a_Values[idx] = biMegaSpruceTaiga;      break;
				case biMegaSpruceTaigaHills: a_Values[idx] = biMegaSpruceTaigaHills; break;
				case biExtremeHillsPlus:     a_Values[idx] = biExtremeHillsPlusM;    break;
				case biSavanna:              a_Values[idx] = biSavannaM;             break;
				case biSavannaPlateau:       a_Values[idx] = biSavannaPlateauM;      break;
				case biMesa:                 a_Values[idx] = biMesaBryce;            break;
				case biMesaPlateauF:         a_Values[idx] = biMesaPlateauFM;        break;
				case biMesaPlateau:          a_Values[idx] = biMesaBryce;            break;
			}
		}  // for idx - a_Values[] / alterations[]
	}

protected:
	Underlying m_Underlying;
	Underlying m_Alteration;
};
