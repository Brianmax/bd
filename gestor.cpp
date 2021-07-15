#include <bits/stdc++.h>
#include <direct.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <limits>
using namespace std;

const string tablesDirectoryName = "Tablas";
const string metadata = "_metadata";
const string filesFormat = ".txt";
const char globalSplitter = ',';

string remove_spaces(string str)
{
    int tam = str.size();
    string tmp;
    for (int i = 0; i < tam; i++)
    {
        if (str[i] == ' ')
        {
            continue;
        }
        tmp += str[i];
    }
    return tmp;
}
string remove_character(string str, char remChar)
{
    int tam = str.size();
    string tmp;
    for (int i = 0; i < tam; i++)
    {
        if (str[i] == remChar)
        {
            continue;
        }
        tmp += str[i];
    }
    return tmp;
}
string replace_character(string str, char oldchar, char newchar)
{
    for (auto &i : str)
        i = i == oldchar ? newchar : i;
    return str;
}

enum operations
{
    crear = 0,
    insertar = 1,
    seleccionar = 2,
    borrar = 3,
    modificar = 4,
    indice = 5
};

vector<string> processQuery(string query, int operation)
{
    query = remove_character(query, '\''); //borrar comillas
    string queryProcessed = query;

    //poner todo en minusculas
    for_each(queryProcessed.begin(), queryProcessed.end(), [](char &c)
             { c = ::tolower(c); });

    vector<string> result;

    vector<vector<string>> operationMatrix =
        {
            {"crear tabla", "(", ")"},
            {"insertar", "(", ")"},
            {"select * desde", "donde"},
            {"borrar desde", "donde"},
            {"modificar", "set", "=", "donde"},
            {"crea indice", "en", "."}};

    size_t found1, found2, found3, found4;

    //todos obtienen el nombre de la tabla de la misma manera
    found1 = queryProcessed.find(operationMatrix[operation][1]);
    int tmp = operationMatrix[operation][0].size();
    result.push_back(query.substr(tmp + 1, found1 - (tmp + 1) - 1)); //tableName
    //para crear e insertar
    if (operation == crear || operation == insertar)
    {
        query.pop_back();
        result.push_back(query.substr(found1 + 1)); //TableCols
    }
    //para seleccionar y borrar
    else if (operation == seleccionar || operation == borrar)
    {
        found2 = queryProcessed.find_first_of("=<>");
        int tmp1 = operationMatrix[operation][1].size();
        result.push_back(query.substr(found1 + tmp1 + 1, found2 - (found1 + tmp1 + 1) - 1)); //filtercol
        result.push_back(query.substr(found2, 1));                                           //condition
        result.push_back(query.substr(found2 + 2));                                          //filterData
    }
    //para modificar
    else if (operation == modificar)
    {
        found2 = queryProcessed.find("=");
        found3 = queryProcessed.find(operationMatrix[operation][3]);
        found4 = queryProcessed.find_last_of("=><");
        int tmp1 = operationMatrix[operation][1].size();
        int tmp2 = operationMatrix[operation][3].size();
        result.push_back(query.substr(found1 + tmp1 + 1, found2 - (found1 + tmp1 + 1) - 1)); //setcol
        result.push_back(query.substr(found2 + 2, found3 - (found2 + 2) - 1));               //setdata
        result.push_back(query.substr(found3 + tmp2 + 1, found4 - (found3 + tmp2 + 1) - 1)); //filter
        result.push_back(query.substr(found4, 1));
        result.push_back(query.substr(found4 + 2));
    }
    // CREA_INDICE IDX_80000 EN “tb_estudiante”.”id_est”
    else if (operation == indice)
    {
        found2 = queryProcessed.find(operationMatrix[operation][2]);
        int tmp1 = operationMatrix[operation][1].size();
        result.push_back(query.substr(found1 + tmp1 + 1, found2 - (found1 + tmp1 + 1)));
        result.push_back(query.substr(found2 + 1));
    }

    return result;
}

vector<string> split_Character(string s, char c)
{
    vector<string> result;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(c)) != std::string::npos)
    {
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
    for (auto i : fields)
    {
        if (i.find("int") != std::string::npos || i.find("char") != std::string::npos)
        {
            result.push_back(stoi(i.substr(i.find("(") + 1, i.find(")") - 1)));
        }
        else if (i.find("date") != std::string::npos)
        {
            result.push_back(10);
        }
    }
    return result;
}

void createOP(string query)
{
    vector<string> fields = processQuery(query, 0);
    mkdir(tablesDirectoryName.c_str());
    mkdir(string(tablesDirectoryName + "/" + fields[0]).c_str());

    fstream file;
    file.open(string(tablesDirectoryName + "/" + fields[0] + "/" + fields[0] + filesFormat).c_str(), std::fstream::out);

    // "Crear Tabla Estudiante (ID_est-int(12), nom-char(30), ape-char(30), edad-int(12), fecnac-date)"
    vector<string> colNames = split_Character(fields[1], ',');
    string tmp_names;
    for (auto i : colNames)
    {
        i = i.substr(0, i.find("-"));
        tmp_names += remove_spaces(i) + ',';
    }
    tmp_names.pop_back();
    tmp_names += ';';
    file << tmp_names;

    file.close();
    file.open(string(tablesDirectoryName + "/" + fields[0] + "/" + fields[0] + metadata + filesFormat).c_str(), std::fstream::out);
    file << fields[0] << ';';
    file << remove_spaces(fields[1]) << ';';

    vector<int> tmp = getNeededSpace(split_Character(fields[1], ','));
    int sum = 0;
    string tmp_cols;
    for (auto i : tmp)
    {
        sum += i;
        tmp_cols += to_string(i) + ',';
    }
    tmp_cols.pop_back();
    tmp_cols += ';';
    file << tmp_cols;
    file.close();

    tmp_names.pop_back();
    tmp_cols.pop_back();

    cout << "Tabla creada : " << fields[0] << endl;
    cout << "Columnas(" << colNames.size() << ") : " << tmp_names << endl;
    cout << "Bytes : " << tmp_cols << endl;
}

//Obtiene cuantos bytes debe ocupor cada columna
vector<int> getSpacePerCol(string tableName)
{
    fstream file;
    file.open(string(tablesDirectoryName + "/" + tableName + "/" + tableName + metadata + filesFormat).c_str(), std::fstream::in);

    string line;
    string file_contents;
    while (std::getline(file, line))
    {
        file_contents += line;
    }
    vector<string> tmp_string = split_Character(file_contents, ';');
    tmp_string = split_Character(tmp_string[2], ',');
    vector<int> tmp_int;
    for (auto i : tmp_string)
    {
        tmp_int.push_back(stoi(i));
    }
    return tmp_int;
}

void insertOP(string query)
{
    //obtenemos el nombre del la tabla y las columnas
    vector<string> fields = processQuery(query, 1);
    fields[1] = remove_spaces(fields[1]);
    //abrimos el archivo
    fstream file;
    file.open(string(tablesDirectoryName + "/" + fields[0] + "/" + fields[0] + filesFormat).c_str(), fstream::app);

    //vector con los espacios disponibles
    vector<int> colsSpaces = getSpacePerCol(fields[0]);
    //vector con los datos
    vector<string> colsData = split_Character(fields[1], ',');

    colsData.pop_back();

    string registerTmp;

    //calculamos cuanto espacio extra hay y lo agregamos por cada col
    size_t tam = colsData.size();
    for (size_t i = 0; i < tam + 1; i++)
    {
        int tmp = colsSpaces[i] - colsData[i].size();
        registerTmp += colsData[i];
        for (int j = 0; j < tmp; j++)
        {
            registerTmp += " ";
        }
        registerTmp += ",";
    }
    registerTmp.pop_back();
    registerTmp.push_back(';');

    file << registerTmp;
    file.close();
}

bool operationsCondition(string a, string b, string condition)
{
    bool result = 0;
    if (condition == "=")
    {
        result = a == b;
    }
    else if (condition == "<")
    {
        result = stoi(a) < stoi(b);
    }
    else if (condition == ">")
    {
        result = stoi(a) > stoi(b);
    }
    return result;
}

void selectOP(string query)
{
    vector<string> fields = processQuery(query, 2);
    ifstream file;
    file.open(string(tablesDirectoryName + "/" + fields[0] + "/" + fields[0] + filesFormat).c_str());
    string tablefields;
    getline(file, tablefields, ';');

    cout << tablefields << endl;
    int tmp = tablefields.find(fields[1]);
    int nField = 0;
    for (int i = 0; i < tmp; i++)
    {
        if (tablefields[i] == ',')
            nField++;
    }
    string line;
    int returnedLines = 0;
    //lee linea por linea, delimitando por ; de modo que line obtiene un registro tratable
    while (getline(file, line, ';'))
    {
        line = remove_spaces(line);
        vector<string> registerTmp = split_Character(line, ',');

        if (operationsCondition(registerTmp[nField], fields[3], fields[2]))
        {
            cout << line << endl;
            returnedLines++;
        }
    }
    cout << "Registros devueltos : " << returnedLines << endl;
}
void deleteOP(string query)
{
    vector<string> fields = processQuery(query, 3);
    for (auto i : fields)
    {
        cout << i << " ";
    }
    cout << endl;
    ifstream file;

    file.open(string(tablesDirectoryName + "/" + fields[0] + "/" + fields[0] + filesFormat).c_str());
    string tablefields;
    getline(file, tablefields, ';');

    int tmp = tablefields.find(fields[1]);
    int nField = 0;
    for (int i = 0; i < tmp; i++)
    {
        if (tablefields[i] == ',')
            nField++;
    }
    string line;
    ofstream file_tmp;
    file_tmp.open(string(tablesDirectoryName + "/" + fields[0] + "/" + "tmp" + filesFormat).c_str());
    file_tmp << tablefields << ";";

    int deletedLines = 0;
    //lee linea por linea, delimitando por ; de modo que line obtiene un registro tratable
    while (getline(file, line, ';'))
    {
        vector<string> registerTmp = split_Character(line, ',');

        if (!operationsCondition(remove_spaces(registerTmp[nField]), fields[3], fields[2]))
        {
            file_tmp << line << ";";
        }
        else
        {
            deletedLines++;
        }
    }
    cout << "Registros borrados : " << deletedLines << endl;
    file.close();
    file_tmp.close();
    remove(string(tablesDirectoryName + "/" + fields[0] + "/" + fields[0] + filesFormat).c_str());
    rename(string(tablesDirectoryName + "/" + fields[0] + "/" + "tmp" + filesFormat).c_str(), string(tablesDirectoryName + "/" + fields[0] + "/" + fields[0] + filesFormat).c_str());
}
// string query5 = "MODIFICAR Estudiante(0) SET nom(1) = 'Rensso'(2) DONDE id_est(3) <(4) 1(5)";
// ID_est-int(12), nom-char(30), ape-char(30), edad-int(12), fecnac-date
void updateOP(string query)
{
    vector<string> fields = processQuery(query, 4);
    ifstream file;

    file.open(string(tablesDirectoryName + "/" + fields[0] + "/" + fields[0] + filesFormat).c_str());
    string tablefields;
    getline(file, tablefields, ';');

    int tmp = tablefields.find(fields[1]);
    int tmp1 = tablefields.find(fields[3]);

    int nFieldSet = 0, nFieldFilter = 0;
    for (int i = 0; i < tmp; i++)
    {
        if (tablefields[i] == ',')
            nFieldSet++;
    }
    for (int i = 0; i < tmp1; i++)
    {
        if (tablefields[i] == ',')
            nFieldFilter++;
    }

    string line;
    ofstream file_tmp;
    file_tmp.open(string(tablesDirectoryName + "/" + fields[0] + "/" + "tmp" + filesFormat).c_str());
    file_tmp << tablefields << ";";

    int uploadedLines = 0;
    //lee linea por linea, delimitando por ; de modo que line obtiene un registro tratable
    while (getline(file, line, ';'))
    {
        vector<string> registerTmp = split_Character(line, ',');

        if (operationsCondition(remove_spaces(registerTmp[nFieldFilter]), fields[5], fields[4]))
        {
            if (remove_spaces(registerTmp[nFieldSet]) != fields[2]) //Si ambos son iguales entonces no habra ningun cambio
            {
                uploadedLines++;
                string regtotxt;
                string setData = fields[2];

                // Agregamos espacios para mantener la consistencia
                for (int i = 0; i < registerTmp[nFieldSet].size() - fields[2].size(); i++)
                {
                    setData.push_back(' ');
                }
                registerTmp[nFieldSet] = setData;
                // Unificamos el vector de strings para que entre al txt
                for (auto i : registerTmp)
                {
                    regtotxt += i + ',';
                }
                regtotxt.pop_back();
                regtotxt += ";";
                file_tmp << regtotxt;
            }
            else
            {
                file_tmp << line << ";";
            }
        }
        else
        {
            file_tmp << line << ";";
        }
    }
    cout << "Registros actualizados :" << uploadedLines << endl;
    file.close();
    file_tmp.close();
    remove(string(tablesDirectoryName + "/" + fields[0] + "/" + fields[0] + filesFormat).c_str());
    rename(string(tablesDirectoryName + "/" + fields[0] + "/" + "tmp" + filesFormat).c_str(), string(tablesDirectoryName + "/" + fields[0] + "/" + fields[0] + filesFormat).c_str());
}

void indexOP(string query)
{
    // fields[0] = nombre del indice, fields[1] = nombre de la tabla, fields[2] = nombre de la columna
    vector<string> fields = processQuery(query, 5);
    vector<int> colsSpaces = getSpacePerCol(fields[1]);
}

vector<string> getDataTypes(string tableName)
{
    ifstream file(string(tablesDirectoryName + "/" + tableName + "/" + tableName + metadata + filesFormat).c_str());
    string meta;
    getline(file, meta, ';');
    getline(file, meta, ';');
    vector<string> dataTypes, tmp;
    tmp = split_Character(meta, ',');
    for (auto i : tmp)
    {
        if (i.find("int") != std::string::npos)
        {
            dataTypes.push_back("int");
        }
        else if (i.find("char") != std::string::npos)
        {
            dataTypes.push_back("char");
        }
        else if (i.find("date") != std::string::npos)
        {
            dataTypes.push_back("date");
        }
    }
    return dataTypes;
}

string fechaRandom()
{
    string fcha;
    int dia = rand() % 31 + 1;
    fcha += to_string(dia) + "-";
    int mes = rand() % 12 + 1;
    fcha += to_string(mes) + "-";
    int anho = rand() % 11 + 1970;
    fcha += to_string(anho);
    return fcha;
}
string cadenaRandom(int tam, string abc)
{
    string cadena;
    int abcTam = abc.size() - 1;
    tam = rand() % tam + 1;
    while (tam)
    {
        cadena += abc[rand() % abcTam + 1];
        if (abcTam < rand() % abcTam + 1)
        {
            cout << "ERRORRR";
        }
        tam--;
    }
    return cadena;
}


// Sistema Gestor de Bases de Datos (SGBD)
void sgbd()
{
    // system("Color DE");
    system("cls");
    for (int i = 0; i < 120; i++)
        cout << "-";
    cout << "\n";
    cout << "SGBD: Donut v0.1" << endl;
    cout << "\n";
    string query;
    while (query.compare("exit") != 0)
    {

        fstream file(string("historial.txt").c_str(), std::fstream::app);
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        cout << "> " << put_time(&tm, "%H:%M") << " ";
        getline(cin, query);
        file << put_time(&tm, "%H:%M");
        file << " ";
        file << query;
        file << "\n";
        file.close();

        cout << "\n";
        string queryPreProcessed = query;

        for_each(queryPreProcessed.begin(), queryPreProcessed.end(), [](char &c)
                 { c = ::tolower(c); });
        if (query.compare("exit") != 0)
        {
            if (queryPreProcessed.find("crear tabla") != std::string::npos)
            {
                createOP(query);
            }
            else if (queryPreProcessed.find("insertar") != std::string::npos)
            {
                insertOP(query);
            }
            else if (queryPreProcessed.find("borrar") != std::string::npos)
            {
                deleteOP(query);
            }
            else if (queryPreProcessed.find("select") != std::string::npos)
            {
                selectOP(query);
            }
            else if (queryPreProcessed.find("modificar") != std::string::npos)
            {
                updateOP(query);
            }
            else if (queryPreProcessed.find("crear indice") != std::string::npos)
            {
                indexOP(query);
            }
            else if (query.substr(0, 6).compare("random") == 0)
            {
                if (query.size() > 7)
                {
                    vector<string> tmp = split_Character(query, ' ');
                    vector<int> colsSpaces = getSpacePerCol(tmp[1]);
                    vector<string> colsTypes = getDataTypes(tmp[1]);
                    int nRegs = stoi(tmp[2]);
                    int nCols = colsSpaces.size();
                    string insertion;
                    for (int i = 0; i < nRegs; i++)
                    {
                        insertion = "insertar " + tmp[1] + " (";
                        for (int j = 0; j < nCols; j++)
                        {
                            if (colsTypes[j] == "int")
                            {
                                insertion += cadenaRandom(colsSpaces[j], "123456789");
                            }
                            else if (colsTypes[j] == "char")
                            {
                                insertion += cadenaRandom(colsSpaces[j], "abcdefghijklmnopqrstuvwxyz");
                            }
                            else
                            {
                                insertion += fechaRandom();
                            }
                            insertion += ",";
                        }
                        insertion.pop_back();
                        insertion += ")";
                        cout << insertion << endl;
                        insertOP(insertion);
                    }
                }
                else
                {
                    cout << "Comando incorrecto, intente nuevamente" << endl;
                }
            }
            else if (query.substr(0, 5).compare("start") == 0)
            {
                if (query.size() > 6)
                {
                    string tabla = query.substr(6);
                    system(string("start Tablas\\" + tabla + "\\" + tabla + ".txt").c_str());
                }
                else
                {
                    cout << "Comando incorrecto, intente nuevamente" << endl;
                }
            }
            else if (query.substr(0, 4).compare("copy") == 0)
            {
                if (query.size() > 5)
                {
                    string tabla1 = query.substr(5, query.find_last_of(" ") - 5);
                    string tabla2 = query.substr(query.find_last_of(" ") + 1);
                    system(string("mkdir Tablas\\" + tabla2 + ">nul").c_str());
                    system(string("xcopy Tablas\\" + tabla1 + " Tablas\\" + tabla2 + ">nul").c_str());

                    ifstream fileIn(string("Tablas/" + tabla2 + "/" + tabla1 + metadata + filesFormat).c_str());
                    ofstream fileOut(string("Tablas/" + tabla2 + "/" + tabla2 + metadata + filesFormat).c_str());

                    string line;
                    string tmp;
                    getline(fileIn, line, ';');
                    fileOut << tabla2;
                    fileOut << ";";

                    while (getline(fileIn, line, ';'))
                    {
                        fileOut << line << ";";
                    }

                    fileIn.close();
                    fileOut.close();

                    rename(string("Tablas/" + tabla2 + "/" + tabla1 + ".txt").c_str(), string("Tablas/" + tabla2 + "/" + tabla2 + ".txt").c_str());
                    remove(string("Tablas/" + tabla2 + "/" + tabla1 + metadata + filesFormat).c_str());
                    cout << tabla1 << " fue replicada en " << tabla2 << endl;
                }
                else
                {
                    cout << "Comando incorrecto, intente nuevamente" << endl;
                }
            }
            else if (query.substr(0, 2).compare("ls") == 0)
            {
                ofstream file;
                file.open("mt.ps1");

                string powershell;
                powershell = "Get-ChildItem -Path Tablas -name  -exclude metadata";
                file << powershell << endl;
                file.close();

                system("powershell -ExecutionPolicy Bypass -F mt.ps1");

                remove("mt.ps1");
            }
            else if (query.compare("help") == 0)
            {
                cout << "uso: donut [--version]\n"
                     << endl;
                cout << "Plantillas de consultas existentes: " << endl;
                cout << "     CREAR TABLA nombre_tabla (campo1-int(12),campo2-char(20))" << endl;
                cout << "     INSERTAR nombre_tabla (dato1,dato2)" << endl;
                cout << "     SELECT * DESDE nombre_tabla DONDE campo1 = dato1" << endl;
                cout << "     BORRAR DESDE nombre_tabla DONDE campo1 = dato1" << endl;
                cout << "     MODIFICAR nombre_tabla SET campo1 = dato1 DONDE campo2 = dato2\n"
                     << endl;

                cout << "Comandos Donut utiles en varias situaciones:" << endl;
                cout << "     ls                              Muestra las tablas" << endl;
                cout << "     start <tabla>                   Abre el archivo txt de la tabla deseada" << endl;
                cout << "     copy  <tabla1> <tabla2>         Copia el contenido de una tabla a otra" << endl;
                cout << "     random <tabla> <nroRegistros>   Llenar la tabla de datos random" << endl;
                cout << "     exit                            Salir del programa\n"
                     << endl;
            }
            else if (query.empty())
            {
                continue;
            }
            else
            {
                cout << query << " : No existe, intente nuevamente" << endl;
            }
            cout << "\n";
        }
    }
}

int main()
{
    cout<<"Hola";
}
