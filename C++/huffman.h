#ifndef HUFFMAN_H_
#define HUFFMAN_H_
#include<fstream>
#include<iostream>
#include<map>
#include<vector>
#include <algorithm>
#include<cmath>
using namespace std;
class huffman{
    private:
    map<char,unsigned long> charset;
    //储存文件中出现的所有字符，char:字符，unsigned long:出现次数
    vector<pair<char,unsigned long>> sorted;
    //储存排序后的字符，char:字符，unsigned long:出现次数
    vector<bool> huffman_code;
    //储存哈夫曼编码，使用bool节约内存
    public:
     string filename;
    //储存待处理文件名(filename)
    bool IsIn(char);
    //检测当前字符(char)是否已出现在字符组中，若已出现返回true
    void join(char);
    //将字符(char)储存在字符组(charset)内
    void Sort();
    //将字符按出现频率从高到低排序
    void store_coding(string filename);
    //将编码方案储存至文件(filename)内
    void parse_coding(string filename);
    //从文件(filename)中读取编码方案
    void write(string file);
    //二次扫描待处理文件,并将哈夫曼编码写入二进制文件(file)中
    unsigned long bit_sum(char);
    //返回给定字符(char)对应编码中为１的总位数
    void read();
    //读取二进制文件
    void parse(string filename);
    //解析二进制文件,并将解析后的字符储存在文件(filename)中
    huffman(string name){
        filename=name;
    }
};
bool huffman::IsIn(char current){
     return charset.find(current)!=charset.end();
}
void huffman::join(char current){
    if(IsIn(current))
        charset[current]++;
    else
        charset[current]=1;
}

void huffman::Sort(){
     for(auto it:charset)
    {
        sorted.push_back(make_pair(it.first,it.second));
    }
sort(sorted.begin(),sorted.end(),[](const pair<char, unsigned long> &x, const pair<char, unsigned long> &y) -> int {
        return x.second > y.second;
    });
}
void huffman::store_coding(string filename){
    ofstream out_coding;
    out_coding.open(filename,ios_base::out);
    if(!out_coding.is_open()){
            cerr <<"写入编码方案失败"<<endl;
            out_coding.clear();
            out_coding.close();
            exit(EXIT_FAILURE);
        }
        else{
             for(auto it:sorted)
                out_coding<<it.first; 
        }
    cout<<"All done!"<<endl;
    out_coding.clear();
    out_coding.close();
}
void huffman::parse_coding(string filename){
ifstream in_coding;
in_coding.open(filename,ios::in);
 if(!in_coding.is_open()){
            cerr <<"无法打开编码文件"<<filename<<endl;
            in_coding.clear();
            in_coding.close();
            exit(EXIT_FAILURE);
        }
        else{
            char current;
            unsigned long i=0;
            while(in_coding.get(current)){
                sorted.push_back(make_pair(current,i++));
            }
            cout<<endl;
        }
       
}
unsigned long huffman::bit_sum(char current){
    unsigned long sum=0;
     for(auto each:sorted){
         if(current==each.first)
         break;
         sum++;
     }
     return sum;
} 
   
 void huffman::write(string file){
     ifstream input_file;
     input_file.open(filename,ios_base::in);
      char current;
         while(input_file.get(current)){
             unsigned long sum=bit_sum(current);
              if(sum==0)
              huffman_code.push_back(false);
              else{
                  huffman_code.insert(huffman_code.end(),sum,true);
                  huffman_code.push_back(false);
              }
         }
         huffman_code.insert(huffman_code.end(),8-huffman_code.size()%8,true);
     ofstream out_bin;
     out_bin.open(file,ios::out|ios::binary);
     if(!out_bin.is_open()){
         cerr<<"无法写入编码到文件"<<"huffman.bin"<<endl;
         out_bin.clear();
         out_bin.close();
     }
     else{
         int count=0;
         for(unsigned long i=0;i<huffman_code.size()/8;i++){
           unsigned char bt=0;
             for(int j=0;j<8;j++){
                 if(huffman_code.at(8*i+j)==true)
                 bt+=(unsigned)pow(2,7-j);
             }
             count++;
             char signed_char=(char)bt;
        //     cout<<"["<<(int)bt<<"]"<<" ";
         out_bin.write((char *)&bt,sizeof(char));
         }    
     }
 }
 void huffman::read(){
     ifstream in_bin;
     in_bin.open(filename,ios::in|ios::binary);
     if(!in_bin.is_open()){
         cerr<<"无法读取压缩后的文件"<<filename<<endl;
         in_bin.clear();
         in_bin.close();
     }
     else{
         char storage;int count =0;
         while(in_bin.read(&storage,sizeof(char))){
             count++;
             unsigned char Char=(unsigned char)storage;
             //cout<<"["<<(int)Char<<"]"<<" ";
             bool bak[8];
             for(int j=0;j<8;j++){
             if(Char%2==1) bak[7-j]=true;
             else bak[7-j]=false;
              Char >>= 1;
             }
            for(int j=0;j<8;j++)
                huffman_code.push_back(bak[j]);
         }
     }
 }
void huffman::parse(string filename){
     ofstream out_file;
        out_file.open(filename,ios::out);
    unsigned long flag=0;
    for(unsigned long i=0;i<huffman_code.size();i++){
        if(!huffman_code.at(i)){
           // cout<<"{"<<i-flag<<"}";
            out_file<<sorted.at(i-flag).first;
            flag=i+1;
        }
    }
    out_file.clear();
    out_file.close();
}
     
#endif