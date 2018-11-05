import intersect
import sys

class street(object):
    #define a street cases
    def __init__ (self, name, vertices):
        #define the name of a street
        self.name = name
        #define the vertices of a street
        self.vertices = vertices

class ParseException(Exception):
    #to raise an exception
    def __init__(self, value):
        self._value = value
    def __str__(self):
        return str(self._value)

def street_input_analysis(input):
    "Check if the command is a, c or r"
    command = input[0]
    input = input.split("(")
    #define the counter and empty string and list
    street_name = ""
    vertices = []
    c = 0
    if command != "r":
        # there has to be more than 1 "("
        if len(input) <= 1:
            raise ParseException("Please check the format of street coordinate")
        for element in input:
            #This is for street name only first iteration
            if c == 0:
                element = element.split('"')
                street_name = element[1]
                c = c + 1
            else:
            # This is for coordinates following iterations
                #build an empty list
                vertex = []
                #split each element by ,
                element = element.split(",")
                #The coordinate has to be sepearted by ,
                if len(element) != 2:
                    raise ParseException("Please check the format of street coordinate")
                else:
                    # There has to be something exsits at two sides of the , sign
                    if len(element[0]) >= 1 and len(element[1]) >= 1:
                        # Check if there is the ")"
                        if ")" not in element[1]:
                            raise ParseException("Please check the format of street coordinate")
                        else:
                            #remove ")" for easy data processing
                            element[1]=element[1].replace(")", "")
                            try:
                                #number conversion
                                number_x = int((element[0][:]))
                                number_y = int((element[1][:]))
                            except ValueError:
                                #ensure all input is number
                                raise ParseException("Please check the input of street coordinate")
                            #add the result into vertice list
                            vertex.append(int(number_x))
                            vertex.append(int(number_y))
                            vertices.append(vertex)
                    else:
                        raise ParseException("Please check the input of street coordinate")
    else:
        # This is for case "r"
        # The r command length has to be 1 if we seperated by "("
        if len(input) > 1:
            raise ParseException("You should only enter street name for remove command")
        elif input[0][-1] != '"' and input[0][-1] != ' ':
            # The r command has to end with '"'
            raise ParseException("The format of street name is wrong")
        else:
            input_infor = input[0].split('"')
            street_name = input_infor[1]
            #print(input[0])
    #update the street name
    street_name = str(street_name).upper()
    return street_name, vertices

def read_input(user_input, street_list):
    #Create an new input from user
    streetname = []
    for key, value in street_list.iteritems():
        #store the information into the empty street
        streetname.append(key)
    if len(user_input) < 1:
        raise ParseException("Ensure something is typed before click the enter button")
    else:
        if user_input[0] == "a":
            name, vertices = street_input_analysis(user_input)
            if index_in_list(streetname, name):
                raise ParseException("The street already exist, please add a different street")
            else:
                street1 = street(name, vertices)
                street_list[street1.name] = street1.vertices
                return street_list
        elif user_input[0] == "c":
            name, vertices = street_input_analysis(user_input)
            if not index_in_list(streetname, name):
                raise ParseException("The street you trying to update is not exist, please type the correct name")
            else:
                street1 = street(name, vertices)
                street_list[street1.name] = street1.vertices
                return street_list
        elif user_input[0] == "r":
            name, vertices = street_input_analysis(user_input)
            if not index_in_list(streetname, name):
                raise ParseException("The street you trying to remove is not exist, please type the correct name")
            else:
                street1 = street(name, vertices)
                street_list.pop(street1.name)
            return street_list
        else:
            raise ParseException("The command you typed is not exist, the correct command has to start with a, c, r or g")

def find_all_intersection(street_list):
    intersection_list = []
    tmp_street_list = street_list.copy()
    for key_main, value_main in street_list.iteritems():
        index_main = 0
        tmp_street_list.pop(key_main)
        for ponits_main in value_main:
            if index_main <= (len(value_main)-2):
                src = intersect.Point(value_main[index_main][0], value_main[index_main][1])
                dst = intersect.Point(value_main[index_main+1][0], value_main[index_main+1][1])
                index_main = index_main+1
                l1 = intersect.Line(src, dst)
                for key_sub, value_sub in tmp_street_list.iteritems():
                    index_sub = 0
                    for points_sub in value_sub:
                        if index_sub <= (len(value_sub) - 2):
                            src = intersect.Point(value_sub[index_sub][0], value_sub[index_sub][1])
                            dst = intersect.Point(value_sub[index_sub + 1][0], value_sub[index_sub + 1][1])
                            l2 = intersect.Line(src, dst)
                            intersect_point = intersect.intersect(l1,l2)
                            index_sub = index_sub + 1
                            if intersect_point != None and type(intersect_point) != list and (not check_exist_point(intersection_list,intersect_point)):
                                intersection_list.append((intersect_point.x,intersect_point.y))
                            elif intersect_point != None and type(intersect_point) == list:
                                for i in range(0, len(intersect_point)):
                                    tmp_point = intersect.Point(intersect_point[i][0],intersect_point[i][1])
                                    if not check_exist_point(intersection_list,tmp_point):
                                        intersection_list.append((tmp_point.x,tmp_point.y))
    return intersection_list

def point_check(point, line):
    x1, y1 = line.src.x, line.src.y
    x2, y2 = line.dst.x, line.dst.y
    if (x2-x1) != 0:
        a = (y2-y1)/(x2-x1)
        b = y1 - a * x1
        delta = a * point.x + b - point.y
    else:
        delta = point.x - x1
    delta = abs(round(delta,10))
    if delta == 0 and (point.x >= min(x1,x2) and point.x <= max(x1,x2)) and (point.y >= min(y1,y2) and point.y <= max(y1,y2)):
        return True
    else:
        return False

def check_exist_point(list, point):
    for element in list:
        if (float(element[0]) == float(point.x)) and (float(element[1]) == float(point.y)):
            return True
    return False


def two_point_equal(point1, point2):
    if point1[0] == point2[0] and point1[1] == point2[1]:
        return True
    return False

def index_in_list(list, index):
    for element in list:
        if element == index:
            return True
    return False

def index_in_dic(dic, element):
    for index, value in dic.iteritems():
        if value[0] == element[0] and value[1] == element[1]:
            return index
    return None

def cal_street_point(street_list,intersection_list):
    tmp_street_list = street_list.copy()
    street_point_list= {}
    index = 1
    for name, lines in tmp_street_list.iteritems():
        for i in range(0, len(lines) - 1):
            tmp_street_point_list = []
            count = 0
            src = intersect.Point(lines[i][0], lines[i][1])
            dst = intersect.Point(lines[i + 1][0], lines[i + 1][1])
            line = intersect.Line(src, dst)
            for point in intersection_list:
                intersect_point = intersect.Point(point[0],point[1])
                #print(intersect_point.x, intersect_point.y)
                if point_check(intersect_point, line):
                    if count == 0:
                        tmp_street_point_list.append((src.x,src.y))
                        tmp_street_point_list.append((dst.x,dst.y))
                        count = count + 1
                    if not check_exist_point(tmp_street_point_list, intersect_point):
                        tmp_street_point_list.append((intersect_point.x,intersect_point.y))
            if len(tmp_street_point_list)> 0:
                street_point_list[index] = tmp_street_point_list
                index = index + 1
    return street_point_list

def sort_street_point_list(street_point_list):
    tmp_street_point_list = street_point_list.copy()
    for name, lines in tmp_street_point_list.iteritems():
        new_lines = sort_point_list(lines)
        street_point_list[name]=new_lines
    return street_point_list

def sort_point_list(list):
    tmp_list_x = []
    tmp_list_y = []
    sort_list = []
    for element in list:
        tmp_list_x.append(element[0])
        tmp_list_y.append(element[1])

    length = len(tmp_list_x)
    if max(tmp_list_x) == min(tmp_list_x):
        for i in range(0, length):
            y_min = min(tmp_list_y)
            for element in list:
                if y_min == element[1]:
                    sort_list.append((element[0],element[1]))
                    tmp_list_y.remove(y_min)
    else:
        for i in range(0, length):
            x_min = min(tmp_list_x)
            for element in list:
                if x_min == element[0]:
                    sort_list.append((element[0],element[1]))
                    tmp_list_x.remove(x_min)
    return sort_list

def update_vertice_list(street_point_list, vertice_list):
    tmp_vertice_list = {}
    tmp_point_list = []
    index_list = []
    for index_street, line in street_point_list.iteritems():
        for point1 in line:
            for index, point2 in vertice_list.iteritems():
                tmp_point = intersect.Point(point1[0], point1[1])
                if two_point_equal(point1, point2):
                    if not check_exist_point(tmp_point_list, tmp_point):
                        tmp_vertice_list[index] = point1
                        tmp_point_list.append(point1)
                        index_list.append(index)

    tmp_index = 1
    for index_street, line in street_point_list.iteritems():
            for point1 in line:
                tmp_point = intersect.Point(point1[0], point1[1])
                if not check_exist_point(tmp_point_list, tmp_point):
                    while index_in_list(index_list, tmp_index):
                        tmp_index = tmp_index + 1
                    tmp_vertice_list[tmp_index] = point1
                    tmp_point_list.append(point1)
                    index_list.append(tmp_index)
    return tmp_vertice_list

def edge_list(street_point_list, vertice_list):
    edge_list = []
    for street_index, lines in street_point_list.iteritems():
        for i in range(0, len(lines)-1):
            index_1 = index_in_dic(vertice_list, lines[i])
            index_2 = index_in_dic(vertice_list, lines[i+1])
            edge = (index_1, index_2)
            if not check_exist_edge(edge_list, edge):
                edge_list.append(edge)
    return edge_list

def print_vertix_list(vertice_list):
    print("V = {")
    for index, value in vertice_list.iteritems():
        print("{}: ({},{})".format(index, value[0], value[1]))
    print("}")

def print_edge_list(edge_list):
    print( "E = {")
    i = 1;
    for value in edge_list:
        if i < len(edge_list):
            print("<{},{}>,".format(value[0], value[1]))
            i = i + 1;
        else:
            print("<{},{}>".format(value[0], value[1]))
            i = i + 1;
    print("}")

def dic_to_list(dic):
    list = []
    for key, value in dic.iteritems():
            list.append(value)
    return list

def check_exist_edge(list, edge):
    for element in list:
        if (element[0] == edge[0] and element[1] == edge[1]) or (element[1] == edge[0] and element[0] == edge[1]):
            return True
    return  False

def check_int(number):
    if float(int(number)) == number:
        return int(number)
    return number

def int_dic(dic):
    new_dic = {}
    for key, value in dic.iteritems():
        new_dic[key] = (check_int(value[0]),check_int(value[1]))
    return new_dic

##Following function is for Assignment 3
def rename_vertices(Vertices_list):
    tmp_vertices_list = {};
    index = 0;
    for key, value in Vertices_list.iteritems():
        tmp_vertices_list[index] = value
        index = index +1
    return tmp_vertices_list

def print_A3(Vertices_list, Edge_list):
    max_key = -1;
    for key, value in Vertices_list.iteritems():
        if max_key <= int(key):
            max_key = int(key)
    max_key = max_key +1
    output_string = "E {"
    index = 1;
    for value in Edge_list:
        if index < len(Edge_list):
            output_string = output_string + "<{},{}>,".format(value[0], value[1])
        else:
            output_string = output_string + "<{},{}>".format(value[0], value[1])
        index = index + 1
    output_string = output_string + "}\n"
    A3_out = "V "+ str(max_key) +"\n"+output_string;
    sys.stdout.write(A3_out);
    sys.stdout.flush()
    #print("V %d\n" %(length))

def print_edge_list_A3(Edge_list):
    output_string = "E {"
    index = 1;
    for value in Edge_list:
        if index < len(Edge_list):
            output_string = output_string + "<{},{}>,".format(value[0], value[1])
        else:
            output_string = output_string + "<{},{}>".format(value[0], value[1])
        index = index + 1
    output_string = output_string + "}\n"
    sys.stdout.write(output_string)
    sys.stdout.flush()
    #print (output_string)

























