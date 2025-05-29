#include "Knight.h"
#include "Board.h"

Knight::Knight(char color):Piece(PieceType::Knight,color) {}
Knight::~Knight() {}

bool Knight::IsMoveAllowed(int ox, int oy, int nx, int ny, const Board& board) const {
	int dx = std::abs(nx - ox);
	int dy = std::abs(ny - oy);

	if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2)) {
		const Piece* target = board.GetPieceAt(nx, ny);
		if (target == nullptr || target->GetColor() != this->GetColor()) {
			return true;
		}
	}
	return false;
}
std::vector<std::pair<int, int>> Knight::GetAvailableMoves(int x, int y, Board& board) const {
    std::vector<std::pair<int, int>> availableMoves;

    const int dx[] = { 2, 2, -2, -2, 1, 1, -1, -1 };
    const int dy[] = { 1, -1, 1, -1, 2, -2, 2, -2 };

    for (int i = 0; i < 8; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (IsMoveAllowed(x,y,nx,ny,board)) {
            availableMoves.push_back({ nx, ny });
        }
    }

    return availableMoves;
}