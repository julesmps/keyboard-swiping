#!/usr/bin/env python3

"""keyboard.py: A python module for a virtual keyboard which supports swiping"""
__author__ = "Juliana Pacheco"
__status__ = "Development"

import tkinter as tk
from tkinter import N,S,E,W
import threading, queue
import subprocess
from subprocess import PIPE

kl_qwerty = (
    ('Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'),
    ('A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L'),
    ('Z', 'X', 'C', 'V', 'B', 'N', 'M')
)
BUILD_PATH = "bazel-bin/"

def dist_square(r1 : tuple, r2 : tuple) -> int:
    assert len(r1) == len(r2), "r1 and r2 must be the same size"
    return sum([(i2 - i1) ** 2 for (i1,i2) in zip(r1,r2)])

def init_keymap(layout : tuple) -> dict:
    map = {}
    for row in layout:
        for key in row:
            map[f'K_{key}'] = key
    return map

def get_first_keycode(strings : tuple) -> str:
    for s in strings:
        if s.startswith('K_'):
            return s
    return ""

g_keyswipe = queue.SimpleQueue()
g_suggestions = queue.SimpleQueue()

def write_changes(sb : subprocess.Popen):
    prev_letters = []
    while sb.poll() is None:
        try:
            letters = g_keyswipe.get(timeout=1)
            if letters != prev_letters:
                sb.stdin.write(f'{len(letters)}\n')
                for letter in letters:
                    sb.stdin.write(f'{letter}\n')
                sb.stdin.flush()
                prev_letters = letters

        except Exception as _:
            pass

def read_changes(sb : subprocess.Popen):
    rcv = []
    target_length = 4
    while sb.poll() is None:
        rcv.append(sb.stdout.readline())
        if len(rcv) >= target_length:
            g_suggestions.put_nowait(tuple(rcv))
            rcv = []

class CanvasKeyboard(tk.Canvas):
    """CanvasKeyboard used by VKeyboard."""
    def __init__(self, root = None, *, layout : tuple = kl_qwerty, key_color : str = 'white', text_color : str = 'black', swipe_color : str = 'blue', delta : int = 5, **keyargs):
        super().__init__(root, keyargs)
        self.master = root
        self.key_color = key_color
        self.text_color = text_color
        self.swipe_color = swipe_color
        self.prev_coords = None
        self.in_movement = False
        self.delta = delta
        self.delta_square = delta ** 2
        self.keymap = init_keymap(layout)
        self.keymap['K_SPACE'] = ' '
        self._draw_keyboard(layout=layout)
        self._create_bindings()
        # must be evaluated after _draw_keyboard is executed
        self.key_delta = min(self.key_width, self.key_height) // 2

    def _get_letters(self, x : int, y : int) -> list:
        _keys = self.find_overlapping(x - self.key_delta,
                                      y - self.key_delta,
                                      x + self.key_delta,
                                      y + self.key_delta
        )
        _letters = []
        for id in _keys:
            tags = self.gettags(id)
            keycode = get_first_keycode(tags)
            if 'key' in tags and keycode:
                _letters.append(self.keymap[keycode])
        return _letters

    def _swiping(self, event : tk.Event):
        if self.prev_coords == None:
            self.prev_coords = (event.x, event.y)
        elif dist_square((event.x, event.y), self.prev_coords) > self.delta_square:
            self.in_movement = True
            self.create_line(self.prev_coords[0], self.prev_coords[1], event.x, event.y, width=3, fill=self.swipe_color, tags=('swipe'))
            self.prev_coords = (event.x, event.y)
            g_keyswipe.put_nowait(self._get_letters(*self.prev_coords))
        else:
            return # step too small --- ignore

    def _release(self, event : tk.Event):
        self.prev_coords = None
        if self.in_movement:
            self.delete('swipe')
            self.in_movement = False
            g_keyswipe.put_nowait([])
        else:
            _match = self.find_overlapping(event.x - self.delta,
                                           event.y - self.delta,
                                           event.x + self.delta,
                                           event.y + self.delta
            )
            if _match:
                for tag in self.gettags(_match[0]):
                    if tag in self.keymap:
                        self.master.write(self.keymap[tag])

    def _create_bindings(self):
        self.bind('<Button1-Motion>', lambda e: self._swiping(e))
        self.bind('<ButtonRelease-1>', lambda e: self._release(e))

    def _draw_keyboard(self, *, layout : tuple, padding : float = 0.03, space_size : int = 6):
        _size = (int(self['width']), int(self['height']))
        _max_letters = max([len(row) for row in layout])
        _padding = int(_size[1] * padding)
        _key_varea = _size[1] // (len(layout) + 1)
        _key_harea = min(_key_varea, _size[0] // _max_letters)
        self.key_height = _key_varea - 2 * _padding
        self.key_width = _key_harea - 2 * _padding
        _left_offset = _size[0] - _key_harea * _max_letters
        assert _left_offset >= 0, "_left_offset cannot be negative"

        for y in range(len(layout)):
            _row_offset = int((_max_letters - len(layout[y])) * 0.3 * _key_harea)
            for x in range(len(layout[y])):
                _key_coords = \
                    (_left_offset + _row_offset + _key_harea * x, _key_varea * y)
                self.create_rectangle(
                    _key_coords[0] + _padding, _key_coords[1] + _padding,
                    _key_coords[0] + _key_harea - _padding,
                    _key_coords[1] + _key_varea - _padding,
                    fill=self.key_color, tags=('key', 'K_' + layout[y][x])
                )
                self.create_text(
                    _left_offset + _row_offset + int(_key_harea * (x + 0.5)),
                    int(_key_varea * (y + 0.5)), text=layout[y][x],
                    fill=self.text_color, justify='center', anchor='center',
                    tags=('key_text', 'K_' + layout[y][x])
                )
        _space_offset = int((_max_letters - space_size) * 0.4 * _key_harea)
        self.create_rectangle(
            _left_offset + _space_offset + _padding,
            _key_varea * len(layout) + _padding,
            _left_offset + _space_offset + space_size * _key_harea - _padding,
            _size[1] - _padding,
            fill=self.key_color, tags=('K_SPACE')
        )

class VKeyboard(tk.Frame):
    """The VKeyboard is a Tk frame for a virtual keyboard."""
    def __init__(self, root = None, *, bg_color : str = "#63686e",
                                       key_color : str = "#4a4d4f",
                                       text_color : str = "#fafafa"):
        super().__init__(root, bg=bg_color)
        self.master = root
        self.suggestions = [tk.StringVar() for i in range(4)]
        self.grid(column=0, row=0, sticky=(N,S,E,W), padx=5, pady=3)
        self.bg_color = bg_color
        self.key_color = key_color
        self.text_color = text_color
        self._create_widgets()
        self._create_bindings()

        self.master.after(1000, self._update_suggestions)

    def write(self, msg : str):
        self.text.insert(tk.END, msg)

    def clear_box(self):
        self.text.delete(0, tk.END)

    def copy_to_clipboard(self):
        self.clipboard_clear()
        self.clipboard_append(self.text.get())

    def _create_bindings(self):
        for i in range(len(self.suggestions)):
            self.suggest_text[i].bind('<Button-1><ButtonRelease-1>',
                lambda e, i=i: self.write(self.suggestions[i].get()[0:-1] + ' '))

    def _update_suggestions(self):
        try:
            if(not g_suggestions.empty()):
                new_suggestions = g_suggestions.get(timeout=1)
                for i in range(len(self.suggestions)):
                    self.suggestions[i].set(new_suggestions[i])
        except Exception as _:
            pass
        self.master.after(200, self._update_suggestions)

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
        self.suggest_text = [
            tk.Label(self.suggest_frame, textvariable=s, bg=self.bg_color, fg=self.text_color)
                for s in self.suggestions
        ]

        self.canvas = CanvasKeyboard(self, width=850, height=378,
            bg=self.bg_color, key_color=self.key_color, text_color=self.text_color)

        self.text_frame.grid(column=0, row=0, sticky=(N,S,E,W), padx=5, pady=5)
        self.text.grid(column=0, row=0, rowspan=2, sticky=(N,S,E,W), padx=5, pady=5)
        self.copy.grid(column=1, row=0, padx=5, pady=5)
        self.clear.grid(column=1, row=1, padx=5, pady=5)
        self.suggest_frame.grid(column=0, row=1, sticky=(E,W))
        for i in range(len(self.suggest_text)):
            self.suggest_text[i].grid(column=i, row=0, sticky=(E,W), pady=3)
            self.suggest_frame.columnconfigure(i, weight=1)
        self.canvas.grid(column=0, row=2, padx=55, pady=3)
        self.text_frame.columnconfigure(0, weight=1)
        self.columnconfigure(0, weight=1)


if __name__ == "__main__":
    prediction = subprocess.Popen([f'./{BUILD_PATH}swipe'], stdin=PIPE, stdout=PIPE, encoding='UTF-8')
    root = tk.Tk()
    root.title("Keyboard Swiping")
    app = VKeyboard(root)

    # Wait for acknowledge/ready token before handing over control to
    #   IO and UI threads / routines
    prediction.stdout.readline()

    send_changes = threading.Thread(target=write_changes, args=(prediction,))
    rcv_changes = threading.Thread(target=read_changes, args=(prediction,))
    send_changes.start()
    rcv_changes.start()
    app.mainloop()

    prediction.stdin.write("-1\n")
    prediction.stdin.flush()
    prediction.wait()
    rcv_changes.join()
    send_changes.join()
