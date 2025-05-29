#include "Game.h"

#pragma once
static class UI{
private:
	static Texture boardTexture;
	static std::map<String, Texture> pieceTextures;
	static Game game;
	
	static void SetWindow(int Width, int Height);

	static void Initialize();
	static void LoadTextures();
	static void LoadBoardTexture();
	static void LoadPieceTexture();
	static void ProcessPieceTree(TreeNode* root);

	static void Update();
	static void Render();
	static void ProcessEvents();
	static void HandleEvent(Event& event);

public:
	 static Sprite boardSprite;
	 static RenderWindow window;
	 static void Run();
	 static const std::map<String, Texture>& GetPieceTextures() { return pieceTextures; }
};

