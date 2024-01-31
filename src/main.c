#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 500
#define NEARBLACK \
    CLITERAL(Color) { 15, 15, 15, 255 }

void Draw(void);
void Update(void);

enum STATE
{
    waiting,
    playing,
} STATE;

Vector2 ballPosition = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};
Vector2 ballSpeed = {0};
void DrawBall(Vector2 position);
void UpdateBall(Vector2 *ballPosition, Vector2 *ballSpeed, Rectangle playerRect, Rectangle opponentRect);

int opponentScore;
Rectangle opponent = (Rectangle){30, (float)SCREEN_HEIGHT / 2 - 80 / 2, 10, 80};
void DrawOpponent(Rectangle opponent);
void UpdateOpponent(Rectangle *opponent, Vector2 ballPosition);

int playerScore;
Rectangle player = (Rectangle){(float)SCREEN_WIDTH - 40, (float)SCREEN_HEIGHT / 2 - 80 / 2, 10, 80};
void DrawPlayer(Rectangle player);
void UpdatePlayer(Rectangle *player);

int main()
{

    SetTargetFPS(60);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib - Pong");

    int randSeed = GetRandomValue(0, 1);
    if (randSeed == 0)
    {
        randSeed = -1;
    }

    STATE = waiting;
    playerScore = 0;
    opponentScore = 0;
    float ballSpeedX = 5.0f * randSeed;
    float ballSpeedY = 4.0f * randSeed;
    ballSpeed = (Vector2){ballSpeedX,
                          ballSpeedY};

    while (!WindowShouldClose())
    {
        if (STATE == waiting)
        {
            Draw();
            DrawText("Press space to start the game", SCREEN_WIDTH / 2 - 150, 75, 20, WHITE);

            if (IsKeyPressed(KEY_SPACE))
            {
                STATE = playing;
            }
        }
        else if (STATE == playing)
        {
            Update();
            Draw();
        }
    }

    CloseWindow();

    return 0;
}

void Update(void)
{
    UpdateBall(&ballPosition, &ballSpeed, player, opponent);
    UpdatePlayer(&player);
    UpdateOpponent(&opponent, ballPosition);
}

void Draw(void)
{
    BeginDrawing();

    ClearBackground(NEARBLACK);
    DrawLineEx((Vector2){SCREEN_WIDTH / 2, 0}, (Vector2){SCREEN_WIDTH / 2, SCREEN_HEIGHT}, 1.0f, WHITE);

    DrawText(TextFormat("%d", opponentScore), SCREEN_WIDTH / 4, 10, 20, WHITE);
    DrawText(TextFormat("%d", playerScore), SCREEN_WIDTH / 2 + SCREEN_WIDTH / 4, 10, 20, WHITE);

    DrawBall(ballPosition);

    DrawPlayer(player);

    DrawOpponent(opponent);

    EndDrawing();
}

void DrawOpponent(Rectangle opponent) { DrawRectangleRec(opponent, WHITE); }
void UpdateOpponent(Rectangle *opponent, Vector2 ballPosition)
{
    float centerY = opponent->y + opponent->height / 2;
    if (ballPosition.y + 4 < centerY)
    {
        opponent->y -= 3.0f;
    }
    if (ballPosition.y + 4 > centerY)
    {
        opponent->y += 3.0f;
    }

    if (opponent->y <= 0)
    {
        opponent->y = 0;
    }
    if (opponent->y >= SCREEN_HEIGHT - opponent->height)
    {
        opponent->y = SCREEN_HEIGHT - opponent->height;
    }
}

void DrawPlayer(Rectangle player) { DrawRectangleRec(player, WHITE); }
void UpdatePlayer(Rectangle *player)
{
    if (IsKeyDown(KEY_W))
    {
        player->y -= 3.0f;
    }
    if (IsKeyDown(KEY_S))
    {
        player->y += 3.0f;
    }

    if (player->y <= 0)
    {
        player->y = 0;
    }
    if (player->y >= SCREEN_HEIGHT - 80)
    {
        player->y = SCREEN_HEIGHT - 80;
    }
}

void DrawBall(Vector2 position) { DrawCircleV(position, 8, WHITE); }
void UpdateBall(Vector2 *ballPosition, Vector2 *ballSpeed, Rectangle playerRect, Rectangle opponentRect)
{
    (*ballPosition).x += (*ballSpeed).x;
    (*ballPosition).y += (*ballSpeed).y;

    bool playerCollision = CheckCollisionCircleRec((Vector2){(*ballPosition).x - 4, (*ballPosition).y + 4}, (float)8.0f, playerRect);
    if (playerCollision)
    {
        (*ballSpeed).x *= -1.0f;
    }

    bool opponentCollision = CheckCollisionCircleRec((Vector2){(*ballPosition).x - 4, (*ballPosition).y + 4}, (float)8.0f, opponentRect);
    if (opponentCollision)
    {
        (*ballSpeed).x *= -1.0f;
    }

    if (((*ballPosition).x >= (GetScreenWidth() - 8)) || ((*ballPosition).x <= 8))
    {
        if ((*ballPosition).x <= 8)
        {
            playerScore += 1;
        }
        else
        {
            opponentScore += 1;
        }
        STATE = waiting;
        (*ballPosition) = (Vector2){(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};

        int randSeed = GetRandomValue(0, 1);
        if (randSeed == 0)
        {
            randSeed = -1;
        }
        float ballSpeedX = 4.0f * randSeed;
        float ballSpeedY = 4.0f * randSeed;
        (*ballSpeed) = (Vector2){ballSpeedX,
                                 ballSpeedY};
    }

    if (((*ballPosition).y >= (GetScreenHeight() - 8)) || ((*ballPosition).y <= 8))
    {
        (*ballSpeed).y *= -1.0f;
    }
}