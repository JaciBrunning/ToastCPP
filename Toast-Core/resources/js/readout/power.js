var power_readouts = (function() {
    var readouts = [];
    var elems = null;

    $(window).on('load', function() {
        elems = {
            volts: document.getElementById("pwr_pdp_volts"),
            temp: document.getElementById("pwr_pdp_temp"),
            total_pwr: document.getElementById("pwr_total_pwr"),
            total_nrg: document.getElementById("pwr_total_nrg"),
            total_cur: document.getElementById("pwr_total_cur"),
            v3: document.getElementById("pwr_3v3"),
            v5: document.getElementById("pwr_5v"),
            v6: document.getElementById("pwr_6v")
        };

        var parent = document.getElementById("pwr_cur_table");
        for (var i = 0; i < 8; i++) {
            var tr = document.createElement("tr");
            for (var j = 0; j < 2; j++) {
                var id = i + (j * 8);
                var p = {
                    id: id,
                    current: 0.0
                };
                var td_channel = document.createElement("td");
                td_channel.innerHTML = id;
                tr.appendChild(td_channel);

                var td_current = document.createElement("td");
                td_current.innerHTML = "---";
                tr.appendChild(td_current);

                p._cur = td_current;

                p.set = function(current) {
                    this.current = current;
                    this._cur.innerHTML = p.current.toFixed(2) + "A";
                };

                readouts[id] = p;
            }
            parent.appendChild(tr);
        }

        Memory.listen(function(shm) {
            var p = shm.power();
            for (var i = 0; i < 16; i++) {
                readouts[i].set(p.get_pdp_current(i));
            }

            elems.volts.innerHTML = p.get_pdp_voltage().toFixed(2) + "V";
            elems.temp.innerHTML = p.get_pdp_temperature().toFixed(1) + "&deg;C";
            elems.total_pwr.innerHTML = p.get_pdp_total_power().toFixed(2) + "W";
            elems.total_nrg.innerHTML = p.get_pdp_total_energy().toFixed(2) + "J";
            elems.total_cur.innerHTML = p.get_pdp_total_current().toFixed(2) + "A";
            
            if (!p.get_rio_3V3_enabled()) {
                elems.v3.innerHTML = "3V3 DISABLED";
                elems.v3.style.color = Common.COL_RED;
            } else {
                elems.v3.innerHTML = p.get_rio_voltage_3V3().toFixed(2) + "V / " + p.get_rio_current_3V3().toFixed(1) + "A";
                elems.v3.style.color = Common.COL_TXT_GRY;
            }

            if (!p.get_rio_5V_enabled()) {
                elems.v5.innerHTML = "5V DISABLED";
                elems.v5.style.color = Common.COL_RED;
            } else {
                elems.v5.innerHTML = p.get_rio_voltage_5V().toFixed(2) + "V / " + p.get_rio_current_5V().toFixed(1) + "A";
                elems.v5.style.color = Common.COL_TXT_GRY;
            }

            if (!p.get_rio_6V_enabled()) {
                elems.v6.innerHTML = "6V DISABLED";
                elems.v6.style.color = Common.COL_RED;
            } else {
                elems.v6.innerHTML = p.get_rio_voltage_6V().toFixed(2) + "V / " + p.get_rio_current_6V().toFixed(1) + "A";
                elems.v6.style.color = Common.COL_TXT_GRY;
            }
        });
    });

    return readouts;
})();