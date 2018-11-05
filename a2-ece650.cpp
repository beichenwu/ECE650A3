#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <sstream>
#include <cstdlib>
using namespace std;


vector<vector<int> > Create_Node_list(int NumberofVertices, vector<vector<int> >& Edge_List){  //Define an array to store all node information
    vector<vector<int> > Vertex_Vector;
    for(int i = 0; i < NumberofVertices; i++){  //For Loop to create an array has size equal to number of Vertices
        vector<int> Vertex;                    //Set vector for each vertex
        Vertex.push_back(i);                   //Set the name for each Vertex
        Vertex.push_back(-1);                   //Set the parts for 0(For None)
        Vertex.push_back(0);                   //Set the Node Status(0 for searched and 1 for searched)
        Vertex_Vector.push_back(Vertex);       //Create the Vector
    }
    Edge_List.clear();                         //Reset Edge List
    return  Vertex_Vector;
}

int Read_Number_Of_Vertices(string User_Input){                //Define an function to analyze the user input and return number of vertices
    int Number_Of_vertices;                                    //Create a variable to store the number of vertices
    int String_Index;                                          //a variable to find where the space from the input
    int String_Length;                                         //a variable to store the length of string
    String_Index = User_Input.find(" ");                       //Locate the position of " "
    String_Length = User_Input.length();                       //The length of the string
    istringstream(User_Input.substr(String_Index+1, String_Length-String_Index)) >> Number_Of_vertices;     //Read the string and get the number of vertex
    return  Number_Of_vertices;
}


vector<string> String_Split(string String_Input, string Deliminator){           //A Function split a string and return a vector
    int String_Index = 0;                                                       //Initialized the String Index
    vector<string> Splited_String;                                              //Initialized the Vector
    string Sub_String;                                                          //Initialized the substring
    string String_For_Split = String_Input;                                     //Record the string for split
    while(String_Index != -1){                                                  //Stop the loop if deliminator is not found
        String_Index = String_For_Split.find(Deliminator);
        Sub_String = String_For_Split.substr(0,String_Index);                   //Find the substring
        Splited_String.push_back(Sub_String);                                   //Added the substring to vector
        String_For_Split.erase(0, String_Index+1);                              //Re-size the string
    }
    return Splited_String;
}

vector<vector<int> > Read_Edge_list(string User_Input, vector<vector<int> > VerticesVector){                  //Define an function to analyze the user input and return array of edge
    int String_Index1;
    int String_Index2;                                                                                      //a variable to find where the space from the input
    int Edge_Number;                                                                                        //a variable to store the number of edge
    int Node_Length = VerticesVector.size();
    string Edge_String = User_Input;                                                                        //Local String to store the user input
    vector<string> Tmp_Edge_List = String_Split(Edge_String, "<");                                          //Store the string in a temporary vector
    Edge_Number = Tmp_Edge_List.size();                                                                     //Determine the length of the edge list
    vector<vector<int> > Edge_List;                                                                         //Initialized Pointer for 2D Edge_list
    for(int i=0; i < Edge_Number; i++){
        if(i >=1) {                                                                                        //Loop Start from 1(Since index 0 is "E {"
            vector<int> Edge_Pair;
            String_Index1 = Tmp_Edge_List[i].find(",");                                                    //Substring to find the vertex number
            String_Index2 = Tmp_Edge_List[i].find(">");
            int tmp_int;
            istringstream(Tmp_Edge_List[i].substr(0, String_Index1)) >> tmp_int;
            Edge_Pair.push_back(tmp_int);
            istringstream(Tmp_Edge_List[i].substr(String_Index1+1, (String_Index2-String_Index1-1))) >> tmp_int;
            Edge_Pair.push_back(tmp_int);
            Edge_List.push_back(Edge_Pair);                                                                //A Function is needed to validate the edge list
        }
    }
    for(int i = 0; i < Edge_List.size(); i++){                                                             //Error Check
        if(Edge_List[i][0]> Node_Length || Edge_List[i][1]> Node_Length ){
            cerr << "Edge List contains nodes beyond the node list" << endl;
            break;
        }
    }
    return Edge_List;
}

vector<int> Find_Start_End(string User_Input){                                                             //Function to find Start End
    vector<int> Start_End_Vector;
    vector<string> Tmp_Edge_List = String_Split(User_Input, " ");                                          //Split String by ","
    int tmp_int;
    istringstream(Tmp_Edge_List[1]) >> tmp_int;
    Start_End_Vector.push_back(tmp_int);                                                                  //Push the first item
    istringstream(Tmp_Edge_List[2]) >> tmp_int;
    Start_End_Vector.push_back(tmp_int);                                                                  //Push the second item
    return  Start_End_Vector;
}

vector<int> Find_Connected_Vertex(int Input_Vertex, vector<vector<int> >& VerticesVector, vector<vector<int> > Edge_List){      //A Function to find the connected Vertex
    int Orgin = Input_Vertex;                                                                                                   //Assign the Input
    int Edge_list_Length = Edge_List.size();                                                                                    //Assign the length of Edge_list
    vector<int> Connected_Vertex;                                                                                               //Define the vector list for Connected Vertex
    VerticesVector = VerticesVector;                                                                                            //Assign the VerticesVector
    for(int i = 0; i < Edge_list_Length; i++) {                                                                                 //Bellman-Ford Search
        if (Orgin == Edge_List[i][0]) {                                                                                         //Check if input vertex match
            int Tmp_Connected_Vertex = Edge_List[i][1];                                                                         //Assign Connected the vertex
            if (VerticesVector[Tmp_Connected_Vertex][2] != 1) {                                                                 //Check if the vertex has been searched or not
                Connected_Vertex.push_back(Tmp_Connected_Vertex);                                                               //Push to the Vector
                VerticesVector[Tmp_Connected_Vertex][1] = Input_Vertex;                                                         //Set Parent
                VerticesVector[Tmp_Connected_Vertex][2] = 1;                                                                    //Searched
            }
        }
        if (Orgin == Edge_List[i][1]) {                                                                                         //Check if input vertex match
            int Tmp_Connected_Vertex = Edge_List[i][0];                                                                         //Same
            if (VerticesVector[Tmp_Connected_Vertex][2] != 1) {
                Connected_Vertex.push_back(Tmp_Connected_Vertex);
                VerticesVector[Tmp_Connected_Vertex][1] = Input_Vertex;
                VerticesVector[Tmp_Connected_Vertex][2] = 1;
            }
        }
    }
    VerticesVector[Orgin][2] = 1;
    return Connected_Vertex;
}

vector<int> Shortest_Path(int Start, int End, vector<vector<int> > &VerticesVector, vector<vector<int> > Edge_List ){
    int Start_Vertex =  Start;                                                           //Start Vertex
    int End_Vertex = End;                                                                //End Vertex
    int Current_Vertex;                                                                  //empty variable to store being search vertex
    int Count = 0;                                                                       //Count
    //VerticesVector = VerticesVector;                                                     //Empty Vector
    vector<int> Tmp_Vertex_Vector;
    queue<int> Vertex_Queue;
    Vertex_Queue.push(Start_Vertex);
    vector<int> Short_Path;
    for(int i = 0; i< VerticesVector.size();i++){                                        //Initialized the VerticesVector
        VerticesVector[i][1] = -1;
        VerticesVector[i][2] = 0;
    }

    do{
        Current_Vertex = Vertex_Queue.front();                                                            //store the vector into the queue
        Tmp_Vertex_Vector = Find_Connected_Vertex(Current_Vertex, VerticesVector, Edge_List);             //Find the connected vector
        Vertex_Queue.pop();                                                                               //Pop the vertex been searched
        for (int i = 0; i < Tmp_Vertex_Vector.size(); i++) {
            Vertex_Queue.push(Tmp_Vertex_Vector[i]);                                                      //Push the searched vertices
        }
        Count++;
    }while(Current_Vertex != End_Vertex && Count < VerticesVector.size() && Vertex_Queue.size() != 0);           //stop if the end vertex been searched or Queue been cleaned

    int Parent_Name = End;
    while(Parent_Name != -1){                                                                      //Check if the End vertex has parent
        Short_Path.push_back(Parent_Name);                                                         //Loop and push parents
        Parent_Name = VerticesVector[Parent_Name][1];
    }
        return Short_Path;
}

string Return_Short_Path(vector<int> Short_Path){
    string Shortest_Path;
    int Count = 0;
    while(Short_Path.size()!=0){                                                                 //For Loop and add the string
        if (Count != 0){                                                                         //Case if not the first vertex
            string tmp_string;
            tmp_string = static_cast<ostringstream*>( &(ostringstream() << Short_Path.back()) )->str();
            Shortest_Path = Shortest_Path + "-" + tmp_string;
            Short_Path.pop_back();
        }else{
            string tmp_string;
            tmp_string = static_cast<ostringstream*>( &(ostringstream() << Short_Path.back()) )->str();
            Shortest_Path = Shortest_Path + tmp_string;                        //Case if for the first vertex
            Short_Path.pop_back();
        }
        Count++;
    }
    return Shortest_Path;
}

bool Check_Node_Within_Range(int node, int size){                //Check if the node is within the input node range
    if(node >= size) {
        return false;
    }
    else{
        return true;
    }
}

int main(int argc, char** argv) {
    string User_Input;                                             //Assign an empty string to read user input
    vector<vector<int> > VerticesVector;                           //Assign an pointer refer to vertices array
    vector<vector<int> > Edge_List;                                //Assign an empty vector to store the edge list
    while(true){                                                  //Main loop of the file

        getline(cin, User_Input);                                 //Take system standard input
        if(User_Input == ""){                                      //Exit the program if user input empty string
            break;
        }
        else if(User_Input.substr(0,1) == "V"){                    //For the case the user input vertices
            int Number_Of_Vertices;                                //Create a variable to store the number of vertices
            Number_Of_Vertices = Read_Number_Of_Vertices(User_Input);
            VerticesVector = Create_Node_list(Number_Of_Vertices, Edge_List);                     //Create the node list
            cout << User_Input << endl;

        }
        else if(User_Input.substr(0,1) == "E"){                    //For the case the user input edge set
            Edge_List = Read_Edge_list(User_Input, VerticesVector);
            cout << User_Input << endl;
        }
        else if(User_Input.substr(0,1) == "s"){                    //For the case the user input to calculate the shortest path
            vector<int> Start_End = Find_Start_End(User_Input);
            int Start_Vertex = Start_End[0];
            int End_Vertex = Start_End[1];


            if((!Check_Node_Within_Range(Start_Vertex, VerticesVector.size())) || (!Check_Node_Within_Range(End_Vertex, VerticesVector.size()))){
                cerr << "The Start and End Vertex are out of range " << endl;
            }
            else {
                vector<int> Path = Shortest_Path(Start_Vertex,End_Vertex,VerticesVector,Edge_List);
                if (Path.size() == 1) {                                 //Error Check for if there is path exist or not
                    cerr << "There is no path exist between these pair of vertices" << endl;
                }
                else{
                    string Path_Output = Return_Short_Path(Path);
                    cout<<Path_Output<<endl;
                }
            }
        }
        else {                                             //This should return an error to user for wrong command
        }
    }

    //cout << "Finish reading input" << endl;
    return 0;
    exit (EXIT_FAILURE);
}