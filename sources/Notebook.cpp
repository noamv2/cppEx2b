#include <string>
#include <iostream>
#include <cctype>
#include "Direction.hpp"
#include "Notebook.hpp"




using namespace ariel;
using std::string;

bool Notebook::isIn(int key){

    return notebook.count(key) != 0;
}

void Notebook::write(int page, int row, int col, Direction dir, string const & str){
    if(page < 0 || row < 0 || col < 0){
        throw "Invalid input";
    }

    for(auto c: str){

        if(isprint(c) == 0 || c == '~'){
            throw "Invalid input";
        }
    }
    //Create the page if needed
    if(!isIn(page)){
        notebook[page] = Page();
    }

    notebook[page].writeRange(row, col, dir,str.length(), str);
}

string Notebook::read(int page, int row, int col, Direction dir, int len){
    const int ROW_END = 99;
    if(page < 0 || len < 0 || row < 0 || col < 0 || col > ROW_END){
        throw "Invalid input";
    }
    //case we are reading page that wasnt created
    if(!isIn(page)){
        notebook[page] = Page();
    }

    return notebook[page].readRange(row, col, dir, len);
}

void Notebook::erase(int page, int row, int col, Direction dir, int len){
    if(page < 0 || len < 0 || row < 0 || col < 0){
        throw "Invalid input";
    }
    //case we are erasing in page that wasnt created yet
    if(!isIn(page)){
        notebook[page] = Page();
    }

    notebook[page].eraseData(row, col, dir, len);
}

void Notebook::show(int page){
    if (page < 0){
        throw "Cant have negetive page values";
    }
    std::cout<<notebook[page].readPage();
}

void Page::newLines(int num){

    for(int i = 0 ; i < num ; i ++){
        vector<char> v (ROW_END,'_');
        rows.push_back(v);
    }
}

void Page::writeRange(int row,int col,Direction dir, int len, std::string const & data){
    
    if(len < 0 || row < 0 || col < 0){
        throw "Invalid input";
    }

    //check if we have enough rows
    int currLen = rows.size();
    if(currLen == 0){
        currLen--; //in case where the page is empty
    }

    int linesToAdd = 0;
    
    if(dir == Direction::Vertical){
        
        //Add new rows if needed
        linesToAdd =(len - (currLen - row));
        if(linesToAdd > 0){
            newLines(linesToAdd);
        } 
        //make sure that we are writing to empty cells
        for(size_t i = static_cast<size_t>(row); i < len + row; i++){
            
            if(rows.at(i).at(static_cast<size_t>(col)) != '_'){
                throw "Cant write the same spot twice";
            }
        }
        //write the actual data
        for(char c: data){
            rows.at(static_cast<size_t>(row)).at(static_cast<size_t>(col)) = c;
            row++;
        }
    }

    else{

        //throw an exception if we are out of the row
        if(len + col > ROW_END){
            throw "Out of row error";
        }
        //Add new rows if needed
        
        linesToAdd =(row - currLen + 1);
        if(linesToAdd > 0){
            newLines(linesToAdd);
        } 
        int rowTemp = row;
        //make sure that we are writing to empty cells
        for(int i = col; i < col + len;i++){
            
            if(rows.at(static_cast<size_t>(rowTemp)).at(static_cast<size_t>(i % ROW_END)) != '_'){
                throw "Cant write the same spot twice";
            }
            rowTemp = row + i /ROW_END;
        }
        
        //write the actual data
        
        for(char c: data){

            rows.at(static_cast<size_t>(row)).at(static_cast<size_t>(col)) = c;
            ++col;

            if(col == ROW_END){
                col = 0;
                ++row;
            }
        }

    }


}

std::string Page::readPage(){
    std::string s;
    
    for(size_t i = 0 ; i < rows.size(); i++){

        s += std::to_string(i) + ": ";
        s += std::string(rows.at(i).data()) +"\n";

    }
    return s;
}

void Page::eraseData(int row,int col,Direction dir, int len){
    
    if(len < 0 || row < 0 || col < 0){
        throw "Invalid input";
    }
    if(len < 0){
        throw "Invalid length";
    }
    
    //check if we have enough rows
    int currLen = rows.size();
    if(currLen == 0){
        currLen--; //in case where the page is empty
    }
    int linesToAdd = 0;

    if(dir == Direction::Vertical){
        
        //Add new rows if needed
        linesToAdd =(len - (currLen - row));
        if(linesToAdd > 0){
            newLines(linesToAdd);
        } 
        //make sure that we are writing to empty cells
        for(int i = row; i < len + row; i++){
            
            rows.at(static_cast<size_t>(i)).at(static_cast<size_t>(col)) = '~';
            
        }
    }

    else{
        if(len + col > ROW_END){
            throw "Out of row error";
        }
        //Add new rows if needed
        
        linesToAdd =(row - currLen + 1);
        if(linesToAdd > 0){
            newLines(linesToAdd);
        } 
        int rowTemp = row;
        //make sure that we are writing to empty cells
        for(int i = col; i < col + len;i++){
            
            rows.at(static_cast<size_t>(rowTemp)).at(static_cast<size_t>(i % ROW_END)) = '~';

            rowTemp = row + i /ROW_END;
        }



    }


}

std::string Page::readRange(int row, int col, Direction dir, int len){




    if(len < 0){
        throw "Invalid length";
    }
    //fill the result string with '_' in case we want to read more then length of the notebook
    std::string res(static_cast<size_t>(len), '_'); 
    int index = 0;
    if(dir == Direction::Vertical){

        
        for(int i = row; i < rows.size() && index < len; i++){
            res.replace(static_cast<size_t>(index),1,1,rows.at(static_cast<size_t>(i)).at(static_cast<size_t>(col)));
            index++;
        }

    }


    else{


        if(len + col > ROW_END){
            throw "Out of row error";
        }
        if (row >= rows.size()){
            return res;
        }
    
        while(row != rows.size() && index < len){

            res.replace(static_cast<size_t>(index),1,1,rows.at(static_cast<size_t>(row)).at(static_cast<size_t>(col)));
            col++;
            index++;
            if(col == ROW_END){//go the next row if we reached the end of the current one
                col = 0;
                ++row;
            }
        }
    }


    return res;
}