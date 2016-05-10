from flask import Flask
import robot

app = Flask(__name__)

@app.route('/')
def index():
    return 'Welcome'

@app.route('/forward')
def forward():
    robot.f()
    return 'forward'

@app.route('/back')
def back():
    robot.b()
    return 'backward'

@app.route('/stop')
def stop():
    robot.stop()
    return 'stop'

if __name__ == '__main__':
    app.run(host='0.0.0.0')
