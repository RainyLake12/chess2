#include "../headers/game.hpp"
#include <utility>

int piece::getValue() {
	switch (type) {
		case PAWN: return PAWN_VALUE;
		case KNIGHT: return KNIGHT_VALUE;
		case BISHOP: return BISHOP_VALUE;
		case ROOK: return ROOK_VALUE;
		case QUEEN: return QUEEN_VALUE;
		case KING: return CHECKMATE_VALUE;
		default: return 0; // Invalid piece type
	}
}