var Game = {};

Game.Boot = function (game) {
    "use strict";

};

Game.Boot.prototype = {
    init: function () {

        // The cursors
        this.input.maxPointers = 1;

        this.stage.disableVisibilityChange = true;

        //this.scale.scaleMode = Phaser.ScaleManager.RESIZE;
        //this.scale.fullScreenScaleMode = Phaser.ScaleManager.RESIZE;
        //this.scale.parentIsWindow = true;

    },

    preload: function () {

        this.load.image('preloaderBar', 'assets/preloaderBar.png');
    },

    create: function () {
        this.scale.scaleMode = Phaser.ScaleManager.SHOW_ALL;
        this.scale.pageAlignHorizontally = true;
        this.scale.pageAlignVertically = true;

        if (this.game.device.desktop) {} else {
            this.scale.forceOrientation(true, false);
        }

        this.state.start('Preloader');
    }
};
