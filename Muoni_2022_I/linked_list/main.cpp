#include <iostream>
/* A Node is a class that has a value and a pointer to the next Node */

class Node{
  private:
    int value;
    Node *next;

  public:
    Node(int value, Node *next = nullptr){
      this->value = value;
      this->next = next;
    }

    int getValue(){return this->value;}
    Node *getNext(){return this->next;}
    void setNext(Node *next){this->next = next;}
};

class LinkedList{
/*
 * It creates a linked list.
 */

  private:
    Node *head;
  public:
    //Constructor
    LinkedList(){
      this->head = nullptr;
    }
    //Destructor
    ~LinkedList(){
      while (this->head != nullptr) {
        pop();
      }
    }
    //Insertion operation
    friend std::ostream & operator <<(std::ostream &os, const LinkedList &rhs){
      for (Node *curNode = rhs.head; curNode != nullptr; curNode = curNode->getNext()) {
        os << curNode->getValue();
        if (curNode->getNext() != nullptr) os << " ";
      }
      return os;
  }

    //Add Links(push)
    void push(int value){
      this->head = new Node(value, this->head);
    }

    //Remove Links (pop)
    int pop(){
      int value;
      if (this->head != nullptr) {
        value = this->head->getValue();
        Node *temp = this->head;
        this->head = this->head->getNext();
        delete temp;
      }else{
        throw std::range_error("List is empty!");
      }
      return value;
    }
    //Final Lenght
    int lenght(){
      int count = 0;
      for (Node *curNode = this->head; curNode != nullptr; curNode = curNode->getNext()){
        count++;
      }
      return count;
    }
    //Find a value

};

int main(int argc, char const *argv[]) {
  LinkedList list;

  list.push(1);
  list.push(2);
  list.push(3);
  list.push(4);
  list.push(5);



  std::cout << "List after adding: " << list << std::endl;

  std::cout << "Lenght: " << list.lenght() << std::endl;

  return 0;
}
