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
	void ProcessEvents();
	void Update();
	void Render();
	void SwitchPlayer();

public:
	Game();
	~Game();
	void Run();
};

