#include "Rook.h"
#include "Board.h"

	Rook::Rook(char color) :Piece(PieceType::Rook, color) {}
	Rook::~Rook(){}

	bool Rook::IsMoveAllowed(int ox, int oy, int nx, int ny, const Board& board) const {
        if (ox != nx && oy != ny) {
            return false;
        }

        const auto& squares = board.GetBoard();

        if (oy == ny) {
            int step = (nx > ox) ? 1 : -1;
            for (int x = ox + step; x != nx; x += step) {
                if (squares[oy][x] != nullptr) {
                    return false;
                }
            }
        }
        else if (ox == nx) {
            int step = (ny > oy) ? 1 : -1;
            for (int y = oy + step; y != ny; y += step) {
                if (squares[y][ox] != nullptr) {
                    return false;
                }
            }
        }

        const Piece* targetPiece = squares[ny][nx];
        if (targetPiece != nullptr && targetPiece->GetColor() == color) {
            return false;
        }

        return true;
	}
    std::vector<std::pair<int, int>> Rook::GetAvailableMoves(int x, int y, Board& board) const {
        std::vector<std::pair<int, int>> availableMoves;


        for (int nx = 0; nx < 8; ++nx) {
            for (int ny = 0; ny < 8; ++ny) {
                if ((nx == x || ny == y) && IsMoveAllowed(x, y, nx, ny, board)) {
                    availableMoves.push_back({ nx, ny });
                }
            }
        }

        return availableMoves;
    }