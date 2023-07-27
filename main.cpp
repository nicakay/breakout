#include "raylib.h"
#include <stdlib.h>

int main()
{
    // Window dimensions
    const int windowWidth{1000};
    const int windowHeight{600};
    InitWindow(windowWidth, windowHeight, "Breakout!");

    // Loading Sound files
    InitAudioDevice();
    Wave paddleHitWav = LoadWave("sounds/paddle_hit.wav");
    Sound paddleHit = LoadSoundFromWave(paddleHitWav);

    
    // --- TEXT BAR ---
    const int space{25};
    const int b_x{760};
    const int r_x{b_x + space};
    const int e_x{r_x + space};
    const int a_x{e_x + space};
    const int k_x{a_x + space};
    const int o_x{k_x + space};
    const int u_x{o_x + space};
    const int t_x{u_x + space};
    const int title_y{20};
    const int titleFontSize{30};

    // --- WALLS ---
    // Top Wall coordinates
    Rectangle topWall;
    topWall.x = 0;
    topWall.y = 65;
    topWall.width = windowWidth;
    topWall.height = 50;

    // Left Wall coordinates
    Rectangle leftWall;
    leftWall.x = topWall.x;
    leftWall.y = topWall.y + topWall.height;
    leftWall.width = 65;
    leftWall.height = windowHeight - topWall.y - topWall.height;

    // Right Wall coordinates
    Rectangle rightWall;
    rightWall.x = windowWidth - leftWall.width;
    rightWall.y = topWall.y + topWall.height;
    rightWall.width = leftWall.width;
    rightWall.height = windowHeight - topWall.y - topWall.height;

    // Bottom (Game Over) Wall coordinates
    Rectangle gameOverWall;
    gameOverWall.x = 65;
    gameOverWall.y = windowHeight - 5;
    gameOverWall.width = windowWidth - leftWall.width - rightWall.width;
    gameOverWall.height = 5;

    // --- PADDLE ---
    // Paddle coordinates
    Rectangle paddle;
    paddle.x = topWall.height + 215;
    paddle.y = 560;
    paddle.width = 100;
    paddle.height = 15;
    const int paddle_speed{10}; // dt 400
    
    // --- BALL ---
    // Ball coordinates
    Rectangle ball;
    ball.x  = 350;
    ball.y = 320;
    ball.width = 15;
    ball.height = 15;
    int ball_direction_x{3}; // dt 230
    int ball_direction_y{4}; // dt 240

    // --- BRICKS ---
    // Bricks dimensions
    Rectangle brick;
    brick.width = 60;
    brick.height = 18;
    const int brickSpacing{5}; 

    // Bricks coordinates
    brick.x = topWall.height + 30;
    brick.y = topWall.y + topWall.height + 15;
    Rectangle brickLine2 = brick;
    brickLine2.y = brick.y + brick.height + brickSpacing;
    Rectangle brickLine3 = brick;
    brickLine3.y = brickLine2.y + brick.height + brickSpacing;
    Rectangle brickLine4 = brick;
    brickLine4.y = brickLine3.y + brick.height + brickSpacing;
    Rectangle brickLine5 = brick;
    brickLine5.y = brickLine4.y + brick.height + brickSpacing;
    Rectangle brickLine6 = brick;
    brickLine6.y = brickLine5.y + brick.height + brickSpacing;
    Rectangle brickLine7 = brick;
    brickLine7.y = brickLine6.y + brick.height + brickSpacing;
    Rectangle brickLine8 = brick;
    brickLine8.y = brickLine7.y + brick.height + brickSpacing;

    // Define the Brick Colors and their Y positions so that I can use it later to create a loop in order to draw all the bricks
    Color brickColors[] = { RED, RED, ORANGE, ORANGE, GREEN, GREEN, YELLOW, YELLOW };
    const float brickYPositions[] = { brick.y, brickLine2.y, brickLine3.y, brickLine4.y,
                          brickLine5.y, brickLine6.y, brickLine7.y, brickLine8.y };

    // I need to store each brick in a separate Rectangle variable, as I will need this later in order to manage collision with the ball and each brick to make them disappear. In my game there are 8 lines with 13 bricks each, so a two-dimensional array would be helpful
    const int numLines{8};
    const int numCols{13};
    Rectangle bricks[numLines][numCols];
    bool helpingArray[numLines][numCols];

    for (int line = 0; line < numLines; line++) {
        for (int col = 0; col < numCols; col++) {
            int brickXPosition = brick.x + col * (brick.width + brickSpacing);
            int brickYPosition = brickYPositions[line];
            bricks[line][col] = { static_cast<float>(brickXPosition), static_cast<float>(brickYPosition),
                                  static_cast<float>(brick.width), static_cast<float>(brick.height) };
            helpingArray[line][col] = true;
        }
    }

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(BLACK);

        if (CheckCollisionRecs(ball, gameOverWall))
        {
            DrawText("Game Over!", 350, 240, 50, RED);
        }
        else
        {
            // **** DRAW section ****

            // Draw the Text Bar
            DrawText("Your Score: 0", 30, 25, 25, YELLOW);
            DrawText("B", b_x, title_y, titleFontSize, RED);
            DrawText("R", r_x, title_y, titleFontSize, ORANGE);
            DrawText("E", e_x, title_y, titleFontSize, GREEN);
            DrawText("A", a_x, title_y, titleFontSize, YELLOW);
            DrawText("K", k_x, title_y, titleFontSize, RED);
            DrawText("O", o_x, title_y, titleFontSize, ORANGE);
            DrawText("U", u_x, title_y, titleFontSize, GREEN);
            DrawText("T", t_x, title_y, titleFontSize, YELLOW);

            // Draw the Top Wall
            DrawRectangle(topWall.x, topWall.y, topWall.width, topWall.height, GRAY);

            // Draw the Left Wall
            DrawRectangle(leftWall.x, leftWall.y, leftWall.width, leftWall.height, GRAY);

            // Draw the Right Wall
            DrawRectangle(rightWall.x, rightWall.y, rightWall.width, rightWall.height, GRAY);

            // Draw the Bottom (Game Over) Wall
            DrawRectangle(gameOverWall.x, gameOverWall.y, gameOverWall.width, gameOverWall.height, BLANK);
            
            // Draw the Paddle
            DrawRectangle(paddle.x, paddle.y, paddle.width, paddle.height, BLUE);

            // Draw all the bricks (8 lines / 13 bricks each) with a nested loop.
            for (int line = 0; line < numLines; line++) {
                for (int col = 0; col < numCols; col++) {
                    if (helpingArray[line][col])
                    {
                        DrawRectangleRec(bricks[line][col], brickColors[line]);
                    }   
                }
            }

            // Check each Brick for collision with the Ball
            for (int line = 0; line < numLines; line++) {
                for (int col = 0; col < numCols; col++) {
                    if (CheckCollisionRecs(ball, bricks[line][col]))
                    {
                        ball_direction_y = -ball_direction_y;
                        helpingArray[line][col] = false;
                    }
                }
            }

            // Draw the Ball
            DrawRectangle(ball.x, ball.y, ball.width, ball.height, BLUE);


            // *** Game Logic section ***

            // Move the Paddle right if the Right Arrow key or the D key is pressed
            if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) && paddle.x < windowWidth - paddle.width - rightWall.width - 15)
            {
                paddle.x += paddle_speed;
            }
            else if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) && paddle.x > leftWall.width + 15)
            {
                paddle.x -= paddle_speed;
            }

            // Move the Ball
            ball.x += ball_direction_x;
            ball.y += ball_direction_y;

            // Check the Ball for collision with the Paddle
            if (CheckCollisionRecs(ball, paddle))
            {
                int randNum = rand() % 3 + 1;

                // Reverse vertical direction and change the angle a little bit
                ball_direction_y = -randNum; 

                // Play sound effect on hit
                PlaySound(paddleHit);

            }

            // Check the Ball for collision with the Top Wall
            if (CheckCollisionRecs(ball, topWall))
            {
                // Reverse vertical direction
                ball_direction_y = -ball_direction_y; 

                // Play sound effect on hit
                PlaySound(paddleHit);
            }

            // Check the Ball for collision with the Left or the Right Wall
            if (CheckCollisionRecs(ball, leftWall) || CheckCollisionRecs(ball, rightWall))
            {
                // Reverse horizontal direction
                ball_direction_x = -ball_direction_x; 

                // Play sound effect on hit
                PlaySound(paddleHit);
            }

        }   

        EndDrawing();
    }
    UnloadSound(paddleHit);
    UnloadWave(paddleHitWav);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}