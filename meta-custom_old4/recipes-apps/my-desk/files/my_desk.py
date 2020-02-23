#!/usr/bin/env python3

import gi

gi.require_version('Gtk', '3.0')

from gi.repository import Gtk, Gdk, GdkPixbuf

class GridWindow(Gtk.Window):

    def __init__(self):
        Gtk.Window.__init__(self, title="STM32 Desktop")
        self.maximize()

        grid = Gtk.Grid()
        self.add(grid)

        # create some buttons
        button1 = Gtk.Button(label="QT Lcd Test")
        pb = GdkPixbuf.Pixbuf.new_from_file_at_size('/usr/local/img/img001.png', 100, 100)
        img = Gtk.Image()
        img.set_from_pixbuf(pb)
        button1.set_image(img)
        button1.set_image_position(Gtk.PositionType.TOP)
        button1.set_always_show_image (True)

        grid.add(button1)
 
        grid.set_margin_top(25)
        grid.set_margin_left(25)


win = GridWindow()
win.connect("delete-event", Gtk.main_quit)
win.show_all()
Gtk.main()
