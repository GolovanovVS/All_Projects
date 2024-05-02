import tkinter as tk
from PIL import Image, ImageTk

from cell import *
from character import *
from static import State
from field import Field
import time


class Graphic:
    def __init__(self, size: int, root: tk.Tk):
        self.field = Field(size)
        self.size = size

        self.root = root
        self.root.title('Text Game')

        self.dist = tk.Label(self.root, text="Distance till treasures {}".format(
            abs(self.field.main.x - self.field.x) + abs(self.field.main.y - self.field.y)))
        self.dist.place(relx=1.0, rely=0.0, anchor='ne')

        screen_width = self.root.winfo_screenwidth()
        screen_height = self.root.winfo_screenheight()
        window_width = int(screen_width / 2)
        window_height = int(screen_height / 2)
        width = window_width - window_height
        self.cord = ((window_width - width / 2) / window_width)

        self.text = tk.Label(self.root, text="Welcome to the club, buddy")
        self.text.place(relx=((window_width - width / 2) / window_width), rely=0.2, anchor='center')

        x = int((screen_width / 2) - (window_width / 2))
        y = int((screen_height / 2) - (window_height / 2))

        self.root.geometry(f'{window_width}x{window_height}+{x}+{y}')

        tup = (int((window_height - 20) / (2 * size + 1)), int((window_height - 20) / (2 * size + 1)))
        tup_small = (int(tup[0] / 3), int(tup[1] / 3))
        self.image_stone = ImageTk.PhotoImage(Image.open('pictures/stone.jpg').resize(tup))
        self.image_start = ImageTk.PhotoImage(Image.open('pictures/start.jpg').resize(tup))
        self.image_trap = ImageTk.PhotoImage(Image.open('pictures/trap.jpg').resize(tup))
        self.image_open = ImageTk.PhotoImage(Image.open('pictures/open_treasure.jpg').resize(tup))
        self.image_monster = ImageTk.PhotoImage(Image.open('pictures/monster.jpg').resize(tup))
        self.image_key = ImageTk.PhotoImage(Image.open('pictures/key.jpg').resize(tup))
        self.image_eliminated = ImageTk.PhotoImage(Image.open('pictures/eliminated_monster.jpg').resize(tup))
        self.image_close = ImageTk.PhotoImage(Image.open('pictures/close-treasure.jpg').resize(tup))
        self.image_chel = ImageTk.PhotoImage(Image.open('pictures/chel.jpg').resize(tup_small))

        self.label = [[0 for _ in range(2 * size + 1)] for _ in range(2 * size + 1)]
        for row in range((2 * size) + 1):
            for col in range((2 * size) + 1):
                if row == col == size:
                    self.label[row][col] = tk.Label(self.root, image=self.image_start)
                else:
                    self.label[row][col] = tk.Label(self.root, image=self.image_stone)
                self.label[row][col].grid(row=row, column=col)

        x_center = self.label[self.field.main.x][self.field.main.y].winfo_x() + self.label[self.field.main.x][
            self.field.main.y].winfo_width() / 2
        y_center = self.label[self.field.main.x][self.field.main.y].winfo_y() + self.label[self.field.main.x][
            self.field.main.y].winfo_height() / 2
        self.here = tk.Label(self.root, image=self.image_chel)
        self.here.place(x=x_center, y=y_center, anchor='center')
        self.here.update()

    def draw_cell(self, cell: Cell):
        # print(cell.x, cell.y)
        if cell.state == State.start:
            return
        if cell.state == State.things:
            if cell.visited == False:
                cell.visited = True
                self.label[cell.y][cell.x].config(image=self.image_close)
            return
        if cell.state == State.monster:
            if cell.visited == False:
                cell.visited = True
                self.label[cell.y][cell.x].config(image=self.image_monster)
            return
        if cell.state == State.trap:
            if cell.visited == False:
                cell.visited = True
                self.label[cell.y][cell.x].config(image=self.image_trap)
            return
        if cell.state == State.treasures:
            if cell.visited == False:
                cell.visited = True
                self.label[self.field.y][self.field.x].config(image=self.image_key)
            return

    def draw_main(self):
        x_center = self.label[self.field.main.x][self.field.main.y].winfo_x() + self.label[self.field.main.x][
            self.field.main.y].winfo_width() / 2
        y_center = self.label[self.field.main.x][self.field.main.y].winfo_y() + self.label[self.field.main.x][
            self.field.main.y].winfo_height() / 2
        self.here.place(x=y_center, y=x_center, anchor='center')
        self.here.update()

    def step(self):
        self.text.config(text="Choose the direction")
        self.text.update()
        time.sleep(1.3)
        self.root.bind("<Left>", self.move)
        self.root.bind("<Right>", self.move)
        self.root.bind("<Up>", self.move)
        self.root.bind("<Down>", self.move)
        self.root.bind("<a>", self.move)
        self.root.bind("<d>", self.move)
        self.root.bind("<w>", self.move)
        self.root.bind("<s>", self.move)

    def move(self, event):
        line = self.field.move(event.char)
        self.text.config(text=line)
        self.text.update()
        self.dist.config(text="Distance till treasures {}".format(
            abs(self.field.main.x - self.field.x) + abs(self.field.main.y - self.field.y)))
        self.dist.update()
        self.draw_main()
        time.sleep(1.3)
        if line.startswith("There") is True:
            return
        self.text.config(text="")
        self.text.update()
        self.draw_cell(self.field.matrix[self.field.main.x][self.field.main.y])
        self.root.update()
        self.field.matrix[self.field.main.x][self.field.main.y].action(self.field.main, self.root, self.cord)
