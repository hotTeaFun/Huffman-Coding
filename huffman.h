#ifndef HUFFMAN_H_
#define HUFFMAN_H_
#include<fstream>
#include<iostream>
#include<map>
#include<vector>
#include <algorithm>
#include<cmath>
using namespace std;
struct node {
    // node* parent;
    node* left;
    node* right;
    char node_char;
    unsigned long value;
    //字符出现次数
    std::string flag;
    //分配编码
    node(unsigned long v,char c){
        value=v;
        node_char=c;
    }
};

class huffman{
    private:
    const char FLAG_LEFT='1';
    const char FLAG_RIGHT='0';
    map<char,unsigned long> charset;
    //储存文件中出现的所有字符，char:字符，unsigned long:出现次数
    vector<node> sorted;
    //储存排序后的字符，char:字符，node:字符相关信息
    map<char,string> project;
    //储存编码方案，char:字符，string:{0|1}+
    vector<bool> huffman_code;
    //储存哈夫曼编码，使用bool节约内存
    public:
     string filename;
    //储存待处理文件名(filename)
    bool IsIn(char);
    //检测当前字符(char)是否已出现在字符组中，若已出现返回true
    void join(char);
    //将字符(char)储存在字符组(charset)内
    void change(pair<char,node>,bool);
    //重新改变编码方案
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
    void allocate();
    //为字符分配编码方案
    void travel(node&);
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
// void huffman::change(pair<char,node> current,bool flag){
//         project[current.first]=current.second;
//         project[current.first].flag=flag?"0":"1"+project[current.first].flag;
// }
void huffman::Sort(){
     for(auto it:charset)
    {
        sorted.push_back(*new node(it.second,it.first));
    }
sort(sorted.begin(),sorted.end(),[](const node &x, const node &y) -> int {
        return x.value > y.value;
    });
}

void huffman::allocate(){
    for(int i=0;i<sorted.size()-1;i++){
        auto left=sorted.back();
        sorted.pop_back();
        auto right=sorted.back();
        sorted.pop_back();
        auto leaf=*new node(left.value+right.value,(char)NULL);
        leaf.left=&left;
        leaf.right=&right;
        // left.second.parent=&leaf;
        // right.second.parent=&leaf;
        sorted.push_back(leaf);
        sort(sorted.begin(),sorted.end(),[](const pair<char, node> &x, const pair<char, node> &y) -> int {
        return x.second.value > y.second.value;
    });
    }
    travel(sorted.back());
}
void huffman::travel(node& root){
    if(root.left)
    project.insert(pair<char,string>(root.node_char,root.flag)); 
    else {
        root.left->flag=FLAG_LEFT+root.left->flag;
        travel(*(root.left));
    }
    if(root.right)
    project.insert(make_pair(root.node_char,root.flag));
    else  {
        root.right->flag=FLAG_RIGHT+root.right->flag;
        travel(*(root.right));
    }
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
             for(auto it:project)
                out_coding<<it.first<<" "<<it.second<<endl; 
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
                string coding;
                in_coding>>coding;
                project.insert(make_pair(current,coding));
            }
        }
       
}   
 void huffman::write(string file){
     ifstream input_file;
     input_file.open(filename,ios_base::in);
      char current;
         while(input_file.get(current))
             for(int i=0;i<project[current].size();i++)
                 huffman_code.push_back(project[current][i]==FLAG_LEFT);
         huffman_code.insert(huffman_code.end(),8-huffman_code.size()%8,true);
     ofstream out_bin;
     out_bin.open(file,ios::out|ios::binary);
     if(!out_bin.is_open()){
         cerr<<"无法写入编码到文件"<<"huffman.bin"<<endl;
         out_bin.clear();
         out_bin.close();
         exit(EXIT_FAILURE);
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
         exit(EXIT_FAILURE);
     }
     else{
         char storage;
         while(in_bin.read(&storage,sizeof(char))){ 
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
            out_file<<endl;
            flag=i+1;
        }
    }
    out_file.clear();
    out_file.close();
}
     
#endif