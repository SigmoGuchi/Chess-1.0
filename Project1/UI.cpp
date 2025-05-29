#include "UI.h"



RenderWindow UI::window(VideoMode(800, 800), "Chess 1.");
Texture UI::boardTexture;
Sprite UI:: boardSprite;
std::map<String, Texture> UI::pieceTextures;

Game UI::game;

void UI::SetWindow(int Width, int Height){
	window.setSize(Vector2u(Width, Height));

	boardSprite.setOrigin(boardTexture.getSize().x / 2, boardTexture.getSize().y / 2);
	boardSprite.setPosition(400, 400);

	float scaleX = static_cast<float>(Width) / boardTexture.getSize().x;
	float scaleY = static_cast<float>(Height) / boardTexture.getSize().y;

	float scale = std::min(scaleX, scaleY);
	boardSprite.setScale(scale, scale);

	View view = window.getDefaultView();
	view.setSize(Width,Height);
	window.setView(view);
}

void UI::Initialize() {
	window.setFramerateLimit(60);
	game.Initialize();
	LoadTextures();	
}
void UI::LoadTextures() {
	LoadBoardTexture();
	LoadPieceTexture();
}
void UI::LoadBoardTexture() {
	if (!boardTexture.loadFromFile("assets/Board.png")) {
		throw std::runtime_error("Failed to load board texture!");
	}
	boardSprite.setTexture(boardTexture);
}
void UI::LoadPieceTexture() {
	for (const auto & treePair : game.GetBoard().GetPieceTrees()) {
		ProcessPieceTree(treePair.second);
	}
}
void UI::ProcessPieceTree(TreeNode* root) {
	if (!root) return;

	std::stack<TreeNode*> stack;
	stack.push(root);

	while (!stack.empty()) {
		TreeNode* current = stack.top();
		stack.pop();

		String pieceTypeStr = PieceTypeToString(current->type);
		String key = std::string(1, current->color) + "_" + pieceTypeStr;

		if (pieceTextures.find(key) == pieceTextures.end()) {
			String filePath = "assets/" + key + ".png";
		    Piece* piece = game.GetBoard().GetPieceAt(current->position.first, current->position.second);
		    if (piece) piece->LoadTexture(filePath);
		}

		if (current->right) stack.push(current->right);
		if (current->left) stack.push(current->left);
	}
}


void UI::Render() {
	window.clear(Color::Black);
	game.Render();
	window.display();
}
void UI::ProcessEvents() {
	Event event;
	while (window.pollEvent(event)) {
		HandleEvent(event);
	}
}
void UI::HandleEvent(Event& event) { 
	if (event.type == Event::Closed) {
		window.close();
	}    
	if (event.type == Event::Resized) {
		float newWidth = static_cast<float>(event.size.width);
		float newHeight = static_cast<float>(event.size.height);

		SetWindow(newWidth,newHeight);
	}
}
void UI::Update() {
	while (window.isOpen()) {
		ProcessEvents();
		Render();
	}
}
void UI::Run() {
	Initialize();
	Update();
}