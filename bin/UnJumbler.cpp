#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

// to store a word or string, so that it can be passed without
// worrying about mutation by a function
struct char_array
{
    char word[11];
};

// variables

ifstream reqd_dict_file;   //to accept input from the corresponding dictionary file
fstream already_found_words;  //to check if the word has already been found

char_array word_to_unjumble;  //holds the jumbled word (to be unjumbled)
char_array word_to_unjumble_copy;   //copy of word_to_unjumble
char_array word_being_searched;  //to read from files and compare
char_array found_words;   //container for already_found_words

char file_name[7]="AA.txt";

char temp;  //for swapping
char choice = 'n';    //exit control for do-while loop
int flag;   //to see if a word has been found - one for yes and zero for no
unsigned int i;  //sidekick

int last_pos; // last position   
int second_last_pos; // second last position

// functions

// searches if a given word exists in the dictionary
void search_for_word(char_array word_to_search)
{
	// find the corresponding dictionary file
    file_name[0] = word_to_search.word[0];
    file_name[1] = word_to_search.word[1];
    
    //open it, or atleast try to
    reqd_dict_file.open(file_name, ios::binary|ios::in);
    
    //check if the file exists and is good for i/o operations
    if (reqd_dict_file.good())  
    {
        reqd_dict_file.read((char *)&word_being_searched, sizeof(char_array));
        
        // cheap optimization to check if third letter matches
        // works because minimum word length asked for is three
        while (!reqd_dict_file.eof() && word_being_searched.word[2] <= word_to_search.word[2])
        {
			// compare full word now
            if (strcmp(word_to_search.word, word_being_searched.word) == 0)
            {
				// word has been found
                flag = 1;
                
                // check if word has already been found
                // this check is required for words with two or more occurrence of the same letter,
                // because the same permutation may be generated
                already_found_words.clear();
                already_found_words.seekg(0);
                already_found_words.read((char *)&found_words, sizeof(char_array));
                while (!already_found_words.eof())
                {
                    if (strcmp(found_words.word, word_being_searched.word) == 0)
                    {
						// word has already already been found once
                        break;
                    }
                    already_found_words.read((char *)&found_words, sizeof(char_array));
                }
                if(already_found_words.eof())
                {
					// word has not been found earlier
					// so add it
                    already_found_words.clear();
                    already_found_words.seekg(0, ios::end);
                    already_found_words.write((char *)&word_being_searched, sizeof(char_array));
                }
            }
            reqd_dict_file.read((char *)&word_being_searched, sizeof(char_array));
        }
    }
    reqd_dict_file.close();
}

// function that generates permutations

/* Key idea:
 * To generate permutations of a word, replace the starting letter of the word
 * by all its subsequent letters in each step, and generate permutations like this:
 * Permutations of a word = starting letter + permutation of all subsequent letters */
 
// current_index says which is the "starting letter" to be picked in the recursion

void generate_permutations(char_array word_to_permute, int current_index)
{
	// swap the last two letters to generate two new permutations
    if(current_index == 2)
    {
		// search for the word as it is
        search_for_word(word_to_permute);
        
        // change last two letters
        temp = word_to_permute.word[last_pos];
        word_to_permute.word[last_pos] = word_to_permute.word[second_last_pos];
        word_to_permute.word[second_last_pos] = temp;
        
        // search for the new word
        search_for_word(word_to_permute);
    }
    else
    {
		// recursively call until control goes to above if condition, i.e.
		// till current index becomes two
        generate_permutations(word_to_permute, current_index - 1);
        
        // then swap first letter with all subsequent letters and try
        for(int ctr = 0; ctr < current_index - 1; ctr++)
        {
            temp = word_to_permute.word[last_pos - ctr];
            word_to_permute.word[last_pos - ctr] = word_to_permute.word[last_pos - current_index + 1];
            word_to_permute.word[last_pos - current_index + 1] = temp;
            
            // generate permutations of words with all subsequent letters
            generate_permutations(word_to_permute, current_index - 1);
        }
    }
}

int main()
{
    system("clear");
    
    // small talk and formalities
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
        cout << "- Some words found may seem gibberish, but most of the \ntime the words do exist. A simple Google search would reveal\nabout the same.\n";
        cout << "Okay, we're ready to roll...LET THE WORD BE ENTERED!\n";
        
        already_found_words.open("found.txt", ios::out | ios::binary | ios::in | ios::trunc);
        
        cin.getline(word_to_unjumble.word, 25);
        
        for(i = 0; i < strlen(word_to_unjumble.word); i++)
        {
            word_to_unjumble.word[i] = toupper(word_to_unjumble.word[i]);
        }
        
        last_pos = strlen(word_to_unjumble.word) - 1;
        second_last_pos = last_pos - 1;
        word_to_unjumble_copy = word_to_unjumble;
        
        // heh heh, already make an excuse
        cout << "Loading results, this may take some time...\n";
        
        // the main call that does the work
        generate_permutations(word_to_unjumble_copy, strlen(word_to_unjumble_copy.word));
        
        if (flag == 1)	// atleast one valid word has been found
        {
            cout << "The following word(s) may form solution(s):\n";
            
            // read from already_found_words_file
            already_found_words.clear();
            already_found_words.seekg(0);
            already_found_words.read((char *)&found_words, sizeof(char_array));
            while (!already_found_words.eof())
            {
                cout << found_words.word << endl;
                already_found_words.read((char *)&found_words, sizeof(char_array));
            }
        }
        else
        {
            cout << "I'm sorry...I could not place any word that matches with your\nset of alphabets. I need to learn :-(\n";
        }
        cout << "Go again? Enter Y or N: ";
        cin >> choice;
        if (choice == 'y')
        {
            system("clear");
            cout << "You might wanna take a look at the rules again:\n";
        }
        already_found_words.close();
        cin.ignore();
    } while(choice == 'y' || choice == 'Y');
    cout << "Thank you for using my program! Do come again! Press Enter key to exit: ";
    cin.get();
    return 0;
}
