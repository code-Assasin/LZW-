/************************************************************************
author:      code-Assasin
date:        13/09/2015
title:       LZW compression and decompression

Description: The code reads the origianl text file 13.txt(text file assigned)
             into a string which is then taken in by the function 
             compress which compresses and returns a vector which is 
             written into a file 13_compressed.txt.
             
             NOTE:Due to large size of file it takes about 1-2mins to 
             compress.
                          
             For decompressing the vector returned by the compress 
             function is put into the decompress function and the 
             resultant output is written into 13_decompressed.txt.
             
             NOTE:Decompression happens considerably fast as the vector 
             is directly given as input and no FileIO is involved.
***************************************************************************/

 #include <string>
 #include <map>
 

template <typename Iterator>
Iterator lzwcompress(const std::string &uncompressed, Iterator result) {
  // Building the dictionary.
  int dictSize = 256;                                     
  std::map<std::string,int> dictionary;                 
  for (int i = 0; i < 256; i++)                              //all Ascii values are put in the dictionary initially
    dictionary[std::string(1, i)] = i;
 
  std::string w;
 
  for (std::string::const_iterator it = uncompressed.begin();
       it != uncompressed.end(); ++it) {
    char c = *it;
    std::string wc = w + c;
    if (dictionary.count(wc))
      w = wc;
    else { 
    //Inserting new values into dictionary
      *result++ = dictionary[w];      
      dictionary[wc] = dictSize++;
      w = std::string(1, c);
    }
  }
 
  // Output the code for w incase it is left out
  if (!w.empty())
    *result++ = dictionary[w];
  return result;
}
 

template <typename Iterator>
 std::string lzwdecompress(Iterator begin, Iterator end) // begin and end denote starting and ending of the vectors
{
  // Building the dictionary.
  int dictSize = 256;
  std::map<int,std::string> dictionary;
  for (int i = 0; i < 256; i++)
    dictionary[i] = std::string(1, i);
 
  std::string w(1, *begin++);
  std::string result = w;
  std::string entry;
  for ( ; begin != end; begin++) {
    int k = *begin;
    if (dictionary.count(k))
      entry = dictionary[k];
    else if (k == dictSize)
      entry = w + w[0];
    else
      throw "Bad compressed k"; //Incase if doesn't get decompressed properly..
 
    result += entry;
 
    // Dictionary Update
    dictionary[dictSize++] = w + entry[0];
 
    w = entry;
  }
  return result;
}
 
 #include <iostream>
 #include <iterator>
 #include <vector>
 #include <fstream>
 #include <iterator>
 #include <string>
 #include <cstdlib>
 
int main() {
       
   //Opening and Reading the text file
   std::string final_line="";
   std::string line;
 
  std::ifstream myfile ("13.txt");
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
      final_line=final_line+line;
    }
    
    myfile.close();
  } 

  else
  {
  std::cout << "Unable to open file"; 
  return 0;
  }
  /**********************************************COMPRESSION**********************************************************/
   std::cout <<"COMPRESSING...." ;
   std::cout <<std::endl;
   std::vector<int> compressed;
   lzwcompress(final_line, std::back_inserter(compressed));
  

  //Writing the compressed vector into a text file
  std::ofstream outfile;
  outfile.open ("13_compressed.txt");
  copy(compressed.begin(), compressed.end(), std::ostream_iterator<int>(outfile,""));
  outfile.close();    
  std::cout <<"\n\nCOMPRESSED\n\n" ;
 
  
  /****************************************DECOMPRESSION**************************************************/
  

  std::cout <<"DECOMPRESSING....\n\n";  
  std::string str_decompressed = lzwdecompress(compressed.begin(), compressed.end());

 //writing the decompressed data to a file
  std::ofstream decomp_outfile;
  decomp_outfile.open ("13_decompressed.txt");
  decomp_outfile<<str_decompressed; 
  decomp_outfile.close();  
  std::cout << "DECOMPRESSED\n\n";

  return 0;
}
