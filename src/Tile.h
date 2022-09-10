/**
 * -----------------------------------------------------------------------------
 * @file   Tile.h
 * @author Stéphane Calderoni (https://github.com/m1cr0lab)
 * @brief  Tile handler
 * -----------------------------------------------------------------------------
 */

#pragma once

#include <ESPboy.h>

class Tile {

    public:

        uint8_t pow2;
        uint8_t x;
        uint8_t y;
        bool    arising;
        bool    sliding;
        bool    collapsing;
        Tile   *collapser;

        Tile(uint8_t const i, uint8_t const j);

        void init(uint8_t const i, uint8_t const j);
        void arise();
        void slide(uint8_t const i, uint8_t const j);
        void collapse();
        void draw(LGFX_Sprite * const fb);
    
    private:

        uint8_t _scale;

        void _draw(LGFX_Sprite * const fb, uint8_t const x, uint8_t const y);
        void _drawScaling(LGFX_Sprite * const fb);

};

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