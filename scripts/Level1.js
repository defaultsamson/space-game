Game.Level1 = function (game) {
    this.game = game;
};

var map;
var layer;
//var cursors;

var player;
var controls = {};
var playerSpeed = 150;
var playerJumpSpeed = 400;
const jumpDelayInterval = 100;
var jumpTimer = 0;
var jumpCount = 2;

var showDebug = false;

Game.Level1.prototype = {
    create: function () {

        this.game.physics.arcade.gravity.y = 1200; //1400 = Earth

        this.game.stage.backgroundColor = '#3A5963'

        map = this.game.add.tilemap('map', 16, 16)

        map.addTilesetImage('tileset');
        layer = map.createLayer(0);
        layer.resizeWorld();

        map.setCollisionBetween(0, 3);
        map.setCollision(130);
        map.setCollision(194);
        map.forEach(function (tile) {
            if (tile.index === 194) {
                //tile.collideDown = false;
                tile.collideLeft = false;
                tile.collideRight = false;
                tile.collideUp = true;
                tile.collideDown = false;
                tile.faceUp = true;
                tile.faceDown = false;
                tile.faceLeft = false;
                tile.faceRight = false;
            }
        }, this, 0, 0, map.width, map.height, layer);

        player = this.add.sprite(100, 50, 'player');
        player.smoothed = false;
        player.anchor.setTo(0.5, 0.5);
        player.animations.add('idle', [3, 8], 1, true);
        player.animations.add('jump', [6], 1, true);
        player.animations.add('run', [0, 1, 2, 3, 4, 5, 6, 7, 8, 9], 20, true);
        this.game.physics.arcade.enable(player);
        this.game.camera.follow(player);
        player.body.collideWorldBounds = true;
        player.body.setSize(20, 32, 8, 8);

        controls = {
            right: this.game.input.keyboard.addKey(Phaser.Keyboard.D),
            right_1: this.game.input.keyboard.addKey(Phaser.Keyboard.RIGHT),
            left: this.game.input.keyboard.addKey(Phaser.Keyboard.A),
            left_1: this.game.input.keyboard.addKey(Phaser.Keyboard.LEFT),
            up: this.game.input.keyboard.addKey(Phaser.Keyboard.W),
            up_1: this.game.input.keyboard.addKey(Phaser.Keyboard.SPACEBAR),
            up_2: this.game.input.keyboard.addKey(Phaser.Keyboard.UP),
            down: this.game.input.keyboard.addKey(Phaser.Keyboard.S),
            down_1: this.game.input.keyboard.addKey(Phaser.Keyboard.DOWN),
        };

        // Allows players to vary their jump height
        controls.up.onUp.add(halfPlayerY);
        controls.up_1.onUp.add(halfPlayerY);
        controls.up_2.onUp.add(halfPlayerY);
        //cursors = this.input.keyboard.createCursorKeys();
        //this.camera.scale.x = 2
        //this.camera.scale.y = 4

        function halfPlayerY() {
            if (player.body.velocity.y < 0) {
                player.body.velocity.y *= 0.5;
            }
        }
    },

    resize: function () {

    },

    update: function () {

        this.game.physics.arcade.collide(player, layer);

        player.body.velocity.x = 0;

        var onFloor = player.body.onFloor() || player.body.touching.down

        if (onFloor) {
            jumpCount = 2;
        }

        if (controls.right.isDown || controls.right_1.isDown) {
            if (onFloor) player.animations.play('run');
            player.scale.setTo(1, 1);
            player.body.velocity.x += playerSpeed;
        }

        if (controls.left.isDown || controls.left_1.isDown) {
            if (onFloor) player.animations.play('run');
            player.scale.setTo(-1, 1);
            player.body.velocity.x -= playerSpeed;
        }

        if (controls.down.downDuration() || controls.down_1.downDuration()) {
            showDebug = !showDebug;

            if (showDebug) {
                layer.debug = true;
            } else {
                layer.debug = false;
                this.game.debug.reset();
            }
        }

        if (player.body.velocity.y == 0 && player.body.velocity.x == 0) {
            player.animations.play('idle');
        }

        if ((controls.up.downDuration() || controls.up_1.downDuration() || controls.up_2.downDuration()) && this.game.time.now > jumpTimer) {
            if (jumpCount > 0) {
                player.animations.play('jump');
                player.body.velocity.y = -playerJumpSpeed;
                jumpTimer = this.game.time.now + jumpDelayInterval;
                jumpCount -= 1;
            }
        }

        /*
        if (cursors.up.isDown) {
            this.camera.y -= 4;
            console.log("y: " + this.camera.y)
        } else if (cursors.down.isDown) {
            this.camera.y += 4;
            console.log("y: " + this.camera.y)
        }

        if (cursors.left.isDown) {
            this.camera.x -= 4;
            console.log("x: " + this.camera.x)
        } else if (cursors.right.isDown) {
            this.camera.x += 4;
            console.log("x: " + this.camera.x)
        }*/
    },

    render: function () {
        if (showDebug) {
            this.game.debug.body(player);
        }
    }
};
