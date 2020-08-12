#include <iostream>
#include <vector>
#include <cmath>
#include "cnf_io.cpp"
using namespace std;

bool vectoresiguales(vector<int> &a,vector<int> &b){
    if(a==b){
        for(int i=0;i<a.size();i++){
            bool igu=0;
            for(int j=0;j<b.size();j++){
                if(a[i]==a[j]){
                    igu=1;
                }
            }
            if(igu==0){
                return 0;
            }
        }
        return 1;
    }else{
        return 0;
    }
}
/*
Implementación SAT propia

bool evaluar(int n, bool *valores, vector<vector<int>> fnc){
    bool res=true;
    for(int i=0;i<n;i++){
        bool tmp=false;
        for(int j=0;j<fnc[i].size();j++){
            int pos=abs(fnc[i][j])-1;
            bool val=false;
            if(fnc[i][j]<0){
                val=!valores[pos];
            }else{
                val=valores[pos];
            }
            if(val==true){
                tmp=true;
            }
        }
        if(tmp==false){
            imprimirval(n,valores);
            cout<<"no cumple"<<endl;
            return false;
        }
    }
    return res;
}

bool backeval(int indice,int n,int n_val,bool *valores,vector<vector<int>> fnc){
    if(indice==n_val){
        if(evaluar(n,valores,fnc)){
            cout<<"Cumple"<<endl;
            imprimirval(n_val,valores);
            return true;
        }
    }else{
        valores[indice]=false;
        indice++;
        if(backeval(indice,n,n_val,valores,fnc)){
            return true;
        }
        indice--;
        valores[indice]=true;
        indice++;
        if(backeval(indice,n,n_val,valores,fnc)){
            return true;
        }
        indice--;
    }
    return false;
}
*/
class creencia{
public:
    vector<vector<int>> fnc;
    int nclau;
    int nvar;
    string cnf_file_name;
    creencia(){
        nclau=0;
        nvar=0;
    }
    creencia(string archivo){
        int c_num;
        cnf_file_name = archivo;
        bool error;
        int *l_c_num;
        int l_num;
        int *l_val;
        int v_num;
        error = cnf_header_read ( cnf_file_name, &v_num, &c_num, &l_num );

        if ( error )
        {
        cout << "\n";
        cout << "  No se pudo leer la cabecera del archivo.\n";
        }

        l_c_num = new int[c_num];
        l_val = new int[l_num];

        cnf_data_read ( cnf_file_name, v_num, c_num, l_num, l_c_num, l_val );
        int c;
        int l;
        int l_c;

        nclau=c_num;
        l = 0;
        for ( c = 0; c < c_num; c++ )
        {   
            vector<int> tmp;
            for ( l_c = 0; l_c < l_c_num[c]; l_c++ )
            {
            tmp.push_back(l_val[l]);
            l = l + 1;
            }
            fnc.push_back(tmp);
        }
        delete [] l_c_num;
        delete [] l_val;

        nvar=v_num;
    }
    bool guardarcnf(){
        bool error;
        ofstream output_unit;

        error = false;
        //
        //  Open the output file.
        //
        output_unit.open ( cnf_file_name.c_str ( ) );

        if ( !output_unit )
        {
            cout << "\n";
            cout << "CNF_WRITE - Fatal error!\n";
            cout << "  Cannot open the output file \"" << cnf_file_name << "\".\n";
            error = true;
            return error;
        }
        //
        //  Write the header.
        //
        error = cnf_header_write ( nvar, nclau, cnf_file_name, output_unit );

        if ( error )
        {
            cout << "\n";
            cout << "CNF_WRITE - Fatal error!\n";
            cout << "  Cannot write the header for the output file \"" << cnf_file_name << "\".\n";
            return error;
        }
        //
        //  Write the data.
        //
        int c;
        int l;
        int l_c;

        error = false;

        l = 0;

        for ( c = 0; c < nclau; c++ )
        {
            for ( l_c = 0; l_c < fnc[c].size(); l_c++ )
            {
            output_unit << " " <<  fnc[c][l_c];

            if (   l_c + 1  == fnc[c].size() )
            {
               output_unit << " " << setw(7) << 0 << "\n";
            }
            }
            
        }

        if ( error )
        {
            cout << "\n";
            cout << "CNF_WRITE - Fatal error!\n";
            cout << "  Cannot write the data for the output file \"" << cnf_file_name << "\".\n";
            return error;
        }
        //
        //  Close the file.
        //
        output_unit.close ( );

        return error;
    }
    int mayor(){
        int x=0;
        for(int i=0;i<fnc.size();i++){
            for(int j=0;j<fnc[i].size();j++){
                if(fnc[i][j]>x){
                    x=fnc[i][j];
                }
            }
        }
        return x;
    }
    bool verificar(){
        string str = "minisat C:/Users/USUARIO/Desktop/2020-A/ADA/TrabajoFinal/satbas/";
        string f=" C:/Users/USUARIO/Desktop/2020-A/ADA/TrabajoFinal/satbas/mini.txt";
        str = str + cnf_file_name + f; 
        const char *command = str.c_str();
        system(command);
        ifstream infile; 
        infile.open("mini.txt"); 
        char data[100];
        infile >> data;
        bool var;
        if(data[0]=='S'){
            var=1;
        }else if(data[0]=='U'){
            var=0;
        }
        infile.close();
        return var;
    }
    void imprimir(){
        for(int i=0;i<nclau;i++){
            cout<<i<<": ";
            for(int j=0;j<fnc[i].size();j++){
                cout<<fnc[i][j]<<" ";
            }
            cout<<endl;
        }
    }
    bool operator + (vector<int> &p){
        bool existe=0;
        for(int i=0;i<fnc.size();i++){
            if(vectoresiguales(fnc[i],p)){
                existe=1;
                return 0;
            }
        }
        if(existe==0){
            fnc.push_back(p);
            nclau++;
            nvar=mayor();
            return 1;
        }
    }
    bool operator - (vector<int> p){
        for(int i=0;i<nclau;i++){
            if(vectoresiguales(fnc[i],p)){
                fnc.erase(fnc.begin()+i);
                nclau--;
                nvar=mayor();
                return 1;
            }
        }
        return 0;
    }
};
bool expansion(creencia &sat,vector<int> &p){
    bool e = sat+p;
    if(e){
        sat.guardarcnf();
        if(sat.verificar()){
            return 1;
        }else{
            sat-p;
            sat.guardarcnf();
            return 0;
        }   
    }else{
        return 1;
    }
}
bool pertenece(creencia &sat,vector<int> &p){
    bool e = sat+p;
    if(e){
        sat.guardarcnf();
        bool per;
        if(sat.verificar()){
            per = 1;
        }else{
            per = 0;
        }
        sat-p;
        sat.guardarcnf();
        return per;
    }else{
        return 1;
    }
}
void intersec(creencia &sat1, creencia &sat2, creencia &res){
    res.cnf_file_name="inter.cnf";
    for(int i=0;i<sat1.fnc.size();i++){
        if(pertenece(sat2,sat1.fnc[i])){
            res+sat1.fnc[i];
        }
    }
    for(int i=0;i<sat2.fnc.size();i++){
        if(pertenece(sat1,sat2.fnc[i])){
            res+sat2.fnc[i];
        }
    }
}
void diferen(creencia &sat1, creencia &sat2, creencia &res){
    res.cnf_file_name="diff.cnf";
    for(int i=0;i<sat1.fnc.size();i++){
        if(!pertenece(sat2,sat1.fnc[i])){
            res+sat1.fnc[i];
        }
    }
}
void simetrica(creencia &sat1, creencia &sat2, creencia &res){
    res.cnf_file_name="sime.cnf";
    for(int i=0;i<sat1.fnc.size();i++){
        if(!pertenece(sat2,sat1.fnc[i])){
            res+sat1.fnc[i];
        }
    }
    for(int i=0;i<sat2.fnc.size();i++){
        if(!pertenece(sat1,sat2.fnc[i])){
            res+sat2.fnc[i];
        }
    }
}
int main(){
    creencia sat1("ejemplo.cnf");
    creencia sat2("ejemplo2.cnf");
    creencia res;
    diferen(sat1,sat2,res);
    res.guardarcnf();
    bool interres =res.verificar();

    creencia res2;
    intersec(sat1,sat2,res2);
    res2.guardarcnf();
    bool intersecres = res2.verificar();

    creencia res3;
    simetrica(sat1,sat2,res3);
    res3.guardarcnf();
    bool simeres = res3.verificar();

    cout<<"****************************************"<<endl;
    cout<<"Diferen"<<endl;
    res.imprimir();
    cout<<"Resultado: "<<interres<<endl;

    cout<<"****************************************"<<endl;
    cout<<"Intersec"<<endl;
    res2.imprimir();
    cout<<"Resultado: "<<intersecres<<endl;

    cout<<"****************************************"<<endl;
    cout<<"Dif-Sime"<<endl;
    res3.imprimir();
    cout<<"Resultado: "<<simeres<<endl;
    
    return 0;
}