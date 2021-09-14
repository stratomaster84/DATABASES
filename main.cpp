#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <fstream>

using namespace std;

struct MyEntry{
    size_t my_id; // not in struct

    string _id;
    string _FIO;
    string _F;
    string _I;
    string _O;
    vector<string> _mails; // две почты
    vector<string> _phones;  // 5 телефонов - ТЛФ1, ТЛФ1 - стандарт, ДОБ, ТЛФ2, ТЛФ2 - стандарт
    vector<string> _spec; // две специальности
    string _text;    // непонятно
    string _city;
    string _addr1;
    string _addr2;
    string _another;
    string _source;
    string _country;
    string _lastentry;

    string extraphone1;
    string extraphone2;
    string extraspec1;
    string extraspec2;
    string extracity1;
    string extracity2;
    string extracity3;
    string extramail1;

    bool equalphones;
    set<string> allmails;
    set<string> allphones;
    set<string> allcities;
    set<string> allspec;
};
bool hasEqualPhones(MyEntry* a,MyEntry* b){
    if(a->allphones.empty())
        return false;
    if(b->allphones.empty())
        return false;
    set<string> _intersection;
    set_intersection(a->allphones.begin(),a->allphones.end(),
                     b->allphones.begin(),b->allphones.end(),
                     inserter(_intersection,_intersection.begin()));
    if(!_intersection.empty())
        return true;
    return false;
}
void mergeEntries(MyEntry* a,MyEntry* b){
    // добавление почты
    for(auto s : b->allmails){
        a->allmails.insert(s);
    }
    // добавление телефонов
    for(auto s : b->allphones){
        a->allphones.insert(s);
    }
    // добавление специальностей
    for(auto s : b->allspec){
        a->allspec.insert(s);
    }
    // добавление города
    for(auto s : b->allcities){
        a->allcities.insert(s);
    }
    // добавление source и country
    if(a->_source=="")
        a->_source = b->_source;
    if(a->_country=="")
        a->_country = b->_country;
}
void Merge(vector<MyEntry*>& base){
    MyEntry* a;
    bool _first = true;
    for(MyEntry * b: base){
        if(_first){
            a = b;
            _first = false;
            continue;
        }
        if(a->_FIO == b->_FIO && hasEqualPhones(a,b)){
            mergeEntries(a,b);
            b->equalphones = false;
        }
        else{
            vector<string> tmp;
            copy(a->allmails.begin(),a->allmails.end(),back_inserter(tmp));
            try {
                a->_mails[0] = tmp.at(0);
                a->_mails[1] = tmp.at(1);
                a->extramail1 = tmp.at(2);
            } catch (...) {}
            tmp.clear();
            copy(a->allphones.begin(),a->allphones.end(),back_inserter(tmp));
            try {
                a->_phones[1] = tmp.at(0);
                a->_phones[4] = tmp.at(1);
                a->extraphone1 = tmp.at(2);
                a->extraphone2 = tmp.at(3);
            } catch (...) {}
            tmp.clear();
            copy(a->allspec.begin(),a->allspec.end(),back_inserter(tmp));
            try {
                a->_spec[0] = tmp.at(0);
                a->_spec[1] = tmp.at(1);
                a->extraspec1 = tmp.at(2);
                a->extraspec2 = tmp.at(3);
            } catch (...) {}
            tmp.clear();
            copy(a->allcities.begin(),a->allcities.end(),back_inserter(tmp));
            try {
                a->_city = tmp.at(0);
                a->extracity1 = tmp.at(1);
                a->extracity2 = tmp.at(2);
                a->extracity3 = tmp.at(3);
            } catch (...) {}
            tmp.clear();
            a = b;
        }
    }
    auto it = remove_if(base.begin(),base.end(),[](MyEntry* a){
            if(a->equalphones == false)
                return true;
            return false;
         }
          );
    base.erase(it,base.end());
}
MyEntry* getNewEntry(ifstream &input){
    string _temp;
    if(!getline(input,_temp,'\t')){
        return nullptr;
    }

    MyEntry *result;
    result = new MyEntry;
    result->_id = _temp;

    getline(input,result->_FIO,'\t');
    getline(input,result->_F,'\t');
    getline(input,result->_I,'\t');
    getline(input,result->_O,'\t');

    getline(input,_temp,'\t');
    result->_mails.push_back(_temp);
    getline(input,_temp,'\t');
    result->_mails.push_back(_temp);

    getline(input,_temp,'\t');
    result->_phones.push_back(_temp);
    getline(input,_temp,'\t');
    result->_phones.push_back(_temp);
    getline(input,_temp,'\t');
    result->_phones.push_back(_temp);
    getline(input,_temp,'\t');
    result->_phones.push_back(_temp);
    getline(input,_temp,'\t');
    result->_phones.push_back(_temp);

    getline(input,_temp,'\t');
    result->_spec.push_back(_temp);
    getline(input,_temp,'\t');
    result->_spec.push_back(_temp);

    getline(input,result->_text,'\t');
    getline(input,result->_city,'\t');
    getline(input,result->_addr1,'\t');
    getline(input,result->_addr2,'\t');
    getline(input,result->_another,'\t');
    getline(input,result->_source,'\t');
    getline(input,result->_country,'\t');
    getline(input,result->_lastentry,'\t');

    getline(input,result->extraphone1,'\t');
    getline(input,result->extraphone2,'\t');
    getline(input,result->extraspec1,'\t');
    getline(input,result->extraspec2,'\t');
    getline(input,result->extracity1,'\t');
    getline(input,result->extracity2,'\t');
    getline(input,result->extracity3);
    return result;
}

void printBase(ofstream &output, vector<MyEntry*> base){
    bool first = true;
    for(const auto &_entry : base){
        /*if(_entry->equalmails == false)
            continue;*/
        if(first){
            first = false;
        }
        else
            output << '\n';
        output << _entry->_id << '\t';
        output << _entry->_FIO << '\t';
        output << _entry->_F << '\t';
        output << _entry->_I << '\t';
        output << _entry->_O << '\t';

        output << _entry->_mails[0] << '\t';
        output << _entry->_mails[1] << '\t';

        output << _entry->_phones[0] << '\t';
        output << _entry->_phones[1] << '\t';
        output << _entry->_phones[2] << '\t';
        output << _entry->_phones[3] << '\t';
        output << _entry->_phones[4] << '\t';

        output << _entry->_spec[0] << '\t';
        output << _entry->_spec[1] << '\t';

        output << _entry->_text << '\t';
        output << _entry->_city << '\t';
        output << _entry->_addr1 << '\t';
        output << _entry->_addr2 << '\t';
        output << _entry->_another << '\t';
        output << _entry->_source << '\t';
        output << _entry->_country << '\t';
        output << _entry->_lastentry << '\t';

        output << _entry->extramail1 << '\t';
        output << _entry->extraphone1 << '\t';
        output << _entry->extraphone2 << '\t';
        output << _entry->extraspec1 << '\t';
        output << _entry->extraspec2 << '\t';
        output << _entry->extracity1 << '\t';
        output << _entry->extracity2 << '\t';
        output << _entry->extracity3;
     }

}

int main()
{ 
    vector<MyEntry*> _base1;
    MyEntry* _temp;
    int i=0;

    ifstream inp("input.txt");
    while(true){
        _temp = getNewEntry(inp);
        if(_temp == nullptr)
            break;
        _temp->my_id = i++;
        _temp->equalphones = false;
        _temp->extramail1 = "";
        _base1.push_back(move(_temp));
    }
    inp.close();

    for_each(_base1.begin(),_base1.end(),
             [](MyEntry* a){
        if(find(a->_mails[0].begin(),a->_mails[0].end(),'@') != a->_mails[0].end())
            a->allmails.insert(a->_mails[0]);
        if(find(a->_mails[1].begin(),a->_mails[1].end(),'@') != a->_mails[1].end())
            a->allmails.insert(a->_mails[1]);
  }
    );
    for_each(_base1.begin(),_base1.end(),
             [](MyEntry* a){
        if(a->_phones[1].size() == 10)
            a->allphones.insert(a->_phones[1]);
        if(a->_phones[4].size() == 10)
            a->allphones.insert(a->_phones[4]);
        if(a->extraphone1.size() == 10)
            a->allphones.insert(a->extraphone1);
        if(a->extraphone2.size() == 10)
            a->allphones.insert(a->extraphone2);
  }
    );
    for_each(_base1.begin(),_base1.end(),
             [](MyEntry* a){
        if(a->_spec[0]!="")
            a->allspec.insert(a->_spec[0]);
        if(a->_spec[1]!="")
            a->allspec.insert(a->_spec[1]);
        if(a->extraspec1!="")
            a->allspec.insert(a->extraspec1);
        if(a->extraspec2!="")
            a->allspec.insert(a->extraspec2);
 }
    );
    for_each(_base1.begin(),_base1.end(),
             [](MyEntry* a){
        if(a->_city!="")
            a->allcities.insert(a->_city);
        if(a->extracity1!="")
            a->allcities.insert(a->extracity1);
        if(a->extracity2!="")
            a->allcities.insert(a->extracity2);
        if(a->extracity3!="")
            a->allcities.insert(a->extracity3);
}
    );
// base1 is read!!!!
   sort(_base1.begin(),_base1.end(),
             [](MyEntry* a,MyEntry* b){
       if(a->_FIO < b->_FIO)
           return true;
       if(a->_FIO > b->_FIO)
           return false;
            if(!hasEqualPhones(a,b))
                return false;

            a->equalphones = true;
            b->equalphones = true;
            if(a->my_id < b->my_id)
                return true;
            return false;  // if all equal
             }
   );
   vector<MyEntry*> rep;
   copy_if(_base1.begin(),_base1.end(),back_inserter(rep),[](MyEntry* a){
      if(a->equalphones == true)
          return true;
      return false;
   }
    );
   auto it = remove_if(_base1.begin(),_base1.end(),[](MyEntry* a){
           if(a->equalphones == true)
               return true;
           return false;
        }
         );
   _base1.erase(it,_base1.end());

   // _base1 - all good
   // rep - repeated
   Merge(rep);
   /*int max =0;
   for(MyEntry* a:rep){
        int tmp = a->allcities.size();
        if(tmp > max)
            max = tmp;

   }*/
    copy(rep.begin(),rep.end(),back_inserter(_base1));
    sort(_base1.begin(),_base1.end(),
              [](MyEntry* a,MyEntry* b){
        if(a->my_id < b->my_id)
            return true;
         return false;  // if all equal
              }
    );
// base1 is ready to print
    ofstream outp("output.txt");
    printBase(outp,_base1);
    outp.close();

    return 0;
}
//----------------------------------------
