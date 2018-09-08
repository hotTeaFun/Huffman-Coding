#include<iostream>
#include<fstream>
#include<cstring>
#include"huffman.h"
#include<cstdlib>
#define DEF_Code_file "coding.hm"
#define DEF_out_bfile "huffman.bin"
#define DEF_out_tfile "origin.txt"
using namespace std;
void Single_Compress(string in_file,
                     string code_file=DEF_Code_file,
                     string out_file=DEF_out_bfile);
void Single_Extract(string in_file,
                    string code_file=DEF_Code_file,
                    string out_file=DEF_out_tfile);
void Group_Compress(string filename[]);
void Group_Extract(string filename[]);
void invalid_arg(int length,string[]);
void Extract_Report(string in_file,
                    string code_file=DEF_Code_file,
                    string out_file=DEF_out_tfile);
void Compress_Report(string in_file,
                     string code_file=DEF_Code_file,
                     string out_file=DEF_out_bfile);                 
inline void help(){
    cout<<"用法：　huffman [选项]？　[文件名 ]＋"<<endl;
    cout<<"选项：　-c|-C 压缩文件;　-e|-E 解压文件；－v|--version 打印版本信息； －h|--help|? 打印帮助信息"<<endl;
    cout<<"        "<<"-o|-O 指定输出文件；　-u|-u 指定编码方案文件"<<endl;
}
inline void cout_version(){
    cout<<"版本：beta 1.0.0"<<endl;
}
int main(int argc, char const *argv[])
{
    cout<<"欢迎使用C++版简易的基于Huffman编码的压缩文件程序。@－@"<<endl;
    string vag[argc-1];
    for(int i=1;i<argc;i++)
       vag[i-1]=argv[i];
    switch(argc){
        case 1: 
        help(); 
        break;

        case 2:
         if(vag[0]=="-v"||vag[0]=="--version")
        cout_version(); 
        else if(vag[0]=="-h"||
                vag[0]=="?"||
                vag[0]=="--help")
            help(); 
        else 
              Single_Compress(vag[0]);
        break;

        case 3:
        if(vag[0]=="-c"||vag[0]=="-C")
           Single_Compress(argv[2]);
      
        else if(vag[0]=="-e"||vag[0]=="-E")
            Single_Extract(vag[1]);
        
        case 4:
        if(vag[0]=="-o"||vag[0]=="-O")
        Single_Compress(vag[2],DEF_Code_file,vag[1]);
        else if(vag[0]=="-u"||vag[0]=="-U")
        Single_Compress(vag[2],vag[1]);

        case 5:
        if(vag[0]=="-o"||vag[0]=="-O")
       { if(vag[2]=="-c"||vag[2]=="-C")
        Single_Compress(vag[3],DEF_Code_file,vag[1]);
        else if(vag[2]=="-e"||vag[2]=="-E")
        Single_Extract(vag[3],DEF_Code_file,vag[1]);
        }
        else if(vag[0]=="-u"||vag[0]=="-U")
        { if(vag[2]=="-c"||vag[2]=="-C")
            Single_Compress(vag[3],vag[1]);
        else if(vag[2]=="-e"||vag[2]=="-E")
        Single_Extract(vag[3],vag[1]);
        }
         if(vag[1]=="-o"||vag[1]=="-O")
       {if(vag[0]=="-c"||vag[0]=="-C")
        Single_Compress(vag[3],DEF_Code_file,vag[2]);
        else if(vag[0]=="-e"||vag[0]=="-E")
        Single_Extract(vag[3],DEF_Code_file,vag[2]); }
        else if(vag[1]=="-u"||vag[1]=="-U")
       { if(vag[0]=="-c"||vag[0]=="-C")
            Single_Compress(vag[3],vag[2]);
        else if(vag[0]=="-e"||vag[0]=="-E")
        Single_Extract(vag[3],vag[2]);
        }
        case 6:
        if((vag[0]=="-o"||vag[0]=="-O")&&
        (vag[2]=="-u"||vag[2]=="-U"))
        Single_Compress(vag[4],vag[3],vag[1]);
        else if((vag[0]=="-u"||vag[0]=="-U")&&
        (vag[2]=="-o"||vag[2]=="-U"))
        Single_Compress(vag[4],vag[1],vag[3]);
        break;

        default:
        if(vag[0]=="-c"||vag[0]=="-C"){
            for(int i=1;i<argc-1;i++){
                if(vag[i][0]=='-')
                invalid_arg(argc-1,vag);
                else{
                     Single_Compress(vag[i],to_string(i-1)+DEF_Code_file,
                     to_string(i-1)+DEF_out_bfile);
                }     
            }
        }
        if(vag[0]=="-E"||vag[0]=="-e"){
            cout<<"此版本尚未支持多文件解压操作"<<endl;
        }

    }
    return 0;
}
void Single_Compress(string in_file,
                     string code_file,
                     string out_file){
    cout<<"尝试压缩文件"<<in_file<<"..."<<endl;
    ifstream input_file;
    input_file.open(in_file,ios_base::in);
    if(!input_file.is_open()){
         cerr <<"无法打开所选文件"<<in_file<<endl;
        input_file.clear();
        input_file.close();
        exit(EXIT_FAILURE);
   }
    huffman HM=huffman(in_file);
    char current;
    while(input_file.get(current)){
    HM.join(current);
    }
    input_file.clear();
    input_file.close();
    HM.Sort();
    HM.store_coding(code_file);
    HM.write(out_file);
    cout<<"已成功压缩文件："<<in_file<<"  "<<
      "编码方案文件为："<<code_file<<"  "<<
      "输出压缩文件为："<<out_file<<endl;   
}
void Single_Extract(string in_file,
                    string code_file,
                    string out_file){
        cout<<"尝试解压文件"<<in_file<<"..."<<endl;
        huffman HM=huffman(in_file);
        HM.parse_coding(code_file);
        HM.read();
        HM.parse(out_file);
        cout<<"已成功解压文件："<<in_file<<"  "<<
      "编码方案文件为："<<code_file<<"  "<<
      "输出解压文件为："<<out_file<<endl; 
}
void invalid_arg(int length,string* arg){
    for(int i=0;i<length;i++)
    cout<<arg[i]<<" ";
    cout<<endl;
}
