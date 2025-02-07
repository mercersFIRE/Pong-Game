#include <raylib.h>
#include <iostream>
#include <cmath>
#include <fstream>
long long loadHighScore()
{
    std::ifstream file("res/highscore.txt");
    long long score = 0;
    if (file.is_open())
    {
        file >> score;
        file.close();
    }
    else
    {
        std::cerr << "Unable to open file for reading." << std::endl;
    }
    return score;
}
void saveHighScore(long long score)
{
    std::ofstream file("res/highscore.txt");
    if (file.is_open())
    {
        file << score;
        file.close();
    }
    else
    {
        std::cerr << "Unable to open file for writing." << std::endl;
    }
}
class Ball
{
public:
    float x = 400, y = 400, rad = 40, speedx = 3, speedy = 3, speedtotal = 6, alpha = 0.0f;
    Color color = RED;

    void draw(float ColorAlpha)
    {
        if (alpha < 1.0f)
            alpha += 0.04f;
        DrawCircleGradient(this->x, this->y, this->rad, WHITE, Fade(this->color, std::min(ColorAlpha, alpha)));
    }
    void move(Sound &bounce)
    {
        if (this->x + this->rad >= GetScreenWidth() || this->x - this->rad <= 0)
        {
            this->speedx *= -1;
            PlaySound(bounce);
            this->color = {(unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), 255};
            alpha = 0.0f;
        }
        if (this->y + this->rad >= GetScreenHeight() || this->y - this->rad <= 0)
        {
            this->speedy *= -1;
            PlaySound(bounce);
            this->color = {(unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), 255};
            alpha = 0.0f;
        }
        this->x += speedx;
        this->y += speedy;
    }

    int lost(float playery, Sound &lost, long long &score, long long &highscore)
    {

        if (this->y + this->rad > playery)
        {
            int textWidth = MeasureText("Press SPACE to Play Again", 50);
            PlaySound(lost);
            float alpha = 0.0f;
            while (!(IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
            {
                ClearBackground(BLACK);
                BeginDrawing();
                alpha += 0.01f;
                if (alpha > 1.0f)
                    alpha = 1.0f;
                DrawText("Press SPACE to Play Again", (GetScreenWidth() - textWidth) / 2, (GetScreenHeight() - 50) / 2, 50, Fade(WHITE, alpha));
                DrawText(TextFormat("Your Score : %ld Highest Score : %ld", score, highscore), (GetScreenWidth() - MeasureText(TextFormat("Your Score : %ld Highest Score : %ld", score, highscore), 50)) / 2, (GetScreenHeight() + 50) / 2, 50, Fade(WHITE, alpha));
                EndDrawing();
                if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
                    return 1;
            }
            saveHighScore(std::max(score, highscore));
            highscore = std::max(highscore, score);
            this->x = GetScreenWidth() / 2;
            this->y = GetScreenHeight() / 2;
            this->speedx = 3;
            this->speedy = 3;
            this->speedtotal = 6;
            score = 0;
        }
        return 0;
    }
};

class rectangle
{
public:
    float x = 400, y = GetScreenHeight() - this->height - 20, width = 300, height = 20;
    Color color = {121, 85, 72, 255};
    void draw(float ColorAlpha)
    {
        DrawRectangleRoundedLines(Rectangle{std::min(this->x = GetMouseX(), GetScreenWidth() - this->width), this->y = GetScreenHeight() - this->height - 20, this->width, this->height}, .9, 0, Fade(this->color, ColorAlpha));
    }
};
int loadmenu()
{
    float alpha = 0.0f;
    while (!(IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ESCAPE) || IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)))
    {
        ClearBackground(BLACK);
        BeginDrawing();
        alpha += 0.01f;
        if (alpha > 1.0f)alpha = 1.0f;
        DrawText("Press SPACE to Start", (GetScreenWidth() - MeasureText("Press SPACE to Start", 50)) / 2, (GetScreenHeight() - 50) / 2, 50, Fade(WHITE, alpha));
        DrawText("Press ESC to Exit", (GetScreenWidth() - MeasureText("Press ESC to Exit", 50)) / 2, (GetScreenHeight() + 50) / 2, 50, Fade(WHITE, alpha));
        EndDrawing();
        if (IsMouseButtonPressed(KEY_ESCAPE) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))return 1;
    }
    return 0;
}

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetTargetFPS(GetMonitorRefreshRate(0));
    InitWindow(GetScreenWidth(), GetScreenHeight(), "Pong Game");
    ToggleFullscreen();
    InitAudioDevice();
    
    float width = GetScreenWidth(), height = GetScreenHeight(), ColorAlpha = 0.0f;
    long long score = 0, highscore = 0;
    highscore = loadHighScore();

    Sound bounce = LoadSound("res/bounce2.wav"), lost = LoadSound("res/lost.wav"), music = LoadSound("res/music.mp3");
    Ball ball;
    rectangle player;
    if (loadmenu())
        return 1;

    HideCursor();
    while (!WindowShouldClose())
    {
        if(!IsWindowFocused())SetWindowState(FLAG_WINDOW_MINIMIZED);
        else 
        {
            ClearWindowState(FLAG_WINDOW_MINIMIZED);
            if(!IsWindowFullscreen())ToggleFullscreen();  
        }
        
        
        if (ball.lost(player.y, lost, score, highscore))
            break;
        ball.move(bounce);

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.rad, Rectangle{player.x, player.y, player.width, player.height}))
        {
            float diff = ball.x - (player.x + player.width / 2);
            ball.speedx = ball.speedtotal * diff / player.width * 2;
            ball.speedy = sqrt(ball.speedtotal * ball.speedtotal - ball.speedx * ball.speedx);
            ball.speedy *= -1;
            ball.speedtotal += .5;
            ball.move(bounce);
            PlaySound(bounce);
            score += ball.speedtotal / 2;
            ball.alpha = 0.0f;
            ball.color = {(unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), 255};
        }
        if (ColorAlpha < 1.0f)
            ColorAlpha += 0.01f;
        ClearBackground(BLACK);
        BeginDrawing();
        DrawCircle(width / 2, height / 2, height/8, Fade(Color{38, 38, 38, 255}, ColorAlpha));
        DrawCircle(width / 2, height / 2, height/14, Fade(Color{67, 67, 67, 255}, ColorAlpha));
        DrawText(TextFormat("Score: %lld", score), 10, 50, 50, Fade(WHITE, ColorAlpha));
        DrawLineEx(Vector2{0, (float)height / 2}, Vector2{(float)GetScreenWidth(), (float)height / 2}, 5, Fade(Color{93, 64, 55, 255}, ColorAlpha));
        player.draw(ColorAlpha);
        ball.draw(ColorAlpha);
        DrawFPS(10, 10);
        EndDrawing();
        if (!IsSoundPlaying(music))
            PlaySound(music);
    }
    UnloadSound(bounce);
    UnloadSound(lost);
    UnloadSound(music);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}