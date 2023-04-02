import tkinter as tk
from tkinter.tix import COLUMN

class Application(tk.Frame):
    def __init__(self, master=None):
        tk.Frame.__init__(self, master)
        self.pack()
        master.title("Mpeol-CID")
        master.iconbitmap("cid.ico")
        # master.attributes("-toolwindow", 1) # 去掉窗口最大化最小化按钮，只保留关闭
        # master.overrideredirect(1) # 隐藏标题栏 最大化最小化按钮
        master.geometry('300x186')
        master.resizable(False,False)
        self.createWidgets()

    def createWidgets(self):
        '''
        self.hi_there = tk.Button(self)
        self.hi_there["text"] = "Hello World\n(click me)"
        self.hi_there["command"] = self.say_hi
        self.hi_there.pack(side="top")

        self.QUIT = tk.Button(self, text="QUIT", fg="red",
                                            command=root.destroy)
        self.QUIT.pack(side="bottom")
        '''
        ft = ('roman', 12, 'normal')
        self.tips = tk.Label(self.master,font=ft , text="The Computer ID:", anchor='w').place(x=5, y=5, width=290, height=48)
        self.cid = tk.Entry(self.master)
        self.cid.place(x=5, y=58, width=290, height=70)
        self.cid.insert("aaa")
        self.copy = tk.Button(self.master, font=ft, text="Copy the CID to Clipboard").place(x=5, y=133, width=290, height=48)





    def say_hi(self):
        print("hi there, everyone!")

root = tk.Tk()
app = Application(master=root)
app.mainloop()
