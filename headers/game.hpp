#pragma once
#include <cstdint>
#include <utility>

#define PAWN 0
#define KNIGHT 1
#define BISHOP 2
#define ROOK 3
#define QUEEN 4
#define KING 5

#define WHITE 0
#define BLACK 1
#define EMPTY 2

#define HAS_MOVED 0x01
#define ADVANCED_TWO_SQUARES 0x02

#define PAWN_VALUE 100
#define KNIGHT_VALUE 300
#define BISHOP_VALUE 300
#define ROOK_VALUE 500
#define QUEEN_VALUE 900

#define CHECKMATE_VALUE 32000 // 320 Pawns

struct position {
	int x, y;
};

typedef piece board[8][8];

class move {

	std::pair<position, position> positions;
	uint8_t specialMoves; // 1: castling 2: en passant

};

class piece {

	uint8_t type; // 0: Pawn, 1: Knight, 2: Bishop, 3: Rook, 4: Queen, 5: King
	uint8_t color; // 0: White, 1: Black, 2: Empty Square
	/*
	Flags:
	0x01 - Has moved (for castling)
	0x02 - Last move was advancing two squares (only for pawn, en passent)
	*/
	uint16_t flags;

	int getValue();

};