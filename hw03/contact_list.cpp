#include "contact_list.h"

#include <algorithm>
#include <numeric>
#include <sstream>

// TODO create implementation here!

/**
 * Given a contact storage, create a new contact entry by name and number.
 */
namespace contact_list {

bool add(storage& contacts, std::string_view name, number_t number){
    if(name.empty()){
        return false;
    }
    for(std::string exist_Name : contacts.names){
        if(exist_Name == name){
            return false;
        }
    }
    contacts.names.push_back(std::string(name));
    contacts.numbers.push_back(number);
    return true;
}
/**
 * Given a contact storage, how many contacts are currently stored?
 */
size_t size(const storage& contacts)
{
    return contacts.names.size();
}
/**
 * Fetch a contact number from storage given a name.
 */
number_t get_number_by_name(storage& contacts, std::string_view name){
        for(size_t i = 0; i< contacts.names.size(); i++){
            if (name == contacts.names[i]){
                return contacts.numbers[i];
            }
            else{

            }
    }return -1;
}
/**
 * Return a string representing the contact list.
 */
std::string to_string(const storage& contacts){
    if(contacts.names.size() == 0){
        return "abook is empty";
    }
    std::string con;
    for(size_t i = 0; i < contacts.names.size(); i++){
        con += "   " + contacts.names[i] + " - " + std::to_string(contacts.numbers[i]) +"   " + "\n";
    }
        return con;
}
/**
 * Remove a contact by name from the contact list.
 */
bool remove(storage& contacts, std::string_view name) {
    for (size_t i = 0; i < contacts.names.size(); i++) {
        if (name == contacts.names[i]) {
            contacts.names.erase(contacts.names.begin() + i);
            contacts.numbers.erase(contacts.numbers.begin() + i);
            return true;
        }
    }
    return false;
}
/**
 * Sort the contact list in-place by name.
 */
void sort(storage& contacts){
    std::vector<size_t> indices(contacts.names.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(), [&contacts](size_t i1, size_t i2) {
            return contacts.names[i1] < contacts.names[i2];
    });
    std::vector<std::string> sorted_names;
    std::vector<number_t> sorted_numbers;
    for (size_t i : indices) {
        sorted_names.push_back(contacts.names[i]);
        sorted_numbers.push_back(contacts.numbers[i]);
    }
    contacts.names = std::move(sorted_names);
    contacts.numbers = std::move(sorted_numbers);
}


/**
 * Fetch a contact name from storage given a number.
 */
std::string get_name_by_number(storage& contacts, number_t number){
    for(size_t i = 0; i< contacts.names.size(); i++){
        if (number == contacts.numbers[i]){
            return contacts.names[i];
        }
    }
    return "";
}
}