#pragma once
#include "../olcPixelGameEngine.h"

#include "Constants.h"

class Board
{
public:
	Board();

	void DrawBoard(olc::PixelGameEngine* pge);
	void Draw(olc::PixelGameEngine* pge);
	void PrintBoard();
	int GetPiece(int coordinate);
	void Move(int piece, int corodinate);

	std::map<int, std::vector<int>> GetValidMoves(int piece);
	bool IsTileOccupied(int coordinate);
	void RemovePieces(std::vector<int>& pieces);
	std::string Winner();
	void MakeKing(int coordinate);
	bool IsSameColor(int pieceA, int pieceB);
	float Evaluate(int color);
	std::vector<int> GetAllPieces(int color);

private:
	bool IsValidTileCoordinate(int corodinate);
	bool IsPieceInGivenColumn(int currentPosition, int col);
	bool IsPieceInGivenRow(int currentPosition, int row);
	bool IsFirstColumnExclusion(int currentPosition, int candidateOffset);
	bool IsEightColumnExclusion(int currentPosition, int candidateOffset);

	void CheckJumpMove(int piece, int destinationCoordinate, int currentOffset, int enemyToRemove, std::map<int, std::vector<int>>& moves);
	int GetJumpMoveOffset(int candidateCoordinateOffset);

	static constexpr const int CANDIDATE_MOVE_COORDINATES[] = { -9, -7, 7, 9 };


private:
	int m_RedLeft = 12;
	int m_WhiteLeft = 12;
	int m_RedKings = 0;
	int m_WhiteKings = 0;

	olc::Sprite* m_Crown = new olc::Sprite("src/Assets/crown1.png");

public:
	int m_Board[NUM_TILES] = { 0, -1, 0, -1, 0, -1, 0, -1,
							   -1, 0, -1, 0, -1, 0, -1, 0, 
							   0, -1, 0, -1, 0, -1, 0, -1, 
							   0, 0, 0, 0, 0, 0, 0, 0, 
		                       0, 0, 0, 0, 0, 0, 0, 0, 
		                       1, 0, 1, 0, 1, 0, 1, 0, 
		                       0, 1, 0, 1, 0, 1, 0, 1, 
		                       1, 0, 1, 0, 1, 0, 1, 0 };
};

