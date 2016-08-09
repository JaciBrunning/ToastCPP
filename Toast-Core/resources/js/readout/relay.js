var relay_readouts = (function() {
    var readouts = [];

    $(window).on('load', function() {
        var parent = document.getElementById("relay_readout_table");
        for (var i = 0; i < 4; i++) {
            var r = {
                rvs: false,
                fwd: false
            };

            var tr = document.createElement("tr");
            var td_port = document.createElement("td");
            td_port.innerHTML = "#" + i;
            tr.appendChild(td_port);
            var td_fwd = document.createElement("td");
            td_fwd.style.backgroundColor = COMMON.COL_GRY_10;
            tr.appendChild(td_fwd);
            var td_rvs = document.createElement("td");
            td_rvs.style.backgroundColor = COMMON.COL_GRY_10;
            tr.appendChild(td_rvs);

            parent.appendChild(tr);

            r._tr = tr;
            r._td_port = td_port;
            r._td_fwd = td_fwd;
            r._td_rvs = td_rvs;

            r.set = function(fwd, rvs) {
                this.fwd = fwd;
                this.rvs = rvs;
            };

            r.draw = function() {
                this._td_fwd.style.backgroundColor = this.fwd ? COMMON.COL_GRN : COMMON.COL_RED;
                this._td_rvs.style.backgroundColor = this.rvs ? COMMON.COL_GRN : COMMON.COL_RED;
            };

            r.update = function(fwd, rvs) {
                this.set(fwd, rvs);
                this.draw();
            };

            readouts[i] = r;
        }
        Memory.listen(function (shm) {
            for (var i = 0; i < 4; i++) {
                var r = readouts[i];
                var m = shm.relay(i);
                if (m.get_init()) {
                    r.update(m.get_fwd(), m.get_rvs());
                }
            }
        });
    });

    return readouts;
})();