# Minesweeper
## Extra Credit assignment ICS 53L

This is a console-based Minesweeper program written in C++. The goal of this program is to be able to figure out the inner game logic behind the game. The program has 3 difficulties as follows:
1) Easy - 8 by 8 grid with 10 hidden mines
2) Intermediate - 16 by 16 grid with 40 mines
3) Advanced - 24 by 24 grid with 99 mines

### What is Minesweeper?

It's a classic puzzle game where the goal is for the player to find all squares (or cells in my program) without mines. If you choose a square with a mine, you lose. At first glance, the game may seem like a random guessing game, but it contains clues that can help you guess where a mine would be. If you encounter a square with a number (let's say N) on it, it means that there are N mines around that square. However, if you find a square with no adjacent mine (and doesn't contain a mine itself), it'll recursively reveal all adjacent squares till they reach squares with a number. There are many variations of this game, but this is the core gameplay of it.

### How do I play?

You can simply compile the Minesweeper.cpp and an executable will be made to run. From there, you can the executable, choose your difficulty, and input the row and column to choose which cell you want reveal.

<img src="images/Minesweeper Console.JPG" alt="Minesweeper on the terminal console"/>
