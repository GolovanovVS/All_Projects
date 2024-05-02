from graphic_editor import GraphicEditor
import tkinter as tk


class TypingTrainer:
    def __init__(self):
        root = tk.Tk()
        GraphicEditor(root)
        root.mainloop()
