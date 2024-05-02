from graphic import *
import tkinter as tk


class Game:
    def __init__(self, root: tk.Tk):
        self.size = int(input("Enter the size of the field "))
        self.graphic = Graphic(self.size, root)

    def play(self):
        self.graphic.step()


if __name__ == '__main__':
    root = tk.Tk()
    game = Game(root)
    game.play()
    root.mainloop()
