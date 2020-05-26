//********************************************************************/
//*** Hash in Exercise 03 by Wu, Y.H.@CYCU-ICE
//********************************************************************/
//10427125  �h�o   10527155 ���§�

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

int StringToInt( string data ) {
    int val ;
    istringstream is( data ) ;
    is >> val ;
    return val ;
} // StringToInt
float StringToFloat ( string data ) {
    float val ;
    istringstream is( data ) ;
    is >> val ;
    return val ;
} // StringToFloat
void Txt2Bin( string input ) {
    ifstream infile ;
    FILE *out ;
    unsigned char score[6];
    string id ;
    string name ;
    string data ;
    float avg ;
    int temp[6] ;
    string txtname = "input" + input + ".txt" ;
    string binname = "input" + input + ".bin" ;
    infile.open( txtname.c_str() ) ;
    out = fopen(binname.c_str(), "wb");
    while( getline( infile, id, '\t' ) ) {
        getline( infile, name, '\t' ) ;
        getline( infile, data, '\t' ) ;
        temp[0] = StringToInt( data ) ;
        getline( infile, data , '\t' ) ;
        temp[1] = StringToInt( data ) ;
        getline( infile, data, '\t' ) ;
        temp[2] = StringToInt( data ) ;
        getline( infile, data, '\t' ) ;
        temp[3] = StringToInt( data ) ;
        getline( infile, data, '\t' ) ;
        temp[4] = StringToInt( data ) ;
        getline( infile, data, '\t' ) ;
        temp[5] = StringToInt( data ) ;
        getline( infile, data, '\n' ) ;
        avg = StringToFloat( data ) ;
        if ( id [0] != '\0' ) {
            for ( int i = 0 ; i < 6 ; i++)
                score[i] = (unsigned char)temp[i];
            fwrite(id.c_str(), 10, sizeof(char), out);
            fwrite(name.c_str(), 10, sizeof(char), out);
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
    } // while
    infile.close() ;
    fclose(out);
} // Txt2Bin()
bool ReadBin(vector<student> &data, string fileName) { // �Nbin�ɪ����pushback�ivector data��
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

		else { // bin���s�b, ��txt�s�b
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
} // ��X�j�����`����*1.2���̤p���

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
} // ��X�j�����`����/3���̤p���

void value( vector<student> &data , int size ) {
	int i = 0, j = 0 ;
	int value = 1 ;
	for ( i = 0 ; i < data.size() ; i++ ) {
		value = 1 ;
		for ( j = 0 ; j < strlen(data[i].id) ; j++ ) {
			value = ( value * data[i].id[j] ) % size ;
		} // for() �C�@�ժ�ASCII�ۭ�%size
		data[i].value = value ; // �Nhash value�s�ivector
	} // for()

} //value()

void copy(student &data, hashtable &table) {
	table.value = data.value ;
	strcpy(table.id,data.id);
	strcpy(table.name,data.name);
	table.avg = data.avg;
	table.empty = false ;
}






void hash( vector<student> &data , vector<hashtable> &table , float &exist ) {
	int i = 0 ,j = 0, count = 0;

	//int increase = 0;
  	int collisions = 0;// �I���y����
	int exist1 = 0;
	int exist2 = 0;



	for ( i = 0 ; i < data.size() ; i++ ) {
		j = data[i].value ;
		int old_idx = 0 ;
		for ( count = 0 ; count < table.size() ; count++ ) {
			if (table[j].empty) { // table�O�Ū�
				copy(data[i],table[j]);
				exist1++;

				break;
			} // table�O�Ū�
			else { // ����l�w��

                collisions++;

				if(data[i].value + pow(collisions,2) < table.size() ){ // �S�W�Ltablesize������U�@��
					j = data[i].value + pow(collisions,2) ;

				}

				else {// �W�Ltablesize
					j = data[i].value + pow(collisions,2) ;
					j =  j  % table.size() ;

				}
				exist2++ ;
			//	system("pause");


			} // else()
		} // for() ��table
		collisions = 0;

	} // for() ��Ƥ@���@����
	exist = exist1+exist2 ;
} // hash()

void doublehash( vector<student> &data , vector<hashtable> &table, float & compare ) {
	int i = 0 ,j = 0, count = 0;
	int step = 1, highstep = 0 ;
	highstep = higheststep( data.size() ) ;

	for ( i = 0 ; i < data.size() ; i++ ) {
		compare ++ ;
		j = data[i].value ;
		if (table[j].empty) { // table�O�Ū�
			copy(data[i],table[j]);
		} // table�O�Ū�
		else { // ����l�w��
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

	} // for() ��Ƥ@���@����

} // doublehash()

void calculatenotexist(float &notexist, vector<hashtable> table, int size) {

	int i = 0 , j = 0 ;
	int collisions = 0;
	int count = 0;
	for ( i = 0 ; i < size ; i++ ) {
		j = i ;
		count = j ;

        if(table[j].empty){
		}

		while(!table[j].empty) {
            collisions++;
            notexist++;

			if( (count  + pow(collisions,2)) < table.size() ){ //�S�W�L

		    	j =count + pow(collisions ,2) ;
			}

			else{

		      	j = count + pow(collisions ,2) ;
		      	j =  j  % table.size() ;
			}




		}

			 collisions = 0 ;





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
	cout << "�п�J�ɮצW��\n" ;
	cin >> fileName ;
	while ( fileName != "0" ) {  				// Ūbin��
		if (ReadBin(data,fileName))
			break;
		cout << "�п�J�ɮצW��\n";
		cin >> fileName ;
	}

	size = hashsize(data.size());				//
	table.resize(size); 						//�Nhashtable�j�p����
	value ( data, size ) ; 					// ��X�C��ǥͪ�����Ȩæs�Jdata.value��
	hash ( data, table, exist) ;				// hash�J�����

	calculatenotexist(notexist, table, size) ;

	notexist = notexist / size ;

	exist = exist / data.size();


	//////////////////////////�}�l�g��

	fileName = "quadratic" + fileName + ".txt" ;
	ofstream outFile(fileName.c_str()) ;
	if ( outFile.fail() )
		cout << "error" ;
	else {
		outFile << " --- Hash Table X --- (Quadratic probing)\n";
		for ( int i = 0 ; i < table.size() ; i++ ) {
			outFile << "[" << i << "]\t" ;
			if ( !table[i].empty ) { //��table�����
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
		cout << "unsuccessful search: " <<  setprecision(5)<<notexist << " comparisons on average\n" ;
		cout << "successful search: " << setprecision(5)<< exist << " comparisons on average\n" ;
	} // if

} // taskone()

void tasktwo() { // tasktwo : build double hash
	vector<student> data;
	vector<hashtable> table;
	string fileName ;
	int size ;
	float compare = 0 ;
	cout << "�п�J�ɮצW��\n" ;
	cin >> fileName ;
	while ( fileName != "0" ) {  				// Ūbin��
		if (ReadBin(data,fileName))
			break;
		cout << "�п�J�ɮצW��\n";
		cin >> fileName ;
	} // while

	size = hashsize(data.size());				//
	table.resize(size); 						//�Nhashtable�j�p����
	value ( data, size ) ; 					// ��X�C��ǥͪ�����Ȩæs�Jdata.value��
	doublehash ( data, table, compare ) ;				// hash�J�����
	compare = compare / data.size() ;
	//////////////////////////�}�l�g��

	fileName = "double" + fileName + ".txt" ;
	ofstream outFile(fileName.c_str()) ;
	if ( outFile.fail() )
		cout << "error" ;
	else {
		outFile << " --- Hash Table Y --- (double hashing)\n";
		for ( int i = 0 ; i < table.size() ; i++ ) {
			outFile << "[" << i << "]\t" ;
			if ( !table[i].empty ) { //��table�����
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
