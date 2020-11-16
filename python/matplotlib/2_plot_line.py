import matplotlib.pyplot as plt
import numpy as np

t = np.arange(0., 5., 0.2) 
'''
plot(*args, **kwargs)
 - args , 主要是讲X, Y，和线条的颜色+风格
 - kwargs, 就是线条属性，e.g.color, lw(or linewidth), lable, ls(linestyle) etc....
label	            string or anything printable with ‘%s’ conversion.
linestyle or ls	    [‘solid’ | ‘dashed’, ‘dashdot’, ‘dotted’ | (offset, on-off-dash-seq) | '-' | '--' | '-.' | ':' | 'None' | ' ' | '']
linewidth or lw	    float value in points
marker	            A valid marker style
'''
lines = plt.plot(t, t, 'r-', t, t**3, 'cp', color='y') #后面附属的线条属性会覆盖前面每条线的简单属性
#lines[0].set_antialiased(False)
print(lines)
plt.show()

# 另一种设置线条属性的方法 
# lines 就是 plot()返回的对象列表，这里有2个值, 可以分别对不同的线条设置属性
lines = plt.plot(t, t, 'r-', t, t**3, 'cp', color='y')
lines[0].set_antialiased(False)
lines[0].set_linewidth(2.0)
lines[1].set_color('r')
plt.show()

# 第3种，是调用 plot的 setp（）,可以对单条线设置，也可以对所有线条一起设置
lines = plt.plot(t, t, 'r-', t, t**3, 'cp', color='y')
#plt.setp(lines, color='r', linewidth=2.0)  ## 全部变为红
plt.setp(lines[0], color='r', linewidth=4.0)  ## 全部变为红
plt.show()
print(plt.setp(lines))   ## 打印所有属性， e.g.

'''所有打印的线条属性

 agg_filter: a filter function, which takes a (m, n, 3) float array and a dpi value, and returns a (m, n, 3) array
  alpha: float or None
  animated: bool
  antialiased or aa: bool
  clip_box: `.Bbox`
  clip_on: bool
  clip_path: Patch or (Path, Transform) or None
  color or c: color
  contains: unknown
  dash_capstyle: {'butt', 'round', 'projecting'}
  dash_joinstyle: {'miter', 'round', 'bevel'}
  dashes: sequence of floats (on/off ink in points) or (None, None)
  data: (2, N) array or two 1D arrays
  drawstyle or ds: {'default', 'steps', 'steps-pre', 'steps-mid', 'steps-post'}, default: 'default'
  figure: `.Figure`
  fillstyle: {'full', 'left', 'right', 'bottom', 'top', 'none'}
  gid: str
  in_layout: bool
  label: object
  linestyle or ls: {'-', '--', '-.', ':', '', (offset, on-off-seq), ...}
  linewidth or lw: float
  marker: marker style string, `~.path.Path` or `~.markers.MarkerStyle`
  markeredgecolor or mec: color
  markeredgewidth or mew: float
  markerfacecolor or mfc: color
  markerfacecoloralt or mfcalt: color
  markersize or ms: float
  markevery: None or int or (int, int) or slice or List[int] or float or (float, float) or List[bool]
  path_effects: `.AbstractPathEffect`
  picker: unknown
  pickradius: float
  rasterized: bool or None
  sketch_params: (scale: float, length: float, randomness: float)
  snap: bool or None
  solid_capstyle: {'butt', 'round', 'projecting'}
  solid_joinstyle: {'miter', 'round', 'bevel'}
  transform: `matplotlib.transforms.Transform`
  url: str
  visible: bool
  xdata: 1D array
  ydata: 1D array
  zorder: float
'''