#include<bits/stdc++.h>
#include<algorithm>
using namespace std;
const string metadata = "_metadata";
const string txt = ".txt";
using namespace std;

string fecha(){
  string fcha;
  int dia=rand()%31+1;
  fcha += to_string(dia)+"-";
  int mes=rand()%12+1;
  fcha += to_string(mes)+"-";
  int anho=rand()%11+1970;
  fcha += to_string(anho);
  return fcha;
}
vector<string> consulta()
{
  	string con = "create table peruano(";
  	int dni = 11111111;
  	vector<string> h;
	  srand(time(nullptr));
	vector<string> f;
  	for(int i = 0; i<2000; i++)
    {
		string nombre = "nombre-";
  		string apellido = "apellido-";
      	string tmp = to_string(dni);
      	nombre = nombre + tmp;
      	apellido = apellido + tmp;
      	int edad  = 0;
	  	edad = rand()%100+1;
	  	string fcha = fecha();
	  	con = con + tmp+","+nombre+","+apellido+","+to_string(edad)+","+fcha+")";
		cout<<con<<endl;
		f.push_back(con);
		con = "create table peruano(";
		dni++;
		
    }

	return f;
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
string blank(string str, int n)
{
	string xd;
	int x = n-str.size();
	for(int i=0; i<x; i++)
		xd = xd + ' ';
	return xd;
}
int get_number(string s)
{
	int t = s.size();
	string tmp;
	for(int i = 1; i<t; i++)
	{
		tmp = tmp + s[i];
	}
	return stoi(tmp);
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
vector<string> datos(string query)
{
	vector<string> d;
	int j = query.find("(")+1;
	int tam = query.size();
	string tmp;
	while(j<tam)
		tmp = tmp + query[j++];
	j = 0;
	string tmp2;
	string g = name_struct(query);
	while(j<tmp.size())
	{
		ofstream file;
		file.open("buffer"+g+".txt",fstream::app);
		if(tmp[j]=='-')
		{
			d.push_back(tmp2);
			tmp2.clear();
			if(tmp.substr(j+1, 3)=="int")
			{
				d.push_back("int");
				j = j+5;
				file<<15<<endl;
			}
			else if(tmp.substr(j+1, 4)=="char")
			{
				d.push_back("char");
				//c h a r ( 3 0 )
				//0 1 2 3 4 5 6 7
				//c h a r ( 9 )
				string t = tmp.substr(j+1,15);
				string m = t.substr(t.find("("), t.find(")")-t.find("("));
				int num = get_number(m);
				file<<num<<endl;
				j = j + m.size()+1+6;
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
	ofstream variables;
	variables.open(nombre+"_""variables.txt");
	for(int j = 1; j<i; j=j+2)
	{
		string tmp = d[j] + " " + d[j-1] + ";";
		file<<tmp<<endl;
        gaa<<d[j-1]+"-"+d[j]+";";
		variables<<d[j]<<endl;
	}
	file<<"};";
	file.close();
}
vector<string> obtener_datos(string query)
{
	vector<string> d;
	int i = query.find("(")+1;
	int tam = query.size()-1;
	string tmp;
	ifstream file;
	string b;
	string g = name_struct(query);
	file.open("buffer" + g + ".txt");
	vector<int> buffer_size;
	while (getline(file, b))
	{
		buffer_size.push_back(stoi(b));
	}
	int a = 0;
	while(query[i]!=')')
	{
		if(query[i]==',')
		{
			d.push_back(tmp+blank(tmp, buffer_size[a++]));
			tmp.clear();
			i++;
		}
		tmp = tmp + query[i];
		i++;
	}
	d.push_back(tmp+blank(tmp, buffer_size[a]));
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
	int tam = vc.size()-1;
	for(int i=0;i<tam;i++)
		file<<vc[i]+",";
	file<<vc[tam]+";";
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
    //size_t k = consulta.find("=");
	size_t k = 0;
	if(consulta.find("=")!=-1)
		k = consulta.find("=");
	else if(consulta.find("<")!=-1)
		k = consulta.find("<");
	else
		k = consulta.find(">");
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
	ifstream myfile;
	string str;
	myfile.open("buffer"+tabla+".txt");
	int buffer_size=0;
	while(getline(myfile,str))
		buffer_size = buffer_size + stoi(str);
    char *buffer = new char[buffer_size+n];
    char cond = condicion.c_str()[0];
	vector<string> var;
	ifstream vars;
	string g;
	vars.open(tabla+"_variables.txt");
	while(getline(vars,g))
		var.push_back(g);
    switch (cond)
    {
    case '=':
        //tamaño registro 10
        //int(2) + ',' string(6) + ';'
        for (int i = 0; i < length; i += buffer_size+n)
        {
            file.read(buffer,buffer_size+n);
            string temporalBuf = buffer;
            VecRegistros = ArmarRegistro(temporalBuf);
            temporalBuf = temporalBuf.substr(0, temporalBuf.size() - 1);
            if (VecRegistros[nCampo] == valorBuscado)
            {
                for (auto i : VecRegistros)
                    cout << i<<"|";
                cout<<endl;
            }
        }
	case '<':
		for(int i = 0; i<length; i = i + buffer_size+n)
		{
			file.read(buffer, buffer_size+ n);
			string temporalBuf = buffer;
			VecRegistros =  ArmarRegistro(temporalBuf);
			temporalBuf = temporalBuf.substr(0, temporalBuf.size()-1);
			int a = 0, b = 0;
			if(var[nCampo]=="int")
			{
				a = stoi(VecRegistros[nCampo]);
				b = stoi(valorBuscado);
			}
			if(a < b)
			{
				for(auto i: VecRegistros)
				{
					cout<<i<<"|";
				}
				cout<<endl;
			}
		}
	case '>':
		for(int i = 0; i<length; i = i + buffer_size+n)
		{
			file.read(buffer, buffer_size+n);
			string temporalBuf = buffer;
			VecRegistros = ArmarRegistro(temporalBuf);
			temporalBuf = temporalBuf.substr(0, temporalBuf.size()-1);
			int a = 0, b = 0;
			if(var[nCampo]=="int")
			{
				a = stoi(VecRegistros[nCampo]);
				b = stoi(valorBuscado);				
			}
			if(a > b)
			{
				for(auto i: VecRegistros)
				{
					cout<<i<<"|";
				}
			}
			cout<<endl;
		}
	}
	cout<<"select finalizado"<<endl;
}
string eliminar(string a)
{
	int i=0;
	string res;
	while(a[i]!=' ')
		res = res + a[i++];
	return res;
}
vector<string> split(string str, char simbolo) {
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    int posInit = 0;
    int posFound = 0;
    string splitted;
    vector<string> results;
    
    while(posFound >= 0){
        posFound = str.find(simbolo, posInit);
        splitted = str.substr(posInit, posFound - posInit);
        posInit = posFound + 1;
        results.push_back(splitted);
    }
    return results;
}
bool datoEncontrado(string registro, char simboloSeparacion, int nCampo,string datoBuscado)
{
    vector<string> vectoRegistro = split(registro,simboloSeparacion);
    return !datoBuscado.compare(vectoRegistro[nCampo]);
}
bool datoEncontrado(string registro, char simboloSeparacion, int nCampo,string datoBuscado, string operador)
{
	vector<string> vectoRegistro = split(registro, simboloSeparacion);
	if(operador == "<")
	{
		int a = stoi(datoBuscado);
		int b = stoi(vectoRegistro[nCampo]);
		return a>b;
	}
	int a = stoi(datoBuscado);
	int b = stoi(vectoRegistro[nCampo]);
	return a<b;
}
void BorrarDef(string consulta){
    string tabla,campoBuscado,datoBuscado;
    size_t tam = consulta.size(); 
    size_t i = consulta.find("desde");
    size_t j = consulta.find("donde");
    size_t k = 0;
	if(consulta.find("=")!=-1)
		k = consulta.find("=");
	else if(consulta.find("<")!=-1)
		k = consulta.find("<");
	else if(consulta.find(">")!=-1)
		k = consulta.find(">");
    tabla = consulta.substr(i+6,j-(i+7));
    campoBuscado = consulta.substr(j+6,k-(j+6));
    datoBuscado = consulta.substr(k+1,tam-(k+1)); 
    string campos = conseguirCampos(tabla);
	string simbolo = consulta.substr(k,1);
    int nCampo = encontrarNcampo(campos, campoBuscado, ';');
    fstream file;
    ofstream file_tmp;
    file.open("tabla_"+tabla+".txt");
    file_tmp.open("tmp.txt");
    string line;
    while(getline(file,line,';')){
		cout<<"#ssddzzzzzzzzz"<<endl;
		if(simbolo=="<" || simbolo==">")
		{
			if(!datoEncontrado(line,',',nCampo,datoBuscado,simbolo))
			{
				file_tmp<<line<<";";
			}
		}
		else if(simbolo=="="){
        	if(!datoEncontrado(line,',',nCampo,datoBuscado)){
        	    //cout<<"dcdc"<<line<<endl;
				file_tmp<<line<<';';
        	}
		}
        else{
            cout<<"Registro eliminado:"<<endl<<line<<endl;
        }
    }
    file.close();
    file_tmp.close();
    tabla+=".txt";
    remove(tabla.c_str());
	tabla = "tabla_"+tabla; 
    rename("tmp.txt",tabla.c_str());
}
vector<string> obtenerArgumentos(string consulta){
    vector<string> args;
    string tabla, campoCondicion, datoCondicion,campoBuscado,nuevoDato;
    size_t tam = consulta.size();
    size_t i = consulta.find("set");
    size_t j = consulta.find("donde");
    size_t l = consulta.find("=");
    size_t m = consulta.rfind("=");
    
    // if(l==m)
    // {
    //     if(consulta.find("<")!=std::string::npos){
    //         m = consulta.find("<");
    //         simboloCondicion = "<";
    //     }
    //     else if(consulta.find("<")!=std::string::npos)
    //     {
    //         m = consulta.find(">");
    //         simboloCondicion=">";
    //     }
    // }
    tabla = consulta.substr(10,i-10);
    campoBuscado = consulta.substr(i+4,l-(i+4));
    nuevoDato = consulta.substr(l+1,j-(l+1));
    campoCondicion = consulta.substr(j+6,m-(j+6));
    datoCondicion = consulta.substr(m+1,tam-(m+1));

    tabla.erase(remove(tabla.begin(), tabla.end(), ' '), tabla.end());
    campoBuscado.erase(remove(campoBuscado.begin(), campoBuscado.end(), ' '), campoBuscado.end());
    nuevoDato.erase(remove(nuevoDato.begin(), nuevoDato.end(), ' '), nuevoDato.end());
    campoCondicion.erase(remove(tabla.begin(), campoCondicion.end(), ' '), campoCondicion.end());
    datoCondicion.erase(remove(datoCondicion.begin(), datoCondicion.end(), ' '), datoCondicion.end());

    if(nuevoDato.find("'")!=std::string::npos)
        nuevoDato = nuevoDato.substr(1,nuevoDato.size()-2);
    if(datoCondicion.find("'")!=std::string::npos)
        datoCondicion = datoCondicion.substr(1,datoCondicion.size()-2);
    args.push_back(tabla);
    args.push_back(campoBuscado);
    args.push_back(nuevoDato);
    args.push_back(campoCondicion);
    args.push_back(datoCondicion);
    return args;
}
string mantenerConsistencia(string &dato,int nCampo, string tabla){
    ifstream file;
    string line;
    file.open("buffer"+tabla+".txt");
    int i = 0;
    while(i<=nCampo){
        getline(file,line);
        i++;
    }
    int buffer = stoi(line,nullptr);
    int nEspacios = buffer-dato.size(); 
    dato.append(nEspacios,' ');
    return dato;
}
void Actualizar(string consulta){
 
    string tabla, campoBuscado,nuevoDato,campoCondicion, datoCondicion;
    vector<string> argsFromConsulta = obtenerArgumentos(consulta);
    tabla = argsFromConsulta[0];
    campoBuscado = argsFromConsulta[1];
    nuevoDato = argsFromConsulta[2];
    campoCondicion = argsFromConsulta[3];
    datoCondicion = argsFromConsulta[4];
    
    string campos = conseguirCampos(tabla);
    int ncampoBuscado = encontrarNcampo(campos, campoBuscado, ';');
    int ncampoCondicion = encontrarNcampo(campos,campoCondicion,';');
    
    fstream file;
    ofstream file_tmp;
    file.open("tabla_"+tabla+txt);
    file_tmp.open("tmp.txt");
    string line,new_line; 
    
    while(getline(file,line,';')){
        if(!datoEncontrado(line,',',ncampoCondicion,datoCondicion)){
            file_tmp<<line<<';';
        }
        else{
            vector<string> tmp = split(line,',');
            tmp[ncampoBuscado] = nuevoDato;
            size_t tam = tmp.size();
            for(size_t i = 0;i<tam;i++)
            {
                mantenerConsistencia(tmp[i],i,tabla);
                new_line+=tmp[i];
                new_line+=',';
            }
            new_line = new_line.substr(0,new_line.size()-1); 
            file_tmp<<new_line<<';';
            //cout<<"Registro actualizado:"<<endl<<line<<" -> "<<new_line<<endl;
            new_line.clear(); 
        }
    }
    file.close();
    file_tmp.close();
    remove(("tabla_"+tabla+txt).c_str());
    rename("tmp.txt",("tabla_"+tabla+txt).c_str());
}
int main()
{
	while (true)
	{
		string query;
		getline(cin, query);
		string tmp = q(query);
		if(tmp == "create")
			create_table(query);
		else if(tmp == "insertar")
			insert(query);
		else if(tmp == "select")
			select(query);
		else if(tmp == "borrar")
			BorrarDef(query);
		else if(tmp == "modificar")
			Actualizar(query);
	}
	return 0;
}
