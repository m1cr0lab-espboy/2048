/**
 * -----------------------------------------------------------------------------
 * @file   main.cpp
 * @author Stéphane Calderoni (https://github.com/m1cr0lab)
 * @brief  Remake of the famous 2048 game you probably know.
 * 
 * @note 2048 is a single-player sliding tile puzzle game written by Italian
 *       web developer Gabriele Cirulli. The game's objective is to slide
 *       numbered tiles on a grid to combine them to create a tile with the
 *       number 2048. However, one can continue to play the game after reaching
 *       the goal, creating tiles with higher numbers.
 * 
 *       2048 was published on GitHub under MIT license in March 2014.
 * 
 * @see  https://github.com/gabrielecirulli/2048
 * 
 * @details You can play the original game online: https://play2048.co/
 * -----------------------------------------------------------------------------
 */

#include "Game.h"

Game game;

void setup() {

    game.begin();

}

void loop() {

    game.loop();

}

/**
 * -----------------------------------------------------------------------------
 * 2048 Game
 * -----------------------------------------------------------------------------
 * Copyright (c) 2022 Stéphane Calderoni (https://github.com/m1cr0lab)
 * Copyright (c) 2014 Gabriele Cirulli (https://github.com/gabrielecirulli/2048)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 * -----------------------------------------------------------------------------
 */