#Zeichnet die Quadrate der Stadt
#Nimmt in der Eingabedatei eine Anzahl von linken unteren Koordinaten entgegen
import turtle

def drawSquare(x, y, s):
    t.up()
    t.goto(x*s, y*s)
    t.down()
    for i in range(4):
        t.forward(s)
        t.left(90)

screen = turtle.getscreen()
t = turtle.Turtle()
t.hideturtle()
t.speed(10)

f = open("test", "r")
squares = int(f.readline())
print(str(squares) + " squares to draw!")
for i in range(squares):
    if i%100 == 1:
        print(str(i-1)+" squares completed!")
    coordinates = f.readline().split(" ")
    drawSquare(int(coordinates[0]), int(coordinates[1]), 5)

a = input()