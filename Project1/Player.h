#pragma once


class Player {
private:
	char color;
	bool isPath = true; 

public:
	Player(char color);

	// Getters
	char GetColor()const { return color; }
};