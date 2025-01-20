#pragma once
#include "Piece.h"

class King :public Piece {
public:
	King(char color);
	~King();

	bool IsMoveAllowed(int ox, int oy, int nx, int ny, const Board& board) const override;
	bool IsInCheck(const Board& board)const;
	bool IsCheckmate(Board& board)const;
	std::vector<std::pair<int, int>> GetAvailableMoves(int x, int y, Board& board) const override;
};