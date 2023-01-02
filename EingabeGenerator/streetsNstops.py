#Zeichnet die Strassen einer Stadt und markiert die Haltestellen rot
import turtle

def drawStreet(x, y, d, s):
    t.up()
    t.goto(x*s, y*s)
    t.down()
    t.left(d)
    t.forward(s)
    t.left(360-d)

screen = turtle.getscreen()
t = turtle.Turtle()
t.hideturtle()
t.speed(10)

f = open("test", "r")
#Einlesen der Strassen
streets = int(f.readline())
print(str(streets) + " streets to draw!")
for i in range(streets):
    if i%100 == 1:
        print(str(i-1)+" streets completed!")
    coordinates = f.readline().split(" ")
    drawStreet(int(coordinates[0]), int(coordinates[1]), int(coordinates[2]), 5)

#Einlesen der Haltestellen
t.pencolor("red")
stops = int(f.readline())
print(str(stops)+" stops to draw!")
for i in range(stops):
    coordinates = f.readline().split()
    t.up()
    t.goto(int(coordinates[0])*5, int(coordinates[1])*5)
    t.down()
    t.dot(8)

a = input()