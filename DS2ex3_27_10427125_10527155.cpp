//********************************************************************/
//*** Hash in Exercise 03 by Wu, Y.H.@CYCU-ICE
//********************************************************************/


#include <stdio.h>
#include<bits/stdc++.h>
#include <iostream>	                                  // cout, endl
#include <fstream>                                    // open, is_open
#include <string>                                      // string
#include <vector>                                     // vector
#include <cstdlib>                                     // atoi, system
#include <iomanip>                                   // setw
#include <new>                                      // new, delete
#include <stdio.h>
#include <string.h>

#include <math.h>

using namespace std;

typedef struct student {
	char id[10];
	char name[10];
	unsigned char	score[6];
	float avg;
	int value ;
}	student;

struct hashtable {
	int value ;
	char id[10];
	char name[10];
	float avg;
	bool empty ;
	hashtable():empty(true) {} ;
};

void Txt2Bin( string input ) {
	FILE *in ;
	FILE *out ;
	unsigned char score[6];
	char id[10] ;
	char name[10] ;
	float avg ;
	int temp[6] ;
	string txtname = "input" + input + ".txt" ;
	string binname = "input" + input + ".bin" ;

	in = fopen(txtname.c_str(), "r");
	out = fopen(binname.c_str(), "wb");
	rewind(in);

	while(1) {
		fscanf(in, "%s %s %d %d %d %d %d %d %f", id, name, &temp[0], &temp[1], &temp[2], &temp[3], &temp[4], &temp[5], &avg);
		if ( id [0] != '\0' ) {
			for ( int i = 0 ; i < 6 ; i++)
				score[i] = (unsigned char)temp[i];
			fwrite(id, 10, sizeof(char), out);
			fwrite(name, 10, sizeof(char), out);
			fwrite(score, sizeof(unsigned char), sizeof(score)+2, out);
			fwrite(&avg, 1, sizeof(float), out);
		}
		for ( int i = 0 , j = 0 ; i < 10 ; i++) {
			id[i] = '\0' ;
			name[i] = '\0' ;
			if ( j < 6 )
				temp[j] = '\0' ;
		}
		avg = '\0' ;
		if(feof(in)) {
			break;
		} // if

	} // while

	fclose(in);
	fclose(out);
} // Txt2Bin()

bool ReadBin(vector<student> &data, string fileName) { // ±NbinÀÉªº¸ê®Æpushback¶ivector data¤¤
	string binfileName, txtfileName ;
	binfileName = "input"+ fileName +".bin";
	txtfileName = "input" + fileName + ".txt" ;
	fstream inFile;
	student temp;
	int total = 0 ;

	inFile.open(binfileName.c_str(), fstream::in | fstream::binary);
	if (!inFile.is_open()) {
		inFile.open( txtfileName.c_str() ) ;
		if ( !inFile.is_open() ) {
			cout << "file does not exist!" ;
			return false ;
		} // if

		else { // bin¤£¦s¦b, ¦ıtxt¦s¦b
			Txt2Bin( fileName ) ;
			inFile.close() ;
			inFile.open(binfileName.c_str(), fstream::in | fstream::binary);
		} // else
	} // if

	inFile.seekg (0, inFile.end);
	total = inFile.tellg() / (sizeof(student)-4);
	inFile.seekg (0, inFile.beg);
	for ( int i = 0 ; i < total ; i++) {
		inFile.read((char *)&temp, sizeof(temp)-4);
		data.push_back(temp);
	} // for
	inFile.close();
	if ( data.size() ) {
		return true ;
	} // if

	return false ;
} // bool ReadBin()

int hashsize ( int total ) {
	int count = 0;
	bool finded = false;
	total = total * 1.2 ;

	while (finded == false) {
		total++;
		count = 0;
		for( int i = 2 ; i <= total-1 ; i++ ) {
			if(total%i==0)
				count++;
		} // for

		if(count == 0)
			finded = true;
	} // while

	return total;
} // §ä¥X¤j©ó¸ê®ÆÁ`µ§¼Æ*1.2ªº³Ì¤p½è¼Æ

int higheststep ( int total ) {
	int count = 0;
	bool finded = false;
	total = total / 3 ;

	while (finded == false) {
		total++;
		count = 0;
		for( int i = 2 ; i <= total-1 ; i++ ) {
			if(total%i==0)
				count++;
		} // for

		if(count == 0)
			finded = true;
	} // while

	return total;
} // §ä¥X¤j©ó¸ê®ÆÁ`µ§¼Æ/3ªº³Ì¤p½è¼Æ

void value( vector<student> &data , int size ) {
	int i = 0, j = 0 ;
	int value = 1 ;
	for ( i = 0 ; i < data.size() ; i++ ) {
		value = 1 ;
		for ( j = 0 ; j < strlen(data[i].id) ; j++ ) {
			value = ( value * data[i].id[j] ) % size ;
		} // for() ¨C¤@²ÕªºASCII¬Û­¼%size
		data[i].value = value ; // ±Nhash value¦s¶ivector
	} // for()

} //value()

void copy(student &data, hashtable &table) {
	table.value = data.value ;
	strcpy(table.id,data.id);
	strcpy(table.name,data.name);
	table.avg = data.avg;
	table.empty = false ;
}



/*
void hash( vector<student> &data , vector<hashtable> &table , float &exist ) {
int i = 0 ,default_pos = 0, count = 0;

//int increase = 0;
int counter_collisions = 0;// ç¢°æ’æµæ°´è™Ÿ
int exist1 = 0;
int exist2 = 0;


for ( i = 0 ; i < data.size() ; i++ ) {
default_pos = data[i].value ;
for ( count = 0 ; count < table.size() ; count++ ) {

if (table[default_pos].empty) { // tableæ˜¯ç©ºçš„
copy(data[i],table[default_pos]);

exist1++;
// cout << "exist1 "<< exist1 <<endl ;
break;
} // tableæ˜¯ç©ºçš„
else { // æ­¤ä½å­å·²æ»¿
int qp_pos = default_pos + pow( ++counter_collisions, 2 );

while( qp_pos < table.size() ){ // æ²’è¶…établesizeç›´æ¥æ‰¾ä¸‹ä¸€å€‹
qp_pos = default_pos + pow( ++counter_collisions, 2 ) ;
exist2++ ;
//cout << "exist2 "<< exist2 <<endl ;
if( qp_pos > table.size() ) default_pos = 0 ;
}

/*if( qp_pos > table.size() ) {// è¶…établesizeè¦å›åˆ°0é–‹å§‹æª¢æŸ¥
// j = j - table.size();
default_pos = 0 ;

}
} // else()
} // for() æ‰¾table
counter_collisions = 0;

} // for() è³‡æ–™ä¸€ç­†ä¸€ç­†æ”¾
exist = exist1+exist2 ;
} // hash()

*/


 
void hash( vector<student> &data , vector<hashtable> &table , float &exist ,float &notexist) {
	int i = 0 ,j = 0, count = 0;

	//int increase = 0;
  	int collisions = 0;// ¸I¼²¬y¤ô¸¹ 
	int exist1 = 0;
	int exist2 = 0;
	

	
	for ( i = 0 ; i < data.size() ; i++ ) {
		j = data[i].value ;
		cout<< "J: "<< j <<data[i].name  <<endl;
		cout<< " "<< data[i].value  <<endl;
		int old_idx = 0 ;
		for ( count = 0 ; count < table.size() ; count++ ) {
			if (table[j].empty) { // table¬OªÅªº
			    cout<< "COPY: "<<data[i].name  <<endl<<"_____________________" <<endl; 
				copy(data[i],table[j]);

		        
	
				exist1++;
			//	cout << "exist1   "<< exist1 <<endl ;
				break;
			} // table¬OªÅªº
			else { // ¦¹¦ì¤l¤wº¡
			
		        cout<< "º¡ " <<endl ; 
			
			
                collisions++; 
                cout << " j + pow(collisions,2) :"<<  data[i].value + pow(collisions,2) <<endl ; 
                
                
				if(data[i].value + pow(collisions,2) < table.size() ){ // ¨S¶W¹Ltablesizeª½±µ§ä¤U¤@­Ó
					j = data[i].value + pow(collisions,2) ;
					cout<< "·sj " << j <<endl ; 
					exist2++ ;
					//cout << "exist2   "<< exist2 <<endl ;
				}
				
				else {// ¶W¹Ltablesize
					// j = j - table.size();
					j = data[i].value + pow(collisions,2) ;
					
					j =  j  % table.size() ;
					cout<< "T SIZE " <<  table.size() <<endl ;
					cout<< "¶W¹L«á¨ú¾l¼Æj " << j <<endl ; 
					
				}
			//	system("pause");
				
				
			} // else()
		} // for() §ätable
		collisions = 0;
	
	} // for() ¸ê®Æ¤@µ§¤@µ§©ñ
	exist = exist1+exist2 ;
} // hash()

void doublehash( vector<student> &data , vector<hashtable> &table, float & compare ) {
	int i = 0 ,j = 0, count = 0;
	int step = 1, highstep = 0 ;
	highstep = higheststep( data.size() ) ;

	for ( i = 0 ; i < data.size() ; i++ ) {
		compare ++ ;
		j = data[i].value ;
		if (table[j].empty) { // table¬OªÅªº
			copy(data[i],table[j]);
		} // table¬OªÅªº
		else { // ¦¹¦ì¤l¤wº¡
            step = 1 ;
			for ( count = 0 ; count < strlen(data[i].id) ; count++ ) {
				step = step * data[i].id[count] ;
				step = step % highstep ;
			} // for()
			step = highstep - step ;

            j = j + step ;
            j = j % table.size() ;
            compare ++ ;
			while( !table[j].empty ) {
				j = j + step ;
				j = j % table.size() ;
				compare ++ ;
			} // while

			if( table[j].empty ) {
				copy(data[i],table[j]);
			} // if

		} // else()

	} // for() ¸ê®Æ¤@µ§¤@µ§©ñ

} // doublehash()

void calculatenotexist(float &notexist, vector<hashtable> table, int size) {
	int i = 0 , j = 0 ;
	int collisions = 1;
	for ( i = 0 ; i < size ; i++ ) {
		j = i ;
		while (!table[j].empty) {
			notexist++;
			j = j+ pow(collisions,2) ;
			//j++;
			if( j > size-1 )
				j = 0 ;
		} // while()
	} // for()
} // notexist()


bool isPrime (int n)
{
    if (n == 2 || n == 3)
        return true;
    if (n == 1 || n % 2 == 0)
        return false;
    for (int i = 3; i * i <= n; i += 2)
        if (n % i == 0)
            return false;
    return true;
}
/*
 * Finding next prime size of the table
 */
int nextPrime(int n)
{
    if (n <= 0)
        n == 3;
    if (n % 2 == 0)
        n++;
    for (; !isPrime( n ); n += 2);
    return n;
}










void taskone() {
	vector<student> data;
	vector<hashtable> table;
	string fileName ;
	int size ;
	float exist = 0 , notexist = 0 ;
	cout << "½Ğ¿é¤JÀÉ®×¦WºÙ\n" ;
	cin >> fileName ;
	while ( fileName != "0" ) {  				// ÅªbinÀÉ
		if (ReadBin(data,fileName))
			break;
		cout << "½Ğ¿é¤JÀÉ®×¦WºÙ\n";
		cin >> fileName ;
	}

	size = hashsize(data.size());				//
	table.resize(size); 						//±Nhashtable¤j¤p­«¾ã
	value ( data, size ) ; 					// ºâ¥X¨C¦ì¾Ç¥ÍªºÂø´ê­È¨Ã¦s¤Jdata.value¤¤
	hash ( data, table, exist,notexist) ;				// hash¤JÂø´êªí
	
	//calculatenotexist(notexist, table, size) ;
	notexist = notexist / size ;
	//cout << "exist:   \n" << exist << "_   "<< data.size() << "\n"; 
	exist = exist / data.size();
	
	
	//////////////////////////¶}©l¼gÀÉ

	fileName = "quadratic" + fileName + ".txt" ;
	ofstream outFile(fileName.c_str()) ;
	if ( outFile.fail() )
		cout << "error" ;
	else {
		outFile << " --- Hash Table X --- (Quadratic probing)\n";
		for ( int i = 0 ; i < table.size() ; i++ ) {
			outFile << "[" << i << "]\t" ;
			if ( !table[i].empty ) { //¦¹table¦³¸ê®Æ
				outFile << table[i].value << ",\t" ;
				outFile << table[i].id << ",\t" ;
				outFile << table[i].name << ",\t" ;
				outFile << table[i].avg  ;
			}
			outFile << "\n" ;
		} // for()
	}// else()

	outFile.close();

	if ( fileName != "linear0.txt" ) {
		cout << "Hash Table X has been created.\n" ;
		cout << "unsuccessful search: " << notexist << " comparisons on average\n" ;
		cout << "successful search: " << exist << " comparisons on average\n" ;
	} // if

} // taskone()

void tasktwo() { // tasktwo : build double hash
	vector<student> data;
	vector<hashtable> table;
	string fileName ;
	int size ;
	float compare = 0 ;
	cout << "½Ğ¿é¤JÀÉ®×¦WºÙ\n" ;
	cin >> fileName ;
	while ( fileName != "0" ) {  				// ÅªbinÀÉ
		if (ReadBin(data,fileName))
			break;
		cout << "½Ğ¿é¤JÀÉ®×¦WºÙ\n";
		cin >> fileName ;
	} // while

	size = hashsize(data.size());				//
	table.resize(size); 						//±Nhashtable¤j¤p­«¾ã
	value ( data, size ) ; 					// ºâ¥X¨C¦ì¾Ç¥ÍªºÂø´ê­È¨Ã¦s¤Jdata.value¤¤
	doublehash ( data, table, compare ) ;				// hash¤JÂø´êªí
	compare = compare / data.size() ;
	//////////////////////////¶}©l¼gÀÉ

	fileName = "double" + fileName + ".txt" ;
	ofstream outFile(fileName.c_str()) ;
	if ( outFile.fail() )
		cout << "error" ;
	else {
		outFile << " --- Hash Table Y --- (double hashing)\n";
		for ( int i = 0 ; i < table.size() ; i++ ) {
			outFile << "[" << i << "]\t" ;
			if ( !table[i].empty ) { //¦¹table¦³¸ê®Æ
				outFile << table[i].value << ",\t" ;
				outFile << table[i].id << ",\t" ;
				outFile << table[i].name << ",\t" ;
				outFile << table[i].avg  ;
			}
			outFile << "\n" ;
		} // for()
	}// else()

	outFile.close();

	if ( fileName != "double0.txt" ) {
		cout << "Hash Table Y has been created.\n" ;
		cout << "successful search: " << compare << " comparisons on average\n" ;
	} // if

} // tasktwo()

int main() {
	int task = 4 ;
	while( task != 0 ) {
		cout << endl << "**********1.Single Hash***************" ;
		cout << endl << "**********2.Double Hash***************" ;
		cout << endl << "input a task(0,1,2) :" ;
		cin >> task ;
		if( task == 1 ) {
			taskone() ;
		} // if
		else if ( task == 2 ) {
			tasktwo() ;
		} // else if
	} // while

	return 0 ;
} // main()
