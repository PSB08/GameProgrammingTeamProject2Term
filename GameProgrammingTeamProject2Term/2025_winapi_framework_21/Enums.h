#pragma once
enum class Layer
{
	DEFAULT,
	BACKGROUND,
	PLAYER,
	ENEMY,
	BOSS,
	BOSSCORE,
	PROJECTILE,
	BOSSPROJECTILE,
	LASER,
	END
};

enum class PenType
{
	RED, GREEN, END
};

enum class BrushType
{
	HOLLOW, RED, GREEN, END
};

enum class FontType
{
	UI, TITLE, END
};

enum class PlayMode { Once, Loop, Counted };

enum class Boss1Pattern
{
	NONE,
	PATTERN1,
	PATTERN2,
	PATTERN3,
	END
};