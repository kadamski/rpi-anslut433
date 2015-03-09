#!/usr/bin/python
from flask import Flask, request, render_template, redirect, url_for
import subprocess

app = Flask(__name__)

units = (
    (0x2aaaaaa, (
     ('Speaker', 0),
     ('Something', 1),
     ('Light', 2),
     ('All', 3),
    )),
    (0x1bbbbbb, (
     ('Test', 0),
    )),
)

def _rc_code(val):
    try:
        val = int(val)
        if val > 0x3ffffff:
            raise ValueError
    except (ValueError, TypeError):
        return None

    ret = str(bin(val))[2:]
    return '0' * (26 - len(ret)) + ret

@app.route('/')
def index():
    button = request.args.get('button', None)
    value = request.args.get('value', None)
    code = _rc_code(request.args.get('code', None))

    if button != None and value != None and code != None:
        subprocess.call(['../test', '-s', '-u', button, code, value])
        return redirect(url_for('index'))
    else:
        return render_template('index.html', units=units)

if __name__ == '__main__':
    app.debug=True
    app.run(host='0.0.0.0')
