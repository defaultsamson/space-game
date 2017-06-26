Game.Preloader = function (game) {

    this.preloadBar = null;

};

Game.Preloader.prototype = {
    preload: function () {

        this.preloadBar = this.add.sprite(this.world.centerX, this.world.centerY, 'preloaderBar');

        this.preloadBar.anchor.setTo(0.5, 0.5);

        this.time.advancedTiming = true;

        this.load.setPreloadSprite(this.preloadBar);

        // LOAD ALL ASSETS

        this.load.tilemap('map', 'assets/level1.csv')
        this.load.image('tileset', 'assets/spritesheet.png')
    },

    create: function () {
        this.state.start('Level1');
    }
};
