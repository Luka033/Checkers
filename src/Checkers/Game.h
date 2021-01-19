#pragma once
#include "../olcPixelGameEngine.h"

#include "Board.h"

class Game
{
public:

	Game(olc::PixelGameEngine* win);

	void Update();
	void Reset();
	bool Select(int coordinate);
	bool Move(int coordinate);
	void ChangeTurn();
	std::string Winner() { return m_Board->Winner(); }

	Board* GetBoard() { return m_Board; }
	void AI_Move(Board* board);

private:
	void Init();
	void DrawValidMoves(olc::PixelGameEngine* win);

public:
	int m_Turn;
	Board* m_Board;

private:
	olc::PixelGameEngine* m_Win;

	int m_Selected;
	std::map<int, std::vector<int>> m_ValidMoves;

};

