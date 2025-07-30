# Chess2
A modified version of chess

# Gameplay

Pawns are like in normal chess they move one square ahead and capture diagonally. They can also advance two square on their first move. There is **no en passant. Pawns are worth 1 point.**

Bishops move infinitly diagonally and can jump over friendly pawns. They can **take one square horizontally** but can still take diagonally as well. **Bishops are worth 3 points.**

Rooks move infinitely horizontally and **cannot be taken by pawns. Rooks are worth 5 points.**

Knights move in an L shape (2 horizontally or vetically and 1 the other way). They can jump over any piece and **when they take they can play again *and yes they can chain multiple moves in a row by taking.* Rooks are worth 7 points.**

Queens move infinitely either horizontally or diagonally. **They can jump over any friendly piece. Queens are worth 9 points**

Kings move one square in any direction. **When your king is taken you lose the game making them worth ∞ points.**

How to win:
1. If you take your opponents king you win.
2. If you take all your opponents pieces you win.
3. If your opponent runs out of time you win.
4. If your opponent resigns you win.

Draws:
1. If no pieces except kings move (pawns do not count) for 50 moves (a move is when one player moves a piece) in a row the game ends in a draw.
2. If no one threatens the king for 100 moves the game ends in a draw.
3. If both players agree to draw the game draws.

# Code

To learn how to make your own bot refer to documentation/how_to_make_your_own_bot.md
To manually build the project use CMake