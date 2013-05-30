#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sstream>
#include <iostream>
//#include "debug.h"
#include "gfile.h"

// Variable globale pour le débogage
// C'est pas sa place, il faudrait le mettre ailleurs, mais bon...
GLogFile glog ((char*)"log/log.txt") ;
GLogFile gstat ((char*)"log/stat.txt") ;
GLogFile gstatcsv ((char*)"log/stat.csv") ;

/*!
 *  \class GInputFile
 *
 *  \brief
 */

/*! \fn FILE* GInputFile::open()
 *  \brief A GInputFile function.
 *  \return a FILE pointer.
 */
void GInputFile::open()
{
    file = fopen(fileName.c_str(), "r");
//    MSG_DEBUG("file.open", "Openning %s", fileName.c_str());
    if(!file){
        std::ostringstream output;
        output << "Un fichier nommé ::" << fileName << " n'existe pas ou est protégé en lecture";
        perror("\nGInputFile::load : input file open error");
        perror(fileName.c_str());

        //system("pause");
        //exit(-1);
//        FATAL(10, "Unable to open file %s", fileName.c_str());
    }
}

void GInputFile::open(char* st)
{
    fileName = st;
    open();
}
void GInputFile::close()
{
    fclose(file);
}

/*!
 * reads a line from the input file
 *
 * \return the line read
 */
char* GInputFile::readLine()
{
    indiceToken = 0 ;
    return fgets(line,MAX_LINE_LENGTH, file);
}

/*!
 * reads the first uncommented line from the input file, that is to say
 * the first line that does not begin with a # (in column 0 !!!)
 *
 * \return the line read
 */
char* GInputFile::readUncommentedLine()
{
    char *l;
    do{
        l = readLine();
    }while((l!=NULL) && (l[0]=='#')) ;
    return l;
}

/*!
 * reads a line from the input file and return the integer in the beginning
 * of the line
 *
 */
char* GInputFile::readInt(int &i)
{
    readUncommentedLine() ;
    i = strtol (line, NULL, 10) ;
    return line ;
}


/*!
 * reads a line from the input file and return the two integers in the
 * beginning of the line
 *
 */
char* GInputFile::readIntInt(int &i, int &j)
{
    char *l=readUncommentedLine() ;
    sscanf (l, "%d %d", &i, &j) ;
    return l ;
}

/*!
 * reads a line from the input file and return the three integers in the
 * beginning of the line, used to know the dimensions of the file: # vertices, #faces, #groups
 *
 */
char* GInputFile::readIntIntInt(int &i, int &j, int &k)
{
    char *l=readUncommentedLine() ;
    sscanf (l, "%d %d %d", &i, &j, &k) ;
    return l ;
}

/*!
 * reads a line from the input file and return the three doubles in the
 * beginning of the line, used to know the coordinates of each point
 */
char* GInputFile::readDoubleDoubleDouble(double &dX, double &dY, double &dZ) {
    char *l = readUncommentedLine() ;

    sscanf(l, "%lf %lf %lf", &dX, &dY, &dZ);

    return l;
}

/*!
 * returns next token from line (token = all symbols except space/tab/newline)
 *
 * \return the next token
 */
char* GInputFile::getNextToken()
{
    int strlenline = strlen(line) ;


    //while ((isspace(line[indiceToken]) || line[indiceToken]==';'|| line[indiceToken]==':') && (indiceToken<strlenline-1)) {
    while ((isspace(line[indiceToken]) || line[indiceToken]==';') && (indiceToken<strlenline-1)) {
        indiceToken++ ;
    }

    int indiceDebut = indiceToken ;
    //while (!(isspace(line[indiceToken]) || line[indiceToken]==';'|| line[indiceToken]==':')&& (indiceToken<strlenline)) {
    while (!(isspace(line[indiceToken]) || line[indiceToken]==';')&& (indiceToken<strlenline)) {
        token[indiceToken-indiceDebut]=line[indiceToken] ;
        indiceToken++ ;
    }
    token[indiceToken-indiceDebut]='\0' ;
    if (token[0]=='\0') return NULL ;
    return token ;
}


/*!
 * returns next token from line (token = all symbols except parameter/newline)
 *
 * \return the next token
 */
char* GInputFile::getNextToken(char separator)
{
    int strlenline = strlen(line) ;


    //while ((isspace(line[indiceToken]) || line[indiceToken]==';'|| line[indiceToken]==':') && (indiceToken<strlenline-1)) {
    while ((isspace(line[indiceToken]) || line[indiceToken]==separator) && (indiceToken<strlenline-1)) {
        indiceToken++ ;
    }

    int indiceDebut = indiceToken ;
    //while (!(isspace(line[indiceToken]) || line[indiceToken]==';'|| line[indiceToken]==':')&& (indiceToken<strlenline)) {
    while (!(isspace(line[indiceToken]) || line[indiceToken]==separator)&& (indiceToken<strlenline)) {
        token[indiceToken-indiceDebut]=line[indiceToken] ;
        indiceToken++ ;
    }
    token[indiceToken-indiceDebut]='\0' ;
    if (token[0]=='\0') return NULL ;
    return token ;
}

/*!
 * Return the end of the line starting from current token.
 *
 */
string GInputFile::getEndOfLine(){
    int strlenline = strlen(line);

    line[strlenline - 1] = 0;

    while( isspace(line[indiceToken]) && indiceToken < strlenline )
        indiceToken++;

    return string(line + indiceToken);
}

/*!
 * returns next int token from line (token = all symbols except space/tab/newline)
 *
 * \return the next int token
 */
int GInputFile::getNextIntToken()
{
    return atoi(getNextToken()) ;
}

/*!
 * returns next float token from line (token = all symbols except space/tab/newline)
 *
 * \return the next float token
 */
float GInputFile::getNextFloatToken()
{
    return atof(getNextToken()) ;
}

/*!
 * seeks a pattern and return the line if found, or NULL if not found
 *
 * \return the line that contains the pattern
 */
char* GInputFile::seek(char *patt){
    int len = strlen(patt);
    int found = 0;

    while( !found ){
        char* l = readUncommentedLine();
        if( l==NULL )
            return NULL;
        l[len] = '\0';
        if( strcmp(l,patt)==0 )
            return l;
    }

    return NULL;
}


/** Retourne les elements d'une ligne separes par des separateurs
    @param _file fichier dans lequel on travaille
    @param separators ensemble des separateurs possible [tabulation,espace,...]
    @param value vecteur de chaines de carateres des elements de la ligne
    @return boolean indiquant si la lecture est correcte
*/
bool GInputFile::getNextLine(ifstream &_file,
							const char* separators,
							vector<string>* value)
{
  bool output=false;
  string line;
  char *cstr, *p;
  p=(char *)malloc(50*sizeof(char));
  (*value).clear();
  getline(_file, line);

  while ( (!_file.eof())
	  &&((line.size()==0)
	     ||(line.at(0)=='#')
	     ||(line.at(0)=='\r')
	     ||(line.at(0)=='\v')
	     ||(line.at(0)=='\f')
	     ||(line.at(0)==' ')
	     ||(line.at(0)=='\t')) )
    getline(_file, line);

  if (!_file.eof())
    {
      output = true;
      cstr = new char [line.size()+1];
      strcpy (cstr, line.c_str());
      p=strtok(cstr, separators);

      while (p != NULL)
        {
	  strcpy (cstr, p);
	  (*value).push_back(cstr);
	  p=strtok(NULL, separators);
	}
      delete[] cstr;
    }
  else
    output = false;
  free(p);
  return output;
}
