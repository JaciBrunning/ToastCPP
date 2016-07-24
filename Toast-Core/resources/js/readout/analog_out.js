var an_out_readouts = (function() {
    var readouts = [];

    var HEIGHT = 15;
    var WIDTH = 300;
    var MAXVOLT = 5.0;
    var FONT = "14px sans-serif";

    $(window).on('load', function() {
        var parent = document.getElementById("an_out_readout_holder");
        for (var i = 0; i < 2; i++) {
            var elem = document.createElement("canvas");
            elem.setAttribute("width", WIDTH);
            elem.setAttribute("height", HEIGHT);
            parent.appendChild(elem);

            var r = {
                voltage: 0.0,
                canvas: elem,
                ctx: elem.getContext("2d"),
                port: i
            };
            r.set = function(v) { this.voltage = v; };
            r.draw = function() { draw(this); };
            r.update = function(v) { this.set(v); this.draw(); };

            r.draw();
            readouts[i] = r;
        }
    });

    function draw(obj) {
        obj.ctx.clearRect(0, 0, WIDTH, HEIGHT);
        obj.ctx.fillStyle = COMMON.COL_GRY_10;
        obj.ctx.fillRect(WIDTH / 3, 0, 2 * WIDTH / 3.5, HEIGHT);

        obj.ctx.fillStyle = COMMON.COL_GRN;
        obj.ctx.fillRect(WIDTH / 3, 0, 2 * WIDTH / 3.5 * (obj.voltage / MAXVOLT), HEIGHT);

        obj.ctx.font = FONT;
        obj.ctx.fillStyle = "#000";
        obj.ctx.fillText(obj.port, 0, HEIGHT - 2);
        obj.ctx.fillText(obj.voltage.toFixed(2) + "V", 35, HEIGHT - 2);
    }

    return readouts;
})();