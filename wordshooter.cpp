#ifndef WORD_SHOOTER_CPP
#define WORD_SHOOTER_CPP

//#include <GL/gl.h>
//#include <GL/glut.h>
#include <iostream>
#include<string>
#include<cmath>
#include<fstream>
#include "util.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
using namespace std;
#define MAX(A,B) ((A) > (B) ? (A):(B)) // defining single line functions....
#define MIN(A,B) ((A) < (B) ? (A):(B))
#define ABS(A) ((A) < (0) ? -(A):(A))
#define FPS 15

string * dictionary;
int dictionarysize = 369646;
#define KEY_ESC 27 // A

// 20,30,30
const int bradius = 30; // ball radius in pixels...

int width = 930, height = 660;
int byoffset = bradius;

int nxcells = (width - bradius) / (2 * bradius);
int nycells = (height - byoffset /*- bradius*/) / (2 * bradius);
int nfrows = 2; // initially number of full rows //
float score = 0;
int **board; // 2D-arrays for holding the data...
int bwidth = 130;
int bheight = 10;
int bsx, bsy;
const int nalphabets = 26;
enum alphabets {
	AL_A, AL_B, AL_C, AL_D, AL_E, AL_F, AL_G, AL_H, AL_I, AL_J, AL_K, AL_L, AL_M, AL_N, AL_O, AL_P, AL_Q, AL_R, AL_S, AL_T, AL_U, AL_W, AL_X, AL_y, AL_Z
};
GLuint texture[nalphabets];
GLuint tid[nalphabets];
string tnames[] = { "a.bmp", "b.bmp", "c.bmp", "d.bmp", "e.bmp", "f.bmp", "g.bmp", "h.bmp", "i.bmp", "j.bmp",
"k.bmp", "l.bmp", "m.bmp", "n.bmp", "o.bmp", "p.bmp", "q.bmp", "r.bmp", "s.bmp", "t.bmp", "u.bmp", "v.bmp", "w.bmp",
"x.bmp", "y.bmp", "z.bmp" };
GLuint mtid[nalphabets];
int awidth = 60, aheight = 60; // 60x60 pixels cookies...

//declarations apni

bool start = true;

int timepassed = 0;

int countdown = 150; 

bool finished = false; 

bool  gamepop = true;



const int rownum = 10;     

const int alpha_row = 15; 

int randomalphabet[rownum][alpha_row];//array for random alpha



int shooterbub;//the alphabet on the shooter
int shooterbub_2;


int sX;//x shooter

int sY = 0;//Y shooter


float x_pos, y_pos;//current position of shooter wala alpha

float posx_inc, posy_inc;//increment so keep moving

float mouseclickx,mouseclicky;//mouse click position
  


bool bubbleshot = false;//shooter alphabet moving or no








                                                                          //untouched
//USED THIS CODE FOR WRITING THE IMAGES TO .bin FILE
void RegisterTextures_Write()
//Function is used to load the textures from the
// files and display
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);
	vector<unsigned char> data;
	ofstream ofile("image-data.bin", ios::binary | ios::out);
	// now load each cookies data...

	for (int i = 0; i < nalphabets; ++i) {

		// Read current cookie

		ReadImage(tnames[i], data);
		if (i == 0) {
			int length = data.size();
			ofile.write((char*)&length, sizeof(int));
		}
		ofile.write((char*)&data[0], sizeof(char) * data.size());

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ofile.close();

}                                                                      
void RegisterTextures()
/*Function is used to load the textures from the
* files and display*/
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);

	vector<unsigned char> data;
	ifstream ifile("image-data.bin", ios::binary | ios::in);

	if (!ifile) {
		cout << " Couldn't Read the Image Data file ";
		//exit(-1);
	}
	// now load each cookies data...
	int length;
	ifile.read((char*)&length, sizeof(int));
	data.resize(length, 0);
	for (int i = 0; i < nalphabets; ++i) {
		// Read current cookie
		//ReadImage(tnames[i], data);
		/*if (i == 0) {
		int length = data.size();
		ofile.write((char*) &length, sizeof(int));
		}*/
		ifile.read((char*)&data[0], sizeof(char)* length);

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ifile.close();
}                                                                    
void DrawAlphabet(const alphabets &cname, int sx, int sy, int cwidth = 60,
	int cheight = 60)
	/*Draws a specfic cookie at given position coordinate
	* sx = position of x-axis from left-bottom
	* sy = position of y-axis from left-bottom
	* cwidth= width of displayed cookie in pixels
	* cheight= height of displayed cookiei pixels.
	* */
{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;
	
	
	
	glColor4f(1, 1, 1, 1);
	
	if(score > 18){
	glColor4f(1, 1, 0, 0);
	}
	
	if(score > 30){
	glColor4f(0, 1, 1, 0);
	
	}
	if(finished){
	glColor4f(1, 1, 1, 1);
	}

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mtid[cname]);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);
	
	

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}                                                                      
int GetAlphabet() {
	return GetRandInRange(0, 25);
	
}

                                                                //MY FUNCTIONS
//apnaye for dictionary
bool Indictionary(char searchword[]){

    int start = 0, count, calc , mid , end = (dictionarysize - 1);   
    
    bool smaller, larger; 
    
    
    while(start <= end){
    
         count = 0;
         
         smaller = false;
         
         larger = false; 
         
         mid = (start + end) / 2; 
        
        
        
        while(searchword[count]!= '\0' &&  dictionary[mid][count] !='\0'){
        
           
                
            if(searchword[count]> dictionary[mid][count]){
            
            
              larger = true;
              
               break;}
               
               
              else if (searchword[count] <dictionary[mid][count]){
              
              
             smaller = true;
             
              break;} 
                
            calc = 1+1;
            count++;//next ch     
          
               }
        
        
        if(!smaller&& !larger){
        
        
        if (searchword[count]== '\0' && dictionary[mid][count]== '\0'){
        
        
        return true;}}
        
        
        
        
        if(smaller||(searchword[count]== '\0'&& dictionary[mid][count]!= '\0')){
        
        calc = 0;
        
         end = mid - 1;
         
         
        }
        
        
        else if (larger||(searchword[count]!= '\0'&& dictionary[mid][count]== '\0')){
        
        calc = 1;
          start = mid + 1;}
          
          
          
          
          
    }
    return false;
    
    
}



void assign_grid(){

for(int i = 2 ; i < 10 ; i++){
    for (int j = 0; j < alpha_row; j++){
    
    
    randomalphabet[i][j] = 99;}}



}




void burst(int row, int col){


ofstream outfile("bursted_words.txt", ios::out | ios::app);



    int len_tillnow = 0 , lencount = 0 , R, C, burst_r ,burst_c,count , j= 0, final4 , a , b , px , py;
    
    char finalword[16] = {0} , limit[16] = {0};//15+1 max word ki length ki
    
    
    int x[] = {0, 1, 1};//r d lrd
    
    
    int y[] = {1, 0, 1};
    
    
    int longestPositions[15][2];
    
    
    
    
    for(int i = 0; i < 3; i++){
    
    
    lencount = 0;
    
    char word[15 + 1] = {0};  
    
    int positions[15][2];    
    
    
    C = col;        
    R = row;
        
     

        while(lencount < 15){
        
        
        
            if (R < 0 || R >= rownum || C < 0 || C >= alpha_row || randomalphabet[R][C] == -1){
                break;}





            word[lencount] = 'a' + randomalphabet[R][C];//lowercasse cuz library mai sab lowercase
            
            
            positions[lencount][1] = C;
            
            positions[lencount][0] = R;
            
            
            lencount++;
            word[lencount] = '\0';  

            
            if (Indictionary(word)){
            
            
                if (lencount > len_tillnow){
                
                    len_tillnow = lencount;
                    
                    
                    
                    while(j < lencount){
                    
                    
                        limit[j] = word[j];
                        
                        
                         j++;}
                         
                         
                    limit[lencount] = '\0'; //terminate the longest word
                    
                    
                    for(int k = 0; k< lencount; k++){
                    
                    
                         px = positions[k][0];
                         
                         
                         py = positions[k][1];
                         
                        longestPositions[k][0] = px;
                        
                        
                        longestPositions[k][1] = py;
                      
               }
               
               
               
             }
             
             
             
             }

          
          
a = x[i];   

b = y[i] ;  





R = R + a;

C = C + b;



}
}

    
    if (len_tillnow > 0){//longest word found zero hoa tu phir tu kuch karna hi nai
    
    
    
        for(int q = 0; q < len_tillnow; q++){
        
          burst_r = longestPositions[q][0];
          
           burst_c = longestPositions[q][1];
           
           
            randomalphabet[burst_r][burst_c] = -1; 
            
            
            score++;//har bub ka
           
        }
        
       
                  
        while(count<15){
        
        
        finalword[count] = limit[count];
        count++;}
        
        
        
        
        
        if(outfile.is_open()){
        
            outfile << limit << endl;}
            
            
            
            
            
    }
    
    
    
    
    

    outfile.close();
}
 
 
void refill(){


 for(int a = 0 ; a< 2 ; a++){
        for(int b = 0 ; b<15 ;b++){
        
        
        if(randomalphabet[a][b]){
        
        
            randomalphabet[a][b] = GetAlphabet();
            
            
            }
            
          }
          
        }



}

void selfburstingstart(){

ofstream outfile("start.txt", ios::out | ios::app);
    
       
    
    char foundWords[60][16];
    
    int len_tillnow[60];
    
    int foundcount = 0, R , C , lencount, burst_r ,burst_c, final4 , duplicate , a,b; 
   
    int x[] = { -1, -1, 0, 1, 1, 1, 0, -1 };
    
    int y[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
    
    // arrays for row and col positions of each letters
    int wordPositionsRow[60][15];
    
    int wordPositionsCol[60][15];
    

   


    //only 2 rows 
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < alpha_row; j++) {
        
        
    
            
            for (int k = 0; k < 8; k++){
            
                 lencount = 0;
                 
                char word[16] = {0};
                
                 R = i;
                 C = j;
                 
                 
                int positionsRow[15];
                
                int positionsCol[15];
                
                

              
                while (lencount < 15) {
                
                
                    if (R < 0 || R >= rownum || C < 0 || C >= alpha_row || randomalphabet[R][C] == 99){
                    
                        break;}
                        

                    

                    
                    word[lencount] = 'a' + randomalphabet[R][C];//lc
                    
                    positionsRow[lencount] = R;
                    
                    positionsCol[lencount] = C;
                    
                    lencount++;
                    
                    
                    
                    
                    word[lencount] = '\0'; 



                  
                    if(Indictionary(word)){
                        
                        int duplicate= 0;//if alr found to avoid duplicates aik jesay hi burst baas 
                        
                        
                        for (int p = 0; p < foundcount; p++){
                        
                            int cmpResult = 0;
                            
                            int l = 0;
                            
                            // Compare foundWords[k] and word character-by-character
                            while (foundWords[p][l] != '\0' && word[l] != '\0'){
                            
                            
                                if (foundWords[p][l] < word[l]){
                                
                                
                                    cmpResult= -1;
                                    
                                    break;
                                    
                                } 
                                
                                else if(foundWords[p][l] > word[l]){
                                
                                
                                
                                  cmpResult = 1;
                                    
                                    break;
                                    
                                }
                                
                                l++;
                                
                                
                            }
                            
                            
                            if(cmpResult == 0 && foundWords[p][l] == '\0' && word[l] == '\0'){//duplicate tu nai
                                
                                
                                
                                duplicate = 1;
                                
                                break;
                                
                                
                            }
                            
                            
                        }
                        
                        
                        if(!duplicate && foundcount < 60){
                        
                        
                            
                           
                            for(int l = 0; l <= lencount; l++) {//store burst wala hi
                            
                            
                                foundWords[foundcount][l] = word[l];
                                
                                
                              }
                            
                            
                            len_tillnow[foundcount] = lencount;
                            
                            
                          
                            
                            
                            for (int l = 0; l < lencount; l++) {
                            
                            
                                wordPositionsRow[foundcount][l] = positionsRow[l];
                                
                                
                                wordPositionsCol[foundcount][l] = positionsCol[l];
                                
                                
                                }
                            
                            
                            foundcount++;
                            
                            
                      }
                        
                        
                     }
                     
                     
a = x[k];

b = y[k];

                  
R =R+ a;
                    
C = C +b;
                    
                    
                    
              }
                
                
                
          }
        
            
          }
        
        
        
        
        
        
        
}

    
    
    char temp[16];
    int templ , tempR,tempC;
    
    for(int i= 0; i< foundcount - 1; i++){
    
        for(int j =(i + 1); j < foundcount;j++){
        
            if(len_tillnow[i] < len_tillnow[j]){               
               
                for (int p= 0; p <= len_tillnow[j]  ; p++){//words
                
                
                    temp[p]= foundWords[i][p];
                    
                    foundWords[i][p]  = foundWords[j][p];
                    
                    foundWords[j][p] = temp[p];
                    
                    
            }
               
                templ=len_tillnow[i];//length
                
                len_tillnow[i] =  len_tillnow[j];
                
                len_tillnow[j]=  templ;

               
               
                for (int k = 0; k < len_tillnow[i]; k++){//positionz
                
                
                    tempR= wordPositionsRow[i][k];
                    
                    tempC =  wordPositionsCol[i][k];
                    

                    wordPositionsRow[i][k]= wordPositionsRow[j][k];
                    
                    wordPositionsCol[i][k] = wordPositionsCol[j][k];
                    

                    wordPositionsRow[j][k] =tempR;
                    
                    wordPositionsCol[j][k] = tempC;
                    
                    
                    
                    
               }
                
              }
            
      }
        
        
        
    }
    


    //sirf 4 karnaye 
    if(foundcount < 4){
    
    final4 = foundcount;
    
    
    }
    
    else{
    
    final4 = 4;
    
    
    }
   
   
   //burst karo ab bhai
    for(int i = 0; i < final4; i++){
        for(int j = 0; j < len_tillnow[i]; j++){
        
        
            int burst_r =wordPositionsRow[i][j];
            
            int burst_c = wordPositionsCol[i][j];
            
            
            randomalphabet[burst_r][burst_c] = 99; // Set to empty
            
        }

      
        score +=  len_tillnow[i];

    
        if (outfile.is_open()){
            
            outfile<< foundWords[i] << endl;
          
        }
        
        

        refill();
    
    
    }
    
    outfile.close();
    
    
    
}


// array jo declare kia tha usmaye store karwana using get alpha
void top2row(){


    for (int i = 0;i < rownum; i++){
        for (int j = 0; j < alpha_row; j++){
        
        if(i< 2 ){
        
        
             randomalphabet[i][j] = GetAlphabet(); 
             
            
            cout << randomalphabet[i][j];}
            
            
            else{
            
            
            
            assign_grid();
            
            
            }
            
           }
       }
     
    
    selfburstingstart();
    
}

                                                 
                                                                
void Pixels2Cell(int px, int py, int & cx, int &cy) {
}                                                          
void Cell2Pixels(int cx, int cy, int & px, int &py)
// converts the cell coordinates to pixel coordinates...
{
}



                                                            //MY FUNCTION

void shooteralpha(int csx, int csy, int cwidth = 60, int cheight = 60){


int widthbub , heightbub;

if(!bubbleshot){
    
    
    heightbub=csy+cheight - 50;//shooter kay upper bilkul
    
    
     widthbub=csx+(cwidth/ 2)- (awidth /2) + 30 ;//cwidth is in pixels red shooter ka , awidth bub ka width (awidth /2) say perfectly center hota warna thura off hota 
     
     
     
       DrawAlphabet((alphabets)shooterbub, widthbub ,heightbub,awidth, aheight );
       
       
        }



}



void DrawShooter(int sx, int sy, int cwidth = 60, int cheight = 60)

{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;
	

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, -1);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
	

	
	//glutSwapBuffers();
	shooteralpha(sx , sy);
	
    
  
  
  
}

void shooterrandom(){

shooterbub = GetAlphabet();

}

/*
* Main Canvas drawing function.
* */
void DisplayFunction() {
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim red and 1 means pure red and so on.
	//#if 0
	int x , y;
	
	
	glClearColor(1/*Red Component*/, 1.0/*Green Component*/,
		1.0/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors
	
	if(score > 18){
	glClearColor(1/*Red Component*/, 1.0/*Green Component*/,
		0.0/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors
	
	}
	
	if(score > 30){
	glClearColor(0/*Red Component*/, 1.0/*Green Component*/,
		1.0/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors
	
	}
	
	
	
        
	//write your drawing commands here or call your drawing functions...
	
	
	
	if(start){
        // Opening display
        DrawAlphabet( (alphabets)22,270, 400, awidth,aheight );
       
       DrawAlphabet( (alphabets)4,330, 400, awidth,aheight );
       
       
       DrawAlphabet( (alphabets)11,390, 400, awidth,aheight );
       
       DrawAlphabet( (alphabets)2,450, 400, awidth,aheight );
       
       DrawAlphabet( (alphabets)14,510, 400, awidth,aheight );
       
       DrawAlphabet( (alphabets)12,570, 400, awidth,aheight );
       
       DrawAlphabet( (alphabets)4,630, 400, awidth,aheight );
       
       
       
        DrawString(width / 2 - 400, height / 2 - 50, width, height, "PRESS/ MOUSE OR CLICK ANYWHERE ON SCREEN TO START PLAYING", colors[RED]);
        
        DrawString(width / 2 - 150, height / 2 - 100, width, height, "SCORE = 0 ", colors[RED]);
        
        DrawString(width / 2 - 250, height / 2 - 150, width, height, "BURST AS MANY AS U CAN TO GET MAXIMUM SCOREE", colors[BLUE_VIOLET]);
        
        DrawString(width / 2 - 250, height / 2 - 200, width, height, "SORRY IF GAME FAULTY......", colors[BLUE_VIOLET]);
    } 
    
    
    if(gamepop){
            
        DrawString(width / 2 - 400, height / 2 - 50, width, height, " ", colors[RED]);}
        
    else{
        
        
        
        
	if(!finished){
	
	
	 for(int i = 0; i <rownum; i++){
          for(int j= 0; j < 15;j++){
          
           if(randomalphabet[i][j] != 99){
           
             x= 10 +(j *60);//cwidth was upper 60 tu between alphas space
             
             y= height-120- (i* 60);//cheight was also 60 upper row ki height
             
             
            DrawAlphabet( (alphabets)randomalphabet[i][j],x, y, awidth,aheight );}
            
            
                 
              }
          }
          
          
          
    if(bubbleshot){
    
    
       DrawAlphabet( static_cast<alphabets>(shooterbub), static_cast<int>(x_pos), static_cast<int>(y_pos),awidth , aheight);} 
       

	
	
	}
	
      else{
        glClearColor(1/*Red Component*/, 1.0/*Green Component*/,
		1.0/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors 
	
	
       
        DrawAlphabet( (alphabets)6 ,270, 400, awidth,aheight );
       
       DrawAlphabet( (alphabets)0,330, 400, awidth,aheight );
       
       
       DrawAlphabet( (alphabets)12,390, 400, awidth,aheight );
       
       DrawAlphabet( (alphabets)4,450, 400, awidth,aheight );
       
       DrawAlphabet( (alphabets)14,510, 400, awidth,aheight );
       
       DrawAlphabet( (alphabets)21,570, 400, awidth,aheight );
       
       DrawAlphabet( (alphabets)4,630, 400, awidth,aheight );
       
        DrawAlphabet( (alphabets)17,690, 400, awidth,aheight );
        
        
       
        DrawString(width /2 - 80,height/ 2 -50, width, height, "Final Score:  " +Num2Str(score), colors[BLUE_VIOLET]);
	
	
	}
	
        
      DrawString(15, height - 20, width, height, "Fizza Hussain ", colors[BLUE]);
      
	DrawString(40, height - 42, width, height + 5, "Initial score = 0 | After Bursting in rows Score " + Num2Str(score), colors[BLUE_VIOLET]);
	
	if(countdown> 0){
	
	
        DrawString(width / 2 - 60, height - 25, width, height, "Time Left: " + Num2Str(countdown) + " secs", colors[RED]);
        
    } 
    
    else{
        DrawString(width / 2 - 30, height - 25, width, height, "Time's Up!", colors[RED]);
    }}
      
		
		
		

	// #----------------- Write your code till here ----------------------------#
	//DO NOT MODIFY THESE LINES
	DrawShooter((width / 2) - 35, 0, bwidth, bheight);
	glutSwapBuffers();
	//DO NOT MODIFY THESE LINES..
}

/* Function sets canvas size (drawing area) in pixels...
*  that is what dimensions (x and y) your game will have
*  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
* */
void SetCanvasSize(int width, int height) {
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.*/
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
* is pressed from the keyboard
*
* You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
*
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
*
* */

void NonPrintableKeys(int key, int x, int y) {
      if(!finished){
        
   

	if (key == GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
		// what to do when left key is pressed...

	}
	else if (key == GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {

	}
	else if (key == GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {
	}
	else if (key == GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	* this function*/
	/*
	glutPostRedisplay();
	*/
}
}
/*This function is called (automatically) whenever your mouse moves witin inside the game window
*
* You will have to add the necessary code here for finding the direction of shooting
*
* This function has two arguments: x & y that tells the coordinate of current position of move mouse
*
* */

void MouseMoved(int x, int y) {
	//If mouse pressed then check than swap the balls and if after swaping balls dont brust then reswap the balls
if(!finished){
         
   }
}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
*
* You will have to add the necessary code here for shooting, etc.
*
* This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
* x & y that tells the coordinate of current position of move mouse
*
* */



void MouseClicked(int button, int state, int x, int y) {

 float m, c , dx , dy , b , cal , u , s;
 
 
 int a;
 
 

  if(!finished){
  
  
	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
	{
		if (state == GLUT_UP)
		{
		
		if (start) {
		
		
                start = false;
                
                }
                
                if(gamepop){
                
                gamepop = false;
                
                }
                
                else {
                
		if(!bubbleshot){
		
		
            mouseclickx = x - (awidth/ 2);//target positions
            
            
            a = height - y;//openGl starts from left bottom
            
            
           mouseclicky = a - (aheight / 2);    
           
           b = 0.01;
            
            x_pos = sX + (bwidth / 2) - (awidth / 2);//shooter ki position
            
            
            y_pos = sY + bheight;
            
            
            
            //allah kay wastay fizza yahan straight line calc dekho kia karni sa horaha infinty search karo kesay declare kartay baar baar error isi ka ata
            
            
            //y = mx +  c
            //gradient
            
            dy =mouseclicky - y_pos;// y 
            
            u = x_pos - y_pos;
            
            
            dx = mouseclickx - x_pos; // x cord
            
            
            if(dx == 0){ 
            
            m = INFINITY;//vertical line
            posy_inc = 1.0;
            cout <<"straight";
          
          }
            
            
          else {
            
            m = dy / dx;
            
          cal = m* x_pos;

          c = y_pos - m* x_pos; // use tu nai hai but why not
          
          
          
          if(dx > 0){//thru  signs
          
          posx_inc = 5.0;//right
          
          
            }
             
              
              else{
          
          posx_inc = -5.0;//left
          
          dx = -dx;//convert 
          
          }
          
                 
           if(dy != 0){
          
          
          s = 5.0;
          
          posy_inc = 5.0 * (dy / dx + b);//ratio between x and y increments
          
          
            }
               
            else{
               
               
               posy_inc = 5.0;}

                

          }
                          

              //dy is positive for upward movement
          
          if(dy < 0){
          
          
          dy = -dy;//convert 
          
          
            }        
                 
              
}       
            
          
          
    
  bubbleshot = true;}
    
     }
      
    
}


	else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
	{

	}
	
	glutPostRedisplay();
  }
}



bool touch(){


int bubxnow ,bubynow , a, l;

float disx , disy , dist;



    for(int i = 0; i < rownum; i++){
    for(int j = 0; j < alpha_row; j++){
    
        if (randomalphabet[i][j] != 99){
        
             l = 0;
              bubynow = height - 101 - i *aheight; //pixel say again 
              
             bubxnow = 10 + j * awidth;
            
            a = bubxnow;
              


                 disx = (y_pos + aheight/ 2)-(bubynow+ aheight/ 2);//shooter - bubble now
                 
                 
                  disy = (x_pos+ awidth/ 2) -(bubxnow+ awidth /2);
                
                
               dist= sqrt( (disx*disx) + (disy*disy) );//formula distance ka hi
               
               
               
                if(dist <= awidth + 3){
                
                l++;
                    return true;
                    
                    }
                    
                    
           }
           
      
    }
    
   }
   
      
return false;

    
}


void stick(){


const int x[] = {0, 1, 1, 1};
int l , m;
const int y[] = {-1, -1, 0, 1};
    
    

int row,col, a,b , c , s , p , q;


    
    
row =  static_cast<int>((height- 110 - y_pos - aheight  / 2) / aheight);

bubbleshot = false;    
    
col =  static_cast<int>((x_pos+ awidth/ 2) /  awidth);


    
    
    //dont go bahir
    if(!(row >= 0)){
    
    row = 0;
    
    }
    
     else if(col < 0){
    
    
    col = 0;}
    
    else if(!(row < rownum)){ 
    
    
     row = rownum - 1;
    
     }
    
    
    if(col >= alpha_row){
    
     col = alpha_row - 1;
    
      }
    
    
    
    if(randomalphabet[row][col] == 99){
    
    
        randomalphabet[row][col] = shooterbub;
        
        burst(row, col);
         s = 0;
         cout <<"s?";
        
        shooterbub = GetAlphabet();//new alph on shooter
        
        
        
    }
 
    else{//sath
    
    
    
    
        for(int i= 0; i< 4; i++){
           
            
            b = col + x[i];     
             c = 0;
             l = 1;// declare for checking faults
             a = row + y[i];          
            
            if (a >= 0 && b >= 0 && a < rownum && b < alpha_row){//in bound
            
            cout << "masla?" << endl;
            
            
              if (randomalphabet[a][b] == 99){
            
            randomalphabet[a][b] =shooterbub;
            cout << "debug check" << endl;
            
            
                    
                     break;}
            
            
            }
           
           
           }
           
 }


burst(row, col);
shooterbub = GetAlphabet();




}

    
  

/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
* is pressed from the keyboard
//* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
* */
void PrintableKeys(unsigned char key, int x, int y) {
	if (key == KEY_ESC/* Escape key ASCII*/) {
		exit(1); // exit the program when escape key is pressed.
	}
}

/*
* This function is called after every 1000.0/FPS milliseconds
* (FPS is defined on in the beginning).
* You can use this function to animate objects and control the
* speed of different moving objects by varying the constant FPS.
*
* */


void timerchaljaopls(int m){

if(!finished){//timer countdown kay liye
     
  if(countdown > 0){//yes tu minus
  
       countdown--;}
       
       
       
else{


  finished = true;
  
  
    }
  
  
   glutPostRedisplay();
  glutTimerFunc(1000.0 , timerchaljaopls, 0);
  
}

}             
                
                
                
void Timer(int m){

int a , b , store ;
//bubble shot kay liye
 if(bubbleshot){
 
          x_pos = x_pos +posx_inc;
          
          a = x_pos;
          
           y_pos = y_pos+ posy_inc;
           
           
          if (x_pos >=   width -awidth ){
          
                 posx_inc = -posx_inc;//reflect
                 
                 
            }
            
             if (x_pos<= 0){
          
                 posx_inc = -posx_inc;//reflect
                 
                 
            }
            
            
            
            store = y_pos + aheight;
          
            if(touch()  || y_pos  > height - aheight ){
             cout << "?"; 
            
                b = a;
                
                stick();
                
                burst(x_pos, y_pos);
                   
                
          }
          
          
      }
      
              
   
   
   else{
                        // Handle collision with occupied slot if necessary
              }
  
  
  
    glutPostRedisplay();
    glutTimerFunc(1000.0 / FPS, Timer, 0);
 
}



void music(const string &filePath){


if(SDL_Init(SDL_INIT_AUDIO) < 0){


      cerr << "SDL ERROR " << SDL_GetError() << endl;
      
      return ;


}


    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
    
        cerr << "SDL_mixer could not initialize! Mix Error: " << Mix_GetError() << endl;
        
        
        SDL_Quit();
        
        return ;
        
    }

    // Load the music file
    Mix_Music* music = Mix_LoadMUS(filePath.c_str());
    
    
    if (!music) {
    
    
        cerr << "Failed to load music file: " << filePath << ". Mix Error: " << Mix_GetError() << endl;
        
        
        Mix_CloseAudio();
        
        SDL_Quit();
        
        return;
        
        
    }

    //in a loop
    Mix_PlayMusic(music, -1);
    
    Mix_VolumeMusic(70); 
    
    
    cout << endl << "Background music playing: " << filePath << endl;


}



/*
* our gateway main function
* */
int main(int argc, char*argv[]) {

        InitRandomizer(); // seed the random number generator...

	//Dictionary for matching the words. It contains the 370099 words.
	dictionary = new string[dictionarysize]; 
	ReadWords("words_alpha.txt", dictionary); // dictionary is an array of strings
	//print first 5 words from the dictionary
	for(int i=0; i < 5; ++i)
		cout<< " word "<< i << " =" << dictionary[i] <<endl;
		
	//Write your code here for filling the canvas with different Alphabets. You can use the Getalphabet function for getting the random alphabets

        top2row();
        
        sX = (width / 2) - (bwidth / 2);
        
        
        shooterrandom();
        
        
        
         music ("music.mp3");

	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("ITCs Word Shooter"); // set the title of our game window
	//SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	RegisterTextures();
	glutDisplayFunc(DisplayFunction); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse

	//// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0 , timerchaljaopls, 0);
	glutTimerFunc(1000.0/FPS, Timer, 0);

	//// now handle the control to library and it will call our registered functions when
	//// it deems necessary...

	glutMainLoop();
	
	Mix_HaltMusic ();
	
        Mix_CloseAudio ();
        SDL_Quit();

	return 1;
}
#endif /* */
