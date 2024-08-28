import json


class Config:
    def __init__(self, path):
        self.configDir = path
        self.config = None
        self.load(path)

    def load(self, path):
        try:
            with open(path, "r") as file:
                self.config = json.load(file)
        except Exception as e:
            print(e)
            self.config = {}
            self.save(path)

    def get(self, key):
        return self.config[key]

    def set(self, key, value):
        self.config[key] = value
        self.save(self.configDir)

    def save(self, path):
        with open(path + "/config.json", "w") as file:
            json.dump(self.config, file)