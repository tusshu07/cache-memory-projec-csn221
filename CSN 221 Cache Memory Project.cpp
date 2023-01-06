#include<bits/stdc++.h>
#define ll long long
using namespace std;

//Function to find binary conversion
string num_to_pa(ll word,ll physical_address_bits)
{
    string temp = "";
    while(word > 0)
    {
        ll bit = word%2;
        if(bit == 1)
        {
            temp += "1";
        }
        else
        {
            temp += "0";
        }
        word = word/2;
    }
    reverse(temp.begin(),temp.end());
    ll n = temp.size();
    string ans = "";
    while(n < physical_address_bits)
    {
        ans += "0";
        n++;
    }
    ans += temp;
    return ans;
}

//Function to find decimal conversion
ll to_dec(string bin)
{
    while(bin.length()>0 && bin[0] == '0')
    {
        bin.erase(bin.begin());
    }
    if(bin.length() == 0)
    {
        return 0;
    }
    ll n = bin.length();
    ll k = 0;
    ll num = 0;
    ll p = n-1;
    while(p >= 0)
    {
        if(bin[p]=='1')
        {
            num += pow(2,k);
        }
        k++;
        p--;
    }
    return num;
}

int main()
{
    ll main_memory_word_count,cache_memory_word_count,k;

    cout<<"Enter main memory word count"<<endl;
    cin>>main_memory_word_count;

    cout<<"Enter cache memory word count"<<endl;
    cin>>cache_memory_word_count;

    cout<<"Enter the value of k"<<endl;
    cin>>k;

    cout<<endl;

    // Checking whether word count in main memory and cache memory is valid or not
    if((main_memory_word_count & (main_memory_word_count-1)) != 0)
    {
        cout<<"Error"; return 0;
    }
    else if((cache_memory_word_count & (cache_memory_word_count-1)) != 0)
    {
        cout<<"Error"; return 0;
    }

    // Number of bits in physical address
    ll physical_address_bits = log2(main_memory_word_count);
    cout<<"Number of bits in physical address are "<<physical_address_bits<<endl;

    // Number of bits in block number
    ll block_bit = physical_address_bits - 2;
    cout<<"Number of bits needed to represent block address are "<<block_bit<<endl;


    ll block_no,word_loc;
    ll tag,set_block;

    // Number of sets
    ll no_sets = cache_memory_word_count/(4*k);
    cout<<"Total number of sets in cache memory are "<<no_sets<<endl;

    // Declaration of cache
    vector<vector<int>> cache(no_sets,vector<int>(k,-1));

    // LRU for each set number
    vector<deque<int>> LRU(no_sets);
    ll lru[no_sets][k];
    ll valid[no_sets][k];
    ll dirty[no_sets][k];
    for(ll i=0;i<no_sets;i++){
        for(ll j=0;j<k;j++){
            lru[i][j]=k-1;
            valid[i][j]=0;
            dirty[i][j]=0;
        }
    }

    //input all the words to be searched for
    cout<<endl<<"Input number of words you want to search"<<endl;
    ll num;
    cin>>num;

    ll hit = 0;
    ll miss = 0;

    while(num--)
    {
        cout<<endl<<"Enter the word to search"<<endl;
        ll word;
        cin>>word;

        // Error
        if(word >= main_memory_word_count){ cout<<"Enter value between 0 and "<<main_memory_word_count<<endl; num++; continue ; }

        string bit_conv = num_to_pa(word,physical_address_bits);
        cout<<"The "<<physical_address_bits<<" digit binary conversion of required word is"<<endl;
        cout<<bit_conv<<endl<<endl;

        //Block number
        string temp = "";
        for(ll i=0;i<block_bit;i++)
        {
            temp += bit_conv[i];
        }
        block_no = to_dec(temp);
        cout<<"The block number for required word is "<<block_no<<endl;

        // Word location in that block
        temp = "";
        for(ll i=block_bit;i<physical_address_bits;i++)
        {
            temp += bit_conv[i];
        }
        word_loc = to_dec(temp);
        cout<<"The word location in that block is "<<word_loc<<endl;

        //Hit or miss
        // Bits required to store set number
        ll bits_in_set_no = log2(no_sets);

        //Tag
        temp = "";
        ll bits_in_tag = physical_address_bits - 2 - bits_in_set_no;
        for(ll i=0;i<bits_in_tag;i++)
        {
            temp += bit_conv[i];
        }
        tag = to_dec(temp);
        cout<<"Tag is "<<tag<<endl;

        //set number
        temp = "";
        for(ll i=bits_in_tag;i<bits_in_tag + bits_in_set_no;i++)
        {
            temp += bit_conv[i];
        }
        set_block = to_dec(temp);
        cout<<"The set number in which block could be is "<<set_block<<endl;

        //Word location (same as previous)
        temp = "";
        for(ll i=bits_in_set_no + bits_in_tag;i<physical_address_bits;i++)
        {
            temp += bit_conv[i];
        }
        word_loc = to_dec(temp);

        bool is_hit = 0;

        for(ll i=0;i<k;i++)
        {
            if(valid[set_block][i]==1&&cache[set_block][i] == block_no)
            {
                //It is a hit
                cout<<"It is a hit"<<endl;

                is_hit = 1;
                hit++;

                //Update LRU
                for(ll j=0;j<k;j++){
                    if(lru[set_block][j]<lru[set_block][i]){
                        lru[set_block][j]++;
                    }
                }
                bool write;
                if(write){
                    dirty[set_block][i]=1;
                }

            }
        }

        if(is_hit == 0)
        {
            //it is a miss
            cout<<"It is a miss"<<endl<<"The number is inserted in cache"<<endl;
            miss++;

            //Update cache
            ll lru_block=0;
            for(ll j=0;j<k;j++){
                if(lru[set_block][j]>lru[set_block][lru_block]){
                    lru_block=j;
                }
                else{
                    lru[set_block][j]++;
                }
            }
            valid[set_block][lru_block]=0;
            cache[set_block][lru_block]=block_no;
            valid[set_block][lru_block]=1;//after replacing the block
            dirty[set_block][lru_block]=0;
        }
    }

    // Calculating the hit and miss ratio
    double hit_ratio = (double)((double)hit/(double)(hit + miss));
    double miss_ratio = (double)((double)miss/(double)(hit + miss));

    cout<<endl<<"The hit ratio is = "<<hit_ratio<<endl;
    cout<<"The miss ratio is = "<<miss_ratio<<endl;
}
