#include<bits/stdc++.h>
using namespace std;
const string metadata = "_metadata";
const string txt = ".txt";
string blank(string str, int n)
{
	string xd;
	int x = n-str.size();
	for(int i=0; i<x; i++)
		xd = xd + " ";
	return xd;
}
int isSubstring(string s1, string s2)
{
    int M = s1.length();
    int N = s2.length();
 
    /* A loop to slide pat[] one by one */
    for (int i = 0; i <= N - M; i++) {
        int j;
 
        /* For current index i, check for
 pattern match */
        for (j = 0; j < M; j++)
            if (s2[i + j] != s1[j])
                break;
 
        if (j == M)
            return i;
    }
 
    return -1;
}
void copiar(string nombre1, string nombre2)
{
	ifstream file1;
	ofstream file2;
	file1.open("estudiante");
	file2.open("header.cpp");
	string str;
	cout<<"xdxd"<<endl;
	while(getline(file1, str))
	{
		file2<<str<<endl;
	}
}
vector<string> datos(string query)
{
	vector<string> d;
	int j = query.find("(")+1;
	string tmp;
	while(query[j]!=')')
		tmp = tmp + query[j++];
	j = 0;
	string tmp2;
	while(j<tmp.size())
	{
		if(tmp[j]=='-')
		{
			d.push_back(tmp2);
			tmp2.clear();
			if(tmp.substr(j+1, 3)=="int")
			{
				d.push_back("int");
				j = j+5;
			}
			else if(tmp.substr(j+1, 4)=="char")
			{
				d.push_back("char");
				j = j + 6;
				//c h a r ( 3 0 )
				//0 1 2 3 4 5 6 7
				//c h a r ( 9 )
			}
			else
				cout<<"Error de sintaxis";
		}
		tmp2 = tmp2 + tmp[j];
		j++;
	}
	//i d - i n t , n o m b r e - c h a r
	//0 1 2 3 4 5 6 7 8 9 1 2 3 4 5 6 7 8
	return d;
}
string name_struct(string q)
{
	int j = q.find("(")-1;
	string tmp;
	while(q[j]!=' ')
		tmp = tmp + q[j--];
	reverse(tmp.begin(), tmp.end());
	return tmp;
}
void create_table(string query)
{
	string tpt = "struct ";
	string nombre = name_struct(query);
    ofstream gaa;
	ofstream nn;
	nn.open("n_"+nombre);
    gaa.open(nombre+"_"+"metadata"+".txt");
	ofstream file;
	file.open(nombre, ios::binary);
	tpt = tpt + nombre;
	file<<tpt<<endl;
	file<<"{"<<endl;;
	vector<string> d = datos(query);
	int i = d.size();
	nn<<i/2;
	for(int j = 1; j<i; j=j+2)
	{
		string tmp = d[j] + " " + d[j-1] + ";";
		file<<tmp<<endl;
        gaa<<d[j-1]+"-"+d[j]+";";
	}
	file<<"};";
	file.close();
}
vector<string> obtener_datos(string query)
{
	vector<string> d;
	int i = query.find("(")+1;
	string tmp;
	while(query[i]!=')')
	{
		if(query[i]==',')
		{
			d.push_back(tmp+blank(tmp, 15));
			tmp.clear();
			i++;
		}
		tmp = tmp + query[i];
		i++;
	}
	d.push_back(tmp+blank(tmp, 15));
	return d;
}
void insert(string str)
{
	string nombre = name_struct(str);
	ofstream file;
	nombre = "tabla_"+nombre+".txt";
	file.open(nombre, fstream::app);
	vector<string> vc;
	vc = obtener_datos(str);
	for(auto x: vc)
		file<<x+",";
}
string q(string query)
{
	int i = 0;
	string tmp;
	while(query[i]!=' ')
		tmp = tmp + query[i++];
	return tmp;
}
string nombre_select(string str)
{
	string tmp;
	int i = isSubstring("desde", str)+6;
	while(str[i]!=' ')
		tmp = tmp + str[i++];
	return tmp;
}
string var(string s)
{
	string f;
	int j = s.find("=");
	while(s[j]!=' ')
		f = f + s[j--];
	reverse(f.begin(), f.end());
	return f;
}
/*void select(string query)
{
	string tmp;
	string n = nombre_select(query);
	ifstream file;
	file.open("tabla_"+n+".txt");
	string f = var(tmp);
	cout<<f<<endl;
}*/
vector<string> ArmarRegistro(string registro)
{
    size_t i = 0;
    size_t tam = registro.length();
    registro = registro.substr(0, tam - 1);
    registro += ",";
    string campoRegistro;
    vector<string> RegistroArmado;

    while (i < tam)
    {
        if (registro[i] == ',')
        {
            RegistroArmado.push_back(campoRegistro);   
            campoRegistro.clear();
            i++;
        }
        if(registro[i]!=' ')
        {
            campoRegistro += registro[i];
        }
        i++;
    }
    return RegistroArmado;
}

int contarSimbolos(string cadena, char simbolo)
{
    size_t tam = cadena.length();
    size_t contador = 0;

    for (size_t i = 0; i < tam; i++)
    {
        if (cadena.at(i) == simbolo)
        {
            contador++;
        }
    }
    return contador;
}
int encontrarNcampo(string campos, string campoBuscado, char simboloDelimitador)
{
    size_t nCampo = 0;

    size_t found = campos.find(campoBuscado);

    for (size_t i = 0; i < found; i++)
    {
        if (campos.at(i) == simboloDelimitador)
        {
            nCampo++;
        }
    }
    return nCampo;
}

string conseguirCampos(string tabla)
{
    ifstream archivo;
    string campos;
    int ncampo = -1;

    archivo.open(tabla + metadata + txt, ios::in);
    getline(archivo, campos);

    archivo.close();
    return campos;
}
void select(string consulta)
{
    string tabla, campoBuscado, valorBuscado;
    string condicion; 
    //string consulta = "select * desde profesor donde id_est=1";
    size_t tam = consulta.size(); 
    size_t i = consulta.find("desde");
    size_t j = consulta.find("donde");
    size_t k = consulta.find("=");
    tabla = consulta.substr(i+6,j-(i+7));
    campoBuscado = consulta.substr(j+6,k-(j+6));
    condicion = consulta.substr(k,1);
    valorBuscado = consulta.substr(k+1,tam-(k+1)); 

    string campos = conseguirCampos(tabla);
    int nCampo = encontrarNcampo(campos, campoBuscado, ';');
    ifstream nn;
	nn.open("n_"+tabla);
	string tmp;
	int n = 0;
	while(getline(nn,tmp))
	{
		n = stoi(tmp);
	}
    ifstream file;
    file.open("tabla_"+tabla + txt, ios::binary);
    file.seekg(0, file.end);
    int length = file.tellg();
    file.seekg(0, file.beg);

    vector<string> VecRegistros;

    char *buffer = new char[15*n+n];
    char cond = condicion.c_str()[0];

    switch (cond)
    {
    case '=':
        //tamaño registro 10
        //int(2) + ',' string(6) + ';'
        for (int i = 0; i < length; i += 15*n+n)
        {
            file.read(buffer,15*n+n);
            string temporalBuf = buffer;
            temporalBuf = temporalBuf.substr(0, temporalBuf.size() - 1);
            VecRegistros = ArmarRegistro(temporalBuf);

            
            if (VecRegistros[nCampo] == valorBuscado)
            {
                for (auto i : VecRegistros)
                    cout << i<<"|";
                cout<<endl;
            }

        }
    }
}
int main()
{
	while (true)
	{
		string query;
		getline(cin, query);
		string tmp = q(query);
		if(tmp == "create")
		{
			create_table(query);
		}
		else if(tmp == "insertar")
		{
			insert(query);
		}
		else if(tmp == "select")
		{
			select(query);
		}
	}
    return 0;
}