var Memory = Memory || (function() {
    var loc = window.location, new_uri;
    new_uri = "ws:"
    new_uri += "//" + loc.host;
    new_uri += "/socket/shared_memory";

    var ns = FWI.Toast.Memory;
    var pool = new ns.SharedPool();
    pool.allocate();

    var socket = new WebSocket(new_uri);
    socket.onmessage = function(event) {
        pool.from_string(atob(event.data));
    };

    var send_update = function() {
        socket.send("UPDATE");
    };
    return { ns: ns, pool: pool, socket: socket, update: send_update };
})();