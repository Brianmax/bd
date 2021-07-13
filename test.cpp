#include <bits/stdc++.h>
#include <direct.h>
#include <iostream>
#include <fstream> 
#include<sstream>
using namespace std;

const string tablesDirectoryName = "Tablas";
const string metadata = "_metadata";
const string filesFormat = ".txt";
const char globalSplitter = ',';

string remove_spaces(string str)
{
    int tam = str.size();
    string tmp;
    for(int i = 0;i<tam;i++)
    {
        if(str[i]==' ')
        {
            continue;
        }
        tmp+=str[i];
    }   
    return tmp;
}
string remove_character(string str, char remChar)
{
    int tam = str.size();
    string tmp;
    for(int i = 0;i<tam;i++)
    {
        if(str[i]==remChar)
        {
            continue;
        }
        tmp+=str[i];
    }   
    return tmp;
}
string replace_character(string str, char oldchar, char newchar)
{
    for(auto &i:str)
        i = i==oldchar? newchar : i;
    return str;
}

enum operations{crear=0,insertar=1,seleccionar=2,borrar=3,modificar=4};

vector<string> processQuery(string query,int operation)
{
    query = remove_character(query,'\'');//borrar comillas
    string queryProcessed = query;

    //poner todo en minusculas
    for_each(queryProcessed.begin(),queryProcessed.end(),[](char &c)
    {
        c=::tolower(c);
    });
    

    vector<string> result;

    vector<vector<string>> operationMatrix = 
    {
        {"crear tabla","(",")"},
        {"insertar","(",")"},
        {"select * desde","donde"},
        {"borrar desde","donde"},
        {"modificar","set","=","donde"}
    };

    size_t found1,found2,found3,found4; 

    //todos obtienen el nombre de la tabla de la misma manera
    found1 = queryProcessed.find(operationMatrix[operation][1]);
    int tmp = operationMatrix[operation][0].size();
    result.push_back(query.substr(tmp+1,found1-(tmp+1)-1)); //tableName 
    //para crear e insertar
    if(operation == crear || operation == insertar)
    {   
        query.pop_back();
        result.push_back(query.substr(found1+1));//TableCols
    }
    //para seleccionar y borrar
    else if(operation == seleccionar || operation == borrar)
    {
        found2 = queryProcessed.find_first_of("=<>");
        int tmp1 = operationMatrix[operation][1].size();
        result.push_back(query.substr(found1+tmp1+1,found2-(found1+tmp1+1)-1));//filtercol
        result.push_back(query.substr(found2,1));//condition
        result.push_back(query.substr(found2+2));//filterData
    } 
    //para modificar
    else if(operation==modificar)
    {
        found2 = queryProcessed.find("="); 
        found3 = queryProcessed.find(operationMatrix[operation][3]);
        found4 = queryProcessed.find_last_of("=><");
        int tmp1 = operationMatrix[operation][1].size();
        int tmp2 = operationMatrix[operation][3].size();    
        result.push_back(query.substr(found1+tmp1+1,found2-(found1+tmp1+1)-1));//setcol
        result.push_back(query.substr(found2+2,found3-(found2+2)-1));//setdata
        result.push_back(query.substr(found3+tmp2+1,found4-(found3+tmp2+1)-1));//filter 
        result.push_back(query.substr(found4,1));
        result.push_back(query.substr(found4+2));
    }

    return result; 
}

vector<string> split_Character(string s,char c)
{   
    vector<string> result;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(c)) != std::string::npos) {
        token = s.substr(0, pos);
        result.push_back(token);
        s.erase(0, pos + 1);
    }
    result.push_back(s);
    return result;
}

vector<int> getNeededSpace(vector<string> fields)
{   
    vector<int> result;
    for(auto i : fields){
        if(i.find("int")!=std::string::npos || i.find("char")!=std::string::npos)
        {
            result.push_back(stoi(i.substr(i.find("(")+1,i.find(")")-1)));
        }
        else if(i.find("date")!=std::string::npos)
        {
            result.push_back(10);
        }
    }
    return result;
}

void crearTabla(string query)
{   
    vector<string> fields = processQuery(query,0);
    mkdir(tablesDirectoryName.c_str());
    mkdir(string(tablesDirectoryName+"/"+fields[0]).c_str());

    fstream file;
    file.open(string(tablesDirectoryName+"/"+fields[0]+"/"+fields[0]+filesFormat).c_str(),std::fstream::out );
    
    // "Crear Tabla Estudiante (ID_est-int(12), nom-char(30), ape-char(30), edad-int(12), fecnac-date)"
    vector<string> colNames = split_Character(fields[1],',');
    string tmp_cols;
    for(auto i:colNames)
    {
        i = i.substr(0,i.find("-"));
        tmp_cols+=remove_spaces(i)+',';
    }
    tmp_cols.pop_back();
    tmp_cols+=';';
    file<<tmp_cols;

    file.close();
    file.open(string(tablesDirectoryName+"/"+fields[0]+"/"+fields[0]+metadata+filesFormat).c_str(),std::fstream::out);
    file<<fields[0]<<';';
    file<<remove_spaces(fields[1])<<';';
    
    vector<int> tmp = getNeededSpace(split_Character(fields[1],','));
    int sum = 0;
    tmp_cols.clear();
    for(auto i:tmp)
    {
        sum+=i;
        tmp_cols+=to_string(i)+',';
    }
    tmp_cols.pop_back();
    tmp_cols+=';';
    file<<tmp_cols;
    file.close();
}

//Obtiene cuantos bytes debe ocupor cada columna
vector<int> getSpacePerCol(string tableName)
{
    fstream file;
    file.open(string(tablesDirectoryName+"/"+tableName+"/"+tableName+metadata+filesFormat).c_str(),std::fstream::in);

    string line;
    string file_contents;
    while (std::getline(file, line))
    {
        file_contents += line;
    } 
    vector<string> tmp_string = split_Character(file_contents,';');
    tmp_string = split_Character(tmp_string[2],',');
    vector<int> tmp_int;
    for(auto i:tmp_string)
    {
        tmp_int.push_back(stoi(i));
    }
    return tmp_int;
}

void insert(string query)
{   
    //obtenemos el nombre del la tabla y las columnas
    vector<string> fields = processQuery(query,1);
    fields[1] = remove_spaces(fields[1]);
    //abrimos el archivo
    fstream file;
    file.open(string(tablesDirectoryName+"/"+fields[0]+"/"+fields[0]+filesFormat).c_str(),fstream::app);

    //vector con los espacios disponibles
    vector<int> colsSpaces = getSpacePerCol(fields[0]);
    //vector con los datos
    vector<string> colsData = split_Character(fields[1],',');
  
    colsData.pop_back();

    string registerTmp; 

    //calculamos cuanto espacio extra hay y lo agregamos por cada col
    size_t tam = colsData.size();
    for(size_t i = 0; i< tam+1; i++)
    {
        int tmp = colsSpaces[i] - colsData[i].size();
        registerTmp+=colsData[i];
        for(int j= 0; j<tmp;j++)
        {
            registerTmp+=" ";
        }
        registerTmp+=",";
    }
    registerTmp.pop_back();
    registerTmp.push_back(';');
    
    file<<registerTmp;
    file.close();
}

bool operationsCondition(string a,string b, string condition)
{
    bool result;
    if (condition=="=")
    {
        result= a == b; 
    }
    else if(condition == "<")
    {
        result= stoi(a) < stoi(b);
    }
    else if(condition == ">")
    {
        result= stoi(a) > stoi(b);
    }
    return result;
}

void select(string query)
{
    vector<string> fields = processQuery(query,2);
    ifstream file;
    file.open(string(tablesDirectoryName+"/"+fields[0]+"/"+fields[0]+filesFormat).c_str());
    string tablefields;
    getline(file,tablefields,';');

    cout<<tablefields<<endl;
    int tmp = tablefields.find(fields[1]);
    int nField = 0;
    for(int i = 0; i <tmp;i++)
    {
        if(tablefields[i]==',')
            nField++;
    }
    string line;
    //lee linea por linea, delimitando por ; de modo que line obtiene un registro tratable
    while(getline(file,line,';'))
    {   
        line = remove_spaces(line);
        vector<string> registerTmp = split_Character(line,',');

        if (operationsCondition(registerTmp[nField],fields[3],fields[2]))
            cout<<line<<endl;
    }
}
void del(string query)
{
    vector<string> fields = processQuery(query,3);
    ifstream file;

    file.open(string(tablesDirectoryName+"/"+fields[0]+"/"+fields[0]+filesFormat).c_str());
    string tablefields;
    getline(file,tablefields,';');

    int tmp = tablefields.find(fields[1]);
    int nField = 0;
    for(int i = 0; i <tmp;i++)
    {
        if(tablefields[i]==',')
            nField++;
    }
    string line;
    ofstream file_tmp;
    file_tmp.open(string(tablesDirectoryName+"/"+fields[0]+"/"+"tmp"+filesFormat).c_str());
    file_tmp<<tablefields<<";";

    int deletedLines = 0;
    //lee linea por linea, delimitando por ; de modo que line obtiene un registro tratable
    while(getline(file,line,';'))
    {   
        vector<string> registerTmp = split_Character(line,',');

        if (!operationsCondition(remove_spaces(registerTmp[nField]),fields[3],fields[2])){
            file_tmp<<line<<";";
        }
        else{
            deletedLines++;
        }
    }
    cout<<"Lineas Borradas = "<<deletedLines<<endl;
    file.close();
    file_tmp.close();
    remove(string(tablesDirectoryName+"/"+fields[0]+"/"+fields[0]+filesFormat).c_str());
    rename(string(tablesDirectoryName+"/"+fields[0]+"/"+"tmp"+filesFormat).c_str(),string(tablesDirectoryName+"/"+fields[0]+"/"+fields[0]+filesFormat).c_str());
}



int main()
{
    string query1 = "Crear Tabla Estudiante (ID_est-int(12), nom-char(30), ape-char(30), edad-int(12), fecnac-date)";
    // crearTabla(query1);
    string query2 = "INSERTAR Estudiante (2, 'Renzo','paredes', 21, '16/02/1998')";
    query2 = "INSERTAR Estudiante (2, 'Renzo','paredes', 21, '16/02/1998')";
    insert(query2);
    query2 = "INSERTAR Estudiante (3, 'Renzo','paredes', 21, '16/02/1998')";
    insert(query2);
    query2 = "INSERTAR Estudiante (4, 'Renzo','paredes', 21, '16/02/1998')";
    insert(query2);
    
    // query2 = "INSERTAR Estudiante (2, 'Renzo','paredes', 22, '16/02/1998')";
    // // insert(query2);
    // // query2 = "INSERTAR Estudiante (3, 'Renzo','paredes', 22, '16/02/1998')";
    // // insert(query2);
    // query2 = "INSERTAR Estudiante (4, 'Renzo','paredes', 27, '16/02/1998')";
    // // insert(query2);

    // string query3 = "SELECT * DESDE Estudiante DONDE ID_est > 0";
    // // select(query3);
    // string query4 = "BORRAR DESDE Estudiante DONDE ID_est = 2";
    // del(query4);
    /*select(query3);*/
    // string query5 = "MODIFICAR Estudiante SET nom = 'Rensso' DONDE id_est < 1";

    // vector<string> data1 = processQuery(query1,0);
    // vector<string> data2 = processQuery(query2,1);
    // vector<string> data3 = processQuery(query3,2);
    // vector<string> data4 = processQuery(query4,3);
    // vector<string> data5 = processQuery(query5,4);

    // for(auto i :data1)
    // {
    //     cout<<"|"<<i<<"|"<<endl;
    // }
    // for(auto i :data2)
    // {
    //     cout<<"|"<<i<<"|"<<endl;
    // }
    // for(auto i :data3)
    // {
    //     cout<<"|"<<i<<"|"<<endl;
    // }
    // for(auto i :data4)
    // {
    //     cout<<"|"<<i<<"|"<<endl;
    // }
    // for(auto i :data5)
    // {
    //     cout<<"|"<<i<<"|"<<endl;
    // }
   
  

}

