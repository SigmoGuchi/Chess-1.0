#include "Game.h"
#include "stack"

Game::Game()
    : whitePlayer('W'), blackPlayer('B'), currentPlayer(&whitePlayer),
    window(VideoMode(800, 800), "Chess 1.0")
{
    board.SetWindow(&window); 
}
Game::~Game() {}

void Game::Initialize() {
    LoadBoardTexture();
    InitializeBoard();
    LoadPiecesTextures();
    FinalizeInitialization();
}
void Game::LoadBoardTexture() {
    if (!boardTexture.loadFromFile("assets/Board.png")) {
        throw std::runtime_error("Failed to load board texture!");
    }
}
void Game::InitializeBoard() {
    board.SetTexture(boardTexture);
    board.Initialize();
}
void Game::LoadPiecesTextures() {
    for (const auto& treePair : board.GetPieceTrees()) {
        ProcessPieceTree(treePair.second);
    }
}
void Game::ProcessPieceTree(TreeNode* root) {
    std::stack<TreeNode*> stack;
    stack.push(root);

    while (!stack.empty()) {
        TreeNode* current = stack.top();
        stack.pop();

        if (current) {
            LoadPieceTexture(current);
            stack.push(current->left);
            stack.push(current->right);
        }
    }
}
void Game::LoadPieceTexture(TreeNode* node) {
    std::string pieceTypeStr = PieceTypeToString(node->type);
    std::string filePath = "assets/" + std::string(1, node->color) + "_" + pieceTypeStr + ".png";
    Piece* piece = board.GetPieceAt(node->position.first, node->position.second);
    if (piece) piece->LoadTexture(filePath);
}
void Game::FinalizeInitialization() {
    std::cout << "The game initialized successfully!" << std::endl;
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

void Game::CheckGameState() {
    char opponentColor = (currentPlayer->GetColor() == 'W') ? 'B' : 'W';
    auto opponentKingPos = board.FindPiece(PieceType::King, opponentColor);
    King* opponentKing = dynamic_cast<King*>(board.GetPieceAt(opponentKingPos.first, opponentKingPos.second));

    if (opponentKing->IsInCheck(board)) {
        std::cout << "Check to the king " << ((opponentKing->GetColor()) ? 'W' : 'B') << std::endl;
    }
    if (opponentKing->IsCheckmate(board)) {
        std::cout << "Checkmate to the king " << ((opponentKing->GetColor()) ? 'W' : 'B') << std::endl;
    }
    if (board.IsDraw()) {
        std::cout << "Draw!" << std::endl;
    }
}
void Game::HandlePieceSelection(int mouseX, int mouseY, int& startX, int& startY) {
    if (board.GetPieceAt(mouseX, mouseY) != nullptr &&
        board.GetPieceAt(mouseX, mouseY)->GetColor() == currentPlayer->GetColor()) {
        startX = mouseX;
        startY = mouseY;
        auto availableMoves = board.GetPieceAt(startX, startY)->GetAvailableMoves(startX, startY, board);
        board.HighlightCells(availableMoves);
    }
}
void Game::HandleMoveExecution(int mouseX, int mouseY, int startX, int startY) {
    if (board.MovePieces(startX, startY, mouseX, mouseY)) {
        CheckGameState();
        SwitchPlayer();
    }
    board.ClearHighlights();
}
void Game::ProcessMouseEvent(int mouseX, int mouseY) {
    static int startX = -1, startY = -1;

    mouseX = static_cast<int>(mouseX / board.GetCellSize());
    mouseY = static_cast<int>(mouseY / board.GetCellSize());

    if (startX == -1 && startY == -1) {
        HandlePieceSelection(mouseX, mouseY, startX, startY);
    }
    else {
        HandleMoveExecution(mouseX, mouseY, startX, startY);
        startX = -1;
        startY = -1;
    }
}
void Game::ProcessEvents() {
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
        }
        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
            ProcessMouseEvent(event.mouseButton.x, event.mouseButton.y);
        }
    }
}

void Game::Render() {
    window.clear();
    board.PrintBoard(window);
    board.DrawHighlights(window);
    window.display();
}
void Game::Update() {
    while (window.isOpen()) {
        ProcessEvents();
        Render(); 
    }
}
void Game::Run() {
    Initialize();
    Update();
    
}