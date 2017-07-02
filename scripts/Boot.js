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
        this.scale.scaleMode = Phaser.ScaleManager.EXACT_FIT; // Phaser.ScaleManager.SHOW_ALL USER_SCALE

        this.scale.pageAlignHorizontally = true;
        this.scale.pageAlignVertically = true;

        this.scale.setResizeCallback(function () {
            var width = window.innerWidth;
            var height = window.innerHeight;
            console.log('size: ' + width + ', ' + height);
            this.camera.setSize(width, height);
            this.game.renderer.resize(width, height);
        }, this);


        /* WORKING
        var scale_diff_x = (this.game.width * this.game.scale.scaleFactorInversed.x) - window.innerWidth;
        var scale_diff_y = (this.game.height * this.game.scale.scaleFactorInversed.y) - window.innerHeight;
        this.game.camera.width -= scale_diff_x;
        this.game.camera.height -= scale_diff_y;
        this.game.camera.x += (scale_diff_x * 0.5) * this.game.scale.scaleFactor.x;
        this.game.camera.y += (scale_diff_y * 0.5) * this.game.scale.scaleFactor.y;

        this.scale.pageAlignHorizontally = true;
        this.scale.pageAlignVertically = true;

        this.scale.setGameSize(window.innerWidth, window.innerHeight);*/


        /*
        this.scale.setResizeCallback(function () {
            console.log('dank: ' + window.innerWidth + ', ' + window.innerHeight);

            var scale_diff_x = (this.game.width * this.game.scale.scaleFactorInversed.x) - window.innerWidth;
            var scale_diff_y = (this.game.height * this.game.scale.scaleFactorInversed.y) - window.innerHeight;
            this.game.camera.width = window.innerWidth - scale_diff_x;
            this.game.camera.height = window.innerHeight - scale_diff_y;
            this.game.camera.x += (scale_diff_x * 0.5) * this.game.scale.scaleFactor.x;
            this.game.camera.y += (scale_diff_y * 0.5) * this.game.scale.scaleFactor.y;

        }, this);*/

        if (this.game.device.desktop) {} else {
            this.scale.forceOrientation(true, false);
        }

        this.state.start('Preloader');
    }
};

/*
// set scaleMode and align
this.game.scale.scaleMode = Phaser.ScaleManager.USER_SCALE;
this.game.scale.compatibility.forceMinimumDocumentHeight = true; // seems to do nothing
this.game.scale.pageAlignHorizontally = true; // seems like I would not even need this
this.game.scale.pageAlignVeritcally = true; // seems to do nothing// calculate the scale factor
scaleFactor = deviceW / origW; // set scale
this.game.scale.setUserScale(scaleFactor, scaleFactor);
this.scale.refresh(); // calculate the scaled dimensions of the canvas
canvasW = origW * scaleFactor;
canvasH = origH * scaleFactor; // calculate the Top Distance
canvasTop = (deviceH - canvasH) / 2; // make it a relevant string
canvasTop = canvasTop.toString() + 'px'; // get the after setUserScale() applied style attributes
this.currentStyle = this.game.canvas.getAttribute("style"); // add the marginTop to it
this.newStyle = this.currentStyle + 'margin-top: ' + canvasTop; // set the attribute
this.game.canvas.setAttribute('style', this.newStyle);*/
