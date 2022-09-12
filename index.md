---
layout: default
---

<div class="demo">
    <div class="espboy small">
        <img src="{{ 'assets/images/2048.png' | relative_url }}">
    </div>
</div>

<div class="overview">
    <h1>2048 Game</h1>
    <p class="credit">&copy; 2022 {{ site.author }}</p>
    <div class="description">
        <p>
            Here is a remake of the famous 2048 game for the ESPboy handheld.
        </p>
        <p markdown="1">
            2048 is a single-player sliding tile puzzle game written by Italian web developer [Gabriele Cirulli][2048]. The game's objective is to slide numbered tiles on a grid to combine them to create a tile with the number 2048. However, one can continue to play the game after reaching the goal, creating tiles with higher numbers.
        </p>
        <p>
            Use the directional buttons to drag the tiles in the desired direction, and the <span class="tag">ACT</span> button to (re)start the game.
        </p>
    </div>
    <div class="flasher">
        <esp-web-install-button
            id="upload"
            manifest="{{ 'bin/2048.json' | relative_url }}"
            hide-progress>
            <button id="button" slot="activate">Install</button>
            <span slot="unsupported">
                Your browser does not support Web Serial, which is required to connect to ESPboy. Use Google Chrome or Microsoft Edge instead...
            </span>
            <span slot="not-allowed">You are not allowed to use this on HTTP!</span>
        </esp-web-install-button>
        <div class="progress-bar">
            <span style="width:0%"></span>
        </div>
    </div>
    <div class="source">
        <p class="iconify github" markdown="1">
            [Source code][source]
        </p>
    </div>
</div>

[2048]:   https://github.com/gabrielecirulli/2048
[espboy]: https://m1cr0lab-espboy.github.io/ESPboy
[source]: https://github.com/m1cr0lab-espboy/2048