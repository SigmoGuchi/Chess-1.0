#include "Bishop.h"
#include "Board.h"

Bishop::Bishop(char color) :Piece(PieceType::Bishop, color) {

}

Bishop::~Bishop() {}

bool Bishop::IsMoveAllowed(int ox, int oy, int nx, int ny, const Board& board) const{
	int dx = std::abs(nx - ox);
	int dy = std::abs(ny - oy); 

	if (dx != dy) {
		return false;
	}

	int xDirection = (nx > ox) ? 1 : -1;
	int yDirection = (ny > oy) ? 1 : -1;
	int steps = dx;

	for (int i = 1; i < steps; ++i) {
		int checkX = ox + i * xDirection;
		int checkY = oy + i * yDirection;

		if (board.GetPieceAt(checkX, checkY) != nullptr) {
			return false;
		}
	}

	const Piece* targetPiece = board.GetPieceAt(nx, ny);
	if (targetPiece != nullptr && targetPiece->GetColor() == this->GetColor()) {
		return false;
	}

	return true;
}
std::vector<std::pair<int, int>> Bishop::GetAvailableMoves(int x, int y, Board& board) const {
    std::vector<std::pair<int, int>> availableMoves;

	for (int nx = 0; nx < 8; ++nx) {
		for (int ny = 0; ny < 8; ++ny) {
			if (abs(nx - x) == abs(ny - y) && IsMoveAllowed(x, y, nx, ny, board)) {
				availableMoves.push_back({ nx, ny });
			}
		}
	}

    return availableMoves;
}