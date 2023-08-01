### Breakout
### An old-school Breakout / Arkanoid game creasted in C++ and Raylib

![](https://github.com/nicakay/breakout/blob/main/gifs/bomb_suicide.gif)

# Main features

* The game visual style and colors are based on the look of the original Breakout released on Arcade and Atari in 70s.
* There are 8 rows like in the Arcade version, however in my game there are 13 columns of bricks (In the original Atari there were 14).
* The game fully supports Controllers.
* I implemented a simple Paddle-Ball collision, that allows the player to have more control over the ball (than if it was only the basic reverse Y axis on collision). The paddle top is divided into 6 separated parts, each will make the ball bounce into a different direction.
* Whether you fail or beat the game, you can restart it and try again by pressing the corresponding button that appears on the screen.
* Upon starting or restarting the game, it generates 8 bombs. Each bomb is planted in a random position of each row. Upon breaking the corresponding brick the bomb will show up in the place of the brick that disappeared and will start falling down. The player needs to avoid the falling bombs to survive.
* There are 3 different game end screens: 'Game Over' (+ a failure sound) - appears if the ball is missed (collides with the invisible floor), 'You Died' (+ a failure sound) - appears if the paddle collides with any of the falling bombs, and 'Victory' (+ an applause sound) - appears as soon as the player hits the last brick.
