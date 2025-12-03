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
	BUTTON,
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
	UI, TITLE, TITLE_BIG, END
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

enum class Boss2Pattern
{
	NONE,
	PATTERN1,
	PATTERN2,
	PATTERN3,
	PATTERN4,
	PATTERN5,
	END
};

enum class Boss3Pattern
{
	NONE,
	STARTPATTERN,
	PATTERN1,
	PATTERN2,
	PATTERN3,
	PATTERN4,
	PATTERN5,
	END
};