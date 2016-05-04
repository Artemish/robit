from flask import render_template, jsonify
from app import app
import robot

response = {
    'id': 1,
    'title': u'Buy groceries',
    'description': u'Milk, Cheese, Pizza, Fruit, Tylenol', 
    'done': False
}

@app.route('/')
@app.route('/index')
def index():
    user = {'nickname': 'Miguel'}  # fake user
    return render_template('index.html',
                           title='Home',
                           user=user)

@app.route('/forward',methods=['GET'])
def forward():
    robot.f()
    return jsonify({'response': response})

@app.route('/back',methods=['GET'])
def back():
    robot.b()
    return jsonify({'response': response})

@app.route('/stop',methods=['GET'])
def stop():
    robot.s()
    return jsonify({'response': response})
