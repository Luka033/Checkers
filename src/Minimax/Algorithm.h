#include <iostream>
#include "../Checkers/Game.h"
#undef max
#undef min

inline Board* SimulateMove(int piece, int move, Board* board, Game* game, std::vector<int> skip);
inline std::pair<float, Board*> Minimax(Board* position, int depth, bool maxPlayer, Game* game);
inline std::vector<Board*> GetAllMoves(Board* board, int color, Game* game);


inline std::pair<float, Board*> Minimax(Board* position, int depth, bool maxPlayer, Game* game)
{
	if (depth == 0 || position->Winner() != "")
		return std::make_pair(position->Evaluate(game->m_Turn), position);

	if (maxPlayer)
	{
		float maxEval = (float)-100000000;
		Board* bestMove = nullptr;
		for (auto& move : GetAllMoves(position, -1, game))
		{
			float evaluation = Minimax(move, depth - 1, false, game).first;
			maxEval = std::max(maxEval, evaluation);
			if (maxEval == evaluation)
				bestMove = move;
			else
				delete move;
		}
		return std::make_pair(maxEval, bestMove);
	}
	else
	{
		float minEval = (float)-100000000;
		Board* bestMove = nullptr;
		for (auto& move : GetAllMoves(position, 1, game))
		{
			float evaluation = Minimax(move, depth - 1, true, game).first;
			minEval = std::min(minEval, evaluation);
			if (minEval == evaluation)
				bestMove = move;
			else
				delete move;
		}
		return std::make_pair(minEval, bestMove);	
	}
}


inline Board* SimulateMove(int piece, int move, Board* board, Game* game, std::vector<int> skip)
{
	board->Move(piece, move);
	if (!skip.empty())
		board->RemovePieces(skip);
	return board;
}

inline std::vector<Board*> GetAllMoves(Board* board, int color, Game* game)
{
	std::vector<Board*> moves;
	for (auto& piece : board->GetAllPieces(color))
	{
		std::map<int, std::vector<int>> validMoves = board->GetValidMoves(piece);
		for (auto& move : validMoves)
		{
			Board* tempBoard = new Board();
			*tempBoard = *board;
			moves.push_back(SimulateMove(piece, move.first, tempBoard, game, move.second));
		}
	}
	return moves;
}

