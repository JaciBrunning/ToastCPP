R"(
window.Simulation = (function() {
    var loc = window.location, new_uri;
    new_uri = "ws:"
    new_uri += "//" + loc.host;
    new_uri += "/simulation/socket/data";

    var socket = new WebSocket(new_uri);
    socket.binaryType = 'arraybuffer';

    var exports = {};
    var gamepads = [null, null, null, null, null, null];
    var gamepad_extra = [{}, {}, {}, {}, {}, {}];
    var gamepad_map = [-1, -1, -1, -1, -1, -1];

    var ds_ns = FWI.Toast.Memory.Shared.DS;
    var mem_ns = FWI.Toast.Memory;

    var joys = [new ds_ns.Joystick(), new ds_ns.Joystick(), new ds_ns.Joystick(),
                new ds_ns.Joystick(), new ds_ns.Joystick(), new ds_ns.Joystick()];
    var empty_joy = new ds_ns.Joystick();
    empty_joy.allocate();

    function buttons_to_pov(horiz, vert) {
        if (horiz == 0 && vert == 0) return -1;
        var val = Math.round(Math.atan2(horiz, vert) * 57.2958);
        if (val < 0) return 360 + val;
        return val;
    }

    var remaps = {
        "xbox360": {
            is_xbox: true,
            type: ds_ns.JoystickType.XINPUT_GAMEPAD,
            axis: [
                { source: { a: 0 }, type: ds_ns.JoystickAxisType.X },
                { source: { a: 1 }, type: ds_ns.JoystickAxisType.Y },
                { source: { b: 6 }, type: ds_ns.JoystickAxisType.Z },
                { source: { b: 7 }, type: ds_ns.JoystickAxisType.TWIST },
                { source: { a: 2 }, type: ds_ns.JoystickAxisType.THROTTLE },
                { source: { a: 3 }, type: ds_ns.JoystickAxisType.X }
            ],
            button: [
                { source: { b: 0 } }, { source: { b: 1 } },
                { source: { b: 2 } }, { source: { b: 3 } },
                { source: { b: 4 } }, { source: { b: 5 } },
                { source: { b: 8 } }, { source: { b: 9 } },
                { source: { b: 10 } }, { source: { b: 11 } }
            ],
            pov: [
                { source: { f: 
                    function(gp) {
                        var vert = 0;
                        var horiz = 0;
                        if (gp.buttons[12].pressed) vert += 1;  // Up
                        if (gp.buttons[13].pressed) vert -= 1;  // Down
                        if (gp.buttons[14].pressed) horiz -= 1; // Left
                        if (gp.buttons[15].pressed) horiz += 1; // Right

                        return buttons_to_pov(horiz, vert);
                    }
                } }
            ]
        },
        "xbox360_mac_driver": {
            is_xbox: true,
            type: ds_ns.JoystickType.XINPUT_GAMEPAD,
            axis: [
                { source: { a: 0 }, type: ds_ns.JoystickAxisType.X },
                { source: { a: 1 }, type: ds_ns.JoystickAxisType.Y },
                { source: { a: 2 }, type: ds_ns.JoystickAxisType.Z },
                { source: { a: 5 }, type: ds_ns.JoystickAxisType.TWIST },
                { source: { a: 3 }, type: ds_ns.JoystickAxisType.THROTTLE },
                { source: { a: 4 }, type: ds_ns.JoystickAxisType.X }
            ],
            button: [
                { source: { b: 0 } }, { source: { b: 1 } },
                { source: { b: 2 } }, { source: { b: 3 } },
                { source: { b: 4 } }, { source: { b: 5 } },
                { source: { b: 6 } }, { source: { b: 7 } },
                { source: { b: 8 } }, { source: { b: 9 } }
            ],
            pov: [
                { source: { f: 
                    function(gp) {
                        var vert = 0;
                        var horiz = 0;
                        if (gp.buttons[11].pressed) vert += 1;  // Up
                        if (gp.buttons[12].pressed) vert -= 1;  // Down
                        if (gp.buttons[13].pressed) horiz -= 1; // Left
                        if (gp.buttons[14].pressed) horiz += 1; // Right

                        return buttons_to_pov(horiz, vert);
                    }
                } }
            ]
        }
    };

    function add_gamepad(gamepad) {
        var parent = document.getElementById("joysticks");

        var html = 
            "<div class='gamepad' id='gamepad-" + gamepad.index + "'>" +
                "<strong class='gamepad-header'> " + gamepad.id + " </strong><br />" +
                "ID: <input gp=" + gamepad.index  + " last_gp=" + gamepad.index + " style='width: 50px; margin-right: 10px' type='number' min=0 max=5 step=1 value=0 onchange='return Simulation.update_gamepad_id(this)'/>" +
                "Remap: <select gp=" + gamepad.index + " style='width: 250px' id='remap' onchange='return Simulation.update_gamepad_map(this)'>" +
                "<option value='xbox360' selected> Xbox 360 </option>" +
                "<option value='xbox360_mac_driver'> Xbox 360 (Mac Driver) </option>" + 
                "</select>" +
            "</div>";

        var child = $('<div/>').html(html).contents()[0];
        gamepad_extra[gamepad.index]["element"] = child;
        gamepad_extra[gamepad.index]["mapping"] = "xbox360";
        gamepad_map[gamepad.index] = gamepad.index;

        parent.appendChild(child);
    };

    function remove_gamepad(gamepad) {
        document.getElementById("gamepad-" + gamepad.index).remove();
    };

    function update_gamepad_id(input) {
        if (input.value >= 6) input.value = 5;
        if (input.value < 0) input.value = 0;

        var last = Number(input.getAttribute('last_gp'));
        var gp_id = Number(input.getAttribute('gp'));
        gamepad_map[last] = -1;
        gamepad_map[input.value] = gp_id;
        input.setAttribute('last_gp', input.value);
        console.log("Gamepad: " + gp_id + " changed DS ID from " + last + " to " + input.value);
        return true;
    };

    function update_gamepad_map(select) {
        var name = select.options[select.selectedIndex].value;
        var gp_id = Number(select.getAttribute("gp"));
        gamepad_extra[gp_id]["mapping"] = name;
        console.log("Gamepad: " + gp_id + " changed mapping to " + name);
        return true;
    };

    function resolve_map(obj, gamepad) {
        if (obj.source.hasOwnProperty("b")) {
            return gamepad.buttons[obj.source.b].value;
        } else if (obj.source.hasOwnProperty("a")) {
            return gamepad.axes[obj.source.a];
        } else if (obj.source.hasOwnProperty("f")) {
            return obj.source.f(gamepad);
        }
        return 0;
    }

    var _states = {
        "disabled": mem_ns.RobotState.DISABLED,
        "auto": mem_ns.RobotState.AUTO,
        "teleop": mem_ns.RobotState.TELEOP,
        "test": mem_ns.RobotState.TEST
    };

    function set_state(state) {
        if (_states.hasOwnProperty(state)) {
            var id = _states[state];
            socket.send(Uint8Array.from([0x02, id]));
        }
    }

    socket.onmessage = function(event) {
        var data = event.data;
        var by = new Uint8Array(data);

        if (by[0] === 0x02) { // States
            for (var st in _states) {
                var is_target = _states[st] === by[1];
                var elem = document.getElementById("but_" + st);

                elem.classList.remove("button-outline");
                if (!is_target) elem.classList.add("button-outline");
            }
        }
    };

    function update_loop() {
        var gps = navigator.getGamepads();
        var internal_map = [empty_joy, empty_joy, empty_joy, empty_joy, empty_joy, empty_joy];
        for (var i = 0; i < gps.length; i++) {
            gamepads[i] = gps[i];
        }

        var ex = false;
        try {
            for (var i = 0; i < 6; i++) {
                var id = gamepad_map[i];

                if (gamepad_map.hasOwnProperty(i) && id != -1 && id != null) {
                    joys[id].allocate();
                    var gamepad = gamepads[id];
                    var extra = gamepad_extra[id];
                    var mapping = remaps[extra.mapping];

                    if (gamepads.hasOwnProperty(id) && gamepad != null && gamepad.connected) {
                        var anyPressed = false;
                        var buttons = [];
                        var axis = [];
                        var pov = [];
                        var but_byte = 0;

                        for (var b = 0; b < mapping.button.length; b++) {
                            buttons[b] = Math.round(resolve_map(mapping.button[b], gamepad)) !== 0;
                            if (buttons[b]) {
                                anyPressed = true;
                                but_byte |= (1 << b);
                            }
                        }
                        joys[id].set_button_mask(but_byte);

                        for (var a = 0; a < mapping.axis.length; a++) {
                            axis[a] = resolve_map(mapping.axis[a], gamepad);
                            joys[id].set_axis(a, axis[a] < 0 ? axis[a] * 128 : axis[a] * 127);
                            joys[id].get_descriptor().set_axis_type(a, mapping.axis[a].type);
                        }
                        for (var p = 0; p < mapping.pov.length; p++) {
                            pov[p] = resolve_map(mapping.pov[p], gamepad);
                            joys[id].set_pov(p, pov[p]);
                        }
                        extra.element.style.backgroundColor = anyPressed ? "#dcd" : "";

                        joys[id].set_num_axis(mapping.axis.length);
                        joys[id].set_num_button(mapping.button.length);
                        joys[id].set_num_pov(mapping.pov.length);

                        joys[id].get_descriptor().set_is_xbox(mapping.is_xbox);
                        joys[id].get_descriptor().set_type(mapping.type);
                        joys[id].get_descriptor().set_name_length(gamepad.id.length);
                        joys[id].get_descriptor().set_name(gamepad.id);
                        joys[id].get_descriptor().set_axis_count(mapping.axis.length);

                        internal_map[i] = joys[id];
                    }
                }   
            }

            for (var i = 0; i < 6; i++) {
                if (socket.readyState === socket.OPEN)
                    socket.send(Uint8Array.from([0x01, i].concat(internal_map[i].get_bytes())));
            }
        } catch (e) {
            console.log(e);
            ex = true;
        }
        var time = Math.floor(1000 / Common.config_sync("Toast-Simulation").http.joystick_update_frequency);
        setTimeout(update_loop, ex ? 1000 : time);
    };

    $(window).on('load', function() {
        var gps = navigator.getGamepads();
        for (var gp in gps) {
            if (gps.hasOwnProperty(gp)) {
                gamepads[gp] = gps[gp];
                add_gamepad(gps[gp]);
            }
        }
        update_loop();
    });

    window.addEventListener("gamepadconnected", function(e) {
        gamepads[e.gamepad.index] = e.gamepad;
        add_gamepad(e.gamepad);
    });

    window.addEventListener("gamepaddisconnected", function(e) {
        gamepads[e.gamepad.index] = null;
        remove_gamepad(e.gamepad);
    });

    exports.gamepads = gamepads;
    exports.gamepad_map = gamepad_map;
    exports.update_gamepad_id = update_gamepad_id;
    exports.update_gamepad_map = update_gamepad_map;
    exports.set_state = set_state;

    return exports;
})();
)"