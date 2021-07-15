#include<bits/stdc++.h>
using namespace std;
template< class T>
struct node
{
    T data;
    vector<int> pos;
    node<T> *ndes[2] = {};
    int weight=1;
	int height = 0;
    int space = 0;
    int st = 0;
    int fact=0;
    node(T d)
    {
        data = d;
    }
};
template<class T>
struct avl_tree
{
    node<T> *root=nullptr;
    void insert(T d, int index);
    bool find(T d, node<T> **&ptr, vector<node<T> **> &arr);
    void update(vector<node<T> **> &arr, int t);
    void inorder(node<T> *n);
    void balanceo(node<T> **n);
	node<T>** rplce(vector<node<T> **> &arr, node<T> **m);
    void remove(T d);
};
template<class T>
void avl_tree<T>::balanceo(node<T> **n)
{
    if((*n)->fact==2)
    {
    	if(((*n)->ndes[1])->fact==1){
        	node<T> *nodeB=(*n)->ndes[1];
        	(*n)->ndes[1]=nodeB->ndes[0];
        	nodeB->ndes[0]=(*n);
        	(*n)=(nodeB);
			node<T> *l, *r, *p;
			p = (*n)->ndes[0]; l = p->ndes[0]; r = p->ndes[1];
			int x,y;
			x = l==nullptr?0:l->weight;
			y = r==nullptr?0:r->weight;
			p->weight = max(x,y)+1;
			(*n)->weight=max(((*n)->ndes[0])->weight, ((*n)->ndes[1])->weight)+1;
		}
		else
		{
            node<T> *a,*b,*c;
            a = (*n);
            b = ((*n)->ndes[1])->ndes[0];
            c = (*n)->ndes[1];
            T t = c->data;
            c->data = b->data; b->data = t;
            a->ndes[1] = b->ndes[0];
            b->ndes[0] = b->ndes[1]; b->ndes[1] = c->ndes[1];
            c->ndes[0] = a; c->ndes[1] = b;
            (*n) = c;
            int x,y;
            x = a->ndes[0]==nullptr?0:(a->ndes[0])->weight;
            y = a->ndes[1]==nullptr?0:(a->ndes[1])->weight;
            a->weight = max(x,y)+1;
            x = b->ndes[0]==nullptr?0:(b->ndes[0])->weight;
            y = b->ndes[1]==nullptr?0:(b->ndes[1])->weight;
            b->weight = max(x,y)+1;
            c->weight = max(a->weight, b->weight)+1;
		}
    }
    else
	{
		if(((*n)->ndes[0])->fact==-1)
		{
        	node<T> *nodeB=(*n)->ndes[0];
			(*n)->ndes[0]=nodeB->ndes[1];
			nodeB->ndes[1]=(*n);
			(*n)=nodeB;
			node<T> *l, *r, *p;
			p = (*n)->ndes[1]; l = p->ndes[0]; r = p->ndes[1];
			int x,y;
			x = l==nullptr?0:l->weight;
			y = r==nullptr?0:r->weight;
			p->weight = max(x,y)+1;
			(*n)->weight=max(((*n)->ndes[0])->weight, ((*n)->ndes[1])->weight)+1;
		}
		else
		{
			node<T> *a,*b,*c;
			a = (*n);
			c = (*n)->ndes[0];
			b = ((*n)->ndes[0])->ndes[1];
			T t; t = c->data;
			c->data = b->data;
			b->data = t;
			a->ndes[0] = b->ndes[1];
			b->ndes[1] = b->ndes[0]; b->ndes[0] = c->ndes[0];
			c->ndes[0] = b; c->ndes[1] = a;
			(*n) = c;
			int x,y;
			x = b->ndes[0]==nullptr?0:(b->ndes[0])->weight;
			y = b->ndes[1]==nullptr?0:(b->ndes[1])->weight;
			b->weight = max(x,y)+1;
			x = a->ndes[0]==nullptr?0:(a->ndes[0])->weight;
			y = a->ndes[1]==nullptr?0:(a->ndes[1])->weight;
			a->weight = max(x,y)+1;
			c->weight = max(a->weight, b->weight)+1;
		}
	}
}
template<class T>
void avl_tree<T>::update(vector<node<T> **> &arr, int t)
{
    int len = arr.size();
	if(t){
		for(int i=len-1; i>=0; i--){
			int a,b;
			a = (*arr[i])->ndes[1]==nullptr?0:((*arr[i])->ndes[1])->weight;
			b = (*arr[i])->ndes[0]==nullptr?0:((*arr[i])->ndes[0])->weight;
			(*arr[i])->weight=(a!=b)?(*arr[i])->weight+1:(*arr[i])->weight;
			(*arr[i])->fact=a-b;
			if(a==b)
				return;
			if(abs((*arr[i])->fact)>1)
			{
				balanceo(arr[i]);
				return;
			}
		}
	}
	else
	{
		for(int i=len-1; i>=0; i--)
		{
			int x,y;
			x = (*arr[i])->ndes[0]==nullptr?0:((*arr[i])->ndes[0])->weight;
			y = (*arr[i])->ndes[1]==nullptr?0:((*arr[i])->ndes[1])->weight;
			(*arr[i])->weight = max(x,y)+1;
			(*arr[i])->fact = y-x;
			if(abs((*arr[i])->fact)>1)
				balanceo(arr[i]);
		}
	}
}
template<class T>
bool avl_tree<T>::find(T d, node<T> **&ptr, vector<node<T> **> &arr)
{
    for(ptr=&root; (*ptr)&&(*ptr)->data!=d;)
    {
        arr.push_back(ptr);
        ptr=&(*ptr)->ndes[(*ptr)->data<d];
    }
    return ((*ptr)!=0);
}
template<class T>
void avl_tree<T>::insert(T d, int index)
{
    vector<node<T> **> arr;
    node<T> **pt;
    if(find(d, pt, arr))
    {
        (*pt)->pos.push_back(index);
        return;
    }
    *pt=new node<T>(d);
    (*pt)->pos.push_back(index);
    update(arr,1);
}
template<class T>
node<T>** avl_tree<T>::rplce(vector<node<T> **> &arr, node<T> **m)
{
    node<T> **n;
	int i = 0;
	arr.push_back(m);
	n = &(*m)->ndes[i];
	for(; (*n)->ndes[!i]; n=&(*n)->ndes[!i])
		arr.push_back(n);
    return n;
}
template<class T>
void avl_tree<T>::remove(T d)
{
	node<T> **r;
	vector<node<T> **> arr;
	if(!find(d,r,arr))
		return;
	if((*r)->ndes[0]!=nullptr && (*r)->ndes[1]!=nullptr)
	{
		node<T> **q;
		q = rplce(arr,r);
		(*r)->data=(*q)->data;
		r=q;
	}
    node<T> *aux = *r;
	(*r)=(*r)->ndes[!(*r)->ndes[0]];
	update(arr, 0);
    delete aux;
}
template<class T>
void avl_tree<T>::inorder(node<T> *n)
{
    if(n==nullptr)
        return;
    inorder(n->ndes[0]);
    int len = n->pos.size();
    cout<<"key: "<<n->data<<endl;
    for(int i = 0; i < len; i++)
        cout<<n->pos[i]<<endl;
    inorder(n->ndes[1]);
}
avl_tree<string> create_avl(vector<pair<string, int>> p)
{
    avl_tree<string> avl1;
    int len = p.size();
    for(int i = 0; i < len; i++)
        avl1.insert(p[i].first, p[i].second);
    return avl1;
}
avl_tree<int> create_avl_int(vector<pair<string, int>> p)
{
    avl_tree<int> avl1;
    int len = p.size();
    for(int i = 0; i < len; i++)
        avl1.insert(stoi(p[i].first), p[i].second);
    return avl1;
}
vector<pair<string, int>> get_datos(vector<string> data, vector<int> p, vector<string> n_colum,
avl_tree<string> &avl1, avl_tree<int> &avl2, vector<string> tipos)
{
    ifstream file;
    file.open(data[1]+".txt");
    string output;
    while (getline(file, output))
        file >> output;
    //cout<<output<<endl;
    file.close();
    string columna = data[2];
    int first = output.find(";");
    int len = output.size();
    string datos = output.substr(first+1, len-1);
    cout<<"datos<<< "<<datos<<endl;
    int pos = 10;
    for(int i=0; i<n_colum.size(); i++)
    {
        if(n_colum[i]==data[2]){
            pos = i;
            break;
        }
    }
    len = datos.size();
    cout<<len<<" gaaaaaaaaa"<<endl;
    int index = 0;
    int f = 0;
    int k = n_colum.size();
    vector<pair<string,int>> elements;
    while (f<len)
    {
        int l = p[index%k];
        if(index%k==pos)
        {
            pair<string, int> tmp;
            string ga = datos.substr(f,l);
            ga.erase(remove(ga.begin(), ga.end(), ' '), ga.end());
            tmp.first = ga;
            tmp.second = f;
            elements.push_back(tmp);
            cout<<"pos: "<<f<<endl;
            cout<<datos[f]<<endl;
            cout<<ga<<endl;
            cout<<"------------------------------"<<endl;
        }
        f = f + p[index%k]+1;
        index++;
    }
    if(tipos[pos]=="string")
    {
        avl1 = create_avl(elements);
        cout<<"gaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"<<endl;
        avl1.inorder(avl1.root);
    }
    else
    {
        avl2 = create_avl_int(elements);
        cout<<"gaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"<<endl;
        avl2.inorder(avl2.root);
    }
    return elements;
}
template<class T>
void UtilSerialize(node<T> *root, string index_name, string table_name, string &txt)
{
    if (root == nullptr)
    {
        txt = txt + "-1#";
        return;
    }
    txt = txt + string(root->data)+",";
    int len = root->pos.size();
    int i = 0;
    for(; i<len-1; i++)
    {
        txt = txt + to_string(root->pos[i])+",";
    }
    txt = txt + to_string(root->pos[len-1])+"#";
    UtilSerialize(root->ndes[0], index_name, table_name, txt);
    UtilSerialize(root->ndes[1], index_name, table_name, txt);
}
template<class T>
void serialize(string index_name, string table_name, node<T> *root)
{
    ofstream file;
    string txt;
    file.open(index_name+".txt");
    UtilSerialize(root, index_name, table_name, txt);
    file << txt;
    file.close();
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
template<class T>
void UtilDeSerialize(node<T> *&root, string &txt)
{
    string tmp;
    tmp  = txt.substr(0,txt.find("#"));
    txt = txt.substr(tmp.size()+1);
    vector<string> posi;
    posi = split_Character(tmp, ',');
    for(auto x: posi)
        cout<<"posi: "<<x<<endl;
    cout<<"------------------------"<<endl;
    if(posi[0]=="-1")
        return;
    root = new node<T>(posi[0]);
    int len = posi.size();
    for(int i = 1; i < len; i++)
        root->pos.push_back(stoi(posi[i]));
    UtilDeSerialize(root->ndes[0], txt);
    UtilDeSerialize(root->ndes[1], txt);
}
int main(){
    vector<string> data;
    vector<int> d;
    vector<string> h;
    vector<string> n_colum;
    vector<string> tipos;
    n_colum.push_back("ID_est");
    n_colum.push_back("nom");
    n_colum.push_back("ape");
    n_colum.push_back("edad");
    n_colum.push_back("fecnac");
    data.push_back("IDX_80000");
    data.push_back("tabla1");
    data.push_back("fecnac");
    d.push_back(12);
    d.push_back(30);
    d.push_back(30);
    d.push_back(12);
    d.push_back(10);
    h.push_back("int");
    h.push_back("string");
    h.push_back("string");
    h.push_back("string");
    h.push_back("int");
    h.push_back("string");
    tipos.push_back("int");
    tipos.push_back("string");
    tipos.push_back("string");
    tipos.push_back("int");
    tipos.push_back("string");
    avl_tree<string> avl1;
    avl_tree<int> avl2;
    vector<pair<string, int>> gaaaaa;
    gaaaaa = get_datos(data, d, n_colum, avl1, avl2, tipos);
    // int len = gaaaaa.size();
    // cout<<"mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm"<<endl;
    // for(int i = 0; i<len; i++)
    //     cout<<gaaaaa[i].first<<"    "<<gaaaaa[i].second<<endl;
    cout<<"zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz"<<endl;
    //avl1.inorder(avl1.root);
    //serialize("index_1", "estudiante", avl1.root);
    string txt;
    UtilSerialize(avl1.root, "index_1", "tabla1", txt);
    avl_tree<string> avl3;
    UtilDeSerialize(avl3.root, txt);
    cout<<"avl"<<endl;
    avl3.inorder(avl3.root);
    return 0;
}
