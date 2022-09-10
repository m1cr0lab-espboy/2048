/**
 * -----------------------------------------------------------------------------
 * @file   Game.h
 * @author Stéphane Calderoni (https://github.com/m1cr0lab)
 * @brief  Game engine
 * -----------------------------------------------------------------------------
 */

#pragma once

#include <ESPboy.h>
#include "Tile.h"

class Game {

    public:

        void begin();
        void loop();

    private:

        static uint8_t    constexpr _EEPROM_ADDR       = 1;
        static char const constexpr _EEPROM_DATA_TAG[] = "2048";

        struct EEPROM_Data {
            char     tag[5];
            uint16_t highscore;
        };

        enum class State : uint8_t {
            SPLASH,
            LAUNCH,
            START,
            INIT,
            SPAWN,
            PLAY,
            SLIDING,
            LOST,
            GAME_OVER
        };

        enum class Direction : uint8_t {
            LEFT,
            UP,
            RIGHT,
            DOWN
        };

        enum class Transform : uint8_t {
            HORIZONTAL_FLIP,
            LEFT_ROTATION,
            RIGHT_ROTATION
        };

        EEPROM_Data _backup_data;

        LGFX_Sprite *_fb;

        Tile *_board[4][4] = { nullptr };
        Tile *_phantom[16] = { nullptr };

        uint8_t  _splash_step;
        uint8_t  _splash_tiles_y[4];
        uint32_t _last;

        uint8_t  _phantom_count;
        uint8_t  _free_tiles;
        uint32_t _score;
        uint32_t _higher;
        uint32_t _moves;
        bool     _spawned;
        bool     _slided;
        bool     _collapsed;
        State    _state;

        void _initSplashFrameBuffer();
        void _initPlayFrameBuffer();

        void _update();
        void _draw();
        void _drawSplash();
        void _drawBoard();
        void _drawGameOver();

        void _launch();
        void _start();
        void _init();
        void _spawn();
        void _play();

        Tile *_spawnTile();

        void _slide(uint8_t const i);
        void _collapse(uint8_t const i);
        void _tweak(Transform const t);
        void _move(Direction const d);
        void _showMove();

        void _slideLeft();
        void _slideRight();
        void _slideUp();
        void _slideDown();

        bool _isSqueezable();

        void _lost();
        void _gameOver();

        void _loadHighScore();
        void _saveHighScore();

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