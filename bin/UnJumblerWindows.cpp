#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

struct chararray  //for string
{
    char letword[11];
};

ifstream wordsrh;   //to accept input from all files
fstream oncedisp;  //to check if the word has already been displayed
chararray jumbword;  //holds the jumbled word (to be unjumbled)
chararray jumbtemp;   //copy of jumbword
chararray word_being_searched;  //to read from files and compare
chararray cforod;   //container for oncedisp
char filename[7]="AA.txt";
char temp;  //for swapping
char ch='n';    //exit control for do-while loop
int lastpos;    //last position
int seclastpos; //second last position
int flag;   //to see if a word has been found - one for yes and zero for no
int i;  //sidekick

void searchword(chararray search)
{
    filename[0] = search.letword[0];
    filename[1] = search.letword[1];
    wordsrh.open(filename, ios::binary|ios::in);
    if (wordsrh.good())  //if the file exists and is good for i/o operations
    {
        wordsrh.read((char *)&word_being_searched, sizeof(chararray));
        while (!wordsrh.eof()&&word_being_searched.letword[2] <= search.letword[2])
        {
            if (strcmp(search.letword, word_being_searched.letword) == 0)
            {
                flag = 1;
                oncedisp.clear();
                oncedisp.seekg(0);
                oncedisp.read((char *)&cforod, sizeof(chararray));
                while (!oncedisp.eof())
                {
                    if (strcmp(cforod.letword, word_being_searched.letword) == 0)
                    {
                        break;
                    }
                    oncedisp.read((char *)&cforod, sizeof(chararray));
                }
                if(oncedisp.eof())
                {
                    oncedisp.clear();
                    oncedisp.seekg(0, ios::end);
                    oncedisp.write((char *)&word_being_searched, sizeof(chararray));
                }
            }
            wordsrh.read((char *)&word_being_searched, sizeof(chararray));
        }
    }
    wordsrh.close();
}

void changeword(chararray p_word, int order)
{
    if(order == 2)
    {
        searchword(p_word);
        temp = p_word.letword[lastpos];
        p_word.letword[lastpos] = p_word.letword[seclastpos];
        p_word.letword[seclastpos] = temp;
        searchword(p_word);
    }
    else
    {
        changeword(p_word, order-1);
        for(int ctr = 0; ctr < order-1; ctr++)
        {
            temp = p_word.letword[lastpos-ctr];
            p_word.letword[lastpos-ctr] = p_word.letword[lastpos-order+1];
            p_word.letword[lastpos-order+1] = temp;
            changeword(p_word, order-1);
        }
    }
}

int main()
{
    system("cls");
    cout << "Hi there! This program tries to un-jumble any word\nthat you want unjumbled.\n";
    cout << "But first some ground rules:\n";
    do
    {
        flag = 0;
        cout << "- The word should have a minimum of three characters\n";
        cout << "- Maximum word size is ten\n";
        cout << "- Symbols are not supported, e.g. -, (space), etc.\n";
        cout << "- Plurals are allowed\n";
        cout << "- Names of animals are allowed\n";
        cout << "- No names of people or places\n";
        cout << "- The longer the word, the longer it takes to\nget to the solution.\n";
        cout << "- Some words displayed may seem gibberish, but most of the \ntime the words do exist. A simple Google search would reveal\nabout the same.\n";
        cout << "Okay, we're ready to roll...LET THE WORD BE ENTERED!\n";
        oncedisp.open("Displayed.txt", ios::out | ios::binary | ios::in | ios::trunc);
        cin.getline(jumbword.letword, 25);
        for(i = 0; i < strlen(jumbword.letword); i++)
        {
            jumbword.letword[i] = toupper(jumbword.letword[i]);
        }
        lastpos = strlen(jumbword.letword) - 1;
        seclastpos = lastpos - 1;
        jumbtemp = jumbword;
        cout << "Loading results, this may take some time...\n";
        changeword(jumbtemp, strlen(jumbtemp.letword));
        if (flag == 1)
        {
            cout << "The following word(s) may form solution(s):\n";
            oncedisp.clear();
            oncedisp.seekg(0);
            oncedisp.read((char *)&cforod, sizeof(chararray));
            while (!oncedisp.eof())
            {
                cout << cforod.letword << endl;
                oncedisp.read((char *)&cforod, sizeof(chararray));
            }
        }
        else
        {
            cout << "I'm sorry...I could not place any word that matches with your\nset of alphabets. I need to learn :-(\n";
        }
        cout << "Go again? Enter Y or N: ";
        cin >> ch;
        if (ch == 'y')
        {
            system("cls");
            cout << "You might wanna take a look at the rules again:\n";
        }
        oncedisp.close();
        cin.ignore();
    } while(ch == 'y' || ch == 'Y');
    cout << "Thank you for using my program! Do come again! Press Enter key to exit: ";
    cin.get();
    return 0;
}


