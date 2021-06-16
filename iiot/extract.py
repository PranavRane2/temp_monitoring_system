import pymysql # Using the pymysql library to connect python with the locally hosted phpmyadmin database
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation as fa
  
time = []
temp = []
tyme = []

# Define a function to extract and plot data from database to my python progam
def ext(i):

    #Create a connection token to the database
    mydb = pymysql.connect(host="localhost", user="vidyutrv", password="password", database="temp_monitoring")
    
    #Creates an object cursor that lets you execute SQL queries
    mycursor = mydb.cursor()
    mycursor.execute("SELECT temperature, reading_time FROM sensorlog ORDER BY reading_time DESC LIMIT 1;  ")
    result = mycursor.fetchall    
    
    #Data exracted is separated and formatted for plotting
    for i in mycursor:
        temp.append(i[0])
        time = i[1]
    tyme.append(time.strftime("%H:%M:%S"))
    
    print(tyme, "\t ", temp )
    
    # Plot the data
    plt.cla()
    plt.xlabel("Time (min)")
    plt.xticks(rotation=30, ha="center")
    plt.ylabel("Temp (°C)")
    plt.title("Temperature Monitoring")

    plt.plot(tyme, temp)

#yse the funcanimation function to live-plot the data
ani = fa(plt.gcf(), ext, interval = 500)

plt.show()