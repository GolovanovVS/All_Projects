from character import *
import tkinter as tk
import random
import time
import sys


class Cell:
    def __init__(self, x: int, y: int):
        self.x = x
        self.y = y
        self.state = State.default
        self.taken = False
        self.visited = False


import tkinter as tk
import sys
import time


class Start(Cell):
    def __init__(self, x: int, y: int):
        super().__init__(x, y)
        self.state = State.start
        self.taken = True

    def action(self, main: Character, root: tk.Tk, cord: float):
        window = tk.Label(root, text="Do you want to exit?\n y or n")
        window.place(relx=cord, rely=0.3, anchor="center")

        def check(event):
            if event.char not in ["y", "n"]:
                return
            window.unbind("<Key>")
            if event.char == "y":
                if main.exit:
                    window.config(text=f"Congratulations! You found the exit! Your money is {main.money}")
                    window.update()
                    root.after(2000, root.destroy)
                    return
                else:
                    window.config(text="Sorry, you should find treasures first!")
            else:
                window.config(text="Have a nice adventure!")
            window.update()
            root.after(2000, window.destroy)

        window.bind("<Key>", check)
        window.focus_set()


class Things(Cell):
    def __init__(self, x: int, y: int):
        super().__init__(x, y)
        self.state = State.things
        self.taken = False
        a = random.randint(1, 100)
        if a <= 15:
            self.thing = Weapon.brass_knuckles
        elif a <= 35:
            self.thing = Weapon.knife
        elif a <= 55:
            self.thing = Weapon.sword
        elif a <= 90:
            self.thing = Weapon.heal
        else:
            self.thing = Weapon.tank

    def action(self, main: Character, root: tk.Tk, cord: float):
        window = tk.Label(root, text="")
        if not self.taken:
            window.config(text="You are in the village ")
            window.place(relx=cord, rely=0.3, anchor="center")
        if self.taken:
            window.config(text="You have already taken all things ")
            window.place(relx=cord, rely=0.3, anchor="center")
            time.sleep(1.3)
            window.destroy()
            return
        string = ""
        answer = ""
        time.sleep(1)
        if self.thing == Weapon.fists:
            string = "fists"
            answer = "Do you wanna bring {}? y or n ".format(string)

        elif self.thing == Weapon.brass_knuckles:
            string = "brass knuckles"
            answer = "Do you wanna bring {}? y or n ".format(string)

        elif self.thing == Weapon.knife:
            string = "knife"
            answer = "Do you wanna bring {}? y or n ".format(string)

        elif self.thing == Weapon.sword:
            string = "sword"
            answer = "Do you wanna bring {}? y or n ".format(string)

        elif self.thing == Weapon.tank:
            string = "tank"
            answer = "Do you wanna bring {}? y or n ".format(string)
        else:
            answer = "Do you wanna heal 30? y or n "
        window.config(text=answer)
        window.update()
        time.sleep(1)
        self.taped = False

        def check(event):
            if event.char != "y" and event.char != "n":
                return
            window.unbind("<Key>")
            self.taped = True
            if event.char == "y":
                self.taken = True
                if self.thing != Weapon.heal:
                    main.weapon = self.thing
                    window.config(text="You have taken {} with you ".format(string))
                else:
                    main.health += 30
                    window.config(text="You have now {} heal ".format(main.health))
            else:
                window.config(text="You did not take any things( ")
            window.update()
            root.after(2000, window.destroy)

        window.bind("<Key>", check)
        window.focus_set()


class Monster(Cell):
    def __init__(self, x: int, y: int):
        super().__init__(x, y)
        self.state = State.monster
        self.health = random.randint(2, 10)
        self.atk = random.randint(3, 10)
        self.award = self.health + self.atk * 3
        self.taken = False

    def action(self, main: Character, root: tk.Tk, cord: float):
        window = tk.Label(root, text="")
        window.place(relx=cord, rely=0.3, anchor="center")

        if not self.taken:
            window.config(text="Monsters you have to fight ")
        if self.taken:
            window.config(text="You have already fought this monster ")
            window.update()
            time.sleep(1.3)
            window.destroy()
            return
        self.taken = True
        damage = 0
        if main.weapon == Weapon.fists:
            damage = 1
        elif main.weapon == Weapon.brass_knuckles:
            damage = 3
        elif main.weapon == Weapon.knife:
            damage = 5
        elif main.weapon == Weapon.sword:
            damage = 7
        elif main.weapon == Weapon.tank:
            damage = 100
        while self.health > 0 and main.health > 0:
            answer = "Your health is " + str(max(main.health, 0)) + " monster health is " + str(self.health)
            window.config(text=answer)
            window.update()
            main.health -= self.atk
            self.health -= damage
            time.sleep(1.3)
        window.config(text="Your health is " + str(max(main.health, 0)) + " monster health is " + str(self.health))
        window.update()
        time.sleep(1.3)
        if main.health <= 0:
            print("GAME OVER")
            window.config(text="GAME OVER")
            root.after(2000, root.destroy)
        else:
            main.money += self.award
            window.config(text="You have won! You have " + str(main.money) + " coins")
            window.update()
            time.sleep(1.3)
            window.destroy()


class Trap(Cell):
    def __init__(self, x: int, y: int):
        super().__init__(x, y)
        self.state = State.trap
        self.damage = random.randint(5, 20)
        self.award = random.randint(-15, -5)
        self.taken = False

    def action(self, main: Character, root: tk.Tk, cord: float):
        window = tk.Label(root, text="")
        window.place(relx=cord, rely=0.3, anchor="center")
        if not self.taken:
            window.config(text="You are in traps ")
            window.update()
            time.sleep(1.3)
        if self.taken:
            window.config(text="you have already hit the trap ")
            window.update()
            time.sleep(1.3)
            window.destroy()
            return
        self.taken = True
        main.health -= self.damage
        window.config(text="You lost {} health, your current health is {} ".format(self.damage, main.health))
        window.update()
        time.sleep(1.3)
        if main.health <= 0:
            window.config(text="GAME OVER")
            root.after(2000, root.destroy())
            return
        window.destroy()


class Treasures(Cell):
    def __init__(self, x: int, y: int):
        super().__init__(x, y)
        self.taken = False
        self.state = State.treasures

    def action(self, main: Character, root: tk.Tk, cord: float):
        window = tk.Label(root, text="")
        window.place(relx=cord, rely=0.3, anchor="center")
        if self.taken:
            window.config(text="you have already take the treasure ")
            window.update()
            time.sleep(1.3)
            window.destroy()
            return
        window.config(text="You find treasures, now you can get out ")
        window.update()
        if not self.taken:
            self.taken = True
            main.exit = True
        time.sleep(1.3)
        window.destroy()
