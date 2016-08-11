window.Memory = window.Memory || (function() {
    var loc = window.location, new_uri;
    new_uri = "ws:"
    new_uri += "//" + loc.host;
    new_uri += "/socket/shared_memory";

    var ns = FWI.Toast.Memory;
    var pool = new ns.SharedPool();
    var listeners = [];
    pool.allocate();

    var socket = new WebSocket(new_uri);
    socket.binaryType = 'arraybuffer';

    var send_update = function() {
        socket.send("U");
    };

    socket.onmessage = function(event) {
        // pool.from_string(atob(event.data));
        var data = event.data;
        pool.buf.bytes = new Uint8Array(data);

        for (list in listeners) {
            if (listeners.hasOwnProperty(list)) listeners[list](pool);
        }

        if (socket.readyState == socket.OPEN)
            setTimeout(send_update, Math.floor(1000 / Common.config_sync("Toast-Bootstrap").http.update_frequency));
    };

    socket.onopen = function(event) {
        send_update();
    };

    return {
        ns: ns, pool: pool, socket: socket, update: send_update,
        listen: function (cb) {
            listeners[listeners.length] = cb;
        }
    };
})();