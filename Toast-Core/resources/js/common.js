var COMMON = (function() {

    var exports = {};

    exports.COL_RED = "#E64A19";
    exports.COL_GRN = "#388E3C";
    for (var i = 1; i <= 10; i++) {
        exports["COL_GRY_" + i + "0"] = "rgba(0, 0, 0, " + i / 10 + ")"
    }

    return exports;
})();