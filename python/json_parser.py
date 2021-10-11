import sys
from PyQt5.QtWidgets import QApplication, QWidget, QLineEdit

class App(QWidget):
    def __init__(self):
        super().__init__()
        self.title = 'Pyqt5 window'
        self.left = 50
        self.top = 50
        self.width = 300
        self.height = 300
        self.initUI()

    def initUI(self):
        self.setWindowTitle(self.title)
        self.setGeometry(self.left, self.top, self.width, self.height)
        self.textbox = QLineEdit(self)
        self.textbox.move(80, 100)
        self.textbox.resize(150,40)
        self.show()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = App()
    sys.exit(app.exec_())

