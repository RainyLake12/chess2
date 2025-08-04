# How to make your own bot

Bots are represented bby .dll files (although i might support .so files sometime in the future)

# Functions

Bots must have these functions in their .dll files:
- string getName()
- int makeMove(board b)
- uint32_t *getIcon()
	- Returns a row-major 2d array of colors representing the bot's icon
