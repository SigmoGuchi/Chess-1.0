#include "Pawn.h"
#include "Board.h"

Pawn::Pawn(char color) :Piece(PieceType::Pawn, color) {}
Pawn::~Pawn(){}


bool Pawn::IsMoveAllowed(int ox, int oy, int nx, int ny, const Board& board) const {
    int direction = (color == 'W') ? -1 : 1;
    int moveY = ny - oy;
    int moveX = nx - ox;

    if (moveX == 0 && moveY == direction) {
        return board.IsCellEmpty(nx, ny);
    }

    if (moveX == 0 && moveY == 2 * direction && ((color == 'W' && oy == 6) || (color == 'B' && oy == 1))) {
        return board.IsCellEmpty(nx, ny) && board.IsCellEmpty(nx, ny - direction);
    }

    if (abs(moveX) == 1 && moveY == direction) {
        Piece* target = board.GetPieceAt(nx, ny);
        return target != nullptr && target->GetColor() != color;
    }

    return false;
}
std::vector<std::pair<int, int>> Pawn::GetAvailableMoves(int x, int y, Board& board) const  {
    std::vector<std::pair<int, int>> availableMoves;

    int direction = (this->GetColor() == 'W') ? -1 : 1;
    int startRow = (this->GetColor() == 'W') ? 6 : 1;

    if (board.GetPieceAt(x, y + direction) == nullptr) {
        availableMoves.push_back({ x, y + direction });

        if (y == startRow && board.GetPieceAt(x, y + 2 * direction) == nullptr) {
            availableMoves.push_back({ x, y + 2 * direction });
        }
    }

    for (int dx : {-1, 1}) {
        int nx = x + dx;
        int ny = y + direction;

        if (IsMoveAllowed(x,y,nx,ny,board)) {
            availableMoves.push_back({ nx, ny });

        }
    }

    return availableMoves;
}

void Pawn::HandlePromotion(Board& board, int x, int y, RenderWindow& window) {
    if ((color == 'W' && y == 0) || (color == 'B' && y == 7)) {
        PieceType choice = PromptPromotionChoiceWithUI(x, y, window,board);
        Promote(board, x, y, choice);
    }
}
void Pawn::Promote(Board& board, int x, int y,PieceType& type) {

    Piece* newPiece = nullptr;
    switch (type)
    {
    case PieceType::Queen:
        newPiece = new Queen(color);
        break;
    case PieceType::Rook:
        newPiece = new Rook(color);
        break;
    case PieceType::Bishop:
        newPiece = new Bishop(color);
        break;
    case PieceType::Knight:
        newPiece = new Knight(color);
        break;
    default:
        newPiece = new Queen(color);
    }

    std::string pieceTypeStr = PieceTypeToString(newPiece->GetType());
    std::string filePath = "assets/" + std::string(1, newPiece->GetColor()) + "_" + pieceTypeStr + ".png";
    newPiece->LoadTexture(filePath);

    board.SetPieceAt(x, y, newPiece);
}
PieceType Pawn::PromptPromotionChoiceWithUI(int x, int y, RenderWindow& window,Board & board) {
    Texture texture;
    if (color == 'W') {
        texture.loadFromFile("assets/W_Transformation.png");
    }
    else {
        texture.loadFromFile("assets/B_Transformation.png");
    }

    Sprite sprite(texture);
    sprite.setPosition(x * board.GetCellSize(), y * board.GetCellSize());

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;

                sf::Vector2f position = sprite.getPosition();
                int relativeX = mouseX - static_cast<int>(position.x);
                int relativeY = mouseY - static_cast<int>(position.y);

                // Breakdown into 4 parts
                int width = texture.getSize().x / 2;
                int height = texture.getSize().y / 2;

                if (relativeX >= 0 && relativeX < width && relativeY >= 0 && relativeY < height) {
                    return PieceType::Rook;
                }
                else if (relativeX >= width && relativeX < 2 * width && relativeY >= 0 && relativeY < height) {
                    return PieceType::Queen;
                }
                else if (relativeX >= 0 && relativeX < width && relativeY >= height && relativeY < 2 * height) {
                 return PieceType::Knight;
                }
                else if (relativeX >= width && relativeX < 2 * width && relativeY >= height && relativeY < 2 * height) {
                    return PieceType::Bishop;
                }
            }
        }

        window.clear();
        board.PrintBoard(window);
        window.draw(sprite);
        window.display();
    }
    return PieceType::Queen;
}