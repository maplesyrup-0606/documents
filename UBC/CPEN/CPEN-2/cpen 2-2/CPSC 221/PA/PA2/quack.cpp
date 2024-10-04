/**
 * @file quack.cpp
 * Implementation of the Quack class.
 *
 */

template <class T>
Quack<T>::Quack() {
    n1=0;
    n2=-1;
}



// template<typename T>
// void show(T const & xs)
// {
//     std::cout << "[ ";
//     for(auto const & x : xs) {
//         std::cout << x << ' ';
//     }
//     std::cout << "]\n";
// }




/**
 * Adds the parameter object to the right of the Quack.
 *
 * @param newItem object to be added to the Quack.
 */
template <class T>
void Quack<T>::pushR(T newItem) {
    data.push_back(newItem);
    n2++;
}

/**
 * Removes the object at the left of the Quack, and returns it to the
 * caller.
 *
 * See .h file for implementation notes.
 *
 * @return The item that used to be at the left of the Quack.
 */

template <class T>
T Quack<T>::popL() {
    //show(data);
    T temp = data.at(n1);
    n1++;
    if(isEmpty()){
        n1 = 0;
        n2 = -1;
        data.resize(0);
        return temp;
    }
    else if(n1 >= n2-(n1-1) && n2-n1+1 > 0){
        vector<T> tempv;
        for(int x = n1 ; x<= n2; x++){
            //cout << x << endl;
            tempv.push_back(data.at(x));
            //cout << data.at(x) << endl;
        }
        data.resize(tempv.size());
        data = tempv;
        cout << data.size() << endl;
        n1 = 0;
        n2 = tempv.size()-1;
        return temp;
    }
    else{
        return temp;
    }
}

/**
 * Removes the object at the right of the Quack, and returns it to the
 * caller.
 *
 * @return The item that used to be at the right of the Quack.
 */
template <class T>
T Quack<T>::popR() {
    //show(data);
    T temp = data.at(n2);
    data.pop_back();
    n2--;
    if(isEmpty()){
        n1 = 0;
        n2 = -1;
        data.resize(0);
        return temp;
    }
    else if(n1 >= n2-(n1-1) && n2 - n1 + 1 > 0){
        vector<T> tempv;
        for(int x =n1 ; x<= n2; x++){
            tempv.push_back(data.at(x));
        }
        data.resize(tempv.size());
        data = tempv;
        n1=0;
        n2 = tempv.size()-1;
        return temp;
    }
    else{
        return temp;
    }
}

/**
 * Finds the object at the left of the Quack, and returns it to the
 * caller. Unlike popL(), this operation does not alter the quack.
 *
 * @return The item at the front of the quack.
 */
template <class T>
T Quack<T>::peekL() {
    return data.at(n1);
}

/**
 * Finds the object at the right of the Quack, and returns it to the
 * caller. Unlike popR(), this operation does not alter the quack.
 *
 * @return the value of The item at the right of the quack.
 */
template <class T>
T Quack<T>::peekR() {
    return data.at(n2);
}

/**
 * Determines if the Quack is empty.
 *
 * @return bool which is true if the Quack is empty, false otherwise.
 */
template <class T>
bool Quack<T>::isEmpty() const {
    if(n2-(n1-1)  == 0){
        return true;
    }
    return false;
    
}