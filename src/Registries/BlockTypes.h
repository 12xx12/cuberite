#pragma once

enum BlockType
{
	AcaciaButton,
	AcaciaDoor,
	AcaciaFence,
	AcaciaFenceGate,
	AcaciaLeaves,
	AcaciaLog,
	AcaciaPlanks,
	AcaciaPressurePlate,
	AcaciaSapling,
	AcaciaSign,
	AcaciaSlab,
	AcaciaStairs,
	AcaciaTrapdoor,
	AcaciaWallSign,
	AcaciaWood,
	ActivatorRail,
	Air,
	Allium,
	AncientDebris,
	Andesite,
	AndesiteSlab,
	AndesiteStairs,
	AndesiteWall,
	Anvil,
	AttachedMelonStem,
	AttachedPumpkinStem,
	AzureBluet,
	Bamboo,
	BambooSapling,
	Barrel,
	Barrier,
	Basalt,
	Beacon,
	Bedrock,
	BeeNest,
	Beehive,
	Beetroots,
	Bell,
	BirchButton,
	BirchDoor,
	BirchFence,
	BirchFenceGate,
	BirchLeaves,
	BirchLog,
	BirchPlanks,
	BirchPressurePlate,
	BirchSapling,
	BirchSign,
	BirchSlab,
	BirchStairs,
	BirchTrapdoor,
	BirchWallSign,
	BirchWood,
	BlackBanner,
	BlackBed,
	BlackCarpet,
	BlackConcrete,
	BlackConcretePowder,
	BlackGlazedTerracotta,
	BlackShulkerBox,
	BlackStainedGlass,
	BlackStainedGlassPane,
	BlackTerracotta,
	BlackWallBanner,
	BlackWool,
	Blackstone,
	BlackstoneSlab,
	BlackstoneStairs,
	BlackstoneWall,
	BlastFurnace,
	BlueBanner,
	BlueBed,
	BlueCarpet,
	BlueConcrete,
	BlueConcretePowder,
	BlueGlazedTerracotta,
	BlueIce,
	BlueOrchid,
	BlueShulkerBox,
	BlueStainedGlass,
	BlueStainedGlassPane,
	BlueTerracotta,
	BlueWallBanner,
	BlueWool,
	BoneBlock,
	Bookshelf,
	BrainCoral,
	BrainCoralBlock,
	BrainCoralFan,
	BrainCoralWallFan,
	BrewingStand,
	BrickSlab,
	BrickStairs,
	BrickWall,
	Bricks,
	BrownBanner,
	BrownBed,
	BrownCarpet,
	BrownConcrete,
	BrownConcretePowder,
	BrownGlazedTerracotta,
	BrownMushroom,
	BrownMushroomBlock,
	BrownShulkerBox,
	BrownStainedGlass,
	BrownStainedGlassPane,
	BrownTerracotta,
	BrownWallBanner,
	BrownWool,
	BubbleColumn,
	BubbleCoral,
	BubbleCoralBlock,
	BubbleCoralFan,
	BubbleCoralWallFan,
	Cactus,
	Cake,
	Campfire,
	Carrots,
	CartographyTable,
	CarvedPumpkin,
	Cauldron,
	CaveAir,
	Chain,
	ChainCommandBlock,
	Chest,
	ChippedAnvil,
	ChiseledNetherBricks,
	ChiseledPolishedBlackstone,
	ChiseledQuartzBlock,
	ChiseledRedSandstone,
	ChiseledSandstone,
	ChiseledStoneBricks,
	ChorusFlower,
	ChorusPlant,
	Clay,
	CoalBlock,
	CoalOre,
	CoarseDirt,
	Cobblestone,
	CobblestoneSlab,
	CobblestoneStairs,
	CobblestoneWall,
	Cobweb,
	Cocoa,
	CommandBlock,
	Comparator,
	Composter,
	Conduit,
	Cornflower,
	CrackedNetherBricks,
	CrackedPolishedBlackstoneBricks,
	CrackedStoneBricks,
	CraftingTable,
	CreeperHead,
	CreeperWallHead,
	CrimsonButton,
	CrimsonDoor,
	CrimsonFence,
	CrimsonFenceGate,
	CrimsonFungus,
	CrimsonHyphae,
	CrimsonNylium,
	CrimsonPlanks,
	CrimsonPressurePlate,
	CrimsonRoots,
	CrimsonSign,
	CrimsonSlab,
	CrimsonStairs,
	CrimsonStem,
	CrimsonTrapdoor,
	CrimsonWallSign,
	CryingObsidian,
	CutRedSandstone,
	CutRedSandstoneSlab,
	CutSandstone,
	CutSandstoneSlab,
	CyanBanner,
	CyanBed,
	CyanCarpet,
	CyanConcrete,
	CyanConcretePowder,
	CyanGlazedTerracotta,
	CyanShulkerBox,
	CyanStainedGlass,
	CyanStainedGlassPane,
	CyanTerracotta,
	CyanWallBanner,
	CyanWool,
	DamagedAnvil,
	Dandelion,
	DarkOakButton,
	DarkOakDoor,
	DarkOakFence,
	DarkOakFenceGate,
	DarkOakLeaves,
	DarkOakLog,
	DarkOakPlanks,
	DarkOakPressurePlate,
	DarkOakSapling,
	DarkOakSign,
	DarkOakSlab,
	DarkOakStairs,
	DarkOakTrapdoor,
	DarkOakWallSign,
	DarkOakWood,
	DarkPrismarine,
	DarkPrismarineSlab,
	DarkPrismarineStairs,
	DaylightDetector,
	DeadBrainCoral,
	DeadBrainCoralBlock,
	DeadBrainCoralFan,
	DeadBrainCoralWallFan,
	DeadBubbleCoral,
	DeadBubbleCoralBlock,
	DeadBubbleCoralFan,
	DeadBubbleCoralWallFan,
	DeadBush,
	DeadFireCoral,
	DeadFireCoralBlock,
	DeadFireCoralFan,
	DeadFireCoralWallFan,
	DeadHornCoral,
	DeadHornCoralBlock,
	DeadHornCoralFan,
	DeadHornCoralWallFan,
	DeadTubeCoral,
	DeadTubeCoralBlock,
	DeadTubeCoralFan,
	DeadTubeCoralWallFan,
	DetectorRail,
	DiamondBlock,
	DiamondOre,
	Diorite,
	DioriteSlab,
	DioriteStairs,
	DioriteWall,
	Dirt,
	Dispenser,
	DragonEgg,
	DragonHead,
	DragonWallHead,
	DriedKelpBlock,
	Dropper,
	EmeraldBlock,
	EmeraldOre,
	EnchantingTable,
	EndGateway,
	EndPortal,
	EndPortalFrame,
	EndRod,
	EndStone,
	EndStoneBrickSlab,
	EndStoneBrickStairs,
	EndStoneBrickWall,
	EndStoneBricks,
	EnderChest,
	Farmland,
	Fern,
	Fire,
	FireCoral,
	FireCoralBlock,
	FireCoralFan,
	FireCoralWallFan,
	FletchingTable,
	FlowerPot,
	FrostedIce,
	Furnace,
	GildedBlackstone,
	Glass,
	GlassPane,
	Glowstone,
	GoldBlock,
	GoldOre,
	Granite,
	GraniteSlab,
	GraniteStairs,
	GraniteWall,
	Grass,
	GrassBlock,
	GrassPath,
	Gravel,
	GrayBanner,
	GrayBed,
	GrayCarpet,
	GrayConcrete,
	GrayConcretePowder,
	GrayGlazedTerracotta,
	GrayShulkerBox,
	GrayStainedGlass,
	GrayStainedGlassPane,
	GrayTerracotta,
	GrayWallBanner,
	GrayWool,
	GreenBanner,
	GreenBed,
	GreenCarpet,
	GreenConcrete,
	GreenConcretePowder,
	GreenGlazedTerracotta,
	GreenShulkerBox,
	GreenStainedGlass,
	GreenStainedGlassPane,
	GreenTerracotta,
	GreenWallBanner,
	GreenWool,
	Grindstone,
	HayBale,
	HeavyWeightedPressurePlate,
	HoneyBlock,
	HoneycombBlock,
	Hopper,
	HornCoral,
	HornCoralBlock,
	HornCoralFan,
	HornCoralWallFan,
	Ice,
	InfestedChiseledStoneBricks,
	InfestedCobblestone,
	InfestedCrackedStoneBricks,
	InfestedMossyStoneBricks,
	InfestedStone,
	InfestedStoneBricks,
	IronBars,
	IronBlock,
	IronDoor,
	IronOre,
	IronTrapdoor,
	JackOLantern,
	Jigsaw,
	Jukebox,
	JungleButton,
	JungleDoor,
	JungleFence,
	JungleFenceGate,
	JungleLeaves,
	JungleLog,
	JunglePlanks,
	JunglePressurePlate,
	JungleSapling,
	JungleSign,
	JungleSlab,
	JungleStairs,
	JungleTrapdoor,
	JungleWallSign,
	JungleWood,
	Kelp,
	KelpPlant,
	Ladder,
	Lantern,
	LapisBlock,
	LapisOre,
	LargeFern,
	Lava,
	Lectern,
	Lever,
	LightBlueBanner,
	LightBlueBed,
	LightBlueCarpet,
	LightBlueConcrete,
	LightBlueConcretePowder,
	LightBlueGlazedTerracotta,
	LightBlueShulkerBox,
	LightBlueStainedGlass,
	LightBlueStainedGlassPane,
	LightBlueTerracotta,
	LightBlueWallBanner,
	LightBlueWool,
	LightGrayBanner,
	LightGrayBed,
	LightGrayCarpet,
	LightGrayConcrete,
	LightGrayConcretePowder,
	LightGrayGlazedTerracotta,
	LightGrayShulkerBox,
	LightGrayStainedGlass,
	LightGrayStainedGlassPane,
	LightGrayTerracotta,
	LightGrayWallBanner,
	LightGrayWool,
	LightWeightedPressurePlate,
	Lilac,
	LilyOfTheValley,
	LilyPad,
	LimeBanner,
	LimeBed,
	LimeCarpet,
	LimeConcrete,
	LimeConcretePowder,
	LimeGlazedTerracotta,
	LimeShulkerBox,
	LimeStainedGlass,
	LimeStainedGlassPane,
	LimeTerracotta,
	LimeWallBanner,
	LimeWool,
	Lodestone,
	Loom,
	MagentaBanner,
	MagentaBed,
	MagentaCarpet,
	MagentaConcrete,
	MagentaConcretePowder,
	MagentaGlazedTerracotta,
	MagentaShulkerBox,
	MagentaStainedGlass,
	MagentaStainedGlassPane,
	MagentaTerracotta,
	MagentaWallBanner,
	MagentaWool,
	MagmaBlock,
	Melon,
	MelonStem,
	MossyCobblestone,
	MossyCobblestoneSlab,
	MossyCobblestoneStairs,
	MossyCobblestoneWall,
	MossyStoneBrickSlab,
	MossyStoneBrickStairs,
	MossyStoneBrickWall,
	MossyStoneBricks,
	MovingPiston,
	MushroomStem,
	Mycelium,
	NetherBrickFence,
	NetherBrickSlab,
	NetherBrickStairs,
	NetherBrickWall,
	NetherBricks,
	NetherGoldOre,
	NetherPortal,
	NetherQuartzOre,
	NetherSprouts,
	NetherWart,
	NetherWartBlock,
	NetheriteBlock,
	Netherrack,
	NoteBlock,
	OakButton,
	OakDoor,
	OakFence,
	OakFenceGate,
	OakLeaves,
	OakLog,
	OakPlanks,
	OakPressurePlate,
	OakSapling,
	OakSign,
	OakSlab,
	OakStairs,
	OakTrapdoor,
	OakWallSign,
	OakWood,
	Observer,
	Obsidian,
	OrangeBanner,
	OrangeBed,
	OrangeCarpet,
	OrangeConcrete,
	OrangeConcretePowder,
	OrangeGlazedTerracotta,
	OrangeShulkerBox,
	OrangeStainedGlass,
	OrangeStainedGlassPane,
	OrangeTerracotta,
	OrangeTulip,
	OrangeWallBanner,
	OrangeWool,
	OxeyeDaisy,
	PackedIce,
	Peony,
	PetrifiedOakSlab,
	PinkBanner,
	PinkBed,
	PinkCarpet,
	PinkConcrete,
	PinkConcretePowder,
	PinkGlazedTerracotta,
	PinkShulkerBox,
	PinkStainedGlass,
	PinkStainedGlassPane,
	PinkTerracotta,
	PinkTulip,
	PinkWallBanner,
	PinkWool,
	Piston,
	PistonHead,
	PlayerHead,
	PlayerWallHead,
	Podzol,
	PolishedAndesite,
	PolishedAndesiteSlab,
	PolishedAndesiteStairs,
	PolishedBasalt,
	PolishedBlackstone,
	PolishedBlackstoneBrickSlab,
	PolishedBlackstoneBrickStairs,
	PolishedBlackstoneBrickWall,
	PolishedBlackstoneBricks,
	PolishedBlackstoneButton,
	PolishedBlackstonePressurePlate,
	PolishedBlackstoneSlab,
	PolishedBlackstoneStairs,
	PolishedBlackstoneWall,
	PolishedDiorite,
	PolishedDioriteSlab,
	PolishedDioriteStairs,
	PolishedGranite,
	PolishedGraniteSlab,
	PolishedGraniteStairs,
	Poppy,
	Potatoes,
	PottedAcaciaSapling,
	PottedAllium,
	PottedAzureBluet,
	PottedBamboo,
	PottedBirchSapling,
	PottedBlueOrchid,
	PottedBrownMushroom,
	PottedCactus,
	PottedCornflower,
	PottedCrimsonFungus,
	PottedCrimsonRoots,
	PottedDandelion,
	PottedDarkOakSapling,
	PottedDeadBush,
	PottedFern,
	PottedJungleSapling,
	PottedLilyOfTheValley,
	PottedOakSapling,
	PottedOrangeTulip,
	PottedOxeyeDaisy,
	PottedPinkTulip,
	PottedPoppy,
	PottedRedMushroom,
	PottedRedTulip,
	PottedSpruceSapling,
	PottedWarpedFungus,
	PottedWarpedRoots,
	PottedWhiteTulip,
	PottedWitherRose,
	PoweredRail,
	Prismarine,
	PrismarineBrickSlab,
	PrismarineBrickStairs,
	PrismarineBricks,
	PrismarineSlab,
	PrismarineStairs,
	PrismarineWall,
	Pumpkin,
	PumpkinStem,
	PurpleBanner,
	PurpleBed,
	PurpleCarpet,
	PurpleConcrete,
	PurpleConcretePowder,
	PurpleGlazedTerracotta,
	PurpleShulkerBox,
	PurpleStainedGlass,
	PurpleStainedGlassPane,
	PurpleTerracotta,
	PurpleWallBanner,
	PurpleWool,
	PurpurBlock,
	PurpurPillar,
	PurpurSlab,
	PurpurStairs,
	QuartzBlock,
	QuartzBricks,
	QuartzPillar,
	QuartzSlab,
	QuartzStairs,
	Rail,
	RedBanner,
	RedBed,
	RedCarpet,
	RedConcrete,
	RedConcretePowder,
	RedGlazedTerracotta,
	RedMushroom,
	RedMushroomBlock,
	RedNetherBrickSlab,
	RedNetherBrickStairs,
	RedNetherBrickWall,
	RedNetherBricks,
	RedSand,
	RedSandstone,
	RedSandstoneSlab,
	RedSandstoneStairs,
	RedSandstoneWall,
	RedShulkerBox,
	RedStainedGlass,
	RedStainedGlassPane,
	RedTerracotta,
	RedTulip,
	RedWallBanner,
	RedWool,
	RedstoneBlock,
	RedstoneLamp,
	RedstoneOre,
	RedstoneTorch,
	RedstoneWallTorch,
	RedstoneWire,
	Repeater,
	RepeatingCommandBlock,
	RespawnAnchor,
	RoseBush,
	Sand,
	Sandstone,
	SandstoneSlab,
	SandstoneStairs,
	SandstoneWall,
	Scaffolding,
	SeaLantern,
	SeaPickle,
	Seagrass,
	Shroomlight,
	ShulkerBox,
	SkeletonSkull,
	SkeletonWallSkull,
	SlimeBlock,
	SmithingTable,
	Smoker,
	SmoothQuartz,
	SmoothQuartzSlab,
	SmoothQuartzStairs,
	SmoothRedSandstone,
	SmoothRedSandstoneSlab,
	SmoothRedSandstoneStairs,
	SmoothSandstone,
	SmoothSandstoneSlab,
	SmoothSandstoneStairs,
	SmoothStone,
	SmoothStoneSlab,
	Snow,
	SnowBlock,
	SoulCampfire,
	SoulFire,
	SoulLantern,
	SoulSand,
	SoulSoil,
	SoulTorch,
	SoulWallTorch,
	Spawner,
	Sponge,
	SpruceButton,
	SpruceDoor,
	SpruceFence,
	SpruceFenceGate,
	SpruceLeaves,
	SpruceLog,
	SprucePlanks,
	SprucePressurePlate,
	SpruceSapling,
	SpruceSign,
	SpruceSlab,
	SpruceStairs,
	SpruceTrapdoor,
	SpruceWallSign,
	SpruceWood,
	StickyPiston,
	Stone,
	StoneBrickSlab,
	StoneBrickStairs,
	StoneBrickWall,
	StoneBricks,
	StoneButton,
	StonePressurePlate,
	StoneSlab,
	StoneStairs,
	Stonecutter,
	StrippedAcaciaLog,
	StrippedAcaciaWood,
	StrippedBirchLog,
	StrippedBirchWood,
	StrippedCrimsonHyphae,
	StrippedCrimsonStem,
	StrippedDarkOakLog,
	StrippedDarkOakWood,
	StrippedJungleLog,
	StrippedJungleWood,
	StrippedOakLog,
	StrippedOakWood,
	StrippedSpruceLog,
	StrippedSpruceWood,
	StrippedWarpedHyphae,
	StrippedWarpedStem,
	StructureBlock,
	StructureVoid,
	SugarCane,
	Sunflower,
	SweetBerryBush,
	TNT,
	TallGrass,
	TallSeagrass,
	Target,
	Terracotta,
	Torch,
	TrappedChest,
	Tripwire,
	TripwireHook,
	TubeCoral,
	TubeCoralBlock,
	TubeCoralFan,
	TubeCoralWallFan,
	TurtleEgg,
	TwistingVines,
	TwistingVinesPlant,
	Vine,
	VoidAir,
	WallTorch,
	WarpedButton,
	WarpedDoor,
	WarpedFence,
	WarpedFenceGate,
	WarpedFungus,
	WarpedHyphae,
	WarpedNylium,
	WarpedPlanks,
	WarpedPressurePlate,
	WarpedRoots,
	WarpedSign,
	WarpedSlab,
	WarpedStairs,
	WarpedStem,
	WarpedTrapdoor,
	WarpedWallSign,
	WarpedWartBlock,
	Water,
	WeepingVines,
	WeepingVinesPlant,
	WetSponge,
	Wheat,
	WhiteBanner,
	WhiteBed,
	WhiteCarpet,
	WhiteConcrete,
	WhiteConcretePowder,
	WhiteGlazedTerracotta,
	WhiteShulkerBox,
	WhiteStainedGlass,
	WhiteStainedGlassPane,
	WhiteTerracotta,
	WhiteTulip,
	WhiteWallBanner,
	WhiteWool,
	WitherRose,
	WitherSkeletonSkull,
	WitherSkeletonWallSkull,
	YellowBanner,
	YellowBed,
	YellowCarpet,
	YellowConcrete,
	YellowConcretePowder,
	YellowGlazedTerracotta,
	YellowShulkerBox,
	YellowStainedGlass,
	YellowStainedGlassPane,
	YellowTerracotta,
	YellowWallBanner,
	YellowWool,
	ZombieHead,
	ZombieWallHead
};
