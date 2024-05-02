from random import randint
from character import Character
from cell import *


class Field:
    def __init__(self, size):
        self.size = size
        self.main = Character(self.size)
        example = [Cell(0, 0) for _ in range(2 * size + 1)]
        self.matrix = [example.copy() for _ in range(2 * size + 1)]
        for i in range(2 * size + 1):
            for j in range(2 * size + 1):
                if i == j == size:
                    self.matrix[i][j] = Start(i, j)
                    continue
                a = random.randint(1, 100)
                if a <= 40:
                    self.matrix[i][j] = Things(i, j)
                elif a <= 70:
                    self.matrix[i][j] = Monster(i, j)
                else:
                    self.matrix[i][j] = Trap(i, j)
        x = random.randint(0, 2 * size)
        y = random.randint(0, 2 * size)
        while x == y == size:
            x = random.randint(0, 2 * size)
            y = random.randint(0, 2 * size)
        self.matrix[x][y] = Treasures(x, y)
        self.x = x
        self.y = y

    def move(self, direction):
        main = self.main
        lin = ""
        if direction == 'w':
            main.walk(0, -1)
            if main.y < 0:
                main.y = 0
                lin = "There is a wall, you cannot move there\n"
            return lin + "You are in {} , {}".format(main.x, main.y)
        elif direction == 's':
            main.walk(0, 1)
            if main.y > 2 * self.size:
                main.y = 2 * self.size
                lin = "There is a wall, you cannot move there\n"
            return lin + "You are in {} , {}".format(main.x, main.y)

        elif direction == 'a':
            main.walk(-1, 0)
            if main.x < 0:
                main.x = 0
                lin = "There is a wall, you cannot move there\n"
            return lin + "You are in {} , {}".format(main.x, main.y)

        elif direction == 'd':
            main.walk(1, 0)
            if main.x > 2 * self.size:
                main.x = 2 * self.size
                lin = "There is a wall, you cannot move there\n"
            return lin + "You are in {} , {}".format(main.x, main.y)

        else:
            return "Choose a valid direction "
