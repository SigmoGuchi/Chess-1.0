#pragma once
#include "Piece.h"
#include "Bishop.h"
#include "King.h"
#include "Knight.h"
#include "Pawn.h"
#include "Queen.h"
#include "Rook.h"
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include<SFML/Graphics.hpp>
using namespace sf;

struct TreeNode {
	PieceType type;
	char color;
	int index;
	int level;
	TreeNode* left;
	TreeNode* right;

	std::pair<int,int> position;

	TreeNode(PieceType type,int index,int level,char color,std::pair<int,int> position):
		type(type),index(index),level(level),left(nullptr),right(nullptr),color(color),position(position){}
   };

	class Board {
	private:
		std::vector<std::vector<Piece*>> board;
		float size = 100.0;

		std::map<PieceType, TreeNode*> pieceTrees;
		std::vector<std::pair<int, int>> highlightedCells;

		float figureWidth = 48.0;
		float figureHeight = 115.0;
		float xOffset = (size - figureWidth) / 2.0f; // «сув по X
		float yOffset = (size - figureHeight) / 2.0f; // «сув по Y

		Sprite boardSprite;

		RenderWindow* Window;

	public:
		Board();
		~Board();


		std::vector<std::pair<int, int>> GetHighlights() { return highlightedCells; }
		void HighlightCells(const std::vector<std::pair<int, int>>& cells) { highlightedCells = cells; }
		void ClearHighlights() { highlightedCells.clear(); }
		void DrawHighlights(sf::RenderWindow& window) {
			for (const auto& cell : highlightedCells) {
				CircleShape  highlight((size/ 2));
				highlight.setPosition((cell.first * size), (cell.second * size));
				highlight.setFillColor(sf::Color(0, 255, 0, 108)); // Translucent green backlight
				window.draw(highlight);
			}
		}

		// Binary trees
		void AddToTree(TreeNode*& root, PieceType type, int index, int level, char color, std::pair<int, int> position);
		void AddToHashTable(PieceType type, int index, char color, std::pair<int, int> position);
		void UpdateTreeNodePosition(TreeNode* root, PieceType type, char color, std::pair<int, int> newPosition);
		TreeNode* FindMin(TreeNode* root) const;
		void RemoveFromTree(TreeNode*& root, PieceType type,char color);
		void RemovePiece(PieceType type, char color);
		int CountPiecesInTree(const TreeNode* root) const;
		bool IsDraw() const;
		void PrintTree(TreeNode* root);
		void PrintTrees();

		void Initialize();
		void PrintBoard(RenderWindow& window);

		//Ћог≥ка дл€ перев≥рки та виконанн€ ходу
		bool IsMoveValid(int ox,int oy,int nx,int ny);
		bool MovePieces(int ox, int oy, int nx, int ny);

		bool IsSquareUnderAttack(int x, int y, char color) const;

		std::pair<int, int> FindPiece(PieceType type, char color) const;


		bool IsCellEmpty(int x, int y) const{ return GetPieceAt(x, y) == nullptr; }

		// Getters
		float GetCellSize() const { return size; }
		const std::vector<std::vector<Piece*>>& GetBoard() const { return board; }
		const std::map<PieceType, TreeNode*>& GetPieceTrees() const { return pieceTrees; }
		Piece* GetPieceAt(int x, int y) const;

		// Setters
		void SetWindow(RenderWindow* window) { Window = window; }
		void SetPieceAt(int x, int y, Piece* piece){ board[y][x] = piece; }
		void SetTexture(Texture& texture) { boardSprite.setTexture(texture); }
	};
