Game.Preloader = function (game) {

    //this.preloadBar = null;
};

Game.Preloader.prototype = {
    preload: function () {

        /*
        this.preloadBar = this.add.sprite(this.world.centerX, this.world.centerY, 'preloaderBar');
        this.preloadBar.anchor.setTo(0.5, 0.5);
        this.load.setPreloadSprite(this.preloadBar);
        */
        this.time.advancedTiming = true;

        // LOAD ALL ASSETS

        //var tilemap = this.load.tilemap('map', 'assets/level1.csv');

        //this.load.image('tileset', 'assets/spritesheet.png');

        //this.load.spritesheet('player', 'assets/running.png', 36, 40)

        this.load.path = 'assets/';

        // Space

        this.load.atlas('space', 'sheets/space.png', 'sheets/space.json', null, Phaser.Loader.TEXTURE_ATLAS_JSON_ARRAY);


        //this.load.image('ship', 'assets/ship/ship.png');
        this.load.spritesheet('fire', 'ship/fire.png', 7, 20);

        this.load.image('stars1', 'ambient/stars1.png');
        this.load.image('stars2', 'ambient/stars2.png');
        this.load.image('stars3', 'ambient/stars3.png');
        this.load.image('stars2ls', 'ambient/stars2lightspeed.png');
        this.load.image('stars3ls', 'ambient/stars3lightspeed.png');
        this.load.image('gradient', 'ambient/gradient.png');
        this.load.image('glow', 'ambient/lightspeedGlow.png');

        //this.load.image('sand', 'assets/planets/sand.png');

        this.load.bitmapFont('carrier_command', 'font/carrier_command.png', 'font/carrier_command.xml');
    },

    create: function () {

        this.game.renderer.setTexturePriority(['space']);

        this.state.start('Space');
    }
};
