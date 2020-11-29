#!/usr/bin/env python3

import tkinter as tk
from tkinter import N,S,E,W

kl_qwerty = (
    ('Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'),
    ('A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L'),
    ('Z', 'X', 'C', 'V', 'B', 'N', 'M')
)

class VKeyboard(tk.Frame):
    """The VKeyboard is a Tk frame for a virtual keyboard."""
    def __init__(self, root = None, bg_color : str = "#63686e", key_color : str = "#4a4d4f", text_color : str = "#fafafa"):
        super().__init__(root, bg=bg_color)
        self.master = root
        self.suggestions = [tk.StringVar() for i in range(4)]
        self.grid(column=0, row=0, sticky=(N,S,E,W), padx=5, pady=3)
        self.bg_color = bg_color
        self.key_color = key_color
        self.text_color = text_color
        self._create_widgets()

    def _draw_keyboard(canvas : tk.Canvas, *, layout : tuple = kl_qwerty,
                       key_color : str = 'white',
                       text_color : str = 'black'):
        _size = (int(canvas['width']), int(canvas['height']))
        _max_letters = max([len(row) for row in layout])
        _padding = int(_size[1] * 0.03) # 5% padding
        _key_varea = _size[1] // (len(layout) + 1)
        _key_harea = min(_key_varea, _size[0] // _max_letters)
        _left_offset = _size[0] - _key_harea * _max_letters
        assert _left_offset >= 0, "_left_offset cannot be negative"

        for y in range(len(layout)):
            _row_offset = int((_max_letters - len(layout[y])) * 0.3 * _key_harea)
            for x in range(len(layout[y])):
                _key_coords = \
                    (_left_offset + _row_offset + _key_harea * x, _key_varea * y)
                canvas.create_rectangle(
                    _key_coords[0] + _padding, _key_coords[1] + _padding,
                    _key_coords[0] + _key_harea - _padding,
                    _key_coords[1] + _key_varea - _padding,
                    fill=key_color, tags=('key', 'K_' + layout[y][x])
                )
                canvas.create_text(
                    _left_offset + _row_offset + int(_key_harea * (x + 0.5)),
                    int(_key_varea * (y + 0.5)), text=layout[y][x],
                    fill=text_color, justify='center', anchor='center',
                    tags=('key_text', 'K_' + layout[y][x])
                )
        canvas.create_rectangle(
            (_size[0] - 6 * _key_harea) // 2 + _padding,
            _key_varea * len(layout) + _padding,
            (_size[0] + 6 * _key_harea) // 2 - _padding,
            _size[1] - _padding,
            fill=key_color, tags=('K_SPACE')
        )

    def clear_box(self):
        self.text.delete(0, tk.END)

    def copy_to_clipboard(self):
        self.clipboard_clear()
        self.clipboard_append(self.text.get())

    def _create_widgets(self):
        self.text_frame = tk.Frame(self, bg=self.bg_color)
        self.text = tk.Entry(self.text_frame)
        self.copy_img = tk.PhotoImage(file='rcs/round_content_copy_white_18dp.png')
        self.clear_img = tk.PhotoImage(file='rcs/round_clear_white_18dp.png')
        self.copy = tk.Button(self.text_frame, image=self.copy_img, bg=self.key_color)
        self.copy['command'] = self.copy_to_clipboard
        self.clear = tk.Button(self.text_frame, image=self.clear_img, bg=self.key_color)
        self.clear['command'] = self.clear_box

        self.suggest_frame = tk.Frame(self, bg=self.bg_color)
        self.suggest_text = [tk.Label(self.suggest_frame, textvariable=s) for s in self.suggestions]

        self.canvas = tk.Canvas(self, width=850, height=378, bg=self.bg_color)

        self.text_frame.grid(column=0, row=0, sticky=(N,S,E,W), padx=5, pady=5)
        self.text.grid(column=0, row=0, rowspan=2, sticky=(N,S,E,W), padx=5, pady=5)
        self.copy.grid(column=1, row=0, padx=5, pady=5)
        self.clear.grid(column=1, row=1, padx=5, pady=5)
        self.suggest_frame.grid(column=0, row=1, sticky=(E,W))
        for i in range(len(self.suggest_text)):
            self.suggest_text[i].grid(column=i, row=0, sticky=(E,W), pady=3)
        self.canvas.grid(column=0, row=2, padx=55, pady=3)
        self.text_frame.columnconfigure(0, weight=1)
        self.columnconfigure(0, weight=1)

        VKeyboard._draw_keyboard(self.canvas, key_color=self.key_color, text_color=self.text_color)


if __name__ == "__main__":
    root = tk.Tk()
    root.title("Keyboard Swiping")
    app = VKeyboard(root)
    app.mainloop()
