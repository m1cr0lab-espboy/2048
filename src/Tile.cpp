/**
 * -----------------------------------------------------------------------------
 * @file   Tile.cpp
 * @author Stéphane Calderoni (https://github.com/m1cr0lab)
 * @brief  Tile handler
 * -----------------------------------------------------------------------------
 */

#include "Tile.h"
#include "assets.h"

Tile::Tile(uint8_t const i, uint8_t const j) { init(i, j); }

void Tile::init(uint8_t const i, uint8_t const j) {

    pow2       = random(10) == 0 ? 2 : 1;
    x          = j * TILE_SIZE + ((j+1) << 2);
    y          = i * TILE_SIZE + ((i+1) << 2);
    collapser  = nullptr;
    _scale     = 50;
    arising    = true;
    sliding    = false;
    collapsing = false;

}

void Tile::arise() {

    uint8_t ds = 100 - _scale;

    if (ds < 4) {
         arising = false;
        _scale   = 100;
    } else {
        _scale += ds >> 1;
    }

}

void Tile::slide(uint8_t const i, uint8_t const j) {

    uint8_t tx = j * TILE_SIZE + ((j+1) << 2);
    uint8_t ty = i * TILE_SIZE + ((i+1) << 2);

    if (collapsing) {

        int8_t  dx = tx - collapser->x;
        int8_t  dy = ty - collapser->y;
        uint16_t d = dx * dx + dy * dy;

        if (d < 4) {
             collapser->x = tx;
             collapser->y = ty;
            _scale        = 150;
             sliding      = false;
        } else {
            collapser->x += dx >> 1;
            collapser->y += dy >> 1;
        }

    }

    int8_t  dx = tx - x;
    int8_t  dy = ty - y;
    uint16_t d = dx * dx + dy * dy;

    if (d < 4) {
         x       = tx;
         y       = ty;
        _scale   = 150;
         if (!collapsing) sliding = false;
    } else {
        x += dx >> 1;
        y += dy >> 1;
    }

}

void Tile::collapse() {

    uint8_t ds = _scale - 100;

    if (ds < 4) {
         collapsing = false;
         collapser  = nullptr;
        _scale      = 100;
    } else {
        _scale -= ds >> 1;
    }

}

void Tile::draw(LGFX_Sprite * const fb) {

    if (arising) { _drawScaling(fb); return; }

    if (collapsing) {
        if (sliding) {
            collapser->_draw(fb, collapser->x, collapser->y);
        } else {
            _drawScaling(fb); return;
        }
    }

    _draw(fb, x, y);

}

void Tile::_drawScaling(LGFX_Sprite * const fb) {

    LGFX_Sprite tile(fb);
    tile.createSprite(TILE_SIZE, TILE_SIZE);
    tile.setColorDepth(8);
    tile.createPalette();

    _draw(&tile, 0, 0);

    float_t zoom = _scale / 100.f;

    tile.pushRotateZoom(
        x + (TILE_SIZE >> 1),
        y + (TILE_SIZE >> 1),
        0,
        zoom,
        zoom
    );

    tile.deleteSprite();

}

void Tile::_draw(LGFX_Sprite * const fb, uint8_t const x, uint8_t const y) {

    uint8_t p = sliding && collapsing ? pow2 - 1 : pow2;

    fb->drawBitmap(
        x,
        y,
        TILE,
        TILE_SIZE,
        TILE_SIZE,
        p
    );

    fb->drawBitmap(
        x + 2,
        y + 5,
        POWER_OF_TWO + (p - 1) * POWER_OF_TWO_FRAME_SIZE,
        POWER_OF_TWO_WIDTH,
        POWER_OF_TWO_HEIGHT,
        p < 3 ? 19 : 20
    );

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