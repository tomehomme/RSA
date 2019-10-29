#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>


using namespace std;


void encryption(int n, int e);
void decryption(int n, int e, int phi);

// simple gcd algorithm
int gcd(int a,int b){
    if (b == 0){ 
        return a;
    } 
    else{ 
        return gcd(b,a%b);
    } 
}
// check for p and q prime
bool isPrime(int num){
    bool flag=true;
    for(int i = 2; i <= num / 2; i++) {
       if(num % i == 0) {
          flag = false;
          break;
       }
    }
    return flag;
}

// Breaks RSA - determines p and q, given n
int Phi (int n){
    int p = 1;
    int q = 1;
    for(int i = 2; i <= n; ++i){
            if(n % i == 0 && isPrime(i)){
                p = i;
                q = n/p; 
            }    
    }
    // calculate phi
    int phi = (p-1)*(q-1);
    return phi;
}

// turns message's characters into the schema numbers
string lettersToNum(string filename){
    ifstream inputs;
    inputs.open(filename);
    if (!inputs.is_open()){
        cout << "could not open file.. exiting" << endl;
        return "";
    }
    vector<int>characters;
    char character;
    while (inputs.get(character)){
        characters.push_back(character);
    }
    inputs.close();
    
    string outfile = "messageNums.txt";
    ofstream output;
    output.open(outfile);
    for (int i = 0; i < characters.size(); i++){
        if (characters.at(i) == 32){
            output << 28 << " ";
        } else {
            output << characters[i]-63 << " ";
        }
    }
    output.close();
    return outfile;
}

// turns schema numbers into message letters
void numtoLetters(string filename){
    ifstream inputs;
    inputs.open(filename);
    if (!inputs.is_open()){
        cout << "could not open file.. exiting" << endl;
        return;
    }

    vector<char>characters;
    int character;
    // convert to ascii characters
    while (inputs >> character){
        if (character == 28){
            characters.push_back(' ');
        } else {
            // cout << character << " ";
            characters.push_back(character+63);
        }
    }
    inputs.close();

    // output file for message:
    // cout << "enter the conversion file: " << endl;
    // string outfile;
    // cin >> outfile;
    ofstream output;
    output.open("decrypted.txt");
    if (!output.is_open()){
        cout << "could not open file.." << endl;
        return;
    }

    for (int i = 0; i < characters.size(); i++){
        // cout << characters[i] << " ";
        output << characters[i];
    }
    output.close();
}



int main(){
    /*
    Input:
        a) Valid public key;
        b) Choice of encryption or decryption;
        Input for encryption:
        c1) English text (in a text file). You can use well known quotes.
        Input for decryption:
        c2) Cipher text (in a text file).
    Output:
        For encryption: cipher text (written into a text file).
        For decryption: decoded message (written into a text file).
    */

    cout << "Entering public keys.." << endl;
    cout << "Enter the value of 'n': " << endl;
    int n = 0;
    cin >> n;
    int phi = Phi(n);
    cout << phi << endl;

    cout << "Enter the value of 'e', the encryption key (e must be coprime to phi(n)): " << endl;
    int e = 2;
    cin >> e;
    cout << e << endl;
    
    while (gcd(e,phi) != 1){
        cout << "Enter the value of 'e', the encryption key (e must be coprime to phi(n)): " << endl;
        cin >> e;
    }

    cout << "Would you like to encrypt a message, or decrypt a message?" << endl;
    string choice;
    cin >> choice;

    if (choice == "encrypt"){
        encryption(n,e);
    } else {
        decryption(n,e,phi);
    }    
    return 0;
}


void encryption(int n, int e){
    cout << "Enter the name of the text file you would like to encrypt: " << endl;
    string filename;
    cin >> filename;

    // convert ascii to numbers
    string outfile = lettersToNum(filename);
    // cout << "reading from: " << outfile << endl;
    ifstream file;
    file.open(outfile);
    if (!file.is_open()){
        cout << "Could not open file..exiting" << endl;
        return;
    }

    // file is in numbers already
    vector<int> messages;
    int message = 1;
    while (file >> message){
        // cout << message << " ";
        messages.push_back(message);
    }
    file.close();

    // output the numbers into a new file, called encrypted.txt
    ofstream output;
    output.open("encrypted.txt");
    if (!output.is_open()){
        cout << "could not open file.. exiting"<<endl;
        return;
    }

    cout << endl;
    // encrypting: M^e (mod n)
    for (int i = 0; i < messages.size(); i++){
        // cout << messages[i] << " -> ";
        message = 1;
        for (int j = 0; j < e; j++){
            message *= messages[i];
            // cout << message << endl;
            message %= n;
        }
        // cout << message << endl;
        output << message << " ";
    }
    cout << "The encrypted (number) textfile is named 'encrypted.txt'" << endl;
    output.close();


}

void decryption( int n, int e, int phi){
    // open encrypted text file(numbers)
    cout << "Enter the name of the text file you would like to decrypt: " << endl;
    string filename;
    cin >> filename;
    ifstream file;
    file.open(filename);
    if (!file.is_open()){
        cout << "Could not open file..exiting" << endl;
        return;
    }
    vector<int> messages;
    int message;
    // get the encrypted numbers from text file
    while (file >> message){
        messages.push_back(message);
    }
    file.close();
 

    //calculate d, the inverse of e
    // using fermat's little theorem, e^phi-2 mod phi = d
    long long d = e;
    cout << "e: " << e << endl;
    for (int i = 0; i < phi-2; i++){
        d *= e;
        // cout << d << endl;
        d %= phi;
    }
    cout << "d: " << d << endl;
    cout << "inverse: " << d << endl;


    //open output file
    ofstream outFile;
    outFile.open("decryptedNumbers.txt");
    if (!outFile.is_open()){
        cout << "Could not open file..exiting" << endl;
        return;
    }


    // convert each integer into its decrypted value;
    // d = message^inverse % n
    for (int i = 0; i < messages.size(); i++){
        message = 1;
        for (int j = 0; j < d; j++){
            message *= messages[i];
            message %= n;
        } 
        outFile << message << " ";
    }
    outFile.close();

    // convert the decrypted numbers into its schema
    cout << "The english text message is named 'decrypted.txt' " << endl;
    numtoLetters("decryptedNumbers.txt");
}
