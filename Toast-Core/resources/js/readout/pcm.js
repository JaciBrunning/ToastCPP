var pcm_readouts = (function() {
    var readouts = [];

    var WIDTH = 115;
    var HEIGHT = 55;
    var SQ_SIZE = 25;
    var SQ_PAD = 5;
    var FONT = "12px sans-serif";

    $(window).on('load', function() {
        var parent = document.getElementById("pcm_readout_table");
        for (var i = 0; i < 2; i++) {
            var p = {
                can_id: -1,
                solenoids: [-2, -2, -2, -2, -2, -2, -2, -2],
                compr_current: -1
            };

            var tr = document.createElement("tr");
            tr.style.height = "55px";
            var td_port = document.createElement("td");
            td_port.innerHTML = "---";
            tr.appendChild(td_port);
            var td_comp_curr = document.createElement("td");
            td_port.innerHTML = "---";
            tr.appendChild(td_comp_curr);
            var td_solenoids = document.createElement("td");
            tr.appendChild(td_solenoids);

            var canvas = document.createElement("canvas");
            canvas.setAttribute("width", WIDTH);
            canvas.setAttribute("height", HEIGHT);
            td_solenoids.appendChild(canvas);
            parent.appendChild(tr);

            p._tr = tr;
            p._td_port = td_port;
            p._td_comp_curr = td_comp_curr;
            p._td_solenoids = td_solenoids;
            p.canvas = canvas;
            p.ctx = canvas.getContext("2d");

            p.setport = function(can_id) {
                this.can_id = can_id;
                this._td_port.innerHTML = can_id;
            };

            p.setcompressor = function(comp_current) {
                this.compr_current = comp_current;
                this._td_comp_curr.innerHTML = comp_current.toFixed(2) + "A";
            };

            p.set = function(id, solenoid, solenoid_black) {
                if (solenoid_black) {
                    this.solenoids[id] = -1;
                } else {
                    this.solenoids[id] = (solenoid ? 1 : 0);
                }
            };

            p.update = function(id, solenoid, solenoid_black) {
                this.set(id, solenoid, solenoid_black);
                this.draw();
            };

            p.draw = function() { draw(this); }
            p.draw();
            readouts[i] = p;
        }
        Memory.listen(function (shm) {
            for (var i = 0; i < 2; i++) {
                var r = readouts[i];
                var m = shm.pneumatics(i);
                if (m.get_init()) {
                    r.setport(m.get_pcm_can_id());
                    r.setcompressor(m.get_comp_current());
                    for (var j = 0; j < 8; j++) {
                        r.set(j, m.get_solenoid(j), m.get_solenoid_black(j));
                    }
                    r.draw();
                }
            }
        });
    });

    function draw(obj) {
        obj.ctx.clearRect(0, 0, WIDTH, HEIGHT);

        var i = 0;
        for (var c = 0; c < 2; c++) {
            for (var r = 0; r < 4; r++) {
                var x = (r * SQ_SIZE + r * SQ_PAD);
                var y = (c * SQ_SIZE + c * SQ_PAD);
                var sol = obj.solenoids[i];

                if (sol == -2) {            // Not Registered
                    obj.ctx.fillStyle = COMMON.COL_GRY_10;
                } else if (sol == -1) {     // Blacklist
                    obj.ctx.fillStyle = COMMON.COL_GRY_80;
                } else if (sol == 0) {      // Off
                    obj.ctx.fillStyle = COMMON.COL_RED;
                } else if (sol == 1) {      // On
                    obj.ctx.fillStyle = COMMON.COL_GRN;
                }

                obj.ctx.fillRect(x, y, SQ_SIZE, SQ_SIZE);
                
                obj.ctx.font = FONT;
                if (sol == -2) {
                    obj.ctx.fillStyle = COMMON.COL_GRY_50;
                } else {
                    obj.ctx.fillStyle = "#FFF";
                }
                obj.ctx.textAlign = "center";
                obj.ctx.textBaseline = "middle";
                obj.ctx.fillText(i, x + SQ_SIZE / 2, y + SQ_SIZE / 2);
                i++;
            }
        } 
    }

    return readouts;
})();