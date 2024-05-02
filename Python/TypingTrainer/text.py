import random

from texts import english, russian, digits


class Text:
    @staticmethod
    def load_exercise_en():
        exercise = random.choice(english.Sentences)
        return exercise

    @staticmethod
    def load_exercise_ru():
        exercise = random.choice(russian.Sentences)
        return exercise

    @staticmethod
    def load_exercise_dig():
        exercise = random.choice(digits.Sentences)
        return exercise
