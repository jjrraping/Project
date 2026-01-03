/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/
//------------------------------------------------------------------------------------
// Includes
#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <iostream>   // printing, debugging
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <raymath.h>
//------------------------------------------------------------------------------------
class Zombie {
public:
	Vector2 position{};
	int health = 100;
	int damage = 50;
	float speed = 1.5f;

	Zombie() {};

	void trackPlayer(Vector2 playerPosition) {
		// Move zombies toward player
		if (position.x < playerPosition.x) {
			position.x += 1.5f;
		}
		if (position.x > playerPosition.x) {
			position.x -= 1.5f;
		}
		if (position.y > playerPosition.y) {
			position.y -= 1.5f;
		}
		if (position.y < playerPosition.y) {
			position.y += 1.5f;
		}
		// Add some random movement
		position.x += (rand() % 3) - 1;
		position.y += (rand() % 3) - 1;
	}
	
};

struct GameStats {
	int round = 1;
	int score = 0;
	int zombiesKilled = 0;
	int zombiesRendered = 0;
	int zombiesUnrendered = 30;
	int zombiesLeft = zombiesRendered + zombiesUnrendered;

	GameStats() {};

	void startNewRound() {
		if (zombiesLeft == 0) {
			//incriment round
			round++;
			//spawn new zombies
			zombiesLeft = round * 5;
			zombiesUnrendered = zombiesLeft;
			zombiesRendered = 0;
		}
	}

	void resetGame() {
		round = 1;
		score = 0;
		zombiesKilled = 0;
		zombiesRendered = 0;
		zombiesUnrendered = 6;
		zombiesLeft = zombiesRendered + zombiesUnrendered;

	}
};
typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen;
class Gun;

struct Bullet {
	Vector2 position{};
	Vector2 velocity{};
	int damage = 20;
};

class Gun {
public:
	int maxBullets = 1000000;
	int numOfBullets = maxBullets;
	std::vector<Bullet> bullets;

	Gun() {};

	void Update()
	{
		for (int i = 0; i < bullets.size(); i++)
		{
			Bullet& bullet = bullets[i];
			bullet.position = Vector2Add(bullet.position, Vector2Scale(bullet.velocity, 5));
		}
	}

	void Draw()
	{
		for (int i = 0; i < bullets.size(); i++)
		{
			Bullet bullet = bullets[i];
			DrawCircle((int)bullet.position.x, (int)bullet.position.y, 8, YELLOW);
		}
	}

	void Shoot(Vector2 playerPos)
	{
		if (numOfBullets > 0)
		{
			Vector2 mousePosition = GetMousePosition();
			Vector2 direction = Vector2Subtract(mousePosition, playerPos);
			direction = Vector2Normalize(direction);
			bullets.push_back( Bullet{ playerPos, direction });
			numOfBullets--;
		}
	}
};

class MiniGun {
public:
	int maxBullets = 1000000;
	int numOfBullets = maxBullets;
	std::vector<Bullet> bullets;

	MiniGun() {};

	void Update()
	{
		for (int i = 0; i < bullets.size(); i++)
		{
			Bullet& bullet = bullets[i];
			bullet.position = Vector2Add(bullet.position, Vector2Scale(bullet.velocity, 10));
		}
	}

	void Draw()
	{
		for (int i = 0; i < bullets.size(); i++)
		{
			Bullet bullet = bullets[i];
			DrawCircle((int)bullet.position.x, (int)bullet.position.y, 8, YELLOW);
		}
	}

	void Shoot(Vector2 playerPos)
	{
		if (numOfBullets > 0)
		{
			Vector2 mousePosition = GetMousePosition();
			Vector2 direction = Vector2Subtract(mousePosition, playerPos);
			direction = Vector2Normalize(direction);
			bullets.push_back( Bullet{ playerPos, direction });
			numOfBullets--;
		}
	}
};

class Player
{
public:
	Vector2 position = { 0, 0 };
	Vector2 velocity = { 0, 0 };
	Texture2D texture;
	Gun gun;
	MiniGun gun2;
	int gunNumber = 0;

	// Player()
	// {
	//     texture = LoadTexture("res/player.jpeg");
	// }

	void Update()
	{
		if (IsKeyDown(KEY_W)) velocity.y -= 1;
		if (IsKeyDown(KEY_S)) velocity.y += 1;
		if (IsKeyDown(KEY_D)) velocity.x += 1;
		if (IsKeyDown(KEY_A)) velocity.x -= 1;
		//if (IsKeyDown(KEY_E)) 

		if (IsMouseButtonPressed(0) && gunNumber == 0)
		{
			gun.Shoot(position);
		}
		if (IsMouseButtonPressed(0) && gunNumber == 1)
		{
			gun2.Shoot(position);
		}

		velocity.x *= 0.8f;
		velocity.y *= 0.8f;

		position.x += velocity.x;
		position.y += velocity.y;

		if (gunNumber == 0) gun.Update();
		else if (gunNumber == 1) gun2.Update();
	}

	void Draw()
	{
		Vector2 mousePosition = GetMousePosition();
		Vector2 difference = Vector2Subtract(mousePosition, position);
		difference = Vector2Normalize(difference);

		float rotation = (atan2f(difference.y, difference.x) * RAD2DEG) - 90;

		DrawTexturePro(texture, { 0, 0, (float)texture.width, (float)texture.height },
			{ position.x, position.y, (float)texture.width, (float)texture.height },
			{ texture.width / 2.f, texture.height / 2.f }, rotation, WHITE);

		//gun.Draw();
		if (gunNumber == 0) gun.Draw();
		else if (gunNumber == 1) gun2.Draw();
	}
};
int main()
{
	// Initialization
	//--------------------------------------------------------------------------------------
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	//initialize variables
	int screenWidth = 1280;
	int screenHeight = 800;
	int playerHealth = 100;
	//Vector2 playerPosition = { (float)screenWidth / 2, (float)screenHeight / 2 };
	Vector2 zombieSpawn[4] = { { 25,25 }, { 1255,25 }, { 1255, 775 }, { 25, 775 } };
	//std::vector<Vector2> zombiePositions;
	std::vector<Zombie> zombies;
	bool startGame = false;
	int framesCounter = 0;
	GameStats stats;

	// Create the window and OpenGL context
	InitWindow(screenWidth, screenHeight, "Window 1");
	GameScreen currentScreen = LOGO;
	//fps target
	SetTargetFPS(60);
	std::vector<bool> col2;
	bool collisionBullet = false;
	bool collisionPlayer = false;
	int zombieTagged = -1;
	Rectangle recCollision = { 0 };
	Rectangle gunBox = { screenWidth / 2.0f, screenHeight / 2.0f, 30, 30 };

	Player player;
	player.position = { screenWidth / 2.0f, screenHeight / 2.0f };

	Bullet bullet;


	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	//Texture wabbit = LoadTexture("wabbit_alpha.png");
	//--------------------------------------------------------------------------------------


	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		// Update
		//----------------------------------------------------------------------------------
		switch (currentScreen) {
		case LOGO:
		{
			if (IsKeyPressed(KEY_ENTER)) startGame = true;
			if (startGame == true) {
				framesCounter++;
			}
			if (framesCounter > 360)
			{
				currentScreen = TITLE;
				framesCounter = 0;
			}
		} break;
		// In title screen
		case TITLE:
		{
			if (IsKeyPressed(KEY_ENTER)) currentScreen = GAMEPLAY;
		} break;
		// In da game
		case GAMEPLAY:
		{
			player.Update();
			//Round Logic here
			//Spawn the zombies 
			//check if we need to spawn more zombies
			while (stats.zombiesRendered < 30 && stats.zombiesUnrendered != 0) {
				//spawn a zombie - health based on round
				//choose random spawn point 1-4
				int idx = GetRandomValue(0, 3);
				Zombie z;
				z.position = zombieSpawn[idx];
				z.health = 100 + (stats.round - 1) * 20;
				zombies.push_back(z);
				//zombiePositions.push_back(z.position);
				//update game stats 
				stats.zombiesRendered++;
				stats.zombiesUnrendered--;
			}
			//starts new round if all zombies are dead
			stats.startNewRound();
			//----------------------------------------------------------------------------------
			//zombies track player
			for (Zombie& zombie : zombies) {
				zombie.trackPlayer(player.position);
			}

			collisionBullet = false;
			if (player.gunNumber == 0){
				for (int i = 0; i < player.gun.bullets.size(); i++)
				{
					Vector2 bulletPos = player.gun.bullets[i].position;
					for (int j = (int)zombies.size() - 1; j >= 0; j--) {
						Zombie& zombie = zombies[j];
						//mark zombie
						if (CheckCollisionCircles(bulletPos, 8, zombie.position, 10))
						{
							collisionBullet = true;
							zombieTagged = j;
							zombie.health -= bullet.damage;
							if (zombie.health <= 0) {
								stats.zombiesKilled++;
								stats.zombiesLeft--;
								stats.score += 10;
								zombieTagged = -1;
								zombies.erase(zombies.begin() + j);
							}
							player.gun.bullets.erase(player.gun.bullets.begin() + i);
							break;
						}
					}
				}
			}
			else if (player.gunNumber == 1){
				for (int i = 0; i < player.gun2.bullets.size(); i++)
				{
					Vector2 bulletPos = player.gun2.bullets[i].position;
					for (int j = (int)zombies.size() - 1; j >= 0; j--) {
						Zombie& zombie = zombies[j];
						//mark zombie
						if (CheckCollisionCircles(bulletPos, 8, zombie.position, 10))
						{
							collisionBullet = true;
							zombieTagged = j;
							zombie.health -= bullet.damage;
							if (zombie.health <= 0) {
								stats.zombiesKilled++;
								stats.zombiesLeft--;
								stats.score += 10;
								zombieTagged = -1;
								zombies.erase(zombies.begin() + j);
							}
							player.gun2.bullets.erase(player.gun2.bullets.begin() + i);
							break;
						}
					}
				}
			}
			//zombie player collision
			collisionPlayer = false;
			for (Zombie& zombie : zombies) {
				//check for zombie player collision
				if (CheckCollisionCircles(player.position, 15.0f, zombie.position, 10.0f)) {
					collisionPlayer = true;
					playerHealth -= zombie.damage;
					if (playerHealth <= 0) {
						currentScreen = ENDING;
					}
				}
				//zombie zombie collision
				for (Zombie& otherZombie : zombies) {
					if (&zombie != &otherZombie) {
						if (CheckCollisionCircles(zombie.position, 10.0f, otherZombie.position, 10.0f)) {
							//simple collision response
							if (zombie.position.x < otherZombie.position.x) {
								zombie.position.x -= 1.0f;
								otherZombie.position.x += 1.0f;
							}
							if (zombie.position.x > otherZombie.position.x) {
								zombie.position.x += 1.0f;
								otherZombie.position.x -= 1.0f;
							}
							if (zombie.position.y < otherZombie.position.y) {
								zombie.position.y -= 1.0f;
								otherZombie.position.y += 1.0f;
							}
							if (zombie.position.y > otherZombie.position.y) {
								zombie.position.y += 1.0f;
								otherZombie.position.y -= 1.0f;
							}
						}
					}
				}
			}
			if (CheckCollisionCircleRec(player.position, 15.0f, gunBox) && IsKeyDown(KEY_E)){
				player.gunNumber = 1;
			}


		} break;
		case ENDING:
		{
			//sets stats back to default
			stats.resetGame();
			playerHealth = 100;
			player.position = { screenWidth / 2.0f, screenHeight / 2.0f };
			//clesar bullets and zombies
			player.gun.bullets.clear();
			zombies.clear();
			if (IsKeyPressed(KEY_ENTER)) currentScreen = TITLE;
		} break;
		default: break;
		}


		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();
		switch (currentScreen)
		{
		case LOGO:
		{
			ClearBackground(RAYWHITE);
			DrawText("Project : Frostbite", 400, 100, 50, SKYBLUE);
			if (framesCounter < 1)
				DrawText("Press Enter To Start", 360, 400, 50, SKYBLUE);
			if (framesCounter >= 60 && framesCounter < 120)
				DrawText("5", 600, 400, 50, SKYBLUE);
			if (framesCounter >= 120 && framesCounter < 180)
				DrawText("4", 600, 400, 50, SKYBLUE);
			if (framesCounter >= 180 && framesCounter < 240)
				DrawText("3", 600, 400, 50, SKYBLUE);
			if (framesCounter >= 240 && framesCounter < 300)
				DrawText("2", 600, 400, 50, SKYBLUE);
			if (framesCounter >= 300 && framesCounter < 360)
				DrawText("1", 600, 400, 50, SKYBLUE);

		} break;
		case TITLE:
		{
			ClearBackground(RAYWHITE);
			DrawText("Press Enter To Start", 360, 400, 50, SKYBLUE);;

		} break;
		case GAMEPLAY:
		{
			ClearBackground(BROWN);
			player.Draw();
			DrawRectangle(screenWidth / 2.0f, screenHeight / 2.0f, 30, 30, ORANGE);
			Vector2 recSize = { 10, 20 };
			DrawCircleV(player.position, 15.0f, SKYBLUE);
			//draw spawn points
			for (int i = 0; i < 4; i++) {
				DrawCircleV(zombieSpawn[i], 25.0f, BLACK);
			}
			//draw zombies
			for (Zombie& zombie : zombies) {
				DrawCircleV(zombie.position, 10, GREEN);
			}
			if (collisionBullet && zombieTagged != -1) {
				DrawCircleV(zombies[zombieTagged].position, 10, RED);
			}
			DrawText("Prototype - Frostbite", 540, 20, 20, LIGHTGRAY);
			//dufault 1280 x 800
			DrawText(TextFormat("FPS: %i", GetFPS()), 100, 20, 20, BLACK);;
			DrawText(TextFormat("Score: %i", stats.score), 600, 775, 20, BLACK);
			DrawText(TextFormat("Round: %i", stats.round), 600, 750, 20, BLACK);
			DrawText(TextFormat("Health: %i", playerHealth), 600, 725, 20, BLACK);
			DrawText(TextFormat("Zombies Left: %i", stats.zombiesLeft), 600, 700, 20, BLACK);
			DrawText(TextFormat("Zombies Killed: %i", stats.zombiesKilled), 600, 675, 20, BLACK);



		} break;
		case ENDING:
		{
			// TODO: Draw ENDING screen here!
			DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
			DrawText("Dead Lol", 20, 20, 40, DARKBLUE);
			DrawText("Press ENTER to return to title screen", 120, 220, 20, DARKBLUE);

		} break;
		default: break;
		}


		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
