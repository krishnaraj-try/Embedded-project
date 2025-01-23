//main.py import os import glob  
import serial  # import serial pacakge import sys  # import system package import 
urllib3 import urllib.request from time import sleep import mysql.connector  
arduino = serial.Serial('COM5', 9600, timeout=.1) count = 0 while True:  
data = arduino.readline()[:-2]  # the last bit gets rid of the new-line chars    
data:  
data = data.decode('utf-8')         
data_v = data_v.split(' ')         
data_v = str(data)         
t = data_v[0]         
r = data_v[3]        
 l = data_v[4]        
 la=data_v[5]  
 if 
print(data_v)         
h = data_v[1]         
s = data_v[2]         
conn = mysql.connector.connect(user='root', password='', host='localhost', 
database='1trpiot')         
iotdata  
cursor = conn.cursor()         
cursor.execute("insert into 
values('','"+t+"','"+h+"','"+s+"','"+r+"','"+l+"','"+la+"')")         
conn.commit()         
conn.close() //app.py from flask import Flask, render_template, flash, 
request,session  
#from wtforms import Form, TextField, TextAreaField, validators, StringField, 
SubmitField  
from werkzeug.utils import secure_filename import mysql.connector import 
tkinter as tk from tkinter import * import datetime import time import yagmail 
app 
= 
Flask(__name__) 
app.config.from_object(__name__) 
app.config['SECRET_KEY'] = '7d441f27d441f27567d441f2b6176a' @app.route("/") def homepage():     conn = mysql.connector.connect(user='root', 
password='', host='localhost', database='1trpiot')    
 cursor = conn.cursor()  
cursor.execute("SELECT * FROM iotdata ORDER BY id DESC LIMIT 6")     
data=cursor.fetchall()  
return 
render_template('index.html',data=data) 
@app.route("/email",methods=['GET','POST']) def view11():     
request.method == 'POST':         
request.form['lvar']  
email = request.form['email']         
if 
lvar = 
conn = mysql.connector.connect(user='root', password='', host='localhost', 
database='1trpiot')        
 cursor = conn.cursor()  
cursor.execute("SELECT * FROM iotdata ORDER BY id DESC LIMIT  
1")         
data = cursor.fetchone()         
password = 'rddwmbynfcbgpywf';         
= [email]        
 body =  
mail = 'testsam360@gmail.com';         
# list of email_id to send the mail         
li 
"X:"+str(data[0])+",Y:"+str(data[1])+",Distance:"+str(data[2])+",Switch:"+str(
 d ata[3])+",Lat-Lan  
Values:"+str(data[4])+","+str(data[5])+",link=https://www.google.com/maps/se
 a rch/?api=1&query="+data[4]+","+data[5]         
password)  
yag = yagmail.SMTP(mail, 
yag.send(to=email, subject="Alert...!", contents=body)  
conn = mysql.connector.connect(user='root', password='', host='localhost', 
database='1trpiot')        
 cursor = conn.cursor()  
cursor.execute("SELECT * FROM iotdata ORDER BY id DESC LIMIT 6")         
data1 = cursor.fetchall()         
return 
render_template("index.html",data=data1) if __name__ == '__main__':     
app.run(debug=True, use_reloader=True) 