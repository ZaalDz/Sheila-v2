def parse_input_command_list(command_list):
    input_command_to_output = {"KeyW": "FORWARD",
                               "KeyS": "BACKWARD",
                               "KeyA": "LEFT",
                               "KeyD": "RIGHT"}
    for each_command in command_list:
        if each_command in input_command_to_output:
            return input_command_to_output[each_command]
