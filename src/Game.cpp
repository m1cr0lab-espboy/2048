/**
 * -----------------------------------------------------------------------------
 * @file   Game.cpp
 * @author Stéphane Calderoni (https://github.com/m1cr0lab)
 * @brief  Game engine
 * -----------------------------------------------------------------------------
 */

#include "Game.h"
#include "assets.h"
#include <ESP_EEPROM.h>

void Game::begin() {

    espboy.begin();

    _loadHighScore();

    _fb = new LGFX_Sprite(&espboy.tft);

    _initSplashFrameBuffer();

    for (uint8_t i = 0; i < 4; ++i) _splash_tiles_y[i] = 128;

    _splash_step = 0;
    _last        = millis();
    _state       = State::SPLASH;

}

void Game::_initSplashFrameBuffer() {

    _fb->createSprite(TFT_WIDTH, TFT_HEIGHT);
    _fb->setColorDepth(16);

}

void Game::_initPlayFrameBuffer() {

    _fb->createSprite(TFT_WIDTH, TFT_HEIGHT);
    _fb->setColorDepth(8);
    _fb->createPalette();

    uint16_t c;
    uint8_t  r, g, b;
    for (uint8_t i = 0; i < PALETTE_SIZE; ++i) {

        c = pgm_read_word(PALETTE + i);
        r = c >> 11;
        g = (c >> 5) & 0x3f;
        b = c & 0x1f;

        _fb->setPaletteColor(
            i,
            (r << 3) | (r >> 2),
            (g << 2) | (g >> 4),
            (b << 3) | (b >> 2)
        );

    }

}

void Game::loop() {

    espboy.update();

    _update();
    _draw();

}

void Game::_update() {

    switch (_state) {

        case State::SPLASH:                 return;
        case State::LAUNCH:    _launch();   break;
        case State::START:     _start();    break;
        case State::INIT:      _init();     break;
        case State::SPAWN:     _spawn();    break;
        case State::PLAY:      _play();     break;
        case State::SLIDING:   _showMove(); break;
        case State::LOST:      _lost();     break;
        case State::GAME_OVER: _gameOver();

    }

}

void Game::_draw() {

    switch (_state) {

        case State::SPLASH:
        case State::LAUNCH:    _drawSplash();   break;
        case State::GAME_OVER: _drawGameOver(); break;

        default: _drawBoard();

    }

}

void Game::_drawSplash() {

    _fb->clear();

    uint8_t top = 17;

    _fb->pushImage(
        (TFT_WIDTH - M1CR0LAB_SIZE) >> 1,
        top,
        M1CR0LAB_SIZE,
        M1CR0LAB_SIZE,
        M1CR0LAB,
        M1CR0LAB_TRANS_COLOR
    );

    _fb->setTextColor(0x07f5);
    _fb->setTextDatum(TC_DATUM);
    _fb->drawString(F("presents"), TFT_WIDTH >> 1, top += M1CR0LAB_SIZE + 8);

    _fb->setTextDatum(CC_DATUM);

    for (uint8_t i = 0; i < 4; ++i) {
        uint8_t y = _splash_tiles_y[i];
        if (y < 128) {

            uint8_t x = i * TILE_SIZE + ((i+1) << 2);
            uint8_t p = i == 0 ? 1 : (i == 1 ? 0 : i);

            _fb->drawBitmap(
                x,
                y,
                TILE,
                TILE_SIZE,
                TILE_SIZE,
                pgm_read_word(PALETTE + p)
            );

            _fb->setTextColor(pgm_read_word(PALETTE + (p == 1 || p == 2 ? 19 : 20)));
            _fb->drawNumber(p == 0 ? 0 : 1 << p, x + (TILE_SIZE >> 1), y + (TILE_SIZE >> 1));

        }
    }

    if (millis() - _last >= 1000) {

        top += 20;

        uint8_t dy;

        for (uint8_t i = 0; i < 4; ++i) {
            if (_splash_step == i) {
                dy = _splash_tiles_y[i] - top;
                if (dy < 2) {
                    _splash_tiles_y[i] = top;
                    _splash_step++;
                } else _splash_tiles_y[i] -= dy >> 1;
            } else _state = State::LAUNCH;
        }

    }

    _fb->pushSprite(0, 0);

}

void Game::_drawBoard() {

    _fb->clear(18);

    for (uint8_t i = 0; i < 4; ++i) {
        for (uint8_t j = 0; j < 4; ++j) {
            _fb->drawBitmap(
                j * TILE_SIZE + ((j+1) << 2),
                i * TILE_SIZE + ((i+1) << 2),
                TILE,
                TILE_SIZE,
                TILE_SIZE,
                0
            );
        }
    }

    Tile *t;
    for (uint8_t i = 0; i < 4; ++i) {
        for (uint8_t j = 0; j < 4; ++j) {
            if ((t = _board[i][j]) != nullptr) t->draw(_fb);
        }
    }

    _fb->pushSprite(0, 0);

}

void Game::_drawGameOver() {

    uint8_t x = TFT_WIDTH >> 1;
    uint8_t y = 22;

    _fb->clear(21);

    _fb->setTextDatum(TC_DATUM);
    _fb->setTextColor(6);
    _fb->drawString(F("GAME OVER"), x, y);
    _fb->setTextColor(14);
    _fb->drawString(F("HIGH SCORE"), x, y + 64);
    _fb->setTextColor(12);
    _fb->drawNumber(_backup_data.highscore, x + 4, y + 76);

    _fb->setTextDatum(TR_DATUM);
    _fb->setTextColor(9);
    _fb->drawString(F("Best"),  x - 4, y + 20);
    _fb->drawString(F("Moves"), x - 4, y + 32);
    _fb->drawString(F("Score"), x - 4, y + 44);

    _fb->setTextDatum(TL_DATUM);
    _fb->setTextColor(20);
    _fb->drawNumber(1 << _higher, x + 4, y + 20);
    _fb->drawNumber(_moves,       x + 4, y + 32);
    _fb->drawNumber(_score,       x + 4, y + 44);

    _fb->pushSprite(0, 0);

}

void Game::_launch() {

    if (espboy.button.pressed(Button::ACT)) {

        _fb->deleteSprite();
        _initPlayFrameBuffer();

        _phantom_count = 0;
        _state         = State::START;

    }

}

void Game::_start() {

    for (uint8_t i = 0; i < 4; ++i) {
        for (uint8_t j = 0; j < 4; ++j) {
            _board[i][j] = nullptr;
        }
    }

    _free_tiles = 16;
    _score      = _higher = _moves = 0;
    _spawned    = false;
    _state      = State::INIT;

}

void Game::_init() {

    static Tile *t1 = nullptr;
    static Tile *t2 = nullptr;

    if (!_spawned) {
        _spawned = true;
        t1       = _spawnTile();
        t2       = _spawnTile();
    }

    if (t1->arising) t1->arise();
    if (t2->arising) t2->arise();

    if (!t1->arising && !t2->arising) _state = State::PLAY;

}

void Game::_spawn() {

    static Tile *t = nullptr;

    if (!_spawned) {
        _spawned = true;
        t        = _spawnTile();
    }

    if (t->arising) {
        t->arise();
    } else if (_free_tiles == 0 && !_isSqueezable()) {
        espboy.pixel.flash(Color::hsv2rgb(0), 100, 5, 200);
        _state = State::LOST;
        _last  = millis();
    } else {
        _state = State::PLAY;
    }

}

void Game::_play() {

         if (espboy.button.pressed(Button::LEFT))  _move(Direction::LEFT);
    else if (espboy.button.pressed(Button::UP))    _move(Direction::UP);
    else if (espboy.button.pressed(Button::RIGHT)) _move(Direction::RIGHT);
    else if (espboy.button.pressed(Button::DOWN))  _move(Direction::DOWN);

}

Tile *Game::_spawnTile() {

    uint8_t i, j;
    Tile *t;
    do { t = _board[i = random(4)][j = random(4)]; } while (t != nullptr);

    _free_tiles--;

    if (_phantom_count) {
        t = _phantom[--_phantom_count];
        t->init(i, j);
    } else {
        t = new Tile(i, j);
    }

    return _board[i][j] = t;

}

void Game::_slide(uint8_t const i) {

    for (uint8_t j = 0; j < 3; ++j) {
        uint8_t k = 1;
        if (_board[i][j] == nullptr) {

            while ((_board[i][j+k] == nullptr) && (j+k < 4)) k++;

            if (j+k < 4) {
                _board[i][j]          = _board[i][j+k];
                _board[i][j+k]        = nullptr;
                _board[i][j]->sliding = true;
                _slided               = true;
            }

        }
    }

}

void Game::_collapse(uint8_t const i) {

    Tile *t, *tt;
    for (uint8_t j = 0; j < 3; ++j) {

        t  = _board[i][j];
        tt = _board[i][j+1];

        if (t != nullptr && tt != nullptr && t->pow2 == tt->pow2) {

            _phantom[_phantom_count] = tt;
            _phantom_count++;

            _board[i][j+1] = nullptr;
            _free_tiles++;

            t->pow2++;
            t->collapser  = tt;
            t->collapsing = true;
            t->sliding    = true;

            _collapsed = true;
            _slided    = true;

            if (t->pow2 == 11) espboy.pixel.rainbow(1000, 2);

            if (t->pow2 > _higher) _higher = t->pow2;
            _score += 1 << t->pow2;

        }

    }

}

void Game::_tweak(Transform const t) {

    Tile *b[4][4]; memcpy(b, _board, 16 * sizeof(Tile*));

    for (uint8_t i = 0; i < 4; ++i) {
        for (uint8_t j = 0; j < 4; ++j) {
            switch (t) {
                case Transform::HORIZONTAL_FLIP: _board[i][j] = b[i][3-j]; break;
                case Transform::LEFT_ROTATION:   _board[i][j] = b[j][3-i]; break;
                case Transform::RIGHT_ROTATION:  _board[i][j] = b[3-j][i];
            }
        }
    }

}

void Game::_move(Direction const d) {

    _slided = _collapsed = false;

    switch (d) {
        case Direction::LEFT:  _slideLeft();  break;
        case Direction::UP:    _slideUp();    break;
        case Direction::RIGHT: _slideRight(); break;
        case Direction::DOWN:  _slideDown();
    }

    if (_slided || _collapsed) { _state = State::SLIDING; return; }

}

void Game::_showMove() {

    Tile *t;
    bool slided    = false;
    bool collapsed = false;

    for (uint8_t i = 0; i < 4; ++i) {
        for (uint8_t j = 0; j < 4; ++j) {

            if ((t = _board[i][j]) != nullptr) {

                if (t->sliding) {
                    t->slide(i, j);
                    slided = true;
                } else if (t->collapsing) {
                    t->collapse();
                    collapsed = true;
                }

            }

        }
    }

    if (!slided && !collapsed) {
        _moves++;
        _spawned = false;
        _state   = State::SPAWN;
    }

}

void Game::_slideLeft() {

    for (uint8_t i = 0; i < 4; ++i) {
        _slide(i);
        _collapse(i);
        _slide(i);
    }

}

void Game::_slideRight() { _tweak(Transform::HORIZONTAL_FLIP); _slideLeft(); _tweak(Transform::HORIZONTAL_FLIP); }
void Game::_slideUp()    { _tweak(Transform::LEFT_ROTATION);   _slideLeft(); _tweak(Transform::RIGHT_ROTATION);  }
void Game::_slideDown()  { _tweak(Transform::RIGHT_ROTATION);  _slideLeft(); _tweak(Transform::LEFT_ROTATION);   }

bool Game::_isSqueezable() {

    for (uint8_t i = 0; i < 4; ++i) {
        for (uint8_t j = 0; j < 4; ++j) {
            if (i < 3 && _board[i][j]->pow2 == _board[i+1][j]->pow2) return true;
            if (j < 3 && _board[i][j]->pow2 == _board[i][j+1]->pow2) return true;
        }
    }

    return false;

}

void Game::_lost() {

    if (millis() - _last < 2000) return;

    _saveHighScore();

    memcpy(_phantom, _board, 16 * sizeof(Tile*));
    _phantom_count = 16;

    espboy.fadeOut(); while (espboy.fading()) espboy.update();
    espboy.fadeIn();

    _state = State::GAME_OVER;

}

void Game::_gameOver() {

    if (espboy.button.pressed(Button::ACT)) {

        espboy.fadeOut(); while (espboy.fading()) espboy.update();
        espboy.fadeIn();

        _state = State::START;

    }

}

void Game::_loadHighScore() {

    EEPROM.begin(sizeof(EEPROM_Data));

    if (EEPROM.percentUsed() >= 0) {
        EEPROM.get(_EEPROM_ADDR, _backup_data);
        if (strcmp(_backup_data.tag, _EEPROM_DATA_TAG) == 0) return;
    }

    strncpy((char*)&_backup_data.tag, _EEPROM_DATA_TAG, sizeof(_EEPROM_DATA_TAG));
    _backup_data.highscore = 0;

}

void Game::_saveHighScore() {

    if (_score > _backup_data.highscore) {

        _backup_data.highscore = _score;

        EEPROM.put(_EEPROM_ADDR, _backup_data);
        EEPROM.commit();

    }

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