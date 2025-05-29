#pragma once
#include "Board.h"
#include "Player.h" 


class Game{
private:
	Board board;
	Player whitePlayer;
	Player blackPlayer;
	Player* currentPlayer;

	Texture boardTexture;

	RenderWindow window;

	void Initialize();
	void LoadBoardTexture();
	void InitializeBoard();
	void LoadPiecesTextures();
	void ProcessPieceTree(TreeNode* root);
	void LoadPieceTexture(TreeNode* root);
	void FinalizeInitialization();
	void SwitchPlayer();

	void CheckGameState();
	void HandlePieceSelection(int mouseX, int mouseY, int& startX, int& startY);
	void HandleMoveExecution(int mouseX, int mouseY, int startX, int startY);
	void ProcessMouseEvent(int mouseX, int mouseY);
	void ProcessEvents();

	void Render();
	void Update();

public:
	static RenderWindow gameWindow;
	Game();
	~Game();
	void Run();
};

