window.BootstrapIndex = (function() {
    var exports = {}

    $(window).on('load', function() {
        $.getJSON("/routes",  function(json) {
            for (var id in json) {
                $("#route_list").append($("<a href='/" + json[id] + "' class='button fade'>" + id + "</a>"))
            }
        });
    });

    return exports;
})();