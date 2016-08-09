var dio_readouts = (function() {
    var readouts = [];

    $(window).on('load', function() {
        var parent = document.getElementById("dio_readout_table");
        for (var i = 0; i < 26; i++) {
            var d = {
                value: false,
                mode: "---",
                pwm: false,
                pulse: false,
                pwm_pulse: 0,
                pulse_len: 0,
                duty_cycle: 0,
                pwm_rate: 0
            };

            if (i == 10) {
                var tr = document.createElement("tr");
                tr.style.fontSize = "12px";
                tr.innerHTML = "MXP";
                parent.appendChild(tr);
            }

            var tr = document.createElement("tr");
            var td_port = document.createElement("td");
            td_port.innerHTML = "---";
            tr.appendChild(td_port);
            var td_mode = document.createElement("td");
            td_mode.innerHTML = "---";
            tr.appendChild(td_mode);
            var td_value = document.createElement("td");
            td_value.style.backgroundColor = COMMON.COL_GRY_10;
            tr.appendChild(td_value);
            var td_pwm_pulse = document.createElement("td");
            td_pwm_pulse.innerHTML = "---";
            tr.appendChild(td_pwm_pulse);

            parent.appendChild(tr);

            d._tr = tr;
            d._td_port = td_port;
            d._td_mode = td_mode;
            d._td_value = td_value;
            d._td_pwm_pulse = td_pwm_pulse;

            d.setmode = function(port, mode) {
                this.mode = mode;
                this._td_port.innerHTML = port;
                this._td_mode.innerHTML = mode;
            };

            d.set = function(value) {
                this.value = value;
                if (value) {
                    this._td_value.style.backgroundColor = COMMON.COL_GRN;
                } else {
                    this._td_value.style.backgroundColor = COMMON.COL_RED;
                }
            };

            d.setpwmpulse = function(pwm_enabled, pulse_enabled, pulse_len, pwm_duty, pwm_rate) {
                this.pwm = pwm_enabled;
                this.pulse = pulse_enabled;
                this.pulse_len = pulse_len;
                this.duty_cycle = pwm_duty;
                this.pwm_rate = pwm_rate;
                
                if (this.pwm) {
                    this._td_pwm_pulse.innerHTML = "PWM [DC: " + pwm_duty.toFixed(2) + ", RT: " + pwm_rate.toFixed(2) + "]";
                } else if (this.pulse) {
                    this._td_pwm_pulse.innerHTML = "PULS [" + pulse_len.toFixed(5) + "]";
                } else {
                    this._td_pwm_pulse.innerHTML = "---";
                }
            };

            readouts[i] = d;
        }

        Memory.listen(function (shm) {
            for (var i = 0; i < 26; i++) {
                // TODO
                var r = readouts[i];
                var m = shm.dio(i);
                if (m.get_init()) {
                    r.setpwmpulse(m.get_pwm_enabled(),
                        m.is_pulsing(),
                        m.get_pulse_length(),
                        m.get_pwm_duty_cycle(),
                        m.get_pwm_rate());
                    r.setmode(i, m.get_mode() == Memory.ns.Shared.IO.DIO_Mode.OUTPUT ? "OUT" : "IN");
                    r.set(m.get_value());
                }
            }
        });
    });

    function draw(obj) {
        obj.ctx.clearRect(0, 0, WIDTH, HEIGHT);

        var i = 0;
        for (var r = 0; r < 4; r++) {
            for (var c = 0; c < 2; c++) {
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