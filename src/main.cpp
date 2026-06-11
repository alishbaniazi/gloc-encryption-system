#include <iostream>
using namespace std;
//--------------------PROTOTYPES-----------------------
unsigned long long int rep_id(int);
int even_odd(unsigned long long int,char);
bool is_prime(int);
int refined_prime(int, int, int);
bool coprime(int,int);
int enc(char,int,int);
int dec(int num,int,int);

//main
int main(){
   
    int orgid=0;
    int id=0;
    char arr[5]={'\0'};                                         //storing in array so that initial zero's are not lost
    cout<<"Enter your 4 digit PIN(eg 1234): ";
    cin>>arr;

    while (arr[3]=='\0'||arr[4]!='\0'){                         //VALIDATION
        cout<<"INVALID INPUT, PLEASE TRY AGAIN!"<<endl;
        cout<<"Enter your 4 digit PIN(eg 1234): ";
        cin>>arr;
    }

    for (int i=0;i<4;i++){                                      //extracting 4 digit pin in int variable
        id*=10;
        id += arr[i]-'0';
    }
    
    unsigned long long int repid=rep_id(id);
    int e=even_odd(repid,'e');
    int o=even_odd(repid,'o');
    int p1=0,p2=0;
    
    int count=0;
    int i=0;
      
    
    int max =e>o?e:o;
    while(count<max){
        
        if (is_prime(i)){
            count++;
            if (count==e){
                p1=i;
            }
            if (count ==o){
                p2=i;
            }
            
    }
    i++;
        
    }
    
    e=e<5?5:e;                       //clamping
    o=o<5?5:o;

    
    //---------------session modulus------------------
    int refined_p1=refined_prime(p1,e,o);
    int refined_p2=refined_prime(p2,e,o);
    if (refined_p1==refined_p2){
        while(true){
            refined_p2++;
            if (is_prime(refined_p2)){
                break;
            }
        }
    }
    int session_modulus=refined_p1*refined_p2;


    //--------------------system totient-----------------
    int system_totient = (refined_p1-1)*(refined_p2-1);

    //--------------------binding exponent---------------
    int ind=1,k=e+o,counter=0,binding_exponent;
    
        while(true){
            if (coprime(system_totient,ind)){
                counter++;
            }
            if (counter==k){
                binding_exponent=ind;
                break;
            }
            ind++;
        }
    //---------------------UNBINDING EXPONENT----------------
        int unbinding_exponent=0;
        for(int i=1;i<=system_totient;i++){
            if(((i*binding_exponent)%system_totient)==1){
                unbinding_exponent=i;
                break;
     
            }
        }

    //--------------------Decryption Keypair------------------
    int decryption_keypair[2]={session_modulus,unbinding_exponent};
    
    //--------------------Encryption Keypair-------------------
    int encryption_keypair[2]={session_modulus,binding_exponent};

    cout<<"------------"<<endl<<"Encryption keypair {"<<encryption_keypair[0]<<", "<<encryption_keypair[1]<<"}"<<endl<<"Decryption Keypair "<<"{"<<decryption_keypair[0]<<", "<<decryption_keypair[1]<<"}"<<endl<<endl;

   
    
    //--------------------Encryption protocol--------------------
    int noOfdig=1;                                 //INITIALISED TO 1, SAMPLE OUTPUT MAIN THA FOR THE STARTING ZERO
    int x=session_modulus;
    
    int choice;
    
    cout<<"-------MENU--------\n0=Exit\n1=Encrytion\n2=Decryption\nEnter your choice: ";              //taking user choice
    cin>>choice;

    switch(choice){

    case 0:{
        cout<<"Exiting the program...";
        exit(0);
        break;
    }

    case 1:{ 
    
    
     //---------------message input--------------------
    char mess_arr[10000]={'\0'};                                       //size hawa main rakha
    cout<<"Enter the message to encrypt::: ";
    int j=0;
    cin.ignore();                                        //ignores prev\n or \0 in the buffer
    cin.get(mess_arr[j]);

    while((mess_arr[j])!='\n'&&j<session_modulus-1){
        j++;
        cin.get(mess_arr[j]);
        if (mess_arr[j]==' '){
            mess_arr[j]='_';
        }
        if (mess_arr[j]=='\n'){
            mess_arr[j]='\0';
            break;
        }
        
    }
        while(x!=0){
            noOfdig++;
            x=x/10;
    }
    int size_bigarr = j*noOfdig+j,ind_bigarr=0;                      //bari array ka index   +j spaces/underscores k lia haiiiiii
    char big_arr[size_bigarr-1];
    for (int i=0;i<j;i++){
        char tempchar=mess_arr[i];
        if (tempchar=='_'){
            tempchar=' ';
        }
        int enc_value=enc(tempchar,binding_exponent,session_modulus);
      

        char ind_arr[noOfdig];
        for (int i=noOfdig-1;i>=0;i--){                          //padded value in indviual array of each char
            ind_arr[i]=enc_value%10+'0';
            enc_value/=10;
        }
       
        int x=0;

        while(x<noOfdig){
            big_arr[ind_bigarr]=ind_arr[x];
            x++;
            ind_bigarr++;
        }   
        if(i+1<j)                                           //to put _ after each charachter's encrypted value  
        big_arr[ind_bigarr++]='_';
        else
        big_arr[ind_bigarr]='\0';                           //this puts null character, after the enc value of last char, and not the _.


    }
    cout<<endl<<"ENCRYPTING THE MESSAGE..."<<endl<<endl;                //output
    cout<<"Encrypted data stream: "<<endl;
    for(i=0;i<size_bigarr;i++){
    cout<<big_arr[i];
    }
    break;
}
    

    //----------------------------------------Decryption----------------------------------
    case 2:{
    char enc_arr[10000]={'\0'};
    
    cout<<endl<<"Enter the Encrypted data stream: ";
    cin>>enc_arr;
    int l=0;
    cout<<endl<<"Decrypting the message..."<<endl<<endl<<"Decrypted Message: "<<endl;
    while(enc_arr[l]!='\0'){

        int temp=0;
        while(true){
            temp+=enc_arr[l]-'0';
            l++;
            if(enc_arr[l]=='\0'||enc_arr[l]=='_'){
                
                l++;
                break;
            }
            else{
                temp*=10;
            }
        }
        cout<<char(dec(temp,session_modulus,unbinding_exponent));   
        
    }
    break;
    }
    default:{
        cout<<"Invalid Input!\nExiting the program...";
        break;
    }
    


}
}



//-------------------------functions-----------------------------
unsigned long long int rep_id(int id){                 //repetitive id
    unsigned long long int repid=0;
    for (int i=0;i<4;i++){
        repid*=10000;
        repid+=id;
    }
    return (repid);
}

int refined_prime(int rawprime, int e, int o){                     //refined prime
    int refined = rawprime;
    if (e>=o){
    while(true){
        refined+=(e%5)+1;
        if (is_prime(refined)){
            break;
        }
        if (refined>61){
            refined-=50;
        }
    }}
    else{
        while(true){
        refined-=(o%5)+1;
        if (is_prime(refined)){
            break;
        }
        if (refined<11){
            refined+=50;
        }
    }
    }
    refined=refined>61?61:refined;
    refined=refined<11?11:refined;
    return refined;
}


int even_odd(unsigned long long int repid,char flag){                       //function for finding handshake primes p1&p2
    int st,total=0;
    if (flag=='e'){
        st=0;
    }
    else if(flag=='o'){
        st=1;
    }
    for(int i=st;i<64;i+=2){
        total+=(repid&(1ULL<<i))>>i;
    }
    return total;

}


bool is_prime(int num){
        
            if (num==2){
                return 1;
            }
            else if (num<=1){
                return 0;
            }
            for(int i=2;i<=num/2;i++){
             if(num%i==0){
                return 0;
            }
        }
        return 1;
}

bool coprime(int num1,int num2){
    int arr1[3600]={0},count1=0;                                                //taking the size as 3600 bcz the two primes are clamped at 61 and so the max system totient can be 60*60
    int arr2[3600]={0},count2=0;
    for (int count=0,i=2;i<=num1;i++){         //stores all factors of num1
            if(num1%i==0){
                arr1[count]=i;
                count++;
                count1++;
            }
    }
    for (int count=0,i=2;i<=num2;i++){                //stores all factors of num2
            if(num2%i==0){
                arr2[count]=i;
                count++;
                count2++;
            }
}
    for (int i=0;i<count1;i++){                    //checks for comon factors
        for(int j=0;j<count2;j++){
            if (arr1[i]==arr2[j]&&(arr1[i]!=0)){
                return 0;
            }
        }
    }
    return 1;
}

int enc(char a,int binding_exponent,int session_modulus){
    int enc_num=1;
    for(int i=0;i<binding_exponent;i++){
        enc_num=(enc_num*a)%session_modulus;
    }
    return enc_num;
}

int dec(int num,int session_modulus, int unbinding_exponent){
    unsigned long long int ascii=1;
    for(int i=0;i<unbinding_exponent;i++){
        ascii=(ascii * num)%session_modulus;         //(a.b)%n = (a%n)(b%n)%n we disregarded the last %n because we initilised to 1 and not the actual value
    }
    

    return int(ascii);
}