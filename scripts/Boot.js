var Game = {};

Game.Boot = function (game) {
    
};

Game.Boot.prototype = {
    init: function () {

        // The cursors
        this.input.maxPointers = 1;

        this.stage.disableVisibilityChange = true;
    },

    preload: function () {

        this.load.image('preloaderBar', 'assets/preloaderBar.png');
    },

    create: function () {
        this.scale.scaleMode = Phaser.ScaleManager.SHOW_ALL;
        this.scale.pageAlignHorizontally = true;
        this.scale.pageAlignVertically = true;
        
        //this.scale.setGameSize(200, 400);

        if (this.game.device.desktop) {} else {
            this.scale.forceOrientation(true, false);
        }

        this.state.start('Preloader');
    }
};
