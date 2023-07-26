#include "raylib.h"

int main()
{
    // Window dimensions
    const int windowWidth{1000};
    const int windowHeight{600};
    InitWindow(windowWidth, windowHeight, "Breakout!");

    // Loading Sound files
    InitAudioDevice();
    Wave paddleHitWav = LoadWave("sounds/paddle_hit3.wav");
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
    rightWall.width = 65;
    rightWall.height = windowHeight - topWall.y - topWall.height;

    // --- PADDLE ---
    // Paddle coordinates
    Rectangle paddle;
    paddle.x = topWall.height + 15;
    paddle.y = 560;
    paddle.width = 100;
    paddle.height = 15;
    const int paddle_speed{15};
    
    // --- BALL ---
    // Ball coordinates
    Rectangle ball;
    ball.x = 350;
    ball.y = 320;
    const int ball_size{15};
    int ball_direction_x{3};
    int ball_direction_y{4};

    // --- BRICKS ---
    // Bricks dimensions
    const int brickWidth{60};
    const int brickHeight(18);
    const int brickSpacing{5}; 

    // Bricks coordinates - Line 1
    const int brickCol1_x = topWall.height + 30;
    const int brickLine1_y = topWall.y + topWall.height + 15;

    // Bricks coordinates - Line from 2 to 8, vector Y only
    const int brickLine2_y{brickLine1_y + brickHeight + brickSpacing};
    const int brickLine3_y{brickLine2_y + brickHeight + brickSpacing};
    const int brickLine4_y{brickLine3_y + brickHeight + brickSpacing};
    const int brickLine5_y{brickLine4_y + brickHeight + brickSpacing};
    const int brickLine6_y{brickLine5_y + brickHeight + brickSpacing};
    const int brickLine7_y{brickLine6_y + brickHeight + brickSpacing};
    const int brickLine8_y{brickLine7_y + brickHeight + brickSpacing};

    // Define the Brick Colors and their Y positions so that I can use it later to create a loop in order to draw all the bricks
    Color brickColors[] = { RED, RED, ORANGE, ORANGE, GREEN, GREEN, YELLOW, YELLOW };
    const int brickYPositions[] = { brickLine1_y, brickLine2_y, brickLine3_y, brickLine4_y,
                          brickLine5_y, brickLine6_y, brickLine7_y, brickLine8_y };

    // I need to store each brick in a separate Rectangle variable, as I will need this later in order to manage collision with the ball and each brick to make them disappear. In my game there are 8 lines with 13 bricks each, so a two-dimensional array would be helpful
    // const int numLines{8};
    // const int numCols{13};
    // Rectangle bricks[numLines][numCols];

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

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
        
        // Draw the Paddle
        DrawRectangle(paddle.x, paddle.y, paddle.width, paddle.height, BLUE);

       // Draw all the bricks (8 lines / 13 bricks each) with a nested loop.
        for (int line = 0; line < 8; line++) {
            for (int col = 0; col < 13; col++) {
                int brickXPosition = brickCol1_x + col * (brickWidth + brickSpacing);
                int brickYPosition = brickYPositions[line];
                DrawRectangle(brickXPosition, brickYPosition, brickWidth, brickHeight, brickColors[line]);
            }
        }

        // Draw the Ball
        DrawRectangle(ball.x, ball.y, ball_size, ball_size, BLUE);


        // *** Game Logic section ***

        // Move the Paddle right if the Right Arrow key or the D key is pressed
        if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) && paddle.x < windowWidth - paddle.width - 60)
        {
            paddle.x += paddle_speed;
        }
        else if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) && paddle.x > topWall.height + 15)
        {
            paddle.x -= paddle_speed;
        }

        // Move the Ball
        ball.x += ball_direction_x;
        ball.y += ball_direction_y;

        // Check the Ball for collision with the Paddle or the Top Wall
        if (CheckCollisionRecs(ball, paddle) || CheckCollisionRecs(ball, topWall))
        {
            ball_direction_y = -ball_direction_y; // Reverse vertical direction
            PlaySound(paddleHit);
        }  

        // Check the Ball for collision with the Left or the Right Wall
        if (CheckCollisionRecs(ball, leftWall) || CheckCollisionRecs(ball, rightWall))
        {
            ball_direction_x = -ball_direction_x; // Reverse horizontal direction
            PlaySound(paddleHit);
        }   

        EndDrawing();
    }
    UnloadSound(paddleHit);
    UnloadWave(paddleHitWav);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}