#include<iostream>
#include<fstream>
#include<cstring>
#include"huffman.h"
#include<cstdlib>
using namespace std;
int main(int argc, char const *argv[])
{
    if(argc!=3){
        cerr<<"请使用两个参数，第一个为-c/-e,第二个为待处理的文件名"<<endl;
        exit(EXIT_FAILURE);
    }
    if(strncmp(argv[1],"-c",2)==0){
        ifstream input_file;
        input_file.open(argv[2],ios_base::in);
 if(!input_file.is_open()){
            cerr <<"无法打开所选文件"<<argv[2]<<endl;
            input_file.clear();
            input_file.close();
            exit(EXIT_FAILURE);
        }
             huffman HM=huffman(argv[2]);
             char current;
             while(input_file.get(current)){
                 HM.join(current);
             }
             input_file.clear();
             input_file.close();
            //  cout<<current;
             cout<<endl;
             HM.Sort();
            //  getchar();
             HM.store_coding("coding.hm");
             HM.write(HM.filename); 
    }
    else if(strncmp(argv[1],"-e",2)==0){
        huffman HM=huffman(argv[2]);
        HM.parse_coding("coding.hm");
        HM.read(HM.filename);
        HM.parse();
       

    }
    else cerr<<"参数错误"<<endl;
    
    
    return 0;
}
