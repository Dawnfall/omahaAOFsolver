#pragma once
enum class Position
{
	CO,
	BU,
	SB,
	BB,
	TOTAL
};

// Function to increment the enum value with wrap-around
inline Position operator+(Position pos, int increment)
{
	return static_cast<Position>((static_cast<int>(pos) + increment) % static_cast<int>(Position::TOTAL));
}

// Function to increment the enum value with wrap-around
inline Position& operator++(Position& pos)
{
	pos = static_cast<Position>((static_cast<int>(pos) + 1) % static_cast<int>(Position::TOTAL));
	return pos;
}

// Function to convert Position enum to string
inline std::string toString(Position pos)
{
    switch (pos)
    {
    case Position::CO: return "CO";
    case Position::BU: return "BU";
    case Position::SB: return "SB";
    case Position::BB: return "BB";
    case Position::TOTAL: return "TOTAL";
    default: return "Unknown";
    }
}