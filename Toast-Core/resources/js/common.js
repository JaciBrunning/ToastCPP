window.Common = window.Common || (function() {
    var exports = {};
    $.ajaxSetup({ cache: false }); // IE is cache-happy

    exports.COL_RED = "#E64A19";
    exports.COL_GRN = "#388E3C";
    exports.COL_TXT_GRY = "#606c76";
    for (var i = 1; i <= 10; i++) {
        exports["COL_GRY_" + i + "0"] = "rgba(0, 0, 0, " + i / 10 + ")"
    }

    var cfg_cache = {};

    exports.config = function(name, callback) {
        if (cfg_cache.hasOwnProperty(name) && cfg_cache[name] != null) callback(cfg_cache[name]);
        else {
            $.getJSON("/api/config/" + name + ".json", function(json) {
                cfg_cache[name] = json;
                callback(json);
            });
        }
    };

    exports.config_sync = function(name) {
        if (cfg_cache.hasOwnProperty(name) && cfg_cache[name] != null) return cfg_cache[name];
        else {
            var json = $.ajax({
                type: 'GET',
                url: '/api/config/' + name + '.json',
                dataType: 'json',
                success: function() { },
                data: {},
                async: false
            }).responseJSON;
            cfg_cache[name] = json;
            return json;
        }
    };

    exports.config_clear_cache = function(name) {
        if (cfg_cache.hasOwnProperty(name))
            cfg_cache[name] = null;
    };

    exports.ls = function(directory, callback) {
        console.log("ls called");
        $.getJSON("/api/dirlist", { path: directory }, callback);
    };

    return exports;
})();