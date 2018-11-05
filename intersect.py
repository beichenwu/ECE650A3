import support_functions

class Point(object):
    def __init__ (self, x, y):
        self.x = float(x)
        self.y = float(y)
    def __str__ (self):
        return '(' + str(self.x) + ',' + str(self.y) + ')'

class Line(object):
    def __init__ (self, src, dst):
        self.src = src
        self.dst = dst

    def __str__(self):
        return str(self.src) + '-->' + str(self.dst)

def intersect (l1, l2):
    x1, y1 = l1.src.x, l1.src.y
    x2, y2 = l1.dst.x, l1.dst.y
    x3, y3 = l2.src.x, l2.src.y
    x4, y4 = l2.dst.x, l2.dst.y
    point1 = Point(x1, y1)
    point2 = Point(x2, y2)
    point3 = Point(x3, y3)
    point4 = Point(x4, y4)

    xden = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))
    yden = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)
    xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4))
    ynum = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)
    if xden != 0 and yden != 0:
        xcoor =  xnum / xden
        ycoor = ynum / yden
        if (xcoor <= max(x1,x2) and xcoor >= min(x1,x2)) and (ycoor <= max(y1,y2) and ycoor >= min(y1,y2)):
            if (xcoor <= max(x3, x4) and xcoor >= min(x3, x4)) and (ycoor <= max(y3, y4) and ycoor >= min(y3, y4)):
                return Point (xcoor, ycoor)
    elif (xden == 0 and yden == 0) and (support_functions.point_check(point3,l1) or support_functions.point_check(point4,l1)):
        point_list= []
        if support_functions.point_check(point3,l1):
            point_list.append((point3.x, point3.y))
        if support_functions.point_check(point4,l1):
            point_list.append((point4.x, point4.y))
        return point_list
    elif (xden == 0 and yden == 0) and (support_functions.point_check(point1,l2) or support_functions.point_check(point2,l2)):
        point_list= []
        if support_functions.point_check(point1,l2):
            point_list.append((point1.x, point1.y))
        if support_functions.point_check(point2,l2):
            point_list.append((point2.x, point2.y))
        return point_list
    else:
        return None

if __name__ == '__main__':
    p1 = Point (0, 0)
    p2 = Point (5, 0)
    p3 = Point (1, 0)
    p4 = Point (6, 0)

    l1 = Line (p1, p2)
    l2 = Line (p3, p4)
    print('Intersect of', l1, 'with', l2, 'is', intersect(l1, l2))
