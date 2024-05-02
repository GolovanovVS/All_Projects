import tkinter as tk
from tkinter import font

from statistics import Statistics
from text import Te xt


class GraphicEditor:
    def __init__(self, root):
        self.exercise = ""
        self.text = Text()

        self.custom_font = font.Font(family="Helvetica", size=20, weight="bold")
        self.custom_font_not_bold = font.Font(family="Helvetica", size=20)

        self.root = root
        self.root.title("Typing Trainer")
        self.stat = Statistics()
        self.start_interface()

    def start_interface(self):
        for widget in self.root.winfo_children():
            widget.destroy()

        label = tk.Label(self.root, text="Choose an exercise", font=("Helvetica", 30))
        screen_width = self.root.winfo_screenwidth()
        screen_height = self.root.winfo_screenheight()

        window_width = int(screen_width / 2)
        window_height = int(screen_height / 2)

        label.place(x=int(window_width / 2), y=int(window_height / 3), anchor="center")

        x = int((screen_width / 2) - (window_width / 2))
        y = int((screen_height / 2) - (window_height / 2))

        self.root.geometry(f"{window_width}x{window_height}+{x}+{y}")
        self.add_buttons()

    def add_buttons(self):
        button_en = tk.Button(self.root, text="English", command=lambda: self.load_exercise("en"), width=20, height=3)
        button_ru = tk.Button(self.root, text="Russian", command=lambda: self.load_exercise("ru"), width=20, height=3)
        button_dig = tk.Button(self.root, text="Digits", command=lambda: self.load_exercise("dig"), width=20, height=3)

        vert = int(self.root.winfo_screenheight() / 4)
        horiz = int(self.root.winfo_screenwidth() / 4)

        button_en.place(y=vert, x=(horiz / 2), anchor="center")
        button_ru.place(y=vert, x=horiz, anchor="center")
        button_dig.place(y=vert, x=horiz + (horiz / 2), anchor="center")

    def load_exercise(self, line: str):
        if line == "en":
            self.exercise = self.text.load_exercise_en()

        if line == "ru":
            self.exercise = self.text.load_exercise_ru()

        if line == "dig":
            self.exercise = self.text.load_exercise_dig()

        for widget in self.root.winfo_children():
            widget.destroy()

        self.exercise_interface()
        self.text_label.config(text=self.exercise, width=200, height=10, font=self.custom_font)
        self.entry_var.set("")

        self.stat.start_exercise()

    def exercise_interface(self):
        self.text_label = tk.Label(self.root, text="")
        self.text_label.pack()

        self.entry_var = tk.StringVar()
        self.entry_var.trace("w", self.on_type)

        self.entry = tk.Entry(self.root, textvariable=self.entry_var, font=self.custom_font_not_bold)
        self.entry.pack()
        self.entry.focus_set()

        self.result_label = tk.Label(self.root, text="")
        self.result_label.pack()

    def on_type(self, *args):
        current_text = self.entry_var.get()

        if not self.exercise.startswith(current_text):
            self.stat.total_errors += 1
            self.entry_var.set(current_text[:-1])

        if self.exercise == current_text:
            result_text = self.stat.finish_exercise(len(self.exercise))
            self.result_label.config(text=result_text)
            self.text_label.config(text="Упражнения завершены!")
            self.entry.config(state="disabled")
            screen_width = self.root.winfo_screenwidth()
            screen_height = self.root.winfo_screenheight()

            button = tk.Button(self.root, text="Retry", command=self.start_interface, width=20, height=5)
            button.place(x=int(screen_width / 4), y=int(screen_height / 2) - 50, anchor="center")
