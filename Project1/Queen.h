#pragma once
#include"Piece.h"
class Queen:public Piece{
public:
	Queen(char color);
	~Queen();

	bool IsMoveAllowed(int ox, int oy, int nx, int ny, const Board& board) const override;
	std::vector<std::pair<int, int>> GetAvailableMoves(int x, int y, Board& board) const override;
};

