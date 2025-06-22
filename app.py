from flask import Flask, request, jsonify
import subprocess
import os
from flask_cors import CORS
import shlex  # Add at top

def has_redirection(command):
    # Detect >, >>, or < outside quotes
    return '>' in command or '<' in command

app = Flask(__name__)
CORS(app)

MINISHELL_BINARY = "/mnt/e/PBL(OS)/MiniShell/minishell"

# Set the base project directory
BASE_PROJECT = "PBL(OS)"
CURRENT_DIR = os.path.abspath(os.getcwd())

VALID_COMMANDS = {
    "cd", "pwd", "echo", "export", "unset", "history", "exit", "mkdir", "cat",
    "sort", "ls", "lsall", "touch", "rm", "rmdir", "chmod", "hello", "help",
    "clr", "calc", "greet", "time", "version", "repeat", "files", "dirs"
}

@app.route('/run', methods=['POST'])
def run_command():
    global CURRENT_DIR
    data = request.get_json()
    command = data.get('command')

    if not command:
        return jsonify({'output': 'No command received'}), 400

    tokens = command.strip().split()

    if tokens:
        if tokens[0] not in VALID_COMMANDS:
            return jsonify({'output': f"❌ Error: Unknown command '{tokens[0]}'"}), 400

    # Handle 'cd' command manually
    if tokens and tokens[0] == 'cd':
        if len(tokens) == 1:
            CURRENT_DIR = os.path.abspath("/mnt/e/PBL(OS)/MiniShell")
        else:
            new_path = os.path.join(CURRENT_DIR, tokens[1])
            if os.path.isdir(new_path):
                CURRENT_DIR = os.path.abspath(new_path)
                trimmed_path = CURRENT_DIR.split(BASE_PROJECT, 1)[-1]
                return jsonify({
                    'output': f'📁 Changed directory to {BASE_PROJECT}{trimmed_path}',
                    'cwd': CURRENT_DIR
                })
            else:
                return jsonify({'output': f'No such directory: {tokens[1]}'}), 400

    # Handle 'pwd' command manually
    if tokens and tokens[0] == 'pwd':
        trimmed_path = CURRENT_DIR.split(BASE_PROJECT, 1)[-1]
        return jsonify({
            'output': f'📍 Current Directory: {BASE_PROJECT}{trimmed_path}',
            'cwd': CURRENT_DIR
        })

    try:
        # Run MiniShell binary inside CURRENT_DIR
        use_capture = not has_redirection(command)
        result = subprocess.run(
            [MINISHELL_BINARY],
            input=command + '\n',
            text=True,
            stdout=subprocess.PIPE if use_capture else None,
            stderr=subprocess.PIPE if use_capture else None,
            cwd=CURRENT_DIR,
            timeout=5
        )

        #raw_output = result.stdout or result.stderr
        #lines = raw_output.splitlines()

        if use_capture:
            raw_output = result.stdout or result.stderr
            lines = raw_output.splitlines()

            def should_keep(line):
                stripped = line.strip()
                return (
                    "MiniShell>" not in stripped and
                    "Welcome to MiniShell" not in stripped and
                    "Exiting MiniShell" not in stripped and
                    not stripped.startswith("/mnt/") and
                    not stripped.startswith("🖥")
                )

            cleaned_lines = list(filter(should_keep, lines))
            clean_output = "\n".join(cleaned_lines)
        else:
            clean_output = '✅ Command executed with redirection.'

        return jsonify({'output': clean_output, 'cwd': CURRENT_DIR})

    except Exception as e:
        return jsonify({'output': f'Error: {str(e)}'}), 500

if __name__ == '__main__':
    app.run(port=5000)
