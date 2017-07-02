Game.Space = function (game) {
    this.game = game;
};

const SCALE = 4;

//var cursors;

var player;
var fire;
var controls = {};
var thrustersMagnitude = 8;
var angularThrustersComponent = Math.sqrt(thrustersMagnitude * thrustersMagnitude / 2);

var stars1;
var stars2;
var stars3;
var lastX;
var lastY;

var showDebug = false;

Game.Space.prototype = {
    create: function () {

        this.game.physics.arcade.gravity.y = 0; //1400 = Earth

        this.game.world.resize(64000, 64000);

        this.game.stage.backgroundColor = '#211a23';
        this.game.renderer.renderSession.roundPixels = true

        stars1 = this.game.add.tileSprite(0, 0, 64000, 64000, 'stars1');
        stars2 = this.game.add.tileSprite(0, 0, 64000, 64000, 'stars2');
        stars3 = this.game.add.tileSprite(0, 0, 64000, 64000, 'stars3');


        player = this.add.sprite(64000 / 2, 64000 / 2, 'ship');
        player.anchor.setTo(0.5, 0.5);
        player.smoothed = false;
        player.scale.setTo(SCALE, SCALE);
        this.game.physics.arcade.enable(player);
        player.body.collideWorldBounds = true;

        this.game.camera.follow(player, Phaser.Camera.FOLLOW_LOCKON, 0.22, 0.22); // FOLLOW_TOPDOWN
        lastX = this.game.camera.position.x;
        lastY = this.game.camera.position.y;

        fire = player.addChild(this.game.add.sprite(0, 20, 'fire'));
        fire.anchor.setTo(0.5, 0.5);
        fire.smoothed = false;
        fire.scale.setTo(1, -1);
        fire.animations.add('on', [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11], 20, true);
        fire.animations.add('off', [12], 1, true);

        controls = {
            d: this.game.input.keyboard.addKey(Phaser.Keyboard.D),
            right: this.game.input.keyboard.addKey(Phaser.Keyboard.RIGHT),
            a: this.game.input.keyboard.addKey(Phaser.Keyboard.A),
            left: this.game.input.keyboard.addKey(Phaser.Keyboard.LEFT),
            w: this.game.input.keyboard.addKey(Phaser.Keyboard.W),
            space: this.game.input.keyboard.addKey(Phaser.Keyboard.SPACEBAR),
            up: this.game.input.keyboard.addKey(Phaser.Keyboard.UP),
            s: this.game.input.keyboard.addKey(Phaser.Keyboard.S),
            down: this.game.input.keyboard.addKey(Phaser.Keyboard.DOWN)
        };
    },

    resize: function () {

    },

    update: function () {

        // Updates background star positions to follow at different speeds 
        {
            var changeX = this.game.camera.position.x - lastX;
            var changeY = this.game.camera.position.y - lastY;

            lastX = this.game.camera.position.x;
            lastY = this.game.camera.position.y;

            stars1.tilePosition.x += changeX / 2;
            stars1.tilePosition.y += changeY / 2;

            stars2.tilePosition.x += changeX / 4;
            stars2.tilePosition.y += changeY / 4;

            stars3.tilePosition.x += changeX / 8;
            stars3.tilePosition.y += changeY / 8;
        }

        var up = controls.w.isDown || controls.up.isDown;
        var down = controls.s.isDown || controls.down.isDown;
        var right = controls.d.isDown || controls.right.isDown;
        var left = controls.a.isDown || controls.left.isDown;

        if (up || down || left || right) {
            fire.animations.play('on');
        } else {
            fire.animations.play('off');
        }

        if (right && up) {
            player.body.velocity.x += angularThrustersComponent;
            player.body.velocity.y -= angularThrustersComponent;
            player.rotation = Math.PI / 4;
        } else if (right && down) {
            player.body.velocity.x += angularThrustersComponent;
            player.body.velocity.y += angularThrustersComponent;
            player.rotation = 3 * Math.PI / 4;
        } else if (left && down) {
            player.body.velocity.x -= angularThrustersComponent;
            player.body.velocity.y += angularThrustersComponent;
            player.rotation = 5 * Math.PI / 4;
        } else if (left && up) {
            player.body.velocity.x -= angularThrustersComponent;
            player.body.velocity.y -= angularThrustersComponent;
            player.rotation = 7 * Math.PI / 4;
        } else if (up) {
            player.body.velocity.y -= thrustersMagnitude;
            player.rotation = 0;
        } else if (right) {
            player.body.velocity.x += thrustersMagnitude;
            player.rotation = Math.PI / 2;
        } else if (down) {
            player.body.velocity.y += thrustersMagnitude;
            player.rotation = Math.PI;
        } else if (left) {
            player.body.velocity.x -= thrustersMagnitude;
            player.rotation = 3 * Math.PI / 2;
        }



        /*
        if (controls.d.isDown) {
            player.body.velocity.x += sideThrusterrs;
        }
        if (controls.right.isDown){
            player.body.angularVelocity += rotationalThrusters;
        }
        if (controls.a.isDown) {
            player.body.velocity.x -= sideThrusterrs;
        }
        if (controls.left.isDown){
            player.body.angularVelocity -= rotationalThrusters;
        }
        if (controls.w.isDown) {
            player.body.velocity.y -= mainThrusters;
        }
        if (controls.s.isDown) {
            player.body.velocity.y += backThrusters;
        }*/


        if (controls.down.downDuration()) {
            showDebug = !showDebug;

            if (showDebug) {} else {
                this.game.debug.reset();
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
