#include "Board.h"

Board::Board()
{
}

void Board::DrawBoard(olc::PixelGameEngine* pge)
{
	pge->Clear(olc::BLACK);
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = row % 2; col < ROWS; col += 2)
		{
			pge->FillRect((row * SQUARE_SIZE), (col * SQUARE_SIZE), SQUARE_SIZE, SQUARE_SIZE, olc::RED);
		}
	}
}

void Board::Draw(olc::PixelGameEngine* pge)
{
	this->DrawBoard(pge);
	int radius = SQUARE_SIZE / 2 - PADDING;
	int x, y;
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			if (m_Board[ROWS * row + col] != 0)
			{
				x = SQUARE_SIZE * col + SQUARE_SIZE / 2;
				y = SQUARE_SIZE * row + SQUARE_SIZE / 2;
				if (m_Board[ROWS * row + col] <= -1)
				{
					pge->FillCircle(x, y, radius + OUTLINE, olc::GREY);
					pge->FillCircle(x, y, radius, olc::WHITE);
				}
				else
				{
					pge->FillCircle(x, y, radius + OUTLINE, olc::GREY);
					pge->FillCircle(x, y, radius, olc::RED);
				}
				if(std::abs(m_Board[ROWS * row + col]) > 1)
					pge->DrawSprite(x - m_Crown->width / 2, y - m_Crown->height / 2, m_Crown);
			}
		}
	}
}

void Board::PrintBoard()
{
	for (int i = 1; i < 65; i++)
	{
		std::cout << m_Board[i - 1] << " ";
		if (i % 8 == 0)
			std::cout << "\n";
	}	
}

int Board::GetPiece(int coordinate)
{

	return m_Board[coordinate];
}

void Board::Move(int piece, int coordinate)
{
	std::swap(m_Board[coordinate], m_Board[piece]);
	if (IsPieceInGivenRow(coordinate, 0) || IsPieceInGivenRow(coordinate, 7))
		MakeKing(coordinate);

	if (piece <= -1)
		m_WhiteKings++;
	else
		m_RedKings++;

}

void Board::MakeKing(int coordinate)
{
	if (GetPiece(coordinate) >= 1)
		m_Board[coordinate] = 2;
	else
		m_Board[coordinate] = -2;
}

bool Board::IsSameColor(int pieceA, int pieceB)
{
	return ((pieceA ^ pieceB) >= 0);
}

float Board::Evaluate(int color)
{
	//return
	return (float)m_WhiteLeft - (float)m_RedLeft + ((float)m_WhiteKings * 0.5 - (float)m_RedKings * 0.5);
}

std::vector<int> Board::GetAllPieces(int color)
{
	std::vector<int> pieces;
	for (int i = 0; i < NUM_TILES; i++)
	{
		if(m_Board[i] != 0 && IsSameColor(GetPiece(i), color))
			pieces.push_back(i);
	}
	return pieces;
}

std::map<int, std::vector<int>> Board::GetValidMoves(int piece)
{
	std::map<int, std::vector<int>> moves;
	for (const int currentOffset : CANDIDATE_MOVE_COORDINATES)
	{
		if (!(std::abs(GetPiece(piece)) > 1) && (currentOffset ^ GetPiece(piece)) > 0)
			continue;

		int destinationCoordinate = piece + (currentOffset);
		if (IsValidTileCoordinate(destinationCoordinate))
		{
			if (IsFirstColumnExclusion(piece, currentOffset) ||
				IsEightColumnExclusion(piece, currentOffset))
				continue;

			if (IsTileOccupied(destinationCoordinate))
			{
				int pieceAtDestination = GetPiece(destinationCoordinate);
				if (IsSameColor(pieceAtDestination, GetPiece(piece)) ||
					IsPieceInGivenColumn(destinationCoordinate, 0) ||
					IsPieceInGivenColumn(destinationCoordinate, 7))
				{
					continue;
				}
				else
				{
					destinationCoordinate += currentOffset;
					if (!IsTileOccupied(destinationCoordinate))
					{
						moves.insert({ destinationCoordinate, {destinationCoordinate - currentOffset} });
						int newCandidateCoordinateOffset = GetJumpMoveOffset(currentOffset);
						CheckJumpMove(piece, destinationCoordinate + newCandidateCoordinateOffset, newCandidateCoordinateOffset, destinationCoordinate - currentOffset, moves);
					}
				}
			}
			else
			{
				moves.insert({ destinationCoordinate, {} });
			}
		}
	}
	return moves;
}

void Board::CheckJumpMove(int piece, int destinationCoordinate, int currentOffset, int enemyToRemove, std::map<int, std::vector<int>>& moves)
{
	if (IsTileOccupied(destinationCoordinate))
	{
		int pieceAtDestination = GetPiece(destinationCoordinate);
		if (!IsSameColor(pieceAtDestination, GetPiece(piece)) && !IsTileOccupied(destinationCoordinate + currentOffset))
		{
			moves.insert({ destinationCoordinate + currentOffset, {destinationCoordinate, enemyToRemove} });
		}
	}
}



int Board::GetJumpMoveOffset(int candidateCoordinateOffset)
{
	switch (candidateCoordinateOffset)
	{
	case -9:
		return -7;
	case -7:
		return -9;
	case 9:
		return 7;
	case 7:
		return 9;
	}
	return NULL;
}

void Board::RemovePieces(std::vector<int>& pieces)
{
	for (auto& piece : pieces)
	{
		if (GetPiece(piece) != 0)
		{
			if (GetPiece(piece) > 0)
				m_RedLeft -= 1;
			else
				m_WhiteLeft -= 1;
		}
		m_Board[piece] = 0;
	}
}

std::string Board::Winner()
{
	if (m_RedLeft <= 0)
		return "White";
	else if (m_WhiteLeft <= 0)
		return "Red";

	return "";
}




bool Board::IsTileOccupied(int coordinate)
{
	return m_Board[coordinate] != 0;
}



bool Board::IsValidTileCoordinate(int coordinate)
{
	return coordinate >= 0 && coordinate < (ROWS* COLS);
}

bool Board::IsPieceInGivenColumn(int currentPosition, int col)
{
	return (currentPosition % COLS) == col;
}

bool Board::IsPieceInGivenRow(int currentPosition, int row)
{
	return (currentPosition / ROWS) == row;
}

bool Board::IsFirstColumnExclusion(int currentPosition, int candidateOffset)
{
	return IsPieceInGivenColumn(currentPosition, 0) && (candidateOffset == -9 || candidateOffset == 7);
}

bool Board::IsEightColumnExclusion(int currentPosition, int candidateOffset)
{
	return IsPieceInGivenColumn(currentPosition, 7) && (candidateOffset == -7 || candidateOffset == 9);
}
