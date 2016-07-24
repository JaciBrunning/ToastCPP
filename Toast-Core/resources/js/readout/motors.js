var motor_readouts = (function() {
    var loc = window.location, new_uri;
    new_uri = "ws:"
    new_uri += "//" + loc.host;
    new_uri += "/socket/readout/motors";

    // var socket = new WebSocket(new_uri);
    // socket.onmessage = function(event) {
    //     //TODO How to format this?
    //     // b1 -> allocated length
    //     // - b2 -> port
    //     // - b3 -> interface
    //     // - b4, b5, b6, b7 -> value
    // };

    var readouts = [];

    var HEIGHT = 15;
    var WIDTH = 300;
    var FONT = "14px sans-serif";
    var FONT2 = "11px sans-serif";

    $(window).on('load', function() {
        var parent = document.getElementById("motor_readout_holder");
        for (var i = 0; i < 16; i++) {
            var pdiv = document.createElement("div");
            pdiv.classList += "tooltip";
            pdiv.style = "margin-right: 15px";

            var elem = document.createElement("canvas");
            elem.setAttribute("width", WIDTH);
            elem.setAttribute("height", HEIGHT);
            pdiv.appendChild(elem);
            parent.appendChild(pdiv);

            var div = document.createElement("div");
            div.classList += "tooltiptext";
            div.innerHTML = "This motor has not been allocated yet";
            pdiv.appendChild(div);
            var r = {
                canvas: elem,
                ctx: elem.getContext("2d"),
                value: 0,
                lastvalue: 0,
                port: "---",     // Number for PWM, {number}c for CAN
                tooltip: div
            };
            r.set = function(v) { this.lastvalue = this.value; this.value = v; };
            r.draw = function() { draw(this); };
            r.update = function(v) { this.set(v); this.draw(); };
            r.setport = function(port, intf) {
                var str = port;
                var intf_str = "PWM";
                if (intf == 1) {
                    str = str + "c";
                    intf_str = "CAN";
                }
                this.port = str;
                this.tooltip.innerHTML = intf_str + " #" + port;
                this.draw();
            };
            r.draw();
            readouts[i] = r;
        }
    });

    function draw(obj) {
        obj.ctx.clearRect(0, 0, WIDTH, HEIGHT);

        obj.ctx.fillStyle = COMMON.COL_GRY_10;
        obj.ctx.fillRect(2 * WIDTH / 3 - WIDTH / 3.5, 0, 2 * WIDTH / 3.5, HEIGHT);

        var delta = obj.value - obj.lastvalue;
        var prefix = delta > 0.0 ? "+" : "";
        if (obj.value > 0) {
            obj.ctx.fillStyle = COMMON.COL_GRN;
        } else {
            obj.ctx.fillStyle = COMMON.COL_RED;
        }
        if (obj.value > 0) {
            obj.ctx.fillRect(2 * WIDTH / 3, 0, WIDTH / 3.5 * obj.value + 1, HEIGHT);
        } else if (obj.value < 0) {
            obj.ctx.fillRect(2 * WIDTH / 3 + (WIDTH / 3.5 * obj.value + 1), 0, WIDTH / 3.5 * -obj.value + 1, HEIGHT);
        }
        if (delta > 0) {
            obj.ctx.fillStyle = COMMON.COL_GRN;
        } else if (delta < 0) {
            obj.ctx.fillStyle = COMMON.COL_RED;
        } else {
            obj.ctx.fillStyle = COMMON.COL_GRY_60;
        }
        obj.ctx.font = FONT2;
        obj.ctx.fillText(prefix + delta.toFixed(2), 75, HEIGHT - 2);
        
        obj.ctx.font = FONT;
        obj.ctx.fillStyle = "#000";
        obj.ctx.fillText(obj.port, 0, HEIGHT - 2);
        obj.ctx.fillText(obj.value.toFixed(2), 35, HEIGHT - 2);

        obj.ctx.fillStyle = COMMON.COL_GRY_70;
        obj.ctx.fillRect(2 * WIDTH / 3 - 2, 0, 2, HEIGHT);
    }

    return readouts;
})();