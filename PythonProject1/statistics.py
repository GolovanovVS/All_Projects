import time


class Statistics:
    def __init__(self):
        self.start_time = time.time()
        self.total_errors = 0
        self.current_exercise_index = 0

    def start_exercise(self):
        self.start_time = time.time()

    def finish_exercise(self, exercise: int):
        end_time = time.time()
        typing_speed = exercise / (end_time - self.start_time) * 60
        result_text = f"Ошибок: {self.total_errors}, Скорость: {typing_speed:.2f} символов в минуту."
        return result_text
