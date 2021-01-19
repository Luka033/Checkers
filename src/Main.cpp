
#include <iostream>
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Checkers/Constants.h"
#include "Checkers/Game.h"
#include "Minimax/Algorithm.h"


using namespace std;
using namespace std::chrono;

class ProjectName : public olc::PixelGameEngine {
public:
	ProjectName() {
		sAppName = "Checkers";
	}
private:
	Game* game;

protected:
	bool OnUserCreate() override
	{
		game = new Game(this);
		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override
	{
		if (game->Winner() != "")
		{
			this->DrawString({ ScreenWidth() / 2, ScreenHeight() / 2 }, game->Winner() + " WON!");
		}		

		if (game->m_Turn < 0)
		{
			using clock = std::chrono::system_clock;
			using sec = std::chrono::duration<double>;
			const auto before = clock::now();

			std::pair<float, Board*> newBoard = Minimax(game->GetBoard(), 4, 1, game);
			game->AI_Move(newBoard.second);

			const sec duration = clock::now() - before;
			std::cout << "Total time: " << duration.count() << std::endl;
		}

		if (GetMouse(0).bPressed)
		{
			int coordinate = GetRowColFromMouse();
			game->Select(coordinate);
		}


		if (GetKey(olc::Key::R).bPressed)
		{
			game->Reset();
		}

		game->Update();

		return true;
	}

	int GetRowColFromMouse()
	{
		return ROWS * (GetMouseY() / SQUARE_SIZE) + (GetMouseX() / SQUARE_SIZE);
	}
};



int main() 
{
	// Use olcConsoleGameEngine derived ap
	ProjectName game;
	if(game.Construct(WIDTH, HEIGHT, 1, 1))
		game.Start();
	return 0;

}