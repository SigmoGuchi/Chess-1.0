#include "Piece.h"

std::string PieceTypeToString(PieceType type) {
	switch (type) {
	case PieceType::King:   return "King";
	case PieceType::Queen:  return "Queen";
	case PieceType::Rook:   return "Rook";
	case PieceType::Bishop: return "Bishop";
	case PieceType::Knight: return "Knight";
	case PieceType::Pawn:   return "Pawn";
	default:                return "Unknown";
	}
}

	Piece::Piece(PieceType type, char color) :type(type), color(color),firstMove(true) {}
	Piece::~Piece() {}

	void Piece::LoadTexture(const std::string& filePath) {
		if (!texture.loadFromFile(filePath)) {
			std::cout << "Error loading textures" << std::endl;
		}
		else {
			sprite.setTexture(texture);
		}
	}