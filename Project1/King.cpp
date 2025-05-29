#include "Board.h"
#include "King.h"
#include <stack>
#include<functional>


    King::King(char color) :Piece(PieceType::King, color) {}
    King::~King() {}

    bool King::IsMoveAllowed(int ox, int oy, int nx, int ny, const Board& board)const {
        int dx = std::abs(nx - ox);
        int dy = std::abs(ny - oy);
        if ((dx <= 1 && dy <= 1) && !(dx == 0 && dy == 0)) {
            return true;
        }

        // Castling
        if (dy == 0 && dx == 2) {

            if (!this->IsFirstMove()) {
                return false;
            }

            int rookX = (nx > ox) ? 7 : 0;
            Piece* rook = board.GetPieceAt(rookX, oy);
            if (rook == nullptr || rook->GetType() != PieceType::Rook || !rook->IsFirstMove()) {
                return false;
            }

            int step = (nx > ox) ? 1 : -1;
            for (int x = ox + step; x != rookX; x += step) {
                if (board.GetPieceAt(x, oy) != nullptr) {
                    return false;
                }
            }

            for (int x = ox; x != nx + step; x += step) {
                if (board.IsSquareUnderAttack(x, oy, this->GetColor())) {
                    return false;
                }
            }
            return true;

        }

        return false;
    }
    bool King::IsInCheck(const Board& board) const {
        auto position = board.FindPiece(PieceType::King, color);
        int kingX = position.first;
        int kingY = position.second;

        return board.IsSquareUnderAttack(kingX, kingY, color);
    }
    bool King::IsCheckmate(Board& board) const {
        auto position = board.FindPiece(PieceType::King, color);
        int kingX = position.first;
        int kingY = position.second;

        if (!IsInCheck(board)) {
            return false;
        }

        std::vector<std::pair<int, int>> possibleMoves = {
            {kingX - 1, kingY - 1}, {kingX, kingY - 1}, {kingX + 1, kingY - 1},
            {kingX - 1, kingY},                           {kingX + 1, kingY},
            {kingX - 1, kingY + 1}, {kingX, kingY + 1}, {kingX + 1, kingY + 1}
        };

        for (const auto& move : possibleMoves) {
            int newX = move.first;
            int newY = move.second;

            if (!board.IsSquareUnderAttack(newX, newY, color) && board.GetPieceAt(newX, newY) == nullptr) {
                return false;
            }
            
        }

        for (const auto& treePair : board.GetPieceTrees()) {
            TreeNode* root = treePair.second;
            std::stack<TreeNode*> stack;
            stack.push(root);

            while (!stack.empty()) {
                TreeNode* current = stack.top();
                stack.pop();

                if (current) {
                    Piece* piece = board.GetPieceAt(current->position.first, current->position.second);
                    if (piece && piece->GetColor() == color) {
                        auto moves = piece->GetAvailableMoves(current->position.first, current->position.second, board);

                        for (const auto& move : moves) {
                            int targetX = move.first;
                            int targetY = move.second;

                            Piece* capturedPiece = board.GetPieceAt(targetX, targetY);
                            board.MovePieces(current->position.first, current->position.second, targetX, targetY);

                            bool kingSafe = !board.IsSquareUnderAttack(kingX, kingY, color);

                            board.MovePieces(targetX, targetY, current->position.first, current->position.second);
                            if (capturedPiece) {
                                board.SetPieceAt(targetX, targetY, capturedPiece);
                            }

                            if (kingSafe) {
                                return false;
                            }
                        }
                    }

                    stack.push(current->left);
                    stack.push(current->right);
                }
            }
        }
      
        return true;
    }
    std::vector<std::pair<int, int>> King::GetAvailableMoves(int x, int y, Board& board) const {
        std::vector<std::pair<int, int>> availableMoves;

        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                int nx = x + dx;
                int ny = y + dy;

                if(board.IsMoveValid(x,y,nx,ny)){
                    availableMoves.push_back({ nx,ny });
                }
            }
        }
        return availableMoves;
    }