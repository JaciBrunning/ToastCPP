window.BootstrapConfig = (function() {
    var exports = {}
    var cfg = null;
    var can_update = false;
    var set_loading = function() {
        can_update = false;
        document.getElementById("config_area").value = "...Loading";
    };

    $(window).on('load', function() {
        Common.ls("toast/config", function(list) {
            var table = document.getElementById("configList").getElementsByTagName('tbody')[0];
            for (var key in list) {
                if (list.hasOwnProperty(key) && list[key] != null) {
                    var name = list[key].replace(".json", "");
                    var row = table.insertRow(table.rows.length);
                    row.insertCell(0).innerHTML = "<a class=\"pointer\" onclick=\"BootstrapConfig.setConfig('" + name + "');\"> " + name + " </a>"
                }
            }
        })
    });

    exports.setConfig = function(name) {
        Common.config_clear_cache(name);
        cfg = name;
        set_loading();
        Common.config(name, function(config) {
            document.getElementById("config_area").value = JSON.stringify(config, null, 2);
            can_update = true;
        });
    };

    exports.updateConfig = function() {
        if (cfg != null && can_update) {
            var text = document.getElementById("config_area").value;
            var parse = JSON.parse(text)
            var gen = JSON.stringify(parse, null, 2);

            set_loading();
            $.ajax({
                type: "POST",
                url: "/api/bootstrap/setconfig/" + cfg,
                data: gen,
                success: function() { exports.setConfig(cfg); },
                dataType: "text"
            });
        }
    };

    return exports;
})();