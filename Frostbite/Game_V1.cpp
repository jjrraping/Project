#include "raylib.h"
#include "resource_dir.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <raymath.h>

// Forward declaration
class Gun;

struct Bullet {
    Vector2 position;
    Vector2 velocity;
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
            Bullet &bullet = bullets[i];
            bullet.position = Vector2Add(bullet.position, Vector2Scale(bullet.velocity, 5));
        }
    }

    void Draw()
    {
        for (int i = 0; i < bullets.size(); i++)
        {
            Bullet bullet = bullets[i];
            DrawCircle(bullet.position.x, bullet.position.y, 2, YELLOW);
        }
    }

    void Shoot(Vector2 playerPos) 
    {
        if (numOfBullets > 0)
        {
            Vector2 mousePosition = GetMousePosition();
            Vector2 direction = Vector2Subtract(mousePosition, playerPos);
            direction = Vector2Normalize(direction);
            bullets.push_back((Bullet) { playerPos, direction });
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

    Player()
    {
        texture = LoadTexture("res/player.jpeg");
    }

    void Update()
    {
        if (IsKeyDown(KEY_W)) velocity.y -= 1;
        if (IsKeyDown(KEY_S)) velocity.y += 1;
        if (IsKeyDown(KEY_D)) velocity.x += 1;
        if (IsKeyDown(KEY_A)) velocity.x -= 1;

        if (IsMouseButtonPressed(0))
        {
            gun.Shoot(position);
        }

        velocity.x *= 0.8;
        velocity.y *= 0.8;

        position.x += velocity.x;
        position.y += velocity.y;

        gun.Update();
    }

    void Draw()
    {
        Vector2 mousePosition = GetMousePosition();
        Vector2 difference = Vector2Subtract(mousePosition, position);
        difference = Vector2Normalize(difference);

        float rotation = (atan2f(difference.y, difference.x) * RAD2DEG) - 90;

        DrawTexturePro(texture, { 0, 0, (float)texture.width, (float)texture.height }, 
                      { position.x, position.y, (float)texture.width, (float)texture.height }, 
                      { texture.width / 2.f, texture.height / 2.f}, rotation, WHITE);
        
        gun.Draw();
    }
};

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    
    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Window 1");
    SetTargetFPS(60);

	bool collision;
	Rectangle recCollision = { 0 };
	Rectangle box = { screenWidth / 2.0f, screenHeight / 2.0f, 30, 30};

    Player player;
    player.position = { screenWidth / 2.0f, screenHeight / 2.0f };

	Bullet bullet;

    SearchAndSetResourceDir("resources");

    while (!WindowShouldClose())
    {
        player.Update();
		collision = false;
        for (int i = 0; i < player.gun.bullets.size(); i++)
        {
            Vector2 bulletPos = player.gun.bullets[i].position;
			collision = CheckCollisionCircleRec(bulletPos,2,box);
        }

        BeginDrawing();
        ClearBackground(BROWN);
        
        player.Draw();
		if (collision){
			DrawRectangleRec(box, RED);
		}else {
			DrawRectangleRec(box, GREEN);
		}
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
