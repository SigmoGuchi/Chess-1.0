#include "Board.h"

    Board::Board():board(8, std::vector<Piece*>(8, nullptr)) {}
    Board::~Board() {
        for (auto& row : board) {
            for (auto& piece : row) {
                delete piece;
                piece = nullptr;
            }
        }

        for (auto& pair : pieceTrees) {
            delete pair.second; // Removal of trees
        }
    }

    // Binary trees
    void Board::AddToTree(TreeNode*& root, PieceType type, int index, int level, char color, std::pair<int, int> position) {
        if (!root) {
            root = new TreeNode(type, index, level, color, position);
            return;
        }

        if (color == 'W') {
            if (index >= root->index) {
                AddToTree(root->left, type, index, level + 1, color, position);
            }
            else {
                AddToTree(root->right, type, index, level + 1, color, position);
            }
        }
        else {
            if (index >= root->index) {
                  // Ignoring the rule of binary trees about writing the larger element to the left
                AddToTree(root->right, type, index, level + 1, color, position);
            }
            else {
                AddToTree(root->left, type, index, level + 1, color, position);
                
            }
        }


    }
    void Board::AddToHashTable(PieceType type, int index, char color, std::pair<int, int> position) {
        if (pieceTrees.find(type) == pieceTrees.end()) { 
            pieceTrees[type] = nullptr;
        }
        AddToTree(pieceTrees[type], type, index, 0, color, position);
    }
    void Board::UpdateTreeNodePosition(TreeNode* root, PieceType type, char color, std::pair<int, int> newPosition) {
        if (!root) return;

        if (root->type == type && root->color == color) {
            root->position = newPosition;
            return;
        }

        // Recursively search in the left and right branches
        UpdateTreeNodePosition(root->left, type, color, newPosition);
        UpdateTreeNodePosition(root->right, type, color, newPosition);
    }
    TreeNode* Board::FindMin(TreeNode* root) const {
        while (root && root->left) {
            root = root->left;
        }
        return root;
    }
    void Board::RemoveFromTree(TreeNode*& root, PieceType type, char color) {
        if (!root) return;

        if (root->type == type && root->color == color) {
            if (!root->left && !root->right) {
                delete root;
                root = nullptr;
            }
            else if (!root->left) {
                TreeNode* temp = root;
                root = root->right;
                delete temp;
            }
            else if (!root->right) {
                TreeNode* temp = root;
                root = root->left;
                delete temp;
            }
            else {
                TreeNode* temp = FindMin(root->right);
                root->type = temp->type;
                root->index = temp->index;
                root->color = temp->color;
                root->position = temp->position;
                RemoveFromTree(root->right, temp->type, temp->color);
            }
        }
        else if (type < root->type || (type == root->type && color < root->color)) {
            RemoveFromTree(root->left, type, color);
        }
        else {
            RemoveFromTree(root->right, type, color);
        }
    }
    void Board::RemovePiece(PieceType type, char color) {
        if (pieceTrees.find(type) != pieceTrees.end()) {
            RemoveFromTree(pieceTrees[type], type, color);
        }
    }
    int Board::CountPiecesInTree(const TreeNode* root) const {
        if (root == nullptr) {
            return 0;
        }
        return 1 + CountPiecesInTree(root->left) + CountPiecesInTree(root->right);
    }
    bool Board::IsDraw() const {
        int pieceCount = 0;

        for (const auto& pair : pieceTrees) {
            PieceType type = pair.first;
            TreeNode* root = pair.second;
            if (root != nullptr) {
                pieceCount += CountPiecesInTree(root);

                if (type != PieceType::King) {
                    return false;
                }
            }
        }
        return pieceCount <= 2;
    }
    void Board::PrintTree(TreeNode* root) {
        if (!root) {
            return;
        }
        PrintTree(root->left);
        std::cout << "Piece: " << static_cast<int>(root->type)
            << ", Index: " << root->index
            << ", Level: " << root->level
            << ", Color: " << root->color
            << ", Position: "<< root->position.first
            <<","<< root->position.second << "\n";
        PrintTree(root->right);
    }
    void Board::PrintTrees() {
        for (const auto& pair : pieceTrees) {
            std::cout << "Tree for type: " << static_cast<int>(pair.first) << "\n";
            if (pair.second) {
                PrintTree(pair.second);
            }
            else {
                std::cout << "Tree is empty.\n";
            }
        }
    }


    void Board::Initialize() {
        board.resize(8, std::vector<Piece*>(8, nullptr));
        pieceTrees.clear();

        for (int i = 0; i < 8; i++) {
             board[1][i] = new Pawn('B'); // White
             board[6][i] = new Pawn('W'); // Black
             AddToHashTable(PieceType::Pawn, i, 'W',{i,6});
             AddToHashTable(PieceType::Pawn, i, 'B', { i,1 });
        }


        // White figures
        board[7][0] = new Rook('W'); // {y, x}
        AddToHashTable(PieceType::Rook, 1, 'W', { 0,7 }); // {x, y}
        board[7][1] = new Knight('W');
        AddToHashTable(PieceType::Knight, 1, 'W', { 1,7 });
        board[7][2] = new Bishop('W');
        AddToHashTable(PieceType::Bishop, 1, 'W', { 2,7 });
        board[7][3] = new Queen('W');
        AddToHashTable(PieceType::Queen, 1, 'W', { 3,7 });
        board[7][4] = new King('W');
        AddToHashTable(PieceType::King, 1, 'W', { 4,7 });
        board[7][5] = new Bishop('W');
        AddToHashTable(PieceType::Bishop, 2, 'W', { 5,7 });
        board[7][6] = new Knight('W');
        AddToHashTable(PieceType::Knight, 2, 'W', { 6,7 });
        board[7][7] = new Rook('W');
        AddToHashTable(PieceType::Rook, 2, 'W', { 7,7 });

        // Black figures
        board[0][0] = new Rook('B');
        AddToHashTable(PieceType::Rook, 3, 'B', { 0,0 });
        board[0][1] = new Knight('B');
        AddToHashTable(PieceType::Knight, 3, 'B', { 1,0 });
        board[0][2] = new Bishop('B');
        AddToHashTable(PieceType::Bishop, 3, 'B', { 2,0 });
        board[0][3] = new Queen('B');
        AddToHashTable(PieceType::Queen, 2, 'B', { 3,0 });
        board[0][4] = new King('B');
        AddToHashTable(PieceType::King, 2, 'B', { 4,0 });
        board[0][5] = new Bishop('B');
        AddToHashTable(PieceType::Bishop, 4, 'B', { 5,0 });
        board[0][6] = new Knight('B');
        AddToHashTable(PieceType::Knight, 4, 'B', { 6,0 });
        board[0][7] = new Rook('B');
        AddToHashTable(PieceType::Rook, 4, 'B', { 7,0 });

        
        PrintTrees();
    }
    void Board::PrintBoard(RenderWindow& window) {
        RectangleShape cell(Vector2f(size, size));
        window.draw(boardSprite);
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                // draw a figure if there is one
                if (board[y][x] != nullptr) {
                    board[y][x]->GetSprite().setPosition(x * size + xOffset, y * size + yOffset);
                    window.draw(board[y][x]->GetSprite());
                }
            }
        }
    }
    
    bool Board::IsMoveValid(int ox, int oy, int nx, int ny) {
        // Checking the coordinates of the board
        if (ox < 0 || oy < 0 || nx < 0 || ny < 0 || ox >= 8 || oy >= 8 || nx >= 8 || ny >= 8) {
            return false;
        }

        // Checking the presence of a figure at the initial position
        Piece* piece = board[oy][ox];
        if (piece == nullptr) {
            return false;
        }

        // Checking for the presence of a piece of the same color in the target position
        Piece* targetPiece = board[ny][nx];
        if (targetPiece != nullptr && targetPiece->GetColor() == piece->GetColor()) {
            return false;
        }

        // Checking the admissibility of the figure's move
        if (!piece->IsMoveAllowed(ox, oy, nx, ny, *this)) {
            return false;
        }

        // Motion simulation
        board[oy][ox] = nullptr;
        board[ny][nx] = piece;

        // Check: whether the king remains in check
        auto kingPosition = FindPiece(PieceType::King, piece->GetColor());
        King* king = dynamic_cast<King*>(board[kingPosition.second][kingPosition.first]);
        bool isKingInCheck = king && king->IsInCheck(*this);

        board[oy][ox] = piece;
        board[ny][nx] = targetPiece;

        return !isKingInCheck;
    }
    bool Board::MovePieces(int ox, int oy, int nx, int ny) {
        Piece* movingPiece = board[oy][ox];
        Piece* targetPiece = board[ny][nx];

        if (!IsMoveValid(ox, oy, nx, ny)) return false;

        if (targetPiece) {
            RemovePiece(targetPiece->GetType(), targetPiece->GetColor());
            delete targetPiece;
        }

        SetPieceAt(nx, ny, movingPiece);
        SetPieceAt(ox, oy, nullptr);

        UpdateTreeNodePosition(pieceTrees[movingPiece->GetType()], movingPiece->GetType(), movingPiece->GetColor(), { nx, ny });
        //PrintTrees();

        if (movingPiece->GetType() == PieceType::Pawn) {
            static_cast<Pawn*>(movingPiece)->HandlePromotion(*this, nx, ny, *Window);
        }


        if (movingPiece->GetType() == PieceType::King && std::abs(nx - ox) == 2) {
            int rookX = (nx > ox) ? 7 : 0;
            int newRookX = (nx > ox) ? nx - 1 : nx + 1;

            Piece* rook = board[oy][rookX];
            board[oy][rookX] = nullptr;
            board[oy][newRookX] = rook;

            if (rook != nullptr) {
                rook->SetFirstMove(false);
            }
        }

        movingPiece->SetFirstMove(false);

        return true;
    }

    bool Board::IsSquareUnderAttack(int x, int y, char color) const {
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                if (board[row][col] != nullptr && board[row][col]->GetColor() != color) {
                    if (board[row][col]->IsMoveAllowed(col, row, x, y, *this)) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    std::pair<int, int> Board::FindPiece(PieceType type, char color) const {
        auto it = pieceTrees.find(type);
        if (it == pieceTrees.end()) {
            return { -1, -1 }; // There is no tree for this type
        }

        TreeNode* current = it->second;
        while (current != nullptr) {
            if (current->type == type && current->color == color) {
                return current->position;
            }

            // Transition logic
            if (color == 'W') {
                current = current->left; // We are looking for white figures to the left
            }
            else {
                current = current->right; // We look for black figures to the right
            }
        }

        return { -1, -1 }; // Figure not found
    }

    // Getters
    Piece* Board::GetPieceAt(int x, int y) const {
        if (x < 0 || x >= 8 || y < 0 || y >= 8) return nullptr;
        return board[y][x];
    }