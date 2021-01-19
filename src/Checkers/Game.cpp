#include "Game.h"

Game::Game(olc::PixelGameEngine* win)
	: m_Win(win)
{
	Init();
}

void Game::Init()
{
	m_Selected = 0;
	delete m_Board;
	m_Board = new Board();
	m_Turn = 1;
	m_ValidMoves = {};
}

void Game::DrawValidMoves(olc::PixelGameEngine* win)
{
	for (auto& move : m_ValidMoves)
	{
		int coordinate = move.first;
		int row = coordinate / ROWS;
		int col = coordinate % ROWS;
		win->FillCircle(col * SQUARE_SIZE + SQUARE_SIZE / 2, row * SQUARE_SIZE + SQUARE_SIZE / 2, 15, olc::BLUE);
	}
}

void Game::Update()
{
	m_Board->Draw(m_Win);
	DrawValidMoves(m_Win);
	if (Winner() != "")
		m_Win->DrawString({ WIDTH / 2 - 150, HEIGHT / 2 - 100 }, "    " + Winner() + "Red WON!\nPress R to Restart", olc::WHITE, 2);
}

void Game::Reset()
{
	delete m_Board;
	Init();
}

bool Game::Select(int coordinate)
{
	if (m_Board->GetPiece(m_Selected) != 0)
	{
		bool result = Move(coordinate);
		if (!result)
		{
			m_Selected = 0;
			Select(coordinate);
		}
	}
	if (m_Board->GetPiece(coordinate) != 0 && m_Board->IsSameColor(m_Board->GetPiece(coordinate), m_Turn))
	{
		m_Selected = coordinate;
		m_ValidMoves = m_Board->GetValidMoves(coordinate);

		return true;
	}
	return false;
}

bool Game::Move(int coordinate)
{
	int piece = m_Board->GetPiece(coordinate);
	if (m_Board->GetPiece(m_Selected) != 0 && piece == 0 && m_ValidMoves.find(coordinate) != m_ValidMoves.end())
	{
		m_Board->Move(m_Selected, coordinate);
		m_Board->RemovePieces(m_ValidMoves[coordinate]);
		ChangeTurn();
	}
	else
	{
		return false;
	}
	return true;
}

void Game::ChangeTurn()
{
	m_ValidMoves.clear();
	if (m_Turn == 1)
		m_Turn = -1;
	else
		m_Turn = 1;
}

void Game::AI_Move(Board* board)
{
	//delete m_Board;
	*m_Board = *board;
	ChangeTurn();
}

