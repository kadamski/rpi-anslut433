#!/usr/bin/python
from flask import Flask, request, render_template, redirect, url_for
import subprocess

app = Flask(__name__)

units = (
    ('Speaker', 0),
    ('Something', 1),
    ('Light', 2),
    ('All', 3),
)

@app.route('/')
def index():
    button = request.args.get('button', None)
    value = request.args.get('value', None)
    if button != None and value != None:
        subprocess.call(['../test', '-s', '-u', button, '10101010101010101010101010', value])
        return redirect(url_for('index'))
    else:
        return render_template('index.html', units=units)

if __name__ == '__main__':
    app.debug=True
    app.run(host='0.0.0.0')
