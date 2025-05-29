#include "Queen.h"
#include "Board.h"


Queen::Queen(char color) :Piece(PieceType::Queen, color) {}
Queen::~Queen(){}

bool Queen::IsMoveAllowed(int ox, int oy, int nx, int ny, const Board& board) const {
    int dx = abs(nx - ox);
    int dy = abs(ny - oy);

    bool isStraightMove = (dx == 0 || dy == 0);
    bool isDiagonalMove = (dx == dy);

    if (!isStraightMove && !isDiagonalMove) {
        return false;
    }

    if (isStraightMove) {
        int xDirection = (nx > ox) ? 1 : (nx < ox ? -1 : 0);
        int yDirection = (ny > oy) ? 1 : (ny < oy ? -1 : 0);
        int steps = std::max(dx, dy);

        for (int i = 1; i < steps; ++i) {
            int checkX = ox + i * xDirection;
            int checkY = oy + i * yDirection;

            if (board.GetPieceAt(checkX, checkY) != nullptr) {
                return false;
            }
        }
    }

    if (isDiagonalMove) {
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
    }

    Piece* targetPiece = board.GetPieceAt(nx, ny);
    if (targetPiece != nullptr && targetPiece->GetColor() == this->GetColor()) {
        return false;
    }

    return true;
}
std::vector<std::pair<int, int>> Queen::GetAvailableMoves(int x, int y, Board& board) const {
    std::vector<std::pair<int, int>> availableMoves;

    for (int nx = 0; nx < 8; ++nx) {
        for (int ny = 0; ny < 8; ++ny) {
            if (IsMoveAllowed(x, y, nx, ny, board)) {
                availableMoves.push_back({ nx, ny });
            }
        }
    }

    return availableMoves;
}