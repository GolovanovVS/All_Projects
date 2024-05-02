from static import *


class Character:
    def __init__(self, size):
        self.health = 80
        self.weapon = Weapon.fists
        self.x = size
        self.y = size
        self.money = 0
        self.exit = False

    def walk(self, x, y):
        self.x += x
        self.y += y

    def distance(self, x, y):
        print("Distance to the target is", abs(self.cor[0] - x) + abs(self.cor[1] - y), "rooms")
