import matplotlib.pyplot as plt
import numpy as np

t = np.arange(0., 5., 0.2)       # (start, stop, step), i.e. 0,0.2,0.4 ....4.8 excluding 5
'''
plot(*args, **kwargs)
args is a variable length argument, allowing for multiple x, y pairs with an optional format string
i.e. (x, y, [format]), (x, y, [format])
1) if only one given, it's y, i.e. (y) is valid, or (y, format)

'''
plt.plot(t, t, 'ro', t, t**2, 'b|', t, t**3, 'g1')
plt.title("np.arange")
plt.show()

t = np.linspace(0., 5.0, 25)       # (start, stop, num), interval is calculated
plt.plot(t, t, t, t**2,'cp', t, t**3)
plt.title("np.linspace")
plt.show()

"""
format string:  consists of color + line-style
=============
======line style=======
character	description
'-'	    solid line style
'--'	dashed line style
'-.'	dash-dot line style
':'	dotted line style
'.'	point marker
','	pixel marker
'o'	circle marker
'v'	triangle_down marker
'^'	triangle_up marker
'<'	triangle_left marker
'>'	triangle_right marker
'1'	tri_down marker
'2'	tri_up marker
'3'	tri_left marker
'4'	tri_right marker
's'	square marker
'p'	pentagon marker
'*'	star marker
'h'	hexagon1 marker
'H'	hexagon2 marker
'+'	plus marker
'x'	x marker
'D'	diamond marker
'd'	thin_diamond marker
'|'	vline marker
'_'	hline marker

The following color abbreviations are supported:
character	color
‘b’	blue
‘g’	green
‘r’	red
‘c’	cyan
‘m’	magenta
‘y’	yellow
‘k’	black
‘w’	white
"""