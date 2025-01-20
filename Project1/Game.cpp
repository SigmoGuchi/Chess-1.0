#include "Game.h"
#include "stack"

Game::Game()
    : whitePlayer('W'), blackPlayer('B'), currentPlayer(&whitePlayer),
    window(VideoMode(800, 800), "Chess 1.0")
{
    board.SetWindow(&window); 
}
Game::~Game() {
}

void Game::Initialize() {
    if (!boardTexture.loadFromFile("assets/Board.png")) {
        throw std::runtime_error("Failed to load board texture!");
    }
    board.SetTexture(boardTexture);
    board.Initialize();

    // Loading textures for shapes
    for (const auto& treePair : board.GetPieceTrees()) { // Get trees from Board
        TreeNode* root = treePair.second;
        std::stack<TreeNode*> stack; 
        stack.push(root);

        while (!stack.empty()) {
            TreeNode* current = stack.top();
            stack.pop();

            if (current) {
                // Loading texture
                std::string pieceTypeStr = PieceTypeToString(current->type);
                std::string filePath = "assets/" + std::string(1, current->color) + "_" + pieceTypeStr + ".png";
                Piece* piece = board.GetPieceAt(current->position.first, current->position.second);
                if (piece) piece->LoadTexture(filePath);

                stack.push(current->left);
                stack.push(current->right);
            }
        }
    }


    std::cout << "The game initialized successfully!" << std::endl;
    window.display();
}

// The main cycle of the game
void Game::Run() {
    Initialize();
    Update();
    
}

void Game::ProcessEvents() {
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
        }
        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
            static int startX = -1, startY = -1;
            int mouseX = static_cast<int>(event.mouseButton.x / board.GetCellSize());
            int mouseY = static_cast<int>(event.mouseButton.y / board.GetCellSize());

            if (startX == -1 && startY == -1) {
                if (board.GetPieceAt(mouseX, mouseY) != nullptr &&
                    board.GetPieceAt(mouseX, mouseY)->GetColor() == currentPlayer->GetColor()) {
                    startX = mouseX;
                    startY = mouseY;

                    auto availableMoves = board.GetPieceAt(startX, startY)->GetAvailableMoves(startX, startY, board);
                    board.HighlightCells(availableMoves);
                }
            }
            else {
                if (board.MovePieces(startX, startY, mouseX, mouseY)) {
                    char opponentColor = (currentPlayer->GetColor() == 'W') ? 'B' : 'W';
                    auto opponentKingPos = board.FindPiece(PieceType::King, opponentColor);
                    King* opponentKing = dynamic_cast<King*>(board.GetPieceAt(opponentKingPos.first, opponentKingPos.second));

                    // We check whether the opponent's king is in check
                    if (opponentKing->IsInCheck(board)) {
                        std::cout << "Check to the king "<< ((opponentKing->GetColor()) ? 'W' : 'B') << std::endl;
                    }
                    if (opponentKing->IsCheckmate(board)) {
                        std::cout << "Checmate to the king" << ((opponentKing->GetColor()) ? 'W' : 'B') << std::endl;
                    }
                    if (board.IsDraw()) {
                        std::cout<< "Draw!" << std::endl;
                    }


                    SwitchPlayer();
                }
                board.ClearHighlights();
                startX = -1;
                startY = -1;
            }
        }
    }
}
void Game::Update() {
    while (window.isOpen()) {
        ProcessEvents();
        Render(); 
    }
}
void Game::Render() {
    window.clear();
    board.PrintBoard(window);
    board.DrawHighlights(window);
    window.display();
}
void Game::SwitchPlayer() {
    if (currentPlayer == &whitePlayer) {
        currentPlayer = &blackPlayer;
    }
    else {
        currentPlayer = &whitePlayer;
    }
    std::cout << "Now the player is walking: " << currentPlayer->GetColor() << std::endl;
}