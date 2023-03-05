#Zeichnet alle Buslinien zwischen den Koordinaten der Bushaltestellen
import turtle
from time import sleep
from tkinter import *


screen = turtle.getscreen()
t = turtle.Turtle(visible=False)
t.speed(10)

f = open("buslineDescription.txt", "r")
outputPath = str(f.readline().rstrip('\n'))

#Einlesen der Buslinien
lines = int(f.readline())
print(str(lines) + " lines to draw!")

for i in range(lines):
    if i%100 == 1:
        print(str(i-1)+" lines completed!")
    coordinates = f.readline().split(" ")
    #Verwenden der Dokumentation auf https://www.python-online.ch/turtle.php?inhalt_links=turtle/navigation.inc.php&inhalt_mitte=turtle/turtledoc.html
    x1 = int(coordinates[0])*5
    y1 = int(coordinates[1])*5
    x2 = int(coordinates[2])*5
    y2 = int(coordinates[3])*5

    t.penup()
    t.goto(x1, y1)
    t.pendown()
    t.setheading(t.towards(x2, y2))
    t.forward(t.distance(x2, y2))

print("Done")
screen.getcanvas().postscript(file=outputPath)
#sleep(5)
