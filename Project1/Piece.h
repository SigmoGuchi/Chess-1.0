#pragma once
#include<SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
class Board;

enum class PieceType {
	King = 1,
	Queen = 2,
	Rook = 3,
	Bishop = 4,
	Knight = 5,
	Pawn = 6
};
std::string PieceTypeToString(PieceType type);

class Piece
{
protected:
	PieceType type;
	char color; // W or B
	bool firstMove;

	Texture texture;
	Sprite sprite;
public:
	Piece(PieceType type, char color);
	virtual ~Piece();

	void LoadTexture(const std::string& filePath);
	virtual bool IsMoveAllowed(int ox, int oy, int nx, int ny, const Board& board) const = 0;
	virtual std::vector<std::pair<int, int>> GetAvailableMoves(int x, int y, Board& board) const = 0;

	//Ґетери
	char GetColor() const { return color; }
	Sprite& GetSprite() {
		return sprite;
		std::cout << "Спрайт" << std::endl;
	}
	PieceType GetType() const { return type; }
	std::string GetTypeStr() { return PieceTypeToString(type); }
	bool IsFirstMove() const { return firstMove; }

	//Cетери
	void SetFirstMove(bool status)  { firstMove = status; }

};