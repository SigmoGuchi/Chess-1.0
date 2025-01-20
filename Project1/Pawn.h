#pragma once
#include "Piece.h"
class Pawn:public Piece{
public:
	Pawn(char color);
	~Pawn();

	bool IsMoveAllowed(int ox, int oy, int nx, int ny, const Board& board) const override;
	std::vector<std::pair<int, int>> GetAvailableMoves(int x, int y, Board& board) const override;

	void HandlePromotion(Board& board, int x, int y, sf::RenderWindow& window);
	void Promote(Board& board,int x,int y,PieceType& type);
	PieceType PromptPromotionChoiceWithUI(int x, int y, RenderWindow& window,Board& board);
};

