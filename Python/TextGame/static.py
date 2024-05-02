import enum


class State(enum.Enum):
    default = 0
    start = 1
    things = 2
    monster = 3
    trap = 4
    treasures = 10000


class Weapon(enum.Enum):
    fists = 1
    brass_knuckles = 3
    knife = 5
    sword = 7
    tank = 100
    heal = 30
